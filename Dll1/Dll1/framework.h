#pragma once

//#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <windows.h>


#include <stdint.h>

#pragma pack(push, 1)

struct Point
{
	int8_t id;
	int32_t rank;
	float x;
	float y;
};

struct Rect
{
	float lx;
	float ly;
	float hx;
	float hy;
};
#pragma pack(pop)


extern "C" {
	struct SearchContext;
	__declspec(dllexport) SearchContext* __stdcall create(const Point* points_begin, const Point* points_end);
	__declspec(dllexport) int32_t __stdcall search(SearchContext* sc, const Rect rect, const int32_t count, Point* out_points);
	__declspec(dllexport) SearchContext* __stdcall destroy(SearchContext* sc);
}

