class Game : public olc::PixelGameEngine
{
public:
    Game() { sAppName = "QuadTree"; }

public:

    Quad* check = new Quad(0, 0, 128, 128);
    QuadTree* qt = new QuadTree(0, 0, 1024, 512);

    ~Game()
    { delete qt; delete check; std::cout <<"Spawned Points: " <<born <<" Killed Points: " <<died <<std::endl; }

    void Count()
    {
        check->x = GetMouseX()-64; check->y = GetMouseY()-64; DrawRect(check->x, check->y, check->w, check->h, olc::YELLOW); std::vector<Point> pts;
        for (auto p : qt->GetPoints(check, pts)) { pts.push_back(p); DrawCircle(p.x, p.y, 3, olc::RED); }
        std::string count = std::to_string(pts.size());
        DrawStringDecal({ 1.0,1.0 }, count, olc::Pixel(255,255,255), {1.0,1.0});
    }

    bool OnUserCreate() override { return true; }
    bool OnUserUpdate(float fElapsedTime) override
    {
        Clear(olc::BLACK);
        // Stuff
        qt->Draw(this);
        if (GetMouse(0).bPressed) { Point* p = new Point(GetMouseX(), GetMouseY()); qt->AddPoint(p); }
        Count();
        // End Frame
        return true;
    }
};
