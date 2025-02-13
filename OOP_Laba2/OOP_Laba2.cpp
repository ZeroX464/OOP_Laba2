﻿#include <stdio.h>
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
    UndefinedPoint(const UndefinedPoint& p) : Point(p), range(p.range) {
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
    Triangle(Point p1, Point p2, Point p3) : p1(new Point(p1)), p2(new Point(p2)), p3(new Point(p3)) { //p1(&p1)
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

class Triangle2 {
protected:
    Point p1;
    Point p2;
    Point p3;
public:
    Triangle2() : p1(Point(1, 0, 0)), p2(Point(0, 1, 0)), p3(Point(0, 0, 1)) {
        printf("Triangle2()\n");
    }
    Triangle2(Point p1, Point p2, Point p3) : p1(p1), p2(p2), p3(p3) {
        printf("Triangle2(Point p1, Point p2, Point p3)\n");
    }
    Triangle2(const Triangle2& t) : p1(t.p1), p2(t.p2), p3(t.p3) {
        printf("Triangle2(const Triangle& t)\n");
    }
    ~Triangle2() {
        printf("~Triangle2()\n");
    }
    void showPoints() {
        p1.showMe();
        p2.showMe();
        p3.showMe();
    }
};

class ExamplePoint : public Point {};

int main()
{
    Point ps1(Point(1,1,1)), ps2(2,2,2), ps3(3,3,3);
    {
    Triangle t(ps1, ps2, ps3); // Здесь ps1, ps2, ps3 передаются "по значению" т.е. создаются копии этих объектов и уже они передаются в констурктор
    // В конструкторе создаются указатели на эти временные объекты
    // После окончания блока т.к. объект был создан статически, высвобождается вся память, в том числе вызываются деструкторы этих временных объектов
    // Поэтому если в деструкторе Triangle есть delete p1, p2, p3, то идёт попытка удаления временных объектов которых уже нету (или сначала delete, потом удаление временных)
    // И вылезает ошибка
    // Решение: в конструкторе с параметрами Triangle создавать динамически объекты, тогда после удаления временных объектов можно будет спокойно вызвать delete
    // Т.к. в аргументе delete будет указатель на новый объект
    }
    /*
    Point p1;
    Point* pk = &p1;
    delete pk; "Использование оператора delete на указателе на объект, который не был создан при помощи оператора new, создает непрогнозируемый результат." Microsoft
    */  
        printf("Static objects:\n");
    {
        Point p;
        p.rLength();
        p.move(5, 10, 15);
        p.rLength();
        Point p2(1, 2, 3);
        Point p3(p2);
    }
    //ps1.rLength();
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
        p2->showMe();
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
    printf("Triangle2:\n");
    {
        { Triangle2 t1; }
        printf("--------------\n");
        {
        Point p1, p2, p3;
        Triangle2 t2(p1, p2, p3);
        }
        printf("--------------\n");
        {
        Point* p4 = new Point(1, 1, 1);
        Point* p5 = new Point(2, 2, 2);
        Point* p6 = new Point(3, 3, 3);
        Triangle2 t3(*p4, *p5, *p6);
        delete p4;
        delete p5;
        delete p6;
        }
        printf("--------------\n");
        {
        Triangle2* t4 = new Triangle2();
        Triangle2* t5 = new Triangle2(*t4);
        delete t4;
        delete t5;
        }
    }
    printf("ExamplePoint:\n");
    {
        ExamplePoint *ep = new ExamplePoint();
        delete ep;
    }
    printf("End\n");
}
