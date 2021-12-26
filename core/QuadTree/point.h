struct Point
{
    float x,y;
    Point(float X, float Y) { x=X; y=Y; born++; }
    ~Point() { died++; }
    void Update(float X, float Y) { x+=X; y+=Y; }
    float Distance(Point p) { float X=x-p.x, Y=y-p.y; return sqrt((X*X)+(Y*Y)); }
};
