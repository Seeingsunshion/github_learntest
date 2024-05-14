#pragma once
#include "IBehavior.h"
#include "IEvent.h"
#include <mutex>
#include <thread>
#include <queue>

class CBehaviorAnalyst : public IBehavior
{
public:
	CBehaviorAnalyst(int id);
	~CBehaviorAnalyst();

	int GetId();

	void SetRegion(std::vector<stRegion>& regionVector);
	void Input(stMouseData& mouseData);
	void SubscribeEvent(enEventID eventId, stSubscribeEventParamMap params, int& sid);
	void UnsubscribeEvent(int sid);
	void SubscribeBase();
	void UnsubscribeBase();

	virtual bool GetRegion(int regionId, stRegion& region) override;

private:
	void startAnalystThread();
	void AnalystThread();
	void stopAnalystThread();
private:
	int m_id;
	int m_sid;
	bool m_bAnalyst;
	std::vector<stRegion> m_regionVector;
	std::queue<stMouseData> m_mouseData;
	bool m_bSubscribeBase; // 是否订阅基础指标

	std::map<int, IEvent*> m_subscribeEvents;
	std::mutex m_mutext;
	std::mutex m_mutextForMouseData;
	std::thread* m_pThread;
};

