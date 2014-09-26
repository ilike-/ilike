// SocketTest.cpp : 定义控制台应用程序的入口点。
//@ilike 2014-09-26

#include "stdafx.h"
#include <iostream>
#include <Winsock2.h>  
#pragma comment(lib, "ws2_32.lib" )

#include "example.pb.h"
#pragma comment(lib,"libprotobuf.lib") 
#pragma comment(lib, "libprotoc.lib")


#include <iostream>
#include <string>
const bool testID = false;
using namespace std;
const int SIZE_SEND_RECV = 50;
int _tmain(int argc, _TCHAR* argv[])
{

		WORD wVersionRequested;
		WSADATA wsaData;
		int err;

		wVersionRequested = MAKEWORD( 1, 1 );//第一个参数为低位字节；第二个参数为高位字节

		err = WSAStartup( wVersionRequested, &wsaData );//对winsock DLL（动态链接库文件）进行初始化，协商Winsock的版本支持，并分配必要的资源。
		if ( err != 0 )
		{
			return 0;
		}

		if ( LOBYTE( wsaData.wVersion ) != 1 ||HIBYTE( wsaData.wVersion ) != 1 )//LOBYTE（）取得16进制数最低位；HIBYTE（）取得16进制数最高（最左边）那个字节的内容		
		{
			WSACleanup( );
			return 0;
		}
		for(int index=0;;index++)
		{
			SOCKET sockClient=socket(AF_INET,SOCK_STREAM,0);

			SOCKADDR_IN addrClt;//需要包含服务端IP信息
			addrClt.sin_family=AF_INET; 

			if (testID == true)
			{
				addrClt.sin_addr.S_un.S_addr=inet_addr("192.168.2.250");// inet_addr将IP地址从点数格式转换成网络字节格式整型。
				addrClt.sin_port=htons(7006);
			}
			else
			{
				addrClt.sin_addr.S_un.S_addr=inet_addr("192.168.2.171");// inet_addr将IP地址从点数格式转换成网络字节格式整型。
				addrClt.sin_port=htons(6000);
			}

			connect(sockClient,(SOCKADDR*)&addrClt,sizeof(SOCKADDR));//客户机向服务器发出连接请求

			TEEBIK::Message message;
			char text[20];
			sprintf_s(text,"HelloWorld, %d", index);
			

			message.set_text(text);
			auto s4 = message.id_size();
		
			for (int i = 0; i < 10; i++)
			{
				message.add_id(19910213+i);
				cout<<message.id(i)<<endl;
			}
			
			string str;
			message.SerializePartialToString(&str);
			//auto size = strlen(str.c_str());
			char sendbuff[SIZE_SEND_RECV];
			strcpy_s(sendbuff, str.c_str());
			auto send_result = send(sockClient, sendbuff, sizeof(sendbuff),0);

			char recvBuf[SIZE_SEND_RECV+1];
			
			int recv_byte = recv(sockClient,recvBuf,50,0);
			if (recv_byte < 0)
			{
				closesocket(sockClient);
				Sleep(2000);
				continue;
			}
			recvBuf[recv_byte] = '\0';
			string strRev = recvBuf;
			TEEBIK::Message messRec;
			//messRec.IsInitialized();
			auto result = messRec.ParseFromString(sendbuff);

			cout<< messRec.text()<<endl;
			closesocket(sockClient);
			Sleep(2000);
		}
		WSACleanup();
		return 0;
}

