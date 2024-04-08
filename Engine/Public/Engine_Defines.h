#pragma once

#pragma warning (disable : 4005)
#pragma warning (disable : 4251)
#pragma warning (disable : 4828)
#pragma warning (disable : 4819)

#include <d3d9.h>
#include <d3dx9.h>

#include <map>
#include <list>
#include <vector>
#include <algorithm>
#include <string>
#include <assert.h>
#include <iostream>
#include <random>
#include <unordered_map>

#include "Engine_Struct.h"
#include "Engine_Function.h"
#include "Engine_Macro.h"
#include "Engine_Typedef.h"

#include <opencv2/imgcodecs.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>

#ifdef _DEBUG

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>


#ifndef DBG_NEW 

#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
#define new DBG_NEW 

#endif
#endif

using namespace Engine;
using namespace std;
using namespace cv;