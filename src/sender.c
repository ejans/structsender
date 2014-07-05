#include "sender.h"

UBX_MODULE_LICENSE_SPDX(GPL-2.0+)

struct sender_info
{
        struct sender_port_cache ports;
	struct structsender_data dat;
};

int create_local_fifo(ubx_block_t *b)
{
        DBG("start of create_local_fifo");
        int ret = 0;
        ubx_block_t *fifo;
        ubx_data_t *d,*d2,*d3;
        ubx_port_t *local;
        ubx_module_t* mod;

        /* if fifo module is not loaded load it */
        DBG("loading fifo module/ create");
        HASH_FIND_STR(b->ni->modules, "std_blocks/lfds_buffers/lfds_cyclic.so", mod);

        if (mod == NULL) {
                ubx_module_load(b->ni, "std_blocks/lfds_buffers/lfds_cyclic.so");
        }

        fifo = ubx_block_create(b->ni, "lfds_buffers/cyclic", "local_fifo");

        /* set type_name */
        DBG("set type name");
        d = ubx_config_get_data(fifo, "type_name");
        int len =  strlen("struct structsender_data")+1;
        ubx_data_resize(d, len);
        strncpy((char*)d->data,STRUCT_STRUCTSENDER,len);

        /* set buffer_len */
        DBG("set buffer length");
        d2 = ubx_config_get_data(fifo, "buffer_len");
        d2->data = malloc(sizeof(uint32_t));
        *(uint32_t*)d2->data = (uint32_t) 1;

        /* set data_len */
	/*
        DBG("set data length");
        d3 = ubx_config_get_data(fifo, "data_len");
        d3->data = malloc(sizeof(uint32_t));
        *(uint32_t*)d3->data = (uint32_t) sizeof(struct structsender_data);
	*/

        /* get ports */
        DBG("get ports");
        local = ubx_port_get(b, "local");

        /* connect ports to internal fifo */
        DBG("connect ports");
        if (ubx_ports_connect_uni(local, local, fifo) != 0) {
                ERR("failed to connect ports to fifo");
                return ret;
        }

        return ret;
}

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
	if (create_local_fifo(b) != 0) {
		ERR("failed to run create_local_fifo");
		goto out;
	}
        ret=0;
out:
        return ret;
}

/* start */
int sender_start(ubx_block_t *b)
{
	struct sender_info *inf = (struct sender_info*) b->private_data;
	struct structsender_data data;
	ubx_block_t *fifo;
	ubx_port_t *local;
	int ret = 0;
	int* num;
	unsigned int clen,clen2;

	fifo = ubx_block_get(b->ni, "local_fifo");
	local = ubx_port_get(b, "local");

	if (fifo->block_state == BLOCK_STATE_PREINIT) {

        	/* init and start the block */
        	DBG("init and start");
        	if(ubx_block_init(fifo) != 0) {
                	ERR("failed to init local fifo");
                	return ret;
        	}

        	if(ubx_block_start(fifo) != 0) {
                	ERR("failed to start local fifo");
                	return ret;
        	}
        	DBG("Complete");
	} else {

		//TODO Do something???
	}

	/*
	inf->dat.text = (char*) ubx_config_get_data_ptr(b, "string", &clen);
	num = (int*) ubx_config_get_data_ptr(b, "number", &clen2);
	inf->dat.number = *num;
	*/
	// Read data from config
	data.text = (char*) ubx_config_get_data_ptr(b, "string", &clen);
	data.number = *(int*) ubx_config_get_data_ptr(b, "number", &clen2);
	// Write data to port of local fifo
	write_local(local, &data);
	DBG("DATA: %p\n", data);
	DBG("DATA.TEXT: %s\n", data.text);
	DBG("DATA.NUMBER: %i\n", data.number);
	// Remove configs
	// if we don't remove these the pointer for the text will still exist.
	//ubx_config_rm(b, "string");
	ubx_config_rm(b, "number");

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
	ubx_block_t* fifo;
	ubx_port_t *local;

	fifo = ubx_block_get(b->ni, "local_fifo");
	local = ubx_port_get(b, "local");

	DBG("disconnecting ports");
	if (ubx_ports_disconnect_uni(local, local, fifo) != 0) {
		ERR("failed to disconnect ports to fifo");
	}
	ubx_block_stop(fifo);
	ubx_block_cleanup(fifo);
	ubx_block_rm(b->ni, "local_fifo");
        free(b->private_data);
}

/* step */
void sender_step(ubx_block_t *b)
{
	/*
	struct sender_info *inf= (struct sender_info*) b->private_data;
	write_data(inf->ports.data, &inf->dat);
	*/
	//TODO Read from port, write to port, write to data
	ubx_block_t *fifo;
	ubx_port_t *local,*datap;
	struct structsender_data data;

	fifo = ubx_block_get(b->ni, "local_fifo");
	local = ubx_port_get(b, "local");
	datap = ubx_port_get(b, "data");

	read_local(local, &data);
	DBG("DATA: %p\n", data);
	DBG("DATA.TEXT: %s\n", data.text);
	DBG("DATA.NUMBER: %i\n", data.number);
	write_data(local, &data);
	write_data(datap, &data);
}

