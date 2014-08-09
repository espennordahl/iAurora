//
//  AUViewController.m
//  iAurora
//
//  Created by Espen Nordahl on 27/07/14.
//  Copyright (c) 2014 Espen Nordahl. All rights reserved.
//

#import "AUViewController.h"

#import "AURenderer.h"

@interface AUViewController ()

@end

@implementation AUViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
	// Do any additional setup after loading the view, typically from a nib.
    
    NSLog(@"Allocating renderer.");
    
    AURenderer *renderer = [[AURenderer alloc] init];
    [renderer render];
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

@end
