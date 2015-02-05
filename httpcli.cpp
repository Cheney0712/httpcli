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
	int nRet = 0;

	if (0 != CSingleton<CUploadMgr>::Instance()->StartUpload())
	{
		return (nRet = -1);
	}
	THttpParam &param = CSingleton<CHttpCli>::Instance()->HttpParam();
	curl_multi_perform(m_pMultiHandle, &m_nRunning);

	do
	{
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
		if (curl_timeo >= 0) {
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
#ifdef _WIN32
			Sleep(100);
			rc = 0;
#else
			/* Portable sleep for platforms other than Windows. */
			struct timeval wait = { 0, 100 * 1000 }; /* 100ms */
			rc = select(0, NULL, NULL, NULL, &wait);
#endif
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
			/*
			 * timeout
			 * 1. check task
			 * 2. add new POST task
			 */
			CSingleton<CUploadMgr>::Instance()->CheckUpload();
			CSingleton<CUploadMgr>::Instance()->StartUpload();
			break;
		default:
			/* readable/writable sockets */
//			CSingleton<CUploadMgr>::Instance()->CheckUpload();
//			CSingleton<CUploadMgr>::Instance()->StartUpload();
			curl_multi_perform(m_pMultiHandle, &m_nRunning);
			break;
		}
	} while (m_nRunning);

	return nRet;
}
