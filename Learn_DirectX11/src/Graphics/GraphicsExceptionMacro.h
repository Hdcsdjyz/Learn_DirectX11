/**
 * @file GraphicsExceptionMacro.h
 * @author Lhxl
 * @date 2025-2-7
 * @version build9
 */

#ifndef GRAPHICSEXCEPTIONMACRO_H
#define GRAPHICSEXCEPTIONMACRO_H

#define GFX_EXCEPT_NOINFO(hr) Graphics::HrException(__LINE__, __FILEW__, (hr))
#define GFX_THROW_NOINFO(hrcall) if (FAILED(hr = (hrcall))) throw Graphics::HrException(__LINE__, __FILEW__, hr)
#ifndef NDEBUG
#define GFX_EXCEPT(hr) Graphics::HrException(__LINE__, __FILEW__, (hr), _infoManager.GetMessages())
#define GFX_THROW_INFO(hrcall) _infoManager.Set(); if (FAILED(hr = (hrcall))) throw GFX_EXCEPT(hr)
#define GFX_DEVICE_EXCEPT(hr) Graphics::DeviceException(__LINE__, __FILEW__, (hr), _infoManager.GetMessages())
#define GFX_THROW_INFO_ONLY(call) _infoManager.Set(); (call); {auto v = _infoManager.GetMessages(); if (!v.empty()) {throw Graphics::InfoException(__LINE__, __FILEW__, v);}}
#else
#define GFX_EXCEPT(hr) Graphics::HrException(__LINE__, __FILEW__, (hr))
#define	GFX_THROW_INFO(hrcall) GFX_THROW_NOINFO(hrcall)
#define	GFX_DEVICE_EXCEPT(hr) Graphics::DeviceException(__LINE__, __FILEW__, (hr))
#define GFX_THROW_INFO_ONLY(call) (call)
#endif

#ifdef NDEBUG
#define INFOMAN(gfx) HRESULT hr
#else
#define INFOMAN(gfx) HRESULT hr; DxgiInfoManager& infoManager = GetInfoManager((gfx))
#endif

#ifdef NDEBUG
#define INFOMAN_NOHR(gfx)
#else
#define INFOMAN_NOHR(gfx) DxgiInfoManager& infoManager = GetInfoManager((gfx))
#endif

#endif