#include <stdio.h>
#include <math.h>
#include <random>
using namespace std;

class Point {
protected:
    int x, y, z;
public:
    Point(): x(0), y(0), z(0) {
        printf("Point()\n");
    }
    Point(int x, int y, int z) : x(x), y(y), z(z) {
        printf("Point(int x, int y, int z)\n");
    }
    Point(const Point& p) : x(p.x), y(p.y), z(p.z) {
        printf("Point(const Point &p)\n");
    }
    virtual ~Point() {
        printf("%d, %d, %d\n", x, y, z);
        printf("~Point()\n");
    }
    void move(int dx, int dy, int dz) {
        x = x + dx;
        y = y + dy;
        z = z + dz;
    }
    void rLength();

    void showMe() {
        printf("%d, %d, %d\n", x, y, z);
    }
};

void Point::rLength() {
    printf("rLength = %f\n", sqrt(pow(x,2) + pow(y,2) + pow(z,2)));
}

class UndefinedPoint: public Point {
private:
    int range;
public:
    UndefinedPoint() : Point(), range(0) {
        printf("UndefinedPoint()\n");
    }
    UndefinedPoint(int x, int y, int z, int randomRange) : Point(x, y, z), range(randomRange) {
        printf("UndefinedPoint(int x, int y, int z)\n");
    }
    UndefinedPoint(const UndefinedPoint& p) : Point(p.x, p.y, p.z), range(p.range) {
        printf("UndefinedPoint(const Point &p)\n");
    }
    ~UndefinedPoint() {
        printf("%d, %d, %d, Range = %d\n", x, y, z, range);
        printf("~UndefinedPoint()\n");
    }
    void changeRange(int randomRange) {
        range = randomRange;
    }
    void defineLocation() {
        x += rand() % (2 * range + 1) - range;
        y += rand() % (2 * range + 1) - range;
        z += rand() % (2 * range + 1) - range;
    }
};

class Triangle {
protected:
    Point* p1;
    Point* p2;
    Point* p3;
public:
    Triangle() : p1(new Point(1,0,0)), p2(new Point(0,1,0)), p3(new Point(0,0,1)) {
        printf("Triangle()\n");
    }
    Triangle(Point p1, Point p2, Point p3) : p1(&p1), p2(&p2), p3(&p3) {
        printf("Triangle(Point p1, Point p2, Point p3)\n");
    }
    Triangle(const Triangle& t) : p1(t.p1), p2(t.p2), p3(t.p3) {
        printf("Triangle(const Triangle& t)\n");
        /* Глубокое
        p1 = new Point(*(t.p1));
        p2 = new Point(*(t.p2));
        p3 = new Point(*(t.p3));
        */
    }
    ~Triangle() {
        printf("~Triangle()\n");
        delete p1;
        delete p2;
        delete p3;
    }
    void showPoints() {
        p1->showMe();
        p2->showMe();
        p3->showMe();
    }
};

class ExamplePoint : public Point {};

int main()
{
    printf("Static objects:\n");
    {
        Point p;
        p.rLength();
        p.move(5, 10, 15);
        p.rLength();
        Point p2(1, 2, 3);
        Point p3(p2);
    }
    printf("Dynamic objects:\n");
    {
        Point *p = new Point;
        p->move(5, 10, 15);
        Point *p2 = new Point(1, 2, 3);
        Point *p3 = new Point(*p2);
        delete p;
        delete p2;
        delete p3;
    }
    printf("UndefinedPoint:\n");
    {
        UndefinedPoint* p = new UndefinedPoint(10, 20, 30, 5);
        Point* p2 = new UndefinedPoint(*p);
        p->defineLocation();
        p2->rLength();
        delete p;
        delete p2;
        UndefinedPoint p3;
    }
    printf("Triangle:\n");
    {
        Triangle* t1 = new Triangle;
        Triangle* t2 = new Triangle(*t1);
        delete t1;
        t2->showPoints();
        //При поверхностном копировании не создаются новые точки, создаются ссылки на старые точки
        //Поэтому после удаления точек t1 точек t2 не существует, остаются ссылки на место где лежит уже что-то другое
        
    }
    printf("ExamplePoint:\n");
    {
        ExamplePoint *ep = new ExamplePoint();
        delete ep;
    }
    printf("End\n");
}
