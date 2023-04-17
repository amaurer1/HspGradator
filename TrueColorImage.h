//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// TrueColorImage.h
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Copyright (c) 2023 Adrian Maurer. All rights reserved.
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
	double calcSizeRatio() const {return !IsNull() ? GetHeight() / static_cast<double>(GetWidth()) : 1.0;}
	template <class T> T getPixel(const int, const int) const;
	void setPixel(const int, const int, const BYTE);
	void setPixel(const int, const int, const ama::Color<BYTE>&);
	void setPixel(const int, const int, const ama::AlphaColor<BYTE, BYTE>&);
	void setPixel(const int, const int, const float);
	void setPixel(const int, const int, const ama::Color<float>&);
	void setPixel(const int, const int, const ama::AlphaColor<float, float>&);
	void setPixel(const int, const int, const double);
	void setPixel(const int, const int, const ama::Color<double>&);
	void setPixel(const int, const int, const ama::AlphaColor<double, double>&);
	void writeStream(std::ostream&) const;
	void readStream(std::istream&);
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
inline ama::AlphaColor<BYTE, BYTE> CTrueColorImage::getPixel(const int x, const int y) const
{
	const BYTE* const p_p = getPixelAddress(x, y);
	return {{p_p[2], p_p[1], p_p[0]}, p_p[3]};
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
inline ama::AlphaColor<float, float> CTrueColorImage::getPixel(const int x, const int y) const
{
	const BYTE* const p_p = getPixelAddress(x, y);
	constexpr float f = 1.0F / 255.0F;
	return {{p_p[2] * f, p_p[1] * f, p_p[0] * f}, p_p[3] * f};
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
inline ama::AlphaColor<double, double> CTrueColorImage::getPixel(const int x, const int y) const
{
	const BYTE* const p_p = getPixelAddress(x, y);
	constexpr double f = 1.0 / 255.0;
	return {{p_p[2] * f, p_p[1] * f, p_p[0] * f}, p_p[3] * f};
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
