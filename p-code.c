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
extern struct funtable ftable[];
extern int expchar;
ptree stack[1024],p;
struct polish po[1024];
int stop=-1,potop=-1,o=-1,printf_num=0,expnum=0;//o保持和c中符号一致就行 用来比较 
char now_func[STRINGLENGTH];
void four(int type,char d[],char s1[],char s2[])
{//$8,$9负责取操作数值出来,$10负责存结果，$11(ld),$12(st)负责找结果地址,$16保存函数返回值,$17函数返回值栈
	//error 条件语句结果仅存在了寄存器10里面
	int i,j=0,k;
	char dst[STRINGLENGTH]={0},src1[STRINGLENGTH]={0},src2[STRINGLENGTH]={0};
	strcpy(dst,d);
	strcpy(src1,s1);
	strcpy(src2,s2);
	switch(type){
		case(PADD):
			printf("ADD %s %s %s\n",dst,src1,src2);
			fprintf(fa,"add $10,$%d,$%d\n",reg_load_value(src1,8),reg_load_value(src2,9));
			get_addr(dst);
			fprintf(fa,"sw $10,0($12)\n");
			fprintf(fa,"\n");

			//fprintf(fa,"",);
			break;
		case(PSUB):
			printf("SUB %s %s %s\n",dst,src1,src2);
			fprintf(fa,"sub $10,$%d,$%d\n",reg_load_value(src1,8),reg_load_value(src2,9));
			get_addr(dst);
			fprintf(fa,"sw $10,0($12)\n");
			fprintf(fa,"\n");
			break;
		case(PMUL):
			printf("MUL %s %s %s\n",dst,src1,src2);
			fprintf(fa,"mul $10,$%d,$%d\n",reg_load_value(src1,8),reg_load_value(src2,9));
			get_addr(dst);
			fprintf(fa,"sw $10,0($12)\n");
			fprintf(fa,"\n");
			break;
		case(PDIV):
			printf("DIV %s %s %s\n",dst,src1,src2);
			fprintf(fa,"div $10,$%d,$%d\n",reg_load_value(src1,8),reg_load_value(src2,9));
			get_addr(dst);
			fprintf(fa,"sw $10,0($12)\n");
			fprintf(fa,"\n");
			break;
		case(MOV):
			printf("MOV %s %s\n",dst,src1);
			get_addr(dst);
			fprintf(fa,"sw $%d,0($12)\n",reg_load_value(src1,8));
			fprintf(fa,"\n");
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

				if (expchar==1)
				{
					fprintf(fa,"li $v0,11\n");
					fprintf(fa,"move $a0 $%d\n",reg_load_value(src1,8));
					fprintf(fa,"syscall\n");
				}
				else{
					fprintf(fa,"li $v0,1\n");
					fprintf(fa,"move $a0 $%d\n",reg_load_value(src1,8));
					fprintf(fa,"syscall\n");
				}

				

				
			}
			break;
		case(LABEL):
			printf("LABEL %s\n",dst);
			fprintf(fa,"%s:\n",dst);
			break;
		case(SCANF):////////////输入数组元素k=notfound
			printf("SCANF %s %s\n",dst,src1);
			i=atoi(src1);
			k=seek(dst,now_func);
			j=table[k].value;
			if(i==INTTK&&table[k].length==0){
				fprintf(fa,"li $v0,5\n");
				fprintf(fa,"syscall\n");
				get_addr(dst);
				fprintf(fa,"sw $v0,0($12)\n");
				fprintf(fa,"\n");
			}
			else if(i==CHARTK&&table[k].length==0){
				fprintf(fa,"li $v0,12\n");
				fprintf(fa,"syscall\n");
				get_addr(dst);
				fprintf(fa,"sw $v0,0($12)\n");
				fprintf(fa,"\n");
			}
			else error(SCANF_NOINTONCHAR);
			break;
			//sp是栈顶第一个元素、fp是栈底最后一个元素的下一个,s0返回值 fprintf(fa,"");
		case(CALLEXP)://1、检查函数类型参数类型??????2、装载参数3、装载返回地址、返回值4、转移过程
			printf("CALL %s\n",dst);
			fprintf(fa,"addi $17,$17,4\n");
			fprintf(fa,"jal %s\n",dst);
			break;
		case(CALLUSE)://1、检查函数类型参数类型??????2、装载参数3、装载返回地址、返回值4、转移过程
			printf("CALL %s\n",dst);
			fprintf(fa,"addi $17,$17,4\n");
			fprintf(fa,"jal %s\n",dst);
			fprintf(fa,"subi $17,$17,4\n");
			break;
		case(RETURN)://
			printf("RETURN %s\n",dst);
			fprintf(fa,"move $16,$%d\n",reg_load_value(dst,8));
			fprintf(fa,"lw $31,0($sp)\n");
			fprintf(fa,"move $fp,$sp \n");
			fprintf(fa,"lw $sp,4($sp)\n");
			fprintf(fa,"la $12,ret($17)\n");
			fprintf(fa,"sw $16,0($12)\n");
			fprintf(fa,"jr $31\n");
			break;
			
		case(STORE)://four(STORE,t->name,tmp,"");
			printf("store %s %s\n",dst,src1);
			get_addr(src1);
			fprintf(fa,"sw $%d,0($12)\n",reg_load_value(dst,8));
			fprintf(fa,"\n");
			break;
		/*case(DIV):
			printf("DIV %s %s %s\n",dst,src1,src2);
			break;*/
		
	}
}
void get_addr(char var[])
{
	int i,j=0,k=0;
	char name[STRINGLENGTH]={0},num[STRINGLENGTH]={0};

	if(var[0]=='$'){
		delfirst(var);
		if(var[0]=='e'){
			delfirst(var);
			i=atoi(var);
			fprintf(fa,"la $12,exp+%d\n",i);
			return ;
		}
	}

	i=seek(var,now_func);
	if (i==NOTFOUND)
	{
		while (chartype(var[j])==LETTER||chartype(var[j])==DIGIT)
		{
			name[j]=var[j++];
		}
		i=seek(name,now_func);
		if (table[i].type==ARRAY_VARIABLE)
		{
			j++;
			if (var[j]=='-')///////////////////////////////////////////-0是可以的
			{
				error(ARRAY_INFO_LSS0);
				return ;
			}
			if(var[j]=='$'){
				j++;
				if(var[j]=='e'){
					j++;
					k=0;
					while (chartype(var[j])==DIGIT)
					{
						num[k++]=var[j++];
					}
					i=atoi(num);
					fprintf(fa,"la $12,exp+%d\n",i);
					fprintf(fa,"lw $12,0($12)\n");
					fprintf(fa,"mul $12,$12,4\n");
					i=seek(name,now_func);
					if (strcmp(table[i].area,"static")==0)
					{
						fprintf(fa,"la $12,static+%d($12)\n",table[i].value);
					}
					else{
						fprintf(fa,"add $12,$12,$sp\n");
						fprintf(fa,"la $12,%d($12)\n",table[i].value);
					}
					//fprintf(fa,"lw $%d,0($12)\n",n);
				}
			}
			else if (chartype(var[j])==DIGIT)
			{
				k=0;
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
						fprintf(fa,"la $12,static+%d\n",table[i].value+k*4);
					}
					else{
						fprintf(fa,"la $12,%d($sp)\n",table[i].value+k*4);
					}
					//fprintf(fa,"lw $%d,0($12)\n",n);
				}
			
			}
		}
	}
	else if (table[i].type==SIMPLE_VARIABLE||table[i].type==PARAMETER)
	{
		if (strcmp(table[i].area,"static")==0)
		{
			fprintf(fa,"la $12,static+%d\n",table[i].value);
		}
		else{
			fprintf(fa,"la $12,%d($sp)\n",table[i].value);
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
	char name[STRINGLENGTH]={0},num[STRINGLENGTH]={0};
	i=seek(src,now_func);
	//printf("load_addr %s,$11\n",src);
	if(i==NOTFOUND){
		while (chartype(src[j])==LETTER||chartype(src[j])==DIGIT)
		{
			name[j]=src[j++];
		}
		j++;
		i=seek(name,now_func);
		
		if (table[i].type==ARRAY_VARIABLE)
		{
			if (src[j]=='-')
			{
				error(ARRAY_INFO_LSS0);
				return ;
			}
			if(src[j]=='$'){
				j++;
				if(src[j]=='e'){
					j++;
					k=0;
					while (chartype(src[j])==DIGIT)
					{
						num[k++]=src[j++];
					}
					i=atoi(num);
					fprintf(fa,"la $11,exp+%d\n",i);
					fprintf(fa,"lw $11,0($11)\n");
					fprintf(fa,"mul $11,$11,4\n");
					i=seek(name,now_func);
					if (strcmp(table[i].area,"static")==0)
					{
						fprintf(fa,"la $11,static+%d($11)\n",table[i].value);
					}
					else{
						fprintf(fa,"add $11,$11,$sp\n");
						fprintf(fa,"la $11,%d($11)\n",table[i].value);
					}
					fprintf(fa,"lw $%d,0($11)\n",n);
				}
			}
			else if (chartype(src[j])==DIGIT)
			{
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
						fprintf(fa,"la $11,static+%d\n",table[i].value+k*4);
					}
					else{
						fprintf(fa,"la $11,%d($sp)\n",table[i].value+k*4);
					}
					fprintf(fa,"lw $%d,0($11)\n",n);
				}
			}
		}
		else if(table[i].type==FUNCTION){
			//fprintf(fa,"move $%d,$16\n",n);
			fprintf(fa,"la $11,ret($17)\n");
			fprintf(fa,"addi $17,$17,-4\n");
			fprintf(fa,"lw $%d,0($11)\n",n);
		} 
	}
	else if (table[i].type==SIMPLE_VARIABLE||table[i].type==PARAMETER)
	{
		if (strcmp(table[i].area,"static")==0)
		{
			fprintf(fa,"la $11,static+%d\n",table[i].value);
		}
		else{
			fprintf(fa,"la $11,%d($sp)\n",table[i].value);
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
	fprintf(fa,"sw $%d,%d($fp)\n",reg_load_value(reg,10),0);//offset从0开始
	fprintf(fa,"addi $fp,$fp,4\n");
	fprintf(fa,"\n");
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
char* deal_exp(char c[][STRINGLENGTH],int n)//c改为二层数组，需要存标识符！改输出 
{
	int i,j=0,k=0,ltop=-1,potop=-1,stop=-1,o=-1;
	char swap[3],last[50][3]={0}; 

	for(i=0;i<n;i++){
		if(ov(c[i])==-1){
			potop++;
			strcpy(po[potop].iden,c[i]);
			po[potop].oprator=-1;
		} 
		else{//括号 
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
							ltop--;
							while(ov(last[ltop])>=ov(c[i])){
								po[++potop].oprator=last[ltop--][0];
							}
							strcpy(last[++ltop],swap);
						}
						else	po[++potop].oprator=last[ltop--][0];
					}
			}
		}
	}
	for( ;ltop>-1;ltop--)
		po[++potop].oprator=last[ltop][0];
	//建二叉树
	for(i=0;i<=potop;i++){
		if(po[i].oprator<0){//如果是操作数，则建立一个单节点树并将指向他的指针推入栈中； 
			p=(ptree)malloc(sizeof(tree));
			strcpy(p->name,po[i].iden);
			p->parent=p->lchild=p->rchild=NULL;
			stack[++stop]=p;
			p->ino=-1;
			p->op=-1;
		}
		else {//就从栈中弹出指向两棵树T1和T2的指针（T1先弹出）并形成一棵新树，树根为该运算符，它的左、右子树分别指向T2和T1，然后将新树的指针压入栈中。
			p=(ptree)malloc(sizeof(tree));
			p->op=po[i].oprator;
			stack[stop]->parent=p;
			p->rchild=stack[stop--];
			stack[stop]->parent=p;
			p->ino=4*(expnum++);
			p->lchild=stack[stop--];
			p->parent=NULL;
			stack[++stop]=p;
		}
	}
	//j=10;
	while(p->parent!=NULL){
		p=p->parent;
	}
	if(p->lchild==NULL&&p->rchild==NULL&&p->parent==NULL)
		p->ino=-1;
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
			}
			else if(t->op=='-'){
				four(PSUB,tmp,inorder(t->lchild),inorder(t->rchild));
				strcpy(t->name,tmp);
			}
			else if(t->op=='*'){
				four(PMUL,tmp,inorder(t->lchild),inorder(t->rchild));
				strcpy(t->name,tmp);
			}
			else if(t->op=='/'){
				four(PDIV,tmp,inorder(t->lchild),inorder(t->rchild));
				strcpy(t->name,tmp);
			}
		}
		if (t->parent==NULL&&t->ino==-1)
		{
			t->ino=4*(expnum++);
			strcat(tmp,itoa(t->ino,inttoa,10));
			four(STORE,t->name,tmp,"");
			strcpy(t->name,tmp);

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
			k=strlentoken();//去掉反斜杠但包含双引号
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