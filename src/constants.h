//
//  constants.h
//  Aurora
//
//  Created by Espen Nordahl on 07/07/2012.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef Aurora_constants_h
#define Aurora_constants_h

// bias
#define CAUSTIC_BLUR 0.3
#define FIREFLY 200

// colors
#define ISBLACK  0.0001

// exr
#define DRAW_DELAY 1

// ray tracing
#define RAY_BIAS 0.0005

    // kd tree
#define KD_INTERSECTCOST 1
#define KD_TRAVERSECOST 1
#define KD_EMPTYBONUS 1

// kelemen albedo cache settings
#define SPECALBEDO_SAMPLES 4

    // quasi monte carlo
    // #define USE_HALTON

#define CAMERA_DOMAIN_X 0
#define CAMERA_DOMAIN_Y 1

#define SURFACE_DOMAIN_X 2
#define SURFACE_DOMAIN_Y 3

#define LIGHT_DOMAIN_X 4
#define LIGHT_DOMAIN_Y 5

#define VERSION "0.3"

// base paths
// #define CONFIGPATH "/Users/espennordahl/Documents/Aurora/XCode/Aurora/Aurora/config.json"

#endif