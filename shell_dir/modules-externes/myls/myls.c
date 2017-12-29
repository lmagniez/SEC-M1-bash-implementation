#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <ctype.h>

#define syserror(x) perror(errormsg[x]), exit(x)
#define ROUGE(m) "\033[01;31m"m"\033[0m"
#define VERT(m) "\033[01;32m"m"\033[0m"
#define BLUE(m) "\033[01;34m"m"\033[0m"
#define CYAN(m) "\033[01;36m"m"\033[0m"


#define MAX_PATH 10
#define MAX_FILE 10
#define MAX_DIR 10
#define RIGHT_LEN 10
#define MAX_FILENAME 100



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
	
	if(path[0]=='~'){
		int size_path=strlen(path)+strlen(homedir)-1;
		res = calloc(sizeof(char)*size_path, size_path);
		strcpy(res,homedir);//delete the ~
		strcat(res,path+1);
		//printf("PATH FULL: %s \n", res);
	}
	else{
		int size_path=strlen(path)+1;
		res = calloc(sizeof(char)*size_path, size_path);
		strcpy(res,path);
		return res;	
	}
	return res;
}


/*
 * Get the file rights and type (store into the buffer rights)
 */
void setrights(char *rights,  struct stat *buf_stat){
	mode_t st_mode;			
	st_mode = buf_stat->st_mode;
		
		
			if(S_IRUSR&st_mode)
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
				rights[0]='-';
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
}

/*
 * Get the last modification date in the date buffer
 */
void get_date_modif(char *date, struct stat *buf_stat){
	char date_modif[64];
	char date_modif_tmp[64];

	struct timespec time = buf_stat->st_mtim ;
	time_t nowtime;
	struct tm *nowtm;

	nowtime = time.tv_sec;
	nowtm = localtime(&nowtime);
	char month_s[4];
	int month_int;
	char *month;

	strftime(month_s, sizeof month_s, "%m",nowtm);
	month_int=atoi(month_s);

	switch(month_int){
		case 1:
			month="jan.";
			break;				
		case 2:
			month="feb.";
			break;				
		case 3:
			month="mar.";
			break;				
		case 4:
			month="apr.";
			break;				
		case 5:
			month="may.";
			break;				
		case 6:
			month="jun.";
			break;				
		case 7:
			month="jul.";
			break;				
		case 8:
			month="aug.";
			break;				
		case 9:
			month="sep.";
			break;				
		case 10:
			month="oct.";
			break;
		case 11:
			month="nov.";
			break;				
		case 12:
			month="dec.";
			break;				
		default:
			month="err.";			
	}

	strftime(date_modif_tmp, sizeof date_modif_tmp, " %d %H:%M", nowtm);
	snprintf(date_modif, sizeof date_modif, "%s", date_modif_tmp);
	
	strcpy(date, month);
	strcat(date, date_modif);

}

/*
 * To lower function
 */
char * tolowerS(char *s){
	int i=0;
	while( s[i] ) {
		s[i]=tolower(s[i]);
      		i++;
   	}
	return s;
}

/*
 * Sort a file list
 */
void sort_list(char **file_list, int nb_file){
	char *s1 = malloc(sizeof(char)*MAX_FILENAME);
	char *s2 = malloc(sizeof(char)*MAX_FILENAME);
	for(int i=0; i<nb_file; i++){
		for(int j=i; j<nb_file; j++){
			char *tmp;
			
			strcpy(s1, file_list[i]);
			strcpy(s2, file_list[j]);
			tolowerS(s1);
			tolowerS(s2);	

			

			if(strcmp(s1, s2)>0){
				tmp=file_list[i];
				file_list[i]=file_list[j];
				file_list[j]=tmp;
			}		
		}	
	}
	free(s1);
	free(s2);
}

/*
 * Get the total number of blocks used for a file list
 */
