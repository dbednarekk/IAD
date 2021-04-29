#ifndef POINT_H
#define POINT_H


class Point{
private:
    double x;
    double y;
public:
    Point(double x, double y);
    virtual ~Point();
    double getX();
    double getY();
    void setX(double x);
    void setY(double y);
};

#endif // POINT_H
