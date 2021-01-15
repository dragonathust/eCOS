/*
 * Copyright (c) 2005, 2006
 *
 * James Hook (james@wmpp.com) 
 * Chris Zimman (chris@wmpp.com)
 *
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the University of California, Berkeley nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE REGENTS AND CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <ctype.h>
#include <stdlib.h>
#include <shell.h>
#include <shell_err.h>
#include <shell_thread.h>
#include <commands.h>

#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>//malloc

#include <cyg/fileio/fileio.h>
#include <cyg/fs/fatfs.h>
#include <cyg/infra/diag.h>            // HAL polled output

#define SHOW_RESULT( _fn, _res ) \
SHELL_PRINT("<FAIL>: " #_fn "() returned %d\n", _res);

#define SHELL_PATH_MAX 128

static char CurrentPath[SHELL_PATH_MAX];
static char HomePath[SHELL_PATH_MAX];
    
#define MY_PAGE_SIZE (64*1024)
#define IOSIZE MY_PAGE_SIZE

#define FILE_SIZE_MB 64

void SetHomePath(char *pName )
{
	strcpy(	CurrentPath,pName);
	strcpy(	HomePath,pName);
}

void ShowPrompt(char *pName )
{
    char CurrentPathTemp[128];

	sprintf(CurrentPathTemp,"%s",pName);
	strcat(CurrentPathTemp,CurrentPath);
	strcat(CurrentPathTemp,"> ");
	SHELL_PRINT("%s",CurrentPathTemp);
}

void list_dir( char *pName )
{
    int err;
    DIR *dirp;
    struct stat sbuf;
    int i;
	
    chdir(pName);				
    dirp = opendir( pName );

	if(dirp)
	{
    		for(;;)
    		{
        		struct dirent *entry = readdir( dirp );
        
       			 if( entry == NULL )
            		break;
		if( (strcmp( entry->d_name,".")!=0) && (strcmp( entry->d_name,"..")!=0) )
			{
			err = stat(entry->d_name, &sbuf );
		         if( err < 0 )
            		{
                			if( errno == ENOSYS )
                    		SHELL_PRINT(" <no status available>");
                			else
				//SHOW_RESULT( stat, err );
				SHELL_PRINT("%16ld %04d-%02d-%02d %s\n",
                            	0,0,0,0,entry->d_name);
            		}
            		else
            		{
/*
               			SHELL_PRINT("%16ld %04d-%02d-%02d %s\n",
                            	(long)sbuf.st_size,1980,1,1,entry->d_name);
*/
               			SHELL_PRINT("%16ld %04d-%02d-%02d ",
                            	(long)sbuf.st_size,1980,1,1);

			i=0;
  			do{
			diag_printf("%c",entry->d_name[i]);
			i++;
			}while(entry->d_name[i] != '\0');	
			diag_printf("\n");	
	
            		}
			}
			else
			{
				SHELL_PRINT("                            %s\n",entry->d_name);
			}
    		}

    	err = closedir( dirp );
    	if( err < 0 ) SHELL_PRINT("Close dir error=%d\n",err );	
	}
	else
	{
 	 SHELL_PRINT("Can't open directory %s.\n",pName);
	}
}


shell_cmd("ls",
	 "Show a list of directory",
	 "",
	 ls);

shell_cmd("pwd",
	 "Show current path",
	 "",
	 pwd);

shell_cmd("cd",
	 "Change path",
	 "",
	 cd);

shell_cmd("mkdir",
	 "Create dir",
	 "",
	 mk_dir);

shell_cmd("rm",
	 "Remove file",
	 "",
	 rm);

shell_cmd("cp",
	 "Copy file",
	 "",
	 cp);

shell_cmd("mv",
	 "Move file",
	 "",
	 mv);
	 
shell_cmd("cmp",
	 "Show different of two files",
	 "",
	 cmp);

shell_cmd("rd",
	 "File read speed test",
	 "",
	 rd);

shell_cmd("wr",
	 "File write speed test",
	 "",
	 wr);

shell_cmd("df",
	 "Get filesystem information",
	 "",
	 df);

