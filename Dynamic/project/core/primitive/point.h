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
    
    Point(float X, float Y, float R=4.0, float S=16.0) { x=X; y=Y; radius=R; senses=S; var::born++;}
    ~Point() { var::dead++; }

    float Distance(Point* p) { float X=p->x-x, Y=p->y-y; return sqrt((X*X)+(Y*Y)); }
    bool Collision(Point* p) { return (Distance(p) <= radius+p->radius); }
    void Seek(float speed=0.001) { vx=(tx-x)*speed; vy=(ty-y)*speed; }
    void Evade(float speed=0.001) { vx=(x-tx)*speed; vy=(y-ty)*speed; }
    void Wander(float speed=0.001) { vx = vx*speed; vy = vy*speed; }

    void Think()
    {
        if (tx-x < senses*2 && ty-y < senses*2) { Seek(); }
        if (tx-x < senses && ty-y < senses)     { Evade(); }
        else                                    { Wander(); }
    }

    void Update()
    {
        Think();
        ax += vx*0.8; ay += vy*0.8;
        if (x > bx && x < bx+bw && y > by && y < by+bh) { in_bounds = true; x += ax; y += ay; }
        else { in_bounds = false; x += ax; y += ay; }
        if (x < 0)           { x += var::width-radius;  tx = -radius; }
        if (x > var::width)  { x -= var::width-radius;  tx =  radius; }
        if (y < 0)           { y += var::height-radius; ty = -radius; }
        if (y > var::height) { y -= var::height-radius; ty =  radius; }
    }
};
