#ifndef __CommonHeader_H__
#define __CommonHeader_H__

#define PI 3.1415926535897932384626433832795f

#define SAFE_DELETE(x) { if(x) delete x; x=0; }

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

#include "glwrapper.h"

#endif //__CommonHeader_H__
