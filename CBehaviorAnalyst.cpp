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
	// TODO ���������߳�
	// 1. ��ȡ���µ�MouseData
	// 2. ��������״̬�����᳤�ȣ����᳤�ȵȵȻ���״̬
	// 3. ���������Ѷ����¼����󣬴���MouseData��MouseStatus��ÿ���¼�����
	// 4. ���ݷ�������ֵ��������¼������ȡ�¼����
	// 5. ���ûص����������¼����
	// 6. �ص�1
	while (m_bAnalyst)
	{
		if (!m_mouseData.empty())
		{
			m_mutextForMouseData.lock();
			stMouseData md = m_mouseData.front();
			m_mouseData.pop();
			m_mutextForMouseData.unlock();

			// ��������״̬
			m_mouseStatus.InputMouseData(md);

			// ���ͻ���ָ��
			stUnderlyingMetrics baseMetrics;
			baseMetrics.totalFrames = md.frame;
			baseMetrics.totalTime = md.timeStamp;
			baseMetrics.avgSpeedMM = m_mouseStatus.SpeedMM;
			baseMetrics.avgSpeedPixel = m_mouseStatus.SpeedPixel;
			baseMetrics.totalDistance = m_mouseStatus.AllMoveMM;
			CBehaviorManager::Instance()->SendBase(m_id, baseMetrics);

			// �����¼����󣬽��з���
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
					// �����ɹ�
					if (pEvent->Analyse(md))
					{
						// ��ȡ���
						std::map<std::string, varParam> result;
						pEvent->GetResult(result);

						stEvent eventInfo;
						CEventManager::GetEventById(eventId, eventInfo);

						// �¼�ID
						result.insert(std::pair<std::string, varParam>("eventId", eventId));
						// �¼�ʱ��
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
	// �������ݻ����� std::stack
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
	// ʵ�����¼�
	// ���ݲ���	

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
		// ��Ч�¼�ID
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