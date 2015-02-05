/*
 * CHttpTask.h
 *
 *  Created on: Jan 22, 2015
 *      Author: szj0306
 */

#ifndef CHTTPTASK_H_
#define CHTTPTASK_H_

#include <string>
#include <curl/curl.h>
#include "range.h"
#include "upload.h"

class CHttpTask
{
public:
	enum EHttpUpStatus
	{
		HTTP_UPLOAD_INVALID = -1,
		HTTP_UPLOAD_RUNNING = 0,
		HTTP_UPLOAD_FINISH = 1,
		HTTP_UPLOAD_FAILED = 2,
		HTTP_UPLOAD_TIMEOUT = 3,
	};

public:
	CHttpTask(CURLM *pMultiHandle, CUpload *pUpload);
	virtual ~CHttpTask();

public:
	virtual void Init(const range &postRng) = 0;
	virtual int UpRecovery() = 0;
	virtual int PostEntireFile() = 0;
	virtual int PostBlock() = 0;
	virtual void HttpUpStatus(EHttpUpStatus status);
	virtual EHttpUpStatus HttpUpStatus();
	virtual const EHttpUpStatus HttpUpStatus() const;
	virtual CUpload *Upload();
	virtual const CUpload *Upload() const;
	virtual CURL *Curl();
	virtual const CURL *Curl() const;

protected:
	CURLM *m_pMultiHandle;
	CURL *m_pCurl;
	CUpload *m_pUpload;
	EHttpUpStatus m_eHttpUpStatus;
};

inline CHttpTask::CHttpTask(CURLM *pMultiHandle, CUpload *pUpload)
	: m_pMultiHandle(pMultiHandle)
	, m_pCurl(NULL)
	, m_pUpload(pUpload)
	, m_eHttpUpStatus(HTTP_UPLOAD_INVALID)
{
	// TODO Auto-generated constructor stub
	m_pCurl = curl_easy_init();
}

inline CHttpTask::~CHttpTask()
{
	// TODO Auto-generated destructor stub
	m_pMultiHandle = m_pUpload = NULL;
	if (m_pCurl != NULL)
	{
		curl_easy_cleanup(m_pCurl);
		m_pCurl = NULL;
	}
}

inline void CHttpTask::HttpUpStatus(EHttpUpStatus status)
{
	m_eHttpUpStatus = status;
}

inline CHttpTask::EHttpUpStatus CHttpTask::HttpUpStatus()
{
	return m_eHttpUpStatus;
}

inline const CHttpTask::EHttpUpStatus CHttpTask::HttpUpStatus() const
{
	return m_eHttpUpStatus;
}

inline CURL *CHttpTask::Curl()
{
	return m_pCurl;
}

inline const CURL *CHttpTask::Curl() const
{
	return m_pCurl;
}

inline CUpload *CHttpTask::Upload()
{
	return m_pUpload;
}

inline const CUpload *CHttpTask::Upload() const
{
	return m_pUpload;
}

#endif /* CHTTPTASK_H_ */
