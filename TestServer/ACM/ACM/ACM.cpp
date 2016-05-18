// ACM.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include<stdio.h>
#include<stdlib.h>
#include<string>
#include<iostream>
using namespace std;
/*
Input各种输入方式
*/

//Hdu1048

int _tmain(int argc, _TCHAR* argv[])
{	
	char a[1000],b[1000];
	int i;
	while(gets(a)){
		if(strcmp(a,"START")==0){
			memset(b,0,sizeof(b));
		}
		else if(strcmp(a,"END")==0){
			cout<<b;
		}else if(strcmp(a,"ENDOFINPUT")==0){
			break;
		}
		for(i=0;a[i]!='\0';i++){
			if(a[i]>='A'&&a[i]<='Z'){
				if(a[i]+21>90)
					b[i] = a[i]-5;
				else
					b[i] =a[i]+21;
			}
			else
				b[i] = a[i];
		}
	}
	system("pause");
	return 0;
}

