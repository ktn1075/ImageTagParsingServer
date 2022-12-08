// HTTPSERVERPROXY.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include"pch.h"
#include"Hparser.h"
#include"CThraed.h"

void work(SOCKET s)
{
	char protocol[] = "HTTP/1.1 200 OK\r\n";
	char servname[] = "SERVER TEST\r\n";
	char cntLen[] = "Content-length:2048\r\n";
	char cntType[] = "Content-Type : text/html\r\n\r\n";
	char sndbuffer[2048];
	char buf[2048];
	// 1.read
	// 2.파싱
	// 3.파싱된 Url 요청
	// 4.이미지 태그에서 파싱 
	// 5. 
	int recvlen = ::recv(s, buf, 2048, 0);
	buf[recvlen - 1] = 0;
	Hparser t1;
	std::string image_link = t1.PasingHtml(buf);
//	std::cout << image_link;
	send(s, protocol, strlen(protocol), 0);
	send(s, servname, strlen(servname), 0);
	send(s, cntLen, strlen(cntLen), 0);
	send(s,cntType,strlen(cntType),0);
	strcpy_s(sndbuffer,image_link.c_str());
	//std::cout << sndbuffer;
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
// 프로그램 실행: <Ctrl+F5> 또는 [디버그] > [디버깅하지 않고 시작] 메뉴
// 프로그램 디버그: <F5> 키 또는 [디버그] > [디버깅 시작] 메뉴

// 시작을 위한 팁: 
//   1. [솔루션 탐색기] 창을 사용하여 파일을 추가/관리합니다.
//   2. [팀 탐색기] 창을 사용하여 소스 제어에 연결합니다.
//   3. [출력] 창을 사용하여 빌드 출력 및 기타 메시지를 확인합니다.
//   4. [오류 목록] 창을 사용하여 오류를 봅니다.
//   5. [프로젝트] > [새 항목 추가]로 이동하여 새 코드 파일을 만들거나, [프로젝트] > [기존 항목 추가]로 이동하여 기존 코드 파일을 프로젝트에 추가합니다.
//   6. 나중에 이 프로젝트를 다시 열려면 [파일] > [열기] > [프로젝트]로 이동하고 .sln 파일을 선택합니다.
