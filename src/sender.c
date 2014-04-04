#include "sender.h"

UBX_MODULE_LICENSE_SPDX(GPL-2.0+)

/* define a structure for holding the block local state. By assigning an
 * instance of this struct to the block private_data pointer (see init), this
 * information becomes accessible within the hook functions.
 */
struct sender_info
{
        /* add custom block local data here */

        /* this is to have fast access to ports for reading and writing, without
         * needing a hash table lookup */
        struct sender_port_cache ports;
	struct structsender_data dat;
};

/* init */
int sender_init(ubx_block_t *b)
{
        int ret = -1;
        struct sender_info *inf;

        /* allocate memory for the block local state */
        if ((inf = (struct sender_info*)calloc(1, sizeof(struct sender_info)))==NULL) {
                ERR("sender: failed to alloc memory");
                ret=EOUTOFMEM;
                goto out;
        }
        b->private_data=inf;
        update_port_cache(b, &inf->ports);
        ret=0;
out:
        return ret;
}

/* start */
int sender_start(ubx_block_t *b)
{
	struct sender_info *inf = (struct sender_info*) b->private_data;
	int ret = 0;
	int* num;
	unsigned int clen,clen2;
	inf->dat.text = (char*) ubx_config_get_data_ptr(b, "string", &clen);
	num = (int*) ubx_config_get_data_ptr(b, "number", &clen2);
	inf->dat.number = *num;
	return ret;
}

/* stop */
void sender_stop(ubx_block_t *b)
{
        /* struct sender_info *inf = (struct sender_info*) b->private_data; */
}

/* cleanup */
void sender_cleanup(ubx_block_t *b)
{
        free(b->private_data);
}

/* step */
void sender_step(ubx_block_t *b)
{
	struct sender_info *inf= (struct sender_info*) b->private_data;
	write_data(inf->ports.data, &inf->dat);
}

