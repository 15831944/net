#include "BattleManager.h"
#include "MapDataLoader.h"
#include "PlayerManager.h"

CBattleInstanceManager::CBattleInstanceManager()
{
     m_BattleKeyGenerator.Create(1,100000);
     m_BattleMap.clear();
     m_AccountKeyMap.clear();
}

CBattleInstanceManager::~CBattleInstanceManager()
{
    m_BattleMap.clear();
    m_AccountKeyMap.clear();
}

uint32      CBattleInstanceManager::Init(uint32 battleMapID,uint32 nBattleTime/*,CBattleInfo battleInfo*/)
{
    m_BattleMapID = battleMapID;
    //m_BattleInfo = battleInfo;
    m_BattleLastTime = nBattleTime;
    return 0;
}

uint32      CBattleInstanceManager::UnInit()
{
    m_BattleMap.clear();
    m_AccountKeyMap.clear();
    return 0;
}

void        CBattleInstanceManager::Update()
{
    //��������ʵ�����ø���ʵ����update�ӿ�
    map<uint32,CBattleRoomPtr>::iterator iter = m_BattleMap.begin();
    for (;iter!=m_BattleMap.end();)
    {
        if (iter->second != NULL)
        {
            (iter->second)->Update();
            if(iter->second->CanDestroy())
            {
                iter->second->UnInit();
                uint32 nKey = iter->first;
                m_BattleMap.erase(iter++);
                for (map<AccountID,uint32>::iterator it = m_AccountKeyMap.begin();it!=m_AccountKeyMap.end();it++ )
                {
                    if (it->second == nKey)
                    {
                        m_AccountKeyMap.erase(it);
                        break;
                    }
                }
                if ( IsKeyValid(nKey))
                {
                    m_BattleKeyGenerator.RestoreKey(nKey);
                }
            }
            else
                iter++;
        }
        else
            iter++;
    }
}

uint32     CBattleInstanceManager::Open(uint32 battleMapID,uint32 nBattleType,AccountID playerID,uint32& uKey)
{
    CBattleRoomPtr  pBattleRoomTemp = CreateBattleInstance(battleMapID);
    uint32 nRst = pBattleRoomTemp->Init(battleMapID,nBattleType,playerID,m_BattleLastTime);
    if (nRst!=0)
    {
        return nRst;
    }
    uKey = m_BattleKeyGenerator.GetKey();
    if (uKey == 0)
    {
        LOG_ERROR(" CBattleInstanceManager::Open: NOT ENOUGH KEY");
        //����һ��������
        return EnumBattleResult_NotEnoughKey;
    }
    m_BattleMap.insert(std::make_pair(uKey,pBattleRoomTemp));
    m_AccountKeyMap.insert(std::make_pair(playerID,uKey)) ;
    CPlayerManager::Instance()->GetPlayerByAccount(playerID)->SetPlayerMapID(battleMapID); //������ҵĵ�ͼid
    return 0;
}

uint32     CBattleInstanceManager::Operate(uint32 unOperType,AccountID playerID,void* pData,uint32 uKey)
{
    map<uint32,CBattleRoomPtr>::iterator iter = m_BattleMap.find(uKey);
    if (iter == m_BattleMap.end())
    {
        return EnumBattleResult_NoThisKey; //û����������
    }
    return iter->second->Operate(unOperType,playerID,pData);
}

CBattleRoomPtr      CBattleInstanceManager::CreateBattleInstance(uint32 nBattleID)
{
    //��������ʵ��
    return CBattleRoomPtr(new CBattleRoom()); 
}

uint32      CBattleInstanceManager::EnterBattleRoom(uint32 battleMapID,AccountID playerID,uint32 nKey)//
{
    //��ҽ��븱��
    map<uint32,CBattleRoomPtr>::iterator iter = m_BattleMap.find(nKey);
    if (iter == m_BattleMap.end())
    {
        return EnumBattleResult_NoThisKey; //û����������
    }
    uint32 nRst = iter->second->EnterBattleRoom(playerID);
    if (nRst != 0) //
    {
        LOG_ERROR(" CBattleInstanceManager::EnterBattleRoom: error nRst = %u",nRst);
    }
    else
    {
       //������ҵĵ�ǰ״̬��ս����
        CPlayerPtr player = CPlayerManager::Instance()->GetPlayerByAccount(playerID);
        player->SetPlayerStatus(PlayerStatus_Battle);
    }

    return nRst;
}

