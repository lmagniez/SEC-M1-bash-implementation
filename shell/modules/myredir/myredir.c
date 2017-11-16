#include "myredir.h"
#include "../../operator.h"


// ls -l > file.txt
void my_redir(char *filename, int del, int fd){
	
	int old_fd, new_fd;
	mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;
	
	int file; 
	if(!del){
		file= open(filename, O_RDWR|O_APPEND|O_CREAT, mode);
		lseek(file, 0, SEEK_END);
	}
	else{
		file= open(filename, O_TRUNC|O_WRONLY|O_CREAT, mode);
	}
	
	if(file<0){
		perror("fopen");
		exit(0);
	}
	
	old_fd = dup(fd);
	close(fd);
	new_fd = dup(file);
	
	/*
	if(close(file)!=0){
		perror("close fd");
		exit(0);
	}*/
	
}

void my_redir_read(char *filename){
	
	int old_fd, new_fd;
	mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;
	
	int file; 
	
	file= open(filename, O_RDONLY|O_CREAT, mode);
	lseek(file, 0, SEEK_SET);
	
	if(file<0){
		perror("fopen");
		exit(0);
	}
	
	old_fd = dup(0);
	close(0);
	new_fd = dup(file);
	
}

// > : delete the content of the file
void my_redir_stdout(char **cmd, char *file){
	my_redir(file, 1, 1);
	execvp(cmd[0],cmd);
}

// >> : add the content at the end of file
void my_redir_stdout_double(char **cmd, char *file){
	my_redir(file, 0, 1);
	execvp(cmd[0],cmd);

}

// 2> : delete the content of the file
void my_redir_stderr(char **cmd, char *file){
	my_redir(file, 1, 2);
	execvp(cmd[0],cmd);

}

// 2>> : add the content at the end of file
void my_redir_stderr_double(char **cmd, char *file){
	my_redir(file, 0, 2);
	execvp(cmd[0],cmd);

}

// >& : stderr and stdout to a file, delete the content of file
void my_redir_stderr_stdout(char **cmd, char *file){
	my_redir(file, 1, 1);
	my_redir(file, 1, 2);
	execvp(cmd[0],cmd);
	
}

// >>& : stderr and stdout to a file, add at the end of file
void my_redir_stderr_stdout_double(char **cmd, char *file){
	my_redir(file, 0, 1);
	my_redir(file, 0, 2);
	execvp(cmd[0],cmd);
}

// < : read the file into the command
void my_redir_stdin(char **cmd, char *file){
	my_redir_read(file);
	execvp(cmd[0],cmd);
}


int main(int argc, char **argv){
	
	
	char ** cmd = malloc(sizeof(char*)*10);
	cmd[0] = "ls";
	cmd[1] = "azeazez";
	cmd[2] = NULL;
	
	char ** cmd2 = malloc(sizeof(char*)*10);
	cmd2[0] = "cat";
	//cmd2[1] = "-l";
	cmd2[1] = NULL;
	
	
	
	
	int pid = fork() ;
	if(pid == 0){
		//my_redir_stderr(cmd, "test.txt");
		exit(0);
	}
	if(pid > 0){
		int status;
		wait(&status);
		//my_redir_stderr_stdout_double(cmd2, "test.txt");
		my_redir_stdin(cmd2, "test.txt");
		
	}
	
	
}



