#ifndef _BATTLEMANAGER_H_
#define _BATTLEMANAGER_H_
#include "NetEngine.h"
#include "Singleton.h"
#include "ObjKeyGenerator.h"
#include <map>

#include "BattleInfo.h"
#include "BattleRoom.h"
#include "Player.h"

using namespace JyCommon;
using namespace NetWork;

typedef map< uint32, CBattleRoomPtr> BATTLEMAP;


class CBattleInstanceManager:public Ref_Object
{
private:
    map<uint32,CBattleRoomPtr>           m_BattleMap;//key-battle map
    map<AccountID,uint32>                m_AccountKeyMap;//���id ����ʵ��key ��map��

    uint32                               m_BattleMapID;//ս��id
    uint32                               m_BattleLastTime;//ս�����ʱ�� ��λ��
    
    //CBattleInfo                          m_BattleInfo;
    CObjKeyGenerator    m_BattleKeyGenerator;       //room������
public:
    CBattleInstanceManager();
    virtual ~CBattleInstanceManager();

    inline uint32       GetNewKey(){ m_BattleKeyGenerator.GetKey();}//���һ���µ�ս������Key
    inline void         RestoreKey( uint32 uiKey ){ m_BattleKeyGenerator.RestoreKey(uiKey);}//�黹һ��ս������Key
public:
    uint32              Init(uint32 battleMapID,uint32 uLastTime/*,CBattleInfo battleInfo*/);
    uint32              UnInit();

    void                Update();
public:
    uint32              ProcessBattleReq(AccountID playerID,uint32  nReqType,uint32 nBattleID,uint32 nBattleType,uint32 nInstanceID,void* pData,uint32& uKey );
    CBattleRoomPtr      FindPlayerBattleRoom(AccountID playerID);
    uint32              FindPlayerBattleUkey(AccountID playerID);

    uint32              LeaveBattleRoom(uint32 battleMapID,AccountID playerID,uint32 nKey);//�뿪ս��

    uint32              GetBattleInstanceCount(){return m_BattleMap.size();}
private:
    bool                IsKeyValid(uint32 uKey){ return uKey >=1 && uKey <= 100000;}
    CBattleRoomPtr      CreateBattleInstance(uint32 nBattleID);

    uint32              Open(uint32 battleMapID,uint32 nBattleType,AccountID playerID,uint32& uKey); //����

    uint32              Operate(uint32 unOperType,AccountID playerID,void* nData,uint32 uKey);//uKey �����

    uint32              EnterBattleRoom(uint32 battleMapID,AccountID playerID,uint32 nKey);//
};

typedef     Smart_Ptr<CBattleInstanceManager>  CBattleInstanceManagerPtr;




class CBattleManager:public Singleton<CBattleManager>
{
private:
    std::map<uint32,CBattleInstanceManagerPtr>          m_BattleInstanceMgrMap;//
    //CSVLoader                                           m_Loader;
    //std::map<uint32, CBattleInfo>                       m_BattleInfoMap;//����ID����
public:
    CBattleManager();
    ~CBattleManager();

public:
    uint32              Init();
    uint32              UnInit();

    CBattleInstanceManagerPtr           getBattleInstanceManager(uint32 unBattleMapID);
public:
    void                Update();
private:                                       
    //uint32  LoadFile(const char* szPath);
};


#endif