#ifndef _BATTLEROM_H_
#define _BATTLEROM_H_

#include "NetEngine.h"
#include "MapData.h"
#include "Player.h"
#include "BattleInfo.h"
#include "IBattleRoomControl.h"

using namespace JyCommon;


class CBattleRoom:public Ref_Object
{
public:
    CBattleRoom();
    ~CBattleRoom();

    void            SetRoomKey( uint32 uiKey ){ m_uiBattleRoomKey = uiKey;}
    uint32          GetRoomKey(){ return m_uiBattleRoomKey;}
    
    void            SetPlayerID(AccountID playerID){m_playerAccountID=playerID;}
    AccountID       GetPlayerID(){return m_playerAccountID;}
public:
    uint32          Init(uint32 battleMapID,uint32 nBattleType,AccountID playerID,uint32 nBattleTime/*,CBattleInfo battleInfo*/);
    uint32          UnInit();

    void            Update();
    uint32          Operate(uint32 unOperType,AccountID playerID,void* nData);

    uint32          EnterBattleRoom(AccountID playerID);        //
    uint32          LeaveBattleRoom(AccountID playerID);        //�뿪ս��

    bool            CheckIsTimeOut();                           //�ж��Ƿ�ʱ
    bool            CanDestroy();                               //�Ƿ��������
public:
    void            SetCanDestroy(bool flag){ m_bCanDestroy = flag;}
private:
    uint32          m_uiBattleRoomKey;                          //ս������KEY
    uint32          m_uiCreateTime;                             //���������ʱ��
    uint32          m_uiBattleTime;                             //ս������ʱ��
    uint32          m_battleMapID;                              //��ǰս����ID
    uint32          m_nBattleType;                              //����

    uint16          m_iStatus;//��ҽ���ս��ǰ��״̬
    //CBattleInfo     m_BattleInfo;                               //ս����Ϣ

    AccountID       m_playerAccountID;                          //���id
    uint32          m_playerOldMapID;                           //���֮ǰ��mapID
    //std::vector<CPlayerPtr>       m_allyAccountIDVec;           //����
    
    IBattleRoomControl*         m_battleControl;              //
    //==================
    bool            m_bCanDestroy;                              //�Ƿ��������
    bool            m_isComplete;                               //�Ƿ��Ѿ����
private:
    void            SetPlayerOldStatus();                          //����Ϊ��ҽ���ս��ǰ��״̬ �� ����
};

typedef Smart_Ptr<CBattleRoom> CBattleRoomPtr;

#endif