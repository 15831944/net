#ifndef _MAPDATA_H_
#define _MAPDATA_H_


#include "Commdefine.h"
#include "protocol_common.h"
using namespace Protocol;

typedef std::pair<uint64,uint32> ITEMNUMPAIR;
typedef std::pair<uint32,uint32> MONSTERIDNUMPAIR;

class CMapData : public Ref_Object
{
public:
	CMapData(){}
	virtual ~CMapData(){}

    bool        IsBattle()
    {
        if (m_mapStyle == MapType_Normal)
        {
            return false;
        }
        return true;
    }
public:
	uint32                      m_mapID; //��ͼID
    uint16                      m_mapMode;//��ͼģʽ
    uint32                      m_mapModeParam;//ģʽ����
	uint32                      m_mapStyle;//��ͼ����
	uint32                      m_mapLvl;//��ͼ�ȼ�
    std::vector<ITEMNUMPAIR>    m_consumeItemVec;
	std::vector<ITEMNUMPAIR>    m_resRewardVec;//��Դ����
    uint64                      m_dropItemID;
	uint32                      m_mapLimit;//�����������
	POSITION                    m_mapBornPos;//������
    uint8                       m_revivalStyle;//��������
    int16                       m_revivalTimes;//�������
    POSITION                    m_revivalPosition;//�����
    std::vector<MONSTERIDNUMPAIR> m_monsterLimit;//����ͬʱ���ڵ���������
};

typedef Smart_Ptr<CMapData> CMapDataPtr;
#endif
