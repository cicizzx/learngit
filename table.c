#include<stdio.h>
#include<string.h>
#include"table.h"
#include"error.h"
#include "lexical.h"
int tptr=0;
int ftptr=0;
struct funtable ftable[TMAX];
struct tb table[TMAX];

void entertable(int type,char id[],int value,int kind,char func[])
{
	if(seek(id,func)!=NOTFOUND)
		error(RE_DECLARATION);	
	table[tptr].type=type;
	table[tptr].kind=kind;
	table[tptr].length=0;
	strcpy(table[tptr].name,id);
	strcpy(table[tptr].area,func);
	if(type==CONSTANT)
	{
		if(kind==INTTK||kind==CHARTK){
			table[tptr].value=value;
		}
	}
	else if(type==SIMPLE_VARIABLE)
		table[tptr].value=value;///address
	else if(type==PARAMETER)
		table[tptr].value=value;
	//printf("table[%d]:name:%s,area:%s\n",tptr,table[tptr].name,table[tptr].area);
	tptr++;
}
void entertablearray(int type,char id[],int value,int kind,char func[],int length)
{
	if(seek(id,func)!=NOTFOUND)
		error(RE_DECLARATION);
	
	table[tptr].type=type;
	table[tptr].kind=kind;
	table[tptr].value=value;//address
	table[tptr].length=length;
	strcpy(table[tptr].area,func);
	strcpy(table[tptr].name,id);
	tptr++;

}
void entertablefun(int type,char id[],int paranum,int para1ptr)
{
	if(seek(id,"static")!=NOTFOUND)
		error(RE_DECLARATION);
	
	ftable[ftptr].type=type;
	ftable[ftptr].paranum=paranum;
	//ftable[ftptr].enterpnum=enter;
	ftable[ftptr].para1=para1ptr;
	strcpy(ftable[ftptr].name,id);	
	ftptr++;
};
int check(char id[], char funid[])
{
	int i;
	for(i=0;i<ftptr;i++)
	{
		if(stricmp(ftable[i].name,id)==0)
			return i;
	}
	for(i=0;i<tptr;i++)
	{
		if((stricmp(table[i].name,id)==0)&&((stricmp(table[i].area,funid)==0)))
			return i;
	}
	return NOTFOUND;
}
int seek(char id[], char funid[])
{
	int i;
	/*for(i=0;i<ftptr;i++)
	{
		if(stricmp(ftable[i].name,id)==0)
			return i;
	}*/
	for(i=0;i<tptr;i++)
	{
		if((stricmp(table[i].name,id)==0)&&((stricmp(table[i].area,funid)==0)))
			return i;
	}
	for(i=0;i<tptr;i++)
	{
		if((stricmp(table[i].name,id)==0)&&((stricmp(table[i].area,"static")==0)))
			return i;
	}
	return NOTFOUND;
}

int fp_offset(char func[])
{
	int i,j=8;
	for(i=0;i<tptr;i++){
		if(strcmp(table[i].area,func)==0)
			j+=4;
	}
	return j;
}