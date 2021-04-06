#pragma once
#ifdef CPP_EXPORTS
#define SEPIA_API __declspec(dllexport)
#else
#define SEPIA_API __declspec(dllimport)
#endif
extern "C" {  SEPIA_API void  Sepia(RGBApixel**input, int i, int skok, int wiersze); }

