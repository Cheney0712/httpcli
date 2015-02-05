#ifndef __SINGLETON_H__
#define __SINGLETON_H__

class CNonCopyable
{
protected:
	CNonCopyable() {}
	~CNonCopyable() {}

private:
	CNonCopyable(const CNonCopyable&);
	const CNonCopyable& operator=(const CNonCopyable&);
};

template <typename T>
class CSingleton
{
private:
	struct ObjCreator
	{
		ObjCreator()
		{
			CSingleton<T>::Instance();
		}
		inline void DoNothing() const {}
	};

	static ObjCreator m_creatorObj;

public:
	static T *Instance()
	{
		static T obj;
		m_creatorObj.DoNothing();
		return &obj;
	}
};

template <typename T>
typename CSingleton<T>::ObjCreator CSingleton<T>::m_creatorObj;

#endif // __SINGLETON_H__
