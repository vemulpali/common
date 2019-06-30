/*
 * Copyright - Nubos Networks
 *
 * Author - Sri
 *
 */


#ifndef _VTL_NL_H_
#define _VTL_NL_H_

/* NOTE - this serves as vtl netlink lib
 */

#include <vtl-nl-cmds.h>
#include <sys/socket.h>
#include <linux/netlink.h>

#ifdef __cplusplus
extern "C" {
#endif

/* sends given nl_cmd to kernel 
 *
 * postconditions : message will be sent
 * on calling thread pid
 *
 * returns 0 if success otherwise -1 
 *
 */
int vtl_nl_send_cmd_to_krn(int sock, 
                           int seq, 
                           const void *cmd,
                           unsigned int cmd_size);

int vtl_nl_send_msg_to_pid(int sock,
                           int seq,
                           const void *cmd,
                           unsigned int cmd_size,
                           int pid);

/* creates message structure to recv messages
 * using recvmsg()
 * returns pointer to data buffer of recv msg
 * caller should call recvmsg() on msg to read data
 * into data buffer
 *
 * postconditions : caller has to call vtl_nl_free_recv_msg()
 *                  on msg
 */
void *vtl_nl_create_recv_msg(struct msghdr *msg, int size);
void vtl_nl_free_recv_msg(struct msghdr *msg);

/* frees the nlh data message buffer
 * preconditions: pointer returned by vtl_nl_recv_msg_frm_krn()
 */
void vtl_nl_free_msg_buff(void *ptr);

/* receives message from given pid and returns pointer
 * to data part of message. 
 *
 * Postconditions : caller has to free the return pointer
 *                  calling vtl_nl_free_msg_buff();
 */
void *vtl_nl_recv_msg_frm_any(int sock, int size);


#ifdef __cplusplus
};
#endif

#endif
