//
//  renderer.cpp
//  Aurora
//
//  Created by Espen Nordahl on 30/06/2012.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "renderer.h"

#include "log.h"
#define lcontext LOG_Renderer

#include <iostream>
#include <math.h>

using namespace Aurora;

void Renderer::render(std::shared_ptr<Scene> scene){
    
    m_scene = scene;
    
	renderImage();
	
	outputStats();
    
    postRenderCleanup();
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
	int width = m_scene->renderCam->getWidthSamples();
	int height = m_scene->renderCam->getHeightSamples();

    std::string fn = (*m_scene->renderEnv.stringGlobals)["fileName"];
    if(fn == ""){
        LOG_ERROR("Render output filename is blank");
    }

    Integrator integrator = Integrator(&m_scene->renderEnv);
    
    time_t drawTime;
    time(&drawTime);
    int lightSamples = (*m_scene->renderEnv.globals)[LightSamples];
    time_t currentTime;
    time_t progressionStartTime;
    time(&progressionStartTime);

    for (int i=0; i < width*height; ++i) {
            // generate sample
        int x = i % width;
        int y = (int)i / width;
        Sample2D current_sample_2d = Sample2D(x, y);
        
        Sample3D sample = m_scene->renderEnv.renderCam->convertSample(current_sample_2d, i);
        
        Integrator::IntegrationResult integrationResult = integrator.integrateCameraSample(sample, lightSamples);
        
        m_scene->displayDriver->appendValue(x, y, integrationResult.color, integrationResult.alpha);
        m_numrays++;
        
        time(&currentTime);
        int timeSinceLastDraw = difftime(currentTime, drawTime);
        
        if(timeSinceLastDraw > DRAW_DELAY) {
                //            m_displayDriver->draw(height);
            time(&drawTime);
            LOG_INFO("Render progress: " << 100 * (i+1.f)/((float)width*height) << "%");
            time(&currentTime);
        }
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
}
