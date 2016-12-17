// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#import "RadarArchiveDocument.h"
#include <Core/Allocation.h>
#include <InterfaceBuilder/InterfaceBuilderInterface.h>
#include <Core/EventLoopThread.h>

@implementation RadarArchiveDocument {
  rl::core::EventLoopThread _interfaceThread;
}

- (NSString*)windowNibName {
  return nil;
}

- (void)windowControllerDidLoadNib:(NSWindowController*)aController {
  [super windowControllerDidLoadNib:aController];
}

- (NSData*)dataOfType:(NSString*)typeName error:(NSError**)outError {
  if (outError) {
    *outError =
        [NSError errorWithDomain:NSCocoaErrorDomain code:unimpErr userInfo:nil];
  }
  return nil;
}

- (BOOL)readFromData:(NSData*)data
              ofType:(NSString*)typeName
               error:(NSError**)outError {
  if (outError) {
    *outError = [NSError errorWithDomain:NSOSStatusErrorDomain
                                    code:unimpErr
                                userInfo:nil];
  }

  auto archive = rl::ib::InterfaceBuilderArchive::Make(
      static_cast<const uint8_t*>(data.bytes), data.length);

  if (archive == nullptr) {
    return NO;
  }

  auto interfaceDelegate =
      std::make_shared<rl::ib::InterfaceBuilderInterface>(std::move(archive));

  if (interfaceDelegate == nullptr) {
    return NO;
  }

  if (outError) {
    *outError = nil;
  }

  _interfaceThread.loop().get()->dispatchAsync([interfaceDelegate]() {
    rl::interface::Interface interface(interfaceDelegate);
    interface.run();
  });
  return YES;
}

+ (BOOL)autosavesInPlace {
  return NO;
}

@end