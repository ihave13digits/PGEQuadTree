struct Quad
{
    float x,y, w,h;
    Quad(float X, float Y, float W, float H) { x=X; y=Y; w=W; h=H; }
    ~Quad() {}
    bool Collision(Quad* q) { return (((x<=q->x+q->w && x>=q->x) || (x+w>=q->x && x+w<=q->x+q->w)) && ((y<=q->y+q->h && y>=q->y) || (y+h>=q->y && y+h<=q->y+q->h))); }
    bool PointInside(Point* p) { return ((p->x >= x && p->x <= x+w) && (p->y >= y && p->y <= y+h)); }
};
