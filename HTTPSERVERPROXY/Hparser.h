#pragma once
#include"pch.h"
class Hparser
{
private:
	std::string taglist[2] = {"twitter:image","og:image"};

public:
	std::string PasingHtml(const char* html);
	std::string httpRequest(std::string uri, std::string location);
	std::string sepaUri(std::string sepa);
};

