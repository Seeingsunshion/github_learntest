#include "pch.h"
#include "CCalc.h"
#include <cmath>
#include <iostream>

TSB::stPoint CCalc::center = { 0, 0 };

// 计算两点之间的距离
float CCalc::calcDistance(int x1, int y1, int x2, int y2)
{
	float movex = (float)(x2 - x1);
	float movey = (float)(y2 - y1);
	return std::sqrt(movex * movex + movey * movey);
}


// 计算点到直线之间的距离
float CCalc::calcPointToLineDistance(int x1, int y1, stPoint point1, stPoint point2)
{
	float x2 = (float)point1.x;
	float y2 = (float)point1.y;
	float x3 = (float)point2.x;
	float y3 = (float)point2.y;
	float x0 = (float)x1;
	float y0 = (float)y1;
	float distance = 0;

	// 点到直线距离公式
	distance = fabs(((y2 - y3) * x0 - (x2 - x3) * y0 + (x2 * y3 - x3 * y2)) / sqrt(pow(y2 - y3, 2) + pow(x2 - x3, 2)));

	return distance;
}


// 计算点到线段之间的距离
float CCalc::calcPointToSegmentDistance(stPoint point1, stPoint point2, stPoint point3)
{
	float a = 0, b = 0, c = 0;
	a = calcDistance(point1.x, point1.y, point3.x, point3.y);
	if (a <= 0.00001)
		return 0.0f;
	b = calcDistance(point2.x, point2.y, point3.x, point3.y);
	if (b <= 0.00001)
		return 0.0f;
	c = calcDistance(point1.x, point1.y, point2.x, point2.y);
	if (c <= 0.00001)
		return a;       //如果point1和point2坐标相同，则退出函数，并返回距离   

	if (b * b >= a * a + c * c)
		return a;      //如果是钝角返回a   
	if (a * a >= b * b + c * c)
		return b;      //如果是钝角返回b   

	float l = (a + b + c) / 2;     //周长的一半   
	float s = sqrt(l * (l - a) * (l - b) * (l - c));  //海伦公式求面积 
	return 2 * s / c;
}

// 判断点是否在区域内
bool CCalc::isPosInRegion(stPoint pos, stRegion region)
{
	if (region.shapeType == enShapType::ST_POINT)
	{
		stPoint desPos = region.points[0];
		if (pos.x == desPos.x && pos.y == desPos.y)
		{
			return true;
		}
	}
	else if (region.shapeType == enShapType::ST_LINE)
	{
		// TODO 
	}
	else if (region.shapeType == enShapType::ST_RECTANGLE)
	{
		// 矩形，坐标顺序 左上，右上， 右下， 左下
		if (pos.x >= region.points[0].x && pos.x <= region.points[2].x
			&& pos.y >= region.points[0].y && pos.y <= region.points[2].y)
		{
			return true;
		}
	}
	else if (region.shapeType == enShapType::ST_CIRCLE)
	{
		stPoint desPos = region.center;
		float distance = calcDistance(pos.x, pos.y, desPos.x, desPos.y);
		if (distance <= region.raduis)
		{
			return true;
		}
	}
	else if (region.shapeType == enShapType::ST_POLYGON)
	{
		// TODO 
	}

	return false;
}

// 计算两帧之间的夹角
double CCalc::AngleOfVector(const stPoint* p1, const stPoint* p2, const stPoint* p3, const stPoint* p4)
{
	std::cout << "p1.x = " << (*p1).x << ", p1.y = " << (*p1).y << std::endl;
	std::cout << "p2.x = " << (*p2).x << ", p2.y = " << (*p2).y << std::endl;
	std::cout << "p3.x = " << (*p3).x << ", p3.y = " << (*p3).y << std::endl;
	std::cout << "p4.x = " << (*p4).x << ", p4.y = " << (*p4).y << std::endl;
	double x1 = (*p1).x, y1 = (*p1).y, x2 = (*p2).x, y2 = (*p2).y;
	// 一般式：a1x+b1y1+c1=0
	double a1 = -(y2 - y1), b1 = x2 - x1, c1 = (y2 - y1) * x1 - (x2 - x1) * y1;
	double x3 = (*p3).x, y3 = (*p3).y, x4 = (*p4).x, y4 = (*p4).y;
	// 一般式：a2x+b2y1+c2=0
	double a2 = -(y4 - y3), b2 = x4 - x3, c2 = (y4 - y3) * x3 - (x4 - x3) * y3;
	std::cout << "a1 = " << a1 << std::endl;
	std::cout << "b1 = " << b1 << std::endl;
	std::cout << "c1 = " << c1 << std::endl;
	std::cout << "a2 = " << a2 << std::endl;
	std::cout << "b2 = " << b2 << std::endl;
	std::cout << "c2 = " << c2 << std::endl;

	bool r = false;                                                             // 判断结果
	double x0 = 0, y0 = 0;                                                      // 交点
	double angle = 0;                                                           // 夹角
	// 判断相交
	if ((a1 == 0 && b1 == 0) || (a2 == 0 && b2 == 0))     // 此时为两个点
		r = false;
	else if (b1 != 0 && b2 != 0 && (a1 / b1 != a2 / b2))  //  两条直线斜率存在，且不平行
		r = true;
	else if (b1 != 0 && b2 == 0)                          // L1斜率存在，L2与x轴垂直
		r = true;
	else if (b1 == 0 && b2 != 0)                          // L1与x轴垂直，L2斜率存在
		r = true;
	else if (0 == (a1 * b2 - a2 * b1))
		r = false;
	else if (b1 == 0 && b2 == 0)
		r = false;
	else
		r = false;
	if (r)
	{
		//计算交点
		x0 = (b1 * c2 - b2 * c1) / (a1 * b2 - a2 * b1);
		y0 = (a1 * c2 - a2 * c1) / (a2 * b1 - a1 * b2);
		center.x = x0;
		center.y = y0;
		// 计算夹角
		double a = sqrt(pow(x4 - x2, 2) + pow(y4 - y2, 2));
		double b = sqrt(pow(x4 - x0, 2) + pow(y4 - y0, 2));
		double c = sqrt(pow(x2 - x0, 2) + pow(y2 - y0, 2));
		if (!a || !b || !c)
		{
			return false;
		}
		angle = acos((b * b + c * c - a * a) / (2 * b * c));
	}
	else
	{
		angle = 0;
	}
	return angle;
}

// 判断旋转的方向
int CCalc::CalculatorDirect(const stPoint* B, const stPoint* C)
{
	int direct = 0;
	if ((*B).x == (*C).x && (*B).y == (*C).y)
	{
		std::cout << "Two points overlap!" << std::endl;
		return 1;
	}
	stPoint A = center;
	stPoint AB = { (*B).x - A.x, (*B).y - A.y};
	stPoint AC = { (*C).x - A.x, (*C).y - A.y};
	direct = AB.x * AC.y - AB.y * AC.x;

	// -1 表示逆时针方向，1表示顺时针方向
	return direct > 0 ? -1 : 1;
}