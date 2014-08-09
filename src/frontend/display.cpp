//
//  openexrDisplay.cpp
//  Aurora
//
//  Created by Espen Nordahl on 05/07/2012.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <iostream>

//#include <ImfRgbaFile.h>
//#include <ImfStringAttribute.h>
//#include <ImfMatrixAttribute.h>
//#include <ImfArray.h>
//#include "drawImage.h"

#include "display.h"

#include "log.h"
#define lcontext LOG_ExrDisplay

using namespace Aurora;

Display::Display(int width, int height, std::string file, RenderEnvironment * renderEnv) :
FrontEndObject("Display:" + file, renderEnv),
m_width(width),
m_height(height)
{
    m_pixels = new u_char[m_width * m_height * 4];
    m_rgb.resize(m_height);
    m_a.resize(m_height);
    m_multisample_buffer.resize(m_height);
    for (int i=0; i<m_height; ++i) {
        m_rgb[i].resize(m_width);
        m_a[i].resize(m_width);
        m_multisample_buffer[i].resize(m_width);
    }
}

#pragma mark -
#pragma mark pipeline
void Display::frameBegin(){
    
}

void Display::frameEnd(){
    
}

int Display::width() const{
    return m_width;
}

int Display::height() const {
    return m_height;
}

void Display::setPixel(int x, int y, const Color &col, float alpha){
    m_rgb[y][x] = col;
    m_a[y][x] = alpha;
}

void Display::appendValue(int x, int y, const Color &col, float alpha){
    m_multisample_buffer[y][x] += 1;
    float sampleNum = m_multisample_buffer[y][x];
    float weight = 1.f/sampleNum;
    m_rgb[y][x] = m_rgb[y][x] * (1.f-weight) + col * weight;
    m_a[y][x] = m_a[y][x] * (1.f-weight) + alpha * weight;

}

u_char *Display::pixels(){
    
    u_char *channel = m_pixels;
    for (int i=0; i<m_height; i++) {
		for (int j=0; j<m_width; j++) {
			Color &p = m_rgb[i][j];
            *channel = 50;//255 * pow(std::min((float)p.r, 1.f), 0.45454545);
            ++channel;
            *channel = 200;//255 * pow(std::min((float)p.g, 1.f), 0.45454545);
            ++channel;
            *channel = 50;//255 * pow(std::min((float)p.b, 1.f), 0.45454545);
            ++channel;
            *channel = 255;//255 * std::min((float)m_a[i][j], 1.f);
            ++channel;
		}
	}
    
    return m_pixels;
}
