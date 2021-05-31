// Dll5.cpp : Defines the exported functions for the DLL application.
//

#include "Dll5.h"

extern "C"{

	/* Load the provided points into an internal data structure. The pointers follow the STL iterator convention, where
	"points_begin" points to the first element, and "points_end" points to one past the last element. The input points are
	only guaranteed to be valid for the duration of the call. Return a pointer to the context that can be used for
	consecutive searches on the data. */
	__declspec(dllexport) SearchContext* (__stdcall create)(const Point* points_begin, const Point* points_end){
		SearchContext * init = new SearchContext;
		return init;
	}

	/* Search for "count" points with the smallest ranks inside "rect" and copy them ordered by smallest rank first in
	"out_points". Return the number of points copied. "out_points" points to a buffer owned by the caller that
	can hold "count" number of Points. */
	__declspec(dllexport) int32_t(__stdcall search)(SearchContext* sc, const Rect rect, const int32_t count, Point* out_points){
		return count;
	}

	/* Release the resources associated with the context. Return nullptr if successful, "sc" otherwise. */
	__declspec(dllexport) SearchContext* (__stdcall destroy)(SearchContext* sc){
		delete sc;
		return sc;
	}

}