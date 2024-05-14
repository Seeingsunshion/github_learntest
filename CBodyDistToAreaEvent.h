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

	// 通过 IEvent 继承
	virtual bool Analyse(const stMouseData& mouseData) override;
	virtual void GetResult(std::map<std::string, varParam>& resultMap) override;

private:

	/* 过程变量 */
	bool m_bTrigger;	// 是否触发
	int m_beginFrame; // 触发开始时帧序号
	int m_endFrame;	// 触发结束时帧序号
	int m_beginTime; // 触发开始时帧序号
	int m_endTime;	// 触发结束时帧序号
};