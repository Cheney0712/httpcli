/*
 * CFileUpload.cpp
 *
 *  Created on: Jan 22, 2015
 *      Author: szj0306
 */

#include <stdlib.h>
#include "fileupload.h"
#include "httpclidef.h"
#include "httpcli.h"

CFileUpload::CFileUpload(CURLM *pMultiHandle)
	: CUpload(pMultiHandle)
	, m_sAllRngQ()
	, m_sPostingRngQ()
	, m_sPostedRngQ()
	, m_nTimePointBeg(0)
{
	// TODO Auto-generated constructor stub
}

CFileUpload::~CFileUpload()
{
	// TODO Auto-generated destructor stub
	m_eRecovery = UPRECOVERY_INVALID;
	m_eUpStatus = UPLOAD_INVALID;
}

void CFileUpload::Init()
{
	THttpParam &param = CSingleton<CHttpCli>::Instance()->HttpParam();
	m_sAllRngQ += range(0, param.FileSize());
	m_nTimePointBeg = time(NULL);
	HTTPCLI_LOG(param, LOGTYPE, "Init upload scene: %s", m_sAllRngQ.to_string().c_str());
}

int CFileUpload::StartPost()
{
	THttpParam &param = CSingleton<CHttpCli>::Instance()->HttpParam();

	int nRet = -1;
	if (IsUploadOver())
	{
		HTTPCLI_LOG(param, ERRORTYPE, "Upload has completely");
		return nRet;
	}
	if (RestUpTask().empty())
	{
		HTTPCLI_LOG(param, ERRORTYPE, "no more POST task, maybe it has uploaded completely");
		return nRet;
	}

	range postRng;
	if (param.UpType() == UPTYPE_FORM)
	{
		HTTPCLI_LOG(param, LOGTYPE, "standary POST in form");
		postRng.set_length(param.FileSize());
		CHttpTask *pHttpTask = new CHttpPostTask(m_pMultiHandle, this);
		pHttpTask->Init(postRng);
		if (0 != (nRet = pHttpTask->PostEntireFile()))
		{
			delete pHttpTask; pHttpTask = NULL;
			return nRet;
		}
		m_sPostingRngQ += postRng;
		m_httpTaskVec.push_back(pHttpTask);
	}
	else if (param.UpType() == UPTYPE_UPBLK)
	{
//		if (m_eRecovery == UPRECOVERY_INVALID)
//		{
//			HTTPCLI_LOG(param, LOGTYPE, "resumable POST in block, "
//						"recovery upscene firstly");
//			CHttpTask *pHttpTask = new CHttpPostTask(m_pMultiHandle, this);
//			pHttpTask->Init(postRng);
//			if (0 != (nRet = pHttpTask->UpRecovery()))
//			{
//				delete pHttpTask; pHttpTask = NULL;
//				return nRet;
//			}
//			m_eRecovery = UPRECOVERY_UNFINISHED;
//			m_httpTaskVec.push_back(pHttpTask);
//		}
//		else if (m_eRecovery == UPRECOVERY_FINISH)
		{
			HTTPCLI_LOG(param, LOGTYPE, "resumable POST in block");
			const std::vector<range> &rngVec = RestUpTask();
			postRng = rngVec[rand() % rngVec.size()];
			if (postRng.length() > param.BlkSize())
			{
				postRng.set_length(param.BlkSize());
			}
			CHttpTask *pHttpTask = new CHttpPostTask(m_pMultiHandle, this);
			pHttpTask->Init(postRng);
			if (0 != (nRet = pHttpTask->PostBlock()))
			{
				delete pHttpTask; pHttpTask = NULL;
				return nRet;
			}
			m_sPostingRngQ += postRng;
			m_httpTaskVec.push_back(pHttpTask);
		}
	}
	m_eUpStatus = UPLOAD_RUNNING;

	return (nRet = 0);
}

void CFileUpload::CheckHttpTask()
{
	TCHttpTaskVec::iterator iter = m_httpTaskVec.begin();
	while (iter != m_httpTaskVec.end())
	{
		const CHttpTask &task(**iter);
		if (task.HttpUpStatus() != CHttpTask::HTTP_UPLOAD_FINISH
				&& task.HttpUpStatus() != CHttpTask::HTTP_UPLOAD_RUNNING)
		{
			delete *iter; *iter = NULL;
			iter = m_httpTaskVec.erase(iter);
		}
		else
		{
			CHttpCli *pHttpCli = CSingleton<CHttpCli>::Instance();
			THttpParam &param = pHttpCli->HttpParam();
			time_t nCurTime = time(NULL);
			if (nCurTime >= m_nTimePointBeg + param.TimeOut())
			{
				m_eUpStatus = UPLOAD_TIMEOUT;
				HTTPCLI_LOG(param, ERRORTYPE, "the POST request has timeout.");
			}

			++iter;
		}
	}
}

bool CFileUpload::IsUploadOver() const
{
	CHttpCli *pHttpCli = CSingleton<CHttpCli>::Instance();
	THttpParam &param = pHttpCli->HttpParam();
	return (m_sPostedRngQ.all_range_length() == param.FileSize());
}

const std::vector<range> CFileUpload::RestUpTask() const
{
	range_queue tmpRngQ;
	m_sAllRngQ.sub_to(m_sPostedRngQ, tmpRngQ);
	tmpRngQ -= m_sPostingRngQ;
	return tmpRngQ.ranges();
}
