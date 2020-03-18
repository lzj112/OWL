#ifndef EPOLL_H_
#define EPOLL_H_

#include <unistd.h>
#include <sys/epoll.h>

#include <vector>
#include <functional>

enum EPOLLEVENTS
{
    pollPri = EPOLLPRI,
    pollErr = EPOLLERR,
    pollLevelTrigger = 0,
    pollHangUp = EPOLLHUP,
    pollReadAble = EPOLLIN,
    pollWriteAble = EPOLLOUT,
    pollEdgeTrigger = EPOLLET,
    pollRDHangUp = EPOLLRDHUP,
    pollOneShot = EPOLLONESHOT,
};

int addfd(int epollfd, int fd, bool isOneShot);
int delfd(int epollfd, int fd);
int ctlfd(int epollfd, int fd, int ev);
int Wait(int epollfd, 
          std::vector<epoll_event>& readyEvents, 
          int timeout = 0);


#endif