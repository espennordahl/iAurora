//
//  renderer.cpp
//  Aurora
//
//  Created by Espen Nordahl on 30/06/2012.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "renderer.h"
#include "shapes.h"
#include "auroraObject.h"
#include "materials.h"
#include "lights.h"
#include "shadingEngine.h"
#include "kdTreeAccelerator.h"

#include "log.h"
#define lcontext LOG_Renderer

#include <iostream>
#include <math.h>

#include "jsonParser.h"

using namespace Aurora;

Renderer::Renderer(std::string file):
m_displayDriver(NULL)
{
    m_filename = file;
}


void Renderer::render(){

    parseSceneDescription();
    
    time_t envBegin;
	time(&envBegin);
    
	buildRenderEnvironment();
    	// Time
	time_t envEnd;
	time(&envEnd);
	int totalTime = difftime(envEnd, envBegin);
    
	LOG_INFO("Total time building render env: "
             << floor(totalTime/60/60) << " h "
             << floor((totalTime/60) % 60) << " min "
             << totalTime % 60 << " sec.");

    
	renderImage();
	
	outputStats();
    
    postRenderCleanup();
}

void Renderer::parseSceneDescription(){
    time_t parseBegin;
	time(&parseBegin);
    
    LOG_INFO("*************************************");
	LOG_INFO("Parsing scene description.");
    
    m_renderEnv = RenderEnvironment();
    m_renderEnv.shadingEngine = new ShadingEngine();
    m_renderEnv.globals =  new AuroraGlobals;
    m_renderEnv.stringGlobals = new StringMap;
    setDefaultOptions(m_renderEnv.globals);

        // open file
    JsonParser parser = JsonParser(m_filename, &m_renderEnv);
    parser.parseScene(NULL);
    m_objects = parser.getObjects();
    m_lights = parser.getLights();
        
	LOG_INFO("Done parsing scene description.");	
    LOG_INFO("*************************************\n");
    
        // Time
	time_t parseEnd;
	time(&parseEnd);
	int totalTime = difftime(parseEnd, parseBegin);
    
	LOG_INFO("Total parsing time: "
             << floor(totalTime/60/60) << " h "
             << floor((totalTime/60) % 60) << " min "
             << totalTime % 60 << " sec.");

}

void Renderer::buildRenderEnvironment(){
    LOG_INFO("*************************************");
	LOG_INFO("Building render environment.");
    
        // preFrame
    for (u_int32_t i=0; i < m_objects.size(); i++) {
        m_objects[i]->frameBegin();
    }
    
        // camera
    m_renderCam = new Camera((*m_renderEnv.globals)[FieldOfView], (*m_renderEnv.globals)[ResolutionX], (*m_renderEnv.globals)[ResolutionY], (*m_renderEnv.globals)[PixelSamples]);
    
	if (!m_objects.size()) {
		LOG_ERROR("No objects found.");
	}
	
        // acceleration structure
    AccelerationStructure *accel;
    
    int numObjects = (int)m_objects.size();
    int numLights = (int)m_lights.size();
    m_attrs = new AttributeState[numObjects + numLights];
    std::vector<RenderableTriangle> renderable;
    for (int i=0; i < numObjects; i++) {
        m_objects[i]->makeRenderable(renderable, m_attrs, i);
    }
    for (int i=0; i < numLights; i++) {
        m_lights[i]->makeRenderable(renderable, m_attrs, numObjects + i);
    }
    
    accel = new KdTreeAccelerator(renderable, KD_INTERSECTCOST, KD_TRAVERSECOST, KD_EMPTYBONUS, (*m_renderEnv.globals)[KD_MaxLeaf], (*m_renderEnv.globals)[KD_MaxDepth]);

	m_renderEnv.accelerationStructure = accel;
	m_renderEnv.attributeState = m_attrs;
	m_renderEnv.lights = m_lights;
	m_renderEnv.envLight = m_envLight;
	m_renderEnv.renderCam = m_renderCam;
    	
	LOG_INFO("Done building render environment.");
    LOG_INFO("*************************************\n");
}

void Renderer::renderImage(){
    LOG_INFO("*************************************");
	LOG_INFO("Rendering image.");
	
        // start render timer
	time(&m_renderTime);
    
        // init stats
    m_rayspeed = 0;
    m_numrays = 0;
	
        // set up buffer and sampler
	int width = m_renderCam->getWidthSamples();
	int height = m_renderCam->getHeightSamples();
	int multisamples = m_renderCam->getPixelSamples();

    std::string fn = (*m_renderEnv.stringGlobals)["fileName"];
    if(fn == ""){
        LOG_ERROR("Render output filename is blank");
    }

    m_displayDriver = new Display(width, height, fn, &m_renderEnv);

    m_integrator = new Integrator::Integrator(&m_renderEnv);
    
    time_t drawTime;
    time(&drawTime);
    int lightSamples = 1;
    for(int i=0; i<multisamples; ++i){
        time_t currentTime;
        time_t progressionStartTime;
        time(&progressionStartTime);

            // generate sample
        int x = i % width;
        int y = (int)i / width;
        Sample2D current_sample_2d = Sample2D(x, y);
        
        Sample3D sample = m_renderEnv.renderCam->convertSample(current_sample_2d, i);
        
        Integrator::IntegrationResult integrationResult = m_integrator->integrateCameraSample(sample, lightSamples);
        
        m_displayDriver->appendValue(x, y, integrationResult.color, integrationResult.alpha);
        m_numrays++;
        
        time(&currentTime);
        int timeSinceLastDraw = difftime(currentTime, drawTime);
        if((!i || timeSinceLastDraw > DRAW_DELAY)) {
//            m_displayDriver->draw(height);
            time(&drawTime);
            lightSamples = (*m_renderEnv.globals)[LightSamples];
        }
        
        LOG_INFO("Render progress: " << 100 * (i+1)/(float)multisamples << "%");
        time(&currentTime);
        int progtime = difftime(currentTime, progressionStartTime);
        int remaining = progtime * (multisamples - (i+1));
        LOG_INFO("Estimated time remaining: " << floor(remaining/60/60) << "h" << floor(remaining/60)%60 << "min" << remaining%60 << "sec.");
        double speed = (m_numrays * 0.001) / std::max(1, progtime);
        LOG_INFO("Current speed: " << speed << "K rays/sec.");
        m_rayspeed += speed / multisamples;
        m_numrays = 0;
    }
}


void Renderer::outputStats(){
    LOG_INFO("*************************************");
	LOG_INFO("Outputting statistics.");
	
    LOG_INFO("Renderer version: Aurora v" << VERSION);
    
	// Time
	time_t renderEnd;
	time(&renderEnd);
	int totalTime = difftime(renderEnd, m_renderTime);
    
	LOG_INFO("Total render time: " 
             << floor(totalTime/60/60) << " h " 
             << floor((totalTime/60) % 60) << " min " 
             << totalTime % 60 << " sec.");
    std::string stringTime =    intToString(floor(totalTime/60/60)) + " h " +
                                intToString(floor((totalTime/60) % 60)) + " min "+
                                intToString(totalTime % 60) + " sec.";
//    m_displayDriver->addMetadata(std::string("renderTime"), stringTime);
    
    // Rays
    LOG_INFO("Average speed: " << m_rayspeed << "K rays/sec.");
    std::string stringSpeed = intToString(m_rayspeed) + "K rays/sec.";
//    m_displayDriver->addMetadata(std::string("RaySpeed"), stringSpeed);
    
	LOG_INFO("Done outputting statistics.");
    LOG_INFO("*************************************\n");
}

void Renderer::postRenderCleanup(){
//    m_displayDriver->draw(m_displayDriver->height());

    pixels = m_displayDriver->pixels();

    delete m_renderEnv.accelerationStructure;
    delete m_renderEnv.attributeState;
    delete m_renderEnv.renderCam;
}
