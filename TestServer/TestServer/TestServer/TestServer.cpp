// TestServer.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#pragma comment(lib, "Ws2_32.lib")

#include <Winsock2.h>
#include <stdio.h>
#include <stdlib.h>
#define PORT  12306

void main()
{
		//��ʼ��socket��̻���
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
		//����Tcp������socket
		SOCKET sockSrv = socket( AF_INET , SOCK_STREAM , 0 );
		//��������ַ
		SOCKADDR_IN addrSrv ;
		addrSrv.sin_addr.S_un.S_addr = inet_addr("202.117.10.83");
		addrSrv.sin_family = AF_INET ;
		addrSrv.sin_port = htons(PORT) ;
		//��socket���ַ����һ��
		bind( sockSrv ,(SOCKADDR*)&addrSrv , sizeof(SOCKADDR) ) ;
		//��ʼ�����ͻ�������,���������Ϊ5
		listen( sockSrv , 5 ) ;
		//���ڴ�ſͻ��˵�ַ
		SOCKADDR_IN addrClient ;
		int len = sizeof( SOCKADDR_IN ) ;
		//���Ͻ��տͻ��˷��͵�����
		SOCKET sockConn;
		//���յ��Ŀͻ�������socket
		printf("accepting.....\n");
		sockConn = accept( sockSrv , (SOCKADDR *)&addrClient , &len ) ;
		printf("accept end.....\n");
		//��������
		char sendBuf[100] ;
		memset(sendBuf, 0, 100);
		int r = 0;
		do {///���ϵõ��ͻ��˷���������
  
  
		   sprintf( sendBuf , "Weclome %s to here:" , inet_ntoa(addrClient.sin_addr) ) ;
		   printf("sending.....\n");
		   r = send( sockConn , sendBuf , strlen(sendBuf)+1 , 0 ) ;
		   printf("send end.....\n");
		   //��������
		   char recvBuf[100] ;
		   printf("receiving.....\n");
		   r = recv( sockConn , recvBuf , 100 , 0 ) ;
		   printf("receive end.....\n");
		   printf("data from client: %s\n" , recvBuf ) ;
		}while ( r != SOCKET_ERROR );
		printf("client connect error! app will exit\n");
		system("PAUSE");
		//�ر�socket
		closesocket( sockConn ) ;
		//����socket��̻���
		WSACleanup() ;
}
