//
//  Burger.cpp
//  LunarLander
//
//  Created by Yukai Yang on 5/5/18.
//

#include "Burger.h"

void Burger::draw(){
    if (modelLoaded){
        model.drawFaces();
    }
    else
        sys.draw();
    
}

void Burger::update(){
    sys.update();
    model.setPosition(sys.particles[0].position.x,sys.particles[0].position.y,sys.particles[0].position.z);
}
