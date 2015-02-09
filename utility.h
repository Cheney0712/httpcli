#ifndef __UTILITY_H__
#define __UTILITY_H__

#include <time.h>
#include <string>

class CToolKit
{
public:
	static std::string Secd2Time(time_t lSecd);
	static const std::string HexToBi(const char *pBuf, int nLen);
	static const std::string BiToHex(const char *pBuf, int nLen);
	static void StringTrim(std::string &sSrc);
	static int GetStringByLabel(const std::string &sSrc,
			const std::string &sLabel1, const std::string &sLabel2,
			std::string &sOut, unsigned int nBeginPos = 0, int nIfRetPos = 0,
			bool ifcasesens = true);
	template<typename PtrType>
	static void ReleasePtr(PtrType *ptr);
};

template<typename PtrType>
inline void CToolKit::ReleasePtr(PtrType *ptr)
{
	if (NULL != ptr)
	{
		delete ptr;
		ptr = NULL;
	}
}

#endif // __UTILITY_H__
