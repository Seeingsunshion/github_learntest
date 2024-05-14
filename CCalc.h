/* 各种通用计算，包括点、线、图形等之间各种关系，比如距离，角度 ，在内，在外*/
/* 需要从各种事件中提取可复用的各种计算表达 */
#pragma once
#include "TopScanBehavior.h"
#define PI 3.1415926

using namespace TSB;

class CCalc
{
public:
	//小鼠移动后两帧之间的交点
	static stPoint center;

	// 计算两点之间距离
	static float calcDistance(int x1, int y1, int x2, int y2);

	// 计算机点到直线之间的距离
	static float calcPointToLineDistance(int x1, int y1, stPoint point1, stPoint point2);

	// 计算点到线段之间的最短距离
	//static float calcPointToSegmentDistance(stPoint point1, stPoint point2, int x, int y);
	static float calcPointToSegmentDistance(stPoint point1, stPoint point2, stPoint point3);

	// TODO 点是否在图形内
	static bool isPosInRegion(stPoint, stRegion);

	// TODO 点在线左侧、右侧、上方、下方

	// TODO 点到线距离

	// TODO 两图形是否相交

	// TODO ...

	/// 
	/// <summary>计算前后两帧画面小鼠移动的角度 </summary>
	/// <param name="p1">小鼠开始时鼻尖的坐标</param>
	/// <param name="p2">小鼠开始时中心点的坐标</param>
	/// <param name="p3">移动后小鼠鼻尖的点坐标</param>
	/// <param name="p4">移动后小鼠中心点的坐标</param>
	/// <returns>两帧之间小鼠移动的角度</returns>
	static double AngleOfVector(const stPoint* p1, const stPoint* p2, const stPoint* p3, const stPoint* p4);

	
	/// 判断旋转的方向 顺时针或者逆时针
	/// </summary>
	/// <param name="B">小鼠开始时鼻尖的点坐标</param>
	/// <param name="C">移动后小鼠鼻尖的点坐标</param>
	/// <returns>顺时针方向 1， 逆时针方向 -1</returns>
	static int CalculatorDirect(const stPoint* B, const stPoint* C);
};