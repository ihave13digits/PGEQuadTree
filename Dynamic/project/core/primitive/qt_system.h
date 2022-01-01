//
///
//

namespace QT { olc::PixelGameEngine* pge = nullptr; }
namespace QT { bool desired = true; int point_count = 0; int born = 0; int dead = 0; }
#include "point.h"
namespace QT { Point*    closest = new Point(0, 0); }//std::vector<Point*> requested_points; std::vector<Point*> user_points; }
#include "quad.h"
namespace QT { Quad*     senses  = new Quad(0, 0, 0, 0); }
#include "quadtree.h"
namespace QT { QuadTree* tree    = new QuadTree(0, 0, var::width, var::height, 4, 0); }

namespace QT
{
    void DestroyQT() { delete tree; delete senses; delete closest; pge = nullptr; Print("Born: "+std::to_string(born)); Print("Dead: "+std::to_string(dead)); }
    void AddAnt(float x, float y) { Point* A = new Point(x, y, 3); if (!QT::tree->AddPoint(A)) { delete A; } }

    void Draw()
    {
        tree->Draw(pge);
        pge->DrawStringDecal({ 1.0,1.0 }, "Point Limit: "+std::to_string(tree->max_points), olc::WHITE, {1.0,1.0});
        pge->DrawStringDecal({ 1.0,9.0 }, "Point Count: "+std::to_string(point_count), olc::WHITE, {1.0,1.0});
    }

    /*
    void GetMousePoint()
    {
        float x = pge->GetMouseX(), y = pge->GetMouseY(), check_size=32;
        senses->x = x-check_size; senses->y = y-check_size;
        senses->w = check_size*2; senses->h = check_size*2;
        closest->x = x; closest->y = y;
        bool found = false;
        desired = true;
        requested_points.clear(); tree->GetPoints(senses);
        for (auto p : requested_points)
        {
            float close_distance = closest->Distance(p);
            if ((p->x != x && p->y != y) && (close_distance <= check_size || close_distance == 0))
            { closest->x = p->x; closest->y = p->y; found = true; if (pge->GetMouse(1).bPressed) { p->Die(); } }
        }
        if (found) { pge->DrawCircle(closest->x, closest->y, check_size/2, olc::GREEN); }
        pge->DrawCircle(x, y, check_size, olc::YELLOW);
    }

    void Scan(Point* e)
    {
        senses->x = e->x-e->senses; senses->y = e->y-e->senses;
        senses->w = e->senses*2; senses->h = e->senses*2;
        closest-> x = e->x; closest->y = e->y;
        bool found = false;
        desired = false;
        user_points.clear(); tree->GetPoints(senses);
        for (auto p : user_points)
        {
            float close_distance = e->Distance(closest);
            if ((p->x != e->x && p->y != e->y) && (e->Distance(p) < close_distance || close_distance == 0)) { closest->x = p->x; closest->y = p->y; found = true; }
        }
        if (found) { e->tx = closest->x; e->ty = closest->y; }
    }

    void Update(float dt)
    {
        point_count = 0; senses->x = 0; senses->y = 0; senses->w = var::width; senses->h = var::height;
        desired = true; requested_points.clear(); tree->GetPoints(senses);
        for (auto p : requested_points)
        { Scan(p); p->Update(dt); point_count++; if (p->in_bounds == false) { var::update_tree = true; } }
        if (var::update_tree)
        {
            var::update_tree = false;
            float a = std::max(point_count, 16) * float(point_count)/(float(point_count)*32.0); int new_limit = std::max(int(a), 1);
            tree->SetLimit(new_limit); tree->Clear();
            for (auto p : requested_points) { p->in_bounds = true; tree->AddPoint(p); }
        }
    }
    */
    
    void GetMousePoint()
    {
        float x = pge->GetMouseX(), y = pge->GetMouseY(), check_size=32;
        senses->x = x-check_size; senses->y = y-check_size;
        senses->w = check_size*2; senses->h = check_size*2;
        closest->x = x; closest->y = y;
        std::vector<Point*> pts;
        bool found = false;
        for (auto p : tree->GetPoints(senses))
        {
            float close_distance = closest->Distance(p);
            if ((p->x != x && p->y != y) && (close_distance <= check_size || close_distance == 0))
            { closest->x = p->x; closest->y = p->y; found = true; if (pge->GetMouse(1).bPressed) { p->Die(); } }
        }
        if (found) { pge->DrawCircle(closest->x, closest->y, check_size/2, olc::GREEN); }
        pge->DrawCircle(x, y, check_size, olc::YELLOW);
    }

    void Scan(Point* e)
    {
        senses->x = e->x-e->senses; senses->y = e->y-e->senses;
        senses->w = e->senses*2; senses->h = e->senses*2;
        closest-> x = e->x; closest->y = e->y;
        std::vector<Point*> pts;
        bool found = false;
        for (auto p : tree->GetPoints(senses))
        {
            float close_distance = e->Distance(closest);
            if ((p->x != e->x && p->y != e->y) && (e->Distance(p) < close_distance || close_distance == 0)) { closest->x = p->x; closest->y = p->y; found = true; }
        }
        if (found) { e->tx = closest->x; e->ty = closest->y; }
    }

    void Update(float dt)
    {
        point_count = 0; std::vector<Point*> pts;
        senses->x = 0; senses->y = 0; senses->w = var::width; senses->h = var::height;
        for (auto p : tree->GetPoints(senses)) { Scan(p); p->Update(dt); pts.push_back(p); point_count++; if (p->in_bounds == false) { var::update_tree = true; } }
        if (var::update_tree)
        {
            var::update_tree = false;
            float a = std::max(point_count, 16) * float(point_count)/(float(point_count)*32.0);
            int new_limit = std::max(int(a), 1);
            tree->SetLimit(new_limit); tree->Clear();
            for (auto p : pts) { p->in_bounds = true; tree->AddPoint(p); }
        }
    }

}
