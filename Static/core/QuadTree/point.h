struct Point
{
    float x,y;
    Point(float X, float Y) { x=X; y=Y; }
    ~Point() { }
    void Update(float X, float Y) { x+=X; y+=Y; }
    float Distance(Point* p) { float X=p->x-x, Y=p->y-y; return sqrt((X*X)+(Y*Y)); }
};
