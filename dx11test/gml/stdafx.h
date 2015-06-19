// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#ifdef GML_EXPORTS
#define GML_API __declspec(dllexport)
#define EXPIMP_TEMPLATE
#else
#define GML_API __declspec(dllimport)
#define EXPIMP_TEMPLATE extern
#endif

#ifndef __SAFE_DELETE__
#define __SAFE_DELETE__
#define SAFE_DELETE(p)       { if (p) { delete (p);     (p)=NULL; } }
#define SAFE_RELEASE(p)      { if (p) { (p)->Release(); (p)=NULL; } }
#endif

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>
#include <assert.h>
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <vector>
#include <stdio.h>
#include <tchar.h>

using namespace std;

// TODO: reference additional headers your program requires here
