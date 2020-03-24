//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// TrueColorImage.cpp
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Copyright (c) 2020 Adrian Maurer. All rights reserved.
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
CTrueColorImage::CTrueColorImage(const CTrueColorImage& mi) : 
	CImage()
{
	if (const CSize is(mi.getSize()); is.cx > 0 && is.cy > 0)
	{
		if (const int b = mi.GetBPP(); b == 8 || b == 24 || b == 32)
		{
			if (Create(is.cx, is.cy, b))
			{
				const int p = abs(mi.GetPitch());
				for (int j = 0; j < is.cy; ++j)
				{
					const BYTE* const p_rs = reinterpret_cast<const BYTE*>(mi.GetPixelAddress(0, j));
					BYTE* const p_rd = reinterpret_cast<BYTE*>(GetPixelAddress(0, j));
					std::memcpy(p_rd, p_rs, p);
				}
			}
		}
	}
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CTrueColorImage::CTrueColorImage(CTrueColorImage&& mi) noexcept
{
	swap(mi);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CTrueColorImage& CTrueColorImage::operator=(const CTrueColorImage& mi)
{
	CTrueColorImage tmi(mi);
	swap(tmi);
	return *this;
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
	HBITMAP hb = !IsNull() ? Detach() : nullptr;
	HBITMAP nhb = !mi.IsNull() ? mi.Detach() : nullptr;
	if (nhb) Attach(nhb);
	if (hb) mi.Attach(hb);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// CTrueColorImage - ELEMENT FUNCTIONS
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CTrueColorImage::initColorTable()
{
	if (GetBPP() == 8)
	{
		const int s = 256;
		std::array<RGBQUAD, s> ca;
		for (int i = 0; i < s; ++i)
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
		const int s = 256;
		std::array<RGBQUAD, s> ca;
		GetColorTable(0, s, ca.data());
		for (int i = 0; i < s; ++i)
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
void CTrueColorImage::setPixel(const int x, const int y, const TColor<BYTE>& c)
{
	BYTE* const p_p = getPixelAddress(x, y);
	p_p[0] = c.b;
	p_p[1] = c.g;
	p_p[2] = c.r;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CTrueColorImage::setPixel(const int x, const int y, const TAlphaColor<BYTE, BYTE>& c)
{
	BYTE* const p_p = getPixelAddress(x, y);
	p_p[0] = c.b;
	p_p[1] = c.g;
	p_p[2] = c.r;
	p_p[3] = c.alpha;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CTrueColorImage::setPixel(const int x, const int y, const float c)
{
	BYTE* const p_p = getPixelAddress(x, y);
	const float f = 255.0F;
	p_p[0] = roundTo<BYTE>(c * f);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CTrueColorImage::setPixel(const int x, const int y, const TColor<float>& c)
{
	BYTE* const p_p = getPixelAddress(x, y);
	const float f = 255.0F;
	p_p[0] = roundTo<BYTE>(c.b * f);
	p_p[1] = roundTo<BYTE>(c.g * f);
	p_p[2] = roundTo<BYTE>(c.r * f);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CTrueColorImage::setPixel(const int x, const int y, const TAlphaColor<float, float>& c)
{
	BYTE* const p_p = getPixelAddress(x, y);
	const float f = 255.0F;
	p_p[0] = roundTo<BYTE>(c.b * f);
	p_p[1] = roundTo<BYTE>(c.g * f);
	p_p[2] = roundTo<BYTE>(c.r * f);
	p_p[3] = roundTo<BYTE>(c.alpha * f);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CTrueColorImage::setPixel(const int x, const int y, const double c)
{
	BYTE* const p_p = getPixelAddress(x, y);
	const double f = 255.0;
	p_p[0] = roundTo<BYTE>(c * f);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CTrueColorImage::setPixel(const int x, const int y, const TColor<double>& c)
{
	BYTE* const p_p = getPixelAddress(x, y);
	const double f = 255.0;
	p_p[0] = roundTo<BYTE>(c.b * f);
	p_p[1] = roundTo<BYTE>(c.g * f);
	p_p[2] = roundTo<BYTE>(c.r * f);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CTrueColorImage::setPixel(const int x, const int y, const TAlphaColor<double, double>& c)
{
	BYTE* const p_p = getPixelAddress(x, y);
	const double f = 255.0;
	p_p[0] = roundTo<BYTE>(c.b * f);
	p_p[1] = roundTo<BYTE>(c.g * f);
	p_p[2] = roundTo<BYTE>(c.r * f);
	p_p[3] = roundTo<BYTE>(c.alpha * f);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CTrueColorImage::writeFile(std::ofstream& os) const
{
	if (!IsNull())
	{
		IStream* p_s = nullptr;
		if (::CreateStreamOnHGlobal(nullptr, TRUE, &p_s) == S_OK)
		{
			if (Save(p_s, Gdiplus::ImageFormatPNG) == S_OK)
			{
				HGLOBAL hg = nullptr;
				if (::GetHGlobalFromStream(p_s, &hg) == S_OK)
				{
					if (BYTE* const p_b = static_cast<BYTE*>(::GlobalLock(hg)))
					{
						const UINT s = static_cast<UINT>(::GlobalSize(hg));
						os.write(reinterpret_cast<const char*>(&s), sizeof(UINT));
						os.write(reinterpret_cast<const char*>(p_b), sizeof(BYTE) * s);
						::GlobalUnlock(hg);
					}
					::GlobalFree(hg);
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
void CTrueColorImage::readFile(const int, std::ifstream& is)
{
	UINT s = 0;
	is.read(reinterpret_cast<char*>(&s), sizeof(UINT));
	if (s > 0)
	{
		if (HGLOBAL hg = ::GlobalAlloc(GMEM_MOVEABLE, static_cast<SIZE_T>(s)))
		{
			if (BYTE* const p_b = static_cast<BYTE*>(::GlobalLock(hg)))
			{
				is.read(reinterpret_cast<char*>(p_b), sizeof(BYTE) * s);
				::GlobalUnlock(hg);
				IStream* p_s = nullptr;
				if (::CreateStreamOnHGlobal(hg, TRUE, &p_s) == S_OK)
				{
					Load(p_s);
					p_s->Release();
				}
			}
			::GlobalFree(hg);
		}
	}
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
