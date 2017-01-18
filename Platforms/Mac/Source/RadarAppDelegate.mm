// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#import "RadarAppDelegate.h"
#include <Core/Trace.h>
#include <Shell/DefaultInterface.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include "RadarWindow.h"

@interface RadarAppDelegate ()

@property(strong) NSWindowController* windowController;

@end

@implementation RadarAppDelegate

- (void)applicationDidFinishLaunching:(NSNotification*)notification {
  _windowController =
      [[NSWindowController alloc] initWithWindowNibName:@"RadarWindow"];

  if ([_windowController.window isKindOfClass:[RadarWindow class]]) {
    RadarWindow* window =
        reinterpret_cast<RadarWindow*>(_windowController.window);
    [window.surface
        launchInterfaceDelegate:rl::shell::CreateDefaultInterface()];
  }
}

- (IBAction)onCaptureTraceSelected:(id)sender {
  NSArray* paths = NSSearchPathForDirectoriesInDomains(NSDesktopDirectory,
                                                       NSUserDomainMask, YES);

  if (paths.count == 0) {
    RL_LOG("Could not find desktop path for writing trace");
    return;
  }

  NSString* desktopPath = paths.firstObject;

  NSURL* traceURL = [[NSURL URLWithString:desktopPath]
      URLByAppendingPathComponent:@"RadarTrace.json"
                      isDirectory:NO];

  std::ofstream traceFile;
  traceFile.open([traceURL absoluteString].UTF8String,
                 std::ios::out | std::ios::trunc);

  if (traceFile.is_open()) {
    std::stringstream stream;
    rl::instrumentation::ProcessTrace::Current().recordToStream(stream);
    traceFile << stream.str();
    traceFile.flush();
    traceFile.close();

    RL_LOG("Trace file written to '%s'...", traceURL.absoluteString.UTF8String);
  }
}

@end
