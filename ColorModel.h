//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ColorModel.h
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Copyright (c) 2019-2024 Adrian Maurer. All rights reserved.
// Distributed under the MIT software license (http://www.opensource.org/licenses/mit-license.php).
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifndef AMA_COLOR_MODEL_H
#define AMA_COLOR_MODEL_H
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include <algorithm>
#include <concepts>
#include "Color.h"
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
namespace ama {
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Rgb<T>
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
template <class T>
using Rgb = Color<T>;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Hsv<T>
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
template <class T>
class Hsv
{
// ELEMENT DATA
public:
	T h, s, v;

// CONSTRUCTION / DESTRUCTION / ASSIGNMENT
public:
	constexpr Hsv() noexcept : h(T(0)), s(T(0)), v(T(0)) {}
	constexpr Hsv(const T vh, const T vs, const T vv) noexcept : h(vh), s(vs), v(vv) {}
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Hsl<T>
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
template <class T>
class Hsl
{
// ELEMENT DATA
public:
	T h, s, l;

// CONSTRUCTION / DESTRUCTION / ASSIGNMENT
public:
	constexpr Hsl() noexcept : h(T(0)), s(T(0)), l(T(0)) {}
	constexpr Hsl(const T vh, const T vs, const T vl) noexcept : h(vh), s(vs), l(vl) {}
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Hsg<T>
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
template <class T>
class Hsg
{
// ELEMENT DATA
public:
	T h, s, g;

// CONSTRUCTION / DESTRUCTION / ASSIGNMENT
public:
	constexpr Hsg() noexcept : h(T(0)), s(T(0)), g(T(0)) {}
	constexpr Hsg(const T vh, const T vs, const T vg) noexcept : h(vh), s(vs), g(vg) {}
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Hsp<T>
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
template <class T>
class Hsp
{
// ELEMENT DATA
public:
	T h, s, p;

// CONSTRUCTION / DESTRUCTION / ASSIGNMENT
public:
	constexpr Hsp() noexcept : h(T(0)), s(T(0)), p(T(0)) {}
	constexpr Hsp(const T vh, const T vs, const T vp) noexcept : h(vh), s(vs), p(vp) {}
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
template <class T>
constexpr bool isChromatic(const Rgb<T>& c) noexcept
requires std::floating_point<T>
{
	return c.r != c.g || c.r != c.b;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
template <class T>
constexpr bool isChromatic(const Hsv<T>& c) noexcept
requires std::floating_point<T>
{
	return c.v > T(0.0) && c.s > T(0.0);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
template <class T>
constexpr bool isChromatic(const Hsl<T>& c) noexcept
requires std::floating_point<T>
{
	return c.l > T(0.0) && c.l < T(1.0) && c.s > T(0.0);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
template <class T>
constexpr bool isChromatic(const Hsg<T>& c) noexcept
requires std::floating_point<T>
{
	return c.s > T(0.0);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
template <class T>
constexpr bool isChromatic(const Hsp<T>& c) noexcept
requires std::floating_point<T>
{
	return c.p > T(0.0) && c.p < T(1.0) && c.s > T(0.0);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
template <class T>
constexpr bool isInRange(const T v1, const T v2, const T v3) noexcept
requires std::floating_point<T>
{
	return v1 >= T(0.0) && v1 <= T(1.0) && v2 >= T(0.0) && v2 <= T(1.0) && v3 >= T(0.0) && v3 <= T(1.0);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
template <class T>
constexpr Rgb<T> calcHueColor(const T h) noexcept
requires std::floating_point<T>
{
	const T v = h < T(1.0) ? h * T(6.0) : T(0.0);
	const int i = static_cast<int>(v);
	const T f = v - i;
	switch (i)
	{
		case 0: return {T(1.0), f, T(0.0)};
		case 1: return {T(1.0) - f, T(1.0), T(0.0)};
		case 2: return {T(0.0), T(1.0), f};
		case 3: return {T(0.0), T(1.0) - f, T(1.0)};
		case 4: return {f, T(0.0), T(1.0)};
		case 5: return {T(1.0), T(0.0), T(1.0) - f};
		default: return {};
	}
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
template <class T>
constexpr Rgb<T> convertToRgb(const Hsv<T>& c) noexcept
requires std::floating_point<T>
{
	if (isInRange(c.h, c.s, c.v))
	{
		return (calcHueColor(c.h) * c.s + (T(1.0) - c.s)) * c.v;
	}
	return {T(0.0), T(0.0), T(0.0)};
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
template <class T>
constexpr Rgb<T> convertToRgb(const Hsl<T>& c) noexcept
requires std::floating_point<T>
{
	if (isInRange(c.h, c.s, c.l))
	{
		const Rgb<T> mc(calcHueColor(c.h) * c.s + T(0.5) * (T(1.0) - c.s));
		if (c.l < T(0.5))
		{
			return mc * c.l / T(0.5);
		}
		else
		{
			const T l = (c.l - T(0.5)) / T(0.5);
			return mc * (T(1.0) - l) + l;
		}
	}
	return {T(0.0), T(0.0), T(0.0)};
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
template <class T>
constexpr Rgb<T> convertToRgb(const Hsg<T>& c) noexcept
requires std::floating_point<T>
{
	if (isInRange(c.h, c.s, c.g))
	{
		return calcHueColor(c.h) * c.s + c.g * (T(1.0) - c.s);
	}
	return {T(0.0), T(0.0), T(0.0)};
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
template <class T>
constexpr T calcPerceivedBrightness(const Rgb<T>& c) noexcept
requires std::floating_point<T>
{
	return c.r * T(0.299) + c.g * T(0.587) + c.b * T(0.114);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
template <class T>
constexpr Rgb<T> convertToRgb(const Hsp<T>& c) noexcept
requires std::floating_point<T>
{
	if (isInRange(c.h, c.s, c.p))
	{
		const Rgb<T> hc(calcHueColor(c.h));
		const T hcpb = calcPerceivedBrightness(hc);
		const Rgb<T> mc(hc * c.s + hcpb * (T(1.0) - c.s));
		if (c.p < hcpb)
		{
			return mc * c.p / hcpb;
		}
		else
		{
			const T pb = (c.p - hcpb) / (T(1.0) - hcpb);
			return mc * (T(1.0) - pb) + pb;
		}
	}
	return {T(0.0), T(0.0), T(0.0)};
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
template <class T>
constexpr T getMax(const Rgb<T> c) noexcept
requires std::floating_point<T>
{
	return (std::max)((std::max)(c.r, c.g), c.b);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
template <class T>
constexpr T getMin(const Rgb<T> c) noexcept
requires std::floating_point<T>
{
	return (std::min)((std::min)(c.r, c.g), c.b);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
template <class T>
constexpr T calcColorHue(const Rgb<T>& c, const T max, const T d) noexcept
requires std::floating_point<T>
{
	T h = T(0.0);
	if (c.r == max) h = (c.g - c.b) / d;
	else if (c.g == max) h = T(2.0) + (c.b - c.r) / d;
	else if (c.b == max) h = T(4.0) + (c.r - c.g) / d;
	h /= T(6.0);
	if (h < T(0.0)) h += T(1.0);
	return h;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
template <class T>
constexpr Hsv<T> convertToHsv(const Rgb<T>& c) noexcept
requires std::floating_point<T>
{
	if (isInRange(c.r, c.g, c.b))
	{
		if (const T max = getMax(c))
		{
			const T min = getMin(c);
			if (const T d = max - min)
			{
				return {calcColorHue(c, max, d), d / max, max};
			}
			return {T(0.0), T(0.0), max};
		}
	}
	return {T(0.0), T(0.0), T(0.0)};
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
template <class T>
constexpr Hsl<T> convertToHsl(const Rgb<T>& c) noexcept
requires std::floating_point<T>
{
	if (isInRange(c.r, c.g, c.b))
	{
		if (const T max = getMax(c))
		{
			const T min = getMin(c);
			const T l = (max + min) / T(2.0);
			if (const T d = max - min)
			{
				const T s = l <= T(0.5) ? d / (l / T(0.5)) : d / (T(1.0) - (l - T(0.5)) / T(0.5));
				return {calcColorHue(c, max, d), s < T(1.0) ? s : T(1.0), l};
			}
			return {T(0.0), T(0.0), l};
		}
	}
	return {T(0.0), T(0.0), T(0.0)};
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
template <class T>
constexpr Hsg<T> convertToHsg(const Rgb<T>& c) noexcept
requires std::floating_point<T>
{
	if (isInRange(c.r, c.g, c.b))
	{
		if (const T max = getMax(c))
		{
			const T min = getMin(c);
			if (const T d = max - min)
			{
				const T g = d == T(1.0) ? T(0.0) : min / (T(1.0) - d);
				return {calcColorHue(c, max, d), d, g < T(1.0) ? g : T(1.0)};
			}
			return {T(0.0), T(0.0), max};
		}
	}
	return {T(0.0), T(0.0), T(0.0)};
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
template <class T>
constexpr Hsp<T> convertToHsp(const Rgb<T>& c) noexcept
requires std::floating_point<T>
{
	if (isInRange(c.r, c.g, c.b))
	{
		if (const T max = getMax(c))
		{
			const T min = getMin(c);
			const T pb = calcPerceivedBrightness(c);
			if (const T d = max - min)
			{
				const T h = calcColorHue(c, max, d);
				const T hcpb = calcPerceivedBrightness(calcHueColor(h));
				const T s = pb <= hcpb ? d / (pb / hcpb) : d / (T(1.0) - (pb - hcpb) / (T(1.0) - hcpb));
				return {h, s < T(1.0) ? s : T(1.0), pb};
			}
			return {T(0.0), T(0.0), pb};
		}
	}
	return {T(0.0), T(0.0), T(0.0)};
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
} //namespace ama
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#endif // AMA_COLOR_MODEL_H
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
