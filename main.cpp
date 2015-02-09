/**
 * ============================================================================
 * @file	main.cpp
 *
 * @brief
 * @details
 *
 * @version 1.
 * @date	2014-06-29 15:42:13
 *
 * @author  shizhijie, shizhijie@baofeng.com
 * ============================================================================
 */

#define _XOPEN_SOURCE
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <getopt.h>
#include <sys/sysinfo.h>
#include <sys/stat.h>
#include "httpcli.h"

using namespace std;

static const char szHelp[] = { "help" };
static const char szBlkSize[] = { "block" };
static const char szFilePath[] = { "file" };
static const char szTimeOut[] = { "timeout" };
static const char szThdNum[] = { "thdnum" };
static const char szInterval[] = { "interval" };
static const char szSilent[] = { "silent" };
static const char szVerbose[] = { "verbose" };
static const char szErrShow[] = { "errshow" };
static const char szVersion[] = { "version" };

static const char* const szOptStr = "hb:f:m:t:l:sveV";
static const struct option szLongOpts[] = {
	{ szHelp,		no_argument,	   NULL, 'h' },
	{ szBlkSize,	optional_argument, NULL, 'b' },
	{ szFilePath,	required_argument, NULL, 'f' },
	{ szTimeOut,	optional_argument, NULL, 'm' },
	{ szThdNum,		optional_argument, NULL, 't' },
	{ szInterval,	optional_argument, NULL, 'l' },
	{ szSilent,	    no_argument,       NULL, 's' },
	{ szVerbose,	no_argument,       NULL, 'v' },
	{ szErrShow,	no_argument,       NULL, 'e' },
	{ szVersion,	no_argument,       NULL, 'V' },
	{ NULL,			no_argument,	   NULL,  0  },
};

static int ParseCmdLine(int argc, char *argv[], THttpParam &param);
static void Usage(const THttpParam &param);
static void ShowVersion(const THttpParam &param);

int main (int argc, char** argv)
{
	THttpParam param;

	if (argc <= 1)
	{
		Usage(param);
		return (0);
	}

	srand(time(NULL));

	if (ParseCmdLine(argc, argv, param) != 0)
	{
		return (-1);
	}

	CHttpCli* pHttpCli = CSingleton<CHttpCli>::Instance();
	if (0 != pHttpCli->Init(param))
	{
		return (-1);
	}
	if (0 != pHttpCli->Start())
	{
		return (-1);
	}
	pHttpCli->CleanUp();

	return (0);
}

static int ParseCmdLine(int argc, char *argv[], THttpParam &param)
{
	int c = 0, nIdx = 0;
	struct tm t;
	memset(&t, 0, sizeof(struct tm));
	while ((c = getopt_long(argc, argv, szOptStr, szLongOpts, &nIdx)) != -1)
	{
		switch (c)
		{
		case 0:
			if (szLongOpts[nIdx].flag != NULL)
			{
				break;
			}
			if (strncmp(szHelp, szLongOpts[nIdx].name, strlen(szHelp)) == 0)
			{
				Usage(param);
				return (0);
			}
			else if (strncmp(szBlkSize, szLongOpts[nIdx].name, strlen(szBlkSize)) == 0)
			{
				param.BlkSize((uint32_t)atoi(optarg));
			}
			else if (strncmp(szFilePath, szLongOpts[nIdx].name, strlen(szFilePath)) == 0)
			{
				param.PostFile(optarg);
			}
			else if (strncmp(szTimeOut, szLongOpts[nIdx].name, strlen(szTimeOut)) == 0)
			{
				param.TimeOut(atoi(optarg));
			}
			else if (strncmp(szThdNum, szLongOpts[nIdx].name, strlen(szThdNum)) == 0)
			{
				param.ThdNum((uint16_t)atoi(optarg));
			}
			else if (strncmp(szInterval, szLongOpts[nIdx].name, strlen(szInterval)) == 0)
			{
				param.Interval(atoi(optarg));
			}
			else if (strncmp(szSilent, szLongOpts[nIdx].name, strlen(szSilent)) == 0)
			{
				param.Silent(true);
			}
			else if (strncmp(szVerbose, szLongOpts[nIdx].name, strlen(szVerbose)) == 0)
			{
				param.Verbose(true);
			}
			else if (strncmp(szErrShow, szLongOpts[nIdx].name, strlen(szErrShow)) == 0)
			{
				param.ErrShow(true);
			}
			else if (strncmp(szVersion, szLongOpts[nIdx].name, strlen(szVersion)) == 0)
			{
				ShowVersion(param);
				return (0);
			}
			break;
		case 'h':
			Usage(param);
			return (0);
		case 'b':
			param.BlkSize((uint32_t)atoi(optarg));
			break;
		case 'f':
			param.PostFile(optarg);
			break;
		case 'm':
			param.TimeOut(atoi(optarg));
			break;
		case 't':
			param.ThdNum((uint16_t)atoi(optarg));
			break;
		case 'l':
			param.Interval(atoi(optarg));
			break;
		case 's':
			param.Silent(true);
			break;
		case 'v':
			param.Verbose(true);
			break;
		case 'e':
			param.ErrShow(true);
			break;
		case 'V':
			ShowVersion(param);
			return (0);
		default:
			Usage(param);
			return (0);
		}
	}

	if (param.Silent())
	{
		param.Verbose(false);
	}

	HTTPCLI_LOG(param, LOGTYPE, "start httpcli ...");

	// request url
	if (!(optind < argc))
	{
		HTTPCLI_LOG(param, ERRORTYPE, "no request url found!!!");
		return (-1);
	}
	param.ReqUrl(argv[optind]);

	// filename
	std::string strPostFile(param.PostFile());
	std::string::size_type nPos = strPostFile.rfind("/");
	if (nPos != std::string::npos)
	{
		param.FileName(strPostFile.substr(nPos + 1));
	}
	else
	{
		param.FileName(param.PostFile());
	}

	// filesize
	struct stat s;
	if (stat(param.PostFile().c_str(), &s) < 0)
	{
		HTTPCLI_LOG(param, ERRORTYPE, "stat: %s", strerror(errno));
		return (-1);
	}
	param.FileSize(s.st_size);

//    // fileid
//	std::string strFileId(param.FileId());
//	if ((int)strFileId.size() != FILEID_LEN_HEX)
//	{
//		HTTPCLI_LOG(param, ERRORTYPE, "invalid length(%d) of fileid: %s",
//					(int)strFileId.size(), strFileId.c_str());
//		return -1;
//	}
//	param.FileId(CToolKit::HexToBi(strFileId.c_str(), strFileId.size()));
//	if ((int)param.FileId().size() != FILEID_LEN_BIN)
//	{
//		HTTPCLI_LOG(param, ERRORTYPE, "invalid format of fileid: %s", strFileId.c_str());
//		return -1;
//	}
//
//    // gcid
//	std::string strGcid(param.Gcid());
//	if ((int)strGcid.size() != GCID_LEN_HEX)
//	{
//		HTTPCLI_LOG(param, ERRORTYPE, "invalid length(%d) of gcid: %s",
//					(int)strGcid.size(), strGcid.c_str());
//		return -1;
//	}
//	param.Gcid(CToolKit::HexToBi(strGcid.c_str(), strGcid.size()));
//	if ((int)param.Gcid().size() != GCID_LEN_BIN)
//	{
//		HTTPCLI_LOG(param, ERRORTYPE, "invalid format of gcid: %s", strGcid.c_str());
//		return -1;
//	}

	// uptype & fsize & blksize check
	if (param.UpType() == UPTYPE_FORM && param.FileSize() > param.BlkSize())
	{
		param.BlkSize(param.FileSize());
		HTTPCLI_LOG(param, LOGTYPE,
				"set blksize(%u) to fsize(%lu) when uptype == UPTYPE_FORM.",
				param.BlkSize(), param.FileSize());
	}

	// thdnum
	if (param.ThdNum() == 0)
	{
		param.ThdNum(sysconf(_SC_NPROCESSORS_ONLN));
	}
	HTTPCLI_LOG(param, LOGTYPE, "httpcli param: \r\n%s", param.ToString().c_str());

	return (0);
}

static void Usage(const THttpParam &param)
{
	static const char* const szHelp = {
		"Usage: httpcli [options...]\n"
		" -h --help     display this help message\n"
		" -b --block    the size of a block to POST, default 4M\n"
		" -f --file     a file with full path to post\n"
		" -m --timeout  timeout of the upload. default 30 min\n"
		" -t --thdnum   number of threads, automatic detection, usually cpu cores\n"
		" -l --interval interval of the POST thread, default 3s\n"
		" -s --silent   silent mode. Don't output anything\n"
		" -v --verbose  make the operation more talkative\n"
		" -e --errshow  show error. with -s make httpcli show errors when they occur\n"
		" -V --version  show version number and quit"
	};
	HTTPCLI_LOG(const_cast<THttpParam &>(param), LOGTYPE, "%s", szHelp);
}

static void ShowVersion(const THttpParam &param)
{
	HTTPCLI_LOG(const_cast<THttpParam &>(param), LOGTYPE, "httpcli v%s", HTTPCLI_VERSION);
}
