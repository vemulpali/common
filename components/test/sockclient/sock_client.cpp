#include <sys/socket.h>
#include <sys/un.h>
#include <sys/types.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

const char *path_name = "localsocket";

#define MAX_STR_LEN   30
const char *data = "this is client send msg";
                    

int main(int argc, char **argv) {

    struct sockaddr_un server_addr;
    int fd  = -1;
    int count = 1;

    if (argc > 1) {
        count = atoi(argv[1]);
    }

    fd = socket(AF_LOCAL, SOCK_DGRAM, 0);
    if (fd <= 0) {
        printf("cannot open the socket\n");
        return -1;
    } 

    server_addr.sun_family = AF_LOCAL;
    strncpy(server_addr.sun_path, path_name, sizeof(server_addr.sun_path) - 1);
    server_addr.sun_path[sizeof(server_addr.sun_path) - 1] = 0;

    if (connect(fd, 
               (struct sockaddr *)&server_addr, 
               sizeof(struct sockaddr_un)) < 0) {
        printf("cannot connect socket to server\n");
        close(fd);
        return -1;
    }

    do {
        if (write(fd, data, strlen(data)) < 0 ) {
            perror("sock write:");
            close(fd);
            return -1;
        }
    } while(--count);

    close(fd);
}
