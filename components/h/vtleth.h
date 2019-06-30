/*
 * Copyright - Nubos Networks
 *
 * Author - Sri
 *
 */

#ifndef _VTL_ETHER_HDR_H_
#define _VTL_ETHER_HDR_H_

#include <stdint.h>
#include <netinet/ether.h>

struct _ethhdr {
    unsigned char h_dest[ETH_ALEN];
    unsigned char h_source[ETH_ALEN];
    uint16_t h_proto;
} __attribute__ ((__packed__));


#endif
