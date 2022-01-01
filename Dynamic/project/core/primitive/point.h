class Point
{

public:

    bool in_bounds = true, is_dead = false;
    float bx, by;
    float bw, bh;
    float tx, ty;
    float vx, vy;
    float ax, ay;
    float  x,  y;
    float radius;
    float senses;
    float slimit;
    
    Point(float X, float Y, float R=4.0, float S=8.0, float L=0.5) { x=X; y=Y; radius=R; senses=S; slimit=L; QT::born++; }
    ~Point() { QT::dead++; }

    float Distance(Point* p) { float X=p->x-x, Y=p->y-y; return sqrt((X*X)+(Y*Y)); }
    bool Collision(Point* p) { return (Distance(p) <= radius+p->radius); }
    void Die() { is_dead = true; }
    void Seek(float dt, float speed) { vx=(tx-x)*speed*dt; vy=(ty-y)*speed*dt; }
    void Evade(float dt, float speed) { vx=(x-tx)*speed*dt; vy=(y-ty)*speed*dt; }
    void Wander(float dt, float speed) { vx = (float((rand()%101)*0.01)-0.5)*speed*dt; vy = (float((rand()%101)*0.01)-0.5)*speed*dt; }

    void Think(float dt)
    {
        if      (tx-x >= senses*4 && ty-y >= senses*4) { Wander(dt, 1.0); }
        else if (tx-x >= senses*2 && ty-y >= senses*2) { Seek(dt,   0.5); }
        else if (tx-x >= senses   && ty-y >= senses  ) { Seek(dt,   0.1); }
        else if (tx-x <= radius*2 && ty-y <= radius*2) { Evade(dt,  2.5); }
        else if (tx-x <= radius   && ty-y <= radius  ) { Evade(dt,  5.0); }
        else                                           { Wander(dt, 0.1); }
    }

    void Update(float dt)
    {
        Think(dt);
        ax += vx*0.8; ay += vy*0.8;
        ax = std::max(std::min(ax, slimit), -slimit); ay = std::max(std::min(ay, slimit), -slimit);
        if (x > bx && x < bx+bw && y > by && y < by+bh) { in_bounds = true; x += ax; y += ay; }
        else { in_bounds = false; x += ax; y += ay; }
        if (x < 0)           { x += var::width-radius;  tx =  radius; }
        if (x > var::width)  { x -= var::width-radius;  tx = -radius; }
        if (y < 0)           { y += var::height-radius; ty =  radius; }
        if (y > var::height) { y -= var::height-radius; ty = -radius; }
    }
};
