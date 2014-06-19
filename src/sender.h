/*
 * sender microblx function block (autogenerated, don't edit)
 */

//#define DEBUG 1
#define STRUCT_STRUCTSENDER "struct structsender_data"

#include <ubx.h>

/* includes types and type metadata */
#include "../types/structsender_data.h"
#include "../types/structsender_data.h.hexarr"

ubx_type_t types[] = {
        def_struct_type(struct structsender_data, &structsender_data_h),
        { NULL },
};

/* block meta information */
char sender_meta[] =
        " { doc='',"
        "   real-time=true,"
        "}";

/* declaration of block configuration */
ubx_config_t sender_config[] = {
        { .name="string", .type_name = "char", .doc="string to send out" },
        { .name="number", .type_name = "int", .doc="number to send out" },
        { NULL },
};

/* declaration port block ports */
ubx_port_t sender_ports[] = {
        { .name="data", .out_type_name="struct structsender_data", .out_data_len=1, .doc="struct containing string and number"  },
	{ .name="local", .in_type_name="struct structsender_data", .out_type_name="struct structsender_data", .doc="local port" },
        { NULL },
};

/* declare a struct port_cache */
struct sender_port_cache {
        ubx_port_t* data;
	ubx_port_t* local;
};

/* declare a helper function to update the port cache this is necessary
 * because the port ptrs can change if ports are dynamically added or
 * removed. This function should hence be called after all
 * initialization is done, i.e. typically in 'start'
 */
static void update_port_cache(ubx_block_t *b, struct sender_port_cache *pc)
{
        pc->data = ubx_port_get(b, "data");
	pc->local = ubx_port_get(b, "local");
}


/* for each port type, declare convenience functions to read/write from ports */
def_write_fun(write_data, struct structsender_data)
def_write_fun(write_local, struct structsender_data)
def_read_fun(read_local, struct structsender_data)

/* block operation forward declarations */
int sender_init(ubx_block_t *b);
int sender_start(ubx_block_t *b);
void sender_stop(ubx_block_t *b);
void sender_cleanup(ubx_block_t *b);
void sender_step(ubx_block_t *b);


/* put everything together */
ubx_block_t sender_block = {
        .name = "sender",
        .type = BLOCK_TYPE_COMPUTATION,
        .meta_data = sender_meta,
        .configs = sender_config,
        .ports = sender_ports,

        /* ops */
        .init = sender_init,
        .start = sender_start,
        .stop = sender_stop,
        .cleanup = sender_cleanup,
        .step = sender_step,
};


/* sender module init and cleanup functions */
int sender_mod_init(ubx_node_info_t* ni)
{
        DBG(" ");
        int ret = -1;
        ubx_type_t *tptr;

        for(tptr=types; tptr->name!=NULL; tptr++) {
                if(ubx_type_register(ni, tptr) != 0) {
                        goto out;
                }
        }

        if(ubx_block_register(ni, &sender_block) != 0)
                goto out;

        ret=0;
out:
        return ret;
}

void sender_mod_cleanup(ubx_node_info_t *ni)
{
        DBG(" ");
        const ubx_type_t *tptr;

        for(tptr=types; tptr->name!=NULL; tptr++)
                ubx_type_unregister(ni, tptr->name);

        ubx_block_unregister(ni, "sender");
}

/* declare module init and cleanup functions, so that the ubx core can
 * find these when the module is loaded/unloaded */
UBX_MODULE_INIT(sender_mod_init)
UBX_MODULE_CLEANUP(sender_mod_cleanup)
