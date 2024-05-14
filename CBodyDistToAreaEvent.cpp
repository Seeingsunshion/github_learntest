#include "pch.h"
#include "CBodyDistToAreaEvent.h"
#include "CCalc.h"
#include<algorithm>
#include <iostream>

CBodyDistToAreaEvent::CBodyDistToAreaEvent()
	: m_bTrigger(false)
	, m_beginFrame(0)
	, m_endFrame(0)
	, m_beginTime(0)
	, m_endTime(0)
{

}

CBodyDistToAreaEvent::~CBodyDistToAreaEvent()
{

}

std::vector<stEventParam> CBodyDistToAreaEvent::GetParams()
{
	std::vector<stEventParam> params;
	stEventParam paramDistanceSetValue("distance_set_value", "距离设置参考值，初始为10mm", PT_INT);
	stEventParam paramCondition("condition", "条件，1-大于，0-小于", PT_INT);
	stEventParam paramRegionId("regionId", "指定区域, 输入矿场ID", PT_INT);
	stEventParam paramBodyPosId("bodyposId", "指定小鼠身体部位，输入身体部位ID", PT_INT);
	params.push_back(paramDistanceSetValue);
	params.push_back(paramCondition);
	params.push_back(paramRegionId);
	params.push_back(paramBodyPosId);
	return params;
}

/// <summary>
/// 分析：动物身体部位距离指定的目标区域大于或小于多少毫米
/// </summary>
/// <param name="mouseData"></param>
/// <returns></returns>
bool CBodyDistToAreaEvent::Analyse(const stMouseData& mouseData)
{
	// 获取参数
	int distance_set_value = std::get<int>(m_params.at("distance_set_value"));
	int condition = std::get<int>(m_params.at("condition"));
	int regionId = std::get<int>(m_params.at("regionId"));
	int bodyposId = std::get<int>(m_params.at("bodyposId"));

	
	if (bodyposId < 0 || regionId <= 0 || distance_set_value <= 0 || (condition != 0 && condition != 1))
	{
		return false;
	}

	// 获取老鼠状态
	CMouseStatus* pMouseStatus = m_pBehavior->GetMouseStatus();

	TSB::stRegion region;
	if (m_pBehavior->GetRegion(regionId, region))
	{
		std::vector<float> dist;
		float distance = 0;
		bool bTrigger = false;
		if (region.shapeType == enShapType::ST_POINT)
		{
			stPoint desPos = region.points[0];

			// 计算老鼠身上部位区域与目标点的距离
			distance = CCalc::calcDistance(mouseData.keyPoints[bodyposId].x, mouseData.keyPoints[bodyposId].y,
				desPos.x, desPos.y);
		}
		else if (region.shapeType == enShapType::ST_LINE)
		{
			stPoint desPoframeSet = region.points[0];
			stPoint desPos2 = region.points[1];

			// 计算老鼠身上部位区域与目标直线的距离
			distance = CCalc::calcPointToLineDistance(mouseData.keyPoints[bodyposId].x, mouseData.keyPoints[bodyposId].y,
				desPoframeSet, desPos2);
			std::cout << "老鼠体位点到直线区域距离为" << distance << std::endl;
		}
		else if (region.shapeType == enShapType::ST_RECTANGLE)
		{
			stPoint desPoframeSet = region.points[0];
			stPoint desPos2 = region.points[1];
			stPoint desPos3 = region.points[2];
			stPoint desPos4 = region.points[3];

			//计算老鼠身上部位区域与目标直线的距离，最后取最小值作为老鼠身体部位与矩形距离
			for (int i = 0; i <= enMousePos::ALL; i++)
			{
				distance = CCalc::calcPointToSegmentDistance(desPoframeSet, desPos2, mouseData.keyPoints[bodyposId]);
				dist.push_back(distance);
				distance = CCalc::calcPointToSegmentDistance(desPoframeSet, desPos4, mouseData.keyPoints[bodyposId]);
				dist.push_back(distance);
				distance = CCalc::calcPointToSegmentDistance(desPos2, desPos3, mouseData.keyPoints[bodyposId]);
				dist.push_back(distance);
				distance = CCalc::calcPointToSegmentDistance(desPos3, desPos4, mouseData.keyPoints[bodyposId]);
				dist.push_back(distance);
			}
			distance = *min_element(dist.begin(), dist.end());
		}
		else if (region.shapeType == enShapType::ST_CIRCLE)
		{
			stPoint desPos = region.center;
			float pointToCenter = 0;

			// 计算老鼠身上部位区域与目标直线的距离
			pointToCenter = CCalc::calcDistance(mouseData.keyPoints[bodyposId].x, mouseData.keyPoints[bodyposId].y,
				desPos.x, desPos.y);
			distance = pointToCenter - region.raduis;
		}

		if (condition == 0)
		{
			// 小于距离设置值
			if (distance > distance_set_value)
			{
				bTrigger = true;
			}
		}
		else
		{
			// 大于距离设置值
			if (distance < distance_set_value)
			{
				bTrigger = true;
			}
		}

		if (m_bTrigger != bTrigger)
		{
			// 有变化才处理
			m_bTrigger = bTrigger;
			if (m_bTrigger)
			{
				// 触发开始
				m_beginFrame = pMouseStatus->FrameId;
				m_beginTime = pMouseStatus->TimeStamp;
				m_endFrame = 0;
				m_endTime = 0;
				m_bTrigger = false;
			}
			else
			{
				// 触发结束
				m_endFrame = pMouseStatus->FrameId;
				m_endTime = pMouseStatus->TimeStamp;
			}
			return true;
		}
	}
	return false;
}

void CBodyDistToAreaEvent::GetResult(std::map<std::string, varParam>& resultMap)
{
	resultMap.insert(std::pair<std::string, varParam>("beginFrame", m_beginFrame));
	resultMap.insert(std::pair<std::string, varParam>("endFrame", m_endFrame));
	resultMap.insert(std::pair<std::string, varParam>("beginTime", m_beginTime));
	resultMap.insert(std::pair<std::string, varParam>("endTime", m_endTime));
}

