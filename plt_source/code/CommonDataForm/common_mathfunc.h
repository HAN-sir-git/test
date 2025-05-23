#ifndef COM_MATHFUNC_H
#define COM_MATHFUNC_H

#include <QPointF>
#include <QLineF>

namespace COMMON_Math {

double angleTo(const QPointF p1, const QPointF p2);

double getAngle(const QPointF p);
// correct angle to be within [0, +PI*2.0)
double correctAngle(const double& angle);
// correct angle to be within [-PI, +PI)
double correctAngle2(const double &angle);
// correct angle to be unsigned [0, +PI)
double correctAngleU(const double &angle);
//! \brief angular difference
double getAngleDifference(double a1, double a2, bool reversed = false);

//! \brief 如果给定的角度在可读范围内，则返回true
bool isAngleReadable(double angle);

/*
*使给定角度的文本可读。使用
*用于尺寸文本和镜像文本。
*
* @返回给定的角度或给定的角度+PI，取决于哪个
*从底部或右侧可读。
*/
double makeAngleReadable(double angle, bool readable = true, bool *corrected = nullptr);

// 判断两个点是否相等的辅助函数，考虑浮点误差
bool arePointsEqual(const QPointF& p1, const QPointF& p2, double epsilon = 1e-6);

// 检查两条线段是否在端点相接
bool areLinesEndpointConnected(const QLineF& line1, const QLineF& line2,QPointF& intersectionPoint);

}

#endif // COM_MATHFUNC_H
