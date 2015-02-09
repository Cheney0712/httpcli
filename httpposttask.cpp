/*
 * ================================================================================
 *
 *	   Filename:  syndatatask.cpp
 *
 *	Description:  
 *
 *		Version:  1.0
 *		Created:  2012年04月17日 15时41分25秒
 *	   Revision:  none
 *	   Compiler:  gcc
 *
 *		 Author:  wangtiefeng (), wangtf418@163.com
 *		Company:  
 *
 * ================================================================================
 */

#include <errno.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/md5.h>
#include "httpposttask.h"
#include "httpclidef.h"
#include "uriparser.h"
#include "fileupload.h"
#include "httpcli.h"
#include "utility.h"
#include "range.h"

CHttpPostTask::CHttpPostTask(CURLM *pMultiHandle, CUpload *pUpload)
	: CHttpTask(pMultiHandle, pUpload)
	, m_pFormPost(NULL)
	, m_pLastPtr(NULL)
	, m_pHdrList(NULL)
	, m_pMultiPartHdrList(NULL)
	, m_strMD5()
	, m_strHttpHdr()
	, m_strHttpBody()
	, m_pPostData(NULL)
	, m_postRng()
{
}

CHttpPostTask::~CHttpPostTask()
{
	if (m_pFormPost != NULL)
	{
		curl_formfree(m_pFormPost);
		m_pFormPost = NULL;
	}
	if (m_pHdrList != NULL)
	{
		curl_slist_free_all(m_pHdrList);
		m_pHdrList = NULL;
	}
	if (m_pMultiPartHdrList != NULL)
	{
		curl_slist_free_all(m_pMultiPartHdrList);
		m_pMultiPartHdrList = NULL;
	}
	if (m_pPostData != NULL)
	{
		delete[] m_pPostData;
		m_pPostData = NULL;
	}
}

void CHttpPostTask::Init(const range &postRng)
{
	THttpParam &param = CSingleton<CHttpCli>::Instance()->HttpParam();
	m_postRng = postRng;
	curl_easy_setopt(m_pCurl, CURLOPT_POST, 1L);
//	curl_easy_setopt(m_pCurl, CURLOPT_TIMEOUT, param.TimeOut());
    /* the DEBUGFUNCTION has no effect until we enable VERBOSE */
	curl_easy_setopt(m_pCurl, CURLOPT_VERBOSE, 1L);
	curl_easy_setopt(m_pCurl, CURLOPT_DEBUGFUNCTION, &CHttpPostTask::Debug);
	curl_easy_setopt(m_pCurl, CURLOPT_HEADERDATA, this);
	curl_easy_setopt(m_pCurl, CURLOPT_HEADERFUNCTION, &CHttpPostTask::HttpHeadProc);
	curl_easy_setopt(m_pCurl, CURLOPT_WRITEDATA, this);
	curl_easy_setopt(m_pCurl, CURLOPT_WRITEFUNCTION, &CHttpPostTask::WriteData);
	curl_easy_setopt(m_pCurl, CURLOPT_URL, param.ReqUrl().c_str());
	curl_easy_setopt(m_pCurl, CURLOPT_NOPROGRESS, true);
	/* tell libcurl to follow redirection */
	curl_easy_setopt(m_pCurl, CURLOPT_FOLLOWLOCATION, 1L);
//	/* make connection get closed at once after use */
//	curl_easy_setopt(m_pCurl, CURLOPT_FORBID_REUSE, 1L);
//	/* Set to 1 to make the next transfer use a new (fresh) connection by force instead of trying to re-use an existing one. */
//	curl_easy_setopt(m_pCurl, CURLOPT_FRESH_CONNECT, 1L);
}

