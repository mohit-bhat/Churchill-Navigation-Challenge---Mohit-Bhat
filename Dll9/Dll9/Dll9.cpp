// Dll9.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#define DLL9_EXPORTS
#include "Dll9.h"


#include <stdio.h>


#define BOXES 50



extern "C"{

	struct node{
		Point p;
		node *left;
		node *equal;
		node *right;
	};

	struct SearchContext{
		node** data;
	};

	node * newNode(const Point* p){
		node* temp = (node*)malloc(sizeof(node));
		temp->p = *p;
		temp->left = NULL;
		temp->right = NULL;
		temp->equal = NULL;
		return temp;
	}

	node * insertNode(node* n, const Point* p){
		if (n == NULL)
			return newNode(p);
		if (p->y < n->p.y)
			n->left = insertNode(n->left, p);
		else if (p->y > n->p.y)
			n->right = insertNode(n->right, p);
		else if (p->y == n->p.y)
			n->equal = insertNode(n->equal, p);
		return n;
	}

	void insertElement(SearchContext* sc, int boxNo, Point* p){
		sc->data[boxNo] = insertNode(sc->data[boxNo], p);
	}

	int numInsert, search_times;
	void insertion(Point n, int32_t count, Point* out_points){
		//printf("\n%i %f %f  %i", n->p.rank, n->p.x, n->p.y, n->p.id);
		if (out_points[count - 1].rank < n.rank) return;
		int i;
		for (i = 0; i < count;){
			if (n.rank < out_points[i].rank){
				break;
			}
			i++;
		}
		if (i == count) return;
		for (int j = count - 2; j >= i; j--){
			out_points[j + 1] = out_points[j];
		}

		out_points[i] = n;
		numInsert++;
		if (numInsert >= count) numInsert = count;
	}

	void searchNodes(node * n, int32_t count, const Rect rect, Point * outPoints){
		if (!n) return;
		if (n->p.x >= rect.lx && n->p.x <= rect.hx && n->p.y >= rect.ly && n->p.y <= rect.hy){
			insertion(n->p, count, outPoints);
		}
		search_times++;
		if (n->p.y >= rect.ly){
			if (n->left) searchNodes(n->left, count, rect, outPoints);
		}
		if (n->p.y <= rect.hy){
			if (n->right) searchNodes(n->right, count, rect, outPoints);
		}
	}

	void deleteTree(node* node){
		if (!node) return;
		
			if (node->left) deleteTree(node->left);
			if (node->right) deleteTree(node->right);
			//if (node->equal) deleteTree(node->equal);
			free(node);
		
	}

	void walkTree(node * n){   //needed to avoid crashes for some reason
		if (!n) return;
		//printf("\n %i", n->p.rank);
		if (n->left) walkTree(n->left);
		if (n->right) walkTree(n->right);
		//if (n->equal) walkTree(n->equal);
	}

	float xMax, xMin, boxSize;
	int noPoints;

	/* Load the provided points into an internal data structure. The pointers follow the STL iterator convention, where
	"points_begin" points to the first element, and "points_end" points to one past the last element. The input points are
	only guaranteed to be valid for the duration of the call. Return a pointer to the context that can be used for
	consecutive searches on the data. */
	DLL9_API SearchContext* (__stdcall create)(const Point* points_begin, const Point* points_end){
		if (points_begin == NULL || points_end == NULL){
			return nullptr;
		}
		xMax = xMin = 0;
		noPoints = 0;
		SearchContext* sc = (SearchContext*) malloc(sizeof(SearchContext));
		Point * temp = (Point*) points_begin; 
		while (temp < (Point*) points_end){		//find min max
			if (temp->x > xMax) xMax = temp->x;
			if (temp->x < xMin) xMin = temp->x;
			temp++;
			noPoints++;
		}
		boxSize = (xMax - xMin) / BOXES;
		sc->data = (node**) calloc(BOXES, sizeof(node*));	//make boxes
		temp = (Point*)points_begin;
		int boxNo;
		while (temp < (Point*)points_end){
			boxNo = (int) ((temp->x - xMin) / boxSize);
			insertElement(sc, boxNo, temp);
			temp++;
		}

		for (int k = 0; k < BOXES; k++){
			walkTree(sc->data[k]);
		}
		return sc;
	}

	/* Search for "count" points with the smallest ranks inside "rect" and copy them ordered by smallest rank first in
	"out_points". Return the number of points copied. "out_points" points to a buffer owned by the caller that
	can hold "count" number of Points. */
	DLL9_API int32_t(__stdcall search)(SearchContext* sc, const Rect rect, const int32_t count, Point* out_points){
		if (!sc || !out_points) return 0;
		for (int k = 0; k < BOXES; k++){
			walkTree(sc->data[k]);
		}
		//printf("\nwalked tree\n");
		for (int i = 0; i < count; i++){
			out_points[i].rank = INT32_MAX;
			out_points[i].id = INT8_MAX;
			out_points[i].x = 0;
			out_points[i].y = 0;
		}
		//printf("\ntest1\n");
		int lBox = (int)((rect.lx - xMin) / boxSize) - 2;
		int rBox = (int)((rect.hx - xMin) / boxSize) + 3;
		if (lBox < 0) lBox = 0;
		if (rBox >= BOXES) rBox = BOXES - 1;
		search_times = 0;
		numInsert = 0;
		//printf("\ntest2\n");
		//Array * a = (Array *) malloc(sizeof(Array));
		//initArray(a, count);

		//printf("\ntest2.5\n");

		for (int i = lBox; i < rBox && i < BOXES; i++){
			//printf("\ntest_i = %i\n", i);
			searchNodes(sc->data[i], count, rect, out_points);
		}

		//printf("\ntest3\n");



		//for (int i = 0; i < a->used; i++){
			//insertion(a->array[i], count, out_points);
		//}
		//printf("\ntest___  %i\n", numInsert); 

		//freeArray(a);
		//free(a);
		//for (int i = 0; i < count; i++)printf(" %i, ", out_points[i].rank);
		return numInsert;
	}

	/* Release the resources associated with the context. Return nullptr if successful, "sc" otherwise. */
	DLL9_API SearchContext* (__stdcall destroy)(SearchContext* sc){
		//printf("\ntest1\n");
		if (!sc) return sc;
		for (int i = 0; i <= BOXES; i++){
			//printf(" %i", i);
			deleteTree(sc->data[i]);
		}
		free(sc->data);
		free(sc);
		sc = nullptr;
		return sc;
	}

}