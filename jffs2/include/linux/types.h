#ifndef __LINUX_TYPES_H__
#define __LINUX_TYPES_H__

//#include "cyg/infra/cyg_type.h"

/*
#define uint8_t cyg_uint8
#define uint16_t cyg_uint16
#define uint32_t cyg_uint32

#define int8_t cyg_int8
#define int16_t cyg_int16
#define int32_t cyg_int32

#define loff_t off_t


#define kvec iovec
*/

#define endian_rev16(x) (((x >> 8) & 255) | ((x & 255) << 8))
#define endian_rev32(x) (((x >> 24) & 255) | ((x >> 8) & 0xff00) \
		| ((x & 0xff00) << 8) | ((x & 255) << 24))

#define  cyg_uint8 uint8_t
#define  cyg_uint16 uint16_t
#define  cyg_uint32 uint32_t
#define cyg_uint64 uint64_t

#define  cyg_int8 int8_t
#define  cyg_int16 int16_t
#define  cyg_int32 int32_t

#define cyg_bool int
#define cyg_bool_t int

#define uint8_t unsigned char
#define uint16_t unsigned short
#define uint32_t unsigned int
#define uint64_t unsigned long long

#define int8_t signed char
#define int16_t short
#define int32_t int

//#define loff_t off_t


#define kvec iovec

#define cyg_io_handle_t uint32_t

#define CYG_MACRO_START do {
#define CYG_MACRO_END   } while (0)

#define CYG_UNUSED_PARAM(type,val)

#define PAGE_SHIFT 12

typedef unsigned int cyg_mutex_t;
typedef unsigned int cyg_flag_t;
typedef unsigned int cyg_handle_t;
typedef unsigned int cyg_thread;
typedef unsigned int cyg_drv_mutex_t;

#define CYG_ADDRWORD unsigned int
#define CYG_ADDRESS unsigned int

#define __externC extern

#ifndef true
#define true 1
#endif

#ifndef false
#define false 0
#endif

#ifndef LONG_MAX
#define LONG_MAX 0x7FFFFFFF
#endif

#define SSIZE_MAX LONG_MAX

#define	_POSIX_LINK_MAX		8
#define LINK_MAX _POSIX_LINK_MAX

#define	_POSIX_NAME_MAX		14
#define NAME_MAX _POSIX_NAME_MAX

#define	_POSIX_PATH_MAX		255
#define PATH_MAX _POSIX_PATH_MAX

#define CYGNUM_FILEIO_NFILE 64
#define OPEN_MAX CYGNUM_FILEIO_NFILE

#define CYGNUM_FILEIO_NFD 64

#define CYGNUM_FILEIO_FSTAB_MAX 8

#define CYGNUM_FILEIO_MTAB_MAX 16

#define CYGNUM_FILEIO_IOVEC_MAX 64

#include "linux/codes.h"

extern Cyg_ErrNo errno; 

#define CYG_ASSERT( _bool_, _msg_ )

#define CYGBLD_ATTRIB_ALIGN(__align__) __attribute__((aligned(__align__)))
#define CYGARC_ALIGNMENT 4

#if 0

#define CYGBLD_ATTRIB_SECTION(__sect__) __attribute__((section (__sect__)))
#define CYGBLD_ATTRIB_USED __attribute__((used))

#define __string(_x) #_x
#define __xstring(_x) __string(_x)
#define CYG_LABEL_DEFN(_label) _label
#define CYGARC_P2ALIGNMENT 4

#ifndef CYG_HAL_TABLE_BEGIN
#define CYG_HAL_TABLE_BEGIN( _label, _name )                                 \
__asm__(".section \".ecos.table." __xstring(_name) ".begin\",\"aw\"\n"       \
    ".globl " __xstring(CYG_LABEL_DEFN(_label)) "\n"                         \
    ".type    " __xstring(CYG_LABEL_DEFN(_label)) ",object\n"                \
    ".p2align " __xstring(CYGARC_P2ALIGNMENT) "\n"                           \
__xstring(CYG_LABEL_DEFN(_label)) ":\n"                                      \
    ".previous\n"                                                            \
       )
#endif

#ifndef CYG_HAL_TABLE_END
#define CYG_HAL_TABLE_END( _label, _name )                                   \
__asm__(".section \".ecos.table." __xstring(_name) ".finish\",\"aw\"\n"      \
    ".globl " __xstring(CYG_LABEL_DEFN(_label)) "\n"                         \
    ".type    " __xstring(CYG_LABEL_DEFN(_label)) ",object\n"                \
    ".p2align " __xstring(CYGARC_P2ALIGNMENT) "\n"                           \
__xstring(CYG_LABEL_DEFN(_label)) ":\n"                                      \
    ".previous\n"                                                            \
       )
#endif

// This macro must be applied to any types whose objects are to be placed in
// tables
#ifndef CYG_HAL_TABLE_TYPE
#define CYG_HAL_TABLE_TYPE CYGBLD_ATTRIB_ALIGN( CYGARC_ALIGNMENT )
#endif

#ifndef CYG_HAL_TABLE_EXTRA
#define CYG_HAL_TABLE_EXTRA( _name ) \
        CYGBLD_ATTRIB_SECTION(".ecos.table." __xstring(_name) ".extra")
#endif

#ifndef CYG_HAL_TABLE_ENTRY
#define CYG_HAL_TABLE_ENTRY( _name ) \
        CYGBLD_ATTRIB_SECTION(".ecos.table." __xstring(_name) ".data") \
        CYGBLD_ATTRIB_USED
#endif

#ifndef CYG_HAL_TABLE_QUALIFIED_ENTRY
#define CYG_HAL_TABLE_QUALIFIED_ENTRY( _name, _qual ) \
        CYGBLD_ATTRIB_SECTION(".ecos.table." __xstring(_name) ".data." \
                              __xstring(_qual))                        \
        CYGBLD_ATTRIB_USED
#endif
#else

#ifndef CYG_HAL_TABLE_TYPE
#define CYG_HAL_TABLE_TYPE CYGBLD_ATTRIB_ALIGN( CYGARC_ALIGNMENT )
#endif

#define CYG_HAL_TABLE_ENTRY(x)
#define CYG_HAL_TABLE_BEGIN(a,b)
#define CYG_HAL_TABLE_END(a,b)

#define cyg_mtab_end cyg_mtab[CYGNUM_FILEIO_MTAB_MAX-1]

#define cyg_fstab_end cyg_fstab[CYGNUM_FILEIO_FSTAB_MAX-1]

#endif

#endif /* __LINUX_TYPES_H__ */

