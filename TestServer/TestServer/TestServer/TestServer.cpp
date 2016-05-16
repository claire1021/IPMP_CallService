// TestServer.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#pragma comment(lib, "Ws2_32.lib")

#include <Winsock2.h>
#include <stdio.h>
#include <stdlib.h>
#define PORT  8111

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
		char sendBuf[100] ;
		memset(sendBuf, 0, 100);
		int r = 0;
		do {///不断得到客户端发来的请求
  
  
		   sprintf( sendBuf , "Weclome %s to here:" , inet_ntoa(addrClient.sin_addr) ) ;
		   printf("sending.....\n");
		   r = send( sockConn , sendBuf , strlen(sendBuf)+1 , 0 ) ;
		   printf("send end.....\n");
		   //接收数据
		   char recvBuf[100] ;
		   printf("receiving.....\n");
		   r = recv( sockConn , recvBuf , 100 , 0 ) ;
		   printf("receive end.....\n");
		   printf("data from client: %s\n" , recvBuf ) ;
		}while ( r != SOCKET_ERROR );
		printf("client connect error! app will exit\n");
		system("PAUSE");
		//关闭socket
		closesocket( sockConn ) ;
		//清理socket编程环境
		WSACleanup() ;
}
