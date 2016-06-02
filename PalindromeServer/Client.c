//
// Created by s-gheldd on 6/1/16.
//


#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <memory.h>
#include <arpa/inet.h>
#include <zconf.h>
#include "inet.h"

int main(int arc, char *argv[]) {
    int socket_fd, connection_fd;
    struct sockaddr_in server_addr;
    char message[256], reply[256];

    if (arc != 2) {
        printf("ERROR: address must be supplied as argument");
        exit(EXIT_FAILURE);
    }


    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd < 0) {
        perror("ERROR: client-socket creation");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sin_addr.s_addr = inet_addr(argv[1]);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PALINDROMESERVER_PORT_NUMBER);

    //Connect to remote server
    if (connect(socket_fd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
        perror("connect failed. Error");
        return 1;
    }

    puts("Connected\n");

    //keep communicating with server
    while (1) {
        bzero(message, 256);
        bzero(reply, 256);

        printf("Enter message : \n");
        scanf("%s", message);
        if (!strcmp(message, "quit")) {
            break;
        }

        //Send some data
        if (send(socket_fd, message, strlen(message), 0) < 0) {
            perror("Send failed");
            return 1;
        }

        if (!strcmp(message, "quit")) {
            break;
        }

        //Receive a reply from the server
        if (recv(socket_fd, reply, 255, 0) < 0) {
            perror("recv failed");
            break;
        }

        printf("%s\n", reply);
    }

    close(socket_fd);
    return 0;


}