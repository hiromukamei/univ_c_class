#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#define MAX_ARGS 10
#define MAX_INPUT 512

void exec_command(char *argv[MAX_ARGS]);
void welcome();

int main(){
	char input[MAX_INPUT];
	const char *delims = " \t\n";
	int argc;
	char *argv[MAX_ARGS];
	char *cp;
	pid_t pid;
	int status;

	welcome();

	while(1){
		printf("kameshell >");

		if(fgets(input, sizeof(input), stdin) == NULL){
			printf("Bye!\n");
			exit(0);
		}


		if(strcmp(input, "\n") == 0){
			continue;
		}
 
		cp = input;
		for(argc = 0; argc < MAX_ARGS - 1; argc++){ //execvpの引数として渡す時、最後がnullポインタでないといけないので、MAXまでつかえない
			if((argv[argc] = strtok(cp, delims)) == NULL){
				break;
			}
			cp = NULL;
		}

		if(strcmp(argv[0], "exit") == 0){
			printf("Bye!\n");
			exit(0);
		}

		pid = fork();
		if(pid == 0){
			exec_command(argv);
		}else if(pid > 0){
			wait(&status);
		}else{
			perror("fork");
		}
	}

	return 0;
}

void exec_command(char *argv[MAX_ARGS]){
	if(execvp(argv[0], argv) == -1){
		perror("Error: ");
		exit(0); //error時、制御が子プロセスに戻ってくるので子プロセスをkillする
	}
}

void welcome(){
	printf("\n=== welcome to kameshell ===\n       ＿＿    ＿\n     ／♯＃＼  /･_)\n    （ｿ♯＃♯＃Ｙ ／\n      (_)――-(_)′\n\n============================\n\n");
}
