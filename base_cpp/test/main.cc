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

#include <partest/partest.h>
#include <embb/base/thread.h>
#include <duration_test.h>
#include <core_set_test.h>
#include <mutex_test.h>
#include <condition_var_test.h>
#include <thread_test.h>
#include <thread_specific_storage_test.h>
#include <atomic_test.h>
#include <memory_allocation_test.h>

#include <embb/base/c/memory_allocation.h>

using embb::base::test::CoreSetTest;
using embb::base::test::DurationTest;
using embb::base::test::ConditionVarTest;
using embb::base::test::MutexTest;
using embb::base::test::ThreadSpecificStorageTest;
using embb::base::test::AtomicTest;
using embb::base::test::MemoryAllocationTest;
using embb::base::test::ThreadTest;

PT_MAIN("Base C++") {
  unsigned int max_threads =
      static_cast<unsigned int>(2 * partest::TestSuite::GetDefaultNumThreads());
  embb_thread_set_max_count(max_threads);
  PT_RUN(CoreSetTest);
  PT_RUN(DurationTest);
  PT_RUN(ConditionVarTest);
  PT_RUN(MutexTest);
  PT_RUN(ThreadSpecificStorageTest);
  PT_RUN(AtomicTest);
  PT_RUN(MemoryAllocationTest);
  PT_RUN(ThreadTest);

  PT_EXPECT(embb_get_bytes_allocated() == 0);
}
