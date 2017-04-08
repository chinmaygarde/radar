/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#import <AppKit/AppKit.h>
#import "RadarSurface.h"

@interface RadarWindow : NSWindow

@property(weak) IBOutlet RadarSurface* surface;

@end
