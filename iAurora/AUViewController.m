//
//  AUViewController.m
//  iAurora
//
//  Created by Espen Nordahl on 27/07/14.
//  Copyright (c) 2014 Espen Nordahl. All rights reserved.
//

#import "AUViewController.h"

#include "renderer.h"

@interface AUViewController ()

@end

@implementation AUViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
    
	// Do any additional setup after loading the view, typically from a nib.
    
    NSLog(@"Allocating renderer.");
    
    NSURL *fileurl = [[NSBundle mainBundle] URLForResource:@"tmp" withExtension:@"asc"];
    NSString *filename = [fileurl path];
    NSLog(@"filename: %@", filename);
    
    NSLog(@"Reading file");
    
    NSString *contents = [NSString stringWithContentsOfFile:filename encoding:NSUTF8StringEncoding error:nil];
    
    NSLog(@"Contents: %@", contents);
    
        // create scene
    
    m_scn = std::shared_ptr<Aurora::Scene>(new Aurora::Scene([filename UTF8String]));
    
    m_rnd = Aurora::Renderer();
    
        //[self didClick:NULL];
}

-(void)render
{
    m_rnd.render(m_scn);
    
    int width = m_scn->displayDriver->width();
    int height = m_scn->displayDriver->height();
    
    CGDataProviderRef provider = CGDataProviderCreateWithData(NULL,
                                                              m_scn->pixels(),
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
    
    [(UIImageView *)self.view setImage:image];
    
    NSLog(@"Updated view. You should see a render at this point..");
    
    [self performSelectorInBackground:@selector(render) withObject:self];
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

-(void)didClick:(id)sender
{
    [self performSelectorInBackground:@selector(render) withObject:self];
}


@end
