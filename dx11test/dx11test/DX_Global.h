#pragma once

#ifndef __DX_GLOBAL__
#define __DX_GLOBAL__

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers.
#endif

//#define _XM_NO_INTRINSICS_

#include <windows.h>
#include <wrl.h>
#include <wrl/client.h>

#ifdef DEBUGW7

		#include <C:\Program Files (x86)\Windows Kits\8.1\Include\shared\dxgi.h>
		#include <C:\Program Files (x86)\Windows Kits\8.0\Include\um\d3dcommon.h>
		#include <C:\Program Files (x86)\Windows Kits\8.0\Include\um\d3d11.h>
		#include <C:\Program Files (x86)\Windows Kits\8.0\Include\um\d3d10_1.h>
		#include <C:\Program Files (x86)\Windows Kits\8.0\Include\um\d3d10.h>

		#include <C:\Program Files (x86)\Windows Kits\8.0\Include\um\d3dx11.h>
		#include <d3dx10math.h>
		#include <d3dx10.h>

#endif

#if defined(DEBUGW8)

	#include <dxgi.h>
	#include <dxgi1_3.h>
	#include <d3dcommon.h>

	#include <d3d11.h>
	#include <d3d10.h>
	#include <DirectXMath.h>
	#include <DirectXPackedVector.h>
	#include <DirectXCollision.h>
	#include <d3dcompiler.h>

#endif

#if defined(DEBUGW10)
	#include <d3d12.h>
	#include <d3d11.h>
	#include <d3d11_1.h>
	#include <d3d11_2.h>
	#include <d3d11_3.h>
	#include <d3d10.h>
	#include <d2d1_2.h>
	#include <d2d1effects_1.h>
	#include <dxgi1_4.h>
	#include <d3dcommon.h>
	#include <D3Dcompiler.h>
	#include <DirectXMath.h>
	#include <DirectXPackedVector.h>
	#include <DirectXColors.h>
	#include <DirectXCollision.h>
	//#include <c:\Program Files (x86)\Windows Kits\10\Include\10.0.10240.0\um\dxg>
//D3DDECLTYPE, D3DFORMAT, and DXGI_FORMAT 

#endif

//#define CDXML_EXPORTS

#include "../cdxml/complex_types.h"
#include "../cdxml/math.h"
#include "../includes/tinyxml2/tinyxml2.h"

#include <string>
#include <wrl.h>
#include <dwrite_2.h>
#include <wincodec.h>
#include <comdef.h>
#include <memory>
//#include <agile.h>
#include <concrt.h>

#define GML_EXPORTS
#define CDXML_EXPORTS
//project settings -> configuation propertes -> c\c++ -> processor -> processor definitions


#define SAFE_DELETE(p)       { if (p) { delete (p);     (p)=nullptr; } }
#define SAFE_RELEASE(p)      { if (p) { (p)->Release(); (p)=nullptr; } }

inline LPCWSTR HresultToString(HRESULT hr)
{
	_com_error err(hr); 
	LPCTSTR errMsg = err.ErrorMessage();
	return errMsg;
}

#endif