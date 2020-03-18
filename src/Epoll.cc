#include <cstdio>
#include <errno.h>
#include <cstdlib>
#include <stdio.h>

#include <iostream>

#include "Log.h"
#include "Epoll.h"
#include "Socket.h"

int addfd(int epollfd, int fd, bool isOneShot) 
{
    epoll_event event;
	event.data.fd = fd;
	event.events = pollReadAble | pollEdgeTrigger | pollRDHangUp;
	if (isOneShot)
		event.events |= pollOneShot;
    setNonBlocking(fd);
    return epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event);
}

int delfd(int epollfd, int fd) 
{
    //防止还有引用技术，之后再进行一次EPOLL_CTL_DEL
    close(fd);
    return epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, 0);
}

int ctlfd(int epollfd, int fd, int ev) 
{
    epoll_event event;
	event.data.fd = fd;
	event.events = ev | pollEdgeTrigger | pollOneShot | pollRDHangUp;

    return epoll_ctl(epollfd, EPOLL_CTL_MOD, fd, &event);
}

int Wait(int epollfd, 
         std::vector<epoll_event>& readyEvents, 
         int timeout) 
{
    int eventsNumber = epoll_wait(epollfd,
                                  readyEvents.data(),
                                  static_cast<int> (readyEvents.size()),
                                  timeout);
    //有就绪事件
    if (eventsNumber >= 0) 
    {
        //返回的事件数量等于容器容量,扩大容量
        if (eventsNumber == readyEvents.size()) 
        {
            readyEvents.resize(eventsNumber * 2);
        }
        return eventsNumber;
    }
    else 
    {
        if (errno == EINTR) //被信号中断
        {
            return 0;
        }
        if (errno == EBADF ||   /*epfd不是一个有效文件描述符*/
            errno == EFAULT ||  /*对于events没有写全权限*/
            errno == EINVAL)    /*epfd不是一个epoll文件描述符或maxevnts<=0*/
        {
            //错误处理 TODO
            ERROR("被信号中断:");
            INFO("epfd = %d, size=%d, timeout=%d", epollfd, 
                                                   static_cast<int> (readyEvents.size()), 
                                                   timeout);
            exit(1);    //退出 FIXME
        }
    }
}