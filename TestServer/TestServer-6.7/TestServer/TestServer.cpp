// TestServer.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#pragma comment(lib, "Ws2_32.lib")

#include <Winsock2.h>
#include <stdio.h>
#include <stdlib.h>
#include<string>
#include<sstream>
#include<map>
#include"../json/json.h"
#define PORT  12306
using namespace std;


char recvBuf[4096] ;
char sendBuf[4096] ;
map<int,string> meetrooms;
int size;


Json::Reader reader;
Json::FastWriter fw;
Json::Value listNode,root,list,item,rootNode;








void clearBuf(){
	memset(recvBuf,0,sizeof(recvBuf));
	memset(recvBuf,0,sizeof(sendBuf));
}

void jsonsend(){
	char str[1024];
	memset(str,0,sizeof(str));
	

	for(int i=420;i<420+3;i++)  
	{    
		stringstream ss;
		string iStr;
		ss<<i;
		ss>>iStr;
		item["meetroom_name"]=Json::Value(iStr);  
		listNode.append(item);  
		//	m_pRs.MoveNext();     
	}  
	rootNode["MRS"]=listNode;  

	strcpy(str,fw.write(rootNode).c_str());
   strcpy(sendBuf,str);
} 



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
		int ret = 0;
		clearBuf();
printf("------------------------------��������һ�ν�������----------------------\n");
		//��������
		printf("receiving.....\n");
		ret = recv( sockConn , recvBuf ,sizeof(recvBuf), 0 ) ;
		if(ret==-1){
			printf("recv err:%d\n",ret);
		}
		//����json
		
		if(reader.parse(recvBuf,root))
		{
			string m_object = root["m_object"].asString();
			string m_op = root["m_op"].asString();
			cout<<"m_object="<<m_object<<endl;
			cout<<"m_op="<<m_op<<endl;
		}
		printf("data from client: %s\n" , recvBuf ) ;
printf("------------------------------��������һ�� recv ���ݽ���----------------------\n\n\n");

printf("------------------------------��������1�� send  ����----------------------\n");
		//�������� ���л����ҵ�
		jsonsend();
		ret = send( sockConn , sendBuf, sizeof(sendBuf) , 0 ) ;
		if(ret==-1){
			printf("recv err:%d\n",ret);
		}
		printf("������%d���ֽ�\n",ret);
		printf("send end.....\n");
printf("------------------------------��������1�� send ���ݽ���----------------------\n\n\n");



printf("------------------------------��������2�� recv  ����----------------------\n");
		//���������û���ѡ�Ļ�����
		clearBuf();
		printf("receiving2.....\n");
		ret = recv( sockConn , recvBuf ,sizeof(recvBuf), 0 ) ;
		if(ret==-1){
			printf("recv err:%d\n",ret);
		}
	

		int size;
		if(reader.parse(recvBuf,root))
		{
			string m_object = root["m_object"].asString();            //m_object:   CallServiceUtils
			string m_op = root["m_op"].asString();                       //m_op    :    selectMR
			size = root["MRS"].size();                                    //MRS       :   "meetroom_name":"420"
			for(int i=0;i<size;i++){                                               
				meetrooms.insert(pair<int,string>(i,root["MRS"][i]["meetroom"].asString()));
			}

cout<<"m_object="<<m_object<<endl;
 cout<<"m_op="<<m_op<<endl;
cout<<"size="<<size<<endl;
			map<int,string>::iterator it;
			for(it=meetrooms.begin();it!=meetrooms.end();it++){
				cout<<it->first<<","<<it->second<<endl;
			}
		}
		printf("data from client: %s\n" , recvBuf ) ;

printf("------------------------------��������2�� recv ���ݽ���----------------------\n\n\n");

printf("------------------------------��������3�� send ����----------------------\n");

	map<int,string>::iterator it;
	Json::Value outerList;
	Json::Value innerList;
	Json::Value item1,item2,item3,item4,item5;
	int i=20;
	for(it=meetrooms.begin();it!=meetrooms.end();it++){
		item1["meetroom_name"] =Json::Value(it->second);
		item2["meet_name"]=Json::Value("�ܻ�");
		item3["meet_time"]=Json::Value("2016-7-6");
		item4["meet_place"]=Json::Value("��һ¥");
		item5["person_num"]=Json::Value(i);
		i+=10;
		innerList.append(item1);
		innerList.append(item2);
		innerList.append(item3);
		innerList.append(item4);
		innerList.append(item5);
	}	

	outerList["MRSInfo"]=innerList;
	outerList["meetroom_Num"] = size;
	memset(sendBuf,0,sizeof(sendBuf));
	strcpy(sendBuf,(fw.write(outerList)).c_str());
	send(sockConn,sendBuf,sizeof(sendBuf),0);
	printf("%s\n",sendBuf);
printf("------------------------------��������3�� send ����----------------------\n");		
	closesocket( sockConn ) ;
	//����socket��̻���
	WSACleanup() ;
	system("PAUSE");

}
