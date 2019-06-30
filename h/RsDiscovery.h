/*
 * Copyright - Nubos Networks
 *
 * Author - Sri
 *
 */

#ifndef _RS_DISCOVERY_H_
#define _RS_DISCOVERY_H_

#include <stdint.h>

/* client server id used by router
 * server to establish bgp peer session
 */
#define CLIENT_ROUTER_ID 0x12345

/* router server id used by clients
 * to establish bgp peer session
 */
#define RS_ROUTER_ID     0x67890

/* NOTE - sess_id is generated randomly using rand(),
 *        so there will be case where two nodes might
 *        be generating same id, causing to clash and
 *        can result in inconsistent entries
 */

struct discovery_msg_t {
    /* this structure will be extended
     * when ported to BGP based disc-
     * overy mechanism
     */
    uint32_t router_id;    /* id that determines provider sent or
                            * consumer sent broadcast */
    uint32_t sess_id;      /* session id of broadcast request from client */
    uint32_t location_ip;  /* location where srv ip is seated */
};

typedef struct discovery_msg_t DiscoveryMsg_t;


#endif
