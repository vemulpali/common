#include <select.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>


// this local filepath is used for testing
const char *path_name = "localsocket";

static int select_call_back(int fd, void *data) {

    char buff[30]; //max 20 bytes from client
    static int count = 1;
    int ret = 0;

    //read the data and display
    printf("select_call_back called\n");

    ret = read(fd, buff, 30);
    if (!ret) {
        perror("select_call_back:");
        printf("select_call_back: no data from buffer\n");
        return 0;
    }

    buff[29] = 0;

    printf("data read:%s\n", buff);

    count++;

    if (count > 3) {
        count = 0;
        return SELECT_RET;
    }

    return 0;
}

static const int max_fds = 4;
int main(int argc, char **argv) {

    int fd_array[max_fds];
    int fd = 0;
    struct sockaddr_un addr;

    // open the fds and pass to select call
    fd = socket(AF_LOCAL, SOCK_DGRAM, 0);
    if (fd <= 0) {
        printf("socket open failed\n");
        return -1;
    }

    unlink(path_name);

    addr.sun_family = AF_LOCAL;
    strncpy(addr.sun_path, path_name, sizeof(addr.sun_path) - 1);
    addr.sun_path[sizeof(addr.sun_path) - 1] = 0;

    if (bind(fd, (struct sockaddr *)&addr, sizeof(addr)) < 0 ) {
        perror("select_call_back:");
        printf("bind the socket to address\n");
        close(fd);
        return -1;
    }
    
    fd_array[0] = fd;
    vtl_read_select(fd_array, 1, select_call_back, NULL); 

    close(fd);
    unlink(path_name);
    return 0;
}


