#ifndef _NORMALBATTLECONTROL_H_
#define _NORMALBATTLECONTROL_H_

#include "NetEngine.h"
#include "Singleton.h"
#include "ObjKeyGenerator.h"
#include <map>

#include "Player.h"
#include "BattleInfo.h"
#include "IBattleRoomControl.h"

using namespace JyCommon;
using namespace NetWork;

class CNormalBattleControl:public IBattleRoomControl
{
private:
    AccountID       m_playerID;
    uint32          m_battleMapID;                              //��ǰս����ID
    uint32          m_nBattleType;                              //����
    //CBattleInfo     m_BattleInfo;

    bool            m_isCompleted;

    //std::vector<CPlayerPtr>       m_allyAccountIDVec;           //����

    typedef std::pair<uint32,uint32>   MONSTERTYPEPAIR;
    std::vector<MONSTERTYPEPAIR>  m_monsterTypeVec;                //��¼������������

public:
    CNormalBattleControl(){}
    virtual ~CNormalBattleControl(){}
public:
    uint32          Init(uint32 battleMapID,uint32 nBattleType,AccountID playerID);//,CBattleInfo battleInfo
    uint32          UnInit();

    void            Update();
    uint32          Operate(uint32 unOperType,AccountID playerID,void* nData);

    uint32          EnterBattleRoom(AccountID playerID);        //
    uint32          LeaveBattleRoom(AccountID playerID);

    bool            IsCompleted() ;                         //�ж��Ƿ����,
    bool            CanDestroy() ;                          //�Ƿ��������
public:
    uint32          OnTimeOut();
    uint32          OnComplete();
};

typedef Smart_Ptr<CNormalBattleControl> CNormalBattleControlPtr;

#endif