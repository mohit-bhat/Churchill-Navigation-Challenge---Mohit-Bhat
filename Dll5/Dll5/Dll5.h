#pragma once

#include <WinSDKVer.h>

#define _WIN32_WINNT _WIN32_WINNT_VISTA

#include <SDKDDKVer.h>

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#endif


#include <windows.h>
#include <stdint.h>

extern "C"{

	/* The following structs are packed with no padding. */
#pragma pack(push, 1)

	/* Defines a point in 2D space with some additional attributes like id and rank. */
	struct Point
	{
		int8_t id;
		int32_t rank;
		float x;
		float y;
	};

	/* Defines a rectangle, where a point (x,y) is inside, if x is in [lx, hx] and y is in [ly, hy]. */
	struct Rect
	{
		float lx;
		float ly;
		float hx;
		float hy;
	};
#pragma pack(pop)

	/* Declaration of the struct that is used as the context for the calls. */
	struct SearchContext{
		int x;
	};

	/* Load the provided points into an internal data structure. The pointers follow the STL iterator convention, where
	"points_begin" points to the first element, and "points_end" points to one past the last element. The input points are
	only guaranteed to be valid for the duration of the call. Return a pointer to the context that can be used for
	consecutive searches on the data. */
	__declspec(dllexport) SearchContext* (__stdcall create)(const Point* points_begin, const Point* points_end);

	/* Search for "count" points with the smallest ranks inside "rect" and copy them ordered by smallest rank first in
	"out_points". Return the number of points copied. "out_points" points to a buffer owned by the caller that
	can hold "count" number of Points. */
	__declspec(dllexport) int32_t(__stdcall search)(SearchContext* sc, const Rect rect, const int32_t count, Point* out_points);

	/* Release the resources associated with the context. Return nullptr if successful, "sc" otherwise. */
	__declspec(dllexport) SearchContext* (__stdcall destroy)(SearchContext* sc);

}