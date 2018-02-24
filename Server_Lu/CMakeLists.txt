cmake_minimum_required(VERSION 3.9)
project(Server_Lu)

set(CMAKE_CXX_STANDARD 11)

include_directories(/Users/lu/CLionProjects/linu)

add_executable(Server_Lu main.cpp Network/NetLib/Epoll.cpp Network/NetLib/Epoll.h Network/NetLib/Server.cpp Network/NetLib/Server.h Network/NetLib/MessageManager.cpp Network/NetLib/MessageManager.h Network/Protocol/Resolver.cpp Network/Protocol/Resolver.h Network/Protocol/EnumProtocol.h Data/AccountInformation.cpp Data/AccountInformation.h Test/TestClient.cpp Test/TestClient.h Controll/GameManager.cpp Controll/GameManager.h Controll/MsgProcess.h)
