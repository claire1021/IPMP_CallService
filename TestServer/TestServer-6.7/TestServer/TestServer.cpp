// TestServer.cpp : 定义控制台应用程序的入口点。
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
		int ret = 0;
		clearBuf();
printf("------------------------------服务器第一次接收数据----------------------\n");
		//接收数据
		printf("receiving.....\n");
		ret = recv( sockConn , recvBuf ,sizeof(recvBuf), 0 ) ;
		if(ret==-1){
			printf("recv err:%d\n",ret);
		}
		//解析json
		
		if(reader.parse(recvBuf,root))
		{
			string m_object = root["m_object"].asString();
			string m_op = root["m_op"].asString();
			cout<<"m_object="<<m_object<<endl;
			cout<<"m_op="<<m_op<<endl;
		}
		printf("data from client: %s\n" , recvBuf ) ;
printf("------------------------------服务器第一次 recv 数据结束----------------------\n\n\n");

printf("------------------------------服务器第1次 send  数据----------------------\n");
		//发送数据 所有会议室的
		jsonsend();
		ret = send( sockConn , sendBuf, sizeof(sendBuf) , 0 ) ;
		if(ret==-1){
			printf("recv err:%d\n",ret);
		}
		printf("发送了%d个字节\n",ret);
		printf("send end.....\n");
printf("------------------------------服务器第1次 send 数据结束----------------------\n\n\n");



printf("------------------------------服务器第2次 recv  数据----------------------\n");
		//继续接收用户所选的会议室
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

printf("------------------------------服务器第2次 recv 数据结束----------------------\n\n\n");

printf("------------------------------服务器第3次 send 数据----------------------\n");

	map<int,string>::iterator it;
	Json::Value outerList;
	Json::Value innerList;
	Json::Value item1,item2,item3,item4,item5;
	int i=20;
	for(it=meetrooms.begin();it!=meetrooms.end();it++){
		item1["meetroom_name"] =Json::Value(it->second);
		item2["meet_name"]=Json::Value("周会");
		item3["meet_time"]=Json::Value("2016-7-6");
		item4["meet_place"]=Json::Value("西一楼");
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
printf("------------------------------服务器第3次 send 结束----------------------\n");		
	closesocket( sockConn ) ;
	//清理socket编程环境
	WSACleanup() ;
	system("PAUSE");

}
