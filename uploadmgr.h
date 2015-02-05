/*
 * CHttpTaskMgr.h
 *
 *  Created on: Jan 22, 2015
 *      Author: szj0306
 */

#ifndef CHTTPTASKMGR_H_
#define CHTTPTASKMGR_H_

#include <vector>
#include <curl/curl.h>
#include "singleton.h"

class CUpload;
class CUploadMgr : public CNonCopyable
{
	typedef std::vector<CUpload *> TCUploadVec;

public:
	CUploadMgr();
	virtual ~CUploadMgr();

public:
	int Init(CURLM *pCurlMHandle);
	int StartUpload();
	CURLM *CurlMultiHandle();
	const CURLM *CurlMultiHandle() const;
	void CheckUpload();

private:
	CUpload *m_pUpload;
	TCUploadVec m_uploadVec;
	CURLM *m_pMultiHandle;
};

inline CURLM *CUploadMgr::CurlMultiHandle()
{
	return m_pMultiHandle;
}

inline const CURLM *CUploadMgr::CurlMultiHandle() const
{
	return m_pMultiHandle;
}

#endif /* CHTTPTASKMGR_H_ */
