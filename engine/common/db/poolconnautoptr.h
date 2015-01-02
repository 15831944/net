#if !defined(_POOLCONNAUTOPTR_H_)
#define _POOLCONNAUTOPTR_H_
#include "../util/autoptr.h"
#include "../thread/thread.h"

/**
 * ���ݿ���������ָ��
 */
template<class T>
class CPoolConnAutoPtr:public CAutoPtr<T>	
{
public :
	//�򵥵Ŀ������캯��ʵ�֣�������ָ�ֵ������ָ�벻�ٱ���
	//ÿ��ֻ��һ������ָ�뱣�档
	CPoolConnAutoPtr(const CPoolConnAutoPtr& Other):CAutoPtr<T>((CAutoPtr<T>&)Other)
	{
		//CAutoPtr::CAutoPtr((CAutoPtr&)Other);
		//m_pPointer = Other.m_pPointer;
		//((CPoolConnAutoPtr&)Other).m_pPointer = NULL;
	}

	/**
	 * ���캯��
	 */
	CPoolConnAutoPtr():CAutoPtr<T>(NULL)
	{
		//m_pPointer = NULL;
	}

	/**
	 * ���캯��
	 */
	CPoolConnAutoPtr(T* Pointer):CAutoPtr<T>(Pointer)
	{		
		//CAutoPtr::m_pPointer = Pointer;
		if ( Pointer != NULL )
			Pointer->m_nOwnThreadId = CThread::getCurrentThreadId();
	}

	CPoolConnAutoPtr& operator = (const CPoolConnAutoPtr& Other)
	{
		//return CAutoPtr::operator =(Other);
		if ( this->m_pPointer != NULL )
			this->m_pPointer->putConnection();
		this->m_pPointer = Other.m_pPointer;
		((CPoolConnAutoPtr&)Other).m_pPointer = NULL;
		return *this;
	}

	/**
	 * ��������
	 */
	virtual ~CPoolConnAutoPtr()
	{
		if ( this->m_pPointer != NULL )
		{
			((T*)this->m_pPointer)->putConnection();
			this->m_pPointer = NULL;
		}
	}

};

#endif
