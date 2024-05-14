#include "pch.h"
#include "CBehaviorManager.h"
#include "CEventManager.h"

CBehaviorManager* CBehaviorManager::m_instance = 0;
CReleaseSingle CBehaviorManager::m_releaseSingle;
CReleaseSingle::CReleaseSingle()
{

}
CReleaseSingle::~CReleaseSingle()
{
	CBehaviorManager::Release();
}

CBehaviorManager::CBehaviorManager()
	: m_id(1)
	, m_pEventCallback(NULL)
	, m_pBaseCallback(NULL)
{
	CEventManager::RegisterAllEvents();
}
CBehaviorManager::~CBehaviorManager()
{
	for (BehaviorAnalystMap::iterator it = m_behaviorAnalystMap.begin(); it != m_behaviorAnalystMap.end(); ++it)
	{
		CBehaviorAnalyst* pChannel = (CBehaviorAnalyst*)it->second;
		if (pChannel)
		{
			delete pChannel;
		}
	}
	m_behaviorAnalystMap.clear();
}

void CBehaviorManager::GetEvents(std::vector<stEvent>& eventVector)
{
	CEventManager::GetEvents(eventVector);
}

bool CBehaviorManager::GetEventById(enEventID eventId, stEvent& event)
{
	return CEventManager::GetEventById(eventId, event);
}

int CBehaviorManager::CreateBehaviorObject()
{
	CBehaviorAnalyst* pObject = GetBehaviorObject(m_id);
	m_id++;
	return pObject->GetId();
}

CBehaviorAnalyst* CBehaviorManager::GetBehaviorObject(int id)
{
	BehaviorAnalystMap::iterator it = m_behaviorAnalystMap.find(id);
	if (it != m_behaviorAnalystMap.end())
	{
		return m_behaviorAnalystMap.at(id);
	}
	else
	{
		CBehaviorAnalyst* pChannel = new CBehaviorAnalyst(id);
		m_behaviorAnalystMap.insert(m_behaviorAnalystMap.end(), std::pair<int, CBehaviorAnalyst*>(id, pChannel));
		return pChannel;
	}
}

void CBehaviorManager::DestroyBehaviorObject(int id)
{
	BehaviorAnalystMap::iterator it = m_behaviorAnalystMap.find(id);
	if (it != m_behaviorAnalystMap.end())
	{
		CBehaviorAnalyst* pChannel = m_behaviorAnalystMap.at(id);
		if (pChannel) {
			delete pChannel;
		}

		m_behaviorAnalystMap.erase(it);
	}
}

void CBehaviorManager::SetEventCallback(TSB_EventCallBack eventCallback)
{
	m_pEventCallback = eventCallback;
}

void CBehaviorManager::SetBaseCallback(TSB_BaseCallBack eventCallback)
{
	m_pBaseCallback = eventCallback;
}

void CBehaviorManager::SendBase(int behaviorId, stUnderlyingMetrics& baseMetrics)
{
	if (m_pBaseCallback)
	{
		m_pBaseCallback(behaviorId, baseMetrics);
	}
}

void CBehaviorManager::SendEvent(int behaviorId, int subscribeId, TSB::enEventID eventId, std::map<std::string, varParam>& retuslt)
{
	if (m_pEventCallback)
	{
		m_pEventCallback(behaviorId, subscribeId, eventId, retuslt);
	}
}