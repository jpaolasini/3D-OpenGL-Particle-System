//
//  Particle.cpp
//  C++ Tutorial
//
//  Created by Jason Paolasini on 2013-09-29.
//  Copyright (c) 2013 Jason Paolasini. All rights reserved.
//

#include "Particle.h"
#include "3DMathLib.h"


particle::particle(){
    
}
/*The particle class was created to keep track of the attributes each particle has in the system
 */


particle::particle(point3D inPoint,double inSpeed,vec3D inDirection,vec3D inSpin,float inSize,class colour inColour,int inBounce,bool inBomb){
    location = inPoint;
    speed = inSpeed;
    size = inSize;
    colour = inColour;
    direction = inDirection;
    bounce = inBounce;
    bomb = inBomb;
    spin = inSpin;
    
    
    
}

