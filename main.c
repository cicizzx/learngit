#include<stdio.h>
#include"grammer.h"
#include"lexical.h"
#include"error.h"
#include "pcode.h"
FILE *fp=NULL;
FILE *fa=NULL;
FILE *fe=NULL;
FILE *ft=NULL;
int main()
{
	char path[STRINGLENGTH]={0},out[STRINGLENGTH]={0};
	fe=fopen("error.txt","w");	
	ft=fopen("table.txt","w");
	printf("please input path��\n");
	gets(path);
	
	while((fp=fopen(path,"r"))==NULL)
	{
		printf("Can not find the file, please re-input��\n");
		gets(path);
	}
	
	/*printf("please output path��\n");
	gets(out);

	while((fa=fopen(out,"w"))==NULL)
	{
	printf("Can not find the file, please re-input��\n");
	gets(out);
	}*/
	
	//fp=fopen("a.txt","r");
	fa=fopen("res.asm","w");

	pre_printf();

	program();
	
	printf("end!\n");
	fclose(fp);
	return 0;
}


