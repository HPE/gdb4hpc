#include <iostream>
#include "rectangle.h"
#include <unistd.h>
#include <fcntl.h>
#include <map>
#include <string>
#include <memory>
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
static void variableSizeDataTest();
static void templateClassTest();
static void obscureDataTypes();
class LinkedList {
public:
    LinkedList() : val(0), next(this) {}
    ~LinkedList() { 
        for (auto n = next; n && n != this; ) {
            auto elem = n;
            n = n->next;
            elem->next = nullptr;
            delete elem;
        }
    }
    void push(int v) { 
        LinkedList* elem = new LinkedList;
        elem->val = v;
        elem->next = next;
        next = elem;
    }
private:
    int val;
    LinkedList* next;
};

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
    LinkedList* ll = new LinkedList;  std::string ss{"So this grasshopper walks into a bar.."};
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
		area1 = rect.area();
		area2 = rectb.area();
        ll->push((i*3)%5);
	}
    void* my_null = nullptr;       
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

    variableSizeDataTest();
    templateClassTest();
    obscureDataTypes();
    delete ll;

	MPI_Finalize();
	return 0;
}

static void f(const Struct1& s, const Classy& c)
{
  int i = 0;
}

int func2(int x, int y) 
{
    return x+2*y;
}
int func1(int x) 
{
    return 3 * x;
}

static size_t f2(const std::vector<std::string>& strings,
               const std::vector<std::vector<int>>& vecOfVecs,
               const std::vector<std::vector<std::string>> vecOfStrings) {
    size_t totalLen = strings.size() + vecOfVecs.size() + vecOfStrings.size();
    return totalLen;
}

static void variableSizeDataTest()
{
    std::vector<std::string> strings;
 
    // I won't assume C++17 compiler just yet
    const char* s[] = { "and", "then", "there", "were", "none" };
    int numStrs = sizeof(s)/sizeof(const char*);
    for (int i=0; i<numStrs; ++i) {
        strings.push_back(s[i]);
    }

    std::vector<std::vector<int>> vecOfVecs;
    for (int i=0; i<10; ++i) {
        std::vector<int> vals;
        for (int j=(10-i); j--; ) {
            vals.push_back(3*j);
        }
        vecOfVecs.push_back(vals); // not trying to be efficient
    }

    std::vector<std::vector<std::string>> vecOfStrings;
    int a = 0;
    for (int i=0; i<6; ++i) {
        std::vector<std::string> vals;
        for (int j=0; j<i; ++j) {
            vals.push_back(strings[(a++) % numStrs]);
        }
        vecOfStrings.push_back(vals);
    }

    // I just need to make sure I have some place to 
    f2(strings, vecOfVecs, vecOfStrings);
}

template <class T>
class Moderne {
public:
    Moderne(T a, T b, T c) {
        v.push_back(a);
        v.push_back(b);
        v.push_back(c);

        sp = std::make_shared<T>(a);
        up = std::make_unique<T>(c);
    }

    size_t getLen() const { return v.size(); }
private:
    std::vector<T> v;
    std::shared_ptr<T> sp;
    std::unique_ptr<T> up;
};



static void templateClassTest() {

    Moderne<int> mi(5,3,0);
    Moderne<double> md(-12.0, 16.2, 17.1);

    std::vector<Moderne<int>> vmi;
    vmi.push_back(Moderne<int>(1,2,3));
    vmi.push_back(Moderne<int>(3,2,0));

    size_t len = mi.getLen();
    size_t lend = md.getLen();
}

union ux_t { int i; double d; };

static void obscureDataTypes() {
    int8_t i8 = 22;
    uint8_t ui8 = 254;
    uint16_t ui16 = 14222;
    uint32_t ui32 = 1 << 30;
    uint64_t ui64 = uint64_t(1) << 62;
    ux_t ux;
    ux.i = 100;

    i8 = -i8;
    ui16 = 1 << 15;
    ui32 <<= 1;
    ui64 <<= 1;
    size_t size64 = sizeof(uint64_t);
    ux.d = -34.25;
    ux.i = 300;
    ux.i = 300;
}
