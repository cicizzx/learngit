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
	IDEN,//非保留字串
	INTCON,//整型
	CHARCON,//char
	STRCON,//string
	SPACE ,//空格回车制表 
	PLUS ,//加法运算符 
	MINUS ,//减法运算符
	MULT ,//星号
	DIV ,//除号
	LPARENT ,//左括号
	RPARENT ,//右括号
	LBRACE ,//左花括号
	RBRACE ,//右花括号
	LBRACK,//左方括号
	RBRACK,//右方括号 
	COMMA ,//逗号
	SEMICN ,//分号
	COLON ,//冒号
	ASSIGN ,//赋值运算
	EQL ,//等判断
	GRE ,//大于号 
	GEQ,//大于等于
	LSS,//小于号
	LEQ,//小于等于
	NEQ,//不等于号
	SINQUO,//单引号
	DBLQUO,//双引号	
	DIGIT,//数字
	LETTER,//字母
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
