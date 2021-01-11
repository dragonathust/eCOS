#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <linux/types.h>
#include <linux/fcntl.h>   
#include <linux/stat.h>
#include <linux/fileio.h>
#include <linux/dirent.h>

#define OS_PRINTF mtos_printk
#define diag_printk OS_PRINTF

#define CYG_TEST_INIT() OS_PRINTF("Test Init...\n")
#define CYG_TEST_FAIL(x) OS_PRINTF("Test Fail: "x)
#define CYG_TEST_PASS_FINISH(x) OS_PRINTF("Test Pass: "x)

#define JFFS2_TEST_DEV "/dev/flash"

int jffs2_main(int argc,char *argv[])
{
	int i;
	int ret;
	int fd;
	int erase=0;
	
	for(i=0; i<argc; i++)
	{
		OS_PRINTF("argv[%d] %s\n", i, argv[i]);
	}

	if(argc ==2)
	erase=atoi(argv[1]);

	jffs2_init(erase);

	OS_PRINTF("Fileio Init...\n");
	
	jffs2_test1_main(argc,argv);

	jffs2_test3_main(argc,argv);
}

//==========================================================================

#define SHOW_RESULT( _fn, _res ) \
diag_printk("<FAIL>: " #_fn "() returned %ld %s\n",  \
            (unsigned long)_res, _res<0?strerror(errno):"");

//==========================================================================

#define IOSIZE  1000

#define LONGNAME1       "long_file_name_that_should_take_up_more_than_one_directory_entry_1"
#define LONGNAME2       "long_file_name_that_should_take_up_more_than_one_directory_entry_2"


//==========================================================================

#ifndef CYGPKG_LIBC_STRING

char *strcat( char *s1, const char *s2 )
{
    char *s = s1;
    while( *s1 ) s1++;
    while( (*s1++ = *s2++) != 0);
    return s;
}

#endif

//==========================================================================

static void listdir( char *name, int statp, int numexpected, int *numgot )
{
    int err;
    DIR *dirp;
    int num=0;
    
    diag_printk("<INFO>: reading directory %s\n",name);
    
    dirp = OPENDIR( name );
    if( dirp == NULL ) SHOW_RESULT( opendir, -1 );

    for(;;)
    {
        struct dirent *entry = READDIR( dirp );
        
        if( entry == NULL )
            break;
        num++;
        diag_printk("<INFO>: entry %14s",entry->d_name);
#ifdef CYGPKG_FS_JFFS2_RET_DIRENT_DTYPE
        diag_printk(" d_type %2d", entry->d_type);
#endif
        if( statp )
        {
            char fullname[PATH_MAX];
            struct stat sbuf;

            if( name[0] )
            {
                strcpy(fullname, name );
                if( !(name[0] == '/' && name[1] == 0 ) )
                    strcat(fullname, "/" );
            }
            else fullname[0] = 0;
            
            strcat(fullname, entry->d_name );
            
            err = STAT( fullname, &sbuf );
            if( err < 0 )
            {
                if( errno == ENOSYS )
                    diag_printk(" <no status available>");
                else SHOW_RESULT( stat, err );
            }
            else
            {
                diag_printk(" [mode %08x ino %08x nlink %d size %ld]",
                            sbuf.st_mode,sbuf.st_ino,sbuf.st_nlink,
                            (unsigned long) sbuf.st_size);
            }
#ifdef CYGPKG_FS_JFFS2_RET_DIRENT_DTYPE
            if ((entry->d_type & S_IFMT) != (sbuf.st_mode & S_IFMT))
              CYG_TEST_FAIL("File mode's don't match between dirent and stat");
#endif
        }

        diag_printk("\n");
    }

    err = CLOSEDIR( dirp );
    if( err < 0 ) SHOW_RESULT( stat, err );
    if (numexpected >= 0 && num != numexpected)
        CYG_TEST_FAIL("Wrong number of dir entries\n");
    if ( numgot != NULL )
        *numgot = num;
}

