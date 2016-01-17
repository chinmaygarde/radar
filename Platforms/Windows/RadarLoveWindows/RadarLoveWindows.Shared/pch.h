// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifdef _WIN32

/*
 *  Including SDKDDKVer.h defines the highest available Windows platform.
 *  If you wish to build your application for a previous Windows platform,
 *  include WinSDKVer.h and set the _WIN32_WINNT macro to the platform you
 *  wish to support before including SDKDDKVer.h.
 */

#include <SDKDDKVer.h>

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#else

#error Non Windows platform file included this header

#endif
