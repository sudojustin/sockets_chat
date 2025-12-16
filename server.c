#include <asm-generic/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

#define PORT 8000

int main() {
    int server_fd, new_socket;
    ssize_t valread;
    struct sockaddr_in address;
    int opt = 1;
    socklen_t addrlen = sizeof(address);
    char buffer[1024] = { 0 };
    char *hello = "Hello from server";

    printf("Creating socket\n");

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        printf("Socket not created...\n");
        exit(EXIT_FAILURE);
    }

    int setsock_return_status = setsockopt(
        server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)
    );
    if (setsock_return_status == -1) {
        printf("Setsockopt error\n");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    int bind_status = bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    if (bind_status == -1) {
        printf("Bind error\n");
        exit(EXIT_FAILURE);
    }

    int listen_status = listen(server_fd, 3);
    if (listen_status == -1) {
        printf("Listen error\n");
        exit(EXIT_FAILURE);
    }

    int accept_status = accept(server_fd, (struct sockaddr*)&address, &addrlen);
    if (accept_status == -1) {
        printf("Accept error\n");
        exit(EXIT_FAILURE);
    }

    valread = read(new_socket, buffer, 1024 - 1);
    printf("%s\n", buffer);
    send(new_socket, hello, strlen(hello), 0);
    printf("Hello message sent\n");

    close(new_socket);
    close(server_fd);

    return 0;
}
