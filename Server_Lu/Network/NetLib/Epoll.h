//
// Created by Lu on 2018/1/17.
//

#ifndef SERVER_LU_EPOLL_H
#define SERVER_LU_EPOLL_H


#include <sys/epoll.h>

namespace LU {

    class Epoll {

    public:
        Epoll(int);
        ~Epoll();
        int EpollWait();
        int EpollCtlAdd(int);
        int EpollCtlDel(int);
        epoll_event& GetEvent(int);

    private:
        Epoll(const Epoll&) = delete;
        Epoll& operator=(const Epoll&) = delete;

        int m_size;
        int m_epoll_fd;
        epoll_event* m_events;
    };

}

#endif //SERVER_LU_EPOLL_H
