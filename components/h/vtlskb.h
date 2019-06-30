/*
 * Copyright - Nubos Networks
 *
 * Author - Sri
 *
 */

#ifndef _VTL_SKB_H_
#define _VTL_SKB_H_

#include <stdint.h>
#include <stdlib.h>

/* This file defines skb struct in userspace.
 * It has structure and semantics similar to
 * kernel skb.
 *
 */

#ifdef __cplusplus
extern "C" {
#endif

#include <vtl-mmap-rec.h>

/*
 * @mod_flag   : indicates whether this skb instance is
 *               modified
 * @path       : information on packet direction, whether
 *               it is RX or TX
 * @action     : userspace datapath makes decision on this
 *               packet and assigns directive to communciate
 *               to kernel
 * @cb         : control block of 48 bytes to pass meta data
 *               when processing this skb
 * @len        : length of this packet, the skb pointing to
 * @mac_len    : mac header length 
 * @kernel_ptr : address of the skb of this packet in kernel
 *
 *
 * @inner_transport_header : offset of the inner L4 header in 
 *                           this packet from head pointer of
 *                           this skb
 * @inner_network_header   : offset of the inner l3 header in
 *                           this packet from head pointer of          
 *                           this skb
 * @inner_mac_header       : offset of the inner l2 header in
 *                           this packet from head pointer of
 *                           this skb
 *
 * NOTE - In non-overlay packet outer header values are only
 *        filled in skb.
 *
 * @transport_header       : offset of the outer l4 header in
 *                           this packet from head pointer of
 *                           this skb.
 * @network_header         : offset of the outer l3 header in
 *                           this packet from head pointer of
 *                           this skb
 * @mac_header             : offset of the outer l2 header in
 *                           this packet from head pointer of
 *                           this skb
 *
 * NOTE - tail, end, data, head pointer have same meaning as
 *        they are in kernel skb
 *
 * @tail : pointer to end of data of this packet in the mbuff
 * @end  : pointer to end of mbuff of this packet, i.e bytes 
 *         room for new data that can be added to this packet
 * @data : starting of outer header of this packet. Example
 *         is l2 header start
 * @head : start of mbuff. Space between @head and @data is
 *         room for new headers that can be pushed for this
 *         packet
 */
    
struct sk_buff {

    int mod_flag;
    pkt_path_t path;
    pkt_action_t action;
    char cb[48];
    unsigned int len;
    uint16_t mac_len;

    unsigned long kernel_ptr;

    uint16_t inner_transport_header;
    uint16_t inner_network_header;
    uint16_t inner_mac_header;

    uint16_t transport_header;
    uint16_t network_header;
    uint16_t mac_header;

    unsigned char *tail;
    unsigned char *end;
    unsigned char *head;
    unsigned char *data;
};

extern inline int build_skb(struct sk_buff *skb, 
                     struct pkt_mmap_record *data);

extern inline int skb_action_get(struct pkt_mmap_record *data,
                                 struct sk_buff *skb);

static inline void skb_set_tx_path(struct sk_buff *skb)
{
    skb->path = TX_PATH;
}

static inline void skb_set_rx_path(struct sk_buff *skb)
{
    skb->path = RX_PATH;
}

static inline int is_skb_tx_path(const struct sk_buff *skb) {
    return (skb->path == TX_PATH);
}

static inline int is_skb_rx_path(const struct sk_buff *skb) {
    return (skb->path == RX_PATH);
}

static inline void skb_modify(struct sk_buff *skb)
{
    skb->mod_flag = 1;
}

static inline int is_skb_modified(struct sk_buff *skb)
{
    return skb->mod_flag;
}

static inline unsigned int skb_end_offset(const struct sk_buff *skb) 
{
    return skb->end - skb->head;
}

static inline unsigned char *skb_end_pointer(const struct sk_buff *skb) 
{
    return skb->end;
}

static inline unsigned char *skb_tail_pointer(const struct sk_buff *skb) 
{
    return skb->tail;
}

static inline void skb_reset_tail_pointer(struct sk_buff *skb) 
{
    skb_modify(skb);
    skb->tail = (unsigned char *) (skb->data - skb->head);
}

static inline void skb_set_tail_pointer(struct sk_buff *skb, const int offset) 
{
    skb_reset_tail_pointer(skb);
    skb->tail += offset;
}

static inline unsigned char *skb_push(struct sk_buff *skb, unsigned int len)
{
    if ((skb->data - len) < skb->head) 
        return NULL; 
    skb->data -= len;
    skb->len  += len;
    skb_modify(skb);
    return skb->data;
}

static inline unsigned char *skb_put(struct sk_buff *skb, unsigned int len)
{
    unsigned char *tmp = skb_tail_pointer(skb);
    if ((skb->tail + len) > skb->head)
        return NULL;
    skb->tail += len;
    skb->len  += len;
    skb_modify(skb);
    return tmp;
}

static inline unsigned char *skb_pull(struct sk_buff *skb, unsigned int len)
{
    if ((skb->data + len) > skb->tail)
        return NULL;
    skb_modify(skb);
    skb->len -= len;
    return skb->data += len;
}


static inline int skb_headroom(const struct sk_buff *skb) 
{
    return skb->data - skb->head;
}

static inline int skb_tailroom(const struct sk_buff *skb)
{
    return skb->end - skb->tail;
}

static inline void skd_reset_inner_headers(struct sk_buff *skb)
{
    skb_modify(skb);
    skb->inner_mac_header = skb->mac_header;
    skb->inner_network_header = skb->network_header;
    skb->inner_transport_header = skb->transport_header;
}

static inline void skb_reset_mac_len(struct sk_buff *skb)
{
    skb_modify(skb);
    skb->mac_len = skb->network_header - skb->mac_header;
}

static inline unsigned char *skb_inner_transport_header(const struct sk_buff *skb)
{
    return skb->head + skb->inner_transport_header;
}

static inline void skb_reset_inner_transport_header(struct sk_buff *skb)
{
    skb_modify(skb);
    skb->inner_transport_header = skb->data - skb->head;
}

static inline void skb_set_inner_transport_header(struct sk_buff *skb, int offset)
{
    skb_reset_inner_transport_header(skb);
    skb->inner_transport_header += offset;
}

static inline unsigned char *skb_inner_network_header(const struct sk_buff *skb)
{
    return skb->head + skb->inner_network_header;
}

static inline void skb_reset_inner_network_header(struct sk_buff *skb)
{
    skb_modify(skb);
    skb->inner_network_header = skb->data - skb->head;
}

static inline void skb_set_inner_network_header(struct sk_buff *skb, int offset)
{
    skb_reset_inner_network_header(skb);
    skb->inner_network_header += offset;
}

static inline unsigned char *skb_inner_mac_header(const struct sk_buff *skb)
{
    return skb->head + skb->inner_mac_header;
}

static inline void skb_reset_inner_mac_header(struct sk_buff *skb)
{
    skb_modify(skb);
    skb->inner_mac_header = skb->data - skb->head;
}

static inline void skb_set_inner_mac_header(struct sk_buff *skb, int offset)
{
    skb_reset_inner_mac_header(skb);
    skb->inner_mac_header += offset;
}

static inline unsigned char *skb_transport_header(const struct sk_buff *skb)
{
    return skb->head + skb->transport_header;
}

static inline void skb_reset_transport_header(struct sk_buff *skb)
{
    skb_modify(skb);
    skb->transport_header = skb->data - skb->head;
}

static inline void skb_set_transport_header(struct sk_buff *skb, int offset)
{
    skb_reset_transport_header(skb);
    skb->transport_header += offset;
}

static inline unsigned char *skb_network_header(const struct sk_buff *skb)
{
    return skb->head + skb->network_header;
}

static inline void skb_reset_network_header(struct sk_buff *skb)
{
    skb_modify(skb);
    skb->network_header = skb->data - skb->head;
}

static inline void skb_set_network_header(struct sk_buff *skb, int offset)
{
    skb_reset_network_header(skb);
    skb->network_header += offset;
}

static inline unsigned char *skb_mac_header(const struct sk_buff *skb)
{
    return skb->head + skb->mac_header;
}

static inline void skb_reset_mac_header(struct sk_buff *skb)
{
    skb_modify(skb);
    skb->mac_header = skb->data - skb->head;
}

static inline void skb_set_mac_header(struct sk_buff *skb, int offset)
{
    skb_reset_mac_header(skb);
    skb->mac_header += offset;
}

static inline int skb_transport_offset(const struct sk_buff *skb)
{
    return skb_transport_header(skb) - skb->data;
}

static inline int skb_network_header_len(const struct sk_buff *skb)
{
    return skb->transport_header - skb->network_header;
}

static inline int skb_inner_network_header_len(const struct sk_buff *skb)
{
    return skb->inner_transport_header - skb->inner_network_header;
}

static inline int skb_network_offset(const struct sk_buff *skb)
{
    return skb_network_header(skb) - skb->data;
}

static inline int skb_inner_network_offset(const struct sk_buff *skb)
{
    return skb_inner_network_header(skb) - skb->data;
}

#ifdef __cplusplus
};
#endif


#endif
