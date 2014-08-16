//
//  AUViewController.h
//  iAurora
//
//  Created by Espen Nordahl on 27/07/14.
//  Copyright (c) 2014 Espen Nordahl. All rights reserved.
//

#import <UIKit/UIKit.h>

#include "renderer.h"

@interface AUViewController : UIViewController

@property (assign) IBOutlet UIButton *renderButton;

-(IBAction) didClick:(id) sender;

@end

Aurora::Renderer m_rnd;
std::shared_ptr<Aurora::Scene> m_scn;
