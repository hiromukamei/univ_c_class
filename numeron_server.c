#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>

#define BUF_SIZE 512

void judge(char *input, char *answer, char *result);

int main(void){

	int server_sockfd, client_sockfd[2];
	int server_len, client_len ;
	struct sockaddr_in server_address;
	struct sockaddr_in client_address;

	server_sockfd = socket(AF_INET,SOCK_STREAM,0);
	server_address.sin_family = AF_INET ;
	server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
	server_address.sin_port = 6666;

	server_len = sizeof(server_address);
	bind(server_sockfd, (struct sockaddr *)&server_address, server_len);


	pid_t pid1, pid2;
	int fd[4][2];
	int status;
	char buf[BUF_SIZE];
	int pnum = 1;
	char end[] = "end";

	listen(server_sockfd , 5);
	printf("server waiting\n");

	/* client1 */
	client_sockfd[0] = accept(server_sockfd, (struct sockaddr *)&client_address, (socklen_t *)&client_len);
	write(client_sockfd[0], &pnum, sizeof(pnum)); //send pnum
	pnum++;
	pipe(fd[0]);
	pipe(fd[1]);
	pid1 = fork();
	if(pid1 == 0){
		close(fd[0][0]);
		close(fd[1][1]);
		close(server_sockfd);

		/* Game start */
		read(fd[1][0], buf, BUF_SIZE);
		write(client_sockfd[0], buf, BUF_SIZE);
		/* Get answer */
		read(client_sockfd[0], buf, BUF_SIZE);
		write(fd[0][1], buf, BUF_SIZE);

		while(1){
			/* your turn or win */
			read(fd[1][0], buf, BUF_SIZE);
			write(client_sockfd[0], buf, BUF_SIZE);
			if(strcmp(buf, end) == 0){
				exit(0);
			}

			/* called number */
			read(client_sockfd[0], buf, BUF_SIZE);
			write(fd[0][1], buf, BUF_SIZE);
			/* show result */
			read(fd[1][0], buf, BUF_SIZE);
			write(client_sockfd[0], buf, BUF_SIZE);

			/* wait or lose */
			read(fd[1][0], buf, BUF_SIZE);
			write(client_sockfd[0], buf, BUF_SIZE);
			if(strcmp(buf, end) == 0){
				exit(0);
			}
		}
	}

	close(fd[0][1]);
	close(fd[1][0]);
	close(client_sockfd[0]);


	/* client2 */
	client_sockfd[1] = accept(server_sockfd, (struct sockaddr *)&client_address, (socklen_t *)&client_len);
	write(client_sockfd[1], &pnum, sizeof(pnum)); //send pnum
	pipe(fd[2]);
	pipe(fd[3]);
	pid2 = fork();
	if(pid2 == 0){
		close(fd[2][0]);
		close(fd[3][1]);
		close(server_sockfd);

		/* Game start */
		read(fd[3][0], buf, BUF_SIZE);
		write(client_sockfd[1], buf, BUF_SIZE);
		/* Get answer */
		read(client_sockfd[1], buf, BUF_SIZE);
		write(fd[2][1], buf, BUF_SIZE);

		while(1){
			/* wait or win */
			read(fd[3][0], buf, BUF_SIZE);
			write(client_sockfd[1], buf, BUF_SIZE);
			if(strcmp(buf, end) == 0){
				exit(0);
			}

			/* your turn or lose */
			read(fd[3][0], buf, BUF_SIZE);
			write(client_sockfd[1], buf, BUF_SIZE);
			if(strcmp(buf, end) == 0){
				exit(0);
			}

			/* called number */
			read(client_sockfd[1], buf, BUF_SIZE);
			write(fd[2][1], buf, BUF_SIZE);
			/* show result */
			read(fd[3][0], buf, BUF_SIZE);
			write(client_sockfd[1], buf, BUF_SIZE);
		}
	}

	close(fd[2][1]);
	close(fd[3][0]);
	close(client_sockfd[1]);

	char answer[2][BUF_SIZE];
	char result[BUF_SIZE];
	int i;

	char str1[] = "game start!!\ndecide your number.";
	char str2[] = "your turn.\ninput number.";
	char str3[] = "wait";
	char correct[] = "3h0b";

	/* game start */
	/* set answer */
	write(fd[1][1], str1, sizeof(str1));
	read(fd[0][0], answer[0], BUF_SIZE);
	answer[0][3] = '\0';
	write(fd[3][1], str1, sizeof(str1));
	read(fd[2][0], answer[1], BUF_SIZE);
	answer[1][3] = '\0';
	printf("c1: %s\nc2: %s\n", answer[0], answer[1]);

	while(1){
		/* wait & your turn */
		write(fd[1][1], str2, sizeof(str2));
		write(fd[3][1], str3, sizeof(str3));

		/* c1 called number */
		read(fd[0][0], buf, BUF_SIZE);
		printf("client1: %s\n", buf);
		judge(buf, answer[1], result);
		write(fd[1][1], result, sizeof(result));

		/* answer correct */
		if(strcmp(correct, result) == 0){
			write(fd[1][1], end, sizeof(end));
			write(fd[3][1], end, sizeof(end));
			break;
		}

		/* wait & your turn */
		write(fd[1][1], str3, sizeof(str3));
		write(fd[3][1], str2, sizeof(str2));

		/* c2 called number */
		read(fd[2][0], buf, BUF_SIZE);
		printf("client2: %s\n", buf);
		judge(buf, answer[0], result);
		write(fd[3][1], result, sizeof(result));

		/* answer correct */
		if(strcmp(correct, result) == 0){
			write(fd[1][1], end, sizeof(end));
			write(fd[3][1], end, sizeof(end));
			break;
		}
	}

	for(int i = 0; i < 2; i++){
		wait(&status);
	}
	printf("game is over\n");

	return 0;
}

void judge(char *input, char *answer, char *result){
	int h = 0;
	int b = 0;
	int i, j;

	for(j = 0; j <= 2; j++){
		for(i = 0; i <= 2; i++){
			if(input[i] == answer[j]){
				if(i == j){
					h++;
				}else{
					b++;
				}
			}
		}
	}

	sprintf(result, "%dh%db", h, b);
}
