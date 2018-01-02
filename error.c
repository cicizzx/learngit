#include<stdio.h>
#include"error.h"
#include"lexical.h"
int errnum=0;
extern int line;
extern FILE *fe;
int error(int errortype)
{
	errnum++;
	fprintf(fe,"Error %d:line:%d ",errnum,line+1);
	switch (errortype)
	{
	case UNKNOWN_SYMBOL:
		fprintf(fe,"unknown symbol in line");break;
	case STRING_TOO_LONG:
		fprintf(fe,"string is too long");break;
	case WRONG_FORMAT_INTEGER:
		fprintf(fe,"wrong format integer");break;
	case WITHOUT_DECLARATION:
		fprintf(fe,"identifier is used without declartion");break;
	case RE_DECLARATION:
		fprintf(fe,"re-declaration");break;
	case WRONG_FORMAT_IDEN:
		fprintf(fe,"wrong format identifier");break;
	case WRONG_EXPRESSION:
		fprintf(fe,"wrong expression");break;
	case EMPTY_CONDITION:
		fprintf(fe,"empty condition");break;
	case WRONG_FORMAT_CHAR:
		fprintf(fe,"wrong format char");break;
	case WRONG_CONST_MISSING_TK:
		fprintf(fe,"const statement misses int/char");break;
	case UNFINISHED_STRING:
		fprintf(fe,"\'\"\'dismatch");break;
	case CODE_AFTER_MAIN:
		fprintf(fe,"code after main function");break;
	case MISSING_SEMICN:
		fprintf(fe,"sentence misses semicn");
		while(read()==SEMICN)	;
		break;
	case PARENT_DISMATCH:
		fprintf(fe,"parent dismatch");break;
	case BRACK_DISMATCH:
		fprintf(fe,"brack dismatch");break;
	case SCANF_NOINTONCHAR:
		fprintf(fe,"scanf can only read int and char");break;
	case ARRAY_INFO_LSS0:
		fprintf(fe,"array index is less than 0");break;
	case ARRAY_OVERFLOW:
		fprintf(fe,"array overflow");break;
	case ASSIGN_WRONG_LEFT:
		fprintf(fe,"the identifier to the left of the equation is wrong");break;
	case FULL_TABLE:
		fprintf(fe,"the table is full");break;
	case WRONG_PARA_KIND:
		fprintf(fe,"the parameter's kind is wrong");break;
	case WRONG_PARA_NUM:
		fprintf(fe,"the parameter's number is wrong");break;
	case DEF_FUNC_MISS_PARENT:
		fprintf(fe,"function definition misses parent");break;
	case DEF_FUNC_MISS_NAME:
		fprintf(fe,"function definition misses name");break;
	case WRONG_DEFINITION:
		fprintf(fe,"function definition misses name");break;
	case CONST_DEF_MISS_IDEN:
		fprintf(fe,"there is no identifier in constant definition");break;
	case CONST_DEF_ERROR:
		fprintf(fe,"constant definition error");break;
	case INT_WRONG:
		fprintf(fe,"integer error");break;
	case VAR_KIND_ERROR:
		fprintf(fe,"var kind error");break;
	case PARA_TABLE_MISS_RPARENT:
		fprintf(fe,"parameter table misses right parent");break;
	case FUNC_MISS_LBRACE:
		fprintf(fe,"function misses left brace");break;
	case FUNC_MISS_RBRACE:
		fprintf(fe,"function misses right brace");break;
	case PARA_KIND_ERROR:
		fprintf(fe,"parameter's kind is wrong");break;
	case PARA_MISS_IDEN:
		fprintf(fe,"parameter misses identifier");break;
	case WRONG_STATEMENT:
		fprintf(fe,"statement error");break;
	case ASSIGN_ERROR:
		fprintf(fe,"assignment error");break;
	case CONDITION_ERROR:
		fprintf(fe,"condition error");break;
	case DO_WHILE_ERROR:
		fprintf(fe,"do_while statement error");break;
	case CONSTANT_ERROR:
		fprintf(fe,"constant error");break;
	case MISS_CASE:
		fprintf(fe,"case token is missed");break;
	case SCANF_MISS_IDEN:
		fprintf(fe,"scanf statement misses identifier");break;
	case PRINTF_ERROR:
		fprintf(fe,"printf statement error");break;
	case RETURN_WRONG_TYPE:
		fprintf(fe,"return value's type is wrong");break;

	}
	fprintf(fe,"\n");
	return errortype;
}
