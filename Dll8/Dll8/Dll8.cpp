// Dll8.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#define DLL8_EXPORTS
#include "Dll8.h"



using namespace std;


struct SearchContext{
	Point* arr;
};

extern "C"{

	int maxIndex = 0;

	/* Load the provided points into an internal data structure. The pointers follow the STL iterator convention, where
	"points_begin" points to the first element, and "points_end" points to one past the last element. The input points are
	only guaranteed to be valid for the duration of the call. Return a pointer to the context that can be used for
	consecutive searches on the data. */
	DLL8_API SearchContext* (__stdcall create)(const Point* points_begin, const Point* points_end){
		SearchContext* sc = new(SearchContext);
		sc->arr = (Point *) malloc((int) points_end - (int) points_begin);
		int i;
		for (i = 0; (int) &points_begin[i] < (int) points_end - sizeof(Point); i++) sc->arr[i] = points_begin[i];
		maxIndex = i - 1;
		return sc;
	}

	/* Search for "count" points with the smallest ranks inside "rect" and copy them ordered by smallest rank first in
	"out_points". Return the number of points copied. "out_points" points to a buffer owned by the caller that
	can hold "count" number of Points. */
	DLL8_API int32_t(__stdcall search)(SearchContext* sc, const Rect rect, const int32_t count, Point* out_points){
		Point* tempArr = (Point*) malloc(count * sizeof(Point));
		Point tmp, tmp2;
		tmp.rank = 2147483647;
		int32_t number = 0;
		for (int j = 0; j < count; j++) tempArr[j] = tmp;
		for (int i = 0; i <= maxIndex; i++){
			if (sc->arr[i].x > rect.lx && sc->arr[i].x < rect.hx){
				if (sc->arr[i].y > rect.ly && sc->arr[i].y < rect.hy){
					for (int j = 0; j < count; j++){
						if (sc->arr[i].rank < tempArr[j].rank){
							number++;
							tmp = tempArr[j];
							tempArr[j] = sc->arr[i];
							for (int k = j + 1; k < count; k++){
								if (k = count - 1) tempArr[k] = tmp;
								else{
									tmp2 = tempArr[k];
									tempArr[k] = tmp;
									tmp = tmp2;
								}
								
							}
							break;
						}
					}
				}
			}
		}
		for (int i = 0; i < count; i++) out_points[i] = tempArr[i];
		free(tempArr);
		return number;
	}

	/* Release the resources associated with the context. Return nullptr if successful, "sc" otherwise. */
	DLL8_API SearchContext* (__stdcall destroy)(SearchContext* sc){
		free(sc->arr);
		delete sc;
		return sc;
	}

}