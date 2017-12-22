#ifndef  __GRAMMAR_H_
#define __GRAMMAR_H_
#define STRINGLENGTH 128
enum out{
	CONST_STATE,
	VAR_STATE,
	DEF_VALFUNC,
	DEF_VOIDFUNC,
	MAIN_FUNC,
	IF_STATE,
	DO_WHILE,
	SWITCH_STATE,
	CASE_STATE,
	VALFUNC,
	VOIDFUNC,
	ASSIGNMENT,
	PRINTF_STATE,
	SCANF_STATE,
	RETURN_STATE,
	EMPTY
	
};
void program();
void var_or_func();
void const_state(char area[]);
void def_const(char area[]);
int integer();
void var_state(char area[],int *offfun);
void def_var(char area[],int *offfun);
void def_func(char name[],int type) ;
void com_state(char area[],int *offfun);
int para_table(char func[],int *offfun);
void main_func();
char* expression(char func[]);
int term(char func[],char exp[][STRINGLENGTH],int *e);
int factor(char func[],char exp[][STRINGLENGTH],int *e);
void statement(char func[]);
void assignment(char func[],char name[]);
void if_state(char func[]);
void condition(char func[],char end[],char start[]);
void do_while(char func[]);
char* constant();
void switch_state(char func[]);
void case_list(char func[],char exp[],char end[]);
void case_state(char func[],char exp[],char end[]);
void use_valfunc(char name[]);
void use_voidfunc(char name[]);
void val_para(char func[],char name[]);
void statements(char func[]);
void scanf_state(char func[]);
void printf_state(char func[]);
void return_state(char func[]);
char* labelname(char s[],char num[]);
//void output(int a);
#endif
