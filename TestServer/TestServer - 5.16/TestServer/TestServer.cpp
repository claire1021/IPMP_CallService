// TestServer.cpp : �������̨Ӧ�ó������ڵ㡣
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

		/*
		do {///���ϵõ��ͻ��˷���������
  
 
		   //��������
		   char recvBuf[1024] ;
		   memset(recvBuf,0,sizeof(recvBuf));
		   printf("receiving.....\n");
		    r = recv( sockConn , recvBuf ,sizeof(recvBuf), 0 ) ;
			//����json
			Json::Reader reader;
			Json::Value root;
			if(reader.parse(recvBuf,root));
			{
				string m_object = root["m_object"].asString();
				string m_op = root["m_op"].asString();
				cout<<"m_object="<<m_object<<endl;
				cout<<"m_op="<<m_op<<endl;
			}
			printf("data from client: %s\n" , recvBuf ) ;
		    printf("receive end.....\n");
		    
	      //��������
		  printf("sending.....\n");
	      r = send( sockConn , "200", 5 , 0 ) ;
		printf("send end.....\n");

		   
		}while ( r != SOCKET_ERROR );
		*/


		//��������
		char recvBuf[1024] ;
		memset(recvBuf,0,sizeof(recvBuf));
		printf("receiving.....\n");
		r = recv( sockConn , recvBuf ,sizeof(recvBuf), 0 ) ;
		//����json
		Json::Reader reader;
		Json::Value root;
		Json::FastWriter fw;
		if(reader.parse(recvBuf,root));
		{
			string m_object = root["m_object"].asString();
			string m_op = root["m_op"].asString();
			cout<<"m_object="<<m_object<<endl;
			cout<<"m_op="<<m_op<<endl;
		}
		printf("data from client: %s\n" , recvBuf ) ;
		printf("receive end.....\n");

		//��������
	
	
		printf("sending.....\n");
		r = send( sockConn , "200", 5 , 0 ) ;
		printf("send end.....\n");
		system("PAUSE");
		//�ر�socket
		closesocket( sockConn ) ;
		//����socket��̻���
		WSACleanup() ;
}
