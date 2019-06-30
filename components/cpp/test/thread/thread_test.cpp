#include <vtlthread.h>
#include <unistd.h>
#include <iostream>




void *thread_start(void *data) {
    int count = 4;

    do {
        printf("thread start %d\n", count);
    }while(--count > 0);

    printf("RTT thread quiting\n");
    return 0;
}



int main() {

    VtlThreadPtr_t thd;

    thd = VtlRTThread::getVtlRTThreadIns(thread_start, NULL, false);
    if (thd == NULL) {
        return -1;
    }

    printf("parent thread - RTT thread created\n");
    sleep(2);

    return 0;
}
