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
    NSURL *fileurl = [[NSBundle mainBundle] URLForResource:@"tmp" withExtension:@"asc"];
    NSString *filename = [fileurl path];
    NSLog(@"filename: %@", filename);
    
    NSLog(@"Reading file");
    
    NSString *contents = [NSString stringWithContentsOfFile:filename encoding:NSUTF8StringEncoding error:nil];
    
    NSLog(@"Contents: %@", contents);
    
    Aurora::Renderer rnd = Aurora::Renderer([filename UTF8String]);
    rnd.render();
}

@end
