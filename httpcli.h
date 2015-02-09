/**
 * ============================================================================
 * @file	httpcli.h
 *
 * @brief   
 * @details 
 *
 * @version 1.0  
 * @date	2014-06-23 11:43:32
 *
 * @author  shizhijie, shizhijie@baofeng.com
 * ============================================================================
 */

#ifndef __BFP2P_SERVER_HTTPCLI_H__
#define __BFP2P_SERVER_HTTPCLI_H__

#include <stdint.h>
#include <string>
#include <sstream>
#include <curl/curl.h>
#include "httpclidef.h"
#include "singleton.h"
#include "httpparam.h"
#include "uploadmgr.h"

class CHttpCli : public CNonCopyable
{
public:
	CHttpCli();
	virtual ~CHttpCli();

public:
	int Init(const THttpParam &param);
	void CleanUp();
	int Start();

	THttpParam &HttpParam();
	const THttpParam &HttpParam() const;

private:
	void CheckInfo();

private:
	CURLM *m_pMultiHandle;
	THttpParam m_httpParam;
};

inline CHttpCli::CHttpCli()
	: m_pMultiHandle(NULL)
	, m_httpParam()
{
}

inline CHttpCli::~CHttpCli()
{
	if (m_pMultiHandle != NULL)
	{
		curl_multi_cleanup(m_pMultiHandle);
		m_pMultiHandle = NULL;
	}
}

inline int CHttpCli::Init(const THttpParam &param)
{
	m_pMultiHandle = curl_multi_init();
	if (NULL == m_pMultiHandle)
	{
		return -1;
	}
	if (CURLE_OK != curl_global_init(CURL_GLOBAL_ALL))
	{
		return -1;
	}
	m_httpParam = param;
	return CSingleton<CUploadMgr>::Instance()->Init(m_pMultiHandle);
}

inline void CHttpCli::CleanUp()
{
	if (NULL != m_pMultiHandle)
	{
		curl_multi_cleanup(m_pMultiHandle);
		m_pMultiHandle = NULL;
	}
	curl_global_cleanup();
}

inline THttpParam &CHttpCli::HttpParam()
{
	return m_httpParam;
}

inline const THttpParam &CHttpCli::HttpParam() const
{
	return m_httpParam;
}

#endif //__BFP2P_SERVER_HTTPCLI_H__
