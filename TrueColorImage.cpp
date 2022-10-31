//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// TrueColorImage.cpp
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Copyright (c) 2022 Adrian Maurer. All rights reserved.
// Distributed under the MIT software license (http://www.opensource.org/licenses/mit-license.php).
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "pch.h"
#include "framework.h"
#include "TrueColorImage.h"
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// CTrueColorImage
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// CTrueColorImage - CONSTRUCTION / DESTRUCTION / ASSIGNMENT
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CTrueColorImage::CTrueColorImage() : 
	CImage()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CTrueColorImage::CTrueColorImage(const CTrueColorImage& tci) : 
	CImage()
{
	if (const CSize s(tci.getSize()); s.cx > 0 && s.cy > 0)
	{
		if (const int b = tci.GetBPP(); b == 8 || b == 24 || b == 32)
		{
			if (Create(s.cx, s.cy, b))
			{
				const int p = abs(tci.GetPitch());
				for (int j = 0; j < s.cy; ++j)
				{
					const BYTE* const p_rs = static_cast<const BYTE*>(tci.GetPixelAddress(0, j));
					BYTE* const p_rd = static_cast<BYTE*>(GetPixelAddress(0, j));
					std::memcpy(p_rd, p_rs, p);
				}
			}
		}
	}
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CTrueColorImage& CTrueColorImage::operator=(const CTrueColorImage& mi)
{
	CTrueColorImage tmi(mi);
	swap(tmi);
	return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CTrueColorImage::CTrueColorImage(CTrueColorImage&& mi) noexcept
{
	swap(mi);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CTrueColorImage& CTrueColorImage::operator=(CTrueColorImage&& mi) noexcept
{
	swap(mi);
	return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CTrueColorImage::swap(CTrueColorImage& mi) noexcept
{
	HBITMAP h_b1 = !IsNull() ? Detach() : nullptr;
	HBITMAP h_b2 = !mi.IsNull() ? mi.Detach() : nullptr;
	if (h_b2) Attach(h_b2);
	if (h_b1) mi.Attach(h_b1);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// CTrueColorImage - ELEMENT FUNCTIONS
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CTrueColorImage::initColorTable()
{
	if (GetBPP() == 8)
	{
		constexpr unsigned int s = 256;
		std::array<RGBQUAD, s> ca{};
		for (unsigned int i = 0; i < s; ++i)
		{
			ca[i].rgbRed = ca[i].rgbGreen = ca[i].rgbBlue = static_cast<BYTE>(i);
		}
		SetColorTable(0, s, ca.data());
	}
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool CTrueColorImage::checkColorTable() const
{
	if (GetBPP() == 8)
	{
		constexpr unsigned int s = 256;
		std::array<RGBQUAD, s> ca{};
		GetColorTable(0, s, ca.data());
		for (unsigned int i = 0; i < s; ++i)
		{
			if (ca[i].rgbRed != i || ca[i].rgbGreen != i || ca[i].rgbBlue != i) return false;
		}
		return true;
	}
	return false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
BOOL CTrueColorImage::create(const CSize& s, const int b)
{
	if (!IsNull() && (s.cx != GetWidth() || s.cy != GetHeight() || b != GetBPP()))
	{
		Destroy();
	}
	if (IsNull())
	{
		if (b == 8)
		{
			if (!Create(s.cx, s.cy, 8)) return FALSE;
			initColorTable();
		}
		else if (b == 24)
		{
			if (!Create(s.cx, s.cy, 24)) return FALSE;
		}
		else if (b == 32)
		{
			if (!Create(s.cx, s.cy, 32, createAlphaChannel)) return FALSE;
		}
	}
	return TRUE;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int CTrueColorImage::load(LPCTSTR pn)
{
	if (Load(pn) == S_OK)
	{
		if (GetBPP() == 8)
		{
			if (checkColorTable()) return 8;
		}
		else if (GetBPP() == 24)
		{
			return 24;
		}
		else if (GetBPP() == 32)
		{
			return 32;
		}
		Destroy();
		return 0;
	}
	return -1;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CTrueColorImage::setPixel(const int x, const int y, const BYTE c)
{
	BYTE* const p_p = getPixelAddress(x, y);
	p_p[0] = c;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CTrueColorImage::setPixel(const int x, const int y, const ama::Color<BYTE>& c)
{
	BYTE* const p_p = getPixelAddress(x, y);
	p_p[0] = c.b;
	p_p[1] = c.g;
	p_p[2] = c.r;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CTrueColorImage::setPixel(const int x, const int y, const ama::AlphaColor<BYTE, BYTE>& c)
{
	BYTE* const p_p = getPixelAddress(x, y);
	p_p[0] = c.color.b;
	p_p[1] = c.color.g;
	p_p[2] = c.color.r;
	p_p[3] = c.alpha;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CTrueColorImage::setPixel(const int x, const int y, const float c)
{
	BYTE* const p_p = getPixelAddress(x, y);
	constexpr float f = 255.0F;
	p_p[0] = ama::round<BYTE>(c * f);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CTrueColorImage::setPixel(const int x, const int y, const ama::Color<float>& c)
{
	BYTE* const p_p = getPixelAddress(x, y);
	constexpr float f = 255.0F;
	p_p[0] = ama::round<BYTE>(c.b * f);
	p_p[1] = ama::round<BYTE>(c.g * f);
	p_p[2] = ama::round<BYTE>(c.r * f);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CTrueColorImage::setPixel(const int x, const int y, const ama::AlphaColor<float, float>& c)
{
	BYTE* const p_p = getPixelAddress(x, y);
	constexpr float f = 255.0F;
	p_p[0] = ama::round<BYTE>(c.color.b * f);
	p_p[1] = ama::round<BYTE>(c.color.g * f);
	p_p[2] = ama::round<BYTE>(c.color.r * f);
	p_p[3] = ama::round<BYTE>(c.alpha * f);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CTrueColorImage::setPixel(const int x, const int y, const double c)
{
	BYTE* const p_p = getPixelAddress(x, y);
	constexpr double f = 255.0;
	p_p[0] = ama::round<BYTE>(c * f);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CTrueColorImage::setPixel(const int x, const int y, const ama::Color<double>& c)
{
	BYTE* const p_p = getPixelAddress(x, y);
	constexpr double f = 255.0;
	p_p[0] = ama::round<BYTE>(c.b * f);
	p_p[1] = ama::round<BYTE>(c.g * f);
	p_p[2] = ama::round<BYTE>(c.r * f);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CTrueColorImage::setPixel(const int x, const int y, const ama::AlphaColor<double, double>& c)
{
	BYTE* const p_p = getPixelAddress(x, y);
	constexpr double f = 255.0;
	p_p[0] = ama::round<BYTE>(c.color.b * f);
	p_p[1] = ama::round<BYTE>(c.color.g * f);
	p_p[2] = ama::round<BYTE>(c.color.r * f);
	p_p[3] = ama::round<BYTE>(c.alpha * f);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CTrueColorImage::writeStream(std::ostream& os) const
{
	if (!IsNull())
	{
		if (IStream* p_s{}; ::CreateStreamOnHGlobal(nullptr, TRUE, &p_s) == S_OK)
		{
			if (Save(p_s, Gdiplus::ImageFormatPNG) == S_OK)
			{
				if (HGLOBAL h_g{}; ::GetHGlobalFromStream(p_s, &h_g) == S_OK)
				{
					if (const BYTE* const p_b = static_cast<BYTE*>(::GlobalLock(h_g)))
					{
						const UINT s = static_cast<UINT>(::GlobalSize(h_g));
						os.write(reinterpret_cast<const char*>(&s), sizeof(UINT));
						os.write(reinterpret_cast<const char*>(p_b), sizeof(BYTE) * s);
						::GlobalUnlock(h_g);
					}
					::GlobalFree(h_g);
				}
			}
			p_s->Release();
		}
	}
	else
	{
		const UINT s = 0;
		os.write(reinterpret_cast<const char*>(&s), sizeof(UINT));
	}
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CTrueColorImage::readStream(std::istream& is)
{
	UINT s = 0;
	is.read(reinterpret_cast<char*>(&s), sizeof(UINT));
	if (s > 0)
	{
		if (HGLOBAL h_g = ::GlobalAlloc(GMEM_MOVEABLE, static_cast<SIZE_T>(s)))
		{
			if (BYTE* const p_b = static_cast<BYTE*>(::GlobalLock(h_g)))
			{
				is.read(reinterpret_cast<char*>(p_b), sizeof(BYTE) * s);
				::GlobalUnlock(h_g);
				if (IStream* p_s{}; ::CreateStreamOnHGlobal(h_g, TRUE, &p_s) == S_OK)
				{
					Load(p_s);
					p_s->Release();
				}
			}
			::GlobalFree(h_g);
		}
	}
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
