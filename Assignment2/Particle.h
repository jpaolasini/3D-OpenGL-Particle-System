 //
//  Particle.h
//  C++ Tutorial
//
//  Created by Jason Paolasini on 2013-09-29.
//  Copyright (c) 2013 Jason Paolasini. All rights reserved.
//

#ifndef __C___Tutorial__Particle__
#define __C___Tutorial__Particle__

#include <iostream>
#include "3DMathLib.h"



class particle
{
    public:
        particle();
        particle(point3D inPoint,double inSpeed,vec3D inDirection,vec3D inSpin,float inSize,colour inColour,int inBounce,bool inBomb);
        point3D point;
        float size;
        double speed;
        point3D location;
        colour colour;
        vec3D direction;
        particle riseUp();
        int bounce;
        bool bomb;
        vec3D spin;
    
    
    
};
#endif /* defined(__C___Tutorial__Particle__) */
