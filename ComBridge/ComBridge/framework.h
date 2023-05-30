#pragma once

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.
// Windows 헤더 파일
#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <string>
#include <memory>
#include <chrono>
#include <functional>
#include <Windows.h>

#include "LogMgr.h"
#include "Util.h"