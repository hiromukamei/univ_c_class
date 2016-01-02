#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define BUF_SIZE 512

int main(){
	int sockfd;
	int len;
	struct sockaddr_in address;
	int result;
	char buf[BUF_SIZE];
	char end[] = "end";
	int pnum;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = inet_addr("127.0.0.1");
	address.sin_port = 6666;
	len = sizeof(address);

	result = connect(sockfd, (struct sockaddr *)&address, len);

	if(result == -1){
		perror("error: client");
		exit(1);
	}

	/* Recieve pnum */
	read(sockfd, &pnum, sizeof(pnum));
	printf("you are player%d\n", pnum);
	/* Game start */
	read(sockfd, buf, BUF_SIZE);
	printf("%s\n", buf);
	/* Set my number */
	fgets(buf, BUF_SIZE, stdin);
	buf[strlen(buf)-1] = '\0';
	write(sockfd, buf, BUF_SIZE);

	if(pnum == 2){
		/* wait */
		read(sockfd, buf, BUF_SIZE);
	}

	while(1){
		/* your turn or lose */
		read(sockfd, buf, BUF_SIZE);
 		if(strcmp(buf, end) == 0){
			printf("you lose\n");
			exit(0);
		}
		printf("%s\n", buf);
		/* call number */
		fgets(buf, BUF_SIZE, stdin);
		buf[strlen(buf)-1] = '\0';
		write(sockfd, buf, BUF_SIZE);
		/* show result */
		read(sockfd, buf, BUF_SIZE);
		printf("%s\n", buf);
		/* wait or win */
		read(sockfd, buf, BUF_SIZE);
		if(strcmp(buf, end) == 0){
			printf("you win\n");
			exit(0);
		}

		printf("%s\n", buf);
	}
}
