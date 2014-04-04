#include "receiver.h"

UBX_MODULE_LICENSE_SPDX(GPL-2.0+)

/* define a structure for holding the block local state. By assigning an
 * instance of this struct to the block private_data pointer (see init), this
 * information becomes accessible within the hook functions.
 */
struct receiver_info
{
        /* add custom block local data here */

        /* this is to have fast access to ports for reading and writing, without
         * needing a hash table lookup */
        struct receiver_port_cache ports;
};

/* init */
int receiver_init(ubx_block_t *b)
{
        int ret = -1;
        struct receiver_info *inf;

        /* allocate memory for the block local state */
        if ((inf = (struct receiver_info*)calloc(1, sizeof(struct receiver_info)))==NULL) {
                ERR("receiver: failed to alloc memory");
                ret=EOUTOFMEM;
                goto out;
        }
        b->private_data=inf;
        update_port_cache(b, &inf->ports);
        ret=0;
out:
        return ret;
}



/* cleanup */
void receiver_cleanup(ubx_block_t *b)
{
        free(b->private_data);
}

/* step */
void receiver_step(ubx_block_t *b)
{
        struct receiver_info *inf = (struct receiver_info*) b->private_data;
	struct structsender_data dat;
	read_data(inf->ports.data, &dat);
	printf("TEXT: %s\n", dat.text);
	printf("NUMBER: %u\n", dat.number);
}