//==========================================================================

static void createfile( char *name, size_t size )
{
    unsigned char buf[IOSIZE];
    int fd;
    ssize_t wrote;
    int i;
    int err;

    diag_printk("<INFO>: create file %s size %d\n",name,size);

    err = ACCESS( name, F_OK );
    if( err < 0 && errno != EACCES ) SHOW_RESULT( access, err );
    
    for( i = 0; i < IOSIZE; i++ ) buf[i] = i%256;
 
    fd = OPEN( name, O_WRONLY|O_CREAT );
    if( fd < 0 ) SHOW_RESULT( open, fd );

    while( size > 0 )
    {
        ssize_t len = size;
        if ( len > IOSIZE ) len = IOSIZE;
        
        wrote = WRITE( fd, buf, len );
        if( wrote != len ) SHOW_RESULT( write, wrote );        

        size -= wrote;
    }

    err = CLOSE( fd );
    if( err < 0 ) SHOW_RESULT( close, err );
}

//==========================================================================

static void checkfile( char *name )
{
    unsigned char buf[IOSIZE];
    int fd;
    ssize_t done;
    int i;
    int err;
    off_t pos = 0;

    diag_printk("<INFO>: check file %s\n",name);
    
    err = ACCESS( name, F_OK );
    if( err != 0 ) SHOW_RESULT( access, err );

    fd = OPEN( name, O_RDONLY );
    if( fd < 0 ) SHOW_RESULT( open, fd );

    for(;;)
    {
        done = READ( fd, buf, IOSIZE );
        if( done < 0 ) SHOW_RESULT( read, done );

        if( done == 0 ) break;

        for( i = 0; i < done; i++ )
            if( buf[i] != i%256 )
            {
                diag_printk("buf[%ld+%d](%02x) != %02x\n",
                            (unsigned long)pos,i,buf[i],i%256);
                CYG_TEST_FAIL("Data read not equal to data written\n");
            }
        
        pos += done;
    }

    err = CLOSE( fd );
    if( err < 0 ) SHOW_RESULT( close, err );
}

//==========================================================================

static void copyfile( char *name2, char *name1 )
{

    int err;
    char buf[IOSIZE];
    int fd1, fd2;
    ssize_t done, wrote;

    diag_printk("<INFO>: copy file %s -> %s\n",name2,name1);

    err = ACCESS( name1, F_OK );
    if( err < 0 && errno != EACCES ) SHOW_RESULT( access, err );

    err = ACCESS( name2, F_OK );
    if( err != 0 ) SHOW_RESULT( access, err );
    
    fd1 = OPEN( name1, O_WRONLY|O_CREAT );
    if( fd1 < 0 ) SHOW_RESULT( open, fd1 );

    fd2 = OPEN( name2, O_RDONLY );
    if( fd2 < 0 ) SHOW_RESULT( open, fd2 );
    
    for(;;)
    {
        done = READ( fd2, buf, IOSIZE );
        if( done < 0 ) SHOW_RESULT( read, done );

        if( done == 0 ) break;

        wrote = WRITE( fd1, buf, done );
        if( wrote != done ) SHOW_RESULT( write, wrote );

        if( wrote != done ) break;
    }

    err = CLOSE( fd1 );
    if( err < 0 ) SHOW_RESULT( close, err );

    err = CLOSE( fd2 );
    if( err < 0 ) SHOW_RESULT( close, err );
    
}

//==========================================================================

static void comparefiles( char *name2, char *name1 )
{
    int err;
    char buf1[IOSIZE];
    char buf2[IOSIZE];
    int fd1, fd2;
    ssize_t done1, done2;
    int i;

    diag_printk("<INFO>: compare files %s == %s\n",name2,name1);

    err = ACCESS( name1, F_OK );
    if( err != 0 ) SHOW_RESULT( access, err );

    err = ACCESS( name1, F_OK );
    if( err != 0 ) SHOW_RESULT( access, err );
    
    fd1 = OPEN( name1, O_RDONLY );
    if( fd1 < 0 ) SHOW_RESULT( open, fd1 );

    fd2 = OPEN( name2, O_RDONLY );
    if( fd2 < 0 ) SHOW_RESULT( open, fd2 );
    
    for(;;)
    {
        done1 = READ( fd1, buf1, IOSIZE );
        if( done1 < 0 ) SHOW_RESULT( read, done1 );

        done2 = READ( fd2, buf2, IOSIZE );
        if( done2 < 0 ) SHOW_RESULT( read, done2 );

        if( done1 != done2 )
            diag_printk("Files different sizes\n");
        
        if( done1 == 0 ) break;

        for( i = 0; i < done1; i++ )
            if( buf1[i] != buf2[i] )
            {
                diag_printk("buf1[%d](%02x) != buf1[%d](%02x)\n",i,buf1[i],i,buf2[i]);
                CYG_TEST_FAIL("Data in files not equal\n");
            }
    }

    err = CLOSE( fd1 );
    if( err < 0 ) SHOW_RESULT( close, err );

    err = CLOSE( fd2 );
    if( err < 0 ) SHOW_RESULT( close, err );
    
}

//==========================================================================

void checkcwd( const char *cwd )
{
    static char cwdbuf[PATH_MAX];
    char *ret;

    ret = GETCWD( cwdbuf, sizeof(cwdbuf));
    if( ret == NULL ) SHOW_RESULT( getcwd, ret );    

    if( strcmp( cwdbuf, cwd ) != 0 )
    {
        diag_printk( "cwdbuf %s cwd %s\n",cwdbuf, cwd );
        CYG_TEST_FAIL( "Current directory mismatch");
    }
}

//==========================================================================
// main

int jffs2_test1_main( int argc, char **argv )
{
    int err;
    int i;
    int existingdirents=-1;

    CYG_TEST_INIT();

    // --------------------------------------------------------------

    err = mount( JFFS2_TEST_DEV, "/", "jffs2" );
    if( err < 0 ) SHOW_RESULT( mount, err );    

for(i=0;i<1;i++)
{
    diag_printk("Test times=%d\n",i);
	
    err = CHDIR( "/" );
    if( err < 0 ) SHOW_RESULT( chdir, err );

    checkcwd( "/" );
    
    listdir( "/", true, -1, &existingdirents );
    if ( existingdirents < 2 )
        CYG_TEST_FAIL("Not enough dir entries\n");

    // --------------------------------------------------------------

    createfile( "/foo", 202 );
    checkfile( "foo" );
    copyfile( "foo", "fee");
    checkfile( "fee" );
    comparefiles( "foo", "/fee" );
    diag_printk("<INFO>: mkdir bar\n");
    err = MKDIR( "/bar", 0 );
    if( err < 0 ) SHOW_RESULT( mkdir, err );

    listdir( "/" , true, existingdirents+3, NULL );

    copyfile( "fee", "/bar/fum" );
    checkfile( "bar/fum" );
    comparefiles( "/fee", "bar/fum" );

    diag_printk("<INFO>: cd bar\n");
    err = CHDIR( "bar" );
    if( err < 0 ) SHOW_RESULT( chdir, err );

    checkcwd( "/bar" );
    
    diag_printk("<INFO>: rename /foo bundy\n");    
    err = RENAME( "/foo", "bundy" );
    if( err < 0 ) SHOW_RESULT( rename, err );
    
    listdir( "/", true, existingdirents+2, NULL );
    listdir( "" , true, 4, NULL );

    checkfile( "/bar/bundy" );
    comparefiles("/fee", "bundy" );

    // --------------------------------------------------------------

    createfile( LONGNAME1, 123 );
    checkfile( LONGNAME1 );
    copyfile( LONGNAME1, LONGNAME2 );

    listdir( "", false, 6, NULL );
    
    diag_printk("<INFO>: unlink " LONGNAME1 "\n");    
    err = UNLINK( LONGNAME1 );
    if( err < 0 ) SHOW_RESULT( unlink, err );

    diag_printk("<INFO>: unlink " LONGNAME2 "\n");    
    err = UNLINK( LONGNAME2 );
    if( err < 0 ) SHOW_RESULT( unlink, err );
    
    
    // --------------------------------------------------------------

    diag_printk("<INFO>: unlink fee\n");    
    err = UNLINK( "/fee" );
    if( err < 0 ) SHOW_RESULT( unlink, err );

    diag_printk("<INFO>: unlink fum\n");        
    err = UNLINK( "fum" );
    if( err < 0 ) SHOW_RESULT( unlink, err );

    diag_printk("<INFO>: unlink /bar/bundy\n");        
    err = UNLINK( "/bar/bundy" );
    if( err < 0 ) SHOW_RESULT( unlink, err );

    diag_printk("<INFO>: cd /\n");        
    err = CHDIR( "/" );
    if( err < 0 ) SHOW_RESULT( chdir, err );

    checkcwd( "/" );
    
    diag_printk("<INFO>: rmdir /bar\n");        
    err = RMDIR( "/bar" );
    if( err < 0 ) SHOW_RESULT( rmdir, err );
    
    listdir( "/", false, existingdirents, NULL );
}
    // --------------------------------------------------------------
#if 0
    diag_printk("<INFO>: mount /jffs2 \n");
    err = mount( JFFS2_TEST_DEV, "/jffs2", "jffs2" );
    if( err < 0 ) SHOW_RESULT( mount, err );    

    createfile( "/jffs2/tinky", 456 );
    copyfile( "/jffs2/tinky", "/jffs2/laalaa" );
    checkfile( "/jffs2/tinky");
    checkfile( "/jffs2/laalaa");
    comparefiles( "/jffs2/tinky", "/jffs2/laalaa" );

    diag_printk("<INFO>: cd /jffs2\n");    
    err = CHDIR( "/jffs2" );
    if( err < 0 ) SHOW_RESULT( chdir, err );

    checkcwd( "/jffs2" );
        
    diag_printk("<INFO>: mkdir noonoo\n");    
    err = MKDIR( "noonoo", 0 );
    if( err < 0 ) SHOW_RESULT( mkdir, err );

    listdir( "." , true, existingdirents+3, NULL);

    diag_printk("<INFO>: cd noonoo\n");
    err = CHDIR( "noonoo" );
    if( err < 0 ) SHOW_RESULT( chdir, err );

    checkcwd( "/jffs2/noonoo" );
    
    createfile( "tinky", 678 );
    checkfile( "tinky" );

    createfile( "dipsy", 3456 );
    checkfile( "dipsy" );
    copyfile( "dipsy", "po" );
    checkfile( "po" );
    comparefiles( "dipsy", "po" );


    /*for(i=0;i<2048;i++) {
        diag_printk("<INFO>: churningchurningchurning................................ITERATION = %d\n", i);    
        createfile( "churningchurningchurning", 4096 );
        diag_printk("<INFO>: unlink churningchurningchurning\n");    
        err = unlink( "churningchurningchurning" );
        if( err < 0 ) SHOW_RESULT( unlink, err );
    }*/


    listdir( ".", true, 5, NULL );
    listdir( "", true, 5, NULL );
    listdir( "..", true, existingdirents+3, NULL );

    // --------------------------------------------------------------

    diag_printk("<INFO>: unlink tinky\n");    
    err = UNLINK( "tinky" );
    if( err < 0 ) SHOW_RESULT( unlink, err );

    diag_printk("<INFO>: unlink dipsy\n");    
    err = UNLINK( "dipsy" );
    if( err < 0 ) SHOW_RESULT( unlink, err );

    diag_printk("<INFO>: unlink po\n");    
    err = UNLINK( "po" );
    if( err < 0 ) SHOW_RESULT( unlink, err );

    diag_printk("<INFO>: cd ..\n"); 
    err = CHDIR( ".." );
    if( err < 0 ) SHOW_RESULT( chdir, err );
    checkcwd( "/jffs2" );
    
    diag_printk("<INFO>: rmdir noonoo\n"); 
    err = RMDIR( "noonoo" );
    if( err < 0 ) SHOW_RESULT( rmdir, err );

    // --------------------------------------------------------------

    err = MKDIR( "x", 0 );
    if( err < 0 ) SHOW_RESULT( mkdir, err );
    
    err = MKDIR( "x/y", 0 );
    if( err < 0 ) SHOW_RESULT( mkdir, err );
    
    err = MKDIR( "x/y/z", 0 );
    if( err < 0 ) SHOW_RESULT( mkdir, err );

    err = MKDIR( "x/y/z/w", 0 );
    if( err < 0 ) SHOW_RESULT( mkdir, err );
    
    diag_printk("<INFO>: cd /jffs2/x/y/z/w\n");
    err = CHDIR( "/jffs2/x/y/z/w" );
    if( err < 0 ) SHOW_RESULT( chdir, err );
    checkcwd( "/jffs2/x/y/z/w" );

    diag_printk("<INFO>: cd ..\n");
    err = CHDIR( ".." );
    if( err < 0 ) SHOW_RESULT( chdir, err );
    checkcwd( "/jffs2/x/y/z" );
    
    diag_printk("<INFO>: cd .\n");
    err = CHDIR( "." );
    if( err < 0 ) SHOW_RESULT( chdir, err );
    checkcwd( "/jffs2/x/y/z" );

    diag_printk("<INFO>: cd ../../y\n");
    err = CHDIR( "../../y" );
    if( err < 0 ) SHOW_RESULT( chdir, err );
    checkcwd( "/jffs2/x/y" );

    diag_printk("<INFO>: cd ../..\n");
    err = CHDIR( "../.." );
    if( err < 0 ) SHOW_RESULT( chdir, err );
    checkcwd( "/jffs2" );

    diag_printk("<INFO>: rmdir x/y/z/w\n"); 
    err = RMDIR( "x/y/z/w" );
    if( err < 0 ) SHOW_RESULT( RMDIR, err );

    diag_printk("<INFO>: RMDIR x/y/z\n"); 
    err = RMDIR( "x/y/z" );
    if( err < 0 ) SHOW_RESULT( RMDIR, err );

    diag_printk("<INFO>: RMDIR x/y\n"); 
    err = RMDIR( "x/y" );
    if( err < 0 ) SHOW_RESULT( RMDIR, err );

    diag_printk("<INFO>: RMDIR x\n"); 
    err = RMDIR( "x" );
    if( err < 0 ) SHOW_RESULT( RMDIR, err );
    
    // --------------------------------------------------------------
    
    diag_printk("<INFO>: unlink tinky\n");    
    err = UNLINK( "tinky" );
    if( err < 0 ) SHOW_RESULT( unlink, err );

    diag_printk("<INFO>: unlink laalaa\n");    
    err = UNLINK( "laalaa" );
    if( err < 0 ) SHOW_RESULT( unlink, err );

    diag_printk("<INFO>: cd /\n");    
    err = CHDIR( "/" );
    if( err < 0 ) SHOW_RESULT( chdir, err );
    checkcwd( "/" );
    
    diag_printk("<INFO>: umount /jffs2\n");    
    err = umount( "/jffs2" );
    if( err < 0 ) SHOW_RESULT( umount, err );    
    
#ifdef CYGDAT_IO_FLASH_BLOCK_DEVICE_NAME_2
    diag_printk("<INFO>: mounting second JFFS2 filesystem on /mnt\n");
    
    err = mount( CYGDAT_IO_FLASH_BLOCK_DEVICE_NAME_2, "/mnt", "jffs2" );
    if( err < 0 ) SHOW_RESULT( mount, err );    

    err = CHDIR( "/" );
    if( err < 0 ) SHOW_RESULT( chdir, err );

    checkcwd( "/" );
    
    listdir( "/", true, -1, &existingdirents );
    if ( existingdirents < 2 )
        CYG_TEST_FAIL("Not enough dir entries\n");

    listdir( "/mnt", true, -1, &existingdirents );
    if ( existingdirents < 2 )
        CYG_TEST_FAIL("Not enough dir entries\n");

    diag_printk("<INFO>: umount /mnt\n");    
    err = umount( "/mnt" );
#endif
#endif
    diag_printk("<INFO>: umount /\n");    
    err = umount( "/" );
    if( err < 0 ) SHOW_RESULT( umount, err );    
    
    CYG_TEST_PASS_FINISH("jffs2_1\n");
}

// -------------------------------------------------------------------------

#define CYG_TEST_INFO OS_PRINTF
#define ITERATIONS 10000
#define NELEM(_x_) (sizeof(_x_)/sizeof(*(_x_)))

// Compute a CRC, using the POSIX 1003 definition

// Same basic algorithm as CRC-16, but the bits are defined in the
// opposite order.  This computation matches the output of the
// Linux 'cksum' program.

static const cyg_uint32 posix_crc32_tab[] = {
  0x00000000, 0x04C11DB7, 0x09823B6E, 0x0D4326D9, 0x130476DC, 0x17C56B6B, 0x1A864DB2, 0x1E475005, 
  0x2608EDB8, 0x22C9F00F, 0x2F8AD6D6, 0x2B4BCB61, 0x350C9B64, 0x31CD86D3, 0x3C8EA00A, 0x384FBDBD,
  0x4C11DB70, 0x48D0C6C7, 0x4593E01E, 0x4152FDA9, 0x5F15ADAC, 0x5BD4B01B, 0x569796C2, 0x52568B75, 
  0x6A1936C8, 0x6ED82B7F, 0x639B0DA6, 0x675A1011, 0x791D4014, 0x7DDC5DA3, 0x709F7B7A, 0x745E66CD, 
  0x9823B6E0, 0x9CE2AB57, 0x91A18D8E, 0x95609039, 0x8B27C03C, 0x8FE6DD8B, 0x82A5FB52, 0x8664E6E5, 
  0xBE2B5B58, 0xBAEA46EF, 0xB7A96036, 0xB3687D81, 0xAD2F2D84, 0xA9EE3033, 0xA4AD16EA, 0xA06C0B5D, 
  0xD4326D90, 0xD0F37027, 0xDDB056FE, 0xD9714B49, 0xC7361B4C, 0xC3F706FB, 0xCEB42022, 0xCA753D95,
  0xF23A8028, 0xF6FB9D9F, 0xFBB8BB46, 0xFF79A6F1, 0xE13EF6F4, 0xE5FFEB43, 0xE8BCCD9A, 0xEC7DD02D, 
  0x34867077, 0x30476DC0, 0x3D044B19, 0x39C556AE, 0x278206AB, 0x23431B1C, 0x2E003DC5, 0x2AC12072, 
  0x128E9DCF, 0x164F8078, 0x1B0CA6A1, 0x1FCDBB16, 0x018AEB13, 0x054BF6A4, 0x0808D07D, 0x0CC9CDCA, 
  0x7897AB07, 0x7C56B6B0, 0x71159069, 0x75D48DDE, 0x6B93DDDB, 0x6F52C06C, 0x6211E6B5, 0x66D0FB02, 
  0x5E9F46BF, 0x5A5E5B08, 0x571D7DD1, 0x53DC6066, 0x4D9B3063, 0x495A2DD4, 0x44190B0D, 0x40D816BA,
  0xACA5C697, 0xA864DB20, 0xA527FDF9, 0xA1E6E04E, 0xBFA1B04B, 0xBB60ADFC, 0xB6238B25, 0xB2E29692, 
  0x8AAD2B2F, 0x8E6C3698, 0x832F1041, 0x87EE0DF6, 0x99A95DF3, 0x9D684044, 0x902B669D, 0x94EA7B2A, 
  0xE0B41DE7, 0xE4750050, 0xE9362689, 0xEDF73B3E, 0xF3B06B3B, 0xF771768C, 0xFA325055, 0xFEF34DE2, 
  0xC6BCF05F, 0xC27DEDE8, 0xCF3ECB31, 0xCBFFD686, 0xD5B88683, 0xD1799B34, 0xDC3ABDED, 0xD8FBA05A, 
  0x690CE0EE, 0x6DCDFD59, 0x608EDB80, 0x644FC637, 0x7A089632, 0x7EC98B85, 0x738AAD5C, 0x774BB0EB,
  0x4F040D56, 0x4BC510E1, 0x46863638, 0x42472B8F, 0x5C007B8A, 0x58C1663D, 0x558240E4, 0x51435D53, 
  0x251D3B9E, 0x21DC2629, 0x2C9F00F0, 0x285E1D47, 0x36194D42, 0x32D850F5, 0x3F9B762C, 0x3B5A6B9B, 
  0x0315D626, 0x07D4CB91, 0x0A97ED48, 0x0E56F0FF, 0x1011A0FA, 0x14D0BD4D, 0x19939B94, 0x1D528623, 
  0xF12F560E, 0xF5EE4BB9, 0xF8AD6D60, 0xFC6C70D7, 0xE22B20D2, 0xE6EA3D65, 0xEBA91BBC, 0xEF68060B, 
  0xD727BBB6, 0xD3E6A601, 0xDEA580D8, 0xDA649D6F, 0xC423CD6A, 0xC0E2D0DD, 0xCDA1F604, 0xC960EBB3,
  0xBD3E8D7E, 0xB9FF90C9, 0xB4BCB610, 0xB07DABA7, 0xAE3AFBA2, 0xAAFBE615, 0xA7B8C0CC, 0xA379DD7B, 
  0x9B3660C6, 0x9FF77D71, 0x92B45BA8, 0x9675461F, 0x8832161A, 0x8CF30BAD, 0x81B02D74, 0x857130C3, 
  0x5D8A9099, 0x594B8D2E, 0x5408ABF7, 0x50C9B640, 0x4E8EE645, 0x4A4FFBF2, 0x470CDD2B, 0x43CDC09C, 
  0x7B827D21, 0x7F436096, 0x7200464F, 0x76C15BF8, 0x68860BFD, 0x6C47164A, 0x61043093, 0x65C52D24, 
  0x119B4BE9, 0x155A565E, 0x18197087, 0x1CD86D30, 0x029F3D35, 0x065E2082, 0x0B1D065B, 0x0FDC1BEC,
  0x3793A651, 0x3352BBE6, 0x3E119D3F, 0x3AD08088, 0x2497D08D, 0x2056CD3A, 0x2D15EBE3, 0x29D4F654, 
  0xC5A92679, 0xC1683BCE, 0xCC2B1D17, 0xC8EA00A0, 0xD6AD50A5, 0xD26C4D12, 0xDF2F6BCB, 0xDBEE767C, 
  0xE3A1CBC1, 0xE760D676, 0xEA23F0AF, 0xEEE2ED18, 0xF0A5BD1D, 0xF464A0AA, 0xF9278673, 0xFDE69BC4, 
  0x89B8FD09, 0x8D79E0BE, 0x803AC667, 0x84FBDBD0, 0x9ABC8BD5, 0x9E7D9662, 0x933EB0BB, 0x97FFAD0C, 
  0xAFB010B1, 0xAB710D06, 0xA6322BDF, 0xA2F33668, 0xBCB4666D, 0xB8757BDA, 0xB5365D03, 0xB1F740B4
   };

cyg_uint32 
cyg_posix_crc32(unsigned char *s, int len)
{
  int i;
  cyg_uint32 crc32val;
  unsigned long length;
  
  crc32val = 0;
  for (i = 0;  i < len;  i++) {
      crc32val = (crc32val << 8) ^ posix_crc32_tab[((crc32val >> 24) ^ *s++) & 0xFF];
  }
  length = len;
  while (length > 0) {
      crc32val = (crc32val << 8) ^ posix_crc32_tab[((crc32val >> 24) ^ length) & 0xFF];
      length >>= 8;
  }
  crc32val = ~crc32val;
  return crc32val;
}

//-----------------------------------------------------------------------------
// Simple random number generator

static cyg_uint32 RAND(void)
{
    static cyg_uint32 seed;
    
    seed = (seed * 1103515245) + 12345; // permutate seed
    
    return seed;
}
 

//==========================================================================
// file creation, deletion and testing functions

static void create_file(int i)
{
  cyg_int32 buffer[1020];
  char name[16];
  cyg_uint32 j;
  int fd, err;
  
  sprintf(name,"test%07d",i);

  fd = CREAT(name, S_IRWXU);
  if (fd == -1) SHOW_RESULT( creat, fd );
  
  for (j=1; j < NELEM(buffer); j++) {
    buffer[j] = RAND();
  }
  
  buffer[0] = 0;
  buffer[0] = cyg_posix_crc32((unsigned char *)buffer, sizeof(buffer));
  
  err = WRITE(fd, buffer, sizeof(buffer));
  if (err == -1) SHOW_RESULT( write, err );
  
  err = CLOSE(fd);
  if (err == -1) SHOW_RESULT( close, err );
}

static void delete_file(int i)
{
  char name[16];
  int err;
  
  sprintf(name,"test%07d",i);

  err = UNLINK(name);
  if (err == -1) SHOW_RESULT( unlink, err );
}

static void check_file(int i)
{
  char name[16];
  int err, fd;
  cyg_int32 buffer[1020];
  cyg_uint32 crc;
  
  sprintf(name,"test%07d",i);

  fd = OPEN(name, O_RDONLY);
  if (fd == -1) SHOW_RESULT( open, fd );
  
  err = READ(fd, buffer, sizeof(buffer));
  if (err == -1) SHOW_RESULT( read, fd );
  
  crc = buffer[0];
  buffer[0] = 0;
  
  if (crc != cyg_posix_crc32((unsigned char *)buffer, sizeof(buffer))) {
    CYG_TEST_FAIL("File corrupt");
  }

  err = CLOSE(fd);
  if (err == -1) SHOW_RESULT( read, fd );
}


//==========================================================================
// main

int jffs2_test3_main( int argc, char **argv )
{
    int err, iteration;
   // struct mallinfo minfo;
    CYG_TEST_INIT();

    // --------------------------------------------------------------

    CYG_TEST_INFO("mount /");      
    err = mount( JFFS2_TEST_DEV, "/", "jffs2" );
    if( err < 0 ) SHOW_RESULT( mount, err );    
    
    CHDIR ("/");
    
    iteration=0;
    create_file(iteration);
    while (iteration < ITERATIONS) {
      if (!(iteration % 1000)) {
	  	/*
        minfo = mallinfo();
        diag_printk("<INFO> Iteration %07d fordblks = %7d\n", 
                    iteration, minfo.fordblks);
              */
                   diag_printk("<INFO> Iteration %07d \n", 
                    iteration);   
      }
      iteration++;
      create_file(iteration);
      check_file(iteration-1);
      delete_file(iteration-1);
      check_file(iteration);
    }
    
    CYG_TEST_INFO("umount /");
    err = umount( "/" );
    if( err < 0 ) SHOW_RESULT( umount, err );    
    
    CYG_TEST_PASS_FINISH("jffs2_3\n");
}


