//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Color.h
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Copyright (c) 2023 Adrian Maurer. All rights reserved.
// Distributed under the MIT software license (http://www.opensource.org/licenses/mit-license.php).
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifndef AMA_COLOR_H
#define AMA_COLOR_H
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
namespace ama {
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Color<T>
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
template <class T>
class Color
{
// ELEMENT DATA
public:
	T r, g, b;

// CONSTRUCTION / DESTRUCTION / ASSIGNMENT
public:
	constexpr Color() noexcept : r(T(0)), g(T(0)), b(T(0)) {}
	constexpr Color(const T v) noexcept : r(v), g(v), b(v) {}
	constexpr Color(const T vr, const T vg, const T vb) noexcept : r(vr), g(vg), b(vb) {}

// ELEMENT FUNCTIONS
public:
	constexpr Color& operator+=(const Color<T>& c) noexcept {r += c.r, g += c.g, b += c.b; return *this;}
	constexpr Color& operator-=(const Color<T>& c) noexcept {r -= c.r, g -= c.g, b -= c.b; return *this;}
	constexpr Color& operator*=(const Color<T>& c) noexcept {r *= c.r, g *= c.g, b *= c.b; return *this;}
	constexpr Color& operator/=(const Color<T>& c) noexcept {r /= c.r, g /= c.g, b /= c.b; return *this;}
	constexpr Color& operator+=(const T v) noexcept {r += v, g += v, b += v; return *this;}
	constexpr Color& operator-=(const T v) noexcept {r -= v, g -= v, b -= v; return *this;}
	constexpr Color& operator*=(const T v) noexcept {r *= v, g *= v, b *= v; return *this;}
	constexpr Color& operator/=(const T v) noexcept {r /= v, g /= v, b /= v; return *this;}
	constexpr const T& operator[](const int) const noexcept;
	constexpr T& operator[](const int) noexcept;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Color<T> - ELEMENT FUNCTIONS
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
template <class T>
constexpr const T& Color<T>::operator[](const int i) const noexcept
{
	switch (i)
	{
		case 0: return r;
		case 1: return g;
		default: return b;
	}
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
template <class T>
constexpr T& Color<T>::operator[](const int i) noexcept
{
	switch (i)
	{
		case 0: return r;
		case 1: return g;
		default: return b;
	}
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Color<T> - GOBAL FUNCTIONS
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
template <class T>
constexpr Color<T> operator+(Color<T> c) noexcept
{
	return c;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
template <class T>
constexpr Color<T> operator-(Color<T> c) noexcept
{
	return {-c.r, -c.g, -c.b};
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
template <class T>
constexpr Color<T> operator+(Color<T> c1, const Color<T>& c2) noexcept
{
	return c1 += c2;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
template <class T>
constexpr Color<T> operator-(Color<T> c1, const Color<T>& c2) noexcept
{
	return c1 -= c2;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
template <class T>
constexpr Color<T> operator*(Color<T> c1, const Color<T>& c2) noexcept
{
	return c1 *= c2;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
template <class T>
constexpr Color<T> operator/(Color<T> c1, const Color<T>& c2) noexcept
{
	return c1 /= c2;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
template <class T>
constexpr Color<T> operator+(Color<T> c, const T v) noexcept
{
	return c += v;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
template <class T>
constexpr Color<T> operator+(const T v, Color<T> c) noexcept
{
	return c += v;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
template <class T>
constexpr Color<T> operator-(Color<T> c, const T v) noexcept
{
	return c -= v;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
template <class T>
constexpr Color<T> operator-(const T v, const Color<T>& c) noexcept
{
	return {v - c.r, v - c.g, v - c.b};
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
template <class T>
constexpr Color<T> operator*(Color<T> c, const T v) noexcept
{
	return c *= v;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
template <class T>
constexpr Color<T> operator*(const T v, Color<T> c) noexcept
{
	return c *= v;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
template <class T>
constexpr Color<T> operator/(Color<T> c, const T v) noexcept
{
	return c /= v;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
template <class T>
constexpr Color<T> operator/(const T v, const Color<T>& c) noexcept
{
	return {v / c.r, v / c.g, v / c.b};
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
} //namespace ama
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#endif // AMA_COLOR_H
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
