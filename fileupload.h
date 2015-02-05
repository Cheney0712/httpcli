/*
 * CFileUpload.h
 *
 *  Created on: Jan 22, 2015
 *      Author: szj0306
 */

#ifndef CFILEUPLOAD_H_
#define CFILEUPLOAD_H_

#include "range_queue.h"
#include "httpposttask.h"
#include "httpcli.h"
#include "upload.h"

class CFileUpload : public CUpload
{
public:
	explicit CFileUpload(CURLM *pMultiHandle);
	virtual ~CFileUpload();

public:
	virtual void Init();
	virtual int StartPost();
	virtual void CheckHttpTask();
	virtual range_queue &AllRngQ() { return m_sAllRngQ; }
	virtual const range_queue &AllRngQ() const { return m_sAllRngQ; }
	virtual range_queue &PostingRngQ() { return m_sPostingRngQ; }
	virtual const range_queue &PostingRngQ() const { return m_sPostingRngQ; }
	virtual range_queue &PostedRngQ() { return m_sPostedRngQ; }
	virtual const range_queue &PostedRngQ() const { return m_sPostedRngQ; }

private:
	bool IsUploadOver() const;
	const std::vector<range> RestUpTask() const;

private:
	range_queue m_sAllRngQ;
    range_queue m_sPostingRngQ;
	range_queue m_sPostedRngQ;
	time_t m_nTimePointBeg;
};

#endif /* CFILEUPLOAD_H_ */
