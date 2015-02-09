/**
 * ============================================================================
 * @file	httpcli.cpp
 *
 * @brief   
 * @details 
 *
 * @version 1.0  
 * @date	2014-06-29 15:21:13
 *
 * @author  shizhijie, shizhijie@baofeng.com
 * ============================================================================
 */

#include "httpcli.h"
#include "httpclidef.h"

int CHttpCli::Start()
{
	THttpParam &param = CSingleton<CHttpCli>::Instance()->HttpParam();
	int nRet = 0, m_nPreRunning = 0, m_nRunning = 0;
	while (0 == CSingleton<CUploadMgr>::Instance()->StartUpload())
		;
	do
	{
		CSingleton<CUploadMgr>::Instance()->CheckUpload();

		struct timeval timeout;
		int rc, maxfd = -1; /* select() return code */
		long curl_timeo = -1;
		CURLMcode mc; /* curl_multi_fdset() return code */
		fd_set fdread, fdwrite, fdexcep;

		FD_ZERO(&fdread);
		FD_ZERO(&fdwrite);
		FD_ZERO(&fdexcep);

		/* set a suitable timeout to play around with */
		timeout.tv_sec = 1;
		timeout.tv_usec = 0;

		curl_multi_timeout(m_pMultiHandle, &curl_timeo);
		if (curl_timeo >= 0)
		{
			timeout.tv_sec = curl_timeo / 1000;
			if (timeout.tv_sec > 1)
			{
				timeout.tv_sec = 1;
			}
			else
			{
				timeout.tv_usec = (curl_timeo % 1000) * 1000;
			}
		}

		/* get file descriptors from the transfers */
		mc = curl_multi_fdset(m_pMultiHandle, &fdread, &fdwrite, &fdexcep, &maxfd);
		if (mc != CURLM_OK)
		{
			nRet = -1;
			HTTPCLI_LOG(param, ERRORTYPE, "curl_multi_fdset() failed, code %s",
					curl_multi_strerror(mc));
			break;
		}

		/* On success the value of maxfd is guaranteed to be >= -1. We call
		 select(maxfd + 1, ...); specially in case of (maxfd == -1) there are
		 no fds ready yet so we call select(0, ...) --or Sleep() on Windows--
		 to sleep 100ms, which is the minimum suggested value in the
		 curl_multi_fdset() doc. */
		if (maxfd == -1)
		{
			/* Portable sleep for platforms other than Windows. */
			struct timeval wait = { 0, 100 * 1000 }; /* 100ms */
			rc = select(0, NULL, NULL, NULL, &wait);
		}
		else
		{
			/* Note that on some platforms 'timeout' may be modified by select().
			 If you need access to the original value save a copy beforehand. */
			rc = select(maxfd + 1, &fdread, &fdwrite, &fdexcep, &timeout);
		}

		switch (rc)
		{
		case -1:
			/* select error */
			nRet = -1;
			break;
		case 0:
			/* timeout */
		default:
			/* readable/writable sockets */
			curl_multi_perform(m_pMultiHandle, &m_nRunning);
			break;
		}

		if (m_nPreRunning != m_nRunning)
		{
			CheckInfo();
			m_nPreRunning = m_nRunning;
		}

	} while (m_nRunning);

	return (nRet);
}

void CHttpCli::CheckInfo()
{
	THttpParam &param = CSingleton<CHttpCli>::Instance()->HttpParam();
	CURLMsg *pMsg = NULL;
	do
	{
		int nMsgNum = 0;
		pMsg = curl_multi_info_read(m_pMultiHandle, &nMsgNum);
		for (int i = 0; i < nMsgNum; ++i)
		{
			/* This easy handle has completed. 'result' contains
			 the CURLcode of the transfer */
			if (CURLMSG_DONE == pMsg[i].msg)
			{
				HTTPCLI_LOG(param, LOGTYPE, "easy handle has completed: %d", pMsg[i].data.result);
				curl_multi_remove_handle(m_pMultiHandle, pMsg[i].easy_handle);
			}
		}
	} while (NULL != pMsg);
}
