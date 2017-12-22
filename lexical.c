#include"lexical.h" 
#include"error.h"
#include<stdio.h>
#define STRINGLENGTH 128
int line=0,tokennum=0,tmp=0;
char token[STRINGLENGTH]={0},ch=0;
extern FILE *fp;
int getword()
{
	//字符串单独拎出来处理 
	int flag=0;//符号还是单词
	int id=0;//标识符还是数字
	clear();
	token[tokennum++]=ch;
	
	{	while(chartype(token[tokennum-1])==DIGIT||chartype(token[tokennum-1])==LETTER){
			if(chartype(token[tokennum-1])==LETTER) 
				id=1;
			token[tokennum++]=getsym();
			flag=1;
			if(tokennum==STRINGLENGTH){
				error(STRING_TOO_LONG);
				break;
			}
		}	
		if(flag)
			token[tokennum-1]=0;
		if(flag==1&&id==1){
			if(keyword(token)==IDEN&&chartype(token[0])!=LETTER)
				error(WRONG_FORMAT_IDEN);
			return keyword();
		}
		else if(flag==1&&id==0){
			return INTCON;
		}
	} 
	//字符串处理 
	{
		if(token[0]=='\"') {
			token[tokennum++]=getsym();
			while(token[tokennum-1]!='\"'&&token[tokennum-1]!='\n'&&token[tokennum-1]!=EOF) {
				flag=1;
				token[tokennum++]=getsym();
				if(tokennum==STRINGLENGTH){
					token[tokennum]=0;
					token[tokennum-1]='/"';
					error(STRING_TOO_LONG);
					break;
			}
			}
			if(flag){
				token[tokennum]=0;
				if(token[tokennum-1]=='\"'){
					getsym();
					return STRCON;
				}
				 else if(token[tokennum-1]=='\n'||token[tokennum-1]==EOF){
				 	error(UNFINISHED_STRING);
				 	getsym();
					return STRCON;
				 	
				 }
			}
				
		
		}
		if(token[0]=='\'') {
			token[tokennum++]=getsym();
			while(token[tokennum-1]!='\''&&token[tokennum-1]!='\n'&&token[tokennum-1]!=EOF) {
				flag=1;
				token[tokennum++]=getsym();
				if(tokennum==STRINGLENGTH){
					token[tokennum]=0;
					token[tokennum-1]='/"';
					error(STRING_TOO_LONG);
					break;
			}
			}
			if(flag){
				token[tokennum]=0;
				if(token[tokennum-1]=='\''){
					if(strlen(token)!=3){
						token[2]='\'';
						token[3]=0;
						error(WRONG_FORMAT_CHAR);
					}
					getsym();
					return CHARCON;
				}
				 else if(token[tokennum-1]=='\n'||token[tokennum-1]==EOF){
				 	error(UNFINISHED_STRING);
				 	getsym();
					return STRCON;
				 	
				 }
			}
				
		
		}
	} 
	 
	//进入符号处理 

	{
		if(token[tokennum-1]=='!'||token[tokennum-1]=='='||token[tokennum-1]=='>'||token[tokennum-1]=='<'){
			getsym();
			
			if(ch=='='){
				token[tokennum++]=ch;
				getsym();
			}
			flag=1;
			return symtype();
		}
		else if(token[tokennum-1]!=EOF&&token[tokennum-1]!='\n'&&token[tokennum-1]!=' '&&token[tokennum-1]!='\t'&&chartype(token[tokennum-1])!=LETTER&&chartype(token[tokennum-1])!=DIGIT){
			token[tokennum++]=getsym();
			flag=2;
		}
		if(flag==2){ 
			token[tokennum-1]=0;
			return symtype();
			} 
		if(flag!=0&&symtype()==0)	{
			error(UNKNOWN_SYMBOL);
			return symtype();//?????
		}
		
	}

	//空白字符处理
	{
	if(ch==EOF)
		return EOFFILE;
	while(token[tokennum-1]==' '||token[tokennum-1]=='\t'){
		token[tokennum-1]=getsym();
		if(tokennum==STRINGLENGTH){
				error(STRING_TOO_LONG);
				break;
			}
	} 
	while(ch=='\n'){
		line++;
		getsym();
	}
	return SPACE;
	}
	
	
}

char getsym()
{	
	ch=fgetc(fp);
	return ch;
}

int stringtype(char c)
{
	return (c==32||c==33||(c>=35&&c<=126));
}


int chartype(char ch)
{
	if(ch<='9'&&ch>='0')	return DIGIT;
	if(ch<='z'&&ch>='a')	return LETTER;
	if(ch<='Z'&&ch>='A')	return LETTER;
	if(ch=='_')			return LETTER;

	if(ch=='\t')			return SPACE;
	if(ch==' ')			return SPACE ;
	if(ch=='\n')			return SPACE;
	return 0;
};

int symtype()
{
	if(strcmp(token,"+")==0)	return PLUS;
	if(strcmp(token,"-")==0)	return MINUS;
	
	if(strcmp(token,"*")==0)	return MULT;
	if(strcmp(token,"/")==0)	return DIV;
	
	if(strcmp(token,"<")==0)	return LSS;
	if(strcmp(token,">")==0)	return GRE;
	if(strcmp(token,"<=")==0)	return LEQ;
	if(strcmp(token,">=")==0)	return GEQ;
	if(strcmp(token,"!=")==0)	return NEQ;
	if(strcmp(token,"==")==0)	return EQL;
	
	if(strcmp(token,":")==0)	return COLON;
	if(strcmp(token,";")==0)	return SEMICN;
	if(strcmp(token,"=")==0)	return ASSIGN;
	if(strcmp(token,",")==0)	return COMMA;
	
	if(strcmp(token,"\'")==0)	return SINQUO;
	if(strcmp(token,"\"")==0)	return DBLQUO;
	if(strcmp(token,"(")==0)	return LPARENT;
	if(strcmp(token,")")==0)	return RPARENT;
	if(strcmp(token,"{")==0)	return LBRACE;
	if(strcmp(token,"}")==0)	return RBRACE;
	if(strcmp(token,"[")==0)	return LBRACK;	
	if(strcmp(token,"]")==0)return RBRACK;
	return 233;
}

int keyword()
{
	if(strcmp(token,"if")==0)		return IFTK;
	if(strcmp(token,"printf")==0)	return PRINTFTK;
	
	if(strcmp(token,"scanf")==0)	return SCANFTK;
	if(strcmp(token,"const")==0)	return CONSTTK;
	if(strcmp(token,"int")==0)		return INTTK;
	if(strcmp(token,"char")==0)		return CHARTK;
	if(strcmp(token,"do")==0)		return DOTK;
	if(strcmp(token,"while")==0)	return WHILETK;
	if(strcmp(token,"return")==0)	return RETURNTK;
	if(strcmp(token,"void")==0)		return VOIDTK;
	if(strcmp(token,"main")==0)		return MAINTK;
	if(strcmp(token,"switch")==0)	return SWITCHTK;
	if(strcmp(token,"case")==0)		return CASETK;
	
	return IDEN;
	//////查保留字表////////
}

void clear()
{
	memset(token,0,sizeof(token));
	tokennum=0;
}

int read()
{
	int i=getword();
	while(i==SPACE){
		i=getword();
	}
	tmp=i;
	return i;
}
