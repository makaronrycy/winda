﻿// header.h: plik dołączany dla standardowych systemowych plików dołączanych,
// lub pliki dołączane specyficzne dla projektu
//

#pragma once

#include "targetver.h"
//#define WIN32_LEAN_AND_MEAN             // Wyklucz rzadko używane rzeczy z nagłówków systemu Windows
// Pliki nagłówkowe systemu Windows
#include <windows.h>
// Pliki nagłówkowe środowiska uruchomieniowego języka C
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <gdiplus.h>
#include <vector>
#include <iterator>
#include <stack>
#include <algorithm>
#pragma comment(lib,"gdiplus.lib")
using namespace Gdiplus;


using namespace std;


// Własne pliki nagłówkowe
#include "elevator.h"
