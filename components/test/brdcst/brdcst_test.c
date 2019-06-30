
#include <vtlsocket.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>


char *msg = "this is broadcast message";

int main(int argc, char **argv) {

    int fd;
    char *intf = "br0";
    char recv_msg[100];
    int len = 100;

    if (argc < 2) {
        printf("usage:exe [c|s]\n");
        return -1;
    }

    memset(recv_msg, 0, sizeof(recv_msg));

    if (vtl_create_broadcast_socket(&fd, intf)) {
        printf("unable to create socket on interface:%s\n", intf);
        return -1;
    }

    //if server then bind the socket to port for listening
    if (!strcmp(argv[1], "s")) {
        if (vtl_bind_broadcast_socket(2000, fd)) {
            printf("unable to bind server socket to 2000 port\n");
            return -1;
        }
    }

    //if client then send the broadcast message
    if (!strcmp(argv[1], "c")) {
        if (vtl_send_broadcast_msg(fd, 2000, msg, strlen(msg))){
            printf("unable to send broadcast message\n");
            return -1;
        }
        printf("sent broadcast message on port 2000\n");
        close(fd);
        return 0;
    }

    if (!strcmp(argv[1], "s")) {
        if (vtl_recv_broadcast_msg(fd, 2000, recv_msg, len)) {
            printf("unable to recv broadcast message\n");
            return -1;
        }
        printf("recved broadcast message:%s\n", recv_msg);
        close(fd);
        return 0;
    }

    return 0;
}