CMD_DECL(ls)
{
    char CurrentPathTemp[SHELL_PATH_MAX];

    if( argc ==0 )
    {		
	list_dir(CurrentPath);  
    }
    else if( argc ==1 )
    {
    	sprintf(CurrentPathTemp,"%s/",CurrentPath);
    	list_dir(strcat(CurrentPathTemp,argv[0]));
    }
    else
    {
    	SHELL_PRINT("ls: not support operand.\n");
    }
    chdir(CurrentPath);
	
    return SHELL_OK;
}

CMD_DECL(pwd)
{
 	 SHELL_PRINT("%s\n",CurrentPath);  

    return SHELL_OK;
}

CMD_DECL(cd)
{
    int ret;
    char CurrentPathTemp[SHELL_PATH_MAX];
	
    if( argc ==0 )
    {		
	strcpy(CurrentPath,HomePath); 
    }
    else if( argc ==1 )
    {
	if( strcmp(argv[0],".") ==0 )
	{
		
	}
	else if( strcmp(argv[0],"..") ==0 )
	{
		strcpy(CurrentPath,HomePath); 
	}
	else
	{	
		strcpy(CurrentPathTemp,CurrentPath);
		strcat(CurrentPathTemp,"/");
		strcat(CurrentPathTemp,argv[0]);

	    	ret = access( CurrentPathTemp, F_OK );
    		if( ret !=0 )
    			{

			    	SHELL_PRINT("cd: no such file or directory: %s.\n",argv[0]);
    			}
		else
			{
				strcpy(CurrentPath,CurrentPathTemp);
			}
    	}
    }
    else
    {
    	SHELL_PRINT("cd: not support operand.\n");
    }

	chdir(CurrentPath);
	
    return SHELL_OK;
}

CMD_DECL(mk_dir)
{
	int ret;
		
    	if( argc !=1 )
    	{
    		SHELL_PRINT("mkdir: missing operand.\n");
    		return -1;
	}

	
 	ret=mkdir(argv[0],O_RDWR|O_CREAT);
	
	if(ret!=0)
	SHELL_PRINT("Can't create dir %s.\n",argv[0]);
	
    return SHELL_OK;
}

CMD_DECL(rm)
{
	int ret;
		
    	if( argc ==1 )
    	{
		ret=unlink(argv[0]);

	if(ret!=0)
	SHELL_PRINT("Can't delete file %s.\n",argv[0]);

	} 
	else
	if( argc == 2)
	{
		if( strcmp(argv[1],"-rf") ==0 )
		{
			ret=rmdir(argv[0]);

			if(ret!=0)
			SHELL_PRINT("Can't delete dir %s.\n",argv[0]);
		}
		else
    		SHELL_PRINT("rm: not support operand.\n");
	}
	else
	{
    		SHELL_PRINT("rm: missing operand.\n");
	}
    return SHELL_OK;
}

static void copyfile( char *name2,char *name1 )
{

    int err,i=0;
    char *buf1=NULL,*buf1_start;
    int fd1, fd2;
    ssize_t done, wrote;
	
    SHELL_PRINT("<INFO>: copy file %s -> %s\n\n",name2,name1);

	buf1=(char *)malloc(IOSIZE);
	//SHELL_PRINT("the copyfile malloc address is:0x%x\n",buf1);
	if(buf1==(char *)NULL)
	{
		SHELL_PRINT("In function copyfile: malloc buf1 is wrong\n");
		return;
	}
	buf1_start=buf1;
	
    err = access( name1, F_OK );
    if( err < 0 && errno != EACCES ) SHOW_RESULT( access, err );

    err = access( name2, F_OK );
    if( err != 0 ) SHOW_RESULT( access, err );
    
    fd1 = open( name1, O_WRONLY|O_CREAT );
    if( fd1 < 0 ) SHOW_RESULT( open, fd1 );

    fd2 = open( name2, O_RDONLY );
    if( fd2 < 0 ) SHOW_RESULT( open, fd2 );
    
    for(;;)
    {	
		buf1=buf1_start;
        done = read( fd2, buf1, IOSIZE );
        if( done < 0 ) SHOW_RESULT( read, (int)done );
		if( done == 0 ) break;
		buf1=buf1_start;
		wrote = write( fd1, buf1, done );
        if( wrote != done ) SHOW_RESULT( write, (int) wrote );
		if( wrote != done ) break;
		i++;
		if(i%20==0) 
		{
			SHELL_PRINT("#");
		}
		if(i%300==0)
			{
				//fsync(fd1);
				fcach_flush(fd1);
			}
		if(i%2000==0)
			{
				SHELL_PRINT("\n");
			}
    }
	fcach_flush(fd1);
	err = close( fd1 );
    if( err < 0 ) SHOW_RESULT( close, err );

    err = close( fd2 );
    if( err < 0 ) SHOW_RESULT( close, err );
	free(buf1);
    SHELL_PRINT("\n");
}

