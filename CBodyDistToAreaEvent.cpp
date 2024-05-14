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
	stEventParam paramDistanceSetValue("distance_set_value", "�������òο�ֵ����ʼΪ10mm", PT_INT);
	stEventParam paramCondition("condition", "������1-���ڣ�0-С��", PT_INT);
	stEventParam paramRegionId("regionId", "ָ������, �����ID", PT_INT);
	stEventParam paramBodyPosId("bodyposId", "ָ��С�����岿λ���������岿λID", PT_INT);
	params.push_back(paramDistanceSetValue);
	params.push_back(paramCondition);
	params.push_back(paramRegionId);
	params.push_back(paramBodyPosId);
	return params;
}

/// <summary>
/// �������������岿λ����ָ����Ŀ��������ڻ�С�ڶ��ٺ���
/// </summary>
/// <param name="mouseData"></param>
/// <returns></returns>
bool CBodyDistToAreaEvent::Analyse(const stMouseData& mouseData)
{
	// ��ȡ����
	int distance_set_value = std::get<int>(m_params.at("distance_set_value"));
	int condition = std::get<int>(m_params.at("condition"));
	int regionId = std::get<int>(m_params.at("regionId"));
	int bodyposId = std::get<int>(m_params.at("bodyposId"));

	
	if (bodyposId < 0 || regionId <= 0 || distance_set_value <= 0 || (condition != 0 && condition != 1))
	{
		return false;
	}

	// ��ȡ����״̬
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

			// �����������ϲ�λ������Ŀ���ľ���
			distance = CCalc::calcDistance(mouseData.keyPoints[bodyposId].x, mouseData.keyPoints[bodyposId].y,
				desPos.x, desPos.y);
		}
		else if (region.shapeType == enShapType::ST_LINE)
		{
			stPoint desPoframeSet = region.points[0];
			stPoint desPos2 = region.points[1];

			// �����������ϲ�λ������Ŀ��ֱ�ߵľ���
			distance = CCalc::calcPointToLineDistance(mouseData.keyPoints[bodyposId].x, mouseData.keyPoints[bodyposId].y,
				desPoframeSet, desPos2);
			std::cout << "������λ�㵽ֱ���������Ϊ" << distance << std::endl;
		}
		else if (region.shapeType == enShapType::ST_RECTANGLE)
		{
			stPoint desPoframeSet = region.points[0];
			stPoint desPos2 = region.points[1];
			stPoint desPos3 = region.points[2];
			stPoint desPos4 = region.points[3];

			//�����������ϲ�λ������Ŀ��ֱ�ߵľ��룬���ȡ��Сֵ��Ϊ�������岿λ����ξ���
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

			// �����������ϲ�λ������Ŀ��ֱ�ߵľ���
			pointToCenter = CCalc::calcDistance(mouseData.keyPoints[bodyposId].x, mouseData.keyPoints[bodyposId].y,
				desPos.x, desPos.y);
			distance = pointToCenter - region.raduis;
		}

		if (condition == 0)
		{
			// С�ھ�������ֵ
			if (distance > distance_set_value)
			{
				bTrigger = true;
			}
		}
		else
		{
			// ���ھ�������ֵ
			if (distance < distance_set_value)
			{
				bTrigger = true;
			}
		}

		if (m_bTrigger != bTrigger)
		{
			// �б仯�Ŵ���
			m_bTrigger = bTrigger;
			if (m_bTrigger)
			{
				// ������ʼ
				m_beginFrame = pMouseStatus->FrameId;
				m_beginTime = pMouseStatus->TimeStamp;
				m_endFrame = 0;
				m_endTime = 0;
				m_bTrigger = false;
			}
			else
			{
				// ��������
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

