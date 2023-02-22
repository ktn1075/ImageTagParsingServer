#include "Hparser.h"

std::string Hparser::PasingHtml(const char* html)
{
	std::string tempparsing = html;
	size_t hostindex = tempparsing.find("=https://");
    size_t endindex = tempparsing.find("end");
	if (hostindex != -1 && endindex != -1)
	{
		std::string temp= tempparsing.substr(hostindex +9, endindex-9-hostindex);
		int l = temp.find("com/");
		if (l == -1)
		{
			l = temp.find("kr/");
			std::string lasttemp = temp.substr(l + 2);
			return httpRequest(temp.substr(0, l + 2), lasttemp);
		}
		std::string lasttemp = temp.substr(l+3);
		return httpRequest(temp.substr(0,l+3),lasttemp);
	}
	// 찾지 못할경우 기본 이미지 리턴되게 코딩 
	return std::string("https://news.imaeil.com/photos/2020/02/20/2020022022361319178_l.jpg"); // 기본 국방부 이미지 리턴 
}
std::wstring s2ws(const std::string& s)
{
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	return r;
}
std::string Hparser::httpRequest(std::string uri, std::string location)
{
	
	DWORD dwSize = 0;
	DWORD dwDownloaded = 0;
	LPSTR pszOutBuffer;
	BOOL  bResults = FALSE;
	std::string x;
	
	HINTERNET  hSession = NULL,
		hConnect = NULL,
		hRequest = NULL;
	// Use WinHttpOpen to obtain a session handle.
	hSession = WinHttpOpen(L"HTTP/1.1 ",
		WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
		WINHTTP_NO_PROXY_NAME,
		WINHTTP_NO_PROXY_BYPASS, 0);

	// Specify an HTTP server.
	if (hSession)  // 해당 서버에접속
		hConnect = WinHttpConnect(hSession, s2ws(uri).c_str(),
			INTERNET_DEFAULT_HTTPS_PORT, 0);

	// Create an HTTP request handle.
	if (hConnect) //location
		hRequest = WinHttpOpenRequest(hConnect, L"GET",s2ws(location).c_str(),
			NULL, WINHTTP_NO_REFERER,
			WINHTTP_DEFAULT_ACCEPT_TYPES,
			WINHTTP_FLAG_SECURE);

	// Send a request.
	if (hRequest)
		bResults = WinHttpSendRequest(hRequest,
			WINHTTP_NO_ADDITIONAL_HEADERS, 0,
			WINHTTP_NO_REQUEST_DATA, 0,
			0, 0);
	// End the request.
	if (bResults)
		bResults = WinHttpReceiveResponse(hRequest, NULL);

	// Keep checking for data until there is nothing left.
	if (bResults)
	{
		do
		{
			// Check for available data.
			dwSize = 0;
			if (!WinHttpQueryDataAvailable(hRequest, &dwSize))
				printf("Error %u in WinHttpQueryDataAvailable.\n",
					GetLastError());

			// Allocate space for the buffer.
			pszOutBuffer = new char[dwSize + 1];
			if (!pszOutBuffer)
			{
				printf("Out of memory\n");
				dwSize = 0;
			}
			else
			{
				// Read the data.
				ZeroMemory(pszOutBuffer, dwSize + 1);

				if (!WinHttpReadData(hRequest, (LPVOID)pszOutBuffer,
					dwSize, &dwDownloaded))
					printf("Error %u in WinHttpReadData.\n", GetLastError());
				
				else
				{
					std::string A = pszOutBuffer;
					std::cout << A;
					//회사별로 명칭이 전부다름
					int find = -1;
					for (int i = 0; i < 2; i++)
					{
						find = A.find(taglist[i]);
					}
					if (find != -1)
					{
						// TODO 나중에 모듈로 파서 제작 
					    std::string test = A.substr(find, 100);
						int front = test.find("https://");
						int back = test.find(">");
						int httpssize = 1;
						if (front == -1)
						{
							front = test.find("http://");
						}
						if (front != -1 && back != -1)
						{
							    x = test.substr(front
								, back-2);
								return x;
						}
						delete[] pszOutBuffer;
						break;
					}
					
				}
				// Free the memory allocated to the buffer.
			}
		} while (dwSize > 0);
	}
	// Report any errors.
	if (!bResults) std::cout<<GetLastError();
	
	// Close any open handles.
	if (hRequest) WinHttpCloseHandle(hRequest);
	if (hConnect) WinHttpCloseHandle(hConnect);
	if (hSession) WinHttpCloseHandle(hSession);

	return "https://news.imaeil.com/photos/2020/02/20/2020022022361319178_l.jpg";
}

std::string Hparser::sepaUri(std::string sepa)
{
	return std::string();
}
