// Dll10.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "Dll10.h"



	struct SearchContext{
		Point* arr;
		int numEntries;
	};

	void pointPrinter(const Point * p){
		if (!p) return;
		printf("\nPoint %i; rank: %i  x:%f   y:%f", p->id, p->rank, p->x, p->y);
	}

	int noPoints;
	

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
		if (i == count) return;
		for (int j = count - 2; j >= i; j--){
			out_points[j + 1] = out_points[j];
		}

		out_points[i] = *n;
		if (numInsert < count) numInsert++;
	}
	
	extern "C"{

		DLL10_API SearchContext* (__stdcall create)(const Point* points_begin, const Point* points_end){
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
			//printf("\nLoading complete: %i Points, start at %p, end at %p", noPoints, points_begin, points_end);

			return sc;
		}


		DLL10_API int32_t(__stdcall search)(SearchContext* sc, const Rect rect, const int32_t count, Point* out_points){
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
								insertion(&sc->arr[i], count, out_points);
								//printf("Test_C");
							}
						}
					}
				}
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


		DLL10_API SearchContext* (__stdcall destroy)(SearchContext* sc){
			if (!sc) return sc;
			free(sc->arr);
			free(sc);
			sc = nullptr;
			return sc;
		}
	}