#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include<string>
#include<fstream>
#include<sstream>
#include<iostream>
#include"../json/json.h"
using namespace std;

int ParseJsonFromFile(const char *filename){
	Json::Reader reader;
	Json::Value root;

	return 0;
}
int main1()
{
	char str[1024];
	memset(str,0,sizeof(str));
	Json::Value rootNode;  
	Json::Value listNode;  
	Json::FastWriter fw;
	Json::Reader reader;

	for(int i=420;i<420+3;i++)  
	{    
		Json::Value item;  

		item["meetroom_name"]=Json::Value(i);
		listNode.append(item);  
	}  
	rootNode["MRS"]=listNode;  

	strcpy_s(str,fw.write(rootNode).c_str());
	cout<<str<<endl;
   //½âÎö
	cout<<"===================================="<<endl;
	Json::Value recvroot;
	reader.parse(str,recvroot);
	int num = recvroot["MRS"].size();

	system("pause");
	return 0;
}
