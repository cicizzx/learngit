#include<stdio.h>
#include<string.h>
#include <stdlib.h>
#include <ctype.h>
#include"pcode.h"
#include"table.h"
#include"error.h"
#include "lexical.h"
#define STRINGLENGTH 128
extern FILE *fa;
extern FILE *fp;
extern struct tb table[];
ptree stack[1024],p;
struct polish po[1024];
int stop=-1,potop=-1,o=-1,printf_num=0;//o���ֺ�c�з���һ�¾��� �����Ƚ� 
char now_func[STRINGLENGTH];
void four(int type,char d[],char s1[],char s2[])
{//$8,$9����ȡ������ֵ����,$10���������$11�����ҽ����ַ
	//error ����������������˼Ĵ���10����
	int i,j,k;
	char dst[STRINGLENGTH]={0},src1[STRINGLENGTH]={0},src2[STRINGLENGTH]={0};
	strcpy(dst,d);
	strcpy(src1,s1);
	strcpy(src2,s2);
	switch(type){
		case(PADD):
			printf("ADD %s %s %s\n",dst,src1,src2);
			fprintf(fa,"add $10,$%d,$%d\n",reg_load_value(src1,8),reg_load_value(src2,9));
			get_addr(dst);
			fprintf(fa,"sw $10,0($11)\n");
			//fprintf(fa,"",);
			break;
		case(PSUB):
			printf("SUB %s %s %s\n",dst,src1,src2);
			fprintf(fa,"sub $10,$%d,$%d\n",reg_load_value(src1,8),reg_load_value(src2,9));
			get_addr(dst);
			fprintf(fa,"sw $10,0($11)\n");
			break;
		case(PMUL):
			printf("MUL %s %s %s\n",dst,src1,src2);
			fprintf(fa,"mul $10,$%d,$%d\n",reg_load_value(src1,8),reg_load_value(src2,9));
			get_addr(dst);
			fprintf(fa,"sw $10,0($11)\n");
			break;
		case(PDIV):
			printf("DIV %s %s %s\n",dst,src1,src2);
			fprintf(fa,"div $10,$%d,$%d\n",reg_load_value(src1,8),reg_load_value(src2,9));
			get_addr(dst);
			fprintf(fa,"sw $10,0($11)\n");
			break;
		case(MOV):
			printf("MOV %s %s\n",dst,src1);
			get_addr(dst);
			fprintf(fa,"sw $%d,0($11)\n",reg_load_value(src1,8));
			break;
		case(PEQL):
			printf("EQL %s %s %s\n",dst,src1,src2);
			fprintf(fa,"seq $%d,$%d,$%d\n",reg_load_value(dst,10),reg_load_value(src1,8),reg_load_value(src2,9));
			break;
		case(PNEQ):
			printf("NEQ %s %s %s\n",dst,src1,src2);
			fprintf(fa,"sub $%d,$%d,$%d\n",reg_load_value(dst,10),reg_load_value(src1,8),reg_load_value(src2,9));
			break;
		case(PLSS):
			printf("LSS %s %s %s\n",dst,src1,src2);
			fprintf(fa,"slt $%d,$%d,$%d\n",reg_load_value(dst,10),reg_load_value(src1,8),reg_load_value(src2,9));
			break;
		case(PLEQ):
			printf("LEQ %s %s %s\n",dst,src1,src2);
			fprintf(fa,"sle $%d,$%d,$%d\n",reg_load_value(dst,10),reg_load_value(src1,8),reg_load_value(src2,9));
			break;
		case(PGRE):
			printf("GRE %s %s %s\n",dst,src1,src2);
			fprintf(fa,"sgt $%d,$%d,$%d\n",reg_load_value(dst,10),reg_load_value(src1,9),reg_load_value(src2,8));
			break;
		case(PGEQ):
			printf("GEQ %s %s %s\n",dst,src1,src2);
			fprintf(fa,"sge $%d,$%d,$%d\n",reg_load_value(dst,10),reg_load_value(src1,9),reg_load_value(src2,8));
			break;
		case(BEQ):
			printf("BEQ %s %s %s\n",dst,src1,src2);
			fprintf(fa,"beq $%d,$%d,%s\n",reg_load_value(src1,8),reg_load_value(src2,9),dst);
			break;
		case(J):
			printf("J %s\n",dst);
			fprintf(fa,"j %s\n",dst);
			break;
		case(BNE):
			printf("BNE %s %s %s\n",dst,src1,src2);
			fprintf(fa,"bne $%d,$%d,%s\n",reg_load_value(src1,8),reg_load_value(src2,9),dst);
			break;
		case(PRINTF):
			printf("PRINTF %s %s\n",dst,src1);
			if(strcmp(dst,"")!=0){
				fprintf(fa,"li $v0,4\n");
				fprintf(fa,"la $a0,String%d\n",printf_num++);
				fprintf(fa,"syscall\n");
			}
			if(strcmp(src1,"")!=0){
				fprintf(fa,"li $v0,1\n");
				fprintf(fa,"move $a0 $%d\n",reg_load_value(src1,8));
				fprintf(fa,"syscall\n");
			}
			break;
		case(LABEL):
			printf("LABEL %s\n",dst);
			fprintf(fa,"%s:\n",dst);
			break;
		case(SCANF):
			printf("SCANF %s %s\n",dst,src1);
			i=atoi(src1);
			k=seek(dst,now_func);
			j=table[k].value;
			if(i==INTTK&&table[k].length==0){
				fprintf(fa,"li $v0,5\n");
				fprintf(fa,"syscall\n");
				get_addr(dst);
				fprintf(fa,"sw $v0,0($11)\n");
			}
			else if(i==CHARTK&&table[k].length==0){
				fprintf(fa,"li $v0,12\n");
				fprintf(fa,"syscall\n");
				get_addr(dst);
				fprintf(fa,"sw $v0,0($11)\n");
			}
			else error(SCANF_NOINTONCHAR);
			break;
			//sp��ջ����һ��Ԫ�ء�fp��ջ�����һ��Ԫ��,v0����ֵ fprintf(fa,"");
		case(CALL)://1����麯�����Ͳ�������??????2��װ�ز���3��װ�ط��ص�ַ������ֵ4��ת�ƹ���
			printf("CALL %s\n",dst);
			//strcpy(now_func,dst);
			fprintf(fa,"jal %s\n",dst);
			break;
		case(RETURN)://
			printf("RETURN %s\n",dst);
			fprintf(fa,"lw $31,0($sp)\n");
			fprintf(fa,"lw $v0,4($sp)\n");
			fprintf(fa,"subi $fp,$sp,4 \n");
			fprintf(fa,"lw $sp,8($sp)\n");
			fprintf(fa,"jr $31\n");
			break;
		/*case(DIV):
			printf("DIV %s %s %s\n",dst,src1,src2);
			break;*/
		
	}
}
void get_addr(char var[])
{
	int i,j=0,k=0;
	char name[STRINGLENGTH]={0};

	if(var[0]=='$'){
		delfirst(var);
		/*if (isdigit(var[0]))
		{
			i=atoi(var);
			return i;
		}*/
		if(var[0]=='e'){
			delfirst(var);
			i=atoi(var);
			fprintf(fa,"la $11,exp+%d\n",i);
			return ;
		}
	}

	i=seek(var,now_func);
	if (i==NOTFOUND)
	{
		while (chartype(var[j])==LETTER)
		{
			name[j]=var[j++];
		}
		i=seek(name,now_func);
		if (table[i].type==ARRAY_VARIABLE)
		{
			j++;
			if (var[j]=='-')
			{
				error(ARRAY_INFO_LSS0);
				return ;
			}
			while (chartype(var[j])==DIGIT)
			{
				k=k*10+var[j++]-'0';
			}
			if (k>table[i].length)
			{
				error(ARRAY_OVERFLOW);
				return ;
			}
			else{
				if (strcmp(table[i].area,"static")==0)
				{
					fprintf(fa,"li $11,%d\n",table[i].value+k*4);
				}
				else{
					fprintf(fa,"la $11,stack+%d\n",table[i].value+k*4);
				}
				//fprintf(fa,"lw $%d,0($%d)\n",n,n);
			}
		}
	}
	else if (table[i].type==SIMPLE_VARIABLE)
	{
		if (strcmp(table[i].area,"static")==0)
		{
			fprintf(fa,"la $11,static+%d\n",table[i].value);
		}
		else{
			fprintf(fa,"la $11,stack+%d\n",table[i].value);
		}
	}
	else{
		error(ASSIGN_WRONG_LEFT);
	}
}
int reg_load_value(char c[],const int n)
{
	char src[STRINGLENGTH]={0};
	int i=0;
	strcpy(src,c);
	if(src[0]=='$'){
		delfirst(src);
		if (isdigit(src[0]))
		{
			i=atoi(src);
			return i;
		}
		else{
			delfirst(src);
			i=atoi(src);
			fprintf(fa,"la $%d,exp+%d\n",n,i);
			fprintf(fa,"lw $%d,0($%d)\n",n,n);
			return n;
		}
	}
	if(isinteger(src)){
		fprintf(fa,"li $%d,%s\n",n,src);
	}
	else if(src[0]=='\''){
		fprintf(fa,"li $%d,%s\n",n,src);
	}
	else if(src[0]=='+'){
		delfirst(src);
		load_value(src,n,1);
	}
	else if(src[0]=='-'){
		delfirst(src);
		load_value(src,n,0);
	}
	else if(chartype(src[0])==LETTER){
		i=seek(src,now_func);
		load_value(src,n,1);
	}
	return n;
}
void load_value(char src[],int n,int positive)
{
	int i,j=0,k=0;
	char name[STRINGLENGTH]={0};
	i=seek(src,now_func);
	if(i==NOTFOUND){
		while (chartype(src[j])==LETTER)
		{
			name[j]=src[j++];
		}
		j++;
		i=seek(name,now_func);
		if(table[i].type==FUNCTION){
			fprintf(fa,"move $%d,$v0\n",n);  
		} 
		else if (table[i].type==ARRAY_VARIABLE)
		{
			if (src[j]=='-')
			{
				error(ARRAY_INFO_LSS0);
				return ;
			}
			while (chartype(src[j])==DIGIT)
			{
				k=k*10+src[j++]-'0';
			}
			if (k>table[i].length)
			{
				error(ARRAY_OVERFLOW);
				return ;
			}
			else{
				if (strcmp(table[i].area,"static")==0)
				{
					fprintf(fa,"li $11,%d\n",table[i].value+k*4);
				}
				else{
					fprintf(fa,"la $11,stack+%d\n",table[i].value+k*4);
				}
				fprintf(fa,"lw $%d,0($11)\n",n);
			}
		}
	}
	else if (table[i].type==SIMPLE_VARIABLE)
	{
		if (strcmp(table[i].area,"static")==0)
		{
			fprintf(fa,"la $11,static+%d\n",table[i].value);
		}
		else{
			fprintf(fa,"la $11,stack+%d\n",table[i].value);
		}
		fprintf(fa,"lw $%d,0($11)\n",n);

	}	
	else if(table[i].type==CONSTANT){
		fprintf(fa,"li $%d,%d\n",n,table[i].value);
	}

	if (positive==0){
		fprintf(fa,"sub $%d,$0,$%d\n",n,n);
	}

}
void load_para(char reg[],int offset)
{
	fprintf(fa,"sw $%d,%d($sp)\n",reg_load_value(reg,10),12+offset);
}
void delfirst(char src[])
{
	int i=0;
	for(i=1;i<strlen(src);i++){
		src[i-1]=src[i];
	}
	src[i-1]=0;
}
int isinteger(char src[])
{
	int i=0;
	if (src[0]=='+'||src[0]=='-'||isdigit(src[0])){
		for(i=1;i<strlen(src);i++){
			if(!isdigit(src[i]))
				return 0;
		}
	}
	else return 0;
	return 1;
}
char* deal_exp(char c[][STRINGLENGTH],int n)//c��Ϊ�������飬��Ҫ���ʶ��������� 
{
	int i,j=0,k=0,ltop=-1,potop=-1,stop=-1,o=-1;
	char swap[3],last[50][3]={0}; 

	for(i=0;i<n;i++){
		if(ov(c[i])==-1){
			potop++;
			strcpy(po[potop].iden,c[i]);
			po[potop].oprator=-1;
		} 
		else{//���� 
			o++;
			if(c[i][0]=='('){
				i=bra(i,c);//c[i-1]==')'||
			}
			else{
				strcpy(last[++ltop],c[i]);
				//last[++ltop]=c[i][0];					
					if(ltop==-1||ov(c[i])>ov(last[ltop-1]))
						;
//					else if(c[i]=='('){
//						i=bra(i);
//					}
					else{
						if(ov(last[ltop-1])>=ov(c[i])){
							strcpy(swap,c[i]);
							//swap=c[i][0];
							ltop--;
							while(ov(last[ltop])>=ov(c[i])){
								po[++potop].oprator=last[ltop--][0];
							}
							strcpy(last[++ltop],swap);
							//last[++ltop]=swap;
						}
						else	po[++potop].oprator=last[ltop--][0];
					}
			}
		}
	}
	for( ;ltop>-1;ltop--)
		po[++potop].oprator=last[ltop][0];
	//��������
	j=1;
	for(i=0;i<=potop;i++){
		if(po[i].oprator<0){//����ǲ�����������һ�����ڵ�������ָ������ָ������ջ�У� 
			p=(ptree)malloc(sizeof(tree));
			strcpy(p->name,po[i].iden);
			p->parent=p->lchild=p->rchild=NULL;
			stack[++stop]=p;
			p->ino=-1;
			p->op=-1;
		}
		else {//�ʹ�ջ�е���ָ��������T1��T2��ָ�루T1�ȵ��������γ�һ������������Ϊ����������������������ֱ�ָ��T2��T1��Ȼ��������ָ��ѹ��ջ�С�
			p=(ptree)malloc(sizeof(tree));
			p->op=po[i].oprator;
			stack[stop]->parent=p;
			p->rchild=stack[stop--];
			//p->rchild->ino=10;
			stack[stop]->parent=p;
			p->lchild=stack[stop--];
			//p->lchild->ino=11;
			p->parent=NULL;
			stack[++stop]=p;
			p->ino=4*(j++);
		}
	}
	//j=10;
	while(p->parent!=NULL){
		p=p->parent;
	}
	//p->ino=4*(j++);
	//printf("\n%d",inorder(p));
	return inorder(p);
}
int ov(char a[])
{
	if(strcmp(a,"(")==0||strcmp(a,")")==0)
		return 3;
	else if(strcmp(a,"*")==0||strcmp(a,"/")==0)
		return 2;
	else if(strcmp(a,"+")==0||strcmp(a,"-")==0)
		return 1;
	else return -1;
}
int bra(int i,char c[][STRINGLENGTH])
{
	char last[50][3],swap[3];
	int ltop=-1;
	i++;
	while(c[i][0]!=')'){
		if(ov(c[i])==-1){
				potop++;
				strcpy(po[potop].iden,c[i]);
				po[potop].oprator=0;
			} 
		else {
			o++;
			if(c[i][0]=='('){
				i=bra(i,c);
			}
			else{
				strcpy(last[++ltop],c[i]);
				//last[++ltop]=c[i][0];					
					if(ltop==-1||ov(c[i])>ov(last[ltop-1]))
						;
					else if(c[i][0]=='('){
						i=bra(i,c);
					}
					else{
						if(ov(last[ltop-1])>=ov(c[i])){
							strcpy(swap,c[i]);
							//swap=c[i][0];
							ltop--;
							while(ov(last[ltop])>=ov(c[i])){
								po[++potop].oprator=last[ltop--][0];
							}
							//last[++ltop]=swap;
							strcpy(last[++ltop],swap);
						}
						else	po[++potop].oprator=last[ltop--][0];
					}
			}
		}
		i++;
	}
	o++;
	for( ;ltop>=0;ltop--)
		po[++potop].oprator=last[ltop][0];
	return i; 
}
char* inorder(ptree t)
{
	char tmp[STRINGLENGTH]={0};
	char inttoa[STRINGLENGTH]={0};
	strcpy(tmp,"$e");
	if(t!=NULL){
		if(t->op>0){
			strcat(tmp,itoa(t->ino,inttoa,10));
			if(t->op=='+'){
				four(PADD,tmp,inorder(t->lchild),inorder(t->rchild));
				strcpy(t->name,tmp);
				//return ;
			}
			else if(t->op=='-'){
				four(PSUB,tmp,inorder(t->lchild),inorder(t->rchild));
				strcpy(t->name,tmp);
				//return tmp;
			}
			else if(t->op=='*'){
				four(PMUL,tmp,inorder(t->lchild),inorder(t->rchild));
				strcpy(t->name,tmp);
				//return tmp;
			}
			else if(t->op=='/'){
				four(PDIV,tmp,inorder(t->lchild),inorder(t->rchild));
				strcpy(t->name,tmp);
				//return tmp;
			}
		}
		return t->name;
}
	return "";
}

void pre_printf()
{
	int i=0,j=0,k=0;
	fprintf(fa,".data\n");
	getsym();
	while (read()!=EOFFILE)
	{
		if(tmp==STRCON){
			fprintf(fa,"String%d:.asciiz %s\n",i++,token);
			k=strlentoken();//ȥ����б�ܵ�����˫����
			j=j-1+k;
		}
	}
	fprintf(fa,".space %d\n",4-j%4);
	line=0;
	fseek(fp,0,SEEK_SET);
}
int strlentoken()
{
	int i,res=0;
	for(i=0;i<strlen(token);i++){
		if(token[i]=='\\'){
			i++;
		}
		res++;
	}
	return res;
}