/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <Core/Trace.h>
#include <Shell/DefaultInterface.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include "RadarAppDelegate.h"
#include "RadarWindow.h"

@interface RadarAppDelegate ()

@property(nonatomic, retain) NSWindowController* windowController;

@end

@implementation RadarAppDelegate

- (void)dealloc {
  [_windowController release];
  [super dealloc];
}

- (void)applicationDidFinishLaunching:(NSNotification*)notification {
  self.windowController = [[[NSWindowController alloc]
      initWithWindowNibName:@"RadarWindow"] autorelease];

  if ([self.windowController.window isKindOfClass:[RadarWindow class]]) {
    RadarWindow* window =
        reinterpret_cast<RadarWindow*>(self.windowController.window);
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
