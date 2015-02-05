#ifndef __UTILITY_H__
#define __UTILITY_H__

#include <time.h>
#include <string>

struct CToolKit
{
	static std::string Secd2Time(time_t lSecd);
	static const std::string HexToBi(const char *pBuf, int nLen);
	static const std::string BiToHex(const char *pBuf, int nLen);
	static void StringTrim(std::string &sSrc);
	static int GetStringByLabel(const std::string &sSrc,
			const std::string &sLabel1, const std::string &sLabel2,
			std::string &sOut, unsigned int nBeginPos = 0, int nIfRetPos = 0,
			bool ifcasesens = true);
};

#endif // __UTILITY_H__
