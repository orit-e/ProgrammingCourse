#include <iostream>

using namespace std;

struct Base
{
    Base() {cout << "Base ctor" << endl;}
    virtual ~Base() {cout << "Base dtor" << endl;}
    Base(const Base&) {cout << "Base cctor" << endl;}
    virtual void operator=(const Base&) {cout << "Base =" << endl;}
    virtual void print() {cout << "Base print" << endl;}
};

struct Derived : public Base
{
    Derived() {cout << "Derived ctor" << endl;}
    virtual ~Derived() {cout << "Derived dtor" << endl;}
    Derived(const Derived&) : Base() {cout << "Derived cctor" << endl;}
    virtual void operator=(const Derived&) {cout << "Derived =" << endl;}
    virtual void print() {cout << "Derived print" << endl;}
};

void Print(Base& p)
{
    p.print();
}

int main()
{
    Base b;
    Base *pb = new Derived;

    Print(b);
    Print(*pb);
    pb->print();

    delete pb;
    //delete static_cast<Derived *>(pb);
    return 0;
}