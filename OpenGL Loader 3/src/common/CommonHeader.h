#ifndef __CommonHeader_H__
#define __CommonHeader_H__

#define PI 3.1415926535897932384626433832795f

#define SAFE_DELETE(x) { if(x) delete x; x=0; }

#if defined(__ANDROID__)
#include "glwrapperAndroid.h"
#elif defined(__APPLE__)
#include "glwrapper.h"
#elif defined (_WIN32) || defined(WIN32)
#include <windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include "../windows/GL/GLExtensions.h"
#endif

#include <stdio.h>
#include <assert.h>
#include <math.h>
#include "LodePNG/lodepng.h"

#include "Vector.h"
#include "Matrix.h"

#include "Helpers.h"
#include "ShaderBase.h"
#include "FrameBuffer.h"

#include "GameCore.h"
#endif //__CommonHeader_H__
