#include<stdio.h>
#include"grammer.h"
#include"lexical.h"
#include"error.h"
#include "pcode.h"
FILE *fp=NULL;
FILE *fa=NULL;
int main()
{
	char path[STRINGLENGTH]={0},out[STRINGLENGTH]={0};

	/*printf("please input path£º\n");
	gets(path);
	
	while((fp=fopen(path,"r"))==NULL)
	{
		printf("Can not find the file, please re-input£º\n");
		gets(path);
	}
	fpre_printf=fopen(path,"r");

	printf("please output path£º\n");
	gets(out);

	while((fa=fopen(out,"w"))==NULL)
	{
	printf("Can not find the file, please re-input£º\n");
	gets(out);
	}*/
	
	fp=fopen("a.txt","r");
	fa=fopen("res.asm","w");

	pre_printf();

	program();
	
	printf("end!\n");
	fclose(fp);
	return 0;
}


