#include <iostream>
#include "rectangle.h"
#include <unistd.h>
#include <fcntl.h>
#include <map>
#include <string>
int main(int argc, char ** argv){
	if(dup2(2,1) < 0)    return 1;
	int x[5] = {1,2,3,4,5};
	Rectangle rect (3,4);
	Rectangle rectb (5,6);
	int i=0;
	std::map<int, int> m;
	while(true){
	  rect.width++;
	  rect.height++;
	  rect.vec.push_back(++i);
	  rect.vec.at(0);
	  std::cout<<"i="<<i<<std::endl;
	  std::cout << "rect area: " << rect.area() << std::endl;
	  std::cout << "rectb area: " << rectb.area() << std::endl;
	  std::cout<<"";
	}
	return 0;
}
