//
// Created by Lu on 2018/1/20.
//

#include "GameManager.h"

namespace LU{

    GameManager* GameManager:: instance = NULL;
    GameManager* GameManager::GetInstance() {

        if(instance == NULL){
            instance = new GameManager();
        }
        return instance;
    }

    GameManager::GameManager() {}


    void GameManager::playerAdd() {

        OnlineNum++;
    }

    void GameManager::playReduce() {

        OnlineNum--;
    }
    int GameManager::getRoomSize() {

        return roomNum;
    }


    bool GameManager::roomPlayerAdd(int room,int uin) {


        map<int,list<int>> :: iterator list = m_roomUins.find(room);
        if(list != m_roomUins.end()){
            list->second.push_back(uin);

            map<int,int> ::iterator iter = m_roomSize.find(room);
            if(iter != m_roomSize.end()){
                iter->second++;
            }

            m_uinsToroom.insert(pair<int,int>(uin,room));

            playerAdd();

            printf("uin %d Add success \n",uin);
            return true;
        }else {
            printf("uin %d Add fail \n", uin);
            return false;

        }
    }

    void GameManager::roomPlayerReduce(int room,int uin) {

        map<int,list<int>> :: iterator list = m_roomUins.find(room);
        if(list != m_roomUins.end()) {
            list->second.remove(uin);

            playReduce();
            m_uinsToroom.erase(uin);
        }


    }

    bool GameManager::isGameStart( int room) {

        map<int, int>::iterator iter = m_roomSize.find(room);
        if (iter != m_roomSize.end()) {

            if (iter->second == singleRoomSize) {

                m_roomToframe.insert(pair<int,int>(room,0));
                return true;
            }
        }
        return false;
    }

    int GameManager::getUinToRoom(int uin) {

        map<int,int> ::iterator iter = m_uinsToroom.find(uin);
        if(iter != m_uinsToroom.end()){

            return iter->second;
        }
    }

    list<int> GameManager::getRoomUins(int room) {

        map<int,list<int>> ::iterator iter = m_roomUins.find(room);
        if(iter != m_roomUins.end()){

            return iter->second;
        }
    }

    int GameManager::getRoomFrame(int room) {

        map<int,int>::iterator iter = m_roomToframe.find(room);
        if(iter != m_roomToframe.end()){

            return iter->second;
        }
    }

    void GameManager::updataFrame(int room) {

        map<int,int>::iterator iter = m_roomToframe.find(room);
        if(iter != m_roomToframe.end()){

           iter->second ++;

        }
    }



}
