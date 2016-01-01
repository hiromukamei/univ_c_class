#include <stdio.h>
#include <string.h>

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

int main(){

	char answer[8];
	char input[8];
	char result[8];
	char correct[] = "3h0b";

	printf("decide answer number\n");
	fgets(answer, sizeof(answer), stdin);
	answer[strlen(answer)-1] = '\0';

	while(1){
		printf("input test number\n");
		fgets(input, sizeof(input), stdin);
		input[strlen(input)-1] = '\0';
		judge(input, answer, result);
		printf("%s\n", result);
		if(strcmp(result, correct) == 0){
			printf("conguratulation!\n");
			return 0;
		}
	}
}
