/* Copyright - Nubos Networks
 *
 * author - Sri
 */

#ifndef _VTL_IO_H_
#define _VTL_IO_H_

#include <map>
#include <memory>
#include <utility>
#include <queue>

#include <pthread.h>

#include <select.h>


class Socket;

/* this class serves as base class
 * definition for message passed
 * between socket class and event
 * processing class
 *
 * respective implementations should
 * derive this base class and imple-
 * ments required function
 *
 * getInstancexxx() function provides atomic
 * initialization of object instance
 *
 * please take a look at current implements
 * to see how to use this class
 *
 */
class Msg {
public:
    typedef void (*FreeFunc_t)(void *); 

    ~Msg();

    /* should be used by event processing class
     * returns buffer where data is read from 
     * socket
     * @buff : pointer dereference to get data buffer
     *
     * returns : size of buffer if data present
     *           otherwise 0
     */
    int getMsgBuff(void **buff);
    void *getMsgBuff();
    Socket *getSocket(); 

    static std::unique_ptr<Msg> getMsgInstanceCreate(int size, 
                                                     Socket &sock);
    /* this function should be called if you have 
     * predefined buffer allocated for message and
     * function that understands semantic of buffer
     * to free
     * @buff : preallocated buffer
     * @func : function to free this given buffer
     */
    static std::unique_ptr<Msg> getMsgInstanceAssign(void *buff,
                                                     int size,
                                                     FreeFunc_t func,
                                                     Socket &sock);

protected:

    void setBuffer(void *buff, int size, FreeFunc_t func);
    int createBuffer(int size);
    void setSocket(Socket &sock);

    Msg(): mMsgBuff(NULL), 
           mBuffSize(0), 
           mBufferCallBack(NULL), 
           mSock(NULL) {}

    //pointer defines the message buffer holder
    void* mMsgBuff;
    int   mBuffSize;

    FreeFunc_t mBufferCallBack;

    //socket instance from where msg received
    Socket *mSock;
};

typedef std::unique_ptr<Msg> Msg_t;


/* generic class definition for any
 * kind of sockets
 *
 * NOTE : the derived class should implement
 *        public static getInstance() function to
 *        create and return instance
 */
class Socket {
public:
    virtual ~Socket() {}
    int getFd() { return mFd; }

    /* reads the data from this socket
     * @buff : buffer contents to read
     * @size : size of @buff
     * returns num bytes read into @buff
     */
    virtual std::unique_ptr<Msg> recv()   = 0;
    /* sends the given buffer of size on
     * this socket
     * @msg  : message to be sent
     * @size : size of message to be sent
     */
    virtual int send(void *msg, int size) = 0;
    /* this function has to be defined and 
     * implemented by every derived class 
     * the function must make sure if all 
     * resource init is not successfull then 
     * close fd before assigning to mFd
     */
    virtual int open() = 0;

    /* NOTE - please do not define close() function
     *        for the derived class. Event handler
     *        of this socket will close the fd.
     *        if derived socket closes the fd, then
     *        it is bug
     */

protected:

    /* derived class member function can create the
     * the instance of this class
     */
    Socket():mFd(-1){}

    int mFd;
};

typedef std::unique_ptr<Socket> Socket_t;

/* processEventMsg() functions return values */
typedef enum EventProcessRet {
    MSG_P_SUCCESS=0, /* function returns success */
    MSG_P_FAIL,      /* if something failure occurrs in processing events */
    MSG_P_RMPAIR,    /* if the socket and event processing pair has to be deleted */
} EventProcessRet_t;

/* generic class definition for event
 * msg processing.
 *
 * NOTE : the derived class should implement
 *        public static getInstance() function to
 *        create and return instance
 */
class EventMsgProcess {
public:
    virtual ~EventMsgProcess() {}

    /* process the given buffer message from event
     * should return enum EventProcessRet type
     * NOTE - if processEventMsg returns MSG_P_RMPAIR
     *        then the pair of socket and event process
     *        will be removed from listening queue.
     */
    virtual EventProcessRet_t  processEventMsg(std::unique_ptr<Msg>) = 0;

protected:

    /* derived class member function can create the
     * the instance of this class
     */
    EventMsgProcess() {}

};

typedef std::unique_ptr<EventMsgProcess> EventMsgProcess_t;

/* event pair for socket and processing socket message */
typedef std::pair<Socket_t, EventMsgProcess_t> EventPair_t;

#define EVENT_MSG_BUFF_SZ  1500

/* this class provides semantics and logic to
 * tie Socket and EventMsgProcess classes as
 * pair to handle events coming on sockets
 *
 */

class EventHandler {

public:
    ~EventHandler() {}

    /* invokes the event handler given
     * to this instance associate to
     * given @fd
     */
    EventProcessRet_t handleEvent(int Fd);

    /* should call this function to wait
     * on events
     * postconditions: without any event pair to monitor
     *                 waitOnEvent will exit
     *                 this is blocking call.
     *
     * unblocks when error occurs
     */
    int waitOnEvent();

    bool isReset()           { return mResetFdQueue;  }
    void clearResetFdQueue() { mResetFdQueue = false; }

    int  addEventPair(EventPair_t );
    void rmEventPair(int fd);

    /* to get event handler ins should call this function
     */
    static std::unique_ptr<EventHandler> getEventHandlerIns();

private:

    void setResetFdQueue()   { mResetFdQueue = true;  }

    /* should call getEventHandlerIns() to create instance 
     * for this class
     * NOTE - recursive mutex is important because eventhandler
     *        internal function calls holds lock, and make calls
     *        to other functions which held the same lock.
     */
    EventHandler(): mMapMutex(PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP) {}

    /* cleans all the sockets and fd from select call */
    void cleanUp();

    std::map<int, EventPair_t> mEventPairMap;
    typedef std::map<int, EventPair_t>::iterator EventPairMapItr_t;
    int mFdArray[MAX_READ_FDSET_SZ];

    /* fds to close */
    std::queue<int> mCloseFdSet;

    /* when new fd or old fd is removed 
     * then this flag is set 
     */
    bool mResetFdQueue;

    /* local buffer used by this event handler*/
    char mBuffer[EVENT_MSG_BUFF_SZ];

    /* mutex for map */
    // NOTE: set this to be recursive lock
    pthread_mutex_t mMapMutex;
};

typedef std::unique_ptr<EventHandler> EventHandler_t;

#endif
