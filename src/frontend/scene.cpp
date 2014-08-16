//
//  scene.cpp
//  iAurora
//
//  Created by Espen Nordahl on 16/08/14.
//  Copyright (c) 2014 Espen Nordahl. All rights reserved.
//

#include "scene.h"

#include "shadingEngine.h"
#include "shapes.h"
#include "auroraObject.h"
#include "materials.h"
#include "lights.h"
#include "shadingEngine.h"
#include "kdTreeAccelerator.h"
#include "jsonParser.h"
#include "camera.h"


#include "log.h"
#define lcontext LOG_Scene

using namespace Aurora;

Scene::Scene(const std::string &file){

    time_t parseBegin;
	time(&parseBegin);
    
    parseSceneDescription(file);

        // Time
	time_t parseEnd;
	time(&parseEnd);
	int totalTime = difftime(parseEnd, parseBegin);
    
	LOG_INFO("Total parsing time: "
             << floor(totalTime/60/60) << " h "
             << floor((totalTime/60) % 60) << " min "
             << totalTime % 60 << " sec.");
    
    time_t envBegin;
	time(&envBegin);
    
	buildRenderEnvironment();
    	// Time
	time_t envEnd;
	time(&envEnd);
    totalTime = difftime(envEnd, envBegin);
    
	LOG_INFO("Total time building render env: "
             << floor(totalTime/60/60) << " h "
             << floor((totalTime/60) % 60) << " min "
             << totalTime % 60 << " sec.");
}

u_char *Scene::pixels(){
    return displayDriver->pixels();
}

void Scene::parseSceneDescription(const std::string &filename){
    
    LOG_INFO("*************************************");
	LOG_INFO("Parsing scene description.");
    
    renderEnv = RenderEnvironment();
    renderEnv.shadingEngine = new ShadingEngine();
    renderEnv.globals =  new AuroraGlobals;
    renderEnv.stringGlobals = new StringMap;
    setDefaultOptions(renderEnv.globals);
    
        // open file
    JsonParser parser = JsonParser(filename, &renderEnv);
    parser.parseScene(NULL);
    objects = parser.getObjects();
    lights = parser.getLights();
    
	LOG_INFO("Done parsing scene description.");
    LOG_INFO("*************************************\n");
    
    
}

void Scene::buildRenderEnvironment(){
    LOG_INFO("*************************************");
	LOG_INFO("Building render environment.");
    
        // preFrame
    for (u_int32_t i=0; i < objects.size(); i++) {
        objects[i]->frameBegin();
    }
    
        // camera
    int width = (*renderEnv.globals)[ResolutionX];
	int height = (*renderEnv.globals)[ResolutionY];

    renderCam = new Camera((*renderEnv.globals)[FieldOfView], width, height, (*renderEnv.globals)[PixelSamples]);

    displayDriver = new Display(width, height, &renderEnv);
    
	if (!objects.size()) {
		LOG_ERROR("No objects found.");
	}
	
        // acceleration structure
    AccelerationStructure *accel;
    
    int numObjects = (int)objects.size();
    int numLights = (int)lights.size();
    attrs = new AttributeState[numObjects + numLights];
    std::vector<RenderableTriangle> renderable;
    for (int i=0; i < numObjects; i++) {
        objects[i]->makeRenderable(renderable, attrs, i);
    }
    for (int i=0; i < numLights; i++) {
        lights[i]->makeRenderable(renderable, attrs, numObjects + i);
    }
    
    accel = new KdTreeAccelerator(renderable, KD_INTERSECTCOST, KD_TRAVERSECOST, KD_EMPTYBONUS, (*renderEnv.globals)[KD_MaxLeaf], (*renderEnv.globals)[KD_MaxDepth]);
    
	renderEnv.accelerationStructure = accel;
	renderEnv.attributeState = attrs;
	renderEnv.lights = lights;
	renderEnv.envLight = envLight;
	renderEnv.renderCam = renderCam;

	LOG_INFO("Done building render environment.");
    LOG_INFO("*************************************\n");
}
