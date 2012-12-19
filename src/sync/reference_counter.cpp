#include "iso-tasty/sync/reference_counter.h"

IsoTasty::Sync::ReferenceCounter::ReferenceCounter() {
  _counter = new IsoTasty::Sync::AtomicCounter(1);
}

IsoTasty::Sync::ReferenceCounter::ReferenceCounter(const ReferenceCounter& b) :
  _counter(b._counter) {
  _counter->increment();
}

IsoTasty::Sync::ReferenceCounter::~ReferenceCounter() {
  if (isAlone()) {
    delete _counter;
  }
  else {
    _counter->decrement();
  }
}

bool IsoTasty::Sync::ReferenceCounter::isAlone() const {
  return _counter->value() == 1;
}

unsigned int IsoTasty::Sync::ReferenceCounter::value() const {
  return _counter->value();
}