#ifndef _SINGLETON_H_
#define _SINGLETON_H_

#include <util/allocator.h>

/*
������ʵ�֣�������Ҫʵ�ֵ���ģʽ���࣬����Ҫ�̳л�ʹ�ô��ࡣ
*/
template <class T,class Alloc = CSystemAllocator>
class CSingleton
{
private :
	static T* m_pInstance;
protected :
	/**
	 * ���캯��
	 *
	 */
	CSingleton() {}


	/**
	 * ��������
	 *
	 */
	virtual ~CSingleton() {}

	CSingleton& operator = ( const CSingleton& other );
public :
	
	/**
	 * ��ȡ��������
	 * @return T
	 */
	static T* getInstance()
	{
		if ( m_pInstance == NULL )
		{
			CAllocator *pAlloc = Alloc::getInstance();
			//����ֻ�ǵ�����������ڴ��л�ȡ��
			if ( pAlloc == NULL )
			{
				m_pInstance = new T();
			}
			else
			{
				m_pInstance = (T*) pAlloc->allocate(sizeof(T));
				m_pInstance = new(m_pInstance)T();
			}
		}
		return m_pInstance;
	}

	/**
	 * ǿ���ͷŵ�����Դ
	 */
	static void destroy()
	{
		if ( m_pInstance )
		{
			delete m_pInstance;
			m_pInstance = NULL;
		}
	}
};

template<class T,class Alloc>
T* CSingleton<T,Alloc>::m_pInstance = NULL;

#endif
