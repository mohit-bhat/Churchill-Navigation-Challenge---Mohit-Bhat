// Dll2.cpp : Defines the exported functions for the DLL.
//

#define DLL2_EXPORTS

#include "pch.h"
#include "framework.h"
#include "Dll2.h"

extern "C" {

struct SearchContext {
};


DLL2_API SearchContext* __stdcall create(const Point* points_begin, const Point* points_end) {
    return nullptr;
}

DLL2_API int32_t __stdcall search(SearchContext* sc, const Rect rect, const int32_t count, Point* out_points) {
    return 0;
}

DLL2_API SearchContext* __stdcall destroy(SearchContext* sc) {
    return nullptr;
}

}