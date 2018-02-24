//
// Created by Lu on 2018/1/18.
//

#ifndef SERVER_LU_MESSAGEPROTOCOL_H
#define SERVER_LU_MESSAGEPROTOCOL_H


namespace LU{

    enum class EnumProtocol{

        LOGIN = 1,
        REGISTER = 2,
        CHAT = 3,
        MATCH = 4,
        START =5,
        ACTION = 6;


    };
}
#endif //SERVER_LU_MESSAGEPROTOCOL_H
