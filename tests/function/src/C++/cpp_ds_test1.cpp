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
#include <sstream>
#include <stdfloat>

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
static void templateClassTest(), templateClassTest2(), nestedTemplateClassTest();
static void obscureDataTypes(), virtualFuncTest();
static void inheritanceTest(), enumTest(), nestedTypedef(),anonymousClass();
static void anonymousNamespace(), mapsTest(), vectorOfSmartPtr(), lists(), functionPointers(), nans();
static float refs();

static void trap() {
    std::cout << "trap function\n";
}

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
    void print() const  {
        auto first = this, p = this;
        do {
            std::cout << (p == first ? "(" : ",") << p->val;
        } while ((p = p->next) != first);
        std::cout << ")\n" << std::flush;
    }
    std::string toString() const {
        auto os = std::ostringstream{};
        auto first = this, p = this;
        do {
            os << (p == first ? "(" : ",") << p->val;
        } while ((p = p->next) != first);
        os << ")";
        return os.str();
    }
        
private:
    int val;
    LinkedList* next;
};

void printList(LinkedList* list) {
    if (list) list->print();
    else std::cout << "NULL\n" << std::flush;
}
std::string toString(LinkedList* list) {
    return (list ? list->toString() : "");
}

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
    std::float16_t f16_t = static_cast<std::float16_t>(3.14159265);
    std::bfloat16_t b16_t = static_cast<std::bfloat16_t>(2.00403123);
    std::vector<_Float16> vf16;
    _Float16 f16 = static_cast<_Float16>(1.234567);
    std::vector<double> vd;
    std::vector<int *> ip;
    std::vector<Rectangle > vv;
    LinkedList* ll = new LinkedList;
    std::string ss{"So this grasshopper walks into a bar.."}, ss2{"hey: \"embedded quotes\""};
    while(i < 10 ){
        vv.push_back(rect);
        ip.push_back((int*)0x1234);
        vd.push_back(i+.5);
        vf.push_back(i+0.521232132);
        vf16.push_back(static_cast<_Float16>(i+0.521232132));
        l.push_back(i);
        d.push_back(i);
        s.push(i);
        q.push(i);
        pq.push(i);  m.insert(std::make_pair((17*i)%13,i));
        rect.width++;
        rect.height++;
        rect.vec.push_back(++i);
        (void) rect.vec.at(0);
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
    templateClassTest2();
    inheritanceTest();
    nestedTemplateClassTest();
    enumTest();
    nestedTypedef();
    anonymousClass();
    virtualFuncTest();
    anonymousNamespace();
    mapsTest();
    vectorOfSmartPtr();
    refs(); lists(); functionPointers(); nans();
    
    delete ll;
    MPI_Finalize();
    return 0;
}

