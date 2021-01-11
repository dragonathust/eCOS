#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sys_types.h"
#include "sys_define.h"
#include "mtos_printk.h"
#include "mtos_misc.h"
#include "mtos_task.h"
#include "mtos_mem.h"
#include "mtos_sem.h"
#include "mtos_timer.h"
#include "drv_dev.h"
#include "charsto.h"

#include <linux/types.h>
#include <linux/stat.h>
#include <linux/fileio.h>
#include <linux/dirent.h>
#include <pkgconf/fs_jffs2.h>
#include <cyg/io/config_keys.h>
#include <cyg/io/flash.h>

//#define JFFS2_DBG(...) OS_PRINTF(__VA_ARGS__)
#define JFFS2_DBG(...)

#define FLASH_BLOCK_SIZE    (64*1024)
#define FLASH_DEV_SIZE    (2*1024*1024)
#define FLASH_DEV_OFFSET    (4*1024*1024)

cyg_mtab_entry cyg_mtab[CYGNUM_FILEIO_MTAB_MAX];
cyg_fstab_entry cyg_fstab[CYGNUM_FILEIO_FSTAB_MAX];

extern cyg_fstab_entry jffs2_fste;

Cyg_ErrNo errno; 

int jffs2_init(int erase)
{
     RET_CODE ret;
	 
  	struct cyg_fstab_entry *f;
	struct cyg_mtab_entry *m;

	cyg_mtab_entry cyg_mtab_jffs2={ "/",    "jffs2", "/dev/flash", 0, 0, NULL, 0 };

	JFFS2_DBG("call jffs2_init()\n");
	
     struct charsto_device *charsto_dev ;

	 if(erase)
	 	{
	  charsto_dev = dev_find_identifier(NULL,
                                                  DEV_IDT_TYPE,
                                                  SYS_DEV_TYPE_CHARSTO);
	  
	OS_PRINTF("jffs2_init() eraseing...\n");

			ret = charsto_erase(charsto_dev, FLASH_DEV_OFFSET, FLASH_DEV_SIZE/FLASH_BLOCK_SIZE);

   			if (SUCCESS != ret)
   			{
   			JFFS2_DBG("charsto_erase addr=0x%08x error!\n",FLASH_DEV_OFFSET);
			return EIO;
   			}	
			
	OS_PRINTF("jffs2_init() erase ok.\n");
	 	}

	    for( f = &cyg_fstab[0]; f != &cyg_fstab_end; f++ )
	    	{
			memset(f,0,sizeof(cyg_fstab_entry));
	    	}
		
	for(m = &cyg_mtab[0]; m!= &cyg_mtab_end; m++ )
		{
			memset(m,0,sizeof(cyg_mtab_entry));
		}

	memcpy(&cyg_fstab[0],&jffs2_fste,sizeof(cyg_fstab_entry));
	memcpy(&cyg_mtab[0],&cyg_mtab_jffs2,sizeof(cyg_mtab_entry));
	
	Cyg_Fileio_Init_Class();
}

void cyg_drv_mutex_init( cyg_drv_mutex_t *mutex )
{
 *mutex = (cyg_drv_mutex_t) mtos_mutex_create(6);
}

cyg_bool_t cyg_drv_mutex_lock( cyg_drv_mutex_t *mutex )
{
 mtos_mutex_take((void*)(*mutex));
}

void cyg_drv_mutex_unlock( cyg_drv_mutex_t *mutex )
{
 mtos_mutex_give((void*)(*mutex));
}

//=============================================================================
// Timestamp support

// Provides the current time as a time_t timestamp for use in filesystem
// data strucures.
/*
time_t cyg_timestamp(void)
{
	return (time_t)mtos_ticks_get();
}
*/
// Lookup a device and return it's handle
Cyg_ErrNo cyg_io_lookup(const char *name,
                        cyg_io_handle_t *handle)
{
struct charsto_device *charsto_dev = NULL;

	if( strcmp(name,"/dev/flash") ==0 )
	{
	  charsto_dev = dev_find_identifier(NULL,
                                                  DEV_IDT_TYPE,
                                                  SYS_DEV_TYPE_CHARSTO);

	if(charsto_dev)
	 {	
	 	*handle = (cyg_io_handle_t)charsto_dev;
		return ENOERR;
	 }
	}
	
	return ENODEV;
}

