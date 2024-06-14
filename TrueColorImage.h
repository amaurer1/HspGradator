//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// TrueColorImage.h
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Copyright (c) 2019-2024 Adrian Maurer. All rights reserved.
// Distributed under the MIT software license (http://www.opensource.org/licenses/mit-license.php).
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#pragma once
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include <fstream>
#include <array>
#include <atlimage.h>
#include "AlphaColor.h"
#include "Function.h"
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// CTrueColorImage
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class CTrueColorImage : public CImage
{
// CONSTRUCTION / DESTRUCTION / ASSIGNMENT
public:
	CTrueColorImage();
	CTrueColorImage(const CTrueColorImage&);
	CTrueColorImage(CTrueColorImage&&) noexcept;
	~CTrueColorImage() = default;
	CTrueColorImage& operator=(const CTrueColorImage&);
	CTrueColorImage& operator=(CTrueColorImage&&) noexcept;
	void swap(CTrueColorImage&) noexcept;

// ELEMENT FUNCTIONS
private:
	void initColorTable();
	bool checkColorTable() const;
	BYTE* getPixelAddress(const int, const int);
	const BYTE* getPixelAddress(const int, const int) const;
public:
	int create(const CSize&, const int);
	int load(LPCTSTR);
	CSize getSize() const {return !IsNull() ? CSize(GetWidth(), GetHeight()) : CSize(0, 0);}
	double calcAspectRatio() const {return !IsNull() ? static_cast<double>(GetWidth()) / GetHeight() : 1.0;}
	template <class T> T getPixel(const int, const int) const;
	template <class T> void setPixel(const int, const int, const T&);
	void readStream(std::istream&);
	void writeStream(std::ostream&) const;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// CTrueColorImage - ELEMENT FUNCTIONS
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
inline BYTE* CTrueColorImage::getPixelAddress(const int x, const int y)
{
	return static_cast<BYTE*>(GetPixelAddress(x, GetHeight() - 1 - y));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
inline const BYTE* CTrueColorImage::getPixelAddress(const int x, const int y) const
{
	return static_cast<const BYTE*>(GetPixelAddress(x, GetHeight() - 1 - y));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
template <>
inline BYTE CTrueColorImage::getPixel(const int x, const int y) const
{
	const BYTE* const p_p = getPixelAddress(x, y);
	return p_p[0];
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
template <>
inline ama::Color<BYTE> CTrueColorImage::getPixel(const int x, const int y) const
{
	const BYTE* const p_p = getPixelAddress(x, y);
	return {p_p[2], p_p[1], p_p[0]};
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
template <>
inline ama::AlphaColor<BYTE> CTrueColorImage::getPixel(const int x, const int y) const
{
	const BYTE* const p_p = getPixelAddress(x, y);
	return {p_p[3], {p_p[2], p_p[1], p_p[0]}};
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
template <>
inline float CTrueColorImage::getPixel(const int x, const int y) const
{
	const BYTE* const p_p = getPixelAddress(x, y);
	constexpr float f = 1.0F / 255.0F;
	return p_p[0] * f;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
template <>
inline ama::Color<float> CTrueColorImage::getPixel(const int x, const int y) const
{
	const BYTE* const p_p = getPixelAddress(x, y);
	constexpr float f = 1.0F / 255.0F;
	return {p_p[2] * f, p_p[1] * f, p_p[0] * f};
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
template <>
inline ama::AlphaColor<float> CTrueColorImage::getPixel(const int x, const int y) const
{
	const BYTE* const p_p = getPixelAddress(x, y);
	constexpr float f = 1.0F / 255.0F;
	return {p_p[3] * f, {p_p[2] * f, p_p[1] * f, p_p[0] * f}};
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
template <>
inline double CTrueColorImage::getPixel(const int x, const int y) const
{
	const BYTE* const p_p = getPixelAddress(x, y);
	constexpr double f = 1.0 / 255.0;
	return p_p[0] * f;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
template <>
inline ama::Color<double> CTrueColorImage::getPixel(const int x, const int y) const
{
	const BYTE* const p_p = getPixelAddress(x, y);
	constexpr double f = 1.0 / 255.0;
	return {p_p[2] * f, p_p[1] * f, p_p[0] * f};
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
template <>
inline ama::AlphaColor<double> CTrueColorImage::getPixel(const int x, const int y) const
{
	const BYTE* const p_p = getPixelAddress(x, y);
	constexpr double f = 1.0 / 255.0;
	return {p_p[3] * f, {p_p[2] * f, p_p[1] * f, p_p[0] * f}};
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
template <>
inline void CTrueColorImage::setPixel(const int x, const int y, const BYTE& c)
{
	BYTE* const p_p = getPixelAddress(x, y);
	p_p[0] = c;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
template <>
inline void CTrueColorImage::setPixel(const int x, const int y, const ama::Color<BYTE>& c)
{
	BYTE* const p_p = getPixelAddress(x, y);
	p_p[0] = c.b;
	p_p[1] = c.g;
	p_p[2] = c.r;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
template <>
inline void CTrueColorImage::setPixel(const int x, const int y, const ama::AlphaColor<BYTE>& c)
{
	BYTE* const p_p = getPixelAddress(x, y);
	p_p[0] = c.color.b;
	p_p[1] = c.color.g;
	p_p[2] = c.color.r;
	p_p[3] = c.alpha;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
template <>
inline void CTrueColorImage::setPixel(const int x, const int y, const float& c)
{
	BYTE* const p_p = getPixelAddress(x, y);
	constexpr float f = 255.0F;
	p_p[0] = ama::round<BYTE>(c * f);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
template <>
inline void CTrueColorImage::setPixel(const int x, const int y, const ama::Color<float>& c)
{
	BYTE* const p_p = getPixelAddress(x, y);
	constexpr float f = 255.0F;
	p_p[0] = ama::round<BYTE>(c.b * f);
	p_p[1] = ama::round<BYTE>(c.g * f);
	p_p[2] = ama::round<BYTE>(c.r * f);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
template <>
inline void CTrueColorImage::setPixel(const int x, const int y, const ama::AlphaColor<float>& c)
{
	BYTE* const p_p = getPixelAddress(x, y);
	constexpr float f = 255.0F;
	p_p[0] = ama::round<BYTE>(c.color.b * f);
	p_p[1] = ama::round<BYTE>(c.color.g * f);
	p_p[2] = ama::round<BYTE>(c.color.r * f);
	p_p[3] = ama::round<BYTE>(c.alpha * f);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
template <>
inline void CTrueColorImage::setPixel(const int x, const int y, const double& c)
{
	BYTE* const p_p = getPixelAddress(x, y);
	constexpr double f = 255.0;
	p_p[0] = ama::round<BYTE>(c * f);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
template <>
inline void CTrueColorImage::setPixel(const int x, const int y, const ama::Color<double>& c)
{
	BYTE* const p_p = getPixelAddress(x, y);
	constexpr double f = 255.0;
	p_p[0] = ama::round<BYTE>(c.b * f);
	p_p[1] = ama::round<BYTE>(c.g * f);
	p_p[2] = ama::round<BYTE>(c.r * f);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
template <>
inline void CTrueColorImage::setPixel(const int x, const int y, const ama::AlphaColor<double>& c)
{
	BYTE* const p_p = getPixelAddress(x, y);
	constexpr double f = 255.0;
	p_p[0] = ama::round<BYTE>(c.color.b * f);
	p_p[1] = ama::round<BYTE>(c.color.g * f);
	p_p[2] = ama::round<BYTE>(c.color.r * f);
	p_p[3] = ama::round<BYTE>(c.alpha * f);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
