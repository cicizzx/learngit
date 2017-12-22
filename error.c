#include<stdio.h>
#include"error.h"
#include"lexical.h"
int errnum=0;
extern int line;
int error(int errortype)
{
	errnum++;
	printf("Error %d:line:%d ",errnum,line+1);
	switch (errortype)
	{
	case UNKNOWN_SYMBOL:
		printf("unknown symbol in line");break;
	case STRING_TOO_LONG:
		printf("string is too long");break;
	case WRONG_FORMAT_INTEGER:
		printf("wrong format integer");break;
	case WITHOUT_DECLARATION:
		printf("identifier is used without declartion");break;
	case RE_DECLARATION:
		printf("re-declaration");break;
	case WRONG_FORMAT_IDEN:
		printf("wrong format identifier");break;
	case WRONG_EXPRESSION:
		printf("wrong expression");break;
	case EMPTY_CONDITION:
		printf("empty condition");break;
	case WRONG_FORMAT_CHAR:
		printf("wrong format char");break;
	case WRONG_CONST_MISSING_TK:
		printf("const statement misses int/char");break;
	case UNFINISHED_STRING:
		printf("\'\"\'dismatch");break;
	case CODE_AFTER_MAIN:
		printf("code after main function");break;
	case MISSING_SEMICN:
		printf("sentence misses semicn");
		while(read()==SEMICN)	;
		break;
	case PARENT_DISMATCH:
		printf("parent dismatch");break;
	case BRACK_DISMATCH:
		printf("brack dismatch");break;
	case SCANF_NOINTONCHAR:
		printf("scanf can only read int and char");break;
	case ARRAY_INFO_LSS0:
		printf("array index is less than 0");break;
	case ARRAY_OVERFLOW:
		printf("array overflow");break;
	case ASSIGN_WRONG_LEFT:
		printf("the identifier to the left of the equation is wrong");break;
	}
	printf("\n");
	return errortype;
}