int CHttpPostTask::PostEntireFile()
{
#if 0
	POST /upload/?key=iN7NgwM31j4-BZacMj HTTP/1.1
	Host: 192.168.200.15
	Content-Length: 500000
	Content-Range: bytes 0-499999/2000000
	Content-MD5: OGFkZDUwNGVhNGY3N2MxMDIwZmQ4NTBmY2IyTY==
	Content-Type: application/octet-stream

	DATA1
#endif
	THttpParam &param = CSingleton<CHttpCli>::Instance()->HttpParam();

	if (m_strMD5.empty() && CalcFileMd5(param.PostFile(), m_strMD5) != 0)
	{
		HTTPCLI_LOG(param, ERRORTYPE, "calculate file md5 failed: %s",
					param.PostFile().c_str());
		m_eHttpUpStatus = HTTP_UPLOAD_FAILED;
		return -1;
	}

	m_pHdrList = curl_slist_append(m_pHdrList, "Expect: ");
	std::ostringstream ostrm;
	{	// Content-Range
		ostrm << HTTP_HEAD_FIELD_CONTENT_RANGE << "bytes 0-" << param.FileSize() - 1 << "/" << param.FileSize();
		m_pHdrList = curl_slist_append(m_pHdrList, ostrm.str().c_str());
	}
	ostrm.clear(); ostrm.str("");
	{	// Content-MD5
		ostrm << HTTP_HEAD_FIELD_CONTENT_MD5 << m_strMD5;
		m_pHdrList = curl_slist_append(m_pHdrList, ostrm.str().c_str());
	}
	ostrm.clear(); ostrm.str("");
	{	// Content-Type
		ostrm << HTTP_HEAD_FIELD_CONTENT_TYPE << HTTP_MEDIA_TYPE_MULTIPART_FORM_DATA;
		m_pHdrList = curl_slist_append(m_pHdrList, ostrm.str().c_str());
	}

	// multi-part Content-Type
	ostrm.clear(); ostrm.str("");
	{	// Content-Type
		ostrm << HTTP_HEAD_FIELD_CONTENT_TYPE << HTTP_MEDIA_TYPE_APPLICATION_OSTRM;
		m_pMultiPartHdrList = curl_slist_append(m_pMultiPartHdrList, ostrm.str().c_str());
	}
	curl_formadd(&m_pFormPost, &m_pLastPtr, CURLFORM_COPYNAME, param.FileName().c_str(),
			CURLFORM_PTRCONTENTS, m_pPostData, CURLFORM_CONTENTSLENGTH, m_postRng.length(),
			CURLFORM_CONTENTHEADER, m_pMultiPartHdrList, CURLFORM_END);

	curl_easy_setopt(m_pCurl, CURLOPT_HTTPHEADER, m_pHdrList);
	curl_easy_setopt(m_pCurl, CURLOPT_HTTPPOST, m_pFormPost);
	curl_multi_add_handle(m_pMultiHandle, m_pCurl);

	m_eHttpUpStatus = HTTP_UPLOAD_RUNNING;

	return 0;
}

int CHttpPostTask::UpRecovery()
{
#if 0
	POST /upload/?key=iN7NgwM31j4-BZacMj HTTP/1.1
	Host: 192.168.200.15
	Content-Length: 0
	Content-Range: bytes */fsize
	Connection: close
#endif

	THttpParam &param = CSingleton<CHttpCli>::Instance()->HttpParam();
	m_pHdrList = curl_slist_append(m_pHdrList, "Expect: ");
	std::ostringstream ostrm;
	{	// Content-Range
		ostrm << HTTP_HEAD_FIELD_CONTENT_RANGE << "bytes */" << param.FileSize();
		m_pHdrList = curl_slist_append(m_pHdrList, ostrm.str().c_str());
	}
	ostrm.clear(); ostrm.str("");
	{	// Content-Type
		ostrm << HTTP_HEAD_FIELD_CONTENT_TYPE << HTTP_MEDIA_TYPE_APPLICATION_OSTRM;
		m_pHdrList = curl_slist_append(m_pHdrList, ostrm.str().c_str());
	}
	curl_easy_setopt(m_pCurl, CURLOPT_HTTPHEADER, m_pHdrList);
	curl_multi_add_handle(m_pMultiHandle, m_pCurl);

	m_eHttpUpStatus = HTTP_UPLOAD_RUNNING;

	return 0;
}

