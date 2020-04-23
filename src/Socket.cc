#include <utility>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <linux/tcp.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <map>
#include <memory>
#include <vector>
#include <string.h>

#include "Log.h"
#include "Socket.h"

int SocketTCP() 
{
    int newSocket = ::socket(AF_INET, SOCK_STREAM, 0);
    if (newSocket == -1)
    ERROR("created a new socket = %d", newSocket);
    // struct linger tmp = {1, 0};
	// setsockopt(newSocket, SOL_SOCKET, SO_LINGER, &tmp, sizeof(tmp));
    int flag = 1;
    setsockopt(newSocket, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag));
    return newSocket;
}


int Bind(int sockfd, int port) 
{
	struct sockaddr_in ser;
	bzero(&ser, sizeof(ser));
	ser.sin_family = AF_INET;
	ser.sin_addr.s_addr = htonl(INADDR_ANY); 
	// inet_pton(AF_INET, ip, &ser.sin_addr);
	ser.sin_port = htons(port);
    
    int ret = ::bind(sockfd,
                     reinterpret_cast<sockaddr *> (&ser),
                     sizeof(ser));
    if (ret == -1) 
        perror("bind skcet failed");
}


int Listen(int sockfd, int backlog) 
{
    int ret = ::listen(sockfd, backlog);
    if (ret == -1)
        ERROR("listen failed");
}

int Accept(int listenfd, struct sockaddr_in& cli)
{
    socklen_t len = sizeof(cli);
    int ret = ::accept(listenfd, 
                      (struct sockaddr *)&cli, 
                      &len);
    if (ret > 0)
        return ret;
    else 
    {
        WARN("accept failed");
        return ret;
    }
}


int Connect() 
{
    errno = 0;
    // return ::connect(sockfd, 
    //                  peerAddr.sockAddr(),
    //                  peerAddr.length());
}

void setNonBlocking(int fd) 
{
    int old_option = fcntl(fd, F_GETFL, 0);
    int new_option = old_option | O_NONBLOCK;
    fcntl(fd, F_SETFL, new_option);
}


void setNoDely(int fd) 
{
    int optval = 1;
    int ret = ::setsockopt(fd,
                           IPPROTO_TCP,
                           TCP_NODELAY,
                           &optval,
                           static_cast<socklen_t> (sizeof(int)));
    if (ret == -1) 
    {
        ERROR("setNoDely failed ");
    }
}

void setReuseAddr(int fd) 
{
    int optval = 1;
    int ret = ::setsockopt(fd,
                           SOL_SOCKET,
                           SO_REUSEADDR,
                           &optval,
                           static_cast<socklen_t> (sizeof(optval)));
    if (ret == -1) 
    {
        ERROR("setReuseAddr falied ");
    }
}

void setKeepLive(int fd) 
{
    int optval = 1;
    int ret = ::setsockopt(fd,
                           SOL_SOCKET,
                           SO_KEEPALIVE,
                           &optval,
                           static_cast<socklen_t> (sizeof(optval)));
    if (ret == -1) 
    {
        ERROR("setKeepLive failed ");
    }
}

int getSocketState(int fd) 
{
    int optval;
    socklen_t optlen = static_cast<socklen_t> (sizeof(optval));
    int ret = ::getsockopt(fd,
                           SOL_SOCKET,
                           SO_ERROR,
                           &optval,
                           &optlen);
    if (ret < 0) 
        return errno;
    else 
        return optval;
}

bool isSelfConnection(int fd) 
{
    struct sockaddr_in local, peer;
    socklen_t len = sizeof(sockaddr_in);
    memset(&local, 0, len);
    memset(&peer, 0, len);

    getpeername(fd, 
                (struct sockaddr *)&peer,
                &len);
    
    getsockname(fd,
                (struct sockaddr *)&local,
                &len);
    return (peer.sin_port == local.sin_port) &&
        (peer.sin_addr.s_addr == local.sin_addr.s_addr);
}