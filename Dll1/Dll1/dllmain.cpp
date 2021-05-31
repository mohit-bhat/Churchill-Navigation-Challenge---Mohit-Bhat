// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"

using namespace std;


extern "C" {
    /*
    BOOL APIENTRY DllMain(HMODULE hModule,
        DWORD  ul_reason_for_call,
        LPVOID lpReserved
    )
    {
        switch (ul_reason_for_call)
        {
        case DLL_PROCESS_ATTACH:
        case DLL_THREAD_ATTACH:
        case DLL_THREAD_DETACH:
        case DLL_PROCESS_DETACH:
            break;
        }




        return TRUE;
    }
    */

    struct SearchContext {
        map<float, Point*> mapped_points;
    };

    __declspec(dllexport) SearchContext* __stdcall create(const Point* points_begin, const Point* points_end) {
        SearchContext* init = new(SearchContext);
        for (Point* i = (Point*)points_begin; i < (Point*)points_end; i += sizeof(Point*)) init->mapped_points.insert(pair<float, Point*>(i->x, i));

        return init;
    }



    __declspec(dllexport) int32_t __stdcall search(SearchContext* sc, const Rect rect, const int32_t count, Point* out_points) {
        std::map<float, Point*>::iterator i;
        std::map<int, Point*> rect_points;
        std::map<int, Point*>::iterator j;
        for (i = sc->mapped_points.lower_bound(rect.lx); i != sc->mapped_points.upper_bound(rect.hx); i++) {
            if ((i->second->y > rect.ly) && (i->second->y < rect.hy))rect_points.insert(pair<int, Point*>(i->second->rank, i->second));
        }
        int32_t number_of_points;
        if (rect_points.size() >= count) number_of_points = count;
        else number_of_points = rect_points.size();
        j = rect_points.begin();
        for (int k = 0; k < number_of_points; k++) {
            out_points[k] = *j->second;
            ++j;
        }
        return number_of_points;
    }

    __declspec(dllexport) SearchContext* __stdcall destroy(SearchContext* sc) {
        sc->mapped_points.clear();
        delete sc;
        return nullptr;
    }



}