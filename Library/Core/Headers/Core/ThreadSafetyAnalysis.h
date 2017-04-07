// Copyright 2017 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADAR_CORE_THREADSAFETYANALYSIS_H_
#define RADAR_CORE_THREADSAFETYANALYSIS_H_

/*
 *  Thread safety analysis macros from:
 *  https://clang.llvm.org/docs/ThreadSafetyAnalysis.html#mutex-h
 */

#if defined(__clang__) && (!defined(SWIG))
#define RL_THREAD_ANNOTATION_ATTRIBUTE__(x)   __attribute__((x))
#else
#define RL_THREAD_ANNOTATION_ATTRIBUTE__(x)   // no-op
#endif

#define RL_THREAD_ANNOTATION_ATTRIBUTE__(x)   __attribute__((x))

#define RL_CAPABILITY(x) \
  RL_THREAD_ANNOTATION_ATTRIBUTE__(capability(x))

#define RL_SCOPED_CAPABILITY \
  RL_THREAD_ANNOTATION_ATTRIBUTE__(scoped_lockable)

#define RL_GUARDED_BY(x) \
  RL_THREAD_ANNOTATION_ATTRIBUTE__(guarded_by(x))

#define RL_PT_GUARDED_BY(x) \
  RL_THREAD_ANNOTATION_ATTRIBUTE__(pt_guarded_by(x))

#define RL_ACQUIRED_BEFORE(...) \
  RL_THREAD_ANNOTATION_ATTRIBUTE__(acquired_before(__VA_ARGS__))

#define RL_ACQUIRED_AFTER(...) \
  RL_THREAD_ANNOTATION_ATTRIBUTE__(acquired_after(__VA_ARGS__))

#define RL_REQUIRES(...) \
  RL_THREAD_ANNOTATION_ATTRIBUTE__(requires_capability(__VA_ARGS__))

#define RL_REQUIRES_SHARED(...) \
  RL_THREAD_ANNOTATION_ATTRIBUTE__(requires_shared_capability(__VA_ARGS__))

#define RL_ACQUIRE(...) \
  RL_THREAD_ANNOTATION_ATTRIBUTE__(acquire_capability(__VA_ARGS__))

#define RL_ACQUIRE_SHARED(...) \
  RL_THREAD_ANNOTATION_ATTRIBUTE__(acquire_shared_capability(__VA_ARGS__))

#define RL_RELEASE(...) \
  RL_THREAD_ANNOTATION_ATTRIBUTE__(release_capability(__VA_ARGS__))

#define RL_RELEASE_SHARED(...) \
  RL_THREAD_ANNOTATION_ATTRIBUTE__(release_shared_capability(__VA_ARGS__))

#define RL_TRY_ACQUIRE(...) \
  RL_THREAD_ANNOTATION_ATTRIBUTE__(try_acquire_capability(__VA_ARGS__))

#define RL_TRY_ACQUIRE_SHARED(...) \
  RL_THREAD_ANNOTATION_ATTRIBUTE__(try_acquire_shared_capability(__VA_ARGS__))

#define RL_EXCLUDES(...) \
  RL_THREAD_ANNOTATION_ATTRIBUTE__(locks_excluded(__VA_ARGS__))

#define RL_ASSERT_CAPABILITY(x) \
  RL_THREAD_ANNOTATION_ATTRIBUTE__(assert_capability(x))

#define RL_ASSERT_SHARED_CAPABILITY(x) \
  RL_THREAD_ANNOTATION_ATTRIBUTE__(assert_shared_capability(x))

#define RL_RETURN_CAPABILITY(x) \
  RL_THREAD_ANNOTATION_ATTRIBUTE__(lock_returned(x))

#define RL_NO_THREAD_SAFETY_ANALYSIS \
  RL_THREAD_ANNOTATION_ATTRIBUTE__(no_thread_safety_analysis)


#endif // RADAR_CORE_THREADSAFETYANALYSIS_H_
