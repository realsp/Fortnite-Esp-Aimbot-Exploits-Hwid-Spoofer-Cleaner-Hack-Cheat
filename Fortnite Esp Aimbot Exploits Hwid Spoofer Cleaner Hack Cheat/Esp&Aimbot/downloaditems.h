#define _CRT_SECURE_NO_WARNINGS
#pragma once
#include "imports.h"

namespace Internet
{
	std::string Download(const char* URL)
	{
		//ik bad method of doing shit

		HINTERNET internetfile;
		std::string buffer2;
		char buffer[999999];
		DWORD bytesRead;

		DeleteUrlCacheEntryA(URL);

		HINTERNET interwebs = InternetOpenA("Imagine_Using_Http_Debugger_for_free_shit_lmfao", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, INTERNET_FLAG_PRAGMA_NOCACHE);

		if (!interwebs)
			return (char*)"";

		internetfile = InternetOpenUrlA(interwebs, URL, NULL, NULL, NULL, NULL);

		if (!internetfile)
			return (char*)"";

		InternetReadFile(internetfile, &buffer, 99999999, &bytesRead);

		buffer2 = buffer;

		InternetCloseHandle(interwebs);
		InternetCloseHandle(internetfile);

		DeleteUrlCacheEntryA(URL);

		return buffer2;
	}
}