#include "NormalBattleControl.h"



//=======

uint32     CNormalBattleControl::Init(uint32 battleMapID,uint32 nBattleType,AccountID playerID)//,CBattleInfo battleInfo
{
    m_playerID = playerID;
    // m_BattleInfo = battleInfo;
    m_battleMapID = battleMapID;
    m_monsterTypeVec.clear();
    m_nBattleType = nBattleType;
    m_isCompleted = false;
    return 0;
}

uint32     CNormalBattleControl::UnInit()
{
    //ս��ʱ�䵽�� ��������ս�� ��Ҫ����������ҵĵ�ͼ
    m_monsterTypeVec.clear();
    return 0;
}

void       CNormalBattleControl::Update()
{
    //do

}

uint32     CNormalBattleControl::Operate(uint32 unOperType,AccountID playerID,void* nData)
{
    switch(unOperType)
    {
    case EnumBattleOper_KillMonster:
        {
            //���¹���״̬
            uint32  nMonsterID = (uint32)nData;

        }
        break;
    case EnumBattleOper_CompleteReport:
        {
            //check 
            m_isCompleted = true;
        }
        break;
    default:
        break;
    }
    return 0;
}

uint32     CNormalBattleControl::EnterBattleRoom(AccountID playerID)        //
{
    //do
    return 0;
}

uint32     CNormalBattleControl::LeaveBattleRoom(AccountID playerID)
{
    //do
    return 0;
}

bool       CNormalBattleControl::IsCompleted()                         //�ж��Ƿ����
{
    return m_isCompleted;
}

bool       CNormalBattleControl::CanDestroy()                          //�Ƿ��������
{
    return false;
}

uint32     CNormalBattleControl::OnTimeOut()
{
    //ս��ʱ�䵽�˴���
    return 0;
}


uint32     CNormalBattleControl::OnComplete()
{
    //ս����ɴ���
    return 0;
}