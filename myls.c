#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pwd.h>

#define syserror(x) perror(errormsg[x]), exit(x)
#define ROUGE(m) "\033[01;31m"m"\033[0m"
#define VERT(m) "\033[01;32m"m"\033[0m"

#define MAX_PATH 10
#define RIGHT_LEN 10


char *errormsg[]={
	"No error",//0
	ROUGE("Invalid options"),//1
	ROUGE("Problem with opendir"),//2
	ROUGE("Problem with stat"),//3
	
	
};

/*
 * Makes a path ending with a /
 * /!\ returns malloc, don't forget to free
 */
char *add_slash_to_path(char *path){
	
	char *res;
	
	if(path[strlen(path)-1]!='/'){
		int size_path_tmp=strlen(path)+2;
		
		res = malloc(sizeof(char)*size_path_tmp);
		strcpy(res, path);
		res[size_path_tmp-2]='/';
		res[size_path_tmp-1]='\0';
	}
	else{
		res = malloc(sizeof(char)*(strlen(path)+1));
		strcpy(res, path);
	}
	return res;
}

/*
 * If a path starts with ~, the function replace it with the user's path.
 * /!\ returns malloc, don't forget to free
 */
char *replace_tilde(char *path){
	char *res;
	
	struct passwd *pw = getpwuid(getuid());
	const char *homedir = pw->pw_dir;
	printf("homedir %s \n", homedir);
	
	if(path[0]=='~'){
		int size_path=strlen(path)+strlen(homedir)-1;
		res = malloc(sizeof(char)*size_path);
		strcpy(res,homedir);//delete the ~
		strcat(res,path+1);
		printf("PATH FULL: %s \n", res);
	}
	return res;
}


/*
 * ls -l to a path
 */
void list_file(char *path){
	
	char * path_tmp;
	char * path_tmp_full;
	
	DIR* directory;
	struct dirent* readen_file;
	printf("ls -l %s \n",path);
	
	
	path_tmp = add_slash_to_path(path);
	path_tmp_full = replace_tilde(path_tmp);
	
	directory = opendir(path_tmp_full);
	if(!directory)syserror(2);
	
	while(readen_file=readdir(directory)){
		
		//to display
		char *filename;//
		char *fullpath;//
		//char * path_tmp;
		int file_type;
		int uid;//
		char *username;
		int gid;//
		char *groupname;
		char rights[]="----------";
		
		int nb_hard_link;		
		int file_size;
		
		char* modif_month;
		int modif_day;
		char* modif_time;
		
		//in process
		mode_t st_mode;
		struct stat *buf_stat;
		
		int fullpath_size;
		
		
		//Get the full path
		filename = readen_file->d_name;
		fullpath_size = strlen(path_tmp_full) + strlen(filename);
		
		fullpath = malloc(sizeof(char)*(fullpath_size+1));
		strcpy(fullpath,path_tmp_full);
		strcat(fullpath,filename);
		
		
		printf("fullpath -> %s\n", fullpath);
		
		buf_stat = malloc(sizeof(struct stat));
		
		if(stat(fullpath, buf_stat)==-1){
			syserror(3);
		}
		
		st_mode = buf_stat->st_mode;
		
		
		if(S_IRUSR&st_mode);
			rights[1]='r';
		if(S_IWUSR&st_mode)
			rights[2]='w';
		if(S_IXUSR&st_mode)
			rights[3]='x';
		if(S_IRGRP&st_mode)
			rights[4]='r';
		if(S_IWGRP&st_mode)
			rights[5]='w';
		if(S_IXGRP&st_mode)
			rights[6]='x';
		if(S_IROTH&st_mode)
			rights[7]='r';
		if(S_IWOTH&st_mode)
			rights[8]='w';
		if(S_IXOTH&st_mode)
			rights[9]='x';
		
		//int file_type;
		if(S_ISREG(st_mode)){
			rights[0]='f';
		}
		else if(S_ISDIR(st_mode)){
			rights[0]='d';
		}
		else if(S_ISBLK(st_mode)){
			rights[0]='b';
		}
		else if(S_ISLNK(st_mode)){
			rights[0]='l';
		}
		else if(S_ISFIFO(st_mode)){
			rights[0]='p';
		}
		else if(S_ISSOCK(st_mode)){
			rights[0]='s';
		}
		else if(S_ISCHR(st_mode)){
			rights[0]='c';
		}
		
		/*
		int uid;//
		char *username;
		int gid;//
		char *groupname;
		
		int nb_hard_link;		
		int file_size;
		
		char* modif_month;
		int modif_day;
		char* modif_time;
		*/
		
		printf("readen file: %s \n", readen_file->d_name); 
		printf("rights: %s\n",rights);
		
		
		
		free(fullpath);
		free(buf_stat);
		
		//stat -t compil.sh 
		//NAME TAILLE BLOCS _ UID GID _ _ NBLIEN _ _ _ _ _ _ _ BLOCE/S
		
		
		
	}
	
	closedir(directory);
	free(path_tmp);
	free(path_tmp_full);
	
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




int main(int argc, char *argv[]){

	//path variables
	char** tab_path = malloc(sizeof(char*)*MAX_PATH); 
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
				char** tab_path_tmp = malloc(sizeof(char*)*cur_size_path);
				memcpy(tab_path_tmp,tab_path,sizeof(tab_path)*cur_size_path/2);
				free(tab_path);
				tab_path=tab_path_tmp;
			}
			tab_path[nb_path++]=argv[i];
		}
		
	}
	/*
	printf("options %d %d ",opt_a, opt_R);
	printf("nb path %d \n",nb_path);
	
	printf("Liste path: \n");
	
	for(int i=0; i<nb_path; i++){
		printf(">> %s\n", path[i]);
	}*/
	
	//list_file("/home/loick/Bureau/Nouveau dossier");
	list_file("~/Bureau");
	free(tab_path);
}
