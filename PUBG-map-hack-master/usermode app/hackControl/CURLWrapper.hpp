#pragma once
#include "../libcurl-vc-x64-release-static-ipv6-sspi-winssl/include/curl/curl.h"
#include "../json/json.hpp"
#include "Types.hpp"
#include <iostream>

class CURLWrapper
{
public:

	// Constructor of this wrapper. Also initializes the CURL.
	CURLWrapper()
	{
		curl_global_init(CURL_GLOBAL_ALL);
		m_curl = curl_easy_init();
	}

	// destructor of this wrapper
	~CURLWrapper()
	{
		curl_easy_cleanup(m_curl);
	}

	bool getReadyState()
	{
		if (m_curl) return true;
		return false;
	}

	static size_t write_data(void* w_buffer, size_t w_size, size_t w_nmemb, void* w_userp)
	{
		return w_size * w_nmemb;
	}

	int sendData(std::string& w_data)
	{
		try
		{
			struct curl_slist *headers = NULL;

			headers = curl_slist_append(headers, "Content-Type: application/json");

			curl_easy_setopt(m_curl, CURLOPT_VERBOSE, 0L);
			curl_easy_setopt(m_curl, CURLOPT_HTTPHEADER, headers);
			curl_easy_setopt(m_curl, CURLOPT_URL, "http://127.0.0.1:7890/");
			curl_easy_setopt(m_curl, CURLOPT_CUSTOMREQUEST, "POST");
			// curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, 30L);
			curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, write_data);
			curl_easy_setopt(m_curl, CURLOPT_POSTFIELDS, w_data.data());
			curl_easy_setopt(m_curl, CURLOPT_NOSIGNAL, 1);

			curl_easy_perform(m_curl);

			curl_slist_free_all(headers);
			curl_easy_reset(m_curl);
		}
		catch (std::exception& e)
		{
			std::cout << e.what() << std::endl;
			return -1;
		}
	}

private:
	CURL* m_curl;			// the curl interface
	json m_data;			// json data wrapper
};