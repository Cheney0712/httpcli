#ifndef __HTTP_PARAM_H__
#define __HTTP_PARAM_H__

#include <stdint.h>
#include <string>
#include "utility.h"
#include "threadcond.h"

struct THttpParam
{
public:
	THttpParam();
	THttpParam(const THttpParam &param);
	THttpParam &operator=(const THttpParam &param);
	uint32_t BlkSize();
	uint8_t UpType();
	std::string PostFile();
	std::string FileName();
	std::string ReqUrl();
	std::string FileId();
	std::string Gcid();
	std::string UpKey();
	uint64_t FileSize();
	uint32_t TimeOut();
	uint64_t DeadLine();
	uint16_t ThdNum();
	uint32_t Interval();
	std::string MasterIp();
	uint16_t MasterPort();
	bool Silent();
	bool ErrShow();
	bool Verbose();
	void BlkSize(uint32_t nBlkSize);
	void UpType(uint8_t nUpType);
	void PostFile(const std::string &strPostFile);
	void FileName(const std::string &strFileName);
	void ReqUrl(const std::string &strReqUrl);
	void FileId(const std::string &strFileId);
	void Gcid(const std::string &strGcid);
	void UpKey(const std::string &strUpKey);
	void FileSize(uint64_t nFielSize);
	void TimeOut(uint32_t nTimeOut);
	void DeadLine(uint64_t nDeadLine);
	void ThdNum(uint16_t nThdNum);
	void Interval(uint32_t nInterval);
	void MasterIp(const std::string &strMasterIp);
	void MasterPort(uint16_t nMasterPort);
	void Silent(bool bSilent);
	void ErrShow(bool bErrShow);
	void Verbose(bool bVerbose);
	std::string ToString();

private:
	uint32_t m_nBlkSize;
	uint8_t m_nUpType;
	std::string m_strPostFile;
	std::string m_strFileName;
	std::string m_strReqUrl;
	std::string m_strFileId;
	std::string m_strGcid;
	std::string m_strUpKey;
	uint64_t m_nFileSize;
	uint32_t m_nTimeOut;
	uint64_t m_nDeadLine;
	uint16_t m_nThdNum;
	uint32_t m_nInterval;
	std::string m_strMasterIp;
	uint16_t m_nMasterPort;
	bool m_bSilent;
	bool m_bErrShow;
	bool m_bVerbose;
	CThreadRwLock m_rwLock;
};
inline uint32_t THttpParam::BlkSize()
{
	TRWLockHelper rwLockHelper(m_rwLock, TRWLockHelper::GETREADLOCK);
	return m_nBlkSize;
}
inline uint8_t THttpParam::UpType()
{
	TRWLockHelper rwLockHelper(m_rwLock, TRWLockHelper::GETREADLOCK);
	return m_nUpType;
}
inline std::string THttpParam::PostFile()
{
	TRWLockHelper rwLockHelper(m_rwLock, TRWLockHelper::GETREADLOCK);
	return m_strPostFile;
}
inline std::string THttpParam::FileName()
{
	TRWLockHelper rwLockHelper(m_rwLock, TRWLockHelper::GETREADLOCK);
	return m_strFileName;
}
inline std::string THttpParam::ReqUrl()
{
	TRWLockHelper rwLockHelper(m_rwLock, TRWLockHelper::GETREADLOCK);
	return m_strReqUrl;
}
inline std::string THttpParam::FileId()
{
	TRWLockHelper rwLockHelper(m_rwLock, TRWLockHelper::GETREADLOCK);
	return m_strFileId;
}
inline std::string THttpParam::Gcid()
{
	TRWLockHelper rwLockHelper(m_rwLock, TRWLockHelper::GETREADLOCK);
	return m_strGcid;
}
inline std::string THttpParam::UpKey()
{
	TRWLockHelper rwLockHelper(m_rwLock, TRWLockHelper::GETREADLOCK);
	return m_strUpKey;
}
inline uint64_t THttpParam::FileSize()
{
	TRWLockHelper rwLockHelper(m_rwLock, TRWLockHelper::GETREADLOCK);
	return m_nFileSize;
}
inline uint32_t THttpParam::TimeOut()
{
	TRWLockHelper rwLockHelper(m_rwLock, TRWLockHelper::GETREADLOCK);
	return m_nTimeOut;
}
inline uint64_t THttpParam::DeadLine()
{
	TRWLockHelper rwLockHelper(m_rwLock, TRWLockHelper::GETREADLOCK);
	return m_nDeadLine;
}
inline uint16_t THttpParam::ThdNum()
{
	TRWLockHelper rwLockHelper(m_rwLock, TRWLockHelper::GETREADLOCK);
	return m_nThdNum;
}
inline uint32_t THttpParam::Interval()
{
	TRWLockHelper rwLockHelper(m_rwLock, TRWLockHelper::GETREADLOCK);
	return m_nInterval;
}
inline std::string THttpParam::MasterIp()
{
	TRWLockHelper rwLockHelper(m_rwLock, TRWLockHelper::GETREADLOCK);
	return m_strMasterIp;
}
inline uint16_t THttpParam::MasterPort()
{
	TRWLockHelper rwLockHelper(m_rwLock, TRWLockHelper::GETREADLOCK);
	return m_nMasterPort;
}
inline bool THttpParam::Silent()
{
	TRWLockHelper rwLockHelper(m_rwLock, TRWLockHelper::GETREADLOCK);
	return m_bSilent;
}
inline bool THttpParam::ErrShow()
{
	TRWLockHelper rwLockHelper(m_rwLock, TRWLockHelper::GETREADLOCK);
	return m_bErrShow;
}
inline bool THttpParam::Verbose()
{
	TRWLockHelper rwLockHelper(m_rwLock, TRWLockHelper::GETREADLOCK);
	return m_bVerbose;
}
inline void THttpParam::BlkSize(uint32_t nBlkSize)
{
	TRWLockHelper rwLockHelper(m_rwLock, TRWLockHelper::GETWRITELOCK);
	m_nBlkSize = nBlkSize;
}
inline void THttpParam::UpType(uint8_t nUpType)
{
	TRWLockHelper rwLockHelper(m_rwLock, TRWLockHelper::GETWRITELOCK);
	m_nUpType = nUpType;
}
inline void THttpParam::PostFile(const std::string &strPostFile)
{
	TRWLockHelper rwLockHelper(m_rwLock, TRWLockHelper::GETWRITELOCK);
	m_strPostFile = strPostFile;
}
inline void THttpParam::FileName(const std::string &strFileName)
{
	TRWLockHelper rwLockHelper(m_rwLock, TRWLockHelper::GETWRITELOCK);
	m_strFileName = strFileName;
}
inline void THttpParam::ReqUrl(const std::string &strReqUrl)
{
	TRWLockHelper rwLockHelper(m_rwLock, TRWLockHelper::GETWRITELOCK);
	m_strReqUrl = strReqUrl;
}
inline void THttpParam::FileId(const std::string &strFileId)
{
	TRWLockHelper rwLockHelper(m_rwLock, TRWLockHelper::GETWRITELOCK);
	m_strFileId = strFileId;
}
inline void THttpParam::Gcid(const std::string &strGcid)
{
	TRWLockHelper rwLockHelper(m_rwLock, TRWLockHelper::GETWRITELOCK);
	m_strGcid = strGcid;
}
inline void THttpParam::UpKey(const std::string &strUpKey)
{
	TRWLockHelper rwLockHelper(m_rwLock, TRWLockHelper::GETWRITELOCK);
	m_strUpKey = strUpKey;
}
inline void THttpParam::FileSize(uint64_t nFielSize)
{
	TRWLockHelper rwLockHelper(m_rwLock, TRWLockHelper::GETWRITELOCK);
	m_nFileSize = nFielSize;
}
inline void THttpParam::TimeOut(uint32_t nTimeOut)
{
	TRWLockHelper rwLockHelper(m_rwLock, TRWLockHelper::GETWRITELOCK);
	m_nTimeOut = nTimeOut;
}
inline void THttpParam::DeadLine(uint64_t nDeadLine)
{
	TRWLockHelper rwLockHelper(m_rwLock, TRWLockHelper::GETWRITELOCK);
	m_nDeadLine = nDeadLine;
}
inline void THttpParam::ThdNum(uint16_t nThdNum)
{
	TRWLockHelper rwLockHelper(m_rwLock, TRWLockHelper::GETWRITELOCK);
	m_nThdNum = nThdNum;
}
inline void THttpParam::Interval(uint32_t nInterval)
{
	TRWLockHelper rwLockHelper(m_rwLock, TRWLockHelper::GETWRITELOCK);
	m_nInterval = nInterval;
}
inline void THttpParam::MasterIp(const std::string &strMasterIp)
{
	TRWLockHelper rwLockHelper(m_rwLock, TRWLockHelper::GETWRITELOCK);
	m_strMasterIp = strMasterIp;
}
inline void THttpParam::MasterPort(uint16_t nMasterPort)
{
	TRWLockHelper rwLockHelper(m_rwLock, TRWLockHelper::GETWRITELOCK);
	m_nMasterPort = nMasterPort;
}
inline void THttpParam::Silent(bool bSilent)
{
	TRWLockHelper rwLockHelper(m_rwLock, TRWLockHelper::GETWRITELOCK);
	m_bSilent = bSilent;
}
inline void THttpParam::ErrShow(bool bErrShow)
{
	TRWLockHelper rwLockHelper(m_rwLock, TRWLockHelper::GETWRITELOCK);
	m_bErrShow = bErrShow;
}

inline void THttpParam::Verbose(bool bVerbose)
{
	TRWLockHelper rwLockHelper(m_rwLock, TRWLockHelper::GETWRITELOCK);
	m_bVerbose = bVerbose;
}

#endif // __HTTP_PARAM_H__
