#pragma once
#include "CUnknownEvent.h"

class CBodyDistToAreaEvent : public CUnknownEvent
{
public:
	CBodyDistToAreaEvent();
	~CBodyDistToAreaEvent();

	static CBodyDistToAreaEvent* Instance()
	{
		return new CBodyDistToAreaEvent();
	}

	static std::vector<stEventParam> GetParams();

	// ͨ�� IEvent �̳�
	virtual bool Analyse(const stMouseData& mouseData) override;
	virtual void GetResult(std::map<std::string, varParam>& resultMap) override;

private:

	/* ���̱��� */
	bool m_bTrigger;	// �Ƿ񴥷�
	int m_beginFrame; // ������ʼʱ֡���
	int m_endFrame;	// ��������ʱ֡���
	int m_beginTime; // ������ʼʱ֡���
	int m_endTime;	// ��������ʱ֡���
};