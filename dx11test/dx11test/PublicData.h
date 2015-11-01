#pragma once

#ifndef __SAFE_DELETE__
#define __SAFE_DELETE__
	#define SAFE_DELETE(p)       { if (p) { delete (p);     (p)=nullptr; } }
	#define SAFE_RELEASE(p)      { if (p) { (p)->Release(); (p)=nullptr; } }
#endif