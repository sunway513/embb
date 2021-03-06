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
#ifndef CONTAINERS_CPP_TEST_STACK_TEST_H_
#define CONTAINERS_CPP_TEST_STACK_TEST_H_

#include <vector>
#include <partest/partest.h>
#include <embb/base/duration.h>

namespace embb {
namespace containers {
namespace test {
template<typename Stack_t>
class StackTest : public partest::TestCase {
 private:
  int n_threads;
  int n_iterations;
  int n_stack_elements_per_thread;
  int n_stack_elements;
  Stack_t stack;
  std::vector<int> expected_stack_elements;
  std::vector<int>* thread_local_vectors;
  embb::base::Atomic<int> stackSize;

 public:
  StackTest();

  void StackTest1_Pre();

  void StackTest1_Post();

  void StackTest1_ThreadMethod();
};
} // namespace test
} // namespace containers
} // namespace embb

#include "./stack_test-inl.h"

#endif  // CONTAINERS_CPP_TEST_STACK_TEST_H_
