#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>

#define BUF_SIZE 512

int main(int argc, char *argv[]){
	struct addrinfo hints;
	struct addrinfo *result, *rp;
	int sockfd, s;

	if(argc != 3){
		fprintf(stderr, "Usage: %s host port\n", argv[0]);
			exit(1);
	}

	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = 0;
	hints.ai_protocol = 0;

	s = getaddrinfo(argv[1], argv[2], &hints, &result);
	if(s != 0){
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
		exit(1);
	}

	for(rp = result; rp != NULL; rp = rp->ai_next){
		sockfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
		if(sockfd == -1){
			continue;
		}

		if(connect(sockfd, rp->ai_addr, rp->ai_addrlen) != -1){
			break;
		}

		close(sockfd);
	}

	if(rp == NULL){
		fprintf(stderr, "Could not connect\n");
		exit(1);
	}

	freeaddrinfo(result);

	char buf[BUF_SIZE];
	char end[] = "end";
	int pnum;

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
