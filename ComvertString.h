#pragma once

#include<Windows.h>
#include<string>
#include<format>


std::wstring ComvertString(const std::string& str);


std::string ComvertString(const std::wstring& str);

void Log(const std::string& message);

void CompliteLog();