//
// Created by Lu on 2018/1/18.
//

#include "MessageManager.h"
#include "Server.h"
#include <unistd.h>


#include <arpa/inet.h>


#define LOGIN 1
#define REGISTER  2
#define CHAT 3
#define MATCH 4
#define START 5
#define ACTION 6

namespace LU{

    MessageManager:: MessageManager( ) {}

    void MessageManager ::Receive() {

         Resolver* m_resolver = Resolver::GetInstance();

        //读长度
        int len,ret;
       if( -1 == (ret = m_resolver->ReadN(m_sock,&len,4))){

            perror("MessageMessage::Receive");
            exit(EXIT_FAILURE);
        }
        len = ntohl(len);
        if(ret < 4 || len < 4){
            printf("<MessageMessage::Receive error!\n");
            return;
        }
        //读类型
        int protocol;
        if(-1 == (ret = m_resolver->ReadN(m_sock,&protocol,4))){

            perror("MessageMessage::Receive");
            exit(EXIT_FAILURE);
        } else if(ret <4){
            printf("MessageMessage::Receive error!\n");
            return;
        }
        //读数据
        protocol = ntohl(protocol);
        char * recvBuf = new char[len -4];
        memset(recvBuf,0,len -4);
        if(-1 == (ret = m_resolver->ReadN(m_sock,recvBuf,len-4))){
            perror("MessageMessage::Receive");
            exit(EXIT_FAILURE);
        }else if(ret < len -4){
            printf("MessageMessage::Receive error!\n");
            return;
        }else{
            /* 成功读取信息，根据protobuf协议序列化对应数据包，再调用对应ResponseMessage方法；
             * protocol ： 1 login
             *             2 register
             *             3 chat　
             */
            printf("Receive client : %d  a new message!\n", m_sock);

            switch (protocol){

                case LOGIN:{

                    LoginMSG loginMSG;
                    loginMSG.ParseFromArray(recvBuf,len-4);
                    m_resolver->ResponseMessage(m_sock,loginMSG);
                    break;
                }

                case REGISTER:{

                    RegisterMSG registerMSG;
                    registerMSG.ParseFromArray(recvBuf,len-4);
                    m_resolver->ResponseMessage(m_sock,registerMSG);
                    break;
                }

                case CHAT:{

                    ChatMSG chatMSG;
                    chatMSG.ParseFromArray(recvBuf,len-4);
                    m_resolver->ResponseMessage(m_sock,chatMSG);
                    break;
                }

                case MATCH:{

                    MatchMSG matchMSG;
                    matchMSG.ParseFromArray(recvBuf,len-4);
                    m_resolver->ResponseMessage(m_sock,matchMSG);
                }

                case ACTION:{

                    CSFrameNotify actionMsg;
                    actionMsg.ParseFromArray(recvBuf,len-4);
                    Server::addMsg(actionMsg);


                }
            }
        }
        delete[] recvBuf;

    }

    void MessageManager::SetSock(int fd){
        m_sock = fd;
    }


}

