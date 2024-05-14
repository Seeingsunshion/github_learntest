#pragma once

#include <map>
#include "CBehaviorAnalyst.h"
#include "TopScanBehavior.h"
using namespace TSB;

typedef std::map<int, CBehaviorAnalyst*> BehaviorAnalystMap;

/// <summary>
/// 为了释放CBehaviorManager单件实例
/// </summary>
class CReleaseSingle {
public:
	CReleaseSingle();
	~CReleaseSingle();
};

/// <summary>
/// 行为分析管理 单件
/// </summary>
class CBehaviorManager
{
public:
	~CBehaviorManager();
	static CBehaviorManager* Instance()
	{
		if (!m_instance)
		{
			m_instance = new CBehaviorManager();
		}
		return m_instance;
	}
	static void Release()
	{
		if (m_instance)
		{
			delete m_instance;
		}
	}

	static CReleaseSingle m_releaseSingle;

	void GetEvents(std::vector<stEvent>& eventVector);
	bool GetEventById(enEventID eventId, stEvent& event);
	int CreateBehaviorObject();
	CBehaviorAnalyst* GetBehaviorObject(int id);
	void DestroyBehaviorObject(int id);

	void SetEventCallback(TSB_EventCallBack eventCallback);
	void SetBaseCallback(TSB_BaseCallBack eventCallback);

	void SendBase(int behaviorId, stUnderlyingMetrics& baseMetrics);
	void SendEvent(int behaviorId, int subscribeId, TSB::enEventID eventId, std::map<std::string, varParam>& retuslt);
private:
	CBehaviorManager();
private:
	static CBehaviorManager* m_instance;
	BehaviorAnalystMap m_behaviorAnalystMap;
	int m_id;

	TSB_EventCallBack m_pEventCallback;
	TSB_BaseCallBack m_pBaseCallback;
};