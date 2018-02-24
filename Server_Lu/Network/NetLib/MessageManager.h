//
// Created by Lu on 2018/1/18.
//

#ifndef SERVER_LU_MESSAGEMANAGER_H
#define SERVER_LU_MESSAGEMANAGER_H


#include "../Protocol/Resolver.h"

namespace LU {


    class MessageManager {

    public:
        MessageManager();
        void Receive();
        void SetSock(int );

    private:
        MessageManager(const MessageManager&) = delete;
        MessageManager& operator = (const MessageManager) = delete;


    private:

        int m_sock;

    };
}


#endif //SERVER_LU_MESSAGEMANAGER_H
