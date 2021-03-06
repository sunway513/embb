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

#ifndef EMBB_DATAFLOW_INTERNAL_SCHEDULER_MTAPI_H_
#define EMBB_DATAFLOW_INTERNAL_SCHEDULER_MTAPI_H_

#include <embb/dataflow/internal/action.h>
#include <embb/dataflow/internal/scheduler.h>
#include <embb/mtapi/node.h>
#include <embb/base/function.h>

namespace embb {
namespace dataflow {
namespace internal {

template <int Slices>
class SchedulerMTAPI : public Scheduler {
 public:
  SchedulerMTAPI() {
    embb::mtapi::Node & node = embb::mtapi::Node::GetInstance();
    for (int ii = 0; ii < Slices; ii++) {
      embb::mtapi::Group & group = node.CreateGroup();
      group_[ii] = &group;
    }
  }
  virtual ~SchedulerMTAPI() {
    embb::mtapi::Node & node = embb::mtapi::Node::GetInstance();
    for (int ii = 0; ii < Slices; ii++) {
      group_[ii]->WaitAll(MTAPI_INFINITE);
      node.DestroyGroup(*group_[ii]);
    }
  }
  virtual void Spawn(Action & action) {
    const int idx = action.GetClock() % Slices;
    group_[idx]->Spawn(embb::base::MakeFunction(action, &Action::RunMTAPI));
  }
  virtual void WaitForSlice(int slice) {
    group_[slice]->WaitAll(MTAPI_INFINITE);
  }

 private:
  embb::mtapi::Group * group_[Slices];
};

} // namespace internal
} // namespace dataflow
} // namespace embb

#endif // EMBB_DATAFLOW_INTERNAL_SCHEDULER_MTAPI_H_