uint32      CBattleInstanceManager::LeaveBattleRoom(uint32 battleMapID,AccountID playerID,uint32 nKey)//�뿪ս��
{
    //����뿪����
    m_AccountKeyMap.erase(playerID);
    if ( IsKeyValid(nKey))
    {
        m_BattleKeyGenerator.RestoreKey(nKey);
    }
    map<uint32,CBattleRoomPtr>::iterator iter = m_BattleMap.find(nKey);
    if (iter == m_BattleMap.end())
    {
        return EnumBattleResult_NoThisKey; //û����������
    }
    uint32 nRst = iter->second->LeaveBattleRoom(playerID);
    //�뿪ս�����ٷ���
    iter->second->UnInit();
    m_BattleMap.erase(iter);
    //����������ҵĵ�ͼΪ ���� ��ս��ʵ���л������ҽ���ս��ʱ�ĵ�ͼ ��ս��ʵ��������֮ǰ�ĵ�ͼ
    //CPlayerManager::Instance()->GetPlayerByAccount(playerID)->SetPlayerMapID();
    return nRst;
}


uint32    CBattleInstanceManager::ProcessBattleReq(AccountID playerID,uint32  nReqType,uint32 nBattleID,uint32 nBattleType,uint32 nInstanceID,void* pData,uint32& uKey )
{
    uint32  nRst = 0;
    //uint32  uKey = 0;
    switch(nReqType)
    {
    case EnumBattleOper_Enter:
        {
            nRst = this->Open(nBattleID,nBattleType,playerID,uKey);
            if (nRst == 0)
            {   
                nRst = this->EnterBattleRoom(nBattleID,playerID,uKey);
                if (nRst!=0)
                {
                    LOG_INFO("CBattleInstanceManager::ProcessBattleReq :EnterBattleRoom return %d",nRst);
                }
            }
            else
                 LOG_INFO("CBattleInstanceManager::ProcessBattleReq : Open battle error %d",nRst);
        }
        break;
    case EnumBattleOper_Exit:
        {
            nRst = this->LeaveBattleRoom(nBattleID,playerID,nInstanceID);
            if (nRst!=0)
            {
                LOG_INFO("CBattleInstanceManager::ProcessBattleReq : LeaveBattleRoom return %d",nRst);
            }
        }
    case EnumBattleOper_Oper:
    case EnumBattleOper_KillMonster:     //ս��ɱ�ֻ㱨
    case EnumBattleOper_CompleteReport:     //��ɱ���
        {
            nRst = this->Operate(nReqType,playerID,pData,nInstanceID);
            if (nRst!=0)
            {
                LOG_INFO("CBattleInstanceManager::ProcessBattleReq : Operate return %d ",nRst);
            }
        }
        break;
    default:
        LOG_INFO("NO CASE");
        break;
    }
    return nRst;
}

CBattleRoomPtr     CBattleInstanceManager::FindPlayerBattleRoom(AccountID playerID)
{
    uint32  uKey = 0;
    map<AccountID,uint32>::iterator iter = m_AccountKeyMap.find(playerID);
    if (iter == m_AccountKeyMap.end())
    {
        LOG_ERROR("CBattleInstanceManager::FindPlayerBattleRoom: error not find playid %u house",playerID);
        return CBattleRoomPtr();
    }
    uKey = iter->second;
    map<uint32,CBattleRoomPtr>::iterator it = m_BattleMap.find(uKey);
    if (it == m_BattleMap.end() || it->second == NULL )
    {
        LOG_ERROR("CBattleInstanceManager::ProcessBattleReq: NOT FIND BattleRoom ");
        m_AccountKeyMap.erase(iter);
        return CBattleRoomPtr();
    }
    return it->second;
}

uint32           CBattleInstanceManager::FindPlayerBattleUkey(AccountID playerID)
{
    uint32  uKey = 0;
    map<AccountID,uint32>::iterator iter = m_AccountKeyMap.find(playerID);
    if (iter != m_AccountKeyMap.end())
    {
        uKey = iter->second;
    }
    return uKey;
}

//========CBattleManager=====

CBattleManager::CBattleManager()
{
    m_BattleInstanceMgrMap.clear();
    //m_BattleInfoMap.clear();
}

CBattleManager::~CBattleManager()
{
    
}

void CBattleManager::Update()
{
    std::map<uint32,CBattleInstanceManagerPtr>::iterator iter = m_BattleInstanceMgrMap.begin();
    for (;iter!=m_BattleInstanceMgrMap.end();)
    {
        if (iter->second != NULL)
        {
            (iter->second)->Update();
            if(iter->second->GetBattleInstanceCount() == 0)
                m_BattleInstanceMgrMap.erase(iter++);
            else
                iter++;
        }
        else
            iter++;
    }
}

uint32     CBattleManager::Init()
{
    //LoadFile(szPath);
    //���ݸ���ID ��������ʵ��������
    /*for( std::map<uint32, CBattleInfo>::iterator iter=m_BattleInfoMap.begin();iter!=m_BattleInfoMap.end();iter++)
    {
        uint32  battleMapID = iter->first;
        uint32  uBattleTime = iter->second.m_nBattleTime;
        CBattleInstanceManagerPtr pBattleInstance(new CBattleInstanceManager());
        if (pBattleInstance != NULL)
        {
            pBattleInstance->Init(battleMapID,uBattleTime,iter->second);
            m_BattleInstanceMgrMap.insert(std::make_pair(battleMapID,pBattleInstance));
        }
    }*/
    /*const map<uint32,CMapDataPtr> mapDatas = CMapDataLoader::Instance()->getMaps();
    map<uint32,CMapDataPtr>::const_iterator it = mapDatas.begin();
    for (;it != mapDatas.end();it++)
    {
        uint32  battleMapID = it->first;
        uint32  uBattleTime = 24*60*60;//24Сʱ
        CBattleInstanceManagerPtr pBattleInstance(new CBattleInstanceManager());
        if (pBattleInstance != NULL)
        {
            pBattleInstance->Init(battleMapID,uBattleTime);
            m_BattleInstanceMgrMap.insert(std::make_pair(battleMapID,pBattleInstance));
        }
    }*/
    return 0;
}

uint32     CBattleManager::UnInit()
{
    m_BattleInstanceMgrMap.clear();
    //m_BattleInfoMap.clear();
    return 0;
}

CBattleInstanceManagerPtr           CBattleManager::getBattleInstanceManager(uint32 unBattleMapID)
{
    std::map<uint32,CBattleInstanceManagerPtr>::iterator iter = m_BattleInstanceMgrMap.find(unBattleMapID);
    if ( iter != m_BattleInstanceMgrMap.end() && iter->second != NULL)
    {
        return iter->second;
    }
    else
    {
        LOG_INFO("CBattleManager::getBattleInstanceManager: no such BattleMapID Create one");
        const map<uint32,CMapDataPtr> mapDatas = CMapDataLoader::Instance()->getMaps();
        map<uint32,CMapDataPtr>::const_iterator it = mapDatas.find(unBattleMapID);
        if(it != mapDatas.end())
        {
            uint32  battleMapID = it->first;
            uint32  uBattleTime = 24*60*60;//24Сʱ
            CBattleInstanceManagerPtr pBattleInstance(new CBattleInstanceManager());
            if (pBattleInstance != NULL)
            {
                pBattleInstance->Init(battleMapID,uBattleTime);
                m_BattleInstanceMgrMap.insert(std::make_pair(battleMapID,pBattleInstance));
                return pBattleInstance;
            }
        }
        else
            return CBattleInstanceManagerPtr();
    } 
}

/*uint32  CBattleManager::LoadFile(const char* szPath)
{
    if ( !m_Loader.Load(szPath) )
    {
        LOG_ERROR( "CBattleManager::Init  %s Faild !!!!!!!!!!!!!!!!!! ", szPath );
        return 1;
    }

    //��ȡ����ID����
    return 0;
}*/