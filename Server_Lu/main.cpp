#include <stdio.h>
#include <stdlib.h>
#include "Network/NetLib/Server.h"


int main(int argc,char** argv) {

    if (argc < 2)
    {
        printf("Input port!\n");
        return 0;
    }

    short port = atoi(argv[1]);
    LU::Server server(port);

    server.ServerLoop();

    return 0;
}