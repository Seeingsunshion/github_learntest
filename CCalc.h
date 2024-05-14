/* ����ͨ�ü��㣬�����㡢�ߡ�ͼ�ε�֮����ֹ�ϵ��������룬�Ƕ� �����ڣ�����*/
/* ��Ҫ�Ӹ����¼�����ȡ�ɸ��õĸ��ּ����� */
#pragma once
#include "TopScanBehavior.h"
#define PI 3.1415926

using namespace TSB;

class CCalc
{
public:
	//С���ƶ�����֮֡��Ľ���
	static stPoint center;

	// ��������֮�����
	static float calcDistance(int x1, int y1, int x2, int y2);

	// ������㵽ֱ��֮��ľ���
	static float calcPointToLineDistance(int x1, int y1, stPoint point1, stPoint point2);

	// ����㵽�߶�֮�����̾���
	//static float calcPointToSegmentDistance(stPoint point1, stPoint point2, int x, int y);
	static float calcPointToSegmentDistance(stPoint point1, stPoint point2, stPoint point3);

	// TODO ���Ƿ���ͼ����
	static bool isPosInRegion(stPoint, stRegion);

	// TODO ��������ࡢ�Ҳࡢ�Ϸ����·�

	// TODO �㵽�߾���

	// TODO ��ͼ���Ƿ��ཻ

	// TODO ...

	/// 
	/// <summary>����ǰ����֡����С���ƶ��ĽǶ� </summary>
	/// <param name="p1">С��ʼʱ�Ǽ������</param>
	/// <param name="p2">С��ʼʱ���ĵ������</param>
	/// <param name="p3">�ƶ���С��Ǽ�ĵ�����</param>
	/// <param name="p4">�ƶ���С�����ĵ������</param>
	/// <returns>��֮֡��С���ƶ��ĽǶ�</returns>
	static double AngleOfVector(const stPoint* p1, const stPoint* p2, const stPoint* p3, const stPoint* p4);

	
	/// �ж���ת�ķ��� ˳ʱ�������ʱ��
	/// </summary>
	/// <param name="B">С��ʼʱ�Ǽ�ĵ�����</param>
	/// <param name="C">�ƶ���С��Ǽ�ĵ�����</param>
	/// <returns>˳ʱ�뷽�� 1�� ��ʱ�뷽�� -1</returns>
	static int CalculatorDirect(const stPoint* B, const stPoint* C);
};