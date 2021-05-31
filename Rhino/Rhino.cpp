/*
* file: Rhino.cpp
* author: Mohit Bhat
*
* For Churchill Navigation Challenge
*
* Defines the exported functions for the DLL application.
*
* This approach stores the input data in an array
* The array is sorted by rank during creation
* Searching is done linearly and stops once the required number of points have been found
*
* Heapsort implimentations adapted from https://www.geeksforgeeks.org/heap-sort/
*
*/

#include "stdafx.h"

 
//External linkage for functions exporeted by DLL
extern "C"{

	RHINO_API SearchContext* (__stdcall create)(const Point* points_begin, const Point* points_end){
		if (points_begin == NULL || points_end == NULL) return nullptr;			// Checking for valid input
		SearchContext * sc = (SearchContext *)malloc(sizeof(SearchContext));	// Allocating search context
		Point * temp = (Point*)points_begin;
		int noPoints = 0;
		while (temp < (Point*)points_end){										// Iterating through array to find number of entries
			temp++;
			noPoints++;
		}
		sc->arr = (Point *)malloc((noPoints + 1) * sizeof(Point));				// Allocating memory for point array in search context based on number of points in input array
		sc->numEntries = noPoints;												// storing number of entries in array for future reference
		for (int i = 0; i < noPoints; i++){
			sc->arr[i] = points_begin[i];										// Directly copies entries from input array into search context array
		}
		heapSort(sc->arr, noPoints);											// Sorts search context array by rank in ascending order
		return sc;																// return search context
	}


	RHINO_API int32_t(__stdcall search)(SearchContext* sc, const Rect rect, const int32_t count, Point* out_points){
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


	RHINO_API SearchContext* (__stdcall destroy)(SearchContext* sc){
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
void heapify(Point arr[], int n, int i){
	int largest = i;
	int l = 2 * i + 1;
	int r = 2 * i + 2;
	if (l < n && arr[l].rank > arr[largest].rank)
		largest = l;
	if (r < n && arr[r].rank > arr[largest].rank)
		largest = r;
	if (largest != i) {
		swap(&arr[i], &arr[largest]);
		heapify(arr, n, largest);
	}
}

// heapsort function. Point arr[] is the array to be sorted and int n is the size of the array
void heapSort(Point arr[], int n){
	for (int i = n / 2 - 1; i >= 0; i--)
		heapify(arr, n, i);
	for (int i = n - 1; i > 0; i--) {
		swap(&arr[0], &arr[i]);
		heapify(arr, i, 0);
	}
}