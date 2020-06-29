#include <iostream>
#include "rectangle.h"
#include <unistd.h>
#include <fcntl.h>
#include <map>
#include <string>
#include "mpi.h"
#include <list>
#include <deque>
#include <stack>
#include <queue>
struct Struct1 {
  double x;
  int y;
};
class Classy {
public:
  Classy(double ax, int ay) : x(ax), y(ay) {}
private:
  double x;
  double y;
};
static void f(const Struct1& s, const Classy& c);

int main(int argc, char ** argv){
	int x[5] = {1,2,3,4,5};
	MPI_Init(&argc, &argv);
	if(dup2(2,1) < 0)    return 1;
	Rectangle rect (3,4);
	Rectangle rectb (5,6);
	int i=0;
	int area1=0;
	int area2=0;
	std::map<int, int> m;
	std::list<int> l;
	std::deque<int> d;
	std::stack<int> s;
	std::queue<int> q;
	std::priority_queue<int> pq;
	std::vector<float> vf;
	std::vector<double> vd;
	std::vector<int *> ip;
	std::vector<Rectangle > vv;
	while(i < 10 ){
		vv.push_back(rect);
		ip.push_back((int*)0x1234);
		vd.push_back(i+.5);
		vf.push_back(i+0.5);
		l.push_back(i);
		d.push_back(i);
		s.push(i);
		q.push(i);
		pq.push(i);
		rect.width++;
		rect.height++;
		rect.vec.push_back(++i);
		rect.vec.at(0);
		std::cout<<"i="<<i<<std::endl;
		area1 = rect.area();
		area2 = rectb.area();
		std::cout << "rect area: " << area1 << std::endl;
		std::cout << "rectb area: " << area2 << std::endl;
		std::cout<<"";
	}
       
	int l_tail = l.back();
	int pq_tail = pq.top();
	int stack_top=s.top();
	int q_queue_tail=q.back();
	int d_queue_tail=d.back();
	float vec_float=vf.back();
	double vec_double=vd.back();
	Struct1 s1 = { 22.1, -5 };
	Classy c1(22.2, -6);
	f(s1,c1);

	MPI_Finalize();
	return 0;
}

static void f(const Struct1& s, const Classy& c)
{
  int i = 0;
}
