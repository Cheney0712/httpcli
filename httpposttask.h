/*
 * =====================================================================================
 *
 *	   Filename:  syndatatask.h
 *
 *	Description:  
 *
 *		Version:  1.0
 *		Created:  2012年04月17日 15时39分30秒
 *	   Revision:  none
 *	   Compiler:  gcc
 *
 *		 Author:  wangtiefeng (), wangtf418@163.com
 *		Company:  
 *
 * =====================================================================================
 */
#ifndef _SYNDATATASK_H_DB138DDA_2C0B_4F9A_90E0_3A490CD5E22F_
#define _SYNDATATASK_H_DB138DDA_2C0B_4F9A_90E0_3A490CD5E22F_

#include <curl/curl.h>
#include "range.h"
#include "range_queue.h"
#include "threadcond.h"
#include "httptask.h"

class CUpload;
class CHttpPostTask : public CHttpTask
{
public:
	CHttpPostTask(CURLM *pMultiHandle, CUpload *pUpload);
	virtual ~CHttpPostTask();

public:
	virtual void Init(const range &postRng);
	virtual int UpRecovery();
	virtual int PostEntireFile();
	virtual int PostBlock();

public:
	const range &PostRng() const { return m_postRng; }
	std::string &HttpBody() { return m_strHttpBody; }
	const std::string &HttpBody() const { return m_strHttpBody; }

private:
	int CalcFileMd5(const std::string &strUpFilePath, std::string &strFileMD5);
	int CalcFileMd5(FILE *pFile, std::string &strFileMD5);

private:
	static size_t HttpHeadProc(void *pData, size_t size, size_t nMemBlk, void *pUserArg/* this */);
	static size_t WriteData(void *pData, size_t size, size_t nMemBlk, void *pUserArg/* this */);
	static int Debug(CURL *pCurl, curl_infotype type, char *data, size_t size, void *pUserArg);
	static void Dump(const char *pText, unsigned char *pData, size_t size);

protected:
	curl_httppost *m_pFormPost;
	curl_httppost *m_pLastPtr;
	curl_slist *m_pHdrList;
	curl_slist *m_pMultiPartHdrList;
	std::string m_strMD5;
	std::string m_strHttpBody;
	char *m_pPostData;
	range m_postRng;
};

#endif //_SYNDATATASK_H_DB138DDA_2C0B_4F9A_90E0_3A490CD5E22F_
