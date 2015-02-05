#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "utility.h"

std::string CToolKit::Secd2Time(time_t lSecd)
{
        char psDateTime[50];
        int iYear, iMonth, iDay, iHour, iMin, iSec;
        struct tm tmstu;
        localtime_r(&lSecd, &tmstu);

        iYear = tmstu.tm_year+1900;
        iMonth = tmstu.tm_mon+1;
        iDay = tmstu.tm_mday;
        iHour = tmstu.tm_hour;
        iMin = tmstu.tm_min;
        iSec = tmstu.tm_sec;

		snprintf(psDateTime, sizeof(psDateTime),
				"%04d-%02d-%02d %02d:%02d:%02d",
				iYear, iMonth, iDay, iHour, iMin, iSec);

		return std::string(psDateTime);
}

const std::string CToolKit::HexToBi(const char *pBuf, int nLen)
{
	std::string sOut;
    if (nLen < 0 || (nLen%2) !=0)
    {
        return sOut;
    }
    int nTmp = nLen/2;
    for (int ii=0; ii < nTmp; ii++)
    {
        char pTmp[3] = {0};
        strncpy(pTmp, pBuf + 2*ii, 2);
        char pChar = (char)strtoul(pTmp, 0, 16);
        sOut.append(1, pChar);
    }
    return sOut;
}

const std::string CToolKit::BiToHex(const char *pBuf, int nLen)
{
	std::string sOut;
    if (nLen < 0)
    {
        return sOut;
    }
    for (int ii=0; ii < nLen; ii++)
    {
        char pTmp[3] = {0};
        snprintf(pTmp, sizeof(pTmp), "%02X", (unsigned char)pBuf[ii]);
        sOut.append(pTmp, 2);
    }
    return sOut;
}

void CToolKit::StringTrim(std::string &sSrc)
{
	int i = 0;
	while ((sSrc[i] == ' ' || sSrc[i] == '\t' || sSrc[i] == '\r'
			|| sSrc[i] == '\n') && i < (int) sSrc.length())
	{
		i++;
	}
	int nBeginPos = i;
	i = sSrc.length() - 1;
	while ((sSrc[i] == ' ' || sSrc[i] == '\t' || sSrc[i] == '\r'
			|| sSrc[i] == '\n') && i >= 0)
	{
		i--;
	}
	int nEnd = i;
	sSrc = sSrc.substr(nBeginPos, nEnd - nBeginPos + 1);
}

int CToolKit::GetStringByLabel(const std::string &sSrc,
		const std::string &sLabel1, const std::string &sLabel2,
		std::string &sOut, unsigned int nBeginPos, int nIfRetPos,
		bool ifcasesens)
{
	if (nBeginPos >= sSrc.length())
	{
		return -1;
	}
	char *pTmp = (char*) sSrc.c_str() + nBeginPos;
	char *pTmp0;
	char *pTmp1;
	if (sLabel1 == "")
	{
		pTmp0 = pTmp;
	}
	else
	{
		pTmp0 = (ifcasesens) ?
				strstr(pTmp, sLabel1.c_str()) :
				strcasestr(pTmp, sLabel1.c_str());
		if (pTmp0 == NULL)
		{
			return -1;
		}
	}
	int ret = pTmp0 - sSrc.c_str() + sLabel1.length();
	if (sLabel2 != "")
	{
		pTmp1 = (ifcasesens) ?
				strstr(pTmp0 + sLabel1.length(), sLabel2.c_str()) :
				strcasestr(pTmp0 + sLabel1.length(), sLabel2.c_str());
		if (pTmp1 == NULL)
		{
			return -1;
		}
		ret = pTmp1 + sLabel2.length() - sSrc.c_str();
		sOut = std::string(pTmp0 + sLabel1.length(),
				pTmp1 - pTmp0 - sLabel1.length());
	}
	else
	{
		sOut = std::string(pTmp0 + sLabel1.length());
		ret = sSrc.length();
	}
	if (nIfRetPos == 0)
	{
		ret = 0;
	}
	return ret;
}
