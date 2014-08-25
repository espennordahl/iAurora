//
//  kelemenMaterial.cpp
//  Aurora
//
//  Created by Espen Nordahl on 23/07/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <iostream>

#include "kelemenMaterial.h"
#include "cookTorrance.h"
#include "shaders.h"
#include "lambert.h"
#include "log.h"
#define lcontext LOG_KelemenMaterial
#include "shadingEngine.h"

#include <math.h>
#include <sstream>

using namespace Aurora;

KelemenMaterial::KelemenMaterial( std::string name, RenderEnvironment *renderEnv,
                                 int _diffColIndex, int _specColIndex,
                                 int _roughnessIndex, float _reflectance,
                                 int normalIndex
                                 ):
Material(name, renderEnv, normalIndex),
reflectance(_reflectance),
roughnessIndex(_roughnessIndex),
specColorIndex(_specColIndex),
diffColorIndex(_diffColIndex)
{
        // lobes
    specBrdf = new CookTorrance( name + ":CookTorrance", renderEnv);
	diffBrdf = new Lambert( name + ":Lambert", renderEnv);
    
        // Reflectance is non varying for now. TODO
    ConstantShader<float> *refShd = new ConstantShader<float>(reflectance);
    reflectanceIndex = renderEnv->shadingEngine->registerShaderFloat(name + ":specReflectance", refShd);
}

#pragma mark -
#pragma mark pipeline

void KelemenMaterial::frameBegin(){
    LOG_DEBUG("FrameBegin!");
    specBrdf->frameBegin();
    diffBrdf->frameBegin();
}

void KelemenMaterial::frameEnd(){
    specBrdf->frameEnd();
    diffBrdf->frameEnd();
}

float KelemenMaterial::getAlbedo(const Vector &Vn, const Vector &Nn, const ShadingGeometry &shdGeo, float roughness){
    float albedo = 0.f;
    for (int i = 0; i < SPECALBEDO_SAMPLES; i++) {
            // sample beckman distribution
        float r1 = (float) rand()/RAND_MAX;
        float r2 = (float) rand()/RAND_MAX;
        
        float z = cosf(atanf(roughness * sqrtf(-log(1.f-r1))));
        float r = sqrtf(fmax(0.f, 1.f - z*z));
        float phi = M_PI * 2 * r2;
        float x = r * cosf(phi);
        float y = r * sinf(phi);
        
        Vector dir = normalize(Vector(x,y,z));
        Vector Hn = normalize(tangentToWorld(dir, Nn));
        
        float vdoth = dot(Vn, Hn);
        
        if (vdoth > 0.000001) {
            Vector Ln = (Hn * 2 * vdoth) - Vn;
            Ln = normalize(Ln);
            float ldotn = dot(Ln, Nn);
            float vdotn = dot(Vn, Nn);
            if (ldotn > 0.) {
                float F = schlick(vdoth, reflectance);
                float a = (F * vdotn * ldotn) / (vdoth * vdoth);
                assert(a >= 0.f && a <= 1.f);
                    // accumulate albedo
                albedo += a/((float)SPECALBEDO_SAMPLES);
            }
        }
    }
    return albedo;
}

BrdfState KelemenMaterial::getBrdf( const Vector &Vn, const Vector &Nn, const ShadingGeometry &shdGeo, bool mattePath) {

    CookTorranceParameters *ctParams = new CookTorranceParameters(
        m_renderEnv->shadingEngine->getColor(specColorIndex, shdGeo),
        m_renderEnv->shadingEngine->getFloat(roughnessIndex, shdGeo),
        reflectance);
    
    if (mattePath) {
        ctParams->roughness = ctParams->roughness * (1.-CAUSTIC_BLUR) + CAUSTIC_BLUR;
    }

    if (ctParams->reflectance < 1.f) { // exception to get specular response only (for debug purposes)
            // get albedo
        float albedo = getAlbedo(Vn, Nn, shdGeo, ctParams->roughness); // TODO: should't need to clamp here
        assert(albedo <= 1. && albedo >= 0.);
        
            // importance sample brdfs
        float r = (float) rand()/RAND_MAX;
        if (r > albedo) {
            delete ctParams;
            LambertParameters *params = new LambertParameters(
            m_renderEnv->shadingEngine->getColor(diffColorIndex, shdGeo)
            );
            BrdfState state = {diffBrdf, (bxdfParameters*)params};
            return state;
        }
    }
    
    BrdfState state = {specBrdf, (bxdfParameters*) ctParams};
    return state;
}
