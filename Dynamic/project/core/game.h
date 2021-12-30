class Game : public olc::PixelGameEngine
{
public:

    Game() { sAppName = "Entity QuadTree"; }
    ~Game() { QT::DestroyQT(); }

public:

    bool paused = true;

    void UpdateQT(float dt) { Clear(olc::BLACK); QT::Update(dt); QT::Draw(this); }

    void Update(float dt)
    {
        if (!paused) { UpdateQT(dt); }
        if (GetMouse(0).bPressed) { QT::AddAnt(GetMouseX(), GetMouseY()); }
        if (GetKey(olc::Key::ENTER).bPressed) { UpdateQT(dt); }
        if (GetKey(olc::Key::R).bPressed) { QT::tree->Reset(); }
        if (GetKey(olc::Key::P).bPressed) { paused = !paused; }
        if (GetKey(olc::Key::A).bPressed) { for (int i = 0; i < 10; i++) { QT::AddAnt(GetMouseX()+(+rand()%50)-25, GetMouseY()+(rand()%50)-25); } }

        if (GetKey(olc::Key::K1).bPressed) { var::show_quad = !var::show_quad; }
        if (GetKey(olc::Key::K2).bPressed) { var::show_point = !var::show_point; }
        if (GetKey(olc::Key::K3).bPressed) { var::show_radius = !var::show_radius; }
        if (GetKey(olc::Key::K4).bPressed) { var::show_senses = !var::show_senses; }

        QT::Draw(this);
    }

    bool OnUserCreate() override
    { return true; }
    
    bool OnUserUpdate(float fElapsedTime) override
    { Clear(olc::BLACK); Update(fElapsedTime); return true; }

};
