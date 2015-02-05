#include <sstream>
#include "httpparam.h"
#include "httpclidef.h"

THttpParam::THttpParam()
	: m_nBlkSize(POST_BLOCK_SIZE)
	, m_nUpType(UPTYPE_UPBLK)
	, m_strPostFile()
	, m_strFileName()
	, m_strReqUrl()
	, m_strFileId()
	, m_strGcid()
	, m_strUpKey()
	, m_nFileSize(0)
	, m_nTimeOut(UPTIMEOUT)
	, m_nDeadLine(0)
	, m_nThdNum(0)
	, m_nInterval(POST_INTERVAL)
	, m_strMasterIp()
	, m_nMasterPort(0)
	, m_bSilent(false)
	, m_bErrShow(true)
	, m_bVerbose(true)
	, m_rwLock()
{
}

THttpParam::THttpParam(const THttpParam &param)
	: m_nBlkSize(param.m_nBlkSize)
	, m_nUpType(param.m_nUpType)
	, m_strPostFile(param.m_strPostFile)
	, m_strFileName(param.m_strFileName)
	, m_strReqUrl(param.m_strReqUrl)
	, m_strFileId(param.m_strFileId)
	, m_strGcid(param.m_strGcid)
	, m_strUpKey(param.m_strUpKey)
	, m_nFileSize(param.m_nFileSize)
	, m_nTimeOut(param.m_nTimeOut)
	, m_nDeadLine(param.m_nDeadLine)
	, m_nThdNum(param.m_nThdNum)
	, m_nInterval(param.m_nInterval)
	, m_strMasterIp(param.m_strMasterIp)
	, m_nMasterPort(param.m_nMasterPort)
	, m_bSilent(param.m_bSilent)
	, m_bErrShow(param.m_bErrShow)
	, m_bVerbose(param.m_bVerbose)
{
}

THttpParam &THttpParam::operator=(const THttpParam &param)
{
	if (this != &param)
	{
		m_nBlkSize = param.m_nBlkSize;
		m_nUpType = param.m_nUpType;
		m_strPostFile = param.m_strPostFile;
		m_strFileName = param.m_strFileName;
		m_strReqUrl = param.m_strReqUrl;
		m_strFileId = param.m_strFileId;
		m_strGcid = param.m_strGcid;
		m_strUpKey = param.m_strUpKey;
		m_nFileSize = param.m_nFileSize;
	    m_nTimeOut = param.m_nTimeOut;
		m_nDeadLine = param.m_nDeadLine;
		m_nThdNum = param.m_nThdNum;
		m_nInterval = param.m_nInterval;
		m_strMasterIp = param.m_strMasterIp;
		m_nMasterPort = param.m_nMasterPort;
		m_bSilent = param.m_bSilent;
		m_bErrShow = param.m_bErrShow;
		m_bVerbose = param.m_bVerbose;
	}
	return *this;
}

std::string THttpParam::ToString()
{
	TRWLockHelper rwLockHelper(m_rwLock, TRWLockHelper::GETREADLOCK);
	const std::string &strFileId = CToolKit::BiToHex(m_strFileId.c_str(), m_strFileId.size());
	const std::string &strGcid = CToolKit::BiToHex(m_strGcid.c_str(), m_strGcid.size());
	std::ostringstream ostrm;
	ostrm << "blksize: " << m_nBlkSize << std::endl
		  << "uptype: " << (int)m_nUpType << std::endl
		  << "postfile: " << m_strPostFile << std::endl
		  << "fname: " << m_strFileName << std::endl
		  << "requrl: " << m_strReqUrl << std::endl
		  << "fileid: " << strFileId << std::endl
		  << "gcid: " << strGcid << std::endl
		  << "upkey: " << m_strUpKey << std::endl
		  << "filesize: " << m_nFileSize << std::endl
		  << "timeout: " << m_nTimeOut << std::endl
		  << "deadline: " << m_nDeadLine << std::endl
		  << "thdnum: " << m_nThdNum << std::endl
		  << "interval: " << m_nInterval << std::endl
		  << "master: " << m_strMasterIp << ":" << m_nMasterPort << std::endl
		  << std::noboolalpha << "silent: " << std::boolalpha << m_bSilent << std::endl
		  << std::noboolalpha << "errshow: " << std::boolalpha << m_bErrShow << std::endl
		  << std::noboolalpha << "verbose: " << std::boolalpha << m_bVerbose << std::endl;
	return ostrm.str();
}