// Write data to a block device
Cyg_ErrNo cyg_io_bwrite(cyg_io_handle_t handle,
                       const void *buf,
                       cyg_uint32 *len,
                       cyg_uint32 pos)
{
     RET_CODE ret;
     struct charsto_device *charsto_dev =(struct charsto_device *) handle;

   JFFS2_DBG("cyg_io_bwrite buf=0x%08x, *len=%d, pos=0x%08x \n",buf,*len,pos);
   
    // Special check.  If length is zero, this just verifies that the 
    // 'bwrite' method exists for the given device.
    if (NULL != len && 0 == *len) {
        return ENOERR;
    }
	
     ret = charsto_writeonly(charsto_dev, FLASH_DEV_OFFSET+pos,(u8*)buf, *len);

   if (SUCCESS != ret)
   	{
   		JFFS2_DBG("cyg_io_bwrite addr=0x%08x error!\n",pos);
		return EIO;
   	}	 

		return ENOERR;
}

// Read data from a block device
Cyg_ErrNo cyg_io_bread(cyg_io_handle_t handle,
                      void *buf,
                      cyg_uint32 *len,
                      cyg_uint32 pos)
{
     RET_CODE ret;
     struct charsto_device *charsto_dev =(struct charsto_device *) handle;

   JFFS2_DBG("cyg_io_bread buf=0x%08x, *len=%d, pos=0x%08x \n",buf,*len,pos);
		
    // Special check.  If length is zero, this just verifies that the 
    // 'bread' method exists for the given device.
    if (NULL != len && 0 == *len) {
        return ENOERR;
    }
	
     ret = charsto_read(charsto_dev,FLASH_DEV_OFFSET+pos,buf,*len);

   if (SUCCESS != ret)
   	{
   		JFFS2_DBG("cyg_io_bread addr=0x%08x error!\n",pos);
		return EIO;
   	}	 

		return ENOERR;	 
}

// Get the configuration of a device
Cyg_ErrNo cyg_io_get_config(cyg_io_handle_t handle,
                            cyg_uint32 key,
                            void *buf,
                            cyg_uint32 *len)
{
     RET_CODE ret;
	cyg_io_flash_getconfig_devsize_t *ds;
	cyg_io_flash_getconfig_blocksize_t *bs;
	cyg_io_flash_getconfig_erase_t *e;
	
     struct charsto_device *charsto_dev =(struct charsto_device *) handle;

	switch(key)
		{

		case CYG_IO_GET_CONFIG_FLASH_DEVSIZE:
			ds=(cyg_io_flash_getconfig_devsize_t *)buf;
			ds->dev_size = FLASH_DEV_SIZE;
			break;

		case CYG_IO_GET_CONFIG_FLASH_BLOCKSIZE:
			bs=(cyg_io_flash_getconfig_blocksize_t *)buf;
			bs->block_size = FLASH_BLOCK_SIZE;
			bs->offset = FLASH_DEV_OFFSET;
			break;

		case CYG_IO_GET_CONFIG_FLASH_ERASE:
		
			e=(cyg_io_flash_getconfig_erase_t *)buf;

   			JFFS2_DBG("cyg_io_get_config ERASE e->offset=0x%08x, e->len=%d\n",e->offset,e->len);
			
	      		ret = charsto_erase(charsto_dev, FLASH_DEV_OFFSET+e->offset, e->len/FLASH_BLOCK_SIZE);

   			if (SUCCESS != ret)
   			{
   			JFFS2_DBG("charsto_erase addr=0x%08x error!\n",e->offset);
			return EIO;
   			}

			e->flasherr =0;
			break;

	
		default:
			break;

		}
	
	return ENOERR;
}

// Change the configuration of a device
Cyg_ErrNo cyg_io_set_config(cyg_io_handle_t handle,
                            cyg_uint32 key,
                            const void *buf,
                            cyg_uint32 *len)
{

}

//=============================================================================
// Default functions.
// Cast to the appropriate type, these functions can be put into any of
// the operation table slots to provide the defined error code.
//==========================================================================
// Default functions
/*
__externC int cyg_fileio_enosys() { return ENOSYS; }
__externC int cyg_fileio_erofs() { return EROFS; }
__externC int cyg_fileio_enoerr() { return ENOERR; }
__externC int cyg_fileio_enotdir() { return ENOTDIR; }

__externC cyg_bool cyg_fileio_seltrue (struct CYG_FILE_TAG *fp, int which, CYG_ADDRWORD info)
{ return 1; }
*/