int get_total_blocks(char *path_tmp_full, char **file_list, int nb_file, int opt_a){
	int total_block = 0;	
	char * fullpath, *filename;
	for(int i=0; i<nb_file; i++){
		struct stat *buf_stat;			
		int fullpath_size;	
		filename = file_list[i];
		if(opt_a||filename[0] != '.'){		
			
			//Get the full path
			fullpath_size = strlen(path_tmp_full) + strlen(filename);
		
			fullpath = malloc(sizeof(char)*(fullpath_size+1));
			strcpy(fullpath,path_tmp_full);
			strcat(fullpath,filename);
		
			buf_stat = malloc(sizeof(struct stat));
		
			if(stat(fullpath, buf_stat)==-1){
				syserror(3);
			}
			total_block += (buf_stat->st_blocks)/2; 
			free(fullpath);
			free(buf_stat);		
		}

	}
	return total_block;
}


/*
 * ls -l to a path
 */
void list_file(char *path, int opt_a, int opt_R){
	
	char * path_tmp;
	char * path_tmp_full;
	//char *filename;
	char *fullpath;	
	int total_block = 0;
	//file list
	int max_file = MAX_FILE;
	int nb_file = 0;
	char ** file_list = malloc(sizeof(char*)*max_file);
	//dir list	
	int max_dir = MAX_DIR;
	int nb_dir = 0;	
	char ** dir_list = malloc(sizeof(char*)*max_dir);
	
	DIR* directory;
	struct dirent* readen_file;
	
	//change the path into the correct format
	path_tmp = add_slash_to_path(path);
	path_tmp_full = replace_tilde(path_tmp);
	
	directory = opendir(path_tmp_full);
	if(!directory)syserror(2);
	while((readen_file=readdir(directory))){
		
		if(nb_file==max_file){
			max_file=max_file*2;
			char ** file_list_tmp = malloc(sizeof(char*)*max_file);
			memcpy(file_list_tmp, file_list, sizeof(file_list)*max_file/2);
			free(file_list);
			file_list = file_list_tmp;
		}
		file_list[nb_file]=malloc(sizeof(char)*MAX_FILENAME);
		strcpy(file_list[nb_file++],readen_file->d_name);

	}
	closedir(directory);

	sort_list(file_list, nb_file);
	total_block = get_total_blocks(path_tmp_full, file_list, nb_file, opt_a);

	printf("%s: \n",path);
	printf("total %d\n",total_block);

	for(int i=0; i<nb_file; i++){
	
		//to display
		char *filename;
		//int file_type;
		int uid;
		char *username;
		int gid;
		char *groupname;
		char rights[]="----------";
		char rights_link[]="----------";
		int nb_hard_link;		
		int file_size;
		char date_mod[64];
		
		char * link_dest = malloc(sizeof(char)*MAX_FILENAME);

		//in process
		struct stat *buf_stat;
		struct stat *link_buf_stat;
		int fullpath_size;
		int is_exec = 0;
		int is_folder = 0;
		int is_link = 0;			

		int link_is_exec = 0;
		int link_is_folder = 0;
		int link_is_link = 0;			

		filename = file_list[i];
		if(opt_a||filename[0]!='.'){		
			
			//Get the full path
			fullpath_size = strlen(path_tmp_full) + strlen(filename);
		
			fullpath = malloc(sizeof(char)*(fullpath_size+1));
			strcpy(fullpath,path_tmp_full);
			strcat(fullpath,filename);
		
			buf_stat = malloc(sizeof(struct stat));
			link_buf_stat = malloc(sizeof(struct stat));

			if(lstat(fullpath, buf_stat)==-1){
				syserror(3);
			}

			//rights
			setrights(rights, buf_stat);
			if(rights[3]=='x'||rights[6]=='x'||rights[9]=='x'){
				is_exec=1;
			}
			if(rights[0]=='d'){
				is_folder=1;			
			}
			if(rights[0]=='l'){
				is_link=1;	
				int size=readlink(fullpath, link_dest, sizeof(char)*MAX_FILENAME);
				link_dest[size]='\0';	
				if(lstat(link_dest, link_buf_stat)==-1){
					syserror(3);
				}
				setrights(rights_link, link_buf_stat);
				if(rights_link[3]=='x'||rights_link[6]=='x'||rights_link[9]=='x'){
					link_is_exec=1;
				}
				if(rights_link[0]=='d'){
					link_is_folder=1;			
				}
				if(rights_link[0]=='l'){
					link_is_link=1;
				}
				
			}
			

			// number of hard links 
			nb_hard_link = buf_stat->st_nlink;       

			//uid
			uid = buf_stat->st_uid;         
			struct passwd *pw = getpwuid(uid);
	  		if (pw)
		 		username = pw->pw_name;

			//gid
			gid = buf_stat->st_gid;
			struct group *gr = getgrgid(gid);
			if(gr)
				groupname = gr->gr_name;

			//file size
			file_size = buf_stat -> st_size;
			
			//date modif
			get_date_modif(date_mod, buf_stat);
			
			if(is_folder){
				printf("%s %d %s %s %d\t%s ",
					rights, nb_hard_link, username, groupname, file_size, date_mod);
				printf(BLUE("%s\n"),filename);
				
				//loop if we add .. or . to the files
				if(strcmp(filename,"..")!=0&&strcmp(filename,".")!=0){				
					if(nb_dir==max_dir-1){
						max_dir=max_dir*2;
						char **dir_list_tmp = malloc(sizeof(char*)*max_dir);
						memcpy(dir_list_tmp, dir_list, sizeof(char*)*(max_dir/2));
						free(dir_list);
						dir_list=dir_list_tmp;	
					}
					dir_list[nb_dir] = malloc(sizeof(char)*MAX_FILENAME);				
					strcpy(dir_list[nb_dir++],fullpath);	
				}
						
			}
			else if(is_link){
				printf("%s %d %s %s %d\t%s ",
					rights, nb_hard_link, username, groupname, file_size, date_mod);
				printf(CYAN("%s"),filename);
				if(link_is_folder)	
					printf(BLUE(" -> %s \n"),link_dest);				
				else if(link_is_link)	
					printf(CYAN(" -> %s \n"),link_dest);
				else if(link_is_exec)	
					printf(VERT(" -> %s \n"),link_dest);
							
			}			
			else if(is_exec){
				printf("%s %d %s %s %d\t%s ",
					rights, nb_hard_link, username, groupname, file_size, date_mod);
				printf(VERT("%s\n"),filename);
			}
			else{
				printf("%s %d %s %s %d\t%s %s\n",rights, nb_hard_link, username, groupname, file_size, date_mod, filename);
			}
			
	
			free(fullpath);
			free(buf_stat);
			free(link_buf_stat);
			
		}
		free(link_dest);
		
	}
	
	
		
	
	for(int i=0; i<nb_file; i++){
		free(file_list[i]);
	}
	free(file_list);	

	free(path_tmp);
	free(path_tmp_full);
	
	if(opt_R){
		for(int i=0; i<nb_dir; i++){
			list_file(dir_list[i], opt_a, opt_R);		
		}
	}
	for(int i=0; i<nb_dir; i++){
		free(dir_list[i]);
	}
	free(dir_list);	
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
		if(argv[i][0]=='-'){
			crs = 1; 
			while(argv[i][crs]){
				cur_opt=argv[i][crs++];
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
	
	//printf("options %d %d ",opt_a, opt_R);
	//printf("nb path %d \n",nb_path);
	
	if(nb_path==0){
		list_file(".",opt_a, opt_R);
	}
	else{
		for(int i=0; i<nb_path; i++){
			list_file(tab_path[i],opt_a,opt_R);
		}
	}

	
	
	//list_file("/home/loick/Bureau/Nouveau dossier",1,1);
	//list_file("~/Bureau");
	//list_file("~/GIT/bash-sec");
	free(tab_path);
}

