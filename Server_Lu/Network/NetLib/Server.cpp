//
// Created by Lu on 2018/1/18.
//

#include "Server.h"
#include "Epoll.h"

#include <cstdio>
#include <cstring>
#include <unistd.h>
#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <list>

#define LISTEN_SIZE 1024
#define MAX_EVENTS 1024
#define BUFF_SIZE 1024
#define MAX_CONNECT 1024


namespace LU{

    pthread_mutex_t  actionList_mutex;
    std :: list<CSFrameNotify>  actionList;

    Server::Server(int port) : m_port(port),m_epoll(LISTEN_SIZE){

        //socket
        m_listenfd = socket(AF_INET,SOCK_STREAM,0);
        if(-1 == m_listenfd){
            printf("Server socket() error!\n");
            return ;
        }

        //address
        struct sockaddr_in listen_addr;
        listen_addr.sin_family = AF_INET;
        listen_addr.sin_addr.s_addr = htonl((INADDR_ANY));
        listen_addr.sin_port = htons(port);

        //set REUSADDR
        int RecvBuf = 32* 1024;
        if(setsockopt(m_listenfd,SOL_SOCKET,SO_REUSEADDR,&RecvBuf, sizeof(RecvBuf))){
            printf("Set REUSEADDR error\n");
            return ;
        }

        //bind
        if (bind(m_listenfd, (struct sockaddr *) &listen_addr, sizeof(listen_addr)) == -1)
        {
            printf("Server bind() error!\n");
            return;
        }

        //listen
        listen(m_listenfd,MAX_CONNECT);

        if(-1 == m_epoll.EpollCtlAdd(m_listenfd)){

            printf("Server epoll ctl error\n");
            return ;
        }

        printf("Server start listen in port: %d \n ",port);

    }

    Server::~Server() {
        close(m_listenfd);
    }

    void Server::ServerLoop() {

        printf("start MsgSendThread \n");

        pthread_t pid;
        if(pthread_create(&pid,NULL,msgSendThread,NULL)!=0){
            printf("msgSendThread error\n");
        }
        pthread_detach(pid);


        printf("start Server Loop \n");
        while(true){

            int len;
            int num = m_epoll.EpollWait();
            for(int i=0;i<num;i++){

                epoll_event event = m_epoll.GetEvent(i);
                int eventEvents = event.events;
                int eventFd = event.data.fd;

                if((eventEvents & EPOLLERR)||(eventEvents & EPOLLHUP) || !(eventEvents & EPOLLIN)) {

                    printf("clientFd : %d error %d\n", eventFd, eventEvents);
                    DeleteClient(eventFd);
                    continue;
                } else if(eventFd == m_listenfd){

                    struct sockaddr client_addr;
                    int client_addr_len = sizeof(client_addr);
                    int newClient = accept(m_listenfd,&client_addr, (socklen_t *)&client_addr_len);
                    if(newClient <0){
                        printf("accept error \n");
                        continue;
                    }
                    printf("A new client : %d\n",newClient);
                    m_epoll.EpollCtlAdd(newClient);
                    Resolver::GetInstance()->GetaccountInformation()->FdAdd(newClient);
                } else{

                    //由MessageManage解析消息，Resolver负责消息处理逻辑和Response
                    m_messageManager.SetSock(eventFd);
                    m_messageManager.Receive();
                }

            }

        }
    }

    void Server::DeleteClient(int sock) {

        printf("CLient : %d close the connect \n",sock);
        m_epoll.EpollCtlDel(sock);
        close(sock);
        m_connectSet.erase(sock);
    }


    void* Server:: msgSendThread(void *ptr) {


        CSFrameNotify actionMsg;
        SCFrameNotify responseMsg ;
        GameManager * manager = GameManager::GetInstance();
        Resolver * resolver = Resolver::GetInstance();
        while (true){
            usleep(50000);

            pthread_mutex_lock(&actionList_mutex);
            if(actionList.size() == 0){

                pthread_mutex_unlock(&actionList_mutex);
            }else{

                int room = manager->getUinToRoom(actionMsg.uin());
                int frame = manager->getRoomFrame(room);
                manager->updataFrame(room);
                responseMsg.set_currentframe(frame);
                responseMsg.set_nextframe(frame+1);

                while(!actionList.empty()){

                    actionMsg = actionList.front();
                    actionList.pop_front();

                    CSFrameNotify *action = responseMsg.add_keys();
                    action->CopyFrom(actionMsg);

                }


                list<int> uins = manager->getRoomUins(room);
                for(int uin : uins){

                    resolver->Send(uin,responseMsg);

                }
                pthread_mutex_unlock(&actionList_mutex);

            }

        }

        printf("msgSendThread stop!! \n");
        pthread_exit(NULL);
    }

    void Server::addMsg(CSFrameNotify actionMsg) {

        pthread_mutex_lock(&actionList_mutex);
        actionList.push_back(actionMsg);
        pthread_mutex_unlock(&actionList_mutex);
        printf("actionList add a actionMsg ,uin: %d\n",actionMsg.uin());
    }
}
