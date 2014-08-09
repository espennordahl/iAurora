//
//  openexrDisplay.h
//  Aurora
//
//  Created by Espen Nordahl on 05/07/2012.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef Aurora_openexrDisplay_h
#define Aurora_openexrDisplay_h

#include "core.h"
#include "frontEndObject.h"

#include <string.h>

namespace Aurora {
	class Display : FrontEndObject{
	public:	
		Display(int width, int height, std::string file, RenderEnvironment * renderEnv);
		
		void setPixel(int width, int height, const Color &col, float alpha);
        void appendValue(int width, int height, const Color &col, float alpha);
        
        int height() const;
        int width() const;
        
        void frameBegin();
        void frameEnd();
        
        u_char *pixels();

	private:
        int m_width;
        int m_height;
        std::vector<std::vector < int > > m_multisample_buffer;
        std::vector<std::vector < Color > > m_rgb;
        std::vector<std::vector < float > > m_a;
        u_char *m_pixels;
	};
}


#endif
