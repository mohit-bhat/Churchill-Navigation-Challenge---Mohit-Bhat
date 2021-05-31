// Dll7.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#define DLL7_EXPORTS
#include "Dll7.h"
#include <stdio.h>


using namespace std;


struct SearchContext{
	map<float, Point*> mapped_points;
};

extern "C"{


	/* Load the provided points into an internal data structure. The pointers follow the STL iterator convention, where
	"points_begin" points to the first element, and "points_end" points to one past the last element. The input points are
	only guaranteed to be valid for the duration of the call. Return a pointer to the context that can be used for
	consecutive searches on the data. */
	DLL7_API SearchContext* (__stdcall create)(const Point* points_begin, const Point* points_end){
		SearchContext* sc = new(SearchContext);
		for (Point* i = (Point*)points_begin; i < (Point*)points_end; i += sizeof(Point)) sc->mapped_points.insert(pair<float, Point*>(i->x, i));
		return sc;
	}

	/* Search for "count" points with the smallest ranks inside "rect" and copy them ordered by smallest rank first in
	"out_points". Return the number of points copied. "out_points" points to a buffer owned by the caller that
	can hold "count" number of Points. */
	DLL7_API int32_t(__stdcall search)(SearchContext* sc, const Rect rect, const int32_t count, Point* out_points){
		std::map<float, Point*>::iterator i;
		std::map<int, Point*> rect_points;
		std::map<int, Point*>::iterator j;
		printf("\n\nTest1\n\n");
		for (i = sc->mapped_points.lower_bound(rect.lx); i != sc->mapped_points.upper_bound(rect.hx); i++) {
			printf("wow");
			if ((i->second->y > rect.ly) && (i->second->y < rect.hy))rect_points.insert(pair<int, Point*>(i->second->rank, i->second));
			printf("wowee");
		}
		printf("\n\nTest2\n\n");
		int32_t number_of_points;
		if (rect_points.size() >= count) number_of_points = count;
		else number_of_points = rect_points.size();
		j = rect_points.begin();
		for (int k = 0; k < number_of_points; k++) {
			out_points[k] = *j->second;
			++j;
		}
		printf("\n\nTest3\n\n");
		return number_of_points;
	}

	/* Release the resources associated with the context. Return nullptr if successful, "sc" otherwise. */
	DLL7_API SearchContext* (__stdcall destroy)(SearchContext* sc){
		sc->mapped_points.clear();
		delete sc;
		return sc;
	}

}