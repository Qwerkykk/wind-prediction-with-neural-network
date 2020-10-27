#ifndef CURVEGRIDLSH_POINT_H
#define CURVEGRIDLSH_POINT_H

typedef struct point{
    double x;
    double y;

    point(double _x, double _y): x(_x), y(_y){}
    double getx(){ return x;}
    double gety(){ return y;}

} Point;

#endif //CURVEGRIDLSH_POINT_H
