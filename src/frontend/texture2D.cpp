//
//  texture2D.cpp
//  Aurora
//
//  Created by Espen Nordahl on 13/08/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <iostream>

#include "texture2D.h"
#include "log.h"
#define lcontext LOG_Texture2D


#include "math_utils.h"

#include <assert.h>

using namespace Aurora;

Texture2D::Texture2D(std::string filename):
m_texname(filename)
{
}

Color Texture2D::read(float s, float t, float filterSize){
    assert(false);
    return Color();
}

float Texture2D::pdf(float s, float t, float filterSize){
//	int x = floor(s * m_width);
//	int y = floor(t * m_height);
//	if (y == m_height) {
//		y -= 1;
//	}
//	if (x == m_width) {
//		x -= 1;
//	}
//        //	assert(x < width && x >= 0);
//        //	assert(y < height && y >= 0);
//	if(x > m_width || x < 0){
//        LOG_WARNING("Texture call outside of 0-1 range");
//        x = min(x,m_width);
//        x = max(x,0);
//    }
//	if(y > m_height || y < 0){
//        LOG_WARNING("Texture call outside of 0-1 range");
//        y = min(y,m_height);
//        y = max(y,0);
//    }
//    Imf::Rgba &p = m_pixel_buffer[y][x];
//	
//    return p.a/m_pdf_normalization;
    assert(false);
    return 1;
}


void Texture2D::initPdf(){
//	float lum = 0;
//    m_pdf_buffer.clear();
//	// we use the alpha for storing the pdf 
//    // in addition to the buffer
//	for (int i=0; i<m_height; i++) {
//		for (int j=0; j<m_width; j++) {
//			Imf::Rgba &p = m_pixel_buffer[i][j];
//            PdfSample sample;
//            sample.pdf = (p.r + p.g + p.b)/3.f;
//            sample.x = j;
//            sample.y = i;
//            m_pdf_buffer.push_back(sample);
//            p.a = sample.pdf;
//			lum += sample.pdf;
//		}
//	}
//	
//    
//    m_pdf_normalization = lum/(m_width * m_height);
//
//        // normalize and distribute values along 0-1 for sampling
//    float pdfAccum = 0;
//    for (int i=0; i < m_pdf_buffer.size(); i++) {
//        m_pdf_buffer[i].pdf += pdfAccum;
//        pdfAccum = m_pdf_buffer[i].pdf;
//        assert(pdfAccum <= lum);
//    }    
}

void Texture2D::sample(float r, float *x, float *y){
//	float offsetX = (float) rand()/RAND_MAX;
//	float offsetY = (float) rand()/RAND_MAX;
//	assert(r >= 0. && r <= 1.);
//    
//    r *= m_pdf_normalization * m_width * m_height;
//        // clunky binary search. TODO: make more elegant
//    if (r > m_pdf_buffer.back().pdf) {
//        *x = (m_pdf_buffer.back().x + offsetX)/m_width;
//        *y = (m_pdf_buffer.back().y + offsetY)/m_height;
//        return;
//    }
//    if (r < m_pdf_buffer.front().pdf) {
//        *x = (m_pdf_buffer.front().x + offsetX)/m_width;
//        *y = (m_pdf_buffer.front().y + offsetY)/m_height;
//        return;        
//    }
//
//    int depth = 1;
//    int currentIndex;
//    currentIndex = floor(m_pdf_buffer.size()/2);
//    while (true) {
//        ++depth;
//        float tmp = m_pdf_buffer[currentIndex-1].pdf;
//        if (r < tmp) {
//            currentIndex -= floor(m_pdf_buffer.size()/(powf(2,depth)));
//        }
//        else {
//            if (r > m_pdf_buffer[currentIndex].pdf) {
//                currentIndex += floor(m_pdf_buffer.size()/(powf(2, depth)));
//            }
//            else {
//                *x = (m_pdf_buffer[currentIndex].x + offsetX)/m_width;
//                *y = (m_pdf_buffer[currentIndex].y + offsetY)/m_height;
//                return;
//            }
//        }
//    } 
    assert(false);
}