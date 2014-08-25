//
//  renderableTriangle.cpp
//  Aurora
//
//  Created by Espen Nordahl on 07/07/2012.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <iostream>

#include "renderableTriangle.h"

using namespace Aurora;

RenderableTriangle::RenderableTriangle( const Point &_p1, const Point &_p2, const Point &_p3, const Vector &v1, const Vector &v2, const Vector &v3, int _attributes ) : RenderableGeometry(_attributes){
	p1 = _p1;
	p2 = _p2;
	p3 = _p3;
//	e1 = p2 - p1;
//	e2 = p3 - p1;
    n1 = v1;
    n2 = v2;
    n3 = v3;
}


BBox RenderableTriangle::worldBound() const{
	BBox bounds = BBox(p1, p2);
	bounds.Union(p3);
	return bounds;
}