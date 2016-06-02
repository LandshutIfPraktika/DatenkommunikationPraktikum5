//
// Created by s-gheldd on 01.06.16.
//

#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <netinet/in.h>
#include <unistd.h>
#include "inet.h"

static void reverse(char *string);

int main(void) {
    int socket_fd, incoming_fd, port_number, client_add;
    struct sockaddr_in socket_adr, client_adr;
    socklen_t client_len;

    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd < 0) {
        perror("Socket creation");
        exit(EXIT_FAILURE);
    }

    port_number = PALINDROMESERVER_PORT_NUMBER;
    memset(&socket_adr, 0, sizeof(socket_adr));
    socket_adr.sin_family = AF_INET;
    socket_adr.sin_addr.s_addr = INADDR_ANY;
    socket_adr.sin_port = htons(port_number);

    if (bind(socket_fd, (struct sockaddr *) &socket_adr, sizeof(socket_adr)) < 0) {
        perror("Socket binding");
        exit(EXIT_FAILURE);
    }
    if (listen(socket_fd, 1) < 0) {
        perror("Could not call listen");
        exit(EXIT_FAILURE);
    }

    client_len = sizeof(client_adr);
    while (1) {
        pid_t pid;

        incoming_fd = accept(socket_fd, (struct sockaddr *) &client_adr, &client_len);
        if (incoming_fd < 0) {
            perror("Socket accept");
            exit(EXIT_FAILURE);
        }

        pid = fork();
        if (pid < 0) {
            perror("ERROR: fork failed");
            exit(EXIT_FAILURE);
        }

        if (!pid) {
            char buffer[256];
            int n;


            if (close(socket_fd) < 0) {
                perror("close server socket in child failed");
                exit(EXIT_FAILURE);
            }

            while (1) {
                bzero(buffer, 256);

                n = recv(incoming_fd, buffer, 255, 0);
                if (n < 0) {
                    perror("recieve failed");
                    exit(EXIT_FAILURE);
                }
                if (!strcmp(buffer, "quit")) {
                    break;
                }
                reverse(buffer);
                n = send(incoming_fd, buffer, strlen(buffer), 0);
                if (n < 0) {
                    perror("send failed");
                    exit(EXIT_FAILURE);
                }
            }
            if (close(incoming_fd) < 0) {
                perror("close incoming socket in child failed");
                exit(EXIT_FAILURE);
            }
            exit(0);
        }

        if (close(incoming_fd) < 0) {
            perror("close incoming socket in parent failed");
            exit(EXIT_FAILURE);
        }
        break;
    }

    if (
            close(socket_fd)
            < 0) {
        perror("close server socket in parent failed");
        exit(EXIT_FAILURE);
    }
}

static void reverse(char *string) {
    int len = strlen(string), i, j;
    char tmp[len];
    strcpy(tmp, string);

    for (i = 0, j = len - 1; i < len; ++i) {
        string[i] = tmp[j];
        j--;
    }
}