int CHttpPostTask::PostBlock()
{
#if 0
	POST /upload/?key=iN7NgwM31j4-BZacMj HTTP/1.1
	Host: 192.168.200.15
	Content-Length: 500000
	Content-Range: bytes 0-499999/2000000
	Content-MD5: OGFkZDUwNGVhNGY3N2MxMDIwZmQ4NTBmY2IyTY==
	Content-Type: application/octet-stream

	DATA1
#endif
	THttpParam &param = CSingleton<CHttpCli>::Instance()->HttpParam();
	if (m_strMD5.empty() && CalcFileMd5(param.PostFile(), m_strMD5) != 0)
	{
		HTTPCLI_LOG(param, ERRORTYPE, "calculate file md5 failed: %s",
					param.PostFile().c_str());
		m_eHttpUpStatus = HTTP_UPLOAD_FAILED;
		return -1;
	}
	m_pHdrList = curl_slist_append(m_pHdrList, "Expect: ");
	std::ostringstream ostrm;
	{	// Content-Range
		ostrm << HTTP_HEAD_FIELD_CONTENT_RANGE << "bytes " << m_postRng.pos()
				<< "-" << m_postRng.pos() + m_postRng.length() - 1 << "/"
				<< param.FileSize();
		m_pHdrList = curl_slist_append(m_pHdrList, ostrm.str().c_str());
	}
	ostrm.clear(); ostrm.str("");
	{	// Content-MD5
		ostrm << HTTP_HEAD_FIELD_CONTENT_MD5 << m_strMD5;
		m_pHdrList = curl_slist_append(m_pHdrList, ostrm.str().c_str());
	}
	ostrm.clear(); ostrm.str("");
	{	// Content-Type
		ostrm << HTTP_HEAD_FIELD_CONTENT_TYPE << HTTP_MEDIA_TYPE_MULTIPART_FORM_DATA;
		m_pHdrList = curl_slist_append(m_pHdrList, ostrm.str().c_str());
	}

	// multi-part Content-Type
	ostrm.clear(); ostrm.str("");
	{	// Content-Type
		ostrm << HTTP_HEAD_FIELD_CONTENT_TYPE << HTTP_MEDIA_TYPE_APPLICATION_OSTRM;
		m_pMultiPartHdrList = curl_slist_append(m_pMultiPartHdrList, ostrm.str().c_str());
	}
	curl_formadd(&m_pFormPost, &m_pLastPtr, CURLFORM_COPYNAME, param.FileName().c_str(),
			CURLFORM_PTRCONTENTS, m_pPostData, CURLFORM_CONTENTSLENGTH, m_postRng.length(),
			CURLFORM_CONTENTHEADER, m_pMultiPartHdrList, CURLFORM_END);

	curl_easy_setopt(m_pCurl, CURLOPT_HTTPHEADER, m_pHdrList);
	curl_easy_setopt(m_pCurl, CURLOPT_HTTPPOST, m_pFormPost);
	curl_multi_add_handle(m_pMultiHandle, m_pCurl);

	m_eHttpUpStatus = HTTP_UPLOAD_RUNNING;

	return 0;
}

int CHttpPostTask::CalcFileMd5(const std::string &strUpFilePath, std::string &strFileMD5)
{
	THttpParam &param = CSingleton<CHttpCli>::Instance()->HttpParam();

	int nRet = 0;
	FILE *pUpFile = fopen(strUpFilePath.c_str(), "rb");
	if (NULL == pUpFile)
	{
		nRet = -1;
		m_eHttpUpStatus = HTTP_UPLOAD_FAILED;
		HTTPCLI_LOG(param, ERRORTYPE, "fopen failed, file: %s", strUpFilePath.c_str());
		goto exit;
	}
	if (CalcFileMd5(pUpFile, strFileMD5) != 0)
	{
		nRet = -1;
		m_eHttpUpStatus = HTTP_UPLOAD_FAILED;
		HTTPCLI_LOG(param, ERRORTYPE, "calculate file md5 failed: %s", strUpFilePath.c_str());
		goto exit;
	}

exit:
	fclose(pUpFile); pUpFile = NULL;
	return nRet;
}

int CHttpPostTask::CalcFileMd5(FILE *pFile, std::string &strFileMD5)
{
	THttpParam &param = CSingleton<CHttpCli>::Instance()->HttpParam();

	const int nBufSize = 1024 * MD5_DIGEST_LENGTH;
	int fd = fileno(pFile);
	if (-1 == lseek(fd, m_postRng.pos(), SEEK_SET))
	{
		HTTPCLI_LOG(param, ERRORTYPE, "lseek offset(%lu) error: %d(%s)",
					m_postRng.pos(), errno, strerror(errno));
		m_eHttpUpStatus = HTTP_UPLOAD_FAILED;
		return -1;
	}

	MD5_CTX md5Engine;
	uint8_t md[MD5_DIGEST_LENGTH] = { 0 };
	MD5_Init(&md5Engine);

	m_pPostData = new char[m_postRng.length()];
	memset(m_pPostData, 0xFF, m_postRng.length());

	int64_t nCalcSize = 0, nLeftSize = 0, nNeedReadSize = 0;
	for (;;)
	{
		nLeftSize = m_postRng.length() - nCalcSize;
		nNeedReadSize = nLeftSize > nBufSize ? nBufSize : nLeftSize;
		uint8_t *pBuf = (uint8_t *)(m_pPostData + nCalcSize);
		int nReadSize = read(fd, pBuf, nNeedReadSize);
		if (nReadSize <= 0)
		{
			if (nCalcSize != (int64_t)m_postRng.length())
			{
				HTTPCLI_LOG(param, ERRORTYPE, "fsize != postRng.length()");
				m_eHttpUpStatus = HTTP_UPLOAD_FAILED;
				return -1;
			}
			break;
		}
		MD5_Update(&md5Engine, pBuf, (size_t)nReadSize);
		if ((nCalcSize += nReadSize) >= (int64_t)m_postRng.length())
		{
			break;
		}
	}
	MD5_Final(&(md[0]), &md5Engine);
	strFileMD5.assign(CToolKit::BiToHex((const char *)md, MD5_DIGEST_LENGTH));
	HTTPCLI_LOG(param, LOGTYPE, "file md5: %s", strFileMD5.c_str());

	return 0;
}

size_t CHttpPostTask::HttpHeadProc(void *pData, size_t size, size_t nMemBlk, void *pUserArg/* this */)
{
#if 0
	HTTP/1.1 308 Resume Incomplete
	Content-Length: 0
	Range: 0-9999,20000-50000,1900000-
	Cache-Control: no-cache
	Server: mxupsrv 1.0
	Connection: close
	Date: Mon Jul 21 14:36:08 2014
#endif

	CHttpCli *pHttpCli = CSingleton<CHttpCli>::Instance();
	THttpParam &param = pHttpCli->HttpParam();

	CHttpPostTask *pHttpPostTask = static_cast<CHttpPostTask *>(pUserArg);
	const char *pHttpHdr = static_cast<const char *>(pData);
	size_t nHdrSize = size * nMemBlk;
	pHttpPostTask->HttpHdr().append(static_cast<const char *>(pData), nHdrSize);
//	HTTPCLI_LOG(param, LOGTYPE, "%s", pHttpHdr);

	long nHttpRespCode = 0;
	curl_easy_getinfo(pHttpPostTask->Curl(), CURLINFO_RESPONSE_CODE, &nHttpRespCode);
	if (nHttpRespCode == HTTP_OK)
	{
		pHttpPostTask->Upload()->PostedRngQ() = pHttpPostTask->Upload()->AllRngQ();
	}
	else if (nHttpRespCode == HTTP_PARTIAL_CONTENT)
	{
		pHttpPostTask->Upload()->PostedRngQ() += pHttpPostTask->PostRng();
	}
	else if (nHttpRespCode == HTTP_RESUME_INCOMPLETE)
	{
		std::string::size_type nPreCommaPos = 0, nCurCommaPos = 0;
		do
		{
			std::string strRngs, strRng;
			if (CToolKit::GetStringByLabel(pHttpHdr, "Range: ", "\r\n", strRngs) != 0 || strRngs.empty())
			{
				break;
			}
			CToolKit::StringTrim(strRngs);
			nCurCommaPos = strRngs.find(",", nPreCommaPos);
			if (nCurCommaPos == std::string::npos)
			{
				// the last range
				strRng = strRngs.substr(nPreCommaPos);
			}
			else
			{
				strRng = strRngs.substr(nPreCommaPos, nCurCommaPos - nPreCommaPos);
				nPreCommaPos = nCurCommaPos + 1;
			}

			std::string::size_type nLinePos = strRng.find("-");
			if (nLinePos == std::string::npos)
			{
				break;
			}
			std::string strRngBeg, strRngEnd;
			strRngBeg = strRng.substr(0, nLinePos);
			strRngEnd = strRng.substr(nLinePos + 1);
			uint64_t nRngBeg = 0, nRngEnd = 0;
			nRngBeg = strtoull(strRngBeg.c_str(), NULL, 10);
			nRngEnd = strtoull(strRngEnd.c_str(), NULL, 10);
			pHttpPostTask->Upload()->PostedRngQ() += range(nRngBeg, nRngEnd - nRngBeg + 1);
		} while (nCurCommaPos != std::string::npos);
		pHttpPostTask->Upload()->Recovery() = CFileUpload::UPRECOVERY_FINISH;
	}
	else if ((nHttpRespCode / HTTP_BAD_REQUEST) == 1)
	{
		HTTPCLI_LOG(param, ERRORTYPE, "POST request failed");
	}
	else if ((nHttpRespCode / HTTP_SPECIAL_RESPONSE) == 1)
	{
		std::string strTmp;
		if (0 == CToolKit::GetStringByLabel(
				pHttpHdr, "Location:", "\r\n", strTmp, 0, 0, false))
		{
			param.ReqUrl(strTmp);
			return nHdrSize;
		}
	}
	pHttpPostTask->Upload()->PostingRngQ() -= pHttpPostTask->PostRng();

	return nHdrSize;
}

size_t CHttpPostTask::WriteData(void *pData, size_t size, size_t nMemBlk, void *pUserArg/* this */)
{
	CHttpCli *pHttpCli = CSingleton<CHttpCli>::Instance();
	THttpParam &param = pHttpCli->HttpParam();
	CHttpPostTask *pHttpPostTask = static_cast<CHttpPostTask *>(pUserArg);

	size_t nSize = size * nMemBlk;
	pHttpPostTask->HttpBody().append(static_cast<const char *>(pData), nSize);

	long nHttpRespCode = 0;
	curl_easy_getinfo(pHttpPostTask->Curl(), CURLINFO_RESPONSE_CODE, &nHttpRespCode);
	pHttpPostTask->Upload()->UploadStatus() =
			(nHttpRespCode == HTTP_OK) ?
					CUpload::UPLOAD_FINISH : CUpload::UPLOAD_FAILED;

	double nUpSize = 0.0f;
	curl_easy_getinfo(pHttpPostTask->Curl(), CURLINFO_SIZE_UPLOAD, &nUpSize);
	if (nUpSize >= pHttpPostTask->PostRng().length())
	{
		HTTPCLI_LOG(param, LOGTYPE, "upload has completely: \r\n%s%s",
				pHttpPostTask->HttpHdr().c_str(),
				pHttpPostTask->HttpBody().c_str());
		pHttpPostTask->HttpUpStatus(HTTP_UPLOAD_FINISH);
	}
	return nSize;
}

int CHttpPostTask::Debug(CURL *pCurl, curl_infotype type, char *data, size_t size, void *pUserArg)
{
	const char *pText = NULL;
	(void) pCurl; /* prevent compiler warning */

	CHttpCli *pHttpCli = CSingleton<CHttpCli>::Instance();
	THttpParam &param = pHttpCli->HttpParam();
	switch (type)
	{
	case CURLINFO_TEXT:
		HTTPCLI_LOG(param, LOGTYPE, "== Info: %s", data);
	default: /* in case a new one is introduced to shock us */
		return 0;
	case CURLINFO_HEADER_OUT:
		pText = "=> Send header";
		break;
	case CURLINFO_DATA_OUT:
		pText = "=> Send data";
		break;
	case CURLINFO_SSL_DATA_OUT:
		pText = "=> Send SSL data";
		break;
	case CURLINFO_HEADER_IN:
		pText = "<= Recv header";
		break;
	case CURLINFO_DATA_IN:
		pText = "<= Recv data";
		break;
	case CURLINFO_SSL_DATA_IN:
		pText = "<= Recv SSL data";
		break;
	}
//	Dump(pText, (unsigned char *)data, size);
	return 0;
}

void CHttpPostTask::Dump(const char *pText, unsigned char *pData, size_t size)
{
	size_t i, c;
	unsigned int width = 0x10;

	fprintf(stderr, "%s, %10.10ld bytes (0x%8.8lx)\n", pText, (long)size, (long)size);
	for (i = 0; i < size; i += width)
	{
		fprintf(stderr, "%4.4lx: ", (long)i);

		/* show hex to the left */
		for (c = 0; c < width; c++)
		{
			if (i + c < size)
			{
				fprintf(stderr, "%02x ", pData[i + c]);
			}
			else
			{
				fputs("   ", stderr);
			}
		}

		/* show data on the right */
		for(c = 0; (c < width) && (i+c < size); c++)
		{
			fputc(((pData[i + c] >= 0x20) && (pData[i + c] < 0x80)) ? pData[i + c] : '.', stderr);
		}
		fputc('n', stderr); /* newline */
	}
}
