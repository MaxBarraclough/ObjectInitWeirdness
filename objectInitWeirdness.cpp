/**
 * Exploring C++'s weird and wonderful object-initialisation rules,
 * as explained in http://stackoverflow.com/a/2418195
 *
 * Obviously, most of this tomfoolery should never see the light of day.
 *
 * Running on my AMD64 Ubuntu machine, using Clang++ 3.2 (+ LLVM 3.2) and g++ 4.8.1,
 * only g++ gives warnings on compilation (using -Wall and -Wextra for both compilers).
 *
 * cppcheck rightly warns about memory-leaks, but doesn't mind the initialisation madness.
 *
 * Also, g++ is harder to get 'real garbage' from; using g++, I see zeroes by happy coincidence
 * for the first two instances given, even with optimisation enabled.
 * With clang++, I'm able to see 'real garbage', but only if I enable optimisation
 * (be it with -Os -O1 -O2 or -O3)
 */

#include <stdio.h>

#define DEFINE_DEFAULT_CONSTRUCTOR 1


class MyClass {

public:
  int testInt;
  int anotherTestInt;

#ifdef DEFINE_DEFAULT_CONSTRUCTOR
  MyClass() : testInt(0), anotherTestInt(0) {
    puts("Constructor run");
  }
#endif

  //   MyClass() :  testInt(3),  anotherTestInt(4)  { puts("Constructing..."); }

  //   ~MyClass()   { puts("Destroying...");   }
};


int main() {

  // Default constructor is used; members contain garbage:
  {
    MyClass c;
    printf("%d,%d\n", c.testInt, c.anotherTestInt);
  }

  // Again: default constructor is used; members contain garbage:
  {
    MyClass *d = new MyClass;
    printf("%d,%d\n", d->testInt, d->anotherTestInt);
  }


  // In all of these, value initialisation (i.e. zero initialisation) is used:
  // (But if class were non-POD, constructor would be used.)
  // (Modulo compiler bugs, that is.)

  {
    MyClass *e = new MyClass();
    printf("%d,%d\n", e->testInt, e->anotherTestInt);
    delete e; // if we did meaningful work in the destructor,
        // we wouldn't want to destroy a garbage-filled object
  }

  {
    MyClass f = MyClass();
    printf("%d,%d\n", f.testInt, f.anotherTestInt);
  }

#ifndef DEFINE_DEFAULT_CONSTRUCTOR
// actually, in Visual Studio, this still compiles fine, but not in g++
  {
    MyClass g = {};
    printf("%d,%d\n", g.testInt, g.anotherTestInt);
  }
#endif

#ifndef DEFINE_DEFAULT_CONSTRUCTOR
  { // Illegal iff a default constructor is explicitly defined:
    // 'non-aggregate type cannot be initialized with an initializer list'
    MyClass h = {0};
    printf("%d,%d\n", h.testInt, h.anotherTestInt);
  }
#endif

  puts("All done, terminating...");
}

