/*
 * CHttpTaskMgr.cpp
 *
 *  Created on: Jan 22, 2015
 *      Author: szj0306
 */

#include "uploadmgr.h"
#include "fileupload.h"
#include "utility.h"

CUploadMgr::CUploadMgr()
	: m_pUpload(NULL)
	, m_uploadVec()
	, m_pMultiHandle(NULL)
{
}

CUploadMgr::~CUploadMgr()
{
	CToolKit::ReleasePtr<CUpload>(m_pUpload);
	for (TCUploadVec::iterator iter = m_uploadVec.begin();
		iter != m_uploadVec.end(); ++iter)
	{
		CToolKit::ReleasePtr<CUpload>(*iter);
	}
	m_pMultiHandle = NULL;
}

int CUploadMgr::Init(CURLM *pCurlMHandle)
{
	if (NULL == (m_pMultiHandle = pCurlMHandle))
	{
		return -1;
	}
	if (NULL == (m_pUpload = new CFileUpload(m_pMultiHandle)))
	{
		return -1;
	}
	m_pUpload->Init();
	return 0;
}

int CUploadMgr::StartUpload()
{
	THttpParam &param = CSingleton<CHttpCli>::Instance()->HttpParam();
	if (NULL == m_pMultiHandle)
	{
		HTTPCLI_LOG(param, ERRORTYPE, "CurlMultiHandle is NULL, Init firstly.");
		return -1;
	}
	if (0 != m_pUpload->StartPost())
	{
		return -1;
	}
//	m_uploadVec.push_back(m_pUpload);
	return 0;
}

void CUploadMgr::CheckUpload()
{
	TCUploadVec::iterator iter = m_uploadVec.begin();
	while (iter != m_uploadVec.end())
	{
		CUpload &upload(**iter);
		if (upload.UploadStatus() != CUpload::UPLOAD_FINISH
				&& upload.UploadStatus() != CUpload::UPLOAD_RUNNING)
		{
			delete *iter; *iter = NULL;
			iter = m_uploadVec.erase(iter);
		}
		else
		{
			upload.CheckHttpTask();
			++iter;
		}
	}
}
