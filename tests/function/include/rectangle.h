#ifndef RECT_H
#define RECT_H
#include <iostream>
#include <vector>

class Rectangle {
  public:
    int width, height;
	std::vector<int> vec;
    Rectangle (int,int);
    int area () {return (width*height);}
};

Rectangle::Rectangle (int a, int b) {
	width = a;
	height = b;
	vec.push_back(0);
}
#endif /* RECT_H */
