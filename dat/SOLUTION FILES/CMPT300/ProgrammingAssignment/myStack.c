#include <stdio.h>
#include <stdlib.h>
#include "LL.h"

LL_t* creat(){
	LL_t* s=LLcreate();
	return s;
}
int isEmpty(LL_t* s){
	return(s->head == NULL);
}
int pop(LL_t* s){
	return LLremove_head(s);
}
void push(LL_t* s, int x){
	LLinsert_head(s,x);
}

int main(){

	LL_t* s=creat();
	if( isEmpty(s) == 1 )
		printf("stack is empty\n");
	else
		printf("stack is NOt empty\n");
	push(s,1);
	push(s,2);
	push(s,3);
	push(s,4);
	push(s,5);
	push(s,6);
	LLprint(s);
	if( isEmpty(s) == 1 )
		printf("stack is empty\n");
	else
		printf("stack is NOt empty\n");
	pop(s);
	pop(s);
	pop(s);
	LLprint(s);
	pop(s);
	pop(s);
	pop(s);
	LLprint(s);
	if( isEmpty(s) == 1 )
		printf("stack is empty\n");
	else
		printf("stack is NOt empty\n");

	return 0;

}
