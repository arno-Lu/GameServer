//
// Created by Lu on 2018/1/20.
//

#ifndef SERVER_LU_GAMEMANAGER_H
#define SERVER_LU_GAMEMANAGER_H

#include <pthread.h>
#include <cstdio>
#include <cstring>
#include <map>
#include <list>

using namespace std;
namespace LU {

    class GameManager {

    public:
        static GameManager* GetInstance();
        ~GameManager();
        void playerAdd();
        void playReduce();
        bool roomPlayerAdd(int ,int );
        void roomPlayerReduce(int ,int );
        int getUinToRoom(int);
        int getRoomSize();
        bool isGameStart(int );
        list<int> getRoomUins(int );
        int getRoomFrame(int);
        void updataFrame(int);

    private:
        GameManager();
        GameManager(const GameManager&) = delete;
        GameManager& operator = (const GameManager&) = delete;

    private:

        const int singleRoomSize = 2;
        int OnlineNum = 0;
        int roomNum = 0;
        map<int,list<int>> m_roomUins;
        map<int,int> m_roomSize;
        map<int,int> m_uinsToroom;
        map<int,int> m_roomToframe;


        static GameManager* instance;



    };

}
#endif //SERVER_LU_GAMEMANAGER_H
