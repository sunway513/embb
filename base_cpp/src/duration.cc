/*
 * Copyright (c) 2014-2015, Siemens AG. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <embb/base/c/internal/unused.h>
#include <embb/base/duration.h>
#include <embb/base/c/duration.h>
#include <embb/base/exceptions.h>
#include <embb/base/c/internal/platform.h>
#include <limits>
#include <cassert>

namespace embb {
namespace base {

void Tick::CheckExceptions(int status, const char* msg) {
  switch (status) {
  case EMBB_SUCCESS: return;
  case EMBB_OVERFLOW: EMBB_THROW(OverflowException, msg);
  case EMBB_UNDERFLOW: EMBB_THROW(UnderflowException, msg);
  default: EMBB_THROW(ErrorException, msg);
  }
}

int Seconds::Set(embb_duration_t& duration, unsigned long long ticks) {
  return embb_duration_set_seconds(&duration, ticks);
}

void Seconds::SetAndCheck(embb_duration_t& duration, unsigned long long ticks) {
  int status = Set(duration, ticks);
  CheckExceptions(status, "Setting duration from seconds");
}

unsigned long long Seconds::Get(const embb_duration_t& duration) {
  unsigned long long ticks = 0;
  int status = embb_duration_as_seconds(&duration, &ticks);
  assert(status == EMBB_SUCCESS);
  EMBB_UNUSED_IN_RELEASE(status);
  return ticks;
}

unsigned long long Seconds::Min() {
  return 1;
}

unsigned long long Seconds::Max() {
  return EMBB_DURATION_MAX_SECONDS;
}

int Milliseconds::Set(embb_duration_t& duration, unsigned long long ticks) {
  return embb_duration_set_milliseconds(&duration, ticks);
}

void Milliseconds::SetAndCheck(
  embb_duration_t& duration, unsigned long long ticks) {
  int status = Set(duration, ticks);
  CheckExceptions(status, "Setting duration from milliseconds");
}

unsigned long long Milliseconds::Get(const embb_duration_t& duration) {
  unsigned long long ticks = 0;
  int status = embb_duration_as_milliseconds(&duration, &ticks);
  assert(status == EMBB_SUCCESS);
  EMBB_UNUSED_IN_RELEASE(status);
  return ticks;
}

unsigned long long Milliseconds::Min() {
#if EMBB_DURATION_MIN_NANOSECONDS > 1000000
  assert(EMBB_DURATION_MIN_NANOSECONDS % 1000000 == 0);
  return EMBB_DURATION_MIN_NANOSECONDS / 1000000;
#endif
  return 1;
}

unsigned long long Milliseconds::Max() {
#if EMBB_DURATION_MAX_SECONDS < ULLONG_MAX / 1000
  return ULLONG_MAX;
#else
  return EMBB_DURATION_MAX_SECONDS * 1000;
#endif
}

int Microseconds::Set(embb_duration_t& duration, unsigned long long ticks) {
  return embb_duration_set_microseconds(&duration, ticks);
}

void Microseconds::SetAndCheck(
  embb_duration_t& duration, unsigned long long ticks) {
  int status = Set(duration, ticks);
  CheckExceptions(status, "Setting duration from microseconds");
}

unsigned long long Microseconds::Get(const embb_duration_t& duration) {
  unsigned long long ticks = 0;

  int status = embb_duration_as_microseconds(&duration, &ticks);

  assert(status == EMBB_SUCCESS);
  EMBB_UNUSED_IN_RELEASE(status);
  return ticks;
}

unsigned long long Microseconds::Min() {
#if EMBB_DURATION_MIN_NANOSECONDS > 1000
  assert(EMBB_DURATION_MIN_NANOSECONDS % 1000 == 0);
  return EMBB_DURATION_MIN_NANOSECONDS / 1000;
#endif
  return 1;
}

unsigned long long Microseconds::Max() {
#if EMBB_DURATION_MAX_SECONDS < ULLONG_MAX / 1000000
  return ULLONG_MAX;
#else
  return EMBB_DURATION_MAX_SECONDS * 1000000;
#endif
}

int Nanoseconds::Set(embb_duration_t& duration, unsigned long long ticks) {
  return embb_duration_set_nanoseconds(&duration, ticks);
}

void Nanoseconds::SetAndCheck(
  embb_duration_t& duration, unsigned long long ticks) {
  int status = Set(duration, ticks);
  CheckExceptions(status, "Setting duration from microseconds");
}

unsigned long long Nanoseconds::Get(const embb_duration_t& duration) {
  unsigned long long ticks = 0;
  int status = embb_duration_as_nanoseconds(&duration, &ticks);
  assert(status == EMBB_SUCCESS);
  EMBB_UNUSED_IN_RELEASE(status);
  return ticks;
}

unsigned long long Nanoseconds::Min() {
  return EMBB_DURATION_MIN_NANOSECONDS;
}

unsigned long long Nanoseconds::Max() {
#if EMBB_DURATION_MAX_SECONDS < ULLONG_MAX / 1000000000
  return ULLONG_MAX;
#else
  return EMBB_DURATION_MAX_SECONDS * 1000000000;
#endif
}

} // namespace base
} // namespace embb
