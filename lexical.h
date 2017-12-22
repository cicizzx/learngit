#ifndef  __LEXICAL_H_
#define __LEXICAL_H_
enum symbol{
	IFTK,	//if
	DOTK,//do
	WHILETK,//while
	SWITCHTK,//switch
	CASETK,//case
	RETURNTK,//return
	SCANFTK,//scanf
	PRINTFTK,//printf
	MAINTK,//main
	CONSTTK,//const
	INTTK,//int
	CHARTK,//char
	VOIDTK,//void
	IDEN,//�Ǳ����ִ�
	INTCON,//����
	CHARCON,//char
	STRCON,//string
	SPACE ,//�ո�س��Ʊ� 
	PLUS ,//�ӷ������ 
	MINUS ,//���������
	MULT ,//�Ǻ�
	DIV ,//����
	LPARENT ,//������
	RPARENT ,//������
	LBRACE ,//������
	RBRACE ,//�һ�����
	LBRACK,//������
	RBRACK,//�ҷ����� 
	COMMA ,//����
	SEMICN ,//�ֺ�
	COLON ,//ð��
	ASSIGN ,//��ֵ����
	EQL ,//���ж�
	GRE ,//���ں� 
	GEQ,//���ڵ���
	LSS,//С�ں�
	LEQ,//С�ڵ���
	NEQ,//�����ں�
	SINQUO,//������
	DBLQUO,//˫����	
	DIGIT,//����
	LETTER,//��ĸ
	EOFFILE
};

int line,tokennum,tmp;
char token[],ch;
int getword();
char getsym();
int stringtype(char c);
int chartype(char ch);
int symtype();
int keyword();
void clear();
int read();
#endif
