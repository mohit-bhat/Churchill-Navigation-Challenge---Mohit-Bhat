// Dll13.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"

#define DIVISIONS 10


Node * KDTree(Point * arr, int depth, int begin, int end){
	//printf("\nKD");
	if (begin == end){
		Node * temp = (Node *)malloc(sizeof(Node));
		temp->p = arr[begin];
		temp->left = temp->right = NULL;
		return temp;
	}
	//printf("1");
	int8_t dim = depth % 2;
	auto length = end - begin;
	//printf("2");
	if (length > 1) heapSort(&arr[begin], end, dim);

	int middle = begin + (length / 2);
	//printf("3");
	auto l_begin = begin;
	auto l_end = middle;
	auto r_begin = middle + 1;
	auto r_end = end;
	//printf("4");
	size_t l_len = length / 2;
	size_t r_len = length - l_len - 1;
	//pointPrinter(&arr[begin]); printf("<-Begin");
	//pointPrinter(&arr[middle]); printf("<-Middle");
	//pointPrinter(&arr[end]); printf("<-End");
	Node * left, *right;
	//printf(" %i, ", num++);
	if (l_len > 0) {
		left = KDTree(arr, depth + 1, l_begin, l_end);
	}
	else {
		left = NULL;
	}
	if (r_len > 0) {
		right = KDTree(arr, depth + 1, r_begin, r_end);
	}
	else {
		right = NULL;
	}
	Node * temp = (Node *)malloc(sizeof(Node));
	temp->p = arr[middle];
	temp->left = left;
	temp->right = right;
	//printf(" %i ", temp->p.rank);
	return temp;
}

//External linkage for functions exporeted by DLL
extern "C"{

	DLL13_API SearchContext* (__stdcall create)(const Point* points_begin, const Point* points_end){
		if (points_begin == NULL || points_end == NULL) return nullptr;			// Checking for valid input
		SearchContext * sc = (SearchContext *)malloc(sizeof(SearchContext));	// Allocating search context
		Point * temp = (Point*)points_begin;
		sc->range.hx = sc->range.hy = sc->range.lx = sc->range.ly = 0;
		sc->numPoints = 0;
		while (temp < (Point*)points_end){										// Iterating through array to find number of entries
			if (temp->x < sc->range.lx) sc->range.lx = temp->x;
			if (temp->y < sc->range.ly) sc->range.ly = temp->y;
			if (temp->x > sc->range.hx) sc->range.hx = temp->x;
			if (temp->y > sc->range.hy) sc->range.hy = temp->y;
			temp++;
			sc->numPoints++;
		}


		return sc;
	}


	DLL13_API int32_t(__stdcall search)(SearchContext* sc, const Rect rect, const int32_t count, Point* out_points){
		if (!sc || !out_points) return 0;										// Checking for valid input
		int numInsert = 0;														// Integer to sort number of entries inserted into the output array
		for (int i = 0; i < sc->numEntries; i++){								// Loops through search context array
			if (sc->arr[i].x >= rect.lx) {
				if (sc->arr[i].x <= rect.hx) {									// Checks if point is in the given rectangle
					if (sc->arr[i].y >= rect.ly) {
						if (sc->arr[i].y <= rect.hy){
							out_points[numInsert] = sc->arr[i];					// Inserts Point in output array at next availible location
							numInsert++;										// Increment insertions
						}														// Since the search context array is sorted by rank, points are found in order
					}
				}
			}
			if (numInsert >= count) break;										// Break loop once count points are inserted into output array
		}
		return numInsert;														// Return number of insertions
	}


	DLL13_API SearchContext* (__stdcall destroy)(SearchContext* sc){
		if (!sc) return sc;														// Checks for invalid input
		free(sc->arr);															// Frees array of pointers in search context
		free(sc);																// Frees the search context
		sc = nullptr;															// Clears pointer
		return sc;																// Returns pointer
	}
}


// Standard heapsort implimentations for array of Points. Sorts by rank in ascending order

// swap two points a and b
void swap(Point *a, Point *b) {
	Point t = *a;
	*a = *b;
	*b = t;
}

// heapsort helper function. Point array arr[] of size int n is used with root at index i
void heapify(Point arr[], int n, int i, int8_t dim){
	int largest = i;
	int l = 2 * i + 1;
	int r = 2 * i + 2;
	switch (dim){
		case 0:
			if (l < n && arr[l].x > arr[largest].x)
				largest = l;
			if (r < n && arr[r].x > arr[largest].x)
				largest = r;
			break;
		case 1:
			if (l < n && arr[l].y > arr[largest].y)
				largest = l;
			if (r < n && arr[r].y > arr[largest].y)
				largest = r;
			break;
		default:
			if (l < n && arr[l].rank > arr[largest].rank)
				largest = l;
			if (r < n && arr[r].rank > arr[largest].rank)
				largest = r;
			break;
	}
	if (largest != i) {
		swap(&arr[i], &arr[largest]);
		heapify(arr, n, largest, dim);
	}
}

// heapsort function. Point arr[] is the array to be sorted and int n is the size of the array
void heapSort(Point arr[], int n, int8_t dim){
	for (int i = n / 2 - 1; i >= 0; i--)
		heapify(arr, n, i, dim);
	for (int i = n - 1; i > 0; i--) {
		swap(&arr[0], &arr[i]);
		heapify(arr, i, 0, dim);
	}
}