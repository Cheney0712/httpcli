#include "upload.h"
#include "httptask.h"
#include "utility.h"

CUpload::CUpload(CURLM *pMultiHandle)
	: m_pMultiHandle(pMultiHandle)
	, m_httpTaskVec()
	, m_eRecovery(UPRECOVERY_INVALID)
	, m_eUpStatus(UPLOAD_INVALID)
{
	// TODO Auto-generated constructor stub
}

CUpload::~CUpload()
{
	// TODO Auto-generated destructor stub
	m_pMultiHandle = NULL;
	for (TCHttpTaskVec::iterator iter = m_httpTaskVec.begin();
			iter != m_httpTaskVec.end(); ++iter)
	{
		CToolKit::ReleasePtr<CHttpTask>(*iter);
	}
}
