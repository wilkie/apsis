#include "apsis/sync/atomic_counter.h"

Apsis::Sync::AtomicCounter::AtomicCounter() {
  AtomicCounter(0);
}

Apsis::Sync::AtomicCounter::AtomicCounter(unsigned int value) :
  _value(value) {
}

unsigned int Apsis::Sync::AtomicCounter::increment() {
  return add((unsigned int)1);
}

unsigned int Apsis::Sync::AtomicCounter::decrement() {
  return add((unsigned int)-1);
}

unsigned int Apsis::Sync::AtomicCounter::add(unsigned int value) {
  unsigned int oldVal;
  unsigned int newVal;
  for (;;) {
    oldVal = _value;
    newVal = _value + value;
    if (_compareExchange(&_value, oldVal, newVal)) {
      return newVal;
    }
  }
}

unsigned int Apsis::Sync::AtomicCounter::value() const {
  return _value;
}

// TODO: Maybe do something with a mutex when architectures don't support things.
bool Apsis::Sync::AtomicCounter::_compareExchange(unsigned int* reference, unsigned int compare, unsigned int exchange) {
#ifdef _MSC_VER // Microsoft C++ Compiler
  __asm {
    // Stack:

    // |    old
    // |RA |ebp|ref|cmp|exc|
    //  EBP  +0  +4 +8  +12

    mov ECX, [EBP+16]; // assign ECX to exchange
    mov EAX, [EBP+12]; // assign accumulator to compare
    mov EDX, [EBP+8];  // get pointer to reference

    lock cmpxchg [EDX], ECX;

    // Afterward, EAX will still be compare if the update occurred
    // otherwise, EAX will be the reference value.

    // ZF is cleared when exchange occurs
    // Therefore set EAX to be 0 when the update did not occur, 1 otherwise
    setz AL;
  }
#elif defined(__GNUC__) // GNU C Compiler
#ifdef __x86_64__
  asm("mov  EAX, ESI;"
      "lock cmpxchg [RDI], EDX;"
      "setz AL;");
#else
  asm("mov ECX, [EBP+12];"
      "mov EAX, [EBP+8];"
      "mov EDX, [EBP+4];"
      "lock cmpxchg [EDX], ECX;"
      "setz AL;");
#endif
#endif
}
