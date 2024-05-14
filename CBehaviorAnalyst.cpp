#include "pch.h"
#include "CBehaviorAnalyst.h"
#include "CEventManager.h"
#include "CBehaviorManager.h"
#include <iostream>
using namespace std;

CBehaviorAnalyst::CBehaviorAnalyst(int id)
	:m_id(id)
	, m_sid(0)
	, m_bAnalyst(false)
	, m_bSubscribeBase(false)
	, m_pThread(NULL)
{
}

CBehaviorAnalyst::~CBehaviorAnalyst()
{
	stopAnalystThread();

	m_mutext.lock();
	for (auto event : m_subscribeEvents)
	{
		IEvent* pEvent = event.second;
		if (pEvent)
		{
			delete pEvent;
		}
	}
	m_subscribeEvents.clear();
	m_mutext.unlock();

	m_mutextForMouseData.lock();
	while (!m_mouseData.empty())
	{
		m_mouseData.pop();
	}
	m_mutextForMouseData.unlock();
}

void CBehaviorAnalyst::AnalystThread()
{
	// TODO 开启分析线程
	// 1. 读取最新的MouseData
	// 2. 分析老鼠状态：体轴长度，腰轴长度等等基础状态
	// 3. 遍历所有已订阅事件对象，传递MouseData和MouseStatus给每个事件对象
	// 4. 根据分析返回值，如果有事件，则获取事件结果
	// 5. 调用回调函数传递事件结果
	// 6. 回到1
	while (m_bAnalyst)
	{
		if (!m_mouseData.empty())
		{
			m_mutextForMouseData.lock();
			stMouseData md = m_mouseData.front();
			m_mouseData.pop();
			m_mutextForMouseData.unlock();

			// 分析老鼠状态
			m_mouseStatus.InputMouseData(md);

			// 发送基础指标
			stUnderlyingMetrics baseMetrics;
			baseMetrics.totalFrames = md.frame;
			baseMetrics.totalTime = md.timeStamp;
			baseMetrics.avgSpeedMM = m_mouseStatus.SpeedMM;
			baseMetrics.avgSpeedPixel = m_mouseStatus.SpeedPixel;
			baseMetrics.totalDistance = m_mouseStatus.AllMoveMM;
			CBehaviorManager::Instance()->SendBase(m_id, baseMetrics);

			// 遍历事件对象，进行分析
			for (auto event : m_subscribeEvents)
			{
				if (!m_bAnalyst)
				{
					break;
				}
				int subscribeId = event.first;
				IEvent* pEvent = event.second;
				if (pEvent)
				{
					enEventID eventId = pEvent->GetEventId();
					//cout << "eventId = " << eventId;
					// 分析成功
					if (pEvent->Analyse(md))
					{
						// 获取结果
						std::map<std::string, varParam> result;
						pEvent->GetResult(result);

						stEvent eventInfo;
						CEventManager::GetEventById(eventId, eventInfo);

						// 事件ID
						result.insert(std::pair<std::string, varParam>("eventId", eventId));
						// 事件时间
						result.insert(std::pair<std::string, varParam>("time", m_mouseStatus.TimeStamp));

						CBehaviorManager::Instance()->SendEvent(m_id, subscribeId, eventId, result);
					}
				}
			}
		}
		else
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(5));
		}
	}
}

void CBehaviorAnalyst::startAnalystThread()
{
	if (!m_pThread)
	{
		m_bAnalyst = true;
		m_pThread = new std::thread(&CBehaviorAnalyst::AnalystThread, this);
		m_pThread->detach();
	}
}

void CBehaviorAnalyst::stopAnalystThread()
{
	m_bAnalyst = false;
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	if (m_pThread)
	{
		delete m_pThread;
		m_pThread = NULL;
	}
}

int CBehaviorAnalyst::GetId()
{
	return m_id;
}

void CBehaviorAnalyst::SetRegion(std::vector<stRegion>& regionVector)
{
	m_regionVector = regionVector;
}

void CBehaviorAnalyst::Input(stMouseData& mouseData)
{
	// 老鼠数据缓冲区 std::stack
	m_mutextForMouseData.lock();
	m_mouseData.push(mouseData);
	m_mutextForMouseData.unlock();

	if (m_pThread == NULL)
	{
		startAnalystThread();
	}
}

void CBehaviorAnalyst::SubscribeEvent(enEventID eventId, stSubscribeEventParamMap params, int& sid)
{
	// 实例化事件
	// 传递参数	

	IEvent* pEvent = CEventManager::CreateEventObject(this, eventId, params);
	if (pEvent != NULL)
	{
		m_sid++;
		m_mutext.lock();
		m_subscribeEvents.insert(std::pair<int, IEvent*>(m_sid, pEvent));
		m_mutext.unlock();
		sid = m_sid;
	}
	else
	{
		// 无效事件ID
		sid = -1;
	}
}

void CBehaviorAnalyst::UnsubscribeEvent(int sid)
{
	m_mutext.lock();
	auto event = m_subscribeEvents.find(sid);
	if (event != m_subscribeEvents.end())
	{
		delete event->second;
	}
	m_subscribeEvents.erase(sid);
	m_mutext.unlock();
}

void CBehaviorAnalyst::SubscribeBase()
{
	m_bSubscribeBase = true;
}

void CBehaviorAnalyst::UnsubscribeBase()
{
	m_bSubscribeBase = false;
}

bool CBehaviorAnalyst::GetRegion(int regionId, stRegion& _region)
{
	bool ret = false;
	for (auto region : m_regionVector)
	{
		if (region.id == regionId)
		{
			_region = region;
			ret = true;
			break;
		}
	}
	return ret;
}