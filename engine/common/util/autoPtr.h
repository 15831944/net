#ifndef _AUTOPTR_H_
#define _AUTOPTR_H_

#include "otherException.h"

template<class T>
class CAutoPtr
{
protected :
	T* m_pPointer;

public :
	//�򵥵Ŀ������캯��ʵ�֣�������ָ�ֵ������ָ�벻�ٱ���
	//ÿ��ֻ��һ������ָ�뱣�档
	CAutoPtr(const CAutoPtr& Other)
	{
		m_pPointer = ((CAutoPtr&)Other).release();
	}

	CAutoPtr& operator = (const CAutoPtr& Other)
	{
		if ( m_pPointer != NULL )
			delete m_pPointer;
		m_pPointer = ((CAutoPtr&)Other).release();
		return *this;
	}
	/**
	 * ���캯��
	 */
	CAutoPtr(T* Pointer)
	{
		m_pPointer = Pointer;
	}
	/**
	 * ��������
	 */
	virtual ~CAutoPtr()
	{
		if ( m_pPointer != NULL )
			delete m_pPointer;
	}
	/**
	 * �ͷ��������ָ�롣
	 * @return T* ָ��
	 */
	T* release()
	{
		T* pPointer = m_pPointer;
		m_pPointer = NULL;
		return pPointer;
	}
	/**
	 * ָ���������
	 */
	T* operator->()
	{
		//��ָ���쳣
		if ( NULL == m_pPointer )
			throw CNullPointException("Null Pointer");
		return m_pPointer;
	}

	T* get()
	{
		return m_pPointer;
	}

	const T* get() const
	{
		return m_pPointer;
	}

	/**
	 * ָ���������
	 */
	const T* operator->() const
	{
		//��ָ���쳣
		if ( NULL == m_pPointer )
			throw CNullPointException("Null Pointer");
		return m_pPointer;
	}

	operator void*() const
	{
		return m_pPointer; 
	}

};


#endif