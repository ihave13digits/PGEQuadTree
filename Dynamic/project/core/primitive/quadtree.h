class QuadTree
{

public:

    bool subdivided = false;
    int max_points, depth;
    std::vector<Point*> points;
    Quad* quad = nullptr;
    QuadTree* NE = nullptr;
    QuadTree* NW = nullptr;
    QuadTree* SE = nullptr;
    QuadTree* SW = nullptr;

    QuadTree(float X, float Y, float W, float H, int P, int D) { quad = new Quad(X,Y,W,H); max_points=P; depth=D; }
    ~QuadTree()
    {
        delete quad; DelPoints();
        if (NE) { NE->DelPoints(); delete NE; }
        if (NW) { NW->DelPoints(); delete NW; }
        if (SE) { SE->DelPoints(); delete SE; }
        if (SW) { SW->DelPoints(); delete SW; }
    }

    void Reset() { if (!subdivided) { DelPoints(); } else { NE->Reset(); NW->Reset(); SE->Reset(); SW->Reset(); subdivided = false; } }
    void Clear() { if (!subdivided) { RemPoints(); } else { NE->Clear(); NW->Clear(); SE->Clear(); SW->Clear(); subdivided = false; } }
    void RemPoint(int index) { if (points[index]) { points.erase(points.begin()+index); } }
    void RemPoints() { if (points.size() > 0) { points.clear(); } }
    void DelPoint(int index) { if (points[index]) { delete points[index]; points.erase(points.begin()+index); } }
    void DelPoints() { if (points.size() > 0) { for (auto p : points) { delete p; } points.clear(); } }

    std::vector<Point*> GetPoints(Quad* area, std::vector<Point*> pts)
    {
        std::vector<Point*> desired_points;
        //for (auto p : pts) { desired_points.push_back(p); }
        if (!subdivided && area->Collision(quad)) { for (auto p : points) { if (area->PointInside(p)) { desired_points.push_back(p); } } }
        else
        {
            if (NE->quad->Collision(area) || area->Collision(NE->quad))
            {
                if (!NE->subdivided)     { for (auto p : NE->points) { if (area->PointInside(p)) { desired_points.push_back(p); } } }
                else if (NE->subdivided) { for (auto p : NE->GetPoints(area, desired_points)) { if (area->PointInside(p)) { desired_points.push_back(p); } } }
            }
            if (NW->quad->Collision(area) || area->Collision(NW->quad))
            {
                if (!NW->subdivided)     { for (auto p : NW->points) { if (area->PointInside(p)) { desired_points.push_back(p); } } }
                else if (NW->subdivided) { for (auto p : NW->GetPoints(area, desired_points)) { if (area->PointInside(p)) { desired_points.push_back(p); } } }
            }
            if (SE->quad->Collision(area) || area->Collision(SE->quad))
            {
                if (!SE->subdivided)     { for (auto p : SE->points) { if (area->PointInside(p)) { desired_points.push_back(p); } } }
                else if (SE->subdivided) { for (auto p : SE->GetPoints(area, desired_points)) { if (area->PointInside(p)) { desired_points.push_back(p); } } }
            }
            if (SW->quad->Collision(area) || area->Collision(SW->quad))
            {
                if (!SW->subdivided)     { for (auto p : SW->points) { if (area->PointInside(p)) { desired_points.push_back(p); } } }
                else if (SW->subdivided) { for (auto p : SW->GetPoints(area, desired_points)) { if (area->PointInside(p)) { desired_points.push_back(p); } } }
            }
        }
        return desired_points;
    }

    void AddPoint(Point* p)
    {
        p->bx = quad->x; p->by = quad->y; p->bw = quad->x+quad->w; p->bh = quad->y+quad->h;
        if (!subdivided)
        {
            if (quad->PointInside(p)) { points.push_back(p); }
            if (points.size() > max_points) { Subdivide(); }
        }
        else if (subdivided)
        {
            if      (NE->quad->PointInside(p)) { NE->AddPoint(p); }
            else if (NW->quad->PointInside(p)) { NW->AddPoint(p); }
            else if (SE->quad->PointInside(p)) { SE->AddPoint(p); }
            else if (SW->quad->PointInside(p)) { SW->AddPoint(p); }
        }
    }

    void Subdivide()
    {
        if (!subdivided)
        {
            int mp = std::max(float(max_points*0.9), 1.0f);
            if (NE == nullptr) NE = new QuadTree(quad->x+quad->w/2, quad->y,           quad->w/2, quad->h/2, mp, depth+1);
            if (NW == nullptr) NW = new QuadTree(quad->x,           quad->y,           quad->w/2, quad->h/2, mp, depth+1);
            if (SE == nullptr) SE = new QuadTree(quad->x+quad->w/2, quad->y+quad->h/2, quad->w/2, quad->h/2, mp, depth+1);
            if (SW == nullptr) SW = new QuadTree(quad->x,           quad->y+quad->h/2, quad->w/2, quad->h/2, mp, depth+1);
            while (points.size() > 0)
            {
                if      (NE->quad->PointInside(points[0])) { NE->AddPoint(points[0]); RemPoint(0); }
                else if (NW->quad->PointInside(points[0])) { NW->AddPoint(points[0]); RemPoint(0); }
                else if (SE->quad->PointInside(points[0])) { SE->AddPoint(points[0]); RemPoint(0); }
                else if (SW->quad->PointInside(points[0])) { SW->AddPoint(points[0]); RemPoint(0); }
                else break;
            }
            subdivided = true;
        }
    }

    void Update()
    {
        //if (!subdivided) { for (auto p : points) { Scan(p); p->Update(); if (!p->in_bounds) { var::update_tree = true; } } }
        //else { NE->Update(); NW->Update(); SE->Update(); SW->Update(); }
    }

    void Draw(olc::PixelGameEngine* pge)
    {
        if (var::show_quad)   { DrawQuads(pge);  }
        if (var::show_point)  { DrawPoint(pge);  }
        if (var::show_radius) { DrawRadius(pge); }
        if (var::show_senses) { DrawSenses(pge); }
    }

    void DrawQuads(olc::PixelGameEngine* pge)
    {
        pge-> DrawRect(quad->x, quad->y, quad->w-1, quad->h-1, olc::Pixel(255*int(subdivided), 255*int(1-subdivided), 0));
        if (subdivided) { NE->Draw(pge); NW->Draw(pge); SE->Draw(pge); SW->Draw(pge); }
    }

    void DrawPoint(olc::PixelGameEngine* pge)
    {
        olc::Pixel color = olc::Pixel(255, 0, 0);
        if (!subdivided) { for (auto p : points) { pge->Draw(p->x, p->y, color); } }
        else { NE->Draw(pge); NW->Draw(pge); SE->Draw(pge); SW->Draw(pge); }
    }

    void DrawRadius(olc::PixelGameEngine* pge)
    {
        olc::Pixel color = olc::Pixel(255, 128, 0);;
        if (!subdivided) { for (auto p : points) { pge->DrawCircle(p->x, p->y, p->radius, color); } }
        else { NE->Draw(pge); NW->Draw(pge); SE->Draw(pge); SW->Draw(pge); }
    }

    void DrawSenses(olc::PixelGameEngine* pge)
    {
        olc::Pixel color = olc::Pixel(255, 255, 0);
        if (!subdivided) { for (auto p : points) { pge->DrawRect(p->x-p->senses, p->y-p->senses, p->senses*2, p->senses*2, color); } }
        else { NE->Draw(pge); NW->Draw(pge); SE->Draw(pge); SW->Draw(pge); }
    }

};
