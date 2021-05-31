// The following ifdef block is the standard way of creating macros which make exporting
// from a DLL simpler. All files within this DLL are compiled with the DLL2_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see
// DLL2_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef DLL2_EXPORTS
#define DLL2_API __declspec(dllexport)
#else
#define DLL2_API __declspec(dllimport)
#endif



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

extern "C"{

struct SearchContext;


DLL2_API SearchContext* __stdcall create(const Point* points_begin, const Point* points_end);

DLL2_API int32_t __stdcall search(SearchContext* sc, const Rect rect, const int32_t count, Point* out_points);

DLL2_API SearchContext* __stdcall destroy(SearchContext* sc);

}
