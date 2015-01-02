#if !defined(_DATETIME_H_)
#define _DATETIME_H_

#ifdef WINDOWS
#include <time.h>
#else
#include <sys/time.h>
#endif

inline bool checkTotolSec( long nSecs )
{
    time_t time;
    time = nSecs;
    if( localtime( &time ) == NULL )
        return false;
    else
        return true;
}

class CCException;
/**
 * usage :
 * ��Ҫinclude <userexception.h>
*/
class CDateTimeException : public CCException
{
public :
	CDateTimeException(const char* pMsg);
	~CDateTimeException() throw();
};

class CInterval;

//Ĭ�ϵ�ʱ���ʽ
static char DEFAULT_DATETIME_FORMAT[50]="YYYY-MM-DD hh:mm:ss";

const char YMD_DATETIME_FORMAT[50] = "YYYY-MM-DD";

//Ĭ�ϵ�ʱ������ʽ
static char DEFAULT_INTERVAL_FORMAT[50]="DD hh:mm:ss.nnn";

// ʱ�䴦���װ��
// ��ȷ����
// ����ʱ�����ã���Ŀǰû��ʵ��
class CDateTime
{
private :
	//ʱ������������ʱ��Ϊ0��1��ʾ��һ��Сʱ��ʱ��
	//��ǰʱ�������õ�ʱ����
	short m_nTimeZone;
	//��ʾʱ�������õ�ʱ��
	short m_nShowZone;
	//��ǰʱ��ĺ�������1970-1-1�յ���ǰ���ܺ�����
	uint64 m_nTimeSpan;
	//����ʱ��
	time_t m_Time;
	//������߻�ȡ��ǰʱ���Ч�ʣ�����ֵ��Ϊ0����CDatetime��Ĭ�Ϲ��캯������ֱ��ͨ��ϵͳapi��ȡ��ǰʱ�䣬����ֱ��ʹ�ô�ֵ
	static time_t g_Time;
	static uint64 g_nTimeSpan;

private :
	/**
	 * ��ȡ��ʱ����ת���ɺ�����
	 */
	long getTimeZoneMills() const;
	/**
	 * ���ݸ�ʽ��д�뵽������
	 */
	bool asYMD(struct tm* pTime,unsigned long Mills,const char Ch,char* Buf,int Index,int Count,int BufSize) const;

public :
	// ���չ涨�ĸ�ʽת��Ϊ�ַ�����
	// format��
	// Y������
	// M������
	// D������
	// h����ʱ
	// m������
	// s������
	// n��������
	std::string asString(const char* Format = DEFAULT_DATETIME_FORMAT) const;

	//Ĭ�Ϲ��캯��������Ϊ��ǰʱ��
	/**
	 * @param bReal �Ƿ�ȡʵʱ�Ļ���ʱ�䣬һ�㲻��������Ϊtrue
	 */
	CDateTime(bool bReal = false);

	// ����ʱ��Ĺ��캯����
	// nYear��nMonth��nDay��nHour��nMinute��nSeconds��nMillSeconds�����ñ����ǺϷ������ڣ���������ں���Ĳ������׳��쳣
	// nYear�������1970��
	CDateTime(int nYear, int nMonth, int nDay, int nHour, int nMinute, int nSecond,int nMillSeconds,short nTimeZone = 0);

	// �������캯��
	CDateTime(const CDateTime& Other);

	//�����ܺ���������ʼ����ǰʱ��
	CDateTime(uint64 nMillSeconds);
	
	// ��ȡ���
	int getYear() const;

	// ��ȡ���������·�
	int getMonth() const;

	// ��ȡ���¼��������
	int getDay() const;

	// ��ȡ��������Сʱ��
	int getHour() const;

	// ��ȡ�������ķ���
	int getMinute() const;

	// ��ȡ������������
	int getSecond() const;

	// ��ȡ���ڼ�
	//0~6��0���������죬1��������һ...
	int getDayOfWeek() const;

	//��ȡ�������ĺ�����
	int getMillSecond() const;

	//��ȡ������������
	int getYday() const;
	
	/**
	 * ���õĳ�ʼ������
	 */
	bool init(int nYear, int nMonth, int nDay, int nHour, int nMinute, int nSecond,int nMillSeconds,short nTimeZone = 0);
    /**
     *@��������Ƿ�Ƿ�
     */
    bool check() const;

	CInterval operator -(const CDateTime& Other) const;

	CDateTime operator -(const CInterval& Interval) const;

	CDateTime operator +(const CInterval& Interval) const;

	const CDateTime& operator -=(const CInterval& Interval);

	const CDateTime& operator +=(const CInterval& Interval);
	
	const CDateTime& operator = (uint64 MillSeconds);

	bool operator==(const CDateTime& Other) const;

	bool operator !=(const CDateTime& Other) const;

	bool operator <=(const CDateTime& Other) const;

	bool operator <(const CDateTime& Other) const;

	bool operator >(const CDateTime& Other) const;

	bool operator >=(const CDateTime& Other) const;

	/** �ֽ����ڸ�ʽ��Valu�ַ�����ת��Ϊʱ�������
	 * Format���Զ����ʽΪ��
	 * Y������ �����Բ���2λ��4λ�ı�ʾ����
	 *           ����2λ�ı�ʾ������Զ��Ϊ��2000�Ժ����ݣ���79���ʾ2079������1979��
	 *            ���Խ��龡��ȫ������4λ��ʾ��
	 * M������
	 * D������
	 * h����ʱ
	 * m������
	 * s������
	 * n��������
	 * �����ʽ���Ǳ�׼ �����ڸ�ʽ�����׳��쳣
	*/
	void parse(const char* Value, const char* Format = DEFAULT_DATETIME_FORMAT);
	/**
	 * ������ʾ��ʱ��
	 * ��������Ĭ��Ϊ�����¼��ʱ��һ��
	 */
	void setShowTimeZone(short TimeZone);
	/**
	 * ȫ�ֺ���������g_TimeΪ��ǰʱ�䣬����Ҫʹ��ȫ�ֺ���ʱʱ����Ϊ�������뱻��ʱ���á�
	 */
	static void tickCount();
	
	/**
	 * ��ȡ�ܵĺ�����
	 */
	uint64 getTotalMills() const;
	/**
	 * ��ȡ�ܵ�����
	 */
	int32 getTotalSecs() const;
	/**
	 * ��̬��������ȡ��ǰ�ĺ�����
	 * @param bReal ��ȡ��ʵ������
	 */
	static uint64 current(bool bReal = false);
};

// ʱ�����࣬���ƴ���ʱ��Ĳ���
// ��ȷ����
class CInterval
{
private :
	friend class CDateTime;
	//ʱ��������λ���롣64λ��¼
	int64 m_nTimeSpan;

private :
	/**
	 * ���ݸ�ʽ��д�뵽������
	 */
	bool asYMD(const char Ch,char* Buf,int Index,int Count,int BufSize) const;

public:
	// ���캯����
	// ����0����ʱ������ǰ��С��0����ʱ��������
	// nDays���������ϵ��ܼ��
	// nHours����Сʱ�ϵ��ܼ��
	// nMinutes���������ϵ��ܼ��
	// nSeconds�������ϵ��ܼ��
	// nMillSeconds���������ϵļ��
	// �����������Ϊ��(10,-24,0,0)
	// ʵ����Ϊʱ����9�졣
	CInterval(int nDays, int nHours, int nMinutes, int nSeconds,int nMillSeconds);
	//�������캯��
	CInterval(const CInterval& Other);
	//�����ܺ��������캯��
	CInterval(const int64 MillSeconds);

	// ��ȡ�ܵļ������
	int getDays() const;

	// ��ȡ�ܵļ������
	int getTotalSeconds() const;

	// ��ȡ�ܵķ�����
	int getTotalMinutes() const;

	// ��ȡ�ܵ�Сʱ��
	int getTotalHours() const;
	// ��ȡ�ܵĺ�����
	int64 getTotalMills() const;

	// ��ȡʱ������������������
	// ����ʱ����Ϊ2��10Сʱ1��12�룬�򷵻�12
	int getSecondsOfDay() const;

	// ��ȡʱ�����������ķ�������
	// ����ʱ����Ϊ2��10Сʱ1��12�룬�򷵻�1
	int getMinutesOfDay() const;

	// ��ȡʱ������������µ���Сʱ����
	// ����ʱ����Ϊ2��10Сʱ1��12�룬�򷵻�2
	int getHoursOfDay() const;

	// ��ȡʱ������������µ��ܺ�������
	// ����ʱ����Ϊ2��10Сʱ1��12��200���룬�򷵻�200
	int getMillsOfDay() const;

	// ����-����������������ʱ���������
	CInterval operator -(const CInterval& Other) const;

	// ���أ�����������������ʱ����֮��
	CInterval operator +(const CInterval& Other) const;

	// �����Լ���������
	const CInterval& operator -=(const CInterval& Other);

	// �����ԼӲ�����
	const CInterval& operator +=(const CInterval& Other);

	// ����==������
	// �Ƚ�����ʱ�����Ƿ�һ��
	bool operator ==(const CInterval& Other) const;

	// ����!=������
	// �Ƚ�����ʱ�����Ĵ�С
	bool operator !=(const CInterval& Other) const;

	// ����<������
	// �Ƚ�����ʱ�����Ĵ�С
	bool operator <(const CInterval& Other) const;

	// ����<=������
	// �Ƚ�����ʱ�����Ĵ�С
	bool operator <=(const CInterval& Other) const;

	// ����>������
	// �Ƚ�����ʱ�����Ĵ�С
	bool operator >(const CInterval& Other) const;

	// ����>=������
	// �Ƚ�����ʱ�����Ĵ�С
	bool operator >=(const CInterval& Other) const;

	/**
	 * �ֽ����ڸ�ʽ���ַ�����ת��Ϊʱ��������
	 * �����ʽ���Ǳ�׼ �����ڸ�ʽ�����׳��쳣���磺Сʱ������[0,23]������
	 */
	void parse(const char* Value,const char* Format = DEFAULT_INTERVAL_FORMAT);
	/**
	 * ����ָ���ĸ�ʽ�����ַ�����ʽ
	 */
	std::string asString(const char* Format = DEFAULT_INTERVAL_FORMAT) const;

};
#endif
