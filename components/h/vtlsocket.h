/*
 * Copyright - Nubos Networks
 *
 *
 * Author - Sri
 */

#ifndef _VTL_SOCKET_C_H_
#define _VTL_SOCKET_C_H_

#include <netinet/in.h>
#include <sys/socket.h>

#define MORE_BYTES_AVAIL 1

#ifdef __cplusplus
extern "C" {
#endif

/* gets the broadcast socket fd for
 * given port number
 * @fd : place holder to return fd
 * @intf : interface to which socket to bind
 * returns 0 if successful otherwise -1
 *
 * NOTE - socket fd is dgram socket
 */
extern
int vtl_create_broadcast_socket(int *fd, char *intf);

/* binds the given socket fd to given port
 * only if given socket is broadcast socket fd
 *
 * @port : port number to bind the broadcast socket
 * @fd   : broadcast fd socket to bind for given port
 *
 * returns 0 if successfully binded. Otherwise -1
 *
 * NOTE - if you want socket to be in receive mode
 *        on select or recv, should call this func
 *        and after successful call, pass the fd to
 *        recv or select function
 */
extern
int vtl_bind_broadcast_socket(int port, 
                              int fd);

/* sends broadcast message on given port and socket
 * fd. socket fd should be created as broadcast socket
 *
 * @fd   : socket fd on which broadcast message to be send
 * @port : port number on which message has to be transmi-
 *         tted
 * @buff : buffer to be appended on message
 * @size : size of the message
 *
 * returns 0 if successfull otherwise -1
 */
extern 
int vtl_send_broadcast_msg(int fd, 
                           int port,
                           char *buff, 
                           int size);

/* Receives broadcast message on given port and socket
 * fd. socket fd should be created as broadcast socket
 *
 * @fd   : socket fd on which broadcast message to be 
 *         received
 * @port : port number on which message has to be rece-
 *         ived.
 * @buff : buffer to where recved msg to be copied
 * @size : size of buffer
 *
 * returns 0 if successfully received all bytes 
 *         1 if fewer bytes are received less than size
 *        -1 if error or no bytes recved. In case wrong
 *           port, message is read in buffer
 */
extern 
int vtl_recv_broadcast_msg(int fd,
                           int port,
                           char *buff,
                           int size);


/* Retrives ipv4 address assigned to given interface
 * @intf  : interface name for which IP address to
 *          be retrived
 * @addr  : ret value where you can find IP address
 *          configured on interface
 *
 * returns 0 if successfully retrived otherwise -1
 */
extern 
int vtl_get_ipv4_on_intf(const char *intf,
                         struct in_addr *addr);

/* Retrives mac address assigned to given interface
 * @intf  : interface name for which mac address to
 *          be retrived
 * @mac   : ret value where you can find mac address
 *          configured on interface
 *
 * returns 0 if successfully retrived otherwise -1
 */
extern
int vtl_get_macaddr_on_intf(const char *intf,
                            char *mac);


#ifdef __cplusplus
};
#endif

#endif
