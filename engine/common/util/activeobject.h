#if !defined (_ACTIVEOBJECT_H_)
#define _ACTIVEOBJECT_H_

/**
 * �����
 * һ���÷�
 * TActiveObject<MyClass, ParamType> activeObject(&MyClass, &MyClass::function, Param);
 * @param MyClassΪĳ�����ָ��
 * @param &MyClass::functionΪĳ�����ĳ����Ա����������������̵߳��õĺ���
 * @param ParamΪ����Զ���Ĳ��������ޣ�ȱʡΪNULL
 * �÷������������
 */
template <typename Container, typename ValueType=Container*>    //���������в�������
class TActiveObject : public CRunnable
{
    public:
        TActiveObject(Container* _Container, void (Container::*_Action)(ValueType), ValueType _Param=NULL):m_pfn(_Action),m_Container(_Container),m_Param(_Param)
        {
            m_pThread = NULL;
        }

        virtual ~TActiveObject()
        {
        	delete m_pThread;
        };

        bool init()
        {
            if ( NULL != m_pThread )
                return true;

            m_pThread = CThreadFactory::createThread(*this);
            
            if ( NULL == m_pThread )
                return false;
            else
                return true;
        }

        void start()
        {
            m_pThread->start();
        };

        void stop()
        {
           m_pThread->stop();
        };

        void setParam(ValueType _Param)
        {
            m_Param = _Param;
        };

        ValueType getParam()
        {
            return m_Param;
        };

    private:
        void run()
        {
            (m_Container->*m_pfn)(m_Param);
        };

        void (Container::*m_pfn)(ValueType); 
        Container* m_Container;
        CThread*         m_pThread;       
        ValueType m_Param;  //������˽�в���
};

#endif

