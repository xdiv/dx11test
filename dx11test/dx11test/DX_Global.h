#pragma once

#ifndef __DX_GLOBAL__
#define __DX_GLOBAL__

#ifdef DEBUGW7

#include <C:\Program Files (x86)\Windows Kits\8.1\Include\shared\dxgi.h>
#include <C:\Program Files (x86)\Windows Kits\8.1\Include\um\d3dcommon.h>
#include <C:\Program Files (x86)\Windows Kits\8.1\Include\um\d3d11.h>
#include <C:\Program Files (x86)\Windows Kits\8.1\Include\um\d3d10_1.h>
#include <C:\Program Files (x86)\Windows Kits\8.1\Include\um\d3d10.h>

#include <d3dx11.h>
#include <d3dx10math.h>
#include <d3dx10.h>
#endif

#ifdef DEBUGW8
#include <dxgi.h>
#include <d3dcommon.h>

#include <d3d11.h>
#include <d3d10_1.h>
#include <d3dx11.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXCollision.h>

#endif

//project settings -> configuation propertes -> c\c++ -> processor -> processor definitions

#endif