// TestServer.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#pragma comment(lib, "Ws2_32.lib")

#include <Winsock2.h>
#include <stdio.h>
#include <stdlib.h>
#include<string>
#include"../json/json.h"
#define PORT  12306
using namespace std;


char recvBuf[4096] ;
char sendBuf[4096] ;

void jsonsend(){
	char str[1024];
	memset(str,0,sizeof(str));
	Json::Value rootNode;  
	Json::Value listNode;  
	Json::FastWriter fw;

	for(int i=420;i<420+3;i++)  
	{    
		Json::Value item;  

		item["meetroom_name"]=Json::Value(i);  
		listNode.append(item);  
		//	m_pRs.MoveNext();     
	}  
	rootNode["MRS"]=listNode;  

	strcpy(str,fw.write(rootNode).c_str());
   strcpy(sendBuf,str);
} 



void main()
{
		//初始化socket编程环境
		WORD wVersionRequested;
		WSADATA wsaData;
		int err;
		wVersionRequested = MAKEWORD( 1, 1 );
		err = WSAStartup( wVersionRequested, &wsaData );
		if ( err != 0 ) 
		{
		   return;
		}
		if ( LOBYTE( wsaData.wVersion ) != 1 ||
		   HIBYTE( wsaData.wVersion ) != 1 ) 
		{
		   WSACleanup( );
		   return; 
		}
		//创建Tcp服务器socket
		SOCKET sockSrv = socket( AF_INET , SOCK_STREAM , 0 );
		//服务器地址
		SOCKADDR_IN addrSrv ;
		addrSrv.sin_addr.S_un.S_addr = inet_addr("202.117.10.83");
		addrSrv.sin_family = AF_INET ;
		addrSrv.sin_port = htons(PORT) ;
		//将socket与地址绑定在一起
		bind( sockSrv ,(SOCKADDR*)&addrSrv , sizeof(SOCKADDR) ) ;
		//开始监听客户端请求,最大连接数为5
		listen( sockSrv , 5 ) ;
		//用于存放客户端地址
		SOCKADDR_IN addrClient ;
		int len = sizeof( SOCKADDR_IN ) ;
		//不断接收客户端发送的请求
		SOCKET sockConn;
		//接收到的客户端请求socket
		printf("accepting.....\n");
		sockConn = accept( sockSrv , (SOCKADDR *)&addrClient , &len ) ;
		printf("accept end.....\n");
		//发送数据
		memset(sendBuf, 0, 100);

		int r = 0;
		//接收数据
		memset(recvBuf,0,sizeof(recvBuf));
		memset(recvBuf,0,sizeof(sendBuf));
		printf("receiving.....\n");
		r = recv( sockConn , recvBuf ,sizeof(recvBuf), 0 ) ;
		//解析json
		Json::Reader reader;
		Json::Value root;
		Json::FastWriter fw;
		if(reader.parse(recvBuf,root))
		{
			string m_object = root["m_object"].asString();
			string m_op = root["m_op"].asString();
			cout<<"m_object="<<m_object<<endl;
			cout<<"m_op="<<m_op<<endl;
		}
		printf("data from client: %s\n" , recvBuf ) ;
		printf("receive end.....\n");

		//发送数据

		printf("sending.....\n");
		cout<<"sendBuf="<<sendBuf<<endl;
		jsonsend();
		r = send( sockConn , sendBuf, sizeof(sendBuf) , 0 ) ;
		if(r)
			printf("发送了%d个字节\n",r);
		printf("send end.....\n");
		system("PAUSE");
		//关闭socket
		closesocket( sockConn ) ;
		//清理socket编程环境
		WSACleanup() ;
}
