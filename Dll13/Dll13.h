// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the DLL13_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// DLL13_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef DLL13_EXPORTS
#define DLL13_API __declspec(dllexport)
#else
#define DLL13_API __declspec(dllimport)
#endif


#include <stdint.h>
#include <stdlib.h>



/* The following structs are packed with no padding. */
#pragma pack(push, 1)

/* Defines a point in 2D space with some additional attributes like id and rank. */

struct Point{
	int8_t id;
	int32_t rank;
	float x;
	float y;
};
/* Defines a rectangle, where a point (x,y) is inside, if x is in [lx, hx] and y is in [ly, hy]. */
struct Rect{
	float lx;
	float ly;
	float hx;
	float hy;
};

struct Node{
	Node *left, *right;
	Point p;
};

//Search context structure, holds data used for consecutive searches
//packing structure as with the rest to improve space
struct SearchContext{
	Node * root;
	int numPoints;
	Rect range;
};
#pragma pack(pop)

//Swaps point a and b
void swap(Point *a, Point *b);

//heapsort helper function
void heapify(Point arr[], int n, int i, int8_t dim);

//heapsort, sorts Point array by rank
void heapSort(Point arr[], int n, int8_t dim);

extern "C"{
	/* Load the provided points into an internal data structure. The pointers follow the STL iterator convention, where
	"points_begin" points to the first element, and "points_end" points to one past the last element. The input points are
	only guaranteed to be valid for the duration of the call. Return a pointer to the context that can be used for
	consecutive searches on the data. */
	DLL13_API SearchContext* (__stdcall create)(const Point* points_begin, const Point* points_end);

	/* Search for "count" points with the smallest ranks inside "rect" and copy them ordered by smallest rank first in
	"out_points". Return the number of points copied. "out_points" points to a buffer owned by the caller that
	can hold "count" number of Points.*/
	DLL13_API int32_t(__stdcall search)(SearchContext* sc, const Rect rect, const int32_t count, Point* out_points);

	/* Release the resources associated with the context. Return nullptr if successful, "sc" otherwise. */
	DLL13_API SearchContext* (__stdcall destroy)(SearchContext* sc);

}