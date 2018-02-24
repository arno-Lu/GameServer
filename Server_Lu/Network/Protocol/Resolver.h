//
// Created by Lu on 2018/1/18.
//

#ifndef SERVER_LU_RESOLVER_H
#define SERVER_LU_RESOLVER_H

#include "ServerMessageProto.pb.h"
#include "../../Data/AccountInformation.h"
#include "../../Controll/GameManager.h"
#include <list>
#include <map>


/*
 * 单例模式工具类
 * 处理数据逻辑以及回复逻辑
 *
 */

namespace LU {

    class Resolver {

    public:
        static Resolver* GetInstance();
        ~Resolver();
        void ResponseMessage(int, LoginMSG);
        void ResponseMessage(int, RegisterMSG);
        void ResponseMessage(int, ChatMSG);
        void ResponseMessage(int, MatchMSG);
        void Send(int,SCFrameNotify);
        int ReadN(int ,void*, size_t);
        int WriteN(int ,const void*, size_t);
        AccountInformation* GetaccountInformation();

    private:
        Resolver() ;
        Resolver(const Resolver&) = delete;
        Resolver&operator = (const Resolver&) = delete;

        void Send(int,LoginRSPS);
        void Send(int,RegisterRSPS);
        void Send(int,ChatRSPS);
        void Send(int,MatchRSPS);
        void Send(int,SCStartGame);
        void Send(int,int , void*, int);


    private:

        AccountInformation* accountInformation;

        static Resolver* instance;





    };
}


#endif //SERVER_LU_RESOLVER_H
