#include <stdio.h>
#include <string.h>

#define SHIFT 1;

int main(void){
	char sentence[256];
	int s_length;
	char tmp;
	printf("シーザー暗号作成プログラム\n");
	printf("文章を入力してください\n");
	fgets(sentence,sizeof(sentence),stdin);
	/*
	 * sizeof(sentence)は256
	 * '\n'にあうとそれを含め配列に保存される
	 *  実質使えるのは'\n','\0'を除いた256-2 bytes(後に改行文字を終端文字で埋めるので今回は不可であるが、実際は改行文字は塗りつぶすことができる)
	 */
	s_length = strlen(sentence);
	for(int i=0; i<s_length; i++){
		tmp = sentence[i];
		if(('a'<=tmp && tmp<='z') || ('A'<=tmp && tmp<='Z')){
			tmp = tmp + SHIFT;
			if(tmp > 'z'){
				tmp = tmp - 'z' + 'a' - 1;
			}
		}
		sentence[i] = tmp;
	}
	sentence[strlen(sentence) - 1] = '\0'; //改行文字を取り除く(最後から2番目の\nを\0に置き換える)
	printf("暗号化された文章は\n%s\nです\n", sentence);
	fflush(stdin);
	return 0;
}
