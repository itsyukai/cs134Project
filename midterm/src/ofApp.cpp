#include "ofApp.h"
//Jonathan Su

//--------------------------------------------------------------
void ofApp::setup(){

	//
	// set up camera stuff
	//
	cam.setDistance(10);
	cam.setNearClip(.1);
	cam.setFov(65.5);   // approx equivalent to 28mm in 35mm format
	ofSetVerticalSync(true);
	ofSetFrameRate(60);
	ofSetBackgroundColor(ofColor::black);

    p.position = ofVec3f(0,4,0);
    p.radius = .5;
    particle.add(p);
    
    tf = new ThrustForce(ofVec3f(0,0,0));
    tf2 = new ThrustForce(ofVec3f(0,0,0));
    
    particle.addForce(tf);
    particle.setLifespan(1000);
    emitter.sys->addForce(tf2);
    
    emitter.setEmitterType(DirectionalEmitter);
    emitter.setLifespan(1000);
    emitter.setPosition(particle.particles[0].position);
}

//--------------------------------------------------------------
void ofApp::update(){

	// udpate your objects here
    
    //particle.position = emitter.sys->particles[0].position;
    particle.update();
    emitter.setPosition(particle.particles[0].position);
    emitter.update();
}

//--------------------------------------------------------------
void ofApp::draw(){

	cam.begin();

		// draw grid
		//
		ofPushMatrix();
		ofRotate(90, 0, 0, 1);
		ofSetLineWidth(1);
		ofSetColor(ofColor::dimGrey);
		ofDrawGridPlane();
		ofPopMatrix();

		// draw your objects here

    particle.draw();
    emitter.draw();
    
	cam.end();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

	switch (key) {
	case 'F':
	case 'f':
		ofToggleFullscreen();
		break;
	case OF_KEY_UP:
            emitter.start();
            emitter.setRate(8);
            emitter.setVelocity(ofVec3f(0,15,0));
            tf->add(ofVec3f(0,-1,0));
            tf2->add(ofVec3f(0,1,0));
            soundPlayer.play();
		break;
	case OF_KEY_DOWN:
            emitter.start();
            emitter.setRate(8);
            emitter.setVelocity(ofVec3f(0,-15,0));
            tf->add(ofVec3f(0,1,0));
            tf2->add(ofVec3f(0,-1,0));
            soundPlayer.play();
		break;
	case OF_KEY_LEFT:
            emitter.start();
            emitter.setRate(8);
            emitter.setVelocity(ofVec3f(-15,0,0));
            tf->add(ofVec3f(1,0,0));
            tf2->add(ofVec3f(-1,0,0));
            soundPlayer.play();
		break;
	case OF_KEY_RIGHT:
            emitter.start();
            emitter.setRate(8);
            emitter.setVelocity(ofVec3f(15,0,0));
            tf->add(ofVec3f(-1,0,0));
            tf2->add(ofVec3f(1,0,0));
            soundPlayer.play();
		break;
	case 'h':
		break;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    switch (key) {
        case OF_KEY_UP:
            emitter.setRate(0);
            tf->set(ofVec3f(0,0,0));
            tf2->set(ofVec3f(0,0,0));
            soundPlayer.stop();
            break;
        case OF_KEY_DOWN:
            emitter.setRate(0);
            tf->set(ofVec3f(0,0,0));
            tf2->set(ofVec3f(0,0,0));
            soundPlayer.stop();
            break;
        case OF_KEY_LEFT:
            emitter.setRate(0);
            tf->set(ofVec3f(0,0,0));
            tf2->set(ofVec3f(0,0,0));
            soundPlayer.stop();
            break;
        case OF_KEY_RIGHT:
            emitter.setRate(0);
            tf->set(ofVec3f(0,0,0));
            tf2->set(ofVec3f(0,0,0));
            soundPlayer.stop();
            break;
        case 'h':
            break;
    }
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

void ofApp::playSound() {
	if (soundFileLoaded) soundPlayer.play();
}
