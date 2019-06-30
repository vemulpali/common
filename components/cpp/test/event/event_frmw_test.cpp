#include <vtlio.h>
#include <string.h>
#include <memory>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>


#define SOCK_NAME_SZ  30

class SocketTest : public Socket {
public:
    ~SocketTest(){ close(); }

    std::unique_ptr<Msg> recv() {
        int size = 20;
        std::unique_ptr<Msg> msg = Msg::getMsgInstanceCreate(size, *this);
        if (msg == NULL) {
            printf("Error: recv() cannot allocate message buffer\n");
            return NULL;
        }
    
        char *buff;
        size = msg->getMsgBuff((void**)&buff);
        int num = read(mFd, buff, size);
        if (num <= 0) {
            perror("read:");
            msg.reset();
            return NULL;
        }
        buff[num - 1] = 0;
        printf("recv - byte:%d message read:%s\n", num, buff);
        return std::move(msg); 
    }

    static std::unique_ptr<SocketTest> getSocketInstance(const char *nameSpace) {
        std::unique_ptr<SocketTest> sock(new SocketTest(nameSpace));
        if (sock == NULL) {
            return NULL;
        }
        if (sock->open()) {
            sock.reset();
            return NULL;
        }
        return std::move(sock);
    }

    int send(void *msg, int size) { return 0; }

private:
    int open() {
        int fd = 0;
        struct sockaddr_un addr;
        addr.sun_family = AF_LOCAL;
        unlink(mSockNameSpace);
        memcpy(addr.sun_path, mSockNameSpace, sizeof(addr.sun_path) - 1);
        addr.sun_path[sizeof(addr.sun_path) - 1] = 0;

        fd = socket(AF_LOCAL, SOCK_DGRAM, 0); 
        if (fd <= 0) {
            printf("socket opened failed\n");
            return -1;
        }

        if (bind(fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
            perror("bind:");
            close();
            return -1;
        }

        mFd = fd;
        return 0;
    }

    void close() { if (mFd != -1) {
                    unlink(mSockNameSpace);
                    ::close(mFd);
                   }
                 }

    SocketTest(const char *nameSpace){ 
        strncpy(mSockNameSpace, nameSpace, SOCK_NAME_SZ);
    }

    char mSockNameSpace[SOCK_NAME_SZ];
};


class EventMsgProcessTest : public EventMsgProcess {
public:
    ~EventMsgProcessTest(){}

    EventProcessRet_t processEventMsg(std::unique_ptr<Msg> msg) {
        if (msg == NULL) {
            return MSG_P_FAIL;
        }
        char *buff;
        msg->getMsgBuff((void**)&buff);
        printf("the message:%s\n", buff);
        return MSG_P_SUCCESS;
    }

    static std::unique_ptr<EventMsgProcessTest> getEventMsgProcessInstance() {
        std::unique_ptr<EventMsgProcessTest> ins(new EventMsgProcessTest());
        if (ins == NULL) {
            return NULL;
        }
        return std::move(ins);
    }

private:
    EventMsgProcessTest(){}
};


int main() {

    Socket_t sock = SocketTest::getSocketInstance("localsocket");
    if (sock == NULL) {
        printf("Error: unable to create socket\n");
        return -1;
    }

    EventMsgProcess_t event = EventMsgProcessTest::getEventMsgProcessInstance();

    if (event == NULL) {
        printf("Error: unable to create event ins\n");
        return -1;
    }

    std::unique_ptr<EventHandler> handler = EventHandler::getEventHandlerIns();
    if (handler == NULL) {
        printf("Error: unable to create event handler\n");
        return -1;
    }

    //make pair of both socket and event instance
    int ret = handler->addEventPair(std::pair<Socket_t, EventMsgProcess_t> 
                                   (std::move(sock), std::move(event)));

    if (ret == -1) {
        printf("Error: unable to add event pair\n");
        return -1;
    }

    handler->waitOnEvent();

    return 0;
}
