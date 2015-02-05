/*
 * CUpload.h
 *
 *  Created on: Jan 22, 2015
 *      Author: szj0306
 */

#ifndef CUPLOAD_H_
#define CUPLOAD_H_

#include <vector>
#include <curl/curl.h>

class range_queue;
class CHttpTask;
class CUpload
{
public:
	enum EUpRecovery
	{
		UPRECOVERY_INVALID = -1,
		UPRECOVERY_FINISH = 0,
		UPRECOVERY_UNFINISHED = 1
	};

	enum EUploadStatus
	{
		UPLOAD_INVALID = -1,
		UPLOAD_RUNNING = 0,
		UPLOAD_FINISH = 1,
		UPLOAD_FAILED = 2,
		UPLOAD_TIMEOUT = 3,
	};

	typedef std::vector<CHttpTask *> TCHttpTaskVec;

public:
	explicit CUpload(CURLM *pMultiHandle);
	virtual ~CUpload();

public:
	virtual void Init() = 0;
	virtual int StartPost() = 0;
	virtual void CheckHttpTask() = 0;
	virtual range_queue &AllRngQ() = 0;
	virtual const range_queue &AllRngQ() const = 0;
	virtual range_queue &PostingRngQ() = 0;
	virtual const range_queue &PostingRngQ() const = 0;
	virtual range_queue &PostedRngQ() = 0;
	virtual const range_queue &PostedRngQ() const = 0;
	virtual EUpRecovery &Recovery() { return m_eRecovery; }
	virtual const EUpRecovery &Recovery() const { return m_eRecovery; }
	virtual EUploadStatus &UploadStatus() { return m_eUpStatus; }
	virtual const EUploadStatus &UploadStatus() const { return m_eUpStatus; }

protected:
	CURLM *m_pMultiHandle;
	TCHttpTaskVec m_httpTaskVec;
	EUpRecovery m_eRecovery;
	EUploadStatus m_eUpStatus;
};

#endif /* CUPLOAD_H_ */
