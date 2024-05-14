#pragma once
#include "CUnknownEvent.h"

class CBasicEvent : public CUnknownEvent
{
public:
	CBasicEvent();
	~CBasicEvent();

	static CBasicEvent* Instance()
	{
		return new CBasicEvent();
	}

	static std::vector<stEventParam> GetParams();

	// ͨ�� IEvent �̳�
	virtual bool Analyse(const stMouseData& mouseData) override;
	virtual void GetResult(std::map<std::string, varParam>& resultMap) override;
};

