#include "myjoker.h"

char *errormsg[]={
	"No error",//0
	ROUGE("Invalid options"),//1
	ROUGE("Problem with opendir"),//2
	ROUGE("Problem with stat"),//3
	
};

//deal one /mot*mot*mot/ at a time
//deal one * at a time

void lire_doc(char * path){
	DIR* directory;
	struct dirent* readen_file;
	
	//file list
	int max_file = MAX_FILE;
	int nb_file = 0;
	char ** file_list = malloc(sizeof(char*)*max_file);
	//dir list	
	int max_dir = MAX_DIR;
	int nb_dir = 0;	
	char ** dir_list = malloc(sizeof(char*)*max_dir);

	directory = opendir(path);
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
	
	for(int i=0; i<nb_file; i++){
		printf("%s\n",file_list[i]);
	}
	for(int i=0; i<nb_file; i++){
		free(file_list[i]);
	}
	
	free(file_list);
	free(dir_list);
}


// ./*e*/aezaze*
// -> ./test/azezaze*
// -> ./bleu/azezaze*


// ./*e*/aezaze*
// -> ./ ->*e*/


// ./*e*
// -> ./ -> *e*
//indicate if we have another folder or not


// given an expression (ex: ./abc/te*t/dir*/file)
// folder -> next folder to look into (./abc/)
// elt -> next element to examine in the folder (te*t/)
// fin -> end of the expression for concatenating later (dir*/file)
// NOTE: if elt==null, it means there are no * anymore
void get_path_from_expression(char * expression, char **folder, char **elt, char **fin){
	//printf(">>>%s \n", expression);
	int crs = 0;
	int crs2 = 0;
	int len = strlen(expression)+1;
	int len_folder, len_elt, len_fin;
	int contain_star=0;
	int found_slash=0;
	
	
	int i, j;
	for(i=0; i<len; i++){
		if(expression[i]=='/')
			crs=i+1;	
		if(expression[i]=='*'||expression[i]=='?'||expression[i]=='[')
			break;
	}
	//printf("crs : %d \n",crs);
	
	//look for the next slash and check if there's star
	for(int i=crs; i<len; i++){
		if(expression[i]=='*'||expression[i]=='?'||expression[i]=='['){
			contain_star=1;
		}
		if(expression[i]=='/'){
			found_slash=1;
			crs2=i+1;
			break;	
		}
	}
	
	
	
	//-> 'abcd/te*t'
	if(contain_star&&!found_slash)
		crs2=len;
	
	//-> 'abcd/test'	
	if(crs2==0)
		crs2=crs;
		
	len_folder = crs;
	len_elt = crs2-crs;
	len_fin = len - (len_folder+len_elt);
	if(len_folder>MAX_LEN)
		*folder=realloc(*folder,sizeof(char)*(len_folder+1));
	if(len_elt>MAX_LEN)
		*elt=realloc(*elt,sizeof(char)*(len_elt+1));
	if(len_fin>MAX_LEN)
		*fin=realloc(*fin,sizeof(char)*(len_fin+1));
	
	//printf("len : %d %d %d\n", len_folder, len_elt, len_fin);
	memcpy(*folder,expression,sizeof(char)*len_folder);
	memcpy(*elt,expression+len_folder,sizeof(char)*len_elt);
	memcpy(*fin,expression+(len_folder+len_elt),sizeof(char)*len_fin);
	(*folder)[len_folder]='\0';(*elt)[len_elt]='\0';(*fin)[len_fin]='\0';
	
	
	//printf("%d %d\n", crs, crs2);
	
}