CMD_DECL(cp)
{
    	if( argc !=2 )
    	{
    		SHELL_PRINT("Usage: filename1 filename2.\n");
    		return -1;
	} 

	copyfile(argv[0],argv[1]);

    return SHELL_OK;
}

CMD_DECL(mv)
{
 	 SHELL_PRINT("%s\n",CurrentPath);  

    return SHELL_OK;
}

CMD_DECL(cmp)
{
	int fd1,fd2;
    	struct stat s1,s2;
    	int i,j;
    	int file_length;
    	int file_left;
	unsigned char *buf1;
	unsigned char *buf2;
    	
    	if( argc !=2 )
    	{
    		SHELL_PRINT("Usage: filename1 filename2.\n");
    		return -1;
	}

    	fd1=open(argv[0],O_RDONLY);
	if(fd1<=0)
	{      SHELL_PRINT("file not exist.\n");
		return -1;
	}	
	
    	fd2=open(argv[1],O_RDONLY);
	if(fd2<=0)
	{      SHELL_PRINT("file not exist.\n");
		close(fd1);
		return -1;
	}	

		SHELL_PRINT("open file [%s] ok!\n",argv[0]);
		fstat(fd1,&s1);

		SHELL_PRINT("size1=%d\n",s1.st_size);	

		SHELL_PRINT("open file [%s] ok!\n",argv[1]);
		fstat(fd2,&s2);

		SHELL_PRINT("size2=%d\n",s2.st_size);	
		
		if( s1.st_size != s2.st_size )
		{      SHELL_PRINT("file length not equal.\n");
			close(fd1);
			close(fd2);
			return -1;
		}
		
		buf1=(char *)malloc(MY_PAGE_SIZE);
		buf2=(char *)malloc(MY_PAGE_SIZE);
		if( (buf1==NULL)||(buf2==NULL) )
		{
			SHELL_PRINT("Out of memory.\n");
			close(fd1);
			close(fd2);
			return -1;
		}
		file_length=s1.st_size;
		for(i=0;i<file_length/MY_PAGE_SIZE;i++)
		{
			read(fd1,buf1,MY_PAGE_SIZE);
			read(fd2,buf2,MY_PAGE_SIZE);
			
			if( memcmp(buf1,buf2,MY_PAGE_SIZE) != 0 )
			{
				SHELL_PRINT("File is different!\n");
				for(j=0;j<MY_PAGE_SIZE;j++)
				{
				if( buf1[j]!=buf2[j] )
				SHELL_PRINT("byte1=0x%02x,byte2=0x%02x\n",buf1[j],buf2[j]);
				}
			goto END;
			}	
		}
		
		file_left=file_length%MY_PAGE_SIZE;
		read(fd1,buf1,file_left);
		read(fd2,buf2,file_left);
		
		if( memcmp(buf1,buf2,file_left) != 0 )
		{
				SHELL_PRINT("File is different!\n");
				for(j=0;j<file_left;j++)
				{
				if( buf1[j]!=buf2[j] )
				SHELL_PRINT("byte1=0x%02x,byte2=0x%02x\n",buf1[j],buf2[j]);
				}
		goto END;
		}	
				
		SHELL_PRINT("File is same!\n");	

END:		
		free(buf1);
		free(buf2);		
		close(fd1);
		close(fd2);

    return SHELL_OK;
}

CMD_DECL(rd)
{
	int fd;
    	struct stat s;
    	int i;
    	int file_length;
    	int file_left;
//    	struct timeval tv;
    	unsigned int time1,time2;
	unsigned char *buf1;
	    	    	
    	if( argc !=1 )
    	{
    		printf("Usage: read filename.\n");
    		return -1;
	}

    	fd=open(argv[0],O_RDONLY);
	if(fd >0)
	{
		printf("open file [%s] ok!\n",argv[0]);
		fstat(fd,&s);

		printf("size=%d\n",s.st_size);	

		buf1=(char *)malloc(MY_PAGE_SIZE);
		if( buf1==NULL )
		{
			SHELL_PRINT("Out of memory.\n");
			close(fd);
			return -1;
		}
				
		//gettimeofday(&tv, NULL);
		//time1=tv.tv_sec*1000 + tv.tv_usec/1000;
		time1=0;
		StartTimer();
		
		file_length=s.st_size;
		for(i=0;i<file_length/MY_PAGE_SIZE;i++)
		{
			read(fd,buf1,MY_PAGE_SIZE);
	
		}

		file_left=file_length%MY_PAGE_SIZE;
		if( file_left!=0 )
		read(fd,buf1,file_left);
		
		//gettimeofday(&tv, NULL);
		//time2=tv.tv_sec*1000 + tv.tv_usec/1000;
		time2= GetTimerMSec();	// Get timer value in second

		free(buf1);	
		close(fd);
		
		printf("Read Speed=%dKB/S\n",(file_length/1024)*1000/(time2-time1));
	}
	else
	{      printf("file not exist.\n");
		return -1;
	}	

    return SHELL_OK;
}

CMD_DECL(wr)
{
	int fd;
    	struct stat s;
    	int i;
    	int file_length;
    	int file_left;
    	//struct timeval tv;
    	unsigned int time1,time2;
	unsigned char *buf1;
	    		
    	if( argc !=1 )
    	{
    		printf("Usage: write filename.\n");
    		return -1;
	}

    	fd=open(argv[0],O_CREAT|O_WRONLY);
	if(fd >0)
	{
		printf("open file [%s] ok!\n",argv[0]);

		buf1=(char *)malloc(MY_PAGE_SIZE);
		if( buf1==NULL )
		{
			SHELL_PRINT("Out of memory.\n");
			close(fd);
			return -1;
		}
		
		//gettimeofday(&tv, NULL);
		//time1=tv.tv_sec*1000 + tv.tv_usec/1000;
		time1=0;
		StartTimer();
		
		for(i=0;i<16*FILE_SIZE_MB;i++)
		{
			write(fd,buf1,MY_PAGE_SIZE);
	
		}
		//gettimeofday(&tv, NULL);
		//time2=tv.tv_sec*1000 + tv.tv_usec/1000;
		time2= GetTimerMSec();	// Get timer value in second

		free(buf1);	
		close(fd);
		
		printf("Write Speed=%dKB/S\n",FILE_SIZE_MB*1024*1000/(time2-time1));
	}
	else
	{      printf("file not exist.\n");
		return -1;
	}	

    return SHELL_OK;
}

CMD_DECL(df)
{
	int err,free_size,total_size,used_size,percent;
	struct cyg_fs_disk_usage usage;
		
	chdir( HomePath );	
	err = cyg_fs_getinfo(HomePath, FS_INFO_DISK_USAGE, &usage, sizeof(usage));
	if( err < 0 )
	{	SHOW_RESULT( cyg_fs_getinfo, err );
		free_size=0;
		total_size=0;
		percent=0;
	}
	free_size=(usage.free_blocks)*(usage.block_size)/1024/1024;
	total_size=usage.total_blocks*usage.block_size/1024/1024;
	used_size=total_size-free_size;

	if(total_size!=0) percent=(used_size*100)/total_size;
		
	printf("Filesystem\t Size    Used    Avail    Use%%        Mounted on\n");
	printf("%s\t %dMB  %dMB  %dMB  %d%%\n",HomePath,total_size,used_size,free_size,percent);

	chdir( CurrentPath );	
}



