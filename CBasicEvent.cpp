#include "pch.h"
#include "CBasicEvent.h"

CBasicEvent::CBasicEvent()
{

}
CBasicEvent::~CBasicEvent()
{

}

std::vector<stEventParam> CBasicEvent::GetParams()
{
	// TODO
	std::vector<stEventParam> params;
	return params;
}

/// <summary>
///
/// </summary>
/// <param name="mouseData"></param>
/// <returns></returns>
bool CBasicEvent::Analyse(const stMouseData& mouseData)
{
	// TODO
	return false;
}

void CBasicEvent::GetResult(std::map<std::string, varParam>& resultMap)
{
	// TODO
}