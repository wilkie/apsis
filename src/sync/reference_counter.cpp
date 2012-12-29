#include "iso-tasty/sync/reference_counter.h"

Apsis::Sync::ReferenceCounter::ReferenceCounter() {
  _counter = new Apsis::Sync::AtomicCounter(1);
}

Apsis::Sync::ReferenceCounter::ReferenceCounter(const ReferenceCounter& b) :
  _counter(b._counter) {
  _counter->increment();
}

Apsis::Sync::ReferenceCounter& Apsis::Sync::ReferenceCounter::operator= (const Apsis::Sync::ReferenceCounter& b) {
  // Decrement our counter
  if (isAlone()) {
    delete _counter;
  }
  else {
    _counter->decrement();
  }

  // Transfer counter
  _counter = b._counter;

  // Increment
  _counter->increment();

  return *this;
}

Apsis::Sync::ReferenceCounter::~ReferenceCounter() {
  if (isAlone()) {
    delete _counter;
  }
  else {
    _counter->decrement();
  }
}

bool Apsis::Sync::ReferenceCounter::isAlone() const {
  return _counter->value() == 1;
}

unsigned int Apsis::Sync::ReferenceCounter::value() const {
  return _counter->value();
}