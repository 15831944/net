/*
��������Ԫ����
*/
#if !defined(_METADATA_H_)
#define _METADATA_H_
namespace mysql
{
//	class CMetaData
//	{
//	private :
//		MYSQL_BIND m_Param;
//	public :
//		/**
//		 *
//		 */
//		CMetaData();
//		/**
//		 *
//		 */
//		~CMetaData();
//		/**
//		 * 
//		 */
//		MYSQL_BIND* MYSQL_BIND1();
//		/**
//		 * ��ջ���
//		 */
//		void clear();
//	};

	class CResultData
	{
	public :
		//������������
		EDATATYPE m_nDataType;
		//�ֶ���
		string m_ColumnName;
	};
}
#endif
