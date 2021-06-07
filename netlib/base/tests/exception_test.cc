#include "netlib/base/Exception.h"
#include "netlib/base/CurrentThread.h"

#include <functional>
#include <vector>
#include <iostream>

class Bar
{
public:
    void test(std::vector<std::string> names = {}){
        std::cout << netlib::CurrentThread::stackTrace(true) << "\n";
        [] {
            std::cout << "stack inside lambad : " << netlib::CurrentThread::stackTrace(true) << "\n";
        }();
        std::function<void()> func([] {
            std::cout << "stack inside std::function: "
                      << netlib::CurrentThread::stackTrace(true) << "\n";
        });
        func();

        func = std::bind(&Bar::callback, this);
        func();

        throw netlib::Exception ("oops");
    }
private:
    void callback() {
        std::cout << "stack inside std::bind: "
                  << netlib::CurrentThread::stackTrace(true) << "\n";
    }
};

void foo() {
    Bar bar;
    bar.test();
}

int main() {
    try {
        foo();
    }
    catch(const netlib::Exception &e) {
        std::cout << "reason: " << e.what() << "\n";
        std::cout << "stack trace: " << e.stackTrace() << "\n";
    }
    return 0;
}