#include <stdio.h>
#include <math.h>
#include <random>
using namespace std;

class Point {
protected:
    int x, y, z;
public:
    Point() {
        printf("Point()\n");
        x = 0;
        y = 0;
        z = 0;
    }
    Point(int x, int y, int z) {
        printf("Point(int x, int y, int z)\n");
        this->x = x;
        this->y = y;
        this->z = z;
    }
    Point(const Point& p) {
        printf("Point(const Point &p)\n");
        x = p.x;
        y = p.y;
        z = p.z;
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
    void length();
};

void Point::length() {
    printf("%f\n", sqrt(pow(x,2) + pow(y,2) + pow(z,2)));
}

class UndefinedPoint: public Point {
private:
    int range;
public:
    UndefinedPoint() : Point() {
        printf("UndefinedPoint()\n");
        range = 0;
    }
    UndefinedPoint(int x, int y, int z, int randomRange) : Point(x, y, z) {
        printf("UndefinedPoint(int x, int y, int z)\n");
        this->range = randomRange;
    }
    UndefinedPoint(const UndefinedPoint& p) {
        printf("UndefinedPoint(const Point &p)\n");
        x = p.x;
        y = p.y;
        z = p.z;
        range = p.range;
    }
    ~UndefinedPoint() {
        printf("%d, %d, %d, %d\n", x, y, z, range);
        printf("~UndefinedPoint()\n");
    }
    void changeRange(int randomRange) {
        range = randomRange;
    }
    void defineLocation() {
        x = rand() % (2 * range + 1) - range;
        y = rand() % (2 * range + 1) - range;
        z = rand() % (2 * range + 1) - range;
    }
};

int main()
{
    printf("Static objects:\n");
    {
        Point p;
        p.length();
        p.move(5, 10, 15);
        p.length();
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
        p2->length();
        delete p;
        delete p2;
        UndefinedPoint p3;
    }
    
    printf("End\n");    
}
