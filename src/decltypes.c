
#include <stdint.h>

#include <ubx.h>

#include "../types/structsender_data.h"
#include "../types/structsender_data.h.hexarr"
/* declare types */
ubx_type_t types[] = {
        def_struct_type(struct structsender_data, &structsender_data_h),
        { NULL },
};

static int decltypes_init(ubx_node_info_t* ni)
{
        DBG(" ");
        ubx_type_t *tptr;
        for(tptr=types; tptr->name!=NULL; tptr++) {
                /* TODO check for errors */
                ubx_type_register(ni, tptr);
        }

        return 0;
}

static void decltypes__cleanup(ubx_node_info_t *ni)
{
        DBG(" ");
        const ubx_type_t *tptr;

        for(tptr=types; tptr->name!=NULL; tptr++)
                ubx_type_unregister(ni, tptr->name);
}

UBX_MODULE_INIT(decltypes_init)
UBX_MODULE_CLEANUP(decltypes__cleanup)
UBX_MODULE_LICENSE_SPDX(BSD-3-Clause)