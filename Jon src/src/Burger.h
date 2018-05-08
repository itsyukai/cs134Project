//
//  Burger.h
//  LunarLander
//
//  Created by Yukai Yang on 5/5/18.
//

#pragma once
#include "ofMain.h"
#include "ParticleSystem.h"
#include "ofxAssimpModelLoader.h"


class Burger{
public:
    Burger(){modelLoaded = false;};

    ParticleSystem sys;
    ofxAssimpModelLoader model;
    
    bool modelLoaded;
    
    void draw();
    void update();
};
