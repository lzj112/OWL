#include <signal.h>

#include <sys/epoll.h>
#include <sys/socket.h>

#include "Log.h"
#include "Locker.h"
#include "Socket.h"
#include "HttpConn.h"
#include "ThreadPool.h"

#define MAX_FD 65536
#define MAX_EVENT_NUMBER 10000

int port = 8000;

void addsig(int sig, void(handler)(int), bool restart = true)
{
	struct sigaction sa;
	memset(&sa, '\0', sizeof(sa));
	sa.sa_handler = handler;
	if (restart)
	{
		sa.sa_flags |= SA_RESTART;
	}
	sigfillset(&sa.sa_mask);
	assert(sigaction(sig, &sa, NULL) != -1);
}

void show_error(int connfd, const char *text)
{
	ERROR("%s\n", text);
	send(connfd, text, strlen(text), 0);
	close(connfd);
}

int main(int argc, char* arhv[])
{

	//忽略SIGPIPE的信号
	addsig(SIGPIPE, SIG_IGN);

	//创建线程池
	threadpool<http_conn> *pool = NULL;
	try
	{
		pool = new threadpool<http_conn>;
	}
	catch (...)
	{
		return 1;
	}
	//预先为每个可能的客户连接分配一个http_conn对象
	http_conn *user = new http_conn[MAX_FD];
	assert(user);
	int user_count = 0;

	int listenfd = SocketTCP();
	Bind(listenfd, port);
	Listen(listenfd, 5);
	
	std::vector<epoll_event> events(MAX_EVENT_NUMBER);
	int epollfd = epoll_create(5);
	assert(epollfd != -1);
	addfd(epollfd, listenfd, false);
	http_conn::m_epollfd = epollfd;

	while (true)
	{
		int num = Wait(epollfd, events, -1);
		if (num < 0 && errno != EINTR)
		{
			WARN("epoll_wait fail\n");
			break;
		}
		for (int i = 0; i < num; i++)
		{
			int sockfd = events[i].data.fd;
			if (sockfd == listenfd)
			{
				struct sockaddr_in cli;
				int connfd = Accept(listenfd, cli);
				if (http_conn::m_user_count >= MAX_FD)
				{
					show_error(connfd, "internet busy");
					continue;
				}
				if (connfd > 0)
				{
					// INFO("new connection\n");
					//初始化客户连接,添加到用户数组
					user[connfd].init(connfd, cli);
				}
			}
			else if (events[i].events & (pollRDHangUp | pollHangUp | pollErr))
			{
				// WARN("异常关闭\n");
				//如果有异常,直接关闭客户连接
				user[sockfd].close_conn();
			}
			else if (events[i].events & EPOLLIN)
			{
				// INFO("epoll开始读事件\n");
				//根据读的结果,决定是否将任务添加到线程池,还是关闭连接
				//添加进线程池任务队列的是对应连接对象的指针
				if (user[sockfd].read())
				{
					// INFO("添加进任务队列\n");
					pool->append(user + sockfd);//sockfd同时是下标,这里就是计算sockfd个偏移
				}
				else
				{
					// INFO("根据read()返回结果关闭连接\n");
					user[sockfd].close_conn();
				}
			}
			else if (events[i].events & EPOLLOUT)
			{
				// INFO("epoll开始写事件\n");
				//根据写的结果,决定是否关闭连接
				if (!user[sockfd].write())
					user[sockfd].close_conn();
			}
			else
			{}
		}
	}
	close(epollfd);
	close(listenfd);
	delete[] user;
	delete[] pool;
	return 0;
}

