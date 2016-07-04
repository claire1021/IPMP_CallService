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


char recvBuf[1024*1000] ;
char sendBuf[8192] ;
//char sendBuf[1024*1000] ;
map<int,string> meetrooms;
int size;


Json::Reader reader;
Json::FastWriter fw;
Json::Value listNode,root,list,item,rootNode;


void clearBuf(){
	memset(recvBuf,0,sizeof(recvBuf));
	memset(sendBuf,0,sizeof(sendBuf));	
}

void jsonsend(){
	char str[1024*100];
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

	//strcpy(str,fw.write(rootNode).c_str());
//   strcpy(sendBuf,str);

	strcpy(sendBuf,fw.write(rootNode).c_str());
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
		printf("accepting.....\n");
		
		//���յ��Ŀͻ�������socket
		SOCKET sockConn= accept( sockSrv , (SOCKADDR *)&addrClient , &len ) ;
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
		int tmp = WSAGetLastError();
		printf("tmp  err:%d\n",tmp);
		if(ret==-1){
			printf("recv err:%d\n",ret);
		}

		printf("������%d���ֽ�\n",ret);
		printf("send end.....\n");
printf("------------------------------��������1�� send ���ݽ���----------------------\n\n\n");
		clearBuf();


printf("------------------------------��������2�� recv  ����----------------------\n");
		//���������û���ѡ�Ļ�����
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
	clearBuf();

	//	memset(sendBuf,0,sizeof(sendBuf));
	//	memset(recvBuf,0,sizeof(recvBuf));

	Json::Value rooter;
	Json::Value root1,root2,item1,item2;                           //��
	Json::Value outerList;                    //��������飬��һ��Ԫ��
	Json::Value UserList,SeatList;         //ÿ������Ԫ���ڵ�User��Seat
	Json::Value seat1,seat2,seat3,seat4,seat5;
	Json::StyledWriter sw;
   
	root1["addr"] = Json::Value("һ¥");
	root1["end_time"] = Json::Value("2016-03-04 12:00:00");
	root1["start_time"] = Json::Value("2016-03-04 10:00:00");
	root1["meet_title"] = Json::Value("tow");
	root1["mr_name"] = Json::Value("420");
	
	item1["s_num"] = Json::Value("0");
	item1["user_name"] = Json::Value("syq");
	UserList.append(item1);
	root1["users"] = UserList;


	seat1["s_left"]=Json::Value("434");
	seat1["s_num"]=Json::Value("0");
	seat1["s_state"]=Json::Value("0");
	seat1["s_top"]=Json::Value("196");
	
	seat2["s_left"]=Json::Value("506");
	seat2["s_num"]=Json::Value("1");
	seat2["s_state"]=Json::Value("0");
	seat2["s_top"]=Json::Value("45");

	seat3["s_left"]=Json::Value("408");
	seat3["s_num"]=Json::Value("2");
	seat3["s_state"]=Json::Value("0");
	seat3["s_top"]=Json::Value("56");

	seat4["s_left"]=Json::Value("319");
	seat4["s_num"]=Json::Value("3");
	seat4["s_state"]=Json::Value("0");
	seat4["s_top"]=Json::Value("104");

	seat5["s_left"]=Json::Value("250");
	seat5["s_num"]=Json::Value("4");
	seat5["s_state"]=Json::Value("0");
	seat5["s_top"]=Json::Value("186");
	
	SeatList.append(seat1);
	SeatList.append(seat2);
	SeatList.append(seat3);
	SeatList.append(seat4);
	SeatList.append(seat5);
	
	root1["seats"]= SeatList;
	

	rooter["m_info"].append(root1);
	rooter["m_op"]=Json::Value("init");

	//strcpy(sendBuf,(fw.write(rooter)).c_str());
	strcpy(sendBuf,(sw.write(rooter)).c_str());
	ret = send(sockConn,sendBuf,sizeof(sendBuf),0);
	if(ret==-1){
		printf("send err:%d\n",ret);
	}
	printf("������%d���ֽ�\n",ret);

	printf("%s\n",sendBuf);
printf("------------------------------��������3�� send ����----------------------\n");		


	clearBuf();
	Json::Value service;
	service["m_op"] = Json::Value("service");
	service["meetroom_name"]=Json::Value("420");
	service["s_name"]=Json::Value("tea");
	service["s_num"]=Json::Value("1");
	service["user_name"]=Json::Value("syq");
	strcpy(sendBuf,(fw.write(service)).c_str());



	Sleep(30000);
	send(sockConn,sendBuf,sizeof(sendBuf),0);
	/*
	while(1){
		send(sockConn,sendBuf,sizeof(sendBuf),0);
		clearBuf();
		Sleep(50000);
	}*/
	
	closesocket( sockConn ) ;
	//����socket��̻���
	WSACleanup() ;
	system("PAUSE");

}
