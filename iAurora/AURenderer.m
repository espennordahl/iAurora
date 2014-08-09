//
//  AURenderer.m
//  iAurora
//
//  Created by Espen Nordahl on 9/08/14.
//  Copyright (c) 2014 Espen Nordahl. All rights reserved.
//

#import "AURenderer.h"

#include "renderer.h"

@implementation AURenderer

-(void)render{
    Aurora::Renderer rnd = Aurora::Renderer("abc");
    rnd.render();
}

@end
