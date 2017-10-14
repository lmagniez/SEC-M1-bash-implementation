#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>


#define syserror(x) perror(errormsg[x])
#define ROUGE(m) "\033[01;31m"m"\033[0m"
#define VERT(m) "\033[01;32m"m"\033[0m"

#define MAX_PATH 10

char *errormsg[]={
	"No error",//0
	ROUGE("Invalid options"),//1
	ROUGE("Problem with opendir"),//2
	
	
};



void list_file(char *path){
	
	DIR* directory;
	struct dirent* readen_file;
	printf("ls -l %s \n",path);
	
	directory = opendir(path);
	if(!directory)syserror(2);
	
	while(readen_file=readdir(directory)){
		
		char *filename;//
		char *fullpath;//
		char* path_tmp;
		int file_type;
		int uid;//
		char *username;
		int gid;//
		char *groupname;
		
		int nb_hard_link;		
		int file_size;
		
		char* modif_month;
		int modif_day;
		char* modif_time;
		
		struct stat *buf_stat;
		
		int fullpath_size;
		
		
		//add a slash to the path if necessary
		if(path[strlen(path)-1]!='/'){
			path_tmp = malloc(sizeof(char)*(strlen(path)+1));
			strcpy(path_tmp, path);
			path_tmp[strlen(path_tmp)-1]='/'; 
		}
		
		//Get the full path
		filename = readen_file->d_name;
		fullpath_size = strlen(path_tmp) + strlen(filename);
		
		fullpath = malloc(sizeof(char)*fullpath_size);
		strcat(fullpath,path_tmp);
		strcat(fullpath,filename);
		
		
		
		buf_stat = malloc(sizeof(struct stat));
		
		
		printf("full path: %s\n",fullpath);
		
		
		//if(stat(filename, bufstat))
		
		
		
		
		
		
		printf("readen file: %s %c\n", readen_file->d_name, readen_file->d_type); 
		
		free(path_tmp);
		free(fullpath);
		free(buf_stat);
		
		//stat -t compil.sh 
		//NAME TAILLE BLOCS _ UID GID _ _ NBLIEN _ _ _ _ _ _ _ BLOCE/S
	
	}
	
	closedir(directory);
	
}

/*
struct dirent {
               ino_t          d_ino;     
               off_t          d_off;       
               unsigned short d_reclen;    
               unsigned char  d_type;      
               char           d_name[256]; 
           };
*/

/*
struct stat {
               dev_t     st_dev;         // ID of device containing file 
               ino_t     st_ino;         // inode number 
     DROITS--->mode_t    st_mode;        // protection 
     NB_HARD-->nlink_t   st_nlink;       // number of hard links 
     UID------>uid_t     st_uid;         // user ID of owner 
     GID------>gid_t     st_gid;         // group ID of owner 
               dev_t     st_rdev;        // device ID (if special file) 
               off_t     st_size;        // total size, in bytes 
     BLOCK_SIZEblksize_t st_blksize;     // blocksize for filesystem I/O 
               blkcnt_t  st_blocks;      // number of 512B blocks allocated 
               
      >>>>>>>>>struct timespec st_atim;  // time of last access 
      >>>>>>>>>struct timespec st_mtim;  // time of last modification 
      >>>>>>>>>struct timespec st_ctim;  // time of last status change 
*/

/*

		   S_ISUID     04000   set-user-ID bit
           S_ISGID     02000   set-group-ID bit (see below)
           S_ISVTX     01000   sticky bit (see below)

           S_IRWXU     00700   owner has read, write, and execute permission
           S_IRUSR     00400   owner has read permission
           S_IWUSR     00200   owner has write permission
           S_IXUSR     00100   owner has execute permission

           S_IRWXG     00070   group has read, write, and execute permission
           S_IRGRP     00040   group has read permission
           S_IWGRP     00020   group has write permission
           S_IXGRP     00010   group has execute permission

           S_IRWXO     00007   others (not in group) have read,  write,  and
                               execute permission
           S_IROTH     00004   others have read permission
           S_IWOTH     00002   others have write permission
           S_IXOTH     00001   others have execute permissio

*/


/*
	Different type of ls:
		   b      block (buffered) special
           c      character (unbuffered) special
           d      directory
           p      named pipe (FIFO)
           f      regular file
           l      symbolic link; this is never true if the  -L  option
                  or  the -follow option is in effect, unless the sym‐
                  bolic link is broken.  If you  want  to  search  for
                  symbolic links when -L is in effect, use -xtype.
           s      socket
           D      door (Solaris)
*/

int main(int argc, char *argv[]){

	//path variables
	char** path = malloc(sizeof(char*)*MAX_PATH); 
	int nb_path = 0; 
	int cur_size_path = MAX_PATH;
	
	//options variables
	int crs = 0;
	char cur_opt;
	int opt_a = 0;
	int opt_R = 0;
	
	for(int i=1; i<argc; i++){
		//printf("%s ",argv[i]);
		if(argv[i][0]=='-'){
			crs = 1; 
			while(argv[i][crs]){
				cur_opt=argv[i][crs++];
				//printf(">%c",cur_opt);
				switch(cur_opt){
					case 'a':
						opt_a = 1;
					break;
					case 'R':
						opt_R = 1;
					break;
					default:
						printf("%s: %c",errormsg[1], cur_opt);
						exit(-1);
					break;
				}
			}
		}
		
		else{
			if(nb_path>=cur_size_path){
				cur_size_path=cur_size_path*2;
				char** path_tmp = malloc(sizeof(char*)*cur_size_path);
				memcpy(path_tmp,path,sizeof(path)*cur_size_path/2);
				free(path);
				path=path_tmp;
			}
			path[nb_path++]=argv[i];
		}
		
	}
	/*
	printf("options %d %d ",opt_a, opt_R);
	printf("nb path %d \n",nb_path);
	
	printf("Liste path: \n");
	
	for(int i=0; i<nb_path; i++){
		printf(">> %s\n", path[i]);
	}*/
	
	
	list_file("../bash-sec");
	
}
