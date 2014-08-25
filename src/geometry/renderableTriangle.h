//
//  renderableTriangle.h
//  Aurora
//
//  Created by Espen Nordahl on 07/07/2012.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef Aurora_renderableTriangle_h
#define Aurora_renderableTriangle_h

#include "core.h"
#include "renderableGeometry.h"
#include "triangleMesh.h"

namespace Aurora {
	class RenderableTriangle : public RenderableGeometry {
	public:
        RenderableTriangle(){};
		RenderableTriangle( const Point &p1, const Point &p2, const Point &p3, const Vector &n1, const Vector &n2, const Vector &n3, int _attributes );

		BBox worldBound() const;
		
//		bool intersectBinary( Ray *ray ) const;
//		bool intersect( Ray *ray, Intersection *intersection ) const;

		Point p1;
		Point p2;
		Point p3;
            // precomputed for intersection
//		Vector e1;
//		Vector e2;
            // normals
        Vector n1;
        Vector n2;
        Vector n3;
        
        inline bool intersectBinary( Ray *ray ) const{
            Vector e1 = p2 - p1;
            Vector e2 = p3 - p1;

                // compute s1
            Vector s1 = cross(ray->direction, e2);
            float divisor = dot(s1, e1);
            if (divisor == 0.) {
                return false;
            }
            
            float invDivisor = 1.f/divisor;
            
                // compute first barycentric coordinate
            
            Vector d = ray->origin - p1;
            float b1 = dot(d, s1) * invDivisor;
            if (b1 < 0. || b1 > 1.) {
                return false;
            }
            
                // compute second barycentric coordinate
            Vector s2 = cross(d, e1);
            float b2 = dot(ray->direction, s2) * invDivisor;
            if (b2 < 0. || (b1 + b2) > 1.) {
                return false;
            }
            
                // compute t to intersection point
            float t = dot(e2,s2) * invDivisor;
            if (t < ray->mint || t > ray->maxt) {
                return false;
            }
            
            return true;
            
        }
        
        inline bool intersect( Ray *ray, Intersection *intersection ) const{
            Vector e1 = p2 - p1;
            Vector e2 = p3 - p1;

                // compute s1
            Vector s1 = cross(ray->direction, e2);
            float divisor = dot(s1, e1);
            if (divisor == 0.) {
                return false;
            }
            
            float invDivisor = 1.f/divisor;
            
                // compute first barycentric coordinate
            
            Vector d = ray->origin - p1;
            float b1 = dot(d, s1) * invDivisor;
            if (b1 < 0. || b1 > 1.) {
                return false;
            }
            
                // compute second barycentric coordinate
            Vector s2 = cross(d, e1);
            float b2 = dot(ray->direction, s2) * invDivisor;
            if (b2 < 0. || (b1 + b2) > 1.) {
                return false;
            }
            
                // compute t to intersection point
            float t = dot(e2,s2) * invDivisor;
            if (t < ray->mint || t > ray->maxt) {
                return false;
            }
            
                // Hit!
            ray->maxt = t;
            
                // fill in differential geo
            Point hitp = ray->origin + (ray->direction * t);
            
            
                //	Vector norm = cross(e1, e2);
                // compute final barycentric coordinate
            float b0 = 1 - b1 - b2;
            Vector norm = Vector(
                                 b0 * n1[0] + b1 * n2[0] + b2 * n3[0],
                                 b0 * n1[1] + b1 * n2[1] + b2 * n3[1],
                                 b0 * n1[2] + b1 * n2[2] + b2 * n3[2]);
                //	if (dot(norm, ray->direction) > 0.) {
                //		norm *= -1;
                //	}
            
            intersection->hitN = normalize(norm);
            intersection->hitP = hitp;
            intersection->attributesIndex = attributes;
            intersection->shdGeo.barCoords = Point(b0,b1,b2);
            intersection->shdGeo.Ng = normalize(norm);
            intersection->shdGeo.Ns = normalize(norm);
            intersection->shdGeo.P = hitp;
            
            return true;
        }
	};
}

#endif
