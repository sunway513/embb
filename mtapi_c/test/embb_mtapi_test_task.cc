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

#include <stdlib.h>

#include <embb_mtapi_test_config.h>
#include <embb_mtapi_test_task.h>

#include <embb/base/c/memory_allocation.h>
#include <embb/base/c/internal/unused.h>

#define JOB_TEST_TASK 42
#define TASK_TEST_ID 23

static void testTaskAction(
  const void* args,
  mtapi_size_t /*arg_size*/,
  void* /*result_buffer*/,
  mtapi_size_t /*result_buffer_size*/,
  const void* /*node_local_data*/,
  mtapi_size_t /*node_local_data_size*/,
  mtapi_task_context_t* task_context) {
  int ii;
  mtapi_uint_t core_num = mtapi_context_corenum_get(task_context, MTAPI_NULL);
  srand(core_num);
  for (ii = 1000; ii < rand()%1000000; ii ++) {
  }
  embb_mtapi_log_info("testTaskAction %d called from worker %d...\n",
    *reinterpret_cast<const int*>(args), core_num);
  EMBB_UNUSED(args);
}

static void testDoSomethingElse() {
}

TaskTest::TaskTest() {
  CreateUnit("mtapi task test").Add(&TaskTest::TestBasic, this);
}

void TaskTest::TestBasic() {
  mtapi_node_attributes_t node_attr;
  mtapi_action_attributes_t action_attr;
  mtapi_affinity_t affinity;
  mtapi_info_t info;
  mtapi_status_t status;
  mtapi_action_hndl_t action;
  mtapi_job_hndl_t job;
  mtapi_task_hndl_t task[100];
  int ii;

  embb_mtapi_log_info("running testTask...\n");

  status = MTAPI_ERR_UNKNOWN;
  mtapi_nodeattr_init(&node_attr, &status);
  MTAPI_CHECK_STATUS(status);

  status = MTAPI_ERR_UNKNOWN;
  mtapi_nodeattr_set(
    &node_attr,
    MTAPI_NODE_TYPE,
    MTAPI_ATTRIBUTE_VALUE(MTAPI_NODE_TYPE_SMP),
    MTAPI_ATTRIBUTE_POINTER_AS_VALUE,
    &status);
  MTAPI_CHECK_STATUS(status);

  status = MTAPI_ERR_UNKNOWN;
  mtapi_initialize(
    THIS_DOMAIN_ID,
    THIS_NODE_ID,
    &node_attr,
    &info,
    &status);
  MTAPI_CHECK_STATUS(status);

  embb_mtapi_log_trace("mtapi successfully initialized...\n");
  embb_mtapi_log_trace(
    "hardware concurrency   : %d\n", info.hardware_concurrency);
  embb_mtapi_log_trace("used memory            : %d\n", info.used_memory);

  status = MTAPI_ERR_UNKNOWN;
  mtapi_affinity_init(&affinity, MTAPI_TRUE, &status);
  MTAPI_CHECK_STATUS(status);

  status = MTAPI_ERR_UNKNOWN;
  mtapi_actionattr_init(&action_attr, &status);
  MTAPI_CHECK_STATUS(status);

  status = MTAPI_ERR_UNKNOWN;
  mtapi_actionattr_set(
    &action_attr,
    MTAPI_ACTION_AFFINITY,
    &affinity,
    MTAPI_ACTION_AFFINITY_SIZE,
    &status);
  MTAPI_CHECK_STATUS(status);

  status = MTAPI_ERR_UNKNOWN;
  action = mtapi_action_create(
    JOB_TEST_TASK,
    testTaskAction,
    MTAPI_NULL,
    0,
    &action_attr,
    &status);
  MTAPI_CHECK_STATUS(status);

  status = MTAPI_ERR_UNKNOWN;
  job = mtapi_job_get(JOB_TEST_TASK, THIS_DOMAIN_ID, &status);
  MTAPI_CHECK_STATUS(status);

  for (ii = 0; ii < 100; ii++) {
    status = MTAPI_ERR_UNKNOWN;
    int arg = ii;
    task[ii] = mtapi_task_start(
      TASK_TEST_ID,
      job,
      reinterpret_cast<const void*>(&arg),
      0,
      MTAPI_NULL,
      0,
      MTAPI_DEFAULT_TASK_ATTRIBUTES,
      MTAPI_GROUP_NONE,
      &status);
    MTAPI_CHECK_STATUS(status);
  }

  testDoSomethingElse();

  for (ii = 0; ii < 100; ii++) {
    status = MTAPI_ERR_UNKNOWN;
    mtapi_task_wait(task[ii], 100, &status);
    MTAPI_CHECK_STATUS(status);
  }

  status = MTAPI_ERR_UNKNOWN;
  mtapi_action_delete(action, 10, &status);
  MTAPI_CHECK_STATUS(status);

  status = MTAPI_ERR_UNKNOWN;
  mtapi_finalize(&status);
  MTAPI_CHECK_STATUS(status);

  PT_EXPECT(embb_get_bytes_allocated() == 0);

  embb_mtapi_log_info("...done\n\n");
}
