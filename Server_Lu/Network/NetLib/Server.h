//
// Created by Lu on 2018/1/18.
//

#ifndef SERVER_LU_SERVER_H
#define SERVER_LU_SERVER_H


#include <set>
#include "Epoll.h"
#include "MessageManager.h"
#include <pthread.h>

namespace LU {

    class Server {

    public:
        Server(int);
        ~Server();
        void ServerLoop();
        void DeleteClient(int);
        void static addMsg(CSFrameNotify );
    private:
        Server(const Server&) = delete;
        Server& operator = (const Server&) = delete;

    private:
        int m_port;
        int m_listenfd;
        Epoll m_epoll;

        std::set<int> m_connectSet;

        MessageManager m_messageManager;

        static void* msgSendThread(void *ptr);

        bool RunFlag = false;


    };
}


#endif //SERVER_LU_SERVER_H
