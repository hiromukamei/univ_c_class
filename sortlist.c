#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

struct entry{
	char name[32];
	char sex;
	int  age;
	struct entry *next;
};

struct entry *CreateEntry(){
	struct entry *ep;

	ep = (struct entry *)malloc(sizeof(struct entry));
	ep->next = (struct entry *)NULL;
	return ep;
}

struct entry *ReadEntryData(struct entry *Top){
	struct entry *ep;
	char *cp, *tp;
	char buf[512];
	char name[32];
	char sex;
	int age;


	while(fgets(buf, sizeof(buf), stdin) != NULL){
		/*
		 * Node setup
		 */
		if(Top == NULL){
			Top = CreateEntry();
			ep = Top;
		}else{
			ep->next = CreateEntry();
			ep = ep->next;
		}

		cp = buf;
		tp = cp;
		if((cp = index(tp, ' ')) == NULL){
			fprintf(stderr, "Input format fail: %s\n", tp);
			exit(-1);
		}
		*cp++ = '\0';
		strcpy(ep->name, tp);

		ep->sex = *cp++;
		cp++;

		ep->age = atoi(cp);
	}
	return(Top);
}

void SwapEntry(struct entry *a, struct entry *b){
	struct entry tmp;

	strcpy(tmp.name, a->name);
	tmp.sex = a->sex;
	tmp.age = a->age;

	strcpy(a->name, b->name);
	a->sex = b->sex;
	a->age = b->age;

	strcpy(b->name, tmp.name);
	b->sex = tmp.sex;
	b->age = tmp.age;
}

struct entry *SortList(struct entry *Top){
	//処理をここに書く
	struct entry *ep;
	struct entry *fp;

	for(ep = Top; ep->next !=NULL; ep = ep->next){
		for(fp = ep; fp->next != NULL; fp = fp->next){
			if(fp->age > fp->next->age){
				SwapEntry(fp, fp->next);
			}
		}
	}
	return(Top);
}


int main(){
	struct entry *Top;
	struct entry *ep;

	/*
	 * read data
	 */
	Top = (struct entry *)NULL;
	Top = ReadEntryData(Top);

	/*
	 * Print out Entry Data
	 */
	printf("Name\tSex\tAge\n");
	for(ep = Top; ep != NULL; ep = ep->next){
		printf("%s\t%c\t%d\n", ep->name, ep->sex, ep->age);
	}

	/* 
	 * sorting by age
	 */
	Top = SortList(Top);

	printf("Name\tSex\tAge\n");
	for(ep = Top; ep != NULL; ep = ep->next){
		printf("%s\t%c\t%d\n", ep->name, ep->sex, ep->age);
	}

}
