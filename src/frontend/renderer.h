//
//  renderer.h
//  Aurora
//
//  Created by Espen Nordahl on 30/06/2012.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef Aurora_renderer_h
#define Aurora_renderer_h

#include "scene.h"
#include "camera.h"
#include "renderEnvironment.h"
#include "auroraObject.h"
#include "display.h"
#include "lights.h"
#include "options.h"
#include "integrator.h"

#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include <map>

namespace Aurora {
    class Session;
}

namespace Aurora{

	class Renderer {
	public:
		void render(std::shared_ptr<Scene> scene);
        
        u_char *pixels;
        
	private:
        void parseSceneDescription();
		void buildRenderEnvironment();
		void renderImage();
		void outputStats();
		void postRenderCleanup();
        
        std::shared_ptr<Scene> m_scene;
		
        time_t m_renderTime;
		float m_renderProgress;
        std::string m_filename;

        long m_numrays;
        double m_rayspeed;
    };
}


#endif
