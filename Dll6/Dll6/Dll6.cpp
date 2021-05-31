// Dll6.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"

#define DLL6_EXPORTS
#include "Dll6.h"

#include <stdio.h>

using namespace std;

struct node{
	Point p;
	float key;
	node *left;
	node *right;
};

struct SearchContext{
	node* root;
};

node * insertNode(node* n, const Point* p){
	if (n == NULL)
		return newNode(p);
	float key = (p->x);
	if (key < n->key)
		n->left = insertNode(n->left, p);
	else if (key > n->key)
		n->right = insertNode(n->right, p);
	return n;
}

node * newNode(const Point* p){
	node* temp = (node*)malloc(sizeof(node));
	temp->p = *p;
	temp->key = (p->x);
	temp->left = NULL;
	temp->right = NULL;
	return temp;
}

void deleteTree(node* node){
	if (node){
		if (node->left) deleteTree(node->left);
		if (node->right) deleteTree(node->right);
		free(node);
	}
}

int numInsert, search_times;
void insertion(node* n, int32_t count, Point* out_points){
	int i;
	for (i = 0; i < count; ){
		if (n->p.rank < out_points[i].rank){
			break;
		}
		i++;
	}
	if (i == count) return;
	for (int j = count - 1; j >= i; j--){
		out_points[j + 1] = out_points[j];
	}

	out_points[i] = n->p;
	numInsert++;
	if (numInsert >= count) numInsert = count;
}

void search(node * n, int32_t count, const Rect rect, Point* out_points){
	if (n->p.x > rect.lx && n->p.x < rect.hx){
		if (n->p.y > rect.ly && n->p.y < rect.hy){
			insertion(n, count, out_points);
		}
	}
	search_times++;
	if (n->p.x >= rect.lx){
		if (n->left) search(n->left, count, rect, out_points);
	}

	if (n->p.x <= rect.hx){
		if (n->right) search(n->right, count, rect, out_points);
	}
}

extern "C"{
	/* Load the provided points into an internal data structure. The pointers follow the STL iterator convention, where
	"points_begin" points to the first element, and "points_end" points to one past the last element. The input points are
	only guaranteed to be valid for the duration of the call. Return a pointer to the context that can be used for
	consecutive searches on the data. */
	DLL6_API SearchContext* (__stdcall create)(const Point* points_begin, const Point* points_end){
		SearchContext* sc = (SearchContext*) malloc(sizeof(SearchContext));
		if (points_begin == NULL || points_end == NULL){
			return nullptr;
		}
		sc->root = insertNode(NULL, points_begin);
		for (Point * i = (Point *)&points_begin[1]; i < points_end; i += sizeof(Point)){
			insertNode(sc->root, i);
		}
		return sc;
	}

	/* Search for "count" points with the smallest ranks inside "rect" and copy them ordered by smallest rank first in
	"out_points". Return the number of points copied. "out_points" points to a buffer owned by the caller that
	can hold "count" number of Points. */
	DLL6_API int32_t(__stdcall search)(SearchContext* sc, const Rect rect, const int32_t count, Point* out_points){
		Point temp;
		temp.rank = INT32_MAX;
		for (int i = 0; i < count; i++){
			out_points[i] = temp;
		}
		if (sc){
			numInsert = 0;
			search_times = 0;
			search(sc->root, count, rect, out_points);
		}
		//printf("\n%i , %i, %i ;", count, numInsert, search_times);
		for (int i = 0; i < count; i++){
			//printf(" %i", out_points[i].rank);
		}

		return numInsert;
	}

	/* Release the resources associated with the context. Return nullptr if successful, "sc" otherwise. */
	DLL6_API SearchContext* (__stdcall destroy)(SearchContext* sc){
		//printf("\ntest1\n");
		if (!sc) return sc;
		deleteTree(sc->root);
		free(sc);
		return sc;
	}

}