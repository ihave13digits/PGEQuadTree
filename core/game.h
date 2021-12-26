class Game : public olc::PixelGameEngine
{
public:
    Game() { sAppName = "QuadTree"; }

public:

    Point* mouse = new Point(0, 0);
    Quad* check = new Quad(0, 0, 128, 128);
    QuadTree* qt = new QuadTree(0, 0, 1024, 512);

    ~Game()
    { delete qt; delete check; delete mouse; }

    void Count()
    {
        mouse->x = GetMouseX(); mouse->y = GetMouseY();
        check->x = mouse->x-64; check->y = mouse->y-64; DrawRect(check->x, check->y, check->w, check->h, olc::YELLOW); std::vector<Point> pts;
        for (auto p : qt->GetPoints(check, pts)) { pts.push_back(p); /*DrawCircle(p.x, p.y, p.Distance(mouse), olc::RED);*/ Draw(p.x, p.y, olc::WHITE); }
        std::string count = std::to_string(pts.size());
        DrawStringDecal({ 1.0,1.0 }, count, olc::Pixel(255,255,255), {1.0,1.0});
    }

    void Generate()
    {
        for (int i = 0; i < 1000; i++) { Point* P = new Point(rand()%1024, rand()%512); qt->AddPoint(P); }
    }

    bool OnUserCreate() override { Generate(); return true; }
    bool OnUserUpdate(float fElapsedTime) override
    {
        Clear(olc::BLACK);
        // Stuff
        //qt->Draw(this);
        if (GetMouse(0).bPressed) { Point* p = new Point(GetMouseX(), GetMouseY()); qt->AddPoint(p); }
        if (GetKey(olc::Key::R).bPressed) { qt->Reset(); }
        if (GetKey(olc::Key::ENTER).bPressed) { Generate(); }
        Count();
        // End Frame
        return true;
    }
};
