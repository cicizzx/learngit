#ifndef TABLE_H
#define TABLE_H
#define COMPARAMETERSNUMBER 10
#define TMAX 128
#define STRINGLENGTH 128
enum tablename{
	NOTFOUND=-1,
	CONSTANT,
	PARAMETER,
	SIMPLE_VARIABLE,
	ARRAY_VARIABLE,
	FUNCTION
};
struct tb
{
	
	int type;//FUNCTION,VARIABLE,CONSTANT
	int kind;//char\int
	char name[STRINGLENGTH];
	char area[STRINGLENGTH];//��Ч��������(VARIABLE,CONSTANT)
	int value;//������char\int����ֵ���򵥱����溯���ڵ�ַƫ��
	int length;//����,�������ȣ���������

};
struct funtable
{
	int type;//char int void
	char name[STRINGLENGTH];
	int paranum;
	int para1;
	
};

void entertable(int type,char id[],int value,int kind,char func[]);
void entertablearray(int type,char id[],int value,int kind,char func[],int length);
void entertablefun(int type,char id[],int paranum,int para1ptr);//name,function var...,area
int check(char id[], char funid[]);
int seek(char id[], char funid[]);
int fp_offset(char func[]);
#endif