static void f(const Struct1& s, const Classy& c)
{
  int i = 0;
  trap();
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
    trap();
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

std::string global_string = "global_string";

static void templateClassTest() {

    Moderne<int> mi(5,3,0);
    Moderne<double> md(-12.0, 16.2, 17.1);

    std::vector<Moderne<int>> vmi;
    vmi.push_back(Moderne<int>(1,2,3));
    vmi.push_back(Moderne<int>(3,2,0));

    size_t len = mi.getLen();
    size_t lend = md.getLen();
    trap();
}

union ux_t { int i; double d; void* p; char* s; };

static void obscureDataTypes() {
    int8_t i8 = 22;
    uint8_t ui8 = 254;
    uint16_t ui16 = 14222;
    uint32_t ui32 = 1 << 30;
    uint64_t ui64 = uint64_t(1) << 62;
    ux_t ux;
    ux.i = 100;
    trap();

    i8 = -i8;
    ui16 = 1 << 15;
    ui32 <<= 1;
    ui64 <<= 1;
    size_t size64 = sizeof(uint64_t);
    ux.d = -34.25;
    ux.i = 300;
    ux.i = 300;
    trap();
}

template <class T, class U>
class Moderne2
{
public:
    Moderne2(T val1, U val2) : t(val1,val1+1,val1+2), u(val2, val2*2+1, val2*3+2) {}
private:
    Moderne<T> t;
    Moderne<U> u;
};

static void templateClassTest2() {
    Moderne2<double,int> mi2(1.0, -8);

    std::vector<Moderne2<double,int>> vmi2;
    vmi2.push_back(Moderne2<double,int>(16.0,117));
    vmi2.push_back(Moderne2<double,int>(4.0,100));

    size_t len = vmi2.size();
    trap();
}

class LeftBase {
public:
    int leftInt = -1;
    float leftFloat = -1.1f;
private:
    double leftDouble = -11.11f;
};

class RightBase {
protected:
    int rightInt = 1;
    float rightFloat = 1.1f;
private:
    double rightDouble = 11.11f;
};

class LinearDerived : public LeftBase {
public:
    int derivedInt = 2;
    float derivedFloat = 2.2f;
};

class GrandChild : protected LinearDerived {
private:
    int grandChildInt = 3;
};

class PrivateGrandChild : private LinearDerived {
private:
    int grandChildInt = 33;
};

class MultiDerived : public LeftBase, protected RightBase {
public:
    int derivedInt = 4;
    float derivedFloat = 4.4f;
};

static void inheritanceTest() {
    auto LB = LeftBase{};
    auto RB = RightBase{};
    auto LD = LinearDerived{};
    auto GC = GrandChild{};
    auto PGC = PrivateGrandChild{};
    auto MD = MultiDerived{};
    trap();
}

namespace excelsior {
template <class T> class Luxure {
public:
    template <class U> struct Nested {
        T tval;
        U uval;
    };
};
}

static void nestedTemplateClassTest() {
    using namespace excelsior;

    auto s = Luxure<int>::Nested<float>{ -104, 1.2981f };
    trap();
}

enum OldEnum { BONY=4, MA, RONY=6 };
enum class ClassEnum { WHACK=-1, A, DOODLE };
enum class CharEnum : char { A, B, C };
struct AroundStruct {
    enum Nested { BETTY, VERONICA, JUGHEAD };
};

static void enumTest() {
    OldEnum o1 = BONY, o2 = MA, o3 = RONY;
    ClassEnum c1 = ClassEnum::WHACK, c2 = ClassEnum::A, c3 = ClassEnum::DOODLE;
    CharEnum ch1 = CharEnum::A, ch2 = CharEnum::B, ch3 = CharEnum::C;
    AroundStruct::Nested n1 = AroundStruct::BETTY, n2 = AroundStruct::VERONICA, n3 = AroundStruct::JUGHEAD;
    trap();
};

class Classy2 {
public:
  using Double = double;
  Classy2(Double ax, int ay) : x(ax), y(ay), n(15) {}

  struct Nesty {
      typedef int Int;
      Nesty(Int a) : ch_ch_ch_cherry_bomb(a) {}
      Int ch_ch_ch_cherry_bomb;
  };
  static void setZZ(int val) { zz = val; }
private:
  Double x;
  Double y;
  Nesty n;
  static int zz;
};

int Classy2::zz = 1;

static void nestedTypedef() {
  Classy2 c2{5.0, -121};
  Classy2::setZZ(1865);
  trap();
}

static void anonymousClass() {
  class {
    int i = 0;
  public:
    void setI(int ii) { i = ii; }
  } obj;
  auto op = &obj;
  obj.setI(5);
  trap();
}

class PureVirtual {
public:
    PureVirtual(int m);
    virtual ~PureVirtual();
    virtual void f() = 0;
protected:
    int baseMember;
};

PureVirtual::PureVirtual(int m) : baseMember(m) {}
PureVirtual::~PureVirtual() {}

class Derived : public PureVirtual {
public:
    Derived(int a, double x);
    void f() override;
private:
    double dmember;
};

Derived::Derived(int a, double x) :
    PureVirtual(a), dmember(x) {}

void Derived::f() {
    trap();
}

static void virtualFuncTest()
{
    Derived a(122, -17.1);
    Derived &b = a;
    Derived* p = &a;
    auto sp = std::make_shared<Derived>(16, 101.25);
    auto bsp = std::shared_ptr<PureVirtual>{sp};
    auto bp = bsp.get();
    a.f();
    
}

namespace {
    class AnonBaseClass {
    public:
        AnonBaseClass(int a) : m_a(a) {}
        virtual ~AnonBaseClass() {}
    private:
        int m_a;
    };
    class AnonDerivedClass : private AnonBaseClass {
    public:
        AnonDerivedClass(double x) : AnonBaseClass(116), m_x(x) {}
    private:
        double m_x;
    };
}

static void anonymousNamespace()
{
    auto d = AnonDerivedClass{101.125};
    auto dp = new AnonDerivedClass{-104.125};
    auto dsp = std::make_shared<AnonDerivedClass>(66.5);

    delete dp;
}

#include <unordered_map>

static void mapsTest()
{
    auto m = std::map<int, int>{ {0,7},{10,-1},{-4,16},{101,0}};
    auto mm = std::multimap<std::string, std::pair<int,int>>
        {
         {"host",{1,2}}, {"with",{3,4}}, {"the",{7,-4}}, {"most",{0,0}}, {"most",{2,3}}};
    auto um = std::unordered_map<int,double>{ {-4,1.2221}, {16,-8.0} };
    auto umm = std::unordered_multimap<int, std::string>{ {0,"in"}, {0,"the"},
                                                          {16,"corner"}, {16,"puts Baby"}};
    auto em = std::map<int,int>{};
    
    trap();
}

static void vectorOfSmartPtr() {
    auto up1 = std::unique_ptr<PureVirtual>{new Derived{-4,0.25}};
    auto ups = std::vector<std::unique_ptr<PureVirtual>>{};
    auto sp1 = std::shared_ptr<PureVirtual>{new Derived{21,-1.125}};
    auto sps = std::vector<std::shared_ptr<PureVirtual>>{};
    auto d = Derived{3,-101.0};
    auto p = &d;
    auto b = (PureVirtual*)p;
    auto pr = std::make_pair(true, sp1);
    auto vps = std::vector<std::pair<int,std::shared_ptr<PureVirtual>>>{};
    auto tup = std::tuple<int,double,std::pair<int,double>,char>{16,-12.25,{-8,14.4},'x'};

    for (int i=0; i<4; ++i) {
        ups.emplace_back(new Derived{i,1.0+i});
        sps.emplace_back(new Derived{-i,-1.0-i});
        vps.push_back(std::make_pair(i, sps.back()));
    }

    trap();
}

static float refs() {
    {   // lvalue refs
        auto s = Struct1{};
        s.x = 11;
        s.y = 22;
        auto cl = Classy(33, 44);
        auto m = Moderne<Struct1>(
            {.x = 55, .y = 66},
            {.x = 77, .y = 88},
            {.x = 99, .y = 00}
        );
        int ia[4] = {555, 666, 777, 888};
        char ca[12] = "lorem ipsum";
        int ia2[2][4] = {{111, 222, 333, 444}, {555, 666, 777, 888}};
        char ca2[2][12] = {"lorem ipsum", "dolor sit a"};

        const auto l = [&]() {
            cl = Classy(55, 66);
            m = Moderne<Struct1>(
                {.x = 555, .y = 666},
                {.x = 777, .y = 888},
                {.x = 999, .y = 0000}
            );
            auto ret =  s.y + ia[0] + ia2[1][1] + ca[0] + ca2[1][2];
            trap();
            return ret;
        }();
    }

    {   // rvalue refs
        auto s = Struct1{};
        s.x = 11;
        s.y = 22;
        auto cl = Classy(33, 44);
        auto m = Moderne<Struct1>(
            {.x = 55, .y = 66},
            {.x = 77, .y = 88},
            {.x = 99, .y = 00}
        );
        int ia[4] = {555, 666, 777, 888};
        char ca[12] = "lorem ipsum";
        int ia2[2][4] = {{111, 222, 333, 444}, {555, 666, 777, 888}};
        char ca2[2][12] = {"lorem ipsum", "dolor sit a"};

        const auto l = [](Struct1&& s, Classy&& cl, Moderne<Struct1>&& m,
			  int (&&ia)[4], char (&&ca)[12],
			  int (&&ia2)[2][4], char (&&ca2)[2][12]) {
            cl = Classy(55, 66);
            m = Moderne<Struct1>(
                {.x = 555, .y = 666},
                {.x = 777, .y = 888},
                {.x = 999, .y = 0000}
            );
            auto ret =  s.y + ia[0] + ia2[1][1] + ca[0] + ca2[1][2];
            trap();
            return ret;
        }(std::move(s), std::move(cl), std::move(m), std::move(ia), std::move(ca), std::move(ia2), std::move(ca2));
    }

    {   // length-encoded pointers
        int ia[2][2] = {{555, 666}, {777, 888}};
	int (*iap)[2] = ia;
	int (**iapp)[2] = &iap;
	int (***iappp)[2] = &iapp;
	auto iar = std::move(ia);

        char ca[2][6] = {"lorem", "ipsum"};
	char (*cap)[6] = &ca[1];
	char (**capp)[6] = &cap;
	char (***cappp)[6] = &capp;
	auto car = std::move(ca);

	// multi level function pointers and function references
	auto& fr = refs;
	auto* fp = refs;
	auto** fpp = &fp;
	auto*** fppp = &fpp;

	trap();
    }

    return 0;
}

static void lists() {
    auto l1 = std::list<int>{0,4,18,210};
    auto l2 = std::list<std::list<std::tuple<double,int>>>{};
    
    for (int i=0; i<4; ++i) {
        auto l3 = std::list<std::tuple<double,int>>{};
        for (int j=0; j<i; ++j) {
            l3.push_back(std::tuple<double,int>{1.5 * (i-j), i+j});
        }
        l2.push_back(std::move(l3));
    }

    auto empty = std::list<float>{};
    trap();

    return;
}

static char charFn() {
    return 'c';
}

static char* charPtrFn() {
    static char chars[] = {"qwerty"};
    return chars;
}

static void functionPointers() {
    // multi level function pointers and function references
    auto& vr = functionPointers;
    auto* vp = functionPointers;

    auto& cr = charFn;
    auto* cp = charFn;

    auto& cpr = charPtrFn;
    auto* cpp = charPtrFn;

    auto& fr = refs;
    auto* fp = refs;

    trap();
}

#include <cmath>
static void nans() {
    double ddiv = 1.0 / 0.0, dndiv = -1.0 / 0.0, di = NAN,
        dlogz = std::log(0.0), dlogneg = -NAN;

    double darr[] = { ddiv, dndiv, di, dlogz, dlogneg };

    float fdiv = 1.0f / 0.0f, fndiv = -1.0f / 0.0f;

    trap();
}
