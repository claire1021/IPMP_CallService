// TestClient.cpp : 定义控制台应用程序的入口点。
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

int getConn(){
	
			WORD wVersionRequested;
			WSADATA wsaData;
			int err;
			int retVal;

			wVersionRequested=MAKEWORD(1,1);
				  
			err=WSAStartup(wVersionRequested,&wsaData);
			if (err !=0 )  //返回0表示成功
				return 0;

			if(LOBYTE(wsaData.wVersion)!=1||HIBYTE(wsaData.wVersion)!=1)
			{
				WSACleanup();
				return 0;
			}
				
			//创建套接字
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

			char recvBuffer[BUF_SIZE];

			memset(recvBuffer,0,sizeof(recvBuffer));
			recv(sockClient,recvBuffer,BUF_SIZE,0);
			printf("%s\n",recvBuffer);
			send(sockClient,recvBuffer,sizeof(recvBuffer),0);
	//		closesocket(sockClient);
		//	WSACleanup();
			return sockClient;
}

void closeSocket(int fd){
	closesocket(sockClient);
	WSACleanup();
}

int _tmain(int argc, _TCHAR* argv[])
{
	int fd;
	fd = getConn();
	
	Json::Value root;
	root["m_object"] = Json::Value("CallService");
	root["m_op"] = Json::Value("selectMr");

	Json::FastWriter fw;
	char sendBuf[1024];
	memset(sendBuf,0,sizeof(sendBuf));                       
	strcpy(sendBuf,(fw.write(root)).c_str());
	//发送数据
	sendBuf[strlen(sendBuf)] = '\0';
	send(fd,sendBuf,strlen(sendBuf)+1,0);*/
	
	
	//接收数据
	char recvBuf[1024];
	int end = recv(sockClient,recvBuf,1024,0);
	recvBuf[end]='\0';//读取结束
	Json::Reader reader;
	reader.parse(recvBuf,root);
	char * stemp = U2G(recvBuf);
	string ltate = root["code"].asString(); 
	const char * logstate = ltate.c_str();
	
	closesocket(fd);
	printf("i'm here\n");
	system("pause");
	return 0;
}


