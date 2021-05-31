// Dll12.cpp : 

//

#include "stdafx.h"
#include "Dll12.h"




struct SearchContext{
	Point* arr;
	int numEntries;
};

void pointPrinter(const Point * p){
	if (!p) return;
	printf("\nPoint %i; rank: %i  x:%f   y:%f", p->id, p->rank, p->x, p->y);
}

int noPoints;

void swap(Point *a, Point *b) {
	Point t = *a;
	*a = *b;
	*b = t;
}

void heapify(Point arr[], int n, int i)
{
	int largest = i; // Initialize largest as root
	int l = 2 * i + 1; // left = 2*i + 1
	int r = 2 * i + 2; // right = 2*i + 2

	// If left child is larger than root
	if (l < n && arr[l].rank > arr[largest].rank)
		largest = l;

	// If right child is larger than largest so far
	if (r < n && arr[r].rank > arr[largest].rank)
		largest = r;

	// If largest is not root
	if (largest != i) {
		swap(&arr[i], &arr[largest]);

		// Recursively heapify the affected sub-tree
		heapify(arr, n, largest);
	}
}

// main function to do heap sort
void heapSort(Point arr[], int n)
{
	// Build heap (rearrange array)
	for (int i = n / 2 - 1; i >= 0; i--)
		heapify(arr, n, i);

	// One by one extract an element from heap
	for (int i = n - 1; i > 0; i--) {
		// Move current root to end
		swap(&arr[0], &arr[i]);

		// call max heapify on the reduced heap
		heapify(arr, i, 0);
	}
}

int numInsert;

extern "C"{

	DLL12_API SearchContext* (__stdcall create)(const Point* points_begin, const Point* points_end){
		if (points_begin == NULL || points_end == NULL) return nullptr;

		SearchContext * sc = (SearchContext *)malloc(sizeof(SearchContext));
		Point * temp = (Point*)points_begin;
		noPoints = 0;
		while (temp < (Point*)points_end){
			temp++;
			noPoints++;
		}
		sc->arr = (Point *)malloc((noPoints + 1) * sizeof(Point));
		temp = (Point *)points_begin;
		sc->numEntries = noPoints;
		for (int i = 0; i < noPoints; i++){
			sc->arr[i] = points_begin[i];
		}
		heapSort(sc->arr, noPoints);
		//printf("\nLoading complete: %i Points, start at %p, end at %p", noPoints, points_begin, points_end);

		return sc;
	}


	DLL12_API int32_t(__stdcall search)(SearchContext* sc, const Rect rect, const int32_t count, Point* out_points){
		if (!sc || !out_points) return 0;
		//printf("\nSearch %i points in [%f,%f]x and [%f,%f]y \n", count, rect.lx, rect.hx, rect.ly, rect.hx);

		//printf("Test1");
		for (int i = 0; i < count; i++){
			out_points[i].rank = INT32_MAX;
			out_points[i].id = INT8_MAX;
			out_points[i].x = 0;
			out_points[i].y = 0;
		}
		//printf(" Test2, %i ", sc->numEntries);
		numInsert = 0;
		for (int i = 0; i < sc->numEntries; i++){
			//printf("Test_A");
			if (sc->arr[i].x >= rect.lx) {
				if (sc->arr[i].x <= rect.hx) {
					if (sc->arr[i].y >= rect.ly) {
						if (sc->arr[i].y <= rect.hy){
							//printf("Test_B");
							//printf("; %i %f %f", sc->arr[i].rank, sc->arr[i].x, sc->arr[i].y);
							out_points[numInsert] = sc->arr[i];
							numInsert++;
							//printf("Test_C");
						}
					}
				}
			}
			if (numInsert >= count) break;
			//printf("Test_D");
		}
		//printf("\nPoints: ");

		for (int i = 0; i < count; i++){
			///pointPrinter(&out_points[i]);
		}
		//printf("\nNumber of Points Inserted: %i", numInsert);
		if (numInsert > count) numInsert = count;
		return numInsert;
	}


	DLL12_API SearchContext* (__stdcall destroy)(SearchContext* sc){
		if (!sc) return sc;
		free(sc->arr);
		free(sc);
		sc = nullptr;
		return sc;
	}
}