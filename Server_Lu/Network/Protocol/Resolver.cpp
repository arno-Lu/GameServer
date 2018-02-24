//
// Created by Lu on 2018/1/18.
//

#include "Resolver.h"

#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>



#define LOGIN 1
#define REGISTER  2
#define CHAT 3
#define MATCH 4
#define START 5
#define ACTION 6

namespace LU{


    Resolver* Resolver::instance = NULL;
    Resolver* Resolver::GetInstance() {

        if(instance == NULL){
            instance = new Resolver();
        }
        return instance;
    }

    Resolver::Resolver() {
        accountInformation = new AccountInformation();
    }

    AccountInformation* Resolver::GetaccountInformation(){
        return accountInformation;
    }


    void Resolver::ResponseMessage(int sock, LoginMSG loginMSG) {

        printf("Receive client :%d a new Login message\n",sock);
        printf("LoginMSG : username = %s  password = %s\n",loginMSG.username().c_str(),loginMSG.password().c_str());
        LoginRSPS loginRSPS;
        loginRSPS.set_type(1);
        if(accountInformation->AccountCheck(loginMSG.username(),loginMSG.password())){

            loginRSPS.set_message("success");
            printf("Response success\n");
        }else{
            loginRSPS.set_message("false");
            printf("Response false\n");

        }
        Send(sock,loginRSPS);
    }


    void Resolver::ResponseMessage(int sock, RegisterMSG registerMSG) {
        printf("Receive client :%d a new Register message\n",sock);
        printf("Register : username = %s  password = %s\n",registerMSG.username().c_str(),registerMSG.password().c_str());
        RegisterRSPS registerRSPS;
        registerRSPS.set_type(2);
        if(accountInformation->AccountAdd(registerMSG.username(),registerMSG.password())){

            registerRSPS.set_message("success");
            printf("Response success\n");
        }else{
            registerRSPS.set_message("false");
            printf("Response false\n");

        Send(sock,registerRSPS);
    }

    void Resolver::ResponseMessage(int sock, ChatMSG chatMSG) {
        printf("Receive client :%d a new Chat message\n",sock);
        printf("Chat : username = %d chatMessage = %s\n",chatMSG.uin(),chatMSG.message().c_str());
        ChatRSPS chatRSPS;
        chatRSPS.set_uin(chatMSG.uin());
        chatRSPS.set_message(chatMSG.message());

        GameManager * manager = GameManager::GetInstance();
        int roomNum = manager->getUinToRoom(chatRSPS.uin());

        list<int> uins = manager->getRoomUins(roomNum);
        for(int uin : uins){
          Send(uin,chatRSPS);
        }

    }

    void Resolver::ResponseMessage(int sock, MatchMSG matchMSG) {
        printf("Receive client :%d a new Match message\n",sock);
        printf("player role : %d\n",matchMSG.role());

        MatchRSPS matchRSPS;
        matchRSPS.set_uin(sock);

        Send(sock,matchRSPS);

        GameManager * manager = GameManager::GetInstance();

        int roomNum = manager->getRoomSize();
        manager->roomPlayerAdd(roomNum,matchRSPS.uin());

        SCStartGame scStartGame;
        if(manager->isGameStart(roomNum)){

            list<int> uins = manager->getRoomUins(roomNum);
            for(int uin : uins){
                scStartGame.add_uins(uin);
            }
            for(int uin : uins){
                Send(uin,scStartGame);
            }
        }

    }



    void Resolver::Send(int sock,LoginRSPS loginRSPS) {

        size_t  size = loginRSPS.ByteSizeLong();
        void *buffer = malloc(size);
        loginRSPS.SerializeToArray(buffer,size);
        Send(sock,LOGIN,buffer,size);
    }

    void Resolver::Send(int sock,RegisterRSPS registerRSPS) {

        size_t size = registerRSPS.ByteSizeLong();
        void *buffer = malloc(size);
        registerRSPS.SerializeToArray(buffer,size);
        Send(sock,REGISTER,buffer,size);
    }

    void Resolver::Send(int sock,ChatRSPS chatRSPS) {

        size_t size = chatRSPS.ByteSizeLong();
        void *buffer = malloc(size);
        chatRSPS.SerializeToArray(buffer,size);
        Send(sock,CHAT,buffer,size);
    }

    void Resolver::Send(int sock,MatchRSPS matchRSPS){

        size_t size = matchRSPS.ByteSizeLong();
        void *buffer = malloc(size);
        matchRSPS.SerializeToArray(buffer,size);
        Send(sock,MATCH,buffer,size);
    }

    void Resolver::Send(int sock, SCStartGame startGame) {

        size_t size = startGame.ByteSizeLong();
        void * buffer = malloc(size);
        startGame.SerializeToArray(buffer,size);
        Send(sock,START,buffer,size);
    }
    void Resolver::Send(int sock, SCFrameNotify actionMSg) {

        size_t size = actionMSg.ByteSizeLong();
        void * buffer = malloc(size);
        actionMSg.SerializeToArray(buffer,size);
        Send(sock,ACTION,buffer,size);
    }



    void Resolver::Send(int sock,int ptrc, void * msg, int size) {

        printf("send a message to client : %d   protocol : %d  size : %d\n",sock,ptrc,size);

        int len = htonl(4+size);
        WriteN(sock,&len,4);
        ptrc = htonl(ptrc);
        WriteN(sock,&ptrc,4);
        int wm = WriteN(sock,msg,size);
        printf("WriteN : %d ,size : %d\n",wm,size);
        printf("send finished\n\n");

    }

    int Resolver::WriteN(int sock,const void * buf, size_t count) {

        size_t residue = count;
        int nwrite;
        char *recvBuf = (char *)buf;
        while(residue >0){

            if((nwrite = write(sock,recvBuf,residue)) < 0){

                if(EINTR == errno)
                    continue;
                return -1;
            }else if(0 == nwrite)
                continue;

            recvBuf += nwrite;
            residue -= nwrite;
        }

        return count;

    }

    int Resolver::ReadN(int sock,void * buf, size_t count) {

        size_t residue = count;
        int nread;
        char *sendBuf = (char *) buf;
        while(residue > 0){

            if((nread = read(sock,sendBuf,residue)) < 0){

                if(EINTR == errno)
                    continue;
                return -1;
            }else if(0==nread)
                continue;
            sendBuf += nread;
            residue -= nread;
        }

        return count;
    }
}


