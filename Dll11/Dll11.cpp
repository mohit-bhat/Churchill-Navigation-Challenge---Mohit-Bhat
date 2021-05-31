// Dll11.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"

struct Node{
	Node *left, *right;
	Point p;
};

struct SearchContext{
	Node * root;
	int noPoints;
};

void pointPrinter(const Point * p){
	if (!p) return;
	printf("\nPoint %i;\t rank: %i\t  x:%f\t   y:%f", p->id, p->rank, p->x, p->y);
}

void walkTree(Node * n){
	if (!n) return;
	//pointPrinter(&n->p);
	if (n->left) walkTree(n->left);
	if (n->right) walkTree(n->right);
}

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

//int num;
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



extern "C"{
	int noSearches;
	/* Load the provided points into an internal data structure. The pointers follow the STL iterator convention, where
	"points_begin" points to the first element, and "points_end" points to one past the last element. The input points are
	only guaranteed to be valid for the duration of the call. Return a pointer to the context that can be used for
	consecutive searches on the data. */
	DLL11_API SearchContext* (__stdcall create)(const Point* points_begin, const Point* points_end){
		if (points_begin == NULL || points_end == NULL) return nullptr;
		int noPoints = 0;
		noSearches = 0;
		Point * temp = (Point*)points_begin;
		try {
			
			//printf("\nTest1\n");
			SearchContext * sc = (SearchContext *)malloc(sizeof(SearchContext));
			
			
			//printf("\nTest2\n");
			while (temp < (Point*)points_end){
				temp++;
				noPoints++;
			}
			//printf("\nTest3\n");
			sc->noPoints = noPoints;
			Point * arr = (Point *)malloc(noPoints * sizeof(Point));
			for (int i = 0; i < noPoints; i++) arr[i] = points_begin[i];
			//quickSort(arr, 0, noPoints, TRUE);
			//printf("\nTest4\n");
			//num = 0;
			sc->root = KDTree(arr, 0, 0, noPoints);
			walkTree(sc->root);
			free(arr);
			//printf("\nTest5\n");
			return sc;
		}
		catch (...) {
			//printf("\n\nERROR %i\n\n", noPoints);
		}
		
	}
	
	int numInsert;
	void insertion(Point * n, int32_t count, Point* out_points){
		if (!n) return;
		if (out_points[count - 1].rank < n->rank) return;
		int i;
		for (i = 0; i < count;){
			if (n->rank < out_points[i].rank){
				break;
			}
			i++;
		}
		for (int j = count - 2; j >= i; j--){
			out_points[j + 1] = out_points[j];
		}

		out_points[i] = *n;
		if (numInsert < count) numInsert++;
	}

	void searching(Node * n, int32_t count, const Rect rect, Point * outPoints, int depth){
		if (!n) return;
		if (n->p.x >= rect.lx) {
			if (n->p.x <= rect.hx) {
				if (n->p.y >= rect.ly) {
					if (n->p.y <= rect.hy){
						//printf("Test_B");
						//printf("; %i %f %f", sc->arr[i].rank, sc->arr[i].x, sc->arr[i].y);
						insertion(&n->p, count, outPoints);
						//printf("Test_C");
					}
				}
			}
		}
		if ((depth % 2)){
			if (n->p.x <= rect.hx) if (n->right) searching(n->right, count, rect, outPoints, depth + 1);
			if (n->p.x >= rect.lx) if (n->left) searching(n->left, count, rect, outPoints, depth + 1);
		}
		else{
			if (n->p.y <= rect.hy) if (n->right) searching(n->right, count, rect, outPoints, depth + 1);
			if (n->p.y >= rect.ly) if (n->left) searching(n->left, count, rect, outPoints, depth + 1);
		}
	}


	/* Search for "count" points with the smallest ranks inside "rect" and copy them ordered by smallest rank first in
	"out_points". Return the number of points copied. "out_points" points to a buffer owned by the caller that
	can hold "count" number of Points.*/
	DLL11_API int32_t(__stdcall search)(SearchContext* sc, const Rect rect, const int32_t count, Point* out_points){
		if (!sc || !out_points) return 0;
		noSearches++;
		//printf("\nSearch %i points in [%f,%f]x and [%f,%f]y \n", count, rect.lx, rect.hx, rect.ly, rect.hx);
		for (int i = 0; i < count; i++){
			out_points[i].rank = INT32_MAX;
			out_points[i].id = INT8_MAX;
			out_points[i].x = 0;
			out_points[i].y = 0;
		}
		numInsert = 0;
		walkTree(sc->root);
		printf("\nRoot: %p", sc->root);
		searching(sc->root, count, rect, out_points, 0);
		//printf("\nNumber of Points Inserted: %i", numInsert);
		for (int i = 0; i < numInsert; i++){
			//pointPrinter(&out_points[i]);
		}
		printf(" Searches: %i", noSearches);
		//if (noSearches > 999) walkTree(sc->root);
		return numInsert;
	}


	void deleteTree(Node * n){
		if (!n) return;
		if (n->right) deleteTree(n->right);
		if (n->left) deleteTree(n->left);
		free(n);
		return;
	}

	/* Release the resources associated with the context. Return nullptr if successful, "sc" otherwise. */
	DLL11_API SearchContext* (__stdcall destroy)(SearchContext* sc){
		if (!sc) return sc;
		deleteTree(sc->root);
		free(sc);
		sc = nullptr;
		return sc;
	}

}