// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#import <Cocoa/Cocoa.h>
#include <Interface/InterfaceDelegate.h>
#include <memory>

@interface RadarSurface : NSOpenGLView

- (void)launchInterfaceDelegate:
    (std::shared_ptr<rl::interface::InterfaceDelegate>)delegate;

@end
