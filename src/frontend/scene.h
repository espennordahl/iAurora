//
//  scene.h
//  iAurora
//
//  Created by Espen Nordahl on 16/08/14.
//  Copyright (c) 2014 Espen Nordahl. All rights reserved.
//

#ifndef __iAurora__scene__
#define __iAurora__scene__

#include <iostream>

#include "core.h"
#include "auroraObject.h"
#include "display.h"

#include <string.h>

namespace Aurora {
	class Scene {
	public:
        Scene(const std::string &file);
        
        u_char *pixels();
        
        Transform *cameraTransform;
        Camera *renderCam;
		std::vector<std::shared_ptr<AuroraObject> > objects;
		std::vector<Light* > lights;
		InfiniteAreaLight *envLight;
		RenderEnvironment renderEnv;
		AttributeState *attrs;
		Display *displayDriver;

    private:
        void parseSceneDescription(const std::string &filename);
        void buildRenderEnvironment();
    };
}


#endif /* defined(__iAurora__scene__) */
