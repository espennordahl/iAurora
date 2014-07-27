//
//  auroraObject.h
//  Aurora
//
//  Created by Espen Nordahl on 01/07/2012.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef Aurora_auroraObject_h
#define Aurora_auroraObject_h

#include "core.h"
#include "shape.h"
#include "material.h"
#include "renderableTriangle.h"
#include "attributeState.h"
#include "attributeChange.h"

namespace Aurora {
	class AuroraObject : public FrontEndObject {
	public:	
		AuroraObject(std::string name, RenderEnvironment *renderEnv): FrontEndObject(name, renderEnv) { };
		AuroraObject( std::string name, RenderEnvironment *renderEnv, ShapePtr shape, Material *material );
		AuroraObject( std::string name, RenderEnvironment *renderEnv, ShapePtr shape, std::string matName );

		BBox worldBound();
		BBox objectBound();
        
        void frameBegin();
        void frameEnd();
        
        void applyAttributeChange(const AttributeChange &change);
        
        std::string m_matName;

		Material *m_material;
        
        std::shared_ptr<Shape> m_shape;
	};
    
    typedef std::shared_ptr<AuroraObject> ObjectPtr;
}


#endif