int elt_belong_to_expr(char *expr, char *elt){
	//printf("-- belong to expr, expr-> %s elt-> %s \n",expr,elt);
	//get the first sub string to scan
	int crs_expr = 0;
	int crs_elt = 0;
	
	int crs_substring_deb = 0;
	int crs_substring_fin = 0;
	int oldpos = 0;
	int found_star = 0;
	int last_is_star = 0;
	
	//expr
	char *substring;
	//elt
	char *newpos;
	int last_substring = 0;
	
	
	int len_expr = strlen(expr);
	int len_elt = strlen(elt);
	int len_substring = 0;
	
	
	//search for the first string and compare with the element
	while(crs_expr<len_expr&&expr[crs_expr]!='*'&&expr[crs_expr]!='?'&&expr[crs_expr]!='['){
		crs_expr++;
	}
	substring = malloc(sizeof(char)*(crs_expr+1));
	memcpy(substring,expr,sizeof(char)*(crs_expr));
	substring[crs_expr]='\0';
	//printf("first: %s\n",substring);
	
	if(*substring!='\0'){
		char *res = strstr(elt,substring);
		if(res != elt){
			free(substring);
			return 0;
		}
	}
	crs_elt = strlen(substring);
	free(substring);
	
	//printf("> %s \n", elt+crs_elt);
	if(*(elt+crs_elt)=='\0')
		return 1;
	
	//for each *, get the next substring and check if it belongs to elt
	while(crs_expr<len_expr&&crs_elt<len_elt){
		last_is_star = 0;
		//printf("again?\n");
		found_star = 0;
		//skip the multiple stars
		if(expr[crs_expr]=='*')
			found_star = 1;
		while(expr[crs_expr]!='\0'&&expr[crs_expr]=='*'&&expr[crs_expr]!='?'&&expr[crs_expr]!='['){
			crs_expr++; 
			//printf("skip *\n");
			if(expr[crs_expr]=='*')
				found_star = 1;
		}
		
		crs_substring_deb = crs_expr;
		
		//last char is star -> accept
		//printf("last c : %c\n",expr[crs_expr]);
		if(expr[crs_expr]=='\0'){
			return 1;
			if(found_star){
				return 1;
			}
			else{
				return 0;
			}
		}
		
		//* case
		if(expr[crs_expr]!='?'&&expr[crs_expr]!='['){
			last_is_star = 1;
			//printf("agagain\n");
			//search for the end of the string (? or *)
			while(expr[crs_expr]!='\0'&&expr[crs_expr]!='*'&&expr[crs_expr]!='?'&&expr[crs_expr]!='[')
				crs_expr++;
			crs_substring_fin = crs_expr;
		
			//create the substring
			len_substring = crs_substring_fin-crs_substring_deb;
			substring = malloc(sizeof(char)*(len_substring+1));
			memcpy(substring,expr+crs_substring_deb,sizeof(char)*(len_substring));
			substring[len_substring]='\0';
			//printf("my substring %s\n",substring);
		
			//check if last substring
			if(expr[crs_expr]=='\0'){
				break;
			}
		
			//if not the last substring, check with the current element
			char *res = strstr(elt+crs_elt,substring);
			if(!res){
				//printf("does not correspond!\n");
				free(substring);
				return 0;
			}
			else{
				crs_elt+=len_substring;
			}
		
			//printf("substring! %s\n",substring);
			free(substring);
		}
		if(expr[crs_expr]=='?'){
			//printf("? ? ? !!\n");
			crs_expr++;
			crs_elt++;
			if(elt[crs_elt]=='\0'&&expr[crs_expr]=='\0'){
				//printf("here?\n");
				return 1;
			}
		}
		if(expr[crs_expr]=='['){
			//printf("[ ] !!\n");
			int etoile = 0;
			crs_substring_deb = crs_expr;
			crs_substring_fin = crs_expr;
			
			len_substring = 0;
			while(expr[crs_substring_fin]!='\0'&&expr[crs_substring_fin]!=']'){
				crs_substring_fin++;
			}		
			if(expr[crs_substring_fin]=='\0')
				return 0;
			if(expr[crs_substring_fin+1]=='*'){
				//printf("[ ]* !!\n");
				etoile = 1;
			}
			crs_substring_fin++;
			len_substring = crs_substring_fin - crs_substring_deb - 2;
			substring = malloc(sizeof(char)*len_substring+1);
			memcpy(substring,expr+crs_substring_deb + 1,sizeof(char)*(len_substring));
			substring[len_substring]='\0';
			
			//printf("substring ->>> %s len_substring : %d \n",substring, len_substring);
			char * str = get_str_from_expr(substring);
			
			//printf("on compare str %s avec c %c\n",str,elt[crs_elt]); 
			//printf("crs expr %c\n",expr[crs_expr]); 
			
			
			
			
			if (!strchr (str, elt[crs_elt])){
				free(str);
				return 0;
			}
			
			
			
			//ls c[a-z]m[a-z]il.sh
			//ls com[a-z]il.sh
			//ls compi[a-z].sh
			//ls compil.[a-z]h
			crs_expr+=len_substring +2;
			crs_elt++;
			
			//[a-z]*
			if(etoile){
				crs_expr++;//skip the star
				while(elt[crs_elt]!='\0'&&strchr(str, elt[crs_elt])){
					crs_elt++;
				}
			}
			free(str);
			//printf("expr now -> %s \n",expr+crs_expr);
			//printf("elt now -> %s \n",elt+crs_elt);
			
			if(expr[crs_expr]=='\0'){
				return 1;
			}
		}
		
		
	}
	
	if(*substring){
		//printf("last substring!! %s cmp %s\n",substring, elt+crs_elt);
		char *res = elt+crs_elt;
		//while(res){
			//printf(">> %s\n",elt+crs_elt);
			res=strstr(elt+crs_elt,substring);
			if(!last_is_star){
				if(res!=NULL){
					if(strcmp(res,elt+crs_elt)!=0){
						return 0;
					}
				}
				else return 0;
			}
			
			//printf("add: %d\n", (res-(elt+crs_elt)));
			if(res)crs_elt = crs_elt + (res-(elt+crs_elt)) + len_substring;
			//printf("res! %d len : %d \n", crs_elt, len_substring);
			//return 0;
		//}
	}
	if(*substring){
		free(substring);
	}
	
	
	//didn't finish reading one of the element
	if(crs_elt<len_elt){
		//printf("didn't finish elt!\n");
		return 0;
	}
	if(crs_expr<len_expr){
		//printf("didn't finish expr!\n");
		return 0;
	}
	
	return 1;
	
}

// *e* test -> true
// *e*e tester -> true
// abc*efg*ijk abcdefghijk -> true 
// abc*d abcd -> true

// read the files in the folder folder, and check if they match with elt.
// search_folder -> Only focus on folder 
// return nb file
int search_in_dir(char *folder, char *elt, char *fin, int search_folder, char ***res){
	//printf(">>>>>>>>>>> search in dir: dir->%s elt->%s fin->%s\n", folder, elt, fin);
	//printf("cherche que dossier? %d\n",search_folder);
	
	
	DIR* directory;
	struct dirent* readen_file;
	
	//buf_stat
	struct stat *buf_stat;
	char *full_path;
	
	//file list
	int max_file = MAX_FILE;
	int nb_file = 0;
	char ** file_list = malloc(sizeof(char*)*max_file);
	*file_list = NULL;
	//dir list	
	int max_dir = MAX_DIR;
	int nb_dir = 0;	
	char ** dir_list = malloc(sizeof(char*)*max_dir);

	directory = opendir(folder);
	if(!directory)syserror(2);
	while((readen_file=readdir(directory))){
		
		char *tmp_file;
		
		if(nb_file==max_file){
			max_file=max_file*2;
			char ** file_list_tmp = malloc(sizeof(char*)*max_file);
			memcpy(file_list_tmp, file_list, sizeof(file_list)*max_file/2);
			
			
			free(file_list);
			file_list = file_list_tmp;
			
			
		}
		
		tmp_file = malloc(sizeof(char)*MAX_FILENAME);
		strcpy(tmp_file, readen_file->d_name);
		
		//printf("read: %s\n", readen_file->d_name);
		
		//check if dir
		buf_stat = malloc(sizeof(struct stat));
		full_path = malloc(sizeof(char)*(strlen(folder)+strlen(tmp_file)+1));
		strcpy(full_path,folder);
		strcat(full_path,tmp_file);
		
		//printf("lstat: %s\n", full_path);
		if(lstat(full_path, buf_stat)==-1){
			syserror(3);
		}
		
		mode_t st_mode;			
		st_mode = buf_stat->st_mode;
		if(((!search_folder)||S_ISDIR(st_mode))&&(tmp_file[0]!='.')){
			char *tmp_elt;
			//remove the /
			if(search_folder){
				tmp_elt = malloc(sizeof(char)*strlen(elt));
				strncpy(tmp_elt, elt, strlen(elt)-1);
				tmp_elt[strlen(elt)-1]='\0';
			}
			else{
				//printf("element ici %s \n",elt);
				tmp_elt = malloc(sizeof(char)*(strlen(elt)+1));
				strcpy(tmp_elt, elt);
				tmp_elt[strlen(elt)]='\0';
			}
			
			//printf("tmpelt! %s tmp_file! %s\n",tmp_elt, tmp_file);
			
			
			if(elt_belong_to_expr(tmp_elt, tmp_file)){
				//printf("ADD!! %s \n",tmp_file);
				file_list[nb_file++]= tmp_file;
			}
		}
		else free(tmp_file);		
		
		
	}
	closedir(directory);
	
	/*
	for(int i=0; i<nb_file; i++){
		printf("> %s\n",file_list[i]);
	}*/
	
	/*
	for(int i=0; i<nb_file; i++){
		printf("%s\n",file_list[i]);
	}
	for(int i=0; i<nb_file; i++){
		free(file_list[i]);
	}
	
	free(file_list);
	*/
	free(dir_list);
	
	file_list[nb_file]= NULL;
	*res=file_list;
	//printf("RETOUR NB FILE %d\n",nb_file);
	return nb_file;
}


char ** get_elements(char *expr){
	//printf("=================== get elements %s \n",expr);
	
	char *new_expr;
	if(expr[0]!='.'&&expr[0]!='/'){
		new_expr = malloc(sizeof(char)*(strlen(expr)+3));
		strcpy(new_expr,"./");
		strcat(new_expr+2,expr);
		new_expr[strlen(expr)+2]='\0';
		//printf("new expression! %s\n",new_expr);
	}
	else{
		new_expr = malloc(sizeof(char)*(strlen(expr)+1));
		strcpy(new_expr,expr);
		new_expr[strlen(expr)]='\0';
	}
	
	char ** elements = malloc(sizeof(char*)*MAX_FILE);
	int nb_elt = 0;
	int max_elt = MAX_FILE;
	
	char *folder = malloc(sizeof(char)*MAX_LEN);
	char *elt = malloc(sizeof(char)*MAX_LEN);
	char *fin = malloc(sizeof(char)*MAX_LEN);
	get_path_from_expression(new_expr, &folder, &elt, &fin);
	
	int len_folder = strlen(folder);
	int len_elt = strlen(elt);
	int len_fin = strlen(fin);
	
	
	/*
	printf("fold: %s, n: %d, c: %c\n", folder, len_folder, folder[len_folder]); 
	printf("elt: %s\n", elt); 
	printf("fin: %s\n", fin); 
	*/
	
	if(*elt!='\0'){
		int nb_file = 0;
		char **list_file = NULL;
		int search_folder = 0;//si search folder, recursif?
		if(elt[len_elt-1]=='/'){
			search_folder = 1;
		}
		nb_file = search_in_dir(folder, elt, fin, search_folder, &list_file);
		//printf(">>> AFTER SEARCH <<< nb_file: %d\n",nb_file);
		if(search_folder){
			for(int i=0; i<nb_file; i++){
				//printf("%s\n",list_file[i]);
				char *full_path = malloc(sizeof(char*)*
					(len_folder+strlen(list_file[i])+strlen(fin)+1+1));
				char **res_tmp;
				strcpy(full_path,folder);
				strcat(full_path,list_file[i]);
				strcat(full_path,"/");
				strcat(full_path,fin);
				full_path[len_folder+strlen(list_file[i])+strlen(fin)+1]='\0';
				
				//printf("> %s\n",full_path);
				
				//ici
				/*
				if(*fin!='\0'){
					//printf("continue !!\n");
					//res_tmp = get_elements(full_path);
				}*/
				res_tmp = get_elements(full_path);

				
				
				int cpt = 0;
				while(res_tmp[cpt]){
					if(nb_elt>=max_elt){
						max_elt = max_elt *2;
						elements = realloc(elements,sizeof(char*)*max_elt);
					}
					
					/*
					elements[nb_elt] = malloc(sizeof(char)*(strlen(res_tmp[cpt])+strlen(folder)+1));
					strcpy(elements[nb_elt],folder);
					strcat(elements[nb_elt],res_tmp[cpt]);
					elements[nb_elt][strlen(res_tmp[cpt])+strlen(folder)]='\0';
					*/
								
					elements[nb_elt] = malloc(sizeof(char)*strlen(res_tmp[cpt])+1);
					memcpy(elements[nb_elt], res_tmp[cpt], strlen(res_tmp[cpt])+1);
					
					nb_elt++;
					cpt++;
				}
				
				free(res_tmp);
				free(full_path);
				
			}
		}
		else{
			//printf("pas folder, que liste \n");
			int cpt = 0;
			while(list_file[cpt]){
				if(nb_elt>=max_elt){
					max_elt = max_elt *2;
					elements = realloc(elements,sizeof(char*)*max_elt);
				}
				
				//printf("elt -> %s \n", list_file[cpt]);
				
				
				elements[nb_elt] = malloc(sizeof(char)*(strlen(list_file[cpt])+strlen(folder)+1));
				strcpy(elements[nb_elt],folder);
				strcat(elements[nb_elt],list_file[cpt]);
				elements[nb_elt][strlen(list_file[cpt])+strlen(folder)]='\0';
				/*
				elements[nb_elt] = malloc(sizeof(char)*strlen(list_file[cpt])+1);
				memcpy(elements[nb_elt], list_file[cpt], strlen(list_file[cpt])+1);
				*/
				nb_elt++;
				cpt++;
				//printf(">>>>>>>>>>>>>>>>>%s\n",list_file[cpt++]);
			}
		}
		
		free(list_file);
		
		//free the 
	}
	
	//elt is empty (no * left): one element
	else{
		int size=(strlen(folder)+strlen(fin)+1);
		elements[nb_elt]=malloc(sizeof(char)*size);
		strcpy(elements[nb_elt],folder);
		strcat(elements[nb_elt],fin);
		//printf("ici: %s\n",elements[nb_elt]);
		nb_elt++;
	}
	
	if(nb_elt>=max_elt){
		max_elt = max_elt *2;
		elements = realloc(elements,sizeof(char*)*max_elt);
	}
	elements[nb_elt] = NULL;
	free(new_expr);
	return elements;
}

char ** replace_cmdarray(char** commandArray, int ind, char **add ){
	int len_cmd=0;
	int len_add=0;
	while(commandArray[len_cmd]!=NULL)
		len_cmd++;

	while(add[len_add]!=NULL)
		len_add++;
	
	int cpt=0;
	char **res = malloc(sizeof(char*)*(len_cmd+len_add));
	//copy the first part of the original command
	for(int i=0; i<ind; i++){
		int len = strlen(commandArray[i])+1;
		res[cpt] = malloc(sizeof(char)*len);
		strcpy(res[cpt++],commandArray[i]);
	}
	//copy the elements to add
	for(int i=0; i<len_add; i++){
		int len = strlen(add[i])+1;
		res[cpt] = malloc(sizeof(char)*len);
		strcpy(res[cpt],add[i]);
		res[cpt][len-1] = '\0';
		cpt++;
	}
	//copy the second part of the original command
	for(int i=ind+1; i<len_cmd; i++){
		int len = strlen(commandArray[i])+1;
		res[cpt] = malloc(sizeof(char)*len);
		strcpy(res[cpt++],commandArray[i]);
	}
	res[cpt]=NULL;
	return res;
	
}


//exemple: a='a' b='d' res='abcd'
// /!\ free!
char *get_str_from_interval(char a, char b){
	//printf("str_int a: %c b: %c \n",a,b);
	int len = b-a+1;
	char *res;
	if(len<0){
		char *res = malloc(sizeof(char)*2);
		res[0] = a;
		res[1] = '\0';
		return res;
	}
		
	if(len==0){
		char *res = malloc(sizeof(char)*2);
		res[0] = a;
		res[1] = '\0';
		return res;
	}
	res = malloc(sizeof(char)*(len+1));
	int cpt = 0;
	for(int i=a; i<=b; i++){
		res[cpt++]=i;
	}
	res[cpt]='\0';
	
	//printf("res > %s\n",res);
	
	return res;
}

//exemple: expr:"a-z.1230-9ea-" res:"abcdefghijklmnopqrstuvwxyz.1230123456789ea-"
// /!\ free!
char* get_str_from_expr(char *expr){
	int len = strlen(expr);
	int max_len = MAX_LEN;
	char *res = malloc(sizeof(char)*max_len);
	int cpt = 0;
	int added = 0;
	
	res[0] = '\0';
	for(int i=0; i<len; i++){
		added = 0;
		//printf("%c \n", expr[i]);
		if(i<len-2){
			if(expr[i+1]=='-'){
				char *ch = get_str_from_interval(expr[i],expr[i+2]);
				if(strlen(ch)+cpt>max_len){
					max_len=max_len*2;
					res=realloc(res,sizeof(char)*max_len);
				}
				cpt = cpt + strlen(ch);
				strcat(res,ch);
				free(ch);
				added = 1;
				i = i+2;
				
			}
		}
		if(!added){
			res[cpt++] = expr[i];
		}
		
	}
	res[cpt]='\0';
	return res;
}

/*

int main(int argc, char **argv){

	
	if(argc==2){
		char ** result = get_elements(argv[1]);
	
		int i=0;
		//printf("result >>> %s\n",result[0]);
		while(result[i]!=NULL){
			printf("result > %s\n",result[i++]);
		}
	}
	
	
	
	else{
		int resb;
		resb=elt_belong_to_expr(argv[1], argv[2]);
		printf(">>>res: %d\n\n", resb);
	}

	char *res= get_str_from_interval('A', 'z');
	printf("res > %s\n", res);
	free(res);


	res = get_str_from_expr("a-z.1230-9ea-");
	printf("res2 > %s\n", res);

}*/
