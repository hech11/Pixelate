#pragma once

// PCH file that RetroGameFramework uses.



// misc.
#include <iostream>
#include <algorithm>
#include <functional>
#include <stdio.h>
#include <math.h>
#include <cmath>
#include <time.h>
#include <ctime>
#include <chrono>
#include <thread>


// data structures.
#include <string>
#include <sstream>
#include <fstream>
#include <memory>
#include <map>
#include <unordered_map>
#include <array>
#include <vector>
#include <queue>
#include <deque>


// OS Specific.
#ifdef RGF_PLATFORM_WINDOWS
#include <Windows.h>
#endif

// If any header files i created are constantly used then it will be put here.
#include "RetroGF/Log.h"