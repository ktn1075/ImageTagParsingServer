// HTTPSERVERPROXY.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include"pch.h"
#include"Hparser.h"
#include"CThraed.h"

void work(SOCKET s)
{
	// 기본 header 정보
	char protocol[] = "HTTP/1.1 200 OK\r\n";
	char servname[] = "SERVER TEST\r\n";
	char cntLen[] = "Content-length:2048\r\n";
	char cntType[] = "Content-Type : text/html\r\n\r\n";
	char sndbuffer[2048];
	char buf[2048];


	int recvlen = ::recv(s, buf, 2048, 0);

	buf[recvlen - 1] = 0;

	Hparser t1;

	std::string image_link = t1.PasingHtml(buf);

	send(s, protocol, strlen(protocol), 0);
	
	send(s, servname, strlen(servname), 0);
	
	send(s, cntLen, strlen(cntLen), 0);
	
	send(s,cntType,strlen(cntType),0);
	
	strcpy_s(sndbuffer,image_link.c_str());
	
	send(s, sndbuffer, strlen(sndbuffer), 0);
	
	closesocket(s);

}
int main()
{
	CThread pool(50);

	WSAData wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	
	SOCKET serversocket = ::socket(AF_INET, SOCK_STREAM, 0);
	
	if (serversocket == INVALID_SOCKET)
	{
		int errCode = ::WSAGetLastError();
		std::cout << "socket error code" << errCode << std::endl;
	}

	SOCKADDR_IN serveraddr;
	::memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = ::htons(10000);
	
	::bind(serversocket, (SOCKADDR*)& serveraddr, sizeof(serveraddr));
	
	::listen(serversocket, 10);

	while (true)
	{
		SOCKADDR_IN clientaddr;

		::memset(&clientaddr, 0, sizeof(clientaddr));
		
		int addlen = sizeof(clientaddr);
		
		SOCKET clientsock = ::accept(serversocket, (SOCKADDR*)& clientaddr, &addlen);
		
		pool.EnqueueJob(work, clientsock);
	}
}