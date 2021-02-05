#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

#define PORT 7890
#define BUFFER_SIZE 1024
#define ADDR_SIZE 20

int main(void)
{
    int fd, client_fd;
    struct sockaddr_in client_addr, server_addr;
    socklen_t slen = sizeof(client_addr);
    char buffer[BUFFER_SIZE];
    char client_addr_str[ADDR_SIZE];
    
    fd = socket(AF_INET, SOCK_STREAM, 0);

    if (fd == -1) {
        perror("Socket error");
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if ((bind(fd, (struct sockaddr *)&server_addr, sizeof(server_addr))) == -1) {
        perror("Bind error");
    }

    listen(fd, 5);
    printf("Listening on port %d...\n", PORT);
    client_fd = accept(fd, (struct sockaddr *)&client_addr, &slen);
    strcpy(client_addr_str, (char *)inet_ntoa((struct in_addr)client_addr.sin_addr));
    printf("Connected with %s.\n", client_addr_str);
    
    while(1) {
        memset(buffer, 0, BUFFER_SIZE);
        if ((read(client_fd, buffer, sizeof(buffer))) == 0) {
            close(client_fd);
            printf("Disconnected.\n");
            exit(1);
        }
        printf("%s", buffer);
    }

    return 0;
}