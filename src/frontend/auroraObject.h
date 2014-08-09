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
        
            // Splits the shape into triangles.
		void dice( std::vector<std::shared_ptr<Geometry> > &diced);
        
            // Called once all objects have been parsed, and
            // is responsible for initializing the correct AttributeState for the object
            // and convert all geometry to renderable triangles and add them to the
            // passed vector.
		void makeRenderable(std::vector<RenderableTriangle> &renderable, AttributeState *attrs, int index);

        
        std::string m_matName;

		Material *m_material;
        
        std::shared_ptr<Shape> m_shape;
	};
    
    typedef std::shared_ptr<AuroraObject> ObjectPtr;
}


#endif
