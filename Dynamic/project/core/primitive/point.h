class Point
{

public:

    bool in_bounds = true;
    float bx, by;
    float bw, bh;
    float tx, ty;
    float vx, vy;
    float ax, ay;
    float  x,  y;
    float radius;
    float senses;
    float slimit;
    
    Point(float X, float Y, float R=0.5, float S=4.0, float L=0.5) { x=X; y=Y; radius=R; senses=S; slimit=L; var::born++; }
    ~Point() { var::dead++; }

    float Distance(Point* p) { float X=p->x-x, Y=p->y-y; return sqrt((X*X)+(Y*Y)); }
    bool Collision(Point* p) { return (Distance(p) <= radius+p->radius); }
    void Seek(float dt, float speed=0.5) { vx=(tx-x)*speed*dt; vy=(ty-y)*speed*dt; }
    void Evade(float dt, float speed=0.3) { vx=(x-tx)*speed*dt; vy=(y-ty)*speed*dt; }
    void Wander(float dt, float speed=0.1) { vx = ((rand()%10*0.01)-0.05)*speed*dt; vy = ((rand()%10*0.01)-0.05)*speed*dt; }

    void Think(float dt)
    {
        if (tx-x < senses*2 && ty-y < senses*2) { Seek(dt); }
        if (tx-x < senses/2 && ty-y < senses/2)     { Evade(dt); }
        else                                    { Wander(dt); }
    }

    void Update(float dt)
    {
        Think(dt);
        ax += vx*0.8; ay += vy*0.8;
        ax = std::max(std::min(ax, slimit), -slimit); ay = std::max(std::min(ay, slimit), -slimit);
        if (x > bx && x < bx+bw && y > by && y < by+bh) { in_bounds = true; x += ax; y += ay; }
        else { in_bounds = false; x += ax; y += ay; }
        if (x < 0)           { x += var::width-radius;  tx = -radius; }
        if (x > var::width)  { x -= var::width-radius;  tx =  radius; }
        if (y < 0)           { y += var::height-radius; ty = -radius; }
        if (y > var::height) { y -= var::height-radius; ty =  radius; }
    }
};
