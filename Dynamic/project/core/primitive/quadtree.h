class QuadTree
{

public:

    bool subdivided = false;
    int max_points, depth, curve;
    std::vector<Point*> points;
    Quad* quad = nullptr;
    QuadTree* NE = nullptr;
    QuadTree* NW = nullptr;
    QuadTree* SE = nullptr;
    QuadTree* SW = nullptr;

    QuadTree(float X, float Y, float W, float H, int P, int D) { quad = new Quad(X,Y,W,H); curve; depth=D; }
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
    void SetLimit(int lmt) { max_points=lmt; if(NE) NE->SetLimit(lmt); if(NW) NW->SetLimit(lmt); if(SE) SE->SetLimit(lmt); if(SW) SW->SetLimit(lmt);}

    std::vector<Point*> GetPoints(Quad* area)
    {
        std::vector<Point*> desired_points;
        if (!subdivided && (area->Collision(quad) || quad->Collision(area))) { for (auto p : points) { if (area->PointInside(p)) { desired_points.push_back(p); } } }
        else
        {
            if (NE->quad->Collision(area) || area->Collision(NE->quad))
            {
                if (!NE->subdivided) { for (auto p : NE->points) { if (area->PointInside(p)) { desired_points.push_back(p); } } }
                else                 { for (auto p : NE->GetPoints(area)) { if (area->PointInside(p)) { desired_points.push_back(p); } } }
            }
            if (NW->quad->Collision(area) || area->Collision(NW->quad))
            {
                if (!NW->subdivided) { for (auto p : NW->points) { if (area->PointInside(p)) { desired_points.push_back(p); } } }
                else                 { for (auto p : NW->GetPoints(area)) { if (area->PointInside(p)) { desired_points.push_back(p); } } }
            }
            if (SE->quad->Collision(area) || area->Collision(SE->quad))
            {
                if (!SE->subdivided) { for (auto p : SE->points) { if (area->PointInside(p)) { desired_points.push_back(p); } } }
                else                 { for (auto p : SE->GetPoints(area)) { if (area->PointInside(p)) { desired_points.push_back(p); } } }
            }
            if (SW->quad->Collision(area) || area->Collision(SW->quad))
            {
                if (!SW->subdivided) { for (auto p : SW->points) { if (area->PointInside(p)) { desired_points.push_back(p); } } }
                else                 { for (auto p : SW->GetPoints(area)) { if (area->PointInside(p)) { desired_points.push_back(p); } } }
            }
        }
        return desired_points;
    }

    bool AddPoint(Point* p)
    {
        bool added_point = true;
        p->bx = quad->x; p->by = quad->y; p->bw = quad->x+quad->w; p->bh = quad->y+quad->h;
        if (!subdivided)
        {
            if (quad->PointInside(p)) { points.push_back(p); }
            if (points.size() > max_points) { Subdivide(); }
        }
        else
        {
            if      (NE->quad->PointInside(p)) { NE->AddPoint(p); }
            else if (NW->quad->PointInside(p)) { NW->AddPoint(p); }
            else if (SE->quad->PointInside(p)) { SE->AddPoint(p); }
            else if (SW->quad->PointInside(p)) { SW->AddPoint(p); }
            else { added_point = false; }
        }
        return added_point;
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
        if (!subdivided)
        {
            if (NE) delete NE; if (NW) delete NW; if (SE) delete SE; if (SW) delete SW;
            //SetLimit(std::max(int(points.size()), 16)/4);
        }
        else { NE->Update(); NW->Update(); SE->Update(); SW->Update(); }
    }

    void PrintTree()
    {
        if (!subdivided)
        {
            std::cout <<depth <<": ";
            for (auto p : points) { std::cout <<"(" <<p->x <<", " <<p->y <<") "; }
            std::cout <<std::endl;
        }
        else { NE->PrintTree(); NW->PrintTree(); SE->PrintTree(); SW->PrintTree(); }
    }

    void Draw()
    {
        olc::Pixel c_point = olc::Pixel(255, 0, 0);
        olc::Pixel c_radius = olc::Pixel(255, 128, 0);
        olc::Pixel c_senses = olc::Pixel(255, 255, 0);
        if (var::show_quad)   { DrawQuads();  }
        
        if (!subdivided)
        {
            for (auto p : points)
            {
                if (var::show_point)  { QT::pge->Draw(p->x, p->y, c_point); }
                if (var::show_radius) { QT::pge->DrawCircle(p->x, p->y, p->radius, c_radius); }
                if (var::show_senses) { QT::pge->DrawRect(p->x-p->senses, p->y-p->senses, p->senses*2, p->senses*2, c_senses); }
            }
        }
        else { NE->Draw(); NW->Draw(); SE->Draw(); SW->Draw(); }
    }

    void DrawQuads() { QT::pge-> DrawRect(quad->x, quad->y, quad->w-1, quad->h-1, olc::Pixel(255-int(255/std::max(depth, 1)), int(255/std::max(depth, 1)), 0)); }

};
