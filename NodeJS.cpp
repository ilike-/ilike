// NodeJS.cpp : 定义控制台应用程序的入口点。
//@ilike 2014-09-26

#include "stdafx.h"
#include <iostream>
#include <Winsock2.h>
#include <string>
#pragma  comment(lib, "ws2_32.lib")
#include "example.pb.h"
#pragma comment(lib,"libprotobuf.lib") 
#pragma comment(lib, "libprotoc.lib")
const int SIZE_SEND_RECV = 50;

int _tmain(int argc, _TCHAR* argv[])
{
	WORD wVersionRequested;

	WSADATA wsaData;
	 
	int err;

	wVersionRequested = MAKEWORD( 1, 1 );



	err = WSAStartup( wVersionRequested, &wsaData );

	if ( err != 0 ) {

		return 0;

	}

	if ( LOBYTE( wsaData.wVersion ) != 1 ||

		HIBYTE( wsaData.wVersion ) != 1 ) {

			WSACleanup( );

			return 0;

	}

	SOCKET sockSrv=socket(AF_INET,SOCK_STREAM,0);

	SOCKADDR_IN addrSrv;

	addrSrv.sin_addr.S_un.S_addr=htonl(INADDR_ANY);

	addrSrv.sin_family=AF_INET;

	addrSrv.sin_port=htons(6000);



	bind(sockSrv,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR));



	listen(sockSrv,5);



	SOCKADDR_IN addrClient;

	int len=sizeof(SOCKADDR);

	while(1)

	{
	
		SOCKET sockConn=accept(sockSrv,(SOCKADDR*)&addrClient,&len);

		printf("TONICE: visiting IP:%s \n", inet_ntoa(addrClient.sin_addr));
		
		char recvBuff[SIZE_SEND_RECV+1];
		auto recvByte = recv(sockConn, recvBuff, SIZE_SEND_RECV, 0);
		//recvBuff[recvByte] = '/0';
		TEEBIK::Message message;
		if (recvByte > 0 )
		{
			std::string str = recvBuff;
			message.ParseFromString(str);
		}
		std::cout<< message.text()<<std::endl;
		std::cout<< message.id()<<std::endl;
		closesocket(sockConn);
	}
	return 0;
}

