/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#import <Cocoa/Cocoa.h>
#include <Interface/InterfaceDelegate.h>
#include <memory>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
@interface RadarSurface : NSOpenGLView
#pragma clang diagnostic pop

- (void)launchInterfaceDelegate:
    (std::shared_ptr<rl::interface::InterfaceDelegate>)delegate;

@end
