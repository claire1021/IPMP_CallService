// TestClient.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#pragma comment(lib, "Ws2_32.lib")
#include <iostream>
#include<string>
#include <WinSock2.h>
#include <stdio.h>
#include <WS2tcpip.h>
#include <process.h>
#include"define.h"
#include"../json/json.h"
using namespace std;

char recvBuf[4096];
char sendBuf[4096];

int getConn(){
	
			WORD wVersionRequested;
			WSADATA wsaData;
			int err;
			int retVal;

			wVersionRequested=MAKEWORD(1,1);
				  
			err=WSAStartup(wVersionRequested,&wsaData);
			if (err !=0 )  //����0��ʾ�ɹ�
				return 0;

			if(LOBYTE(wsaData.wVersion)!=1||HIBYTE(wsaData.wVersion)!=1)
			{
				WSACleanup();
				return 0;
			}
				
			//�����׽���
			SOCKET sockClient= socket(AF_INET,SOCK_STREAM,0);
			if(sockClient>0){
				printf("socket create succedd! socket fd=%d\n",sockClient);
			}

			SOCKADDR_IN  addrServ;
			addrServ.sin_addr.S_un.S_addr = inet_addr("202.117.10.83");
			addrServ.sin_family = AF_INET;
			addrServ.sin_port = htons(12306);
				
			retVal = connect(sockClient,(SOCKADDR*)&addrServ,sizeof(SOCKADDR));
			if(retVal>0){
				printf("socket connect succedd! socket retVal=%d\n",retVal);
			}
			return sockClient;
}

void closeSocket(int fd){
	closesocket(fd);
	WSACleanup();
}

int _tmain(int argc, _TCHAR* argv[])
{
	int fd;
	fd = getConn();
	printf("fd=%d\n",fd);

	//��װjson
	Json::Value root;
	Json::Reader reader;
	Json::FastWriter fw;

	root["m_object"] = Json::Value("CallService");
	root["m_op"] = Json::Value("selectMR");
				
				
	memset(sendBuf,0,sizeof(sendBuf));                       
	memset(recvBuf,0,sizeof(recvBuf));                       
	strcpy(sendBuf,(fw.write(root)).c_str());
	//��������
	int ret = send(fd,sendBuf,sizeof(sendBuf),0);
	if(ret==-1){
		printf("send err:%d\n",ret);
	}
	recv(fd,recvBuf,sizeof(recvBuf),0);

	printf("%s\n",recvBuf);

	//������������
	reader.parse(recvBuf,root);
		
	Json::Value roomArray = root["MRS"];                //��Ա�������ɹ�ѡ��Ļ�������Ŀ
	int size = roomArray.size();
printf("size of roomNum = %d\n",size);

		for(int i=0;i<size;i++){
			int name = roomArray[i]["meetroom_name"].asInt();
			printf("%d\n",name);
		}

		
		//---------------------------------------------------------------------------------

	closesocket(fd);
	printf("i'm here\n");
	system("pause");
	return 0;
}


