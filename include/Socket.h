#ifndef SOCKET_H_
#define SOCKET_H_

int SocketTCP();
int Bind(int sockfd, int port);
int Listen(int fd, int backlog);
int Accept(int listenfd, struct sockaddr_in& cli);
// int Connect(InitSockAddr peerAddr);
void setNonBlocking(int fd);
void setNoDely(int fd);
void setReuseAddr(int fd);
void setKeepLive(int fd);
int getSocketState(int fd);
bool isSelfConnection(int fd);


#endif