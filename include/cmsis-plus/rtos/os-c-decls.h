/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2016 Liviu Ionescu.
 *
 * µOS++ is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, version 3.
 *
 * µOS++ is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/*
 * This file is part of the CMSIS++ proposal, intended as a CMSIS
 * replacement for C++ applications.
 *
 * The structures declared in this file are used both in the C API
 * and in the legacy CMSIS API.
 *
 * Since there is no method to automatically sync them with the C++
 * definitions, they must be manually adjusted to match them, otherwise
 * the validation checks in os-c-wrapper.cpp will fail.
 */

#ifndef CMSIS_PLUS_RTOS_OS_C_DECLS_H_
#define CMSIS_PLUS_RTOS_OS_C_DECLS_H_

// ----------------------------------------------------------------------------

#include <cmsis-plus/rtos/os-app-config.h>
#include <cmsis-plus/rtos/port/os-c-decls.h>

#include <stdint.h>
#include <stddef.h>

// We definitely use CMSIS++.
#define OS_USE_CMSIS_PLUS

// ----------------------------------------------------------------------------

#ifdef  __cplusplus
extern "C"
{
#endif

  // --------------------------------------------------------------------------

#define OS_PRIOTHREAD_SIZE_PTR (OS_INTEGER_MAX_NUMBER_OF_THREADS+1)

  // --------------------------------------------------------------------------

  typedef uint32_t os_result_t;

  typedef uint32_t os_systicks_t;

  typedef struct os_thread_list_s
  {
    void* content[OS_PRIOTHREAD_SIZE_PTR];
  } os_thread_list_t;

  typedef uint32_t os_flags_mode_t;
  typedef uint32_t os_flags_mask_t;

  // --------------------------------------------------------------------------

#define OS_PRIOTHREAD_SHIFT   (0)

  enum
  {
    os_priority_none = 0, // not defined
    os_priority_idle = 1,
    os_priority_lowest = 2, // lowest
    os_priority_low = (2 << OS_PRIOTHREAD_SHIFT),
    os_priority_below_normal = (4 << OS_PRIOTHREAD_SHIFT),
    os_priority_normal = (6 << OS_PRIOTHREAD_SHIFT), // default
    os_priority_above_normal = (8 << OS_PRIOTHREAD_SHIFT),
    os_priority_high = (10 << OS_PRIOTHREAD_SHIFT),
    os_priority_realtime = (12 << OS_PRIOTHREAD_SHIFT),
    os_priority_highest = ((16 << OS_PRIOTHREAD_SHIFT) - 3),
    os_priority_isr = ((16 << OS_PRIOTHREAD_SHIFT) - 2),
    os_priority_error = ((16 << OS_PRIOTHREAD_SHIFT) - 1)
  };

  enum
  {
    os_thread_inactive = 0,
    os_thread_ready,
    os_thread_running,
    os_thread_waiting
  };

  enum
  {
    os_timer_once = 0, os_timer_periodic = 1
  };

  typedef void* os_thread_func_args_t;
  typedef void*
  (*os_thread_func_t) (os_thread_func_args_t args);

  typedef uint8_t os_thread_state_t;

  typedef uint8_t os_thread_prio_t;

  typedef os_flags_mask_t os_thread_sigset_t;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"

  typedef struct os_thread_attr_s
  {
    const char* name;
    void* stack_address;
    size_t stack_size_bytes;
    os_thread_prio_t priority;
  } os_thread_attr_t;

  typedef struct os_thread_s
  {
    const char* name;
    void* stack_addr;
    os_thread_func_t func;
    os_thread_func_args_t func_args;
    void* func_result_;
    void* impl;
    void* impl_event_flags;
    size_t stack_size_bytes;
    os_thread_state_t state;
    os_thread_prio_t prio;
    os_result_t wakeup_reason;
    os_thread_sigset_t signals;
    os_thread_user_storage_t user_storage;
  } os_thread_t;

#pragma GCC diagnostic pop

  // --------------------------------------------------------------------------

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"

  typedef struct os_systick_clock_current_s
  {
    uint64_t ticks; // Count of SysTick ticks since core reset
    uint32_t cycles; // Count of SysTick cycles since timer reload (24 bits)
    uint32_t divisor; // SysTick reload value (24 bits)
    uint32_t core_frequency_hz; // Core clock frequency Hz
  } os_systick_clock_current_t;

#pragma GCC diagnostic pop

  typedef uint64_t os_systick_clock_rep_t;
  typedef uint32_t os_systick_clock_sleep_rep_t;

  typedef uint64_t os_realtime_clock_rep_t;
  typedef uint32_t os_realtime_clock_sleep_rep_t;

  // --------------------------------------------------------------------------

  typedef void* os_timer_func_args_t;
  typedef void
  (*os_timer_func_t) (os_timer_func_args_t args);

  typedef uint8_t os_timer_type_t;

  enum
  {
    //
    os_timer_run_once = 0,
    os_timer_run_periodic = 1
  };

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"

  typedef struct os_timer_attr_s
  {
    const char* name;
    os_timer_type_t type;
  } os_timer_attr_t;

  typedef struct os_timer_s
  {
    const char* name;
    os_timer_func_t func;
    os_timer_func_args_t func_args;
#if defined(OS_INCLUDE_PORT_RTOS_TIMER)
    os_timer_port_data_t port_;
#endif
    os_timer_type_t type;
  } os_timer_t;

#pragma GCC diagnostic pop

  // --------------------------------------------------------------------------

  typedef int16_t os_mutex_count_t;
  typedef uint8_t os_mutex_type_t;
  typedef uint8_t os_mutex_protocol_t;
  typedef uint8_t os_mutex_robustness_t;

  enum
  {
    //
    os_mutex_protocol_none = 0,
    os_mutex_protocol_inherit = 1,
    os_mutex_protocol_protect = 2
  };

  enum
  {
    //
    os_mutex_robustness_stalled = 0,
    os_mutex_robustness_robust = 1
  };

  enum
  {
    //
    os_mutex_type_normal = 0,
    os_mutex_type_errorcheck = 1,
    os_mutex_type_recursive = 2,
  };

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"

  typedef struct os_mutex_attr_s
  {
    const char* name;
    os_thread_prio_t priority_ceiling;
    os_mutex_protocol_t protocol;
    os_mutex_robustness_t robustness;
    os_mutex_type_t type;
  } os_mutex_attr_t;

  typedef struct os_mutex_s
  {
    const char* name;
    void* owner;
#if defined(OS_INCLUDE_PORT_RTOS_MUTEX)
    os_mutex_port_data_t port;
#endif
    os_mutex_count_t count;
    os_thread_prio_t prio_ceiling;
    os_mutex_type_t type;
    os_mutex_protocol_t protocol;
    os_mutex_robustness_t robustness;
  } os_mutex_t;

#pragma GCC diagnostic pop

  // --------------------------------------------------------------------------

  typedef struct os_condvar_attr_s
  {
    const char* name;
  } os_condvar_attr_t;

  typedef struct os_condvar_s
  {
    const char* name;
  } os_condvar_t;

  // --------------------------------------------------------------------------

  typedef int16_t os_semaphore_count_t;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"

  typedef struct os_semaphore_attr_s
  {
    const char* name;
    os_semaphore_count_t initial_count;
    os_semaphore_count_t max_count;
  } os_semaphore_attr_t;

  typedef struct os_semaphore_s
  {
    const char* name;
#if !defined(OS_INCLUDE_PORT_RTOS_SEMAPHORE)
    os_thread_list_t list;
#endif
#if defined(OS_INCLUDE_PORT_RTOS_SEMAPHORE)
    os_semaphore_port_data_t port;
#endif
    os_semaphore_count_t initial_count;
    os_semaphore_count_t count;
    os_semaphore_count_t max_count;
  } os_semaphore_t;

#pragma GCC diagnostic pop

  // --------------------------------------------------------------------------

  typedef uint16_t os_mempool_size_t;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"

  typedef struct os_mempool_attr_s
  {
    const char* name;
    void* pool_addr;
  } os_mempool_attr_t;

  typedef struct os_mempool_s
  {
    const char* name;
#if !defined(OS_INCLUDE_PORT_RTOS_MEMORY_POOL)
    os_thread_list_t list;
#endif
    void* pool_addr;
#if defined(OS_INCLUDE_PORT_RTOS_MEMORY_POOL)
    os_mempool_port_data_t port;
#endif
    os_mempool_size_t blocks;
    os_mempool_size_t block_size_bytes;
    os_mempool_size_t count;
    void* first;
    uint8_t flags_;
  } os_mempool_t;

#pragma GCC diagnostic pop

  // --------------------------------------------------------------------------

  typedef uint16_t os_mqueue_size_t;
  typedef uint8_t os_mqueue_prio_t;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"

  typedef struct os_mqueue_attr_s
  {
    const char* name;
    void* queue_addr;
    size_t queue_size_bytes;
  } os_mqueue_attr_t;

  typedef struct os_mqueue_s
  {
    const char* name;
#if !defined(OS_INCLUDE_PORT_RTOS_MESSAGE_QUEUE)
    os_thread_list_t send_list;
    os_thread_list_t receive_list;
#endif

    void* queue_addr;

#if defined(OS_INCLUDE_PORT_RTOS_MESSAGE_QUEUE)
    os_mqueue_port_data_t port;
#endif

    size_t queue_size_bytes;

    os_mqueue_size_t msgs;
    os_mqueue_size_t msg_size_bytes;

    os_mqueue_size_t count;
  } os_mqueue_t;

#pragma GCC diagnostic pop

  // --------------------------------------------------------------------------

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"

  typedef struct os_evflags_attr_s
  {
    const char* name;
  } os_evflags_attr_t;

  typedef struct os_evflags_s
  {
    const char* name;
#if !defined(OS_INCLUDE_PORT_RTOS_EVENT_FLAGS)
    os_thread_list_t list;
#endif

#if defined(OS_INCLUDE_PORT_RTOS_EVENT_FLAGS)
    os_evflags_port_data_t port_;
#endif

    os_flags_mask_t flags;
  } os_evflags_t;

#pragma GCC diagnostic pop

  // --------------------------------------------------------------------------

  typedef struct os_mail_queue_s
  {
    os_mempool_t pool;
    os_mqueue_t queue;
  } os_mail_queue_t;

// ----------------------------------------------------------------------------

#ifdef  __cplusplus
}
#endif

#endif /* CMSIS_PLUS_RTOS_OS_C_STRUCTS_H_ */