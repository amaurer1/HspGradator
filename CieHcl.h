//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// CieHcl.h
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Copyright (c) 2019-2024 Adrian Maurer. All rights reserved.
// Distributed under the MIT software license (http://www.opensource.org/licenses/mit-license.php).
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Based on the conversion formulas from EasyRGB (https://www.easyrgb.com/en/math.php).
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifndef AMA_CIE_HCL_H
#define AMA_CIE_HCL_H
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include <cmath>
#include <concepts>
#include "ColorModel.h"
#include "Constant.h"
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
namespace ama {
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
template <class T>
class CieHcl
{
// ELEMENT DATA
public:
	T h, c, l;

// CONSTRUCTION / DESTRUCTION / ASSIGNMENT
public:
	constexpr CieHcl() noexcept : h(T(0.0)), c(T(0.0)), l(T(0.0)) {}
	constexpr CieHcl(const T vh, const T vc, const T vl) noexcept : h(vh), c(vc), l(vl) {}
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
template <class T>
inline CieHcl<T> convertToCieHcl(const Rgb<T>& c)
requires std::floating_point<T>
{
	// Gamma correction
	const T tr = c.r > T(0.04045) ? pow((c.r + T(0.055)) / T(1.055), T(2.4)) : c.r / T(12.92);
	const T tg = c.g > T(0.04045) ? pow((c.g + T(0.055)) / T(1.055), T(2.4)) : c.g / T(12.92);
	const T tb = c.b > T(0.04045) ? pow((c.b + T(0.055)) / T(1.055), T(2.4)) : c.b / T(12.92);
	// Xyz
	const T x = tr * T(0.4124) + tg * T(0.3576) + tb * T(0.1805);
	const T y = tr * T(0.2126) + tg * T(0.7152) + tb * T(0.0722);
	const T z = tr * T(0.0193) + tg * T(0.1192) + tb * T(0.9505);
	// Illuminant = D65/2�
	const T tx = x * T(100.0 /  95.047);
	const T ty = y * T(100.0 / 100.000);
	const T tz = z * T(100.0 / 108.883);
	// Lab
	const T ux = tx > T(0.008856) ? pow(tx, T(1.0 / 3.0)) : (T(7.787) * tx) + T(16.0 / 116.0);
	const T uy = ty > T(0.008856) ? pow(ty, T(1.0 / 3.0)) : (T(7.787) * ty) + T(16.0 / 116.0);
	const T uz = tz > T(0.008856) ? pow(tz, T(1.0 / 3.0)) : (T(7.787) * tz) + T(16.0 / 116.0);
	const T l = T(116.0) * uy - T(16.0);
	const T a = T(500.0) * (ux - uy);
	const T b = T(200.0) * (uy - uz);
	// Hcl
	const T ta = atan2(b, a);
	const T th = ta < T(0.0) ? ta + T(rad_360) : ta;
	const T tc = sqrt(a * a + b * b);
	return {th / T(rad_360), tc / T(200.0), l / T(100.0)};
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
template <class T>
inline Rgb<T> convertToRgb(const CieHcl<T>& c)
requires std::floating_point<T>
{
	// Hcl
	const T th = c.h * T(rad_360);
	const T tc = c.c * T(200.0);
	const T l = c.l * T(100.0);
	const T a = cos(th) * tc;
	const T b = sin(th) * tc;
	// Lab
	const T ty = (l + T(16.0)) / T(116.0);
	const T tx = ty + a / T(500.0);
	const T tz = ty - b / T(200.0);
	const T ux = tx * tx * tx;
	const T uy = ty * ty * ty;
	const T uz = tz * tz * tz;
	const T vx = ux > T(0.008856) ? ux : (tx - T(16.0 / 116.0)) * T(1.0 / 7.787);
	const T vy = uy > T(0.008856) ? uy : (ty - T(16.0 / 116.0)) * T(1.0 / 7.787);
	const T vz = uz > T(0.008856) ? uz : (tz - T(16.0 / 116.0)) * T(1.0 / 7.787);
	// Illuminant = D65/2�
	const T wx = vx * T( 95.047 / 100.0);
	const T wy = vy * T(100.000 / 100.0);
	const T wz = vz * T(108.883 / 100.0);
	// Xyz
	const T tr = wx * T( 3.2406) + wy * T(-1.5372) + wz * T(-0.4986);
	const T tg = wx * T(-0.9689) + wy * T( 1.8758) + wz * T( 0.0415);
	const T tb = wx * T( 0.0557) + wy * T(-0.2040) + wz * T( 1.0570);
	// Gamma correction
	const T ur = tr > T(0.0031308) ? T(1.055) * pow(tr, T(1.0 / 2.4)) - T(0.055) : tr * T(12.92);
	const T ug = tg > T(0.0031308) ? T(1.055) * pow(tg, T(1.0 / 2.4)) - T(0.055) : tg * T(12.92);
	const T ub = tb > T(0.0031308) ? T(1.055) * pow(tb, T(1.0 / 2.4)) - T(0.055) : tb * T(12.92);
	return {ur, ug, ub};
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
} //namespace ama
  //--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#endif // AMA_CIE_HCL_H
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
