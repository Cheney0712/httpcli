/**
 * ============================================================================
 * @file	threadcond.h
 *
 * @brief   
 * @details 
 *
 * @version 1.0  
 * @date	2014-12-20 22:37:14
 *
 * @author  shizhijie, shizhijie@baofeng.com
 * ============================================================================
 */

#ifndef __BFP2P_SERVER_THREADCOND_H__
#define __BFP2P_SERVER_THREADCOND_H__

#include <pthread.h>

class CThdMutex
{
public:
	CThdMutex();
	~CThdMutex();
	void Lock();
	void UnLock();
private:
	pthread_mutex_t  m_thdMutex;
};

inline CThdMutex::CThdMutex()
{
	pthread_mutex_init(&m_thdMutex, NULL);
}

inline CThdMutex::~CThdMutex()
{
	pthread_mutex_destroy(&m_thdMutex);
}

inline void CThdMutex::Lock()
{
	pthread_mutex_lock(&m_thdMutex);
}

inline void CThdMutex::UnLock()
{
	pthread_mutex_unlock(&m_thdMutex);
}

class CThdMutexLock
{
public:
	CThdMutexLock(CThdMutex *pMutex);
	virtual ~CThdMutexLock();
private:
	CThdMutex *m_pMutex;
};

inline CThdMutexLock::CThdMutexLock(CThdMutex *pMutex)
	: m_pMutex(pMutex)
{
	m_pMutex->Lock();
}

inline CThdMutexLock::~CThdMutexLock()
{
	m_pMutex->UnLock();
}

class CThreadRwLock
{
public:
	CThreadRwLock();
	virtual ~CThreadRwLock();
	int GetReadLock();
	int GetWriteLock();
	int ReleaseLock();
private:
	pthread_rwlock_t m_rwlock;
};

struct TRWLockHelper
{
public:
	enum ELockType { GETREADLOCK = 0, GETWRITELOCK = 1 };

public:
	TRWLockHelper(CThreadRwLock &rwlock, ELockType type);
	virtual ~TRWLockHelper();

private:
	void Lock();
	void Release();

private:
	CThreadRwLock &m_ThdRwLock;
	ELockType m_nELockType;
};

inline TRWLockHelper::TRWLockHelper(CThreadRwLock &rwlock, ELockType type)
	: m_ThdRwLock(rwlock), m_nELockType(type)
{
	Lock();
}

inline TRWLockHelper::~TRWLockHelper()
{
	Release();
}

inline void TRWLockHelper::Lock()
{
	if (m_nELockType == GETREADLOCK)
	{
		m_ThdRwLock.GetReadLock();
	}
	else if (m_nELockType == GETWRITELOCK)
	{
		m_ThdRwLock.GetWriteLock();
	}
}

inline void TRWLockHelper::Release()
{
	m_ThdRwLock.ReleaseLock();
}

inline CThreadRwLock::CThreadRwLock()
{
	pthread_rwlock_init(&m_rwlock, NULL);
}

inline CThreadRwLock::~CThreadRwLock()
{
	pthread_rwlock_destroy(&m_rwlock);
}

inline int CThreadRwLock::GetReadLock()
{
	pthread_rwlock_rdlock(&m_rwlock);
	return 0;
}

inline int CThreadRwLock::GetWriteLock()
{
	pthread_rwlock_wrlock(&m_rwlock);
	return 0;
}

inline int CThreadRwLock::ReleaseLock()
{
	pthread_rwlock_unlock(&m_rwlock);
	return 0;
}

class CThreadCond
{
	class CThdMutexLock
	{
	public:
		CThdMutexLock(pthread_mutex_t *pThdMutex);
		~CThdMutexLock(void);

	private:
		pthread_mutex_t *m_pThdMutex;
	};
public:
	CThreadCond(void);
	virtual ~CThreadCond(void);
	virtual void ThdCondSignal();
	virtual void ThdCondWait();

protected:
	pthread_mutex_t m_thdMutex;
	pthread_cond_t m_thdCond;
	bool m_bExit;
};

inline CThreadCond::CThreadCond() : m_bExit(false)
{
	pthread_mutex_init(&m_thdMutex, NULL);
	pthread_cond_init(&m_thdCond, NULL);
}

inline CThreadCond::~CThreadCond()
{
	pthread_mutex_destroy(&m_thdMutex);
	pthread_cond_signal(&m_thdCond);
	pthread_cond_destroy(&m_thdCond);
}

inline void CThreadCond::ThdCondSignal()
{
	CThdMutexLock thdLock(&m_thdMutex);
	m_bExit = true;
	pthread_cond_signal(&m_thdCond);
}

inline void CThreadCond::ThdCondWait()
{
	CThdMutexLock thdLock(&m_thdMutex);
	while (!m_bExit)
	{
		pthread_cond_wait(&m_thdCond, &m_thdMutex);
	}
}

inline CThreadCond::CThdMutexLock::CThdMutexLock(pthread_mutex_t *pThdMutex)
	: m_pThdMutex(pThdMutex)
{
	pthread_mutex_lock(m_pThdMutex);
}

inline CThreadCond::CThdMutexLock::~CThdMutexLock()
{
	pthread_mutex_unlock(m_pThdMutex);
}

#endif // __BFP2P_SERVER_THREADCOND_H__
