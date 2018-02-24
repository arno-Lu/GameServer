//
// Created by Lu on 2018/1/18.
//

#include "AccountInformation.h"

#include <cstring>
#include <cstdio>
#include <map>
#include <set>


namespace LU{

    AccountInformation::AccountInformation() {};

    void AccountInformation::FdAdd(int fd) {

        if(m_connectFd.insert(fd).second){
            printf("ConnectFd %d Add success \n",fd);
        }else
            printf("ConnectFd %d Add fail \n",fd);
    }

    void AccountInformation::FdReduce(int fd) {

        if(m_connectFd.erase(fd)){
            printf("ConnectFd %d Reduce success \n",fd);
        }else{
            printf("ConnectFd %d Reduce fail \n",fd);
        }
    }


    set<int> AccountInformation::FdSet() {

        return m_connectFd;
    }

    bool AccountInformation::AccountAdd(string username, string password) {

       if( m_acount.insert(pair<string,string>(username,password)).second){

           printf("Account %s Add success \n",username.c_str());
           return true;
       }else {
           printf("Account %s Add fail \n", username.c_str());
           return false;

       }
    }

    bool AccountInformation::AccountCheck(string username, string passsword) {

        map<string, string>::iterator iter = m_acount.find(username);
        if(iter != m_acount.end()&& (iter->second == passsword)){
            return true;
        } else
            return false;
    }

    void AccountInformation::OnlineAdd(string username) {

        m_onlineUsers.insert(username);
    }

    void AccountInformation::OnlineReduce(string username) {

        if(!m_onlineUsers.erase(username)){
            printf("onlineUsers erase %s fail\n",username.c_str());
        }
    }
}