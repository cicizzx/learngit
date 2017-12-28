#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include"pcode.h"
#include"grammer.h"
#include"lexical.h"
#include "table.h"
#include"error.h"
#define STRINGLENGTH 128
extern int tokennum,tmp,tptr,text=0;
extern char token[];
extern struct tb table[];
extern FILE *fa;
extern char now_func[];
extern int expnum;
int ifnum=0,donum=0,casenum=0,switchnum=0,offset=0,expchar=1;
char all[]="static";

//������    ::= �ۣ�����˵�����ݣۣ�����˵������{���з���ֵ�������壾|���޷���ֵ�������壾}����������
void program()
{
	int rflag=0;
	int type;
	char name[STRINGLENGTH]={0};
	getsym();
	read();
	
	while(tmp==CONSTTK){
		//��һ��const�Ѿ����� 
		const_state(all);
	} 
		//�����Ǳ���˵��Ҳ�������з���ֵ����
	if(tmp==INTTK||tmp==CHARTK){
		var_or_func();	//ǰyes��no 	
		}
	if(!text){
		text=1;
		fprintf(fa,"static:	.space %d\n",offset);
		fprintf(fa,"ret:	.space 512\n");
		fprintf(fa,"exp:	.space 10240\n");
		fprintf(fa,"stack:	.space 512\n");
		fprintf(fa,".text\n");
		fprintf(fa,"la $sp,stack\n");
		fprintf(fa,"la $fp,stack\n");
		fprintf(fa,"li $17,-4\n");
		four(CALLUSE,"main","","");
		fprintf(fa,"li $v0 10\n");
		fprintf(fa,"syscall\n");

	}
	while(tmp==INTTK||tmp==CHARTK||tmp==VOIDTK){
		if(tmp==INTTK||tmp==CHARTK){
			type=tmp;
			read();
		if(tmp==IDEN){
			strcpy(name,token);
			read();
			//�з���ֵ���� 
			if(tmp==LPARENT){
				//output(DEF_VALFUNC);
				def_func(name,type);
				
			}
	}
	}	//�޷���ֵ���� 
	if(tmp==VOIDTK){
			if(read()==IDEN&&keyword()!=MAINTK){
				//output(DEF_VOIDFUNC);
				strcpy(name,token);
				if(read()==LPARENT)
						def_func(name,VOIDTK);				
			}
			//������ 
			else if(keyword()==MAINTK){
				//output(MAIN_FUNC);
				main_func();
				if(read()!=EOFFILE)
					error(CODE_AFTER_MAIN);
			}   
		} 
	}
	
		

}
void var_or_func()
{
	char name[STRINGLENGTH];
	int length;

	int isint=(tmp==INTTK);
		read();
		if(tmp==IDEN){
			strcpy(name,token);
			read();
			//�з���ֵ���� 
			if(tmp==LPARENT){
				//output(DEF_VALFUNC);
				def_func(name,(isint?INTTK:CHARTK));
				
			}
			
			//����˵��
			else if(token[0]=='['||token[0]==','||token[0]==';'){
				//output(VAR_STATE);				
				switch(token[0]){
					case('['):
						if(read()==INTCON){
							if(check(name,all)==NOTFOUND) {
								entertablearray(ARRAY_VARIABLE,name,offset,(isint?INTTK:CHARTK),all,atoi(token));
								offset=offset+atoi(token)*4;
							}
								
							else{
								error(RE_DECLARATION);
							}
							
							read(); 
							if(token[0]=']'){
								read();
								if(token[0]==','){
									do{
										if(read()==IDEN){
											strcpy(name,token);											
											read();
											if(token[0]==','){
												if(check(name,all)==NOTFOUND) {
													entertable(SIMPLE_VARIABLE,name,offset,(isint?INTTK:CHARTK),all);
													offset=offset+4;
												}
												else{
													error(RE_DECLARATION);
												}
												
												continue;
											} 
											else if(token[0]=='['){
												if((read())==INTCON){
													if(check(name,all)==NOTFOUND) {
														entertablearray(ARRAY_VARIABLE,name,offset,(isint?INTTK:CHARTK),all,atoi(token));
														offset=offset+atoi(token)*4;
													}
														
													else{
														error(RE_DECLARATION);
													}
													read();
													if(token[0]==']'){
														read();
														if(token[0]==','){
															continue;
														} 
													}
													else {
														error(BRACK_DISMATCH);
													}
												}
											}
										} 
									}while(token[0]==',');
								}
								if(token[0]==';'){
									read();
									if(tmp==INTTK||tmp==CHARTK)
										var_or_func();
								}
								else{
									error(MISSING_SEMICN);
								}
							}
							else {
								error(BRACK_DISMATCH);
							}
						}
						break;
					case(','):
						entertable(SIMPLE_VARIABLE,name,offset,(isint?INTTK:CHARTK),all);
						offset=offset+4;
							do{
								if(read()==IDEN){
									strcpy(name,token);
									read();
									if(token[0]==','){
										if(check(name,all)==NOTFOUND) {
											entertable(SIMPLE_VARIABLE,name,offset,(isint?INTTK:CHARTK),all);
											offset=offset+4;
										}
										else{
											error(RE_DECLARATION);
										}
										
										continue;
									} 
									else if(token[0]=='['){
										if(read()==INTCON){
											if(check(name,all)==NOTFOUND) {
												entertablearray(ARRAY_VARIABLE,name,offset,(isint?INTTK:CHARTK),all,atoi(token));
												offset=offset+atoi(token)*4;
											}
												
											else{
												error(RE_DECLARATION);
											}
											read();
											if(token[0]==']'){
												read();
												if(token[0]==','){
													continue;
												} 
											}
											else {
												error(BRACK_DISMATCH);
											}
										}
									}
								} 
							}while(token[0]==',');
							if(token[0]==';'){
								read();
									if(tmp==INTTK||tmp==CHARTK)
										var_or_func();
							}
							else{
									error(MISSING_SEMICN);
								}
							break;
					case(';'):
						if(read()==(INTTK||CHARTK)){						
							//read();
								if(tmp==INTTK||tmp==CHARTK)
									var_or_func();
						}
							break;
				}
				//read();//maybe error 
			}
		} 
} 
// ������˵���� ::=  const���������壾;{ const���������壾;}
void const_state(char area[])
{
	
	do{
		def_const(area);
		if(token[0]!=';'){
			error(MISSING_SEMICN);
		}
		read();
	}while(keyword()==CONSTTK);
	
}
//���������壾   ::=   int����ʶ��������������{,����ʶ��������������}
                            //| char����ʶ���������ַ���{,����ʶ���������ַ���}
void def_const(char area[])
{
	char name[STRINGLENGTH];
	int positive=0;
	//output(CONST_STATE);
	read();
	if(keyword()==INTTK){
		do{
			if(read()==IDEN){
				strcpy(name,token);				
				if(read()==ASSIGN){
					positive=integer();
					if(check(name,area)==NOTFOUND) 
						entertable(CONSTANT,name,positive*atoi(token),INTTK,area);
					else{
						error(RE_DECLARATION);
					}
					read(); //�������� 
				}
			}
		}while(token[0]==',');		
	}
	else if(keyword()==CHARTK){
		do{
			if(read()==IDEN){
				strcpy(name,token);
				if(read()==ASSIGN){
					if(read()==CHARCON){
						if(check(name,area)==NOTFOUND) 
							entertable(CONSTANT,name,token[1],CHARTK,area);
						else{
							error(RE_DECLARATION);
						}
						
						read();
						
					}
				}
			}
		}while(token[0]==',');
		
	}
	else if(keyword()==IDEN){
		strcpy(name,token);
		error(WRONG_CONST_MISSING_TK);
		if(read()==ASSIGN){
					read();
					if(tmp==PLUS||tmp==MINUS){
						positive=(tmp==PLUS)?1:-1;	
						if(read()==INTCON) {
							entertable(CONSTANT,name,positive*atoi(token),INTTK,area);
							read();
						}
						while(token[0]==','){
							if(read()==IDEN){
								if(read()==ASSIGN){
									positive=integer();
									if(check(name,area)==NOTFOUND) 
										entertable(CONSTANT,name,positive*atoi(token),INTTK,area);
									else{
										error(RE_DECLARATION);
									}
									
									read(); //�������� 
								}
							}
						}
					}
					else if(tmp==INTCON){
						entertable(CONSTANT,name,atoi(token),INTTK,area);
						read();
						while(token[0]==','){
							if(read()==IDEN){
								if(read()==ASSIGN){
									positive=integer();
									if(check(name,area)==NOTFOUND) 
										entertable(CONSTANT,name,positive*atoi(token),INTTK,area);
									else{
										error(RE_DECLARATION);
									}
									
									read(); //�������� 
								}
							}
						}
					}
					else if(tmp==CHARCON){
						read();
						while(token[0]==','){
							if(read()==IDEN){
								if(read()==ASSIGN){
									if(read()==CHARCON){
										if(check(name,area)==NOTFOUND) 
											entertable(CONSTANT,name,token[1],CHARTK,area);
										else{
											error(RE_DECLARATION);
										}
										
										read();
										
									} 
								}
							}
						}
					}
				}
	}
}
//��������        ::= �ۣ������ݣ��޷�������������
int integer()
{
	read();
	if(token[0]=='+'){
		
		if(read()==INTCON){
			return 1;
		}
	}
	else if(token[0]=='-'){
		
		if(read()==INTCON){
			return -1;
		}
	}
	else if(tmp==INTCON){
		return 1;
	}
	return 0;
}
//������˵����  ::= ���������壾;{���������壾;}
void var_state(char area[],int *offfun)
{
	do{
		def_var(area,offfun);
		read();
	}while(tmp==INTTK||tmp==CHARTK);
}
//���������壾  ::= �����ͱ�ʶ����(����ʶ����|����ʶ������[�����޷�����������]��)
				//{,(����ʶ����|����ʶ������[�����޷�����������]��) }
void def_var(char area[],int *offfun)
{
	int isint=(tmp==INTTK)?1:0;
	char name[STRINGLENGTH];
	//output(VAR_STATE);
	if(tmp==INTTK||tmp==CHARTK){
		do{
			if(read()==IDEN){
				strcpy(name,token);
				read();
				if(token[0]==','){
					if(check(name,area)==NOTFOUND){
						entertable(SIMPLE_VARIABLE,name,*offfun,(isint?INTTK:CHARTK),area);
						*offfun+=4;
					}
						
					else{
						error(RE_DECLARATION);
					} 
					continue;
				} 
				else if(token[0]=='['){
					if(read()==INTCON){
						if(check(name,area)==NOTFOUND){
							entertablearray(ARRAY_VARIABLE,name,*offfun,(isint?INTTK:CHARTK),area,atoi(token));
							*offfun=*offfun+atoi(token)*4;
						}
						else{
							error(RE_DECLARATION);
						} 
						
						read();
						if(token[0]==']'){
							read();
							if(token[0]==','){
								continue;
							} 
						}
						else {
								error(BRACK_DISMATCH);
							}
					}
				}
				else if(token[0]==';'){
					if(check(name,area)==NOTFOUND){
						entertable(SIMPLE_VARIABLE,name,*offfun,(isint?INTTK:CHARTK),area);
						*offfun+=4;
					}

					else{
						error(RE_DECLARATION);
					} 
					continue;
				} 
			} 
		}while(token[0]==',');
	}
	
}
//���ޡ��з���ֵ�������壾  ::=  ������ͷ������(������������)�� ��{����������䣾��}��
void def_func(char name[],int type) 
{
	int para1=tptr+1;
	int offfun=8;
	int paranum=para_table(name,&offfun);	
	strcpy(now_func,name);
	if(!text){
		text=1;
		fprintf(fa,"static:	.space %d\n",offset);
		fprintf(fa,"ret:	.space 1024\n");
		fprintf(fa,"exp:	.space 10240\n");
		fprintf(fa,"stack:	.space 512\n");
		fprintf(fa,".text\n");
		fprintf(fa,"la $sp,stack\n");
		fprintf(fa,"la $fp,stack\n");
		fprintf(fa,"li $17,-4\n");
		four(CALLUSE,"main","","");
		fprintf(fa,"li $v0 10\n");
		fprintf(fa,"syscall\n");

	}
	four(LABEL,name,"","");
	if(check(name,all)==NOTFOUND){

		entertablefun(type,name,paranum,para1);
		entertable(FUNCTION,name,0,type,all);
	
	}
	else{
		error(RE_DECLARATION);
	} 
	
	if(token[0]==')'){
		read();
		if(token[0]=='{'){
			com_state(name,&offfun);
		}
		if(read()==RBRACE)	;
	}
	four(RETURN,"","","");
	//over
}
//��������䣾   ::=  �ۣ�����˵�����ݣۣ�����˵�����ݣ�����У�
void com_state(char area[],int *offfun)
{
	//ifnum=0;donum=0;switchnum=0;
	read();
	while(tmp==CONSTTK){
		const_state(area);
	} 
	if(tmp==INTTK||tmp==CHARTK){
		var_state(area,offfun);			
		}
	fprintf(fa,"subi $fp,$fp,%d\n",sp_offset(area)+8);
	fprintf(fa,"sw $ra,0($fp)\n");//return address
	fprintf(fa,"sw $sp,4($fp)\n");
	fprintf(fa,"move $sp,$fp\n");
	fprintf(fa,"addi $fp,$fp,%d\n",fp_offset(area)+8);
	statements(area); 
	
}
//��������    ::=  �����ͱ�ʶ��������ʶ����{,�����ͱ�ʶ��������ʶ����}| ���գ�
int para_table(char func[],int *offfun)
{
	int type;
	int num=0;
	do{
		read();
		if(tmp==INTTK||tmp==CHARTK){
			type=tmp;
			read();			
			if(tmp==IDEN){
				if(check(token,func)==NOTFOUND){
					entertable(PARAMETER,token,*offfun,type,func);
					(*offfun)+=4;}
				else{
					error(RE_DECLARATION);
				} 
				
				num++;
				read();
			}
		}
	}while(token[0]==',');
	return num;
}
//����������    ::= void main��(����)�� ��{����������䣾��}��
void main_func()
{
	int offfun=8;
	strcpy(now_func,"main");
	four(LABEL,"main","","");
	if(read()==LPARENT){
		if(read()==RPARENT){
			if(read()==LBRACE){
				com_state(now_func,&offfun);
				//poptable("main");
			}
		}
	}
	four(RETURN,"","","");
}
//�����ʽ��    ::= �ۣ������ݣ��{���ӷ�����������}
//�ѱ��ʽ��ÿ���������Ͳ������ֿ����浽exp�� 
char* expression(char func[])
{
	char exp[1024][STRINGLENGTH]={0};
	int e=0,i=0;
	expchar=1;
	if(tmp==PLUS||tmp==MINUS){
		strcpy(exp[(e)++],token);
		e--;
		read();
		i=(i==1?1:term(func,exp,&e));
	}
	if(tmp==IDEN||tmp==INTCON||tmp==CHARCON||tmp==LPARENT){
		i=(i==1?1:term(func,exp,&e));
	}
	while(tmp==PLUS||tmp==MINUS){
		//strcpy(exp[(e)++],token);
		read();
		i=(i==1?1:term(func,exp,&e));
	}
	//memset(exp[e-1],0,sizeof(exp[e-1]));
	if(i==0)
		return deal_exp(exp,e-1);
	else return "error";
} 
//���     ::= �����ӣ�{���˷�������������ӣ�}
int term(char func[],char exp[][STRINGLENGTH],int *e)
{
	do{
		if(tmp==MULT||tmp==DIV){
			read();
		}
		if(factor(func,exp,e))	return 1;
		//read();//Ԥ�� 
	}while(tmp==MULT||tmp==DIV);
	return 0;
}
//�����ӣ�    ::= ����ʶ����������ʶ������[�������ʽ����]������������|���ַ�����
//				���з���ֵ����������䣾|��(�������ʽ����)��
int factor(char func[],char exp[][STRINGLENGTH],int *e)
{
	int t,i;
	char name[STRINGLENGTH]={0},funcname[STRINGLENGTH]={0};
	
	switch(tmp){
		case(IDEN):
			if((i=seek(token,func))!=NOTFOUND)
				;
			else error(WITHOUT_DECLARATION);
			if(table[i].kind==INTTK&&expchar==1){
				expchar=0;
			}
			strcpy(name,token);
			strcpy(funcname,token);
			read();
			
			if(tmp==LBRACK){//����ʶ������[�������ʽ����]��
				strcat(name,token); 
				read();
				t=expchar;
				strcat(name,expression(func));
				expchar=t;
				if(tmp==RBRACK){
					strcat(name,token);
					strcat(exp[(*e)++],name);
					read();
					strcpy(exp[(*e)++],token);
				}
				else {
					error(BRACK_DISMATCH);
						}
			}
			else if(tmp==LPARENT) {//���з���ֵ����������䣾
				strcat(name,token);//����copy��һ��������
				t=expchar;
				val_para(func,name);//yes
				name[strlen(name)-2]=0;
				four(CALLEXP,funcname,"","");
				expchar=t;
				if(tmp==RPARENT)	{
					strcat(exp[(*e)++],name);
					read();
					strcpy(exp[(*e)++],token);
				}
				else error(WRONG_EXPRESSION);
			}
			else{//iden
				strcat(exp[(*e)++],name);
				strcpy(exp[(*e)++],token);
				if(tmp==SEMICN||tmp==RPARENT||tmp==COMMA||tmp==RBRACK||(tmp>=EQL&&tmp<=NEQ))
					;//(*e)++;
			}
			break;	
		case(PLUS)://����
			strcpy(exp[(*e)],token);
				if(read()==INTCON){
					if (expchar==1)
					{
						expchar=0;
					}
					strcat(exp[(*e)++],token);
				}
			read();
			strcpy(exp[(*e)++],token);
			break;
		case(MINUS)://���� 
			strcpy(exp[(*e)],token);
				if(read()==INTCON){
					if (expchar==1)
					{
						expchar=0;
					}
					strcat(exp[(*e)++],token);
				}
			read();
			strcpy(exp[(*e)++],token);
			break;
		case(INTCON)://���� 
			if (expchar==1)
			{
				expchar=0;
			}
			strcat(exp[(*e)++],token);
			read();
			strcpy(exp[(*e)++],token);
			break;
		case(CHARCON):
			if(expchar==1){
				expchar=0;
			}
			strcpy(exp[(*e)++],token);
			read();
			strcpy(exp[(*e)++],token);
			break;
		case(LPARENT):
			read();
			strcpy(exp[(*e)++],expression(func));
			if(tmp==RPARENT) {
				read();
				strcpy(exp[(*e)++],token);
			}		    
			else error(WRONG_EXPRESSION);
			break;
			/*strcpy(name,token);
			read();
			strcat(name,expression(func));
			if(tmp==RPARENT) {
			strcat(name,token);
			strcpy(exp[(*e)++],name);
			read();
			strcpy(exp[(*e)++],token);

			}		    
			else error(WRONG_EXPRESSION);*/		
			break;
		default:
			error(WRONG_EXPRESSION);
			read();
			return 1;
	}
	return 0;
}
/*����䣾    ::= ��������䣾����ѭ����䣾��<������>|��{��������У���}����
				���з���ֵ����������䣾; |���޷���ֵ����������䣾;������ֵ��䣾;
					��������䣾;����д��䣾;�����գ�;����������䣾;*/
void statement(char func[])
{
	char name[STRINGLENGTH]={0},funcname[STRINGLENGTH]={0};
	//expnum=0;
	switch(tmp){
		case(IFTK):
			if_state(func);
			break;
		case(DOTK):
			do_while(func);
			break;
		case(SWITCHTK):
			switch_state(func);
			break;
		case(LBRACE):
			read();
			statements(func);
			break;
		case(VOIDTK):
			use_voidfunc(func);
			break;
		case(IDEN)://assign,func
			if(seek(token,func)==NOTFOUND)	error(WITHOUT_DECLARATION);
			strcpy(name,token);
			strcpy(funcname,token);
			read();
			if(tmp==ASSIGN||tmp==LBRACK)
				assignment(func,name);
			else if(tmp==LPARENT){
				use_valfunc(func,name);
				four(CALLUSE,funcname,"","");
			}
			break;
		case(SCANFTK):
			scanf_state(func);
			break;
		case(PRINTFTK):
			printf_state(func);
			break;
		case(RETURNTK):
			return_state(func);
			break;
		case(SEMICN):
			//output(EMPTY);
			break;
	}
}
//����ֵ��䣾   ::=  ����ʶ�����������ʽ��|����ʶ������[�������ʽ����]��=�����ʽ��
void assignment(char func[],char name[])
{
	char src[STRINGLENGTH];
	//output(ASSIGNMENT);
	if(tmp==ASSIGN){
		read();
		strcpy(src,expression(func));//
	}
	else if(tmp==LBRACK){
		strcat(name,token);
		read();
		strcat(name,expression(func));
		if(tmp==RBRACK){
			strcat(name,token);
			if(read()==ASSIGN){
				read();
				strcpy(src,expression(func));
			}
			if(tmp==RBRACK||tmp==SEMICN)
			    ;
			else {
				error(BRACK_DISMATCH);
							}
			//read();
		} 
		else if(tmp==ASSIGN){
			error(BRACK_DISMATCH);
			read();
			strcpy(src,expression(func));
			if(tmp==RBRACK||tmp==SEMICN)
			    ;
			else {
				error(BRACK_DISMATCH);
							}
			return;
		}
	}
	four(MOV,name,src,"");
	if(tmp==SEMICN)
				;
	else{
		error(MISSING_SEMICN);
	}
}
//��������䣾  ::=  if ��(������������)������䣾
void if_state(char func[])
{ 

	char a[STRINGLENGTH]={0};
	itoa(ifnum++,a,10);
	if(read()==LPARENT){
		condition(func,labelname("ifEnd",a),labelname("ifStart",a));
		four(LABEL,labelname("ifStart",a),"","");
		read();
		statement(func);
		four(LABEL,labelname("ifEnd",a),"","");
	}
	else{
			error(PARENT_DISMATCH);
			return;
		}
}

//������    ::=  �����ʽ������ϵ������������ʽ���������ʽ��
void condition(char func[],char end[],char start[])
{
	int op=0;
	char zero[]="0";
	char src1[STRINGLENGTH],src2[STRINGLENGTH];
	read();
	if(tmp==RPARENT){
		error(EMPTY_CONDITION);
		return;
	}
	strcpy(src1,expression(func));
	if(tmp>=EQL&&tmp<=NEQ){
		op=tmp;
		read();
		strcpy(src2,expression(func));
		switch(op){
			case (EQL):
				four(PEQL,"$10",src1,src2);
				break;
			case (NEQ):
				four(PNEQ,"$10",src1,src2);
				break;
			case (LSS):
				four(PLSS,"$10",src1,src2);
				break;
			case (LEQ):
				four(PLEQ,"$10",src1,src2);
				break;
			case (GRE):
				four(PGRE,"$10",src1,src2);
				break;
			case (GEQ):
				four(PGEQ,"$10",src1,src2);
				break;
		}
	}
	else if (tmp==RPARENT)
	{
		four(PNEQ,"$10",src1,"0");
	}
	if(tmp==RPARENT){
		four(BEQ,end,"$10",zero);
		four(BNE,start,"$10",zero);
		return;
	}
}
//��ѭ����䣾   ::=  do����䣾while ��(������������)��
void do_while(char func[])
{
	char a[STRINGLENGTH]={0};//,b[STRINGLENGTH],c[STRINGLENGTH];
	itoa(donum++,a,10);
	
	if(tmp==DOTK){
		four(LABEL,labelname("doStart",a),"","");
		read();
		statement(func);
		if(read()==WHILETK){
			if(read()==LPARENT){
				condition(func,labelname("doEnd",a),labelname("doStart",a));
			}
		}
	}
	four(LABEL,labelname("doEnd",a),"","");
}
//��������   ::=  ��������|���ַ��� 
char* constant()
{
	char con[STRINGLENGTH];
	read();
	strcpy(con,token);
	if(tmp==PLUS||tmp==MINUS){
		if(read()==INTCON){
			strcat(con,token);
		}
	}
	if(tmp==INTCON)	;
	else if(tmp==CHARCON){
		;
	}
	return con;
}
//�������䣾  ::=  switch ��(�������ʽ����)�� ��{��������� ��}��
void switch_state(char func[])
{
	char exp[STRINGLENGTH]={0};
	char a[STRINGLENGTH]={0},b[STRINGLENGTH];
	itoa(switchnum++,a,10);
	if(read()==LPARENT){
		read();
		strcpy(exp,expression(func));
		if(tmp==RPARENT&&read()==LBRACE){
			case_list(func,exp,labelname("switchEnd",a));
		}
	}
	else{
			error(PARENT_DISMATCH);
			return;
		}
	itoa(casenum++,b,10);
	four(LABEL,labelname("case",b),"","");
	four(LABEL,labelname("switchEnd",a),"","");
}
//�������   ::=  ���������䣾{���������䣾}
void case_list(char func[],char exp[],char end[])
{	
	if(read()==CASETK){
		do{
			case_state(func,exp,end);
		}while(read()==CASETK);
		if(tmp==RBRACE)
			;
	}
}
//���������䣾  ::=  case��������������䣾
void case_state(char func[],char exp[],char end[])
{
	char con[STRINGLENGTH];
	char a[STRINGLENGTH]={0},b[STRINGLENGTH];
	itoa(casenum++,a,10);
	four(LABEL,labelname("case",a),"","");
	strcpy(con,constant());
	itoa(casenum,b,10);
	four(BNE,labelname("case",b),con,exp);
	if(read()==COLON){
		read();
		statement(func);
		four(J,end,"","");
	}
}
//���з���ֵ����������䣾 ::= ����ʶ������(����ֵ��������)��
void use_valfunc(char func[],char name[])
{
	if(tmp==LPARENT){
			val_para(func,name);
		}
	if(read()==SEMICN||tmp==MULT||tmp==DIV||tmp==PLUS||tmp==MINUS||tmp==RPARENT||tmp==RBRACK)
				strcat(name,token);

}
//���޷���ֵ����������䣾 ::= ����ʶ������(����ֵ��������)��
void use_voidfunc(char func[])
{
	char debug[STRINGLENGTH]={0};
	char name[STRINGLENGTH]={0};
	//output(VOIDFUNC);
	strcpy(debug,func);

	if(tmp==IDEN){
		if(read()==LPARENT){
			val_para(func,name);
		}
	}
	if(read()==SEMICN)
				;
	else{
		error(MISSING_SEMICN);
	}
}
//��ֵ������   ::= �����ʽ��{,�����ʽ��}�����գ�
void val_para(char func[],char returnexp[])//�ڶ����ǵ��õĸú��������������ر��ʽ��
{
	
	char returnreg[STRINGLENGTH]={0};
	char funcname[STRINGLENGTH]={0};
	int paranum=0;
	strcpy(funcname,returnexp);
	if (funcname[strlen(funcname)-1]=='(')
	{
		funcname[strlen(funcname)-1]=0;
	}
	read();
	fprintf(fa,"addi $fp,$fp,8\n");
	if(tmp==RPARENT){
		strcat(returnexp,token);
		//four(CALL,funcname,"","");
		return ;
	}
	else{
		strcpy(returnreg,expression(func));
		load_para(returnreg,paranum*4);
		strcat(returnexp,returnreg);
		paranum++;
	}
	while(tmp==COMMA){
		read();
		strcpy(returnreg,expression(func));
		load_para(returnreg,paranum*4);
		strcat(returnexp,returnreg);
		paranum++;
	}
	if(tmp==RPARENT){
		if(returnexp!=NULL)
			strcat(returnexp,token);
	}
	//four(CALL,funcname,"","");//�����Ǵ�����ʽ֮ǰ//////////////////////////////error��������
}
//������У�   ::=������䣾��
void statements(char func[]) 
{
	while(tmp!=RBRACE){
		statement(func);
		read();
	}
} 
//������䣾    ::=  scanf ��(������ʶ����{,����ʶ����}��)��
void scanf_state(char func[])
{
	//output(SCANF_STATE);
	char debug[STRINGLENGTH]={0};
	if(tmp==SCANFTK){
		if(read()==LPARENT){
			do{
				if(read()==IDEN){
					int i;
					if((i=seek(token,func))==NOTFOUND)	error(WITHOUT_DECLARATION);
					else{
						four(SCANF,token,itoa(table[i].kind,debug,10),"");
						read();
				}
				}
			}while(tmp==COMMA);
			if(tmp==RPARENT){
				;
			}
		}
		else{
			error(PARENT_DISMATCH);
			return;
		}
		if(read()==SEMICN)
				;
		else{
		error(MISSING_SEMICN);
	}
	}
}
//��д��䣾    ::=  printf��(�����ַ�����,�����ʽ����)��|printf ��(�����ַ�������)��
//					|printf ��(�������ʽ����)��
void printf_state(char func[])
{
	char string[STRINGLENGTH];
	//output(PRINTF_STATE);
	if(tmp==PRINTFTK){
		if(read()==LPARENT){
			read();
				if(tmp==STRCON){
					strcpy(string,token);
					read();
					if(tmp==RPARENT){
						four(PRINTF,string,"","");;
					}
					else if(tmp==COMMA){
						read();
						if(tmp==PLUS||tmp==MINUS||tmp==IDEN||tmp==INTCON||tmp==CHARCON||tmp==LPARENT){
							four(PRINTF,string,expression(func),"");
							if(tmp==RPARENT){
								;
							}
						}
					}
					}
				if(tmp==PLUS||tmp==MINUS||tmp==IDEN||tmp==INTCON||tmp==CHARCON||tmp==LPARENT){
					four(PRINTF,"",expression(func),"");
					if(tmp==RPARENT){
						;
					}
					else{
						error(PARENT_DISMATCH);
						return;
					}
				}
			if(read()==SEMICN)
				;
			else{
					error(MISSING_SEMICN);
				}
		}
		else{
			error(PARENT_DISMATCH);
			return;
		}
	}
}
//��������䣾   ::=  return[��(�������ʽ����)��]
void return_state(char func[])
{
	//output(RETURN_STATE);
	if(tmp==RETURNTK){
		read();
		if(tmp==LPARENT){
			read();
			if(tmp==PLUS||tmp==MINUS||tmp==IDEN||tmp==INTCON||tmp==CHARCON||tmp==LPARENT){
				four(RETURN,expression(func),"","");
				if(tmp==RPARENT){
					read();//
					if(tmp==SEMICN){
						;
					}
					else{
						error(MISSING_SEMICN);
					}
				}
			}
			if(tmp==SEMICN){
						;
					}
			else{
				error(MISSING_SEMICN);
			}
		}
		else if(tmp==SEMICN){
			four(RETURN,"","","");
		}
		else{
			error(MISSING_SEMICN);
		}
	}
}
char* labelname(char s[],char num[])
{
	char *c = (char *) malloc(strlen(s) + strlen(num) + 1);
	char *tempc = c; //���׵�ַ������
	strcpy(c,s);
	strcat(c,num);
	return tempc;

}

/*void //output(int a)
{
	printf("This is a");
	switch(a){
		case(CONST_STATE):
			printf(" constant description.\n");
			break;
		case(VAR_STATE):
			printf(" var description.\n");
			break;
		case(DEF_VALFUNC):
			printf(" return value function definition.\n");
			break;
		case(DEF_VOIDFUNC):
			printf(" void function definition.\n");
			break;
		case(MAIN_FUNC):
			printf(" main function statement.\n");
			break;
		case(IF_STATE):
			printf("n if statement.\n");
			break;
		case(DO_WHILE):
			printf(" do-while statement.\n");
			break;
		case(SWITCH_STATE):
			printf(" switch statement.\n");
			break;
		case(CASE_STATE):
			printf(" case statement.\n");
			break;
		case(VALFUNC):
			printf(" return value function use.\n");
			break;
		case(VOIDFUNC):
			printf(" void function use.\n");
			break;
		case(ASSIGNMENT):
			printf("n assignment.\n");
			break;
		case(PRINTF_STATE):
			printf(" printf statement.\n");
			break;
		case(SCANF_STATE):
			printf(" scanf statement.\n");
			break;
		case(RETURN_STATE):
			printf(" return statement.\n");
			break;
		case(EMPTY):
			printf("n empty statement.\n");
			break;
	}
}*/
