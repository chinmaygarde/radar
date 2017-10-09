/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Core/Timing.h>
#include <cerrno>
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>

/*
 *  Logging.
 */

#define _RL_FILE_LAST_COMPONENT(file) \
  (strrchr(file, '/') ? strrchr(file, '/') + 1 : file)

#define _RL_LOG_FMT "%.3fs %s:%d: "
#define _RL_LOG_ARG                                \
  rl::core::clock::LoggingClockDuration().count(), \
      _RL_FILE_LAST_COMPONENT(__FILE__), __LINE__

/**
 *  Logs the given message to the console.
 *
 *  @param message the message to log.
 *  @param args    optional arguments to the message in printf format.
 */
#define RL_LOG(message, ...) \
  printf(_RL_LOG_FMT message "\n", _RL_LOG_ARG, ##__VA_ARGS__);

#define RL_LOG_ERRNO() RL_LOG("UNIX Error: %s (%d)", strerror(errno), errno)
#define RL_LOG_MACHERROR(res) RL_LOG("Mach Error: %s", mach_error_string(res))

/**
 *  Log the current line to the console.
 */
#define RL_LOG_HERE RL_LOG("%s", __FUNCTION__)

/*
 *  Assertions.
 */

static inline void _RL_AssertLog(const char* file,
                                 int line,
                                 const char* message,
                                 ...) {
  printf("Assertion Failed: %s:%d\nReason: ", _RL_FILE_LAST_COMPONENT(file),
         line);
  va_list args;
  va_start(args, message);
  vprintf(message, args);
  va_end(args);
  printf("\n");
}

/**
 *  Assert that a condition is satisfied along with a given message.
 *
 *  @param condition the condition to check the satisfaction of.
 *  @param message   the message to log when the assertion fails.
 *  @param args      optional arguments to the message when the assertion fails.
 */
#define RL_ASSERT_MSG(condition, message, ...)                   \
  do {                                                           \
    if (!(condition)) {                                          \
      _RL_AssertLog(__FILE__, __LINE__, message, ##__VA_ARGS__); \
      abort();                                                   \
    }                                                            \
  } while (0)

/**
 *  Assert that a given condition is satisfied. This variant takes no message.
 *  Use `RL_ASSERT_MSG` to add a message when the assertion fails.
 *
 *  @param condition the condition to check the assertion of.
 */
#define RL_ASSERT(condition) \
  RL_ASSERT_MSG((condition), "Condition Failed: (" #condition ")")

/*
 *  Mark an expression as unused.
 *
 *  @param expr the expression to be marked as unused.
 */
#define RL_UNUSED(expr) \
  do {                  \
    (void)(expr);       \
  } while (0)

/*
 *  Denotes that this code path is a work-in-progress.
 */
#define RL_WIP RL_ASSERT_MSG(false, "WIP")

/*
 *  Denotes that this code path is unexpected.
 */
#define RL_UNEXPECTED RL_LOG("Did not expect %s:%d", __FILE__, __LINE__)

/*
 *  Error Checking.
 */

/**
 *  Check that the given Unix call successfully completed. If a failure is
 *  detected, the ERRNO is logged and an assertion tripped.
 *
 *  @param call     the Unix call to check the result of.
 *  @param expected the expected value of the unix call.
 */
#define RL_CHECK_EXPECT(call, expected)           \
  {                                               \
    if ((call) != (expected)) {                   \
      RL_LOG_ERRNO();                             \
      RL_ASSERT_MSG(false, "Error in UNIX call"); \
    }                                             \
  }

/**
 *  Check that the given Unix call is successfully completed. In case of
 *  falure, the ERRNO is logged and an assertion tripped.
 *
 *  @param call the Unix call to check the result of.
 */
#define RL_CHECK(call) RL_CHECK_EXPECT(call, 0)

/**
 *  Retries operation on `EINTR`. Just like `TEMP_FAILURE_RETRY` but available
 *  with non-GNU headers.
 */
#define RL_TEMP_FAILURE_RETRY(exp)         \
  ({                                       \
    __typeof__(exp) _rc;                   \
    do {                                   \
      _rc = (exp);                         \
    } while (_rc == -1 && errno == EINTR); \
    _rc;                                   \
  })

#define RL_TEMP_FAILURE_RETRY_AND_CHECK(exp) \
  ({                                         \
    __typeof__(exp) _rc;                     \
    do {                                     \
      _rc = (exp);                           \
    } while (_rc == -1 && errno == EINTR);   \
    RL_CHECK_EXPECT(_rc, 0);                 \
    _rc;                                     \
  })

/**
 *  Performs the Mach operation and aborts on failure before logging the error.
 *
 *  @param exp the mach operation returning `kern_return_t`.
 *
 *  @return the return value of the Mach operation.
 */
#define RL_MACH_CHECK(exp)                                \
  ({                                                      \
    __typeof__(exp) _rc = (exp);                          \
    if (_rc != KERN_SUCCESS) {                            \
      RL_LOG("Mach Error: '%s'", mach_error_string(_rc)); \
      RL_ASSERT_MSG(false, "Fatal Error on Mach Call");   \
    }                                                     \
    _rc;                                                  \
  })

/*
 *  C++ Compiler Macros
 */

/**
 *  Disable the copy constructor on the given type. Must appear (anywhere) in
 *  the class definition.
 *
 *  @param TypeName the type to disable the copy constructor on.
 */
#define RL_DISALLOW_COPY(TypeName) TypeName(const TypeName&) = delete;

/**
 *  Disable the assignment operator on the given type. Must appear (anywhere) in
 *  the class definition.
 *
 *  @param TypeName the type to disable assignement operator on.
 */
#define RL_DISALLOW_ASSIGN(TypeName) void operator=(const TypeName&) = delete;

/**
 *  Disable the copy constructor and the assignement operator on the given type.
 *  Must appear (anywhere) in the class definition.
 *
 *  @param TypeName the type to disable the copy constructor and assignement
 *                  operator on.
 */
#define RL_DISALLOW_COPY_AND_ASSIGN(TypeName) \
  RL_DISALLOW_COPY(TypeName)                  \
  RL_DISALLOW_ASSIGN(TypeName)

/*
 *  Extra Warnings.
 */
#if defined(__GNUC__)
#define RL_WARN_UNUSED_RESULT __attribute__((warn_unused_result))
#else
#define RL_WARN_UNUSED_RESULT
#endif

/*
 *  Debugging Macros.
 */
#define DEBUG_BREAK() __builtin_trap()

/*
 *  Macros for repetitious error checking.
 */
#define RL_RETURN_IF_FALSE(exp) \
  if (!(exp)) {                 \
    DEBUG_BREAK();              \
    return false;               \
  }
