//
// Created by Lu on 2018/1/18.
//

#ifndef SERVER_LU_ACCOUNTINFORMATION_H
#define SERVER_LU_ACCOUNTINFORMATION_H

#include <map>
#include <set>
#include <string>

using namespace std;

namespace LU {

    class AccountInformation {

    public:
        AccountInformation();
        ~AccountInformation();

        void FdAdd(int);
        void FdReduce(int);
        set<int> FdSet();
        bool AccountAdd(string ,string);
        bool AccountCheck(string ,string);
        void OnlineAdd(string);
        void OnlineReduce(string);

    private:
        AccountInformation(const AccountInformation&) = delete;
        AccountInformation& operator = (const AccountInformation&) = delete;

    private:
         set<int > m_connectFd;
         set<string> m_onlineUsers;
         map<string, string> m_acount;


    };
}

#endif //SERVER_LU_ACCOUNTINFORMATION_H
