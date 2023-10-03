#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <cassert>
#include <windows.h>
#include <string>
#include <format>

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
std::wstring ComvertString(const std::string& str);

std::string ComvertString(const std::wstring& str);
