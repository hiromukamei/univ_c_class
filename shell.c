#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

char *read_line();
char **split_line(char *line);
void exec_command(char **argv);
void welcome();

int main(){
	char *line;
	char **argv;
	pid_t pid;
	int status;

	welcome();

	while(1){
		printf("kameshell> ");
		line = read_line();
		argv = split_line(line);

		if(argv[0] == NULL){
			continue;
		}else if(strcmp(argv[0], "exit") == 0){
			printf("Bye!\n");
			break;
		}

		pid = fork();
		if(pid == 0){
			exec_command(argv);
		}else if(pid > 0){
			wait(&status);
		}else{
			perror("fork: ");
		}

		free(line);
		free(argv);

	}

	return 0;
}

#define INPUT_BUFSIZE 512

char *read_line(){
	int bufsize = INPUT_BUFSIZE;
	int position = 0;
	char *buffer = malloc(sizeof(char) * bufsize);
	int c;

	if(!buffer){
		fprintf(stderr, "shell: allocation error\n");
		exit(1);
	}

	while(1){
		c = getchar();

		if(c == EOF || c == '\n'){
			buffer[position] = '\0';
			return buffer;
		}else{
			buffer[position] = c;
		}
		position++;

		if(position == bufsize){
			bufsize += INPUT_BUFSIZE;
			buffer = realloc(buffer, bufsize);
			if(!buffer){
				fprintf(stderr, "shell: allocation error\n");
				exit(1);
			}
		}
	}
}

#define TOK_BUFSIZE 64
#define TOK_DELIM " \t\n"

char **split_line(char *line){
	int bufsize = TOK_BUFSIZE;
	int position = 0;
	char **tokens = malloc(bufsize * sizeof(char*));
	char *token;

	if(!tokens){
		fprintf(stderr, "shell: allocation error\n");
		exit(1);
	}

	token = strtok(line, TOK_DELIM);
	while(token != NULL){
		tokens[position] = token;
		position++;

		if(position == bufsize){
			bufsize += TOK_BUFSIZE;
			tokens = realloc(tokens, bufsize * sizeof(char*));
			if(!tokens){
				fprintf(stderr, "shell: allocation error\n");
				exit(1);
			}
		}

		token = strtok(NULL, TOK_DELIM);
	}
	tokens[position] = NULL; //execvpに渡す時、最後にnull pointerが必要なため
	return tokens;
}

void exec_command(char **argv){
	if(argv[0] == NULL){
		exit(0);
	}

	if(execvp(argv[0], argv) == -1){
		perror("shell: ");
		exit(1); //error時、制御が子プロセスに戻ってくるのでexit必須
	}
}

void welcome(){
	printf("\n=== welcome to kameshell ===\n       ＿＿    ＿\n     ／♯＃＼  /･_)\n    （ｿ♯＃♯＃Ｙ ／\n      (_)――-(_)′\n\n============================\n\n");
}
