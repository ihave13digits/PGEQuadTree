#include "point.h"
#include "quad.h"
#include "quadtree.h"

namespace QT
{
    Point* closest = new Point(0, 0);
    Quad* senses = new Quad(0, 0, 0, 0);
    QuadTree* tree = new QuadTree(0, 0, var::width, var::height, 256, 0);

    void DestroyQT() { delete tree; delete senses; delete closest; Print("Born: "+std::to_string(var::born)); Print("Dead: "+std::to_string(var::dead)); }
    void Draw(olc::PixelGameEngine* pge) { tree->Draw(pge); }
    void AddAnt(float x, float y) { Point* A = new Point(x, y, 3); QT::tree->AddPoint(A); }

    void Scan(Point* e)
    {
        senses->x = e->x-e->senses; senses->y = e->y-e->senses;
        senses->w = e->senses*2; senses->h = e->senses*2;
        closest-> x = e->x; closest->y = e->y;
        std::vector<Point*> pts;
        for (auto p : tree->GetPoints(senses, pts))
        {
            float close_distance = e->Distance(closest);
            if ((p->x != e->x && p->y != e->y && (e->Distance(p) < close_distance) || close_distance == 0))
            //{ if (!e->Collision(p))
                { closest->x = p->x; closest->y = p->y; }
            //}
        }
        e->tx = closest->x; e->ty = closest->y;
    }

    void Update()
    {
        std::vector<Point*> pts;
        senses->x = 0; senses->y = 0; senses->w = var::width; senses->h = var::height;
        for (auto p : tree->GetPoints(senses, pts)) { Scan(p); p->Update(); pts.push_back(p); if (p->in_bounds == false) { var::update_tree = true; } }
        if (var::update_tree) { var::update_tree = false; tree->Clear(); for (auto p : pts) { p->in_bounds = true; tree->AddPoint(p); } }
    }

}
