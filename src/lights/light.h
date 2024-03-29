//
//  light.h
//  Aurora
//
//  Created by Espen Nordahl on 05/07/2012.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef Aurora_light_h
#define Aurora_light_h

#include "core.h"
#include "renderableTriangle.h"
#include "attributeState.h"

namespace Aurora {
    
    enum LightType{
        type_envLight,
        type_areaLight
    };
    
	class Light : public FrontEndObject {
	public:
		Light( Transform *o2c, Transform *c2o, Transform *o2w, Transform *w2o, Transform *c2w, Transform *w2c, float exposure, Color color, std::string name, RenderEnvironment *renderEnv);
		
            // Generates a sample on the light to be used during light transport.
		virtual Sample3D generateSample( const Point &orig, const Vector &Nn, const IntegrationDomain &integrationDomain ) = 0;
		
            // Evalues the light emmision for a given sample
		virtual Color eval( const Sample3D &sample, const Vector &Nn ) = 0;
		
            // Evalutes the pdf for a given sample
		virtual float pdf( Sample3D *sample, const Vector &Nn, const IntegrationDomain &integrationDomain ) = 0;

        virtual bool visible( const Point &orig, const Vector &Nn, const IntegrationDomain &integrationDomain ) = 0;
        
        float exposure() const {
            return m_exposure;
        }
        
        virtual void setExposure(float newExposure){
            m_exposure = newExposure;
            m_intensity = powf(2,m_exposure);
        }
        
            // Checks whether a ray intersects the light or not.
		virtual bool intersectBinary( Ray *ray ) const = 0;
		
            // Converts the light into renderable triangles.
		virtual void makeRenderable(std::vector<RenderableTriangle> &renderable, AttributeState *attrs, int index)= 0;
	        
            // TODO: Do we need this?
        virtual Color emission() = 0;
        
            // TODO: Do we need this?
        virtual std::shared_ptr<Shape> shape() = 0;
        
        void applyAttributeChange(const AttributeChange &change);
        
            // TODO: Do we need this?
        LightType lightType;
    
            // light color tint
		Color color;

	protected:
		Transform *objectToCamera;
		Transform *cameraToObject;
		Transform *objectToWorld;
		Transform *worldToObject;
		Transform *cameraToWorld;
		Transform *worldToCamera;
		
            // light Exposure
		float m_exposure;
        float m_intensity;

            // TODO: Do we need this?
        int numSamples;
	};
}

#endif
