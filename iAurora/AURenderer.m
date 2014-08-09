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

-(UIImage *)render{
    NSURL *fileurl = [[NSBundle mainBundle] URLForResource:@"tmp" withExtension:@"asc"];
    NSString *filename = [fileurl path];
    NSLog(@"filename: %@", filename);
    
    NSLog(@"Reading file");
    
    NSString *contents = [NSString stringWithContentsOfFile:filename encoding:NSUTF8StringEncoding error:nil];
    
    NSLog(@"Contents: %@", contents);
    
    Aurora::Renderer rnd = Aurora::Renderer([filename UTF8String]);
    rnd.render();
    
    int width = 512; int height = 512;
    
    CGDataProviderRef provider = CGDataProviderCreateWithData(NULL,
                                                              rnd.pixels,
                                                              width*height*4,
                                                              NULL);
    int bitsPerComponent = 8;
    int bitsPerPixel = 4*bitsPerComponent;
    int bytesPerRow = bitsPerPixel * width / 8;
    
    CGColorSpaceRef colorSpaceRef = CGColorSpaceCreateDeviceRGB();
    CGBitmapInfo bitmapInfo = kCGImageAlphaLast;
    CGColorRenderingIntent renderingIntent = kCGRenderingIntentDefault;

    CGImageRef imageRef = CGImageCreate(width,
                                        height,
                                        bitsPerComponent,
                                        bitsPerPixel,
                                        bytesPerRow,
                                        colorSpaceRef,
                                        bitmapInfo,
                                        provider,
                                        NULL,
                                        NO,
                                        renderingIntent);
    
    UIImage *image = [UIImage imageWithCGImage:imageRef];
    
    NSLog(@"Created image with size: %f %f", image.size.width, image.size.height);
    
    return image;
}

@end
