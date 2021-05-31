using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Dll4
{
    struct Point
    {
        byte id;
        int rank;
        float x;
        float y;
    };

    struct Rect
    {
        float lx;
        float ly;
        float hx;
        float hy;
    };
    struct SearchContext
    {
        SortedDictionary<float, Point*> mapped_points;
    };

    
    unsafe public class dll4
    {
        unsafe public SearchContext* create(Point* points_begin, Point* points_end)
        {
            SearchContext* init = new(SearchContext);
            for (Point* i = (Point*)points_begin; i < (Point*)points_end; i += sizeof(Point*)) init->mapped_points.insert(pair<float, Point*>(i->x, i));
            return init;
        }z

    }
}
