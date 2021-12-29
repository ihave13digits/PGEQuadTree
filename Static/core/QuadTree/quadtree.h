class QuadTree
{

public:

    bool subdivided = false;
    int max_points = 4;
    Quad* quad = nullptr;
    std::vector<Point*> points;
    QuadTree* NE = nullptr;
    QuadTree* NW = nullptr;
    QuadTree* SE = nullptr;
    QuadTree* SW = nullptr;

    QuadTree(float X, float Y, float W, float H) { quad = new Quad(X,Y,W,H); }
    ~QuadTree()
    {
        delete quad; DelPoints();
        if (NE) { NE->DelPoints(); delete NE; }
        if (NW) { NW->DelPoints(); delete NW; }
        if (SE) { SE->DelPoints(); delete SE; }
        if (SW) { SW->DelPoints(); delete SW; }
    }

    void Reset() { if (!subdivided) { DelPoints(); } else { NE->Reset(); NW->Reset(); SE->Reset(); SW->Reset(); } }

    std::vector<Point> GetPoints(Quad* area, std::vector<Point> pts)
    {
        std::vector<Point> desired_points;
        if (!subdivided && area->Collision(quad)) { for (auto p : points) { if (area->PointInside(p)) { Point P = Point(p->x, p->y); desired_points.push_back(P); } } }
        else
        {
            if (NE->quad->Collision(area) || area->Collision(NE->quad))
            {
                if (!NE->subdivided)     { for (auto p : NE->points) { if (area->PointInside(p)) { Point P = Point(p->x, p->y); desired_points.push_back(P); } } }
                else if (NE->subdivided) { for (auto p : NE->GetPoints(area, desired_points))
                    { Point* P = new Point(p.x, p.y); if (area->PointInside(P)) { desired_points.push_back(p); } delete P; } } }
            if (NW->quad->Collision(area) || area->Collision(NW->quad))
            {
                if (!NW->subdivided)     { for (auto p : NW->points) { if (area->PointInside(p)) { Point P = Point(p->x, p->y); desired_points.push_back(P); } } }
                else if (NW->subdivided) { for (auto p : NW->GetPoints(area, desired_points))
                    { Point* P = new Point(p.x, p.y); if (area->PointInside(P)) { desired_points.push_back(p); } delete P; } } }
            if (SE->quad->Collision(area) || area->Collision(SE->quad))
            {
                if (!SE->subdivided)     { for (auto p : SE->points) { if (area->PointInside(p)) { Point P = Point(p->x, p->y); desired_points.push_back(P); } } }
                else if (SE->subdivided) { for (auto p : SE->GetPoints(area, desired_points))
                    { Point* P = new Point(p.x, p.y); if (area->PointInside(P)) { desired_points.push_back(p); } delete P; } } }
            if (SW->quad->Collision(area) || area->Collision(SW->quad))
            {
                if (!SW->subdivided)     { for (auto p : SW->points) { if (area->PointInside(p)) { Point P = Point(p->x, p->y); desired_points.push_back(P); } } }
                else if (SW->subdivided) { for (auto p : SW->GetPoints(area, desired_points))
                    { Point* P = new Point(p.x, p.y); if (area->PointInside(P)) { desired_points.push_back(p); } delete P; } } }
        }
        return desired_points;
    }

    void AddPoint(Point* p)
    {
        if (quad->PointInside(p) && !subdivided)
        {
            points.push_back(p);
            if (points.size() > max_points) { Subdivide(); }
        }
        else
        {
            if      (NE->quad->PointInside(p)) { NE->AddPoint(p); }
            else if (NW->quad->PointInside(p)) { NW->AddPoint(p); }
            else if (SE->quad->PointInside(p)) { SE->AddPoint(p); }
            else if (SW->quad->PointInside(p)) { SW->AddPoint(p); }
        }
    }
    void DelPoint(int index) { if (points[index]) { /*delete points[index];*/ points.erase(points.begin()+index); } }
    void DelPoints() { if (points.size()) { for (auto p : points) { delete p; } points.clear(); } }

    void Subdivide()
    {
        if (!subdivided)
        {
            NE = new QuadTree(quad->x+quad->w/2, quad->y,           quad->w/2, quad->h/2);
            NW = new QuadTree(quad->x,           quad->y,           quad->w/2, quad->h/2);
            SE = new QuadTree(quad->x+quad->w/2, quad->y+quad->h/2, quad->w/2, quad->h/2);
            SW = new QuadTree(quad->x,           quad->y+quad->h/2, quad->w/2, quad->h/2);
            while (points.size() > 0)
            {
                if      (NE->quad->PointInside(points[0])) { NE->AddPoint(points[0]); DelPoint(0); }
                else if (NW->quad->PointInside(points[0])) { NW->AddPoint(points[0]); DelPoint(0); }
                else if (SE->quad->PointInside(points[0])) { SE->AddPoint(points[0]); DelPoint(0); }
                else if (SW->quad->PointInside(points[0])) { SW->AddPoint(points[0]); DelPoint(0); }
                else break;
            }
        }
        subdivided = true;
    }

    void Draw(olc::PixelGameEngine* pge)
    {
        for (int p = 0; p < points.size(); p++) { pge->Draw(points[p]->x, points[p]->y, olc::WHITE); }
        if (!subdivided) { pge-> DrawRect(quad->x, quad->y, quad->w-1, quad->h-1, olc::GREEN); }
        if (subdivided)
        {
            pge->DrawRect(NE->quad->x+1, NE->quad->y+1, NE->quad->w-2, NE->quad->h-2, olc::RED);
            pge->DrawRect(NW->quad->x+1, NW->quad->y+1, NW->quad->w-2, NW->quad->h-2, olc::RED);
            pge->DrawRect(SE->quad->x+1, SE->quad->y+1, SE->quad->w-2, SE->quad->h-2, olc::RED);
            pge->DrawRect(SW->quad->x+1, SW->quad->y+1, SW->quad->w-2, SW->quad->h-2, olc::RED);
            NE->Draw(pge); NW->Draw(pge); SE->Draw(pge); SW->Draw(pge);
        }
    }

};
