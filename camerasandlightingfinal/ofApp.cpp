
//--------------------------------------------------------------
//
//  Kevin M. Smith
//
//  Mars HiRise Project - startup scene
// 
//  This is an openFrameworks 3D scene that includes an EasyCam
//  and example 3D geometry which I have reconstructed from Mars
//  HiRis photographs taken the Mars Reconnaisance Orbiter
//
//  You will use this source file (and include file) as a starting point
//  to implement assignment 5  (Parts I and II)
//
//  Please do not modify any of the keymappings.  I would like 
//  the input interface to be the same for each student's 
//  work.  Please also add your name/date below.

//  Please document/comment all of your work !
//  Have Fun !!
//
// Student Names:
//    Jonathan Su:
//      - Provided Octree Solution
//      - Found Burger Models
//      - Tested Emmitter
//    Yukai Yang
//      - Combined Midterm and Octree Solution
//      - Imported Burger Model
//      - Wrote Collision detector
//    David
//      - Provided Midterm Solution
//      - Tested Camera
//  Date: 04/19/2018


#include "ofApp.h"
#include "Util.h"



//--------------------------------------------------------------
// setup scene, lighting, state and load geometry
//
void ofApp::setup(){

	bWireframe = false;
	bDisplayPoints = false;
	bAltKeyDown = false;
	bCtrlKeyDown = false;
	bRoverLoaded = false;
	bTerrainSelected = true;
//	ofSetWindowShape(1024, 768);
	cam.setDistance(10);
	cam.setNearClip(.1);
	cam.setFov(65.5);   // approx equivalent to 28mm in 35mm format
	cam.setPosition(ofVec3f(-20,50,50));
	ofSetVerticalSync(true);
	cam.disableMouseInput();
	ofEnableSmoothing();
	ofEnableDepthTest();
	ofEnableLighting();
	// setup rudimentary lighting 
	//
	//initLightingAndMaterials();

    //load background image
    background.load("geo/background/background.jpg");
    
	//mars.loadModel("geo/mars-low-v2.obj");
    mars.loadModel("geo/island2/island.obj");
	mars.setScaleNormalization(false);
    
	boundingBox = meshBounds(mars.getMesh(0));
	
    
    /* Tree Setup */
    float treeStart = ofGetElapsedTimeMillis();
    
    //create Octree to x levels
    tree.create(mars.getMesh(0),20);
    
    float treeEnd = ofGetElapsedTimeMillis();
    cout << "Time creating Octree: " << treeEnd - treeStart << " milliseconds" << endl;
    
    
    //draw tree to 5 levels
    levels = 5;
    currLevel = 0;
    
    //gui.setup();
    //gui.add(levelSlider.setup("Draw Levels", 5, 0, 15));
    
    /* particle Setup */
    p.position = ofVec3f(0,40,0);
    p.radius = .5;
    particle.add(p);
    
    //create forces
    tf = new ThrustForce(ofVec3f(0,0,0));
    tf2 = new ThrustForce(ofVec3f(0,0,0));
    ipf = new ImpulseRadialForce(100);
    gf = new GravityForce(ofVec3f(0,-.1,0));
    impulseForce = new ImpulseForce();
    restitution = 0.2;
    
    particle.addForce(impulseForce);
    particle.addForce(tf);
    particle.addForce(gf);
    particle.setLifespan(1000000000);
    
    emitter.sys->addForce(tf2);
    emitter.sys->addForce(ipf);
    
    emitter.setRandomLife(true);
    emitter.setLifespan(10000);
    emitter.setLifespanRange(ofVec2f(0.01,.3));
    emitter.setPosition(particle.particles[0].position);
    emitter.setGroupSize(20);
    emitter.setOneShot(false);
    emitter.setRate(0);
    emitter.start();
    
    
    burger.model.loadModel("geo/burger/burger.obj");
    burger.modelLoaded = true;
    burger.sys = particle;
    burger.model.setScaleNormalization(false);
    
    burger.model.setScale(.5,.5,.5);

    Box b = meshBounds(burger.model.getMesh(0));
    burgerBBox = Box(b.min() * .5, b.max() *.5);
    
    thrusterSound.load("sounds/thrusterSound.mp3");
    //thrusterSound.setLoop(true);
    thrusterSound.setMultiPlay(true);

	cam.setTarget(burger.getPosition());
	cam.lookAt(burger.getPosition());
	trackingpos = mars.getPosition() + ofVec3f(0,50,0);

	/* light setup*/
	keyLight.setup();
	keyLight.enable();
	keyLight.setAreaLight(1, 1);
	keyLight.setAmbientColor(ofFloatColor(.1, .1, .1));
	keyLight.setDiffuseColor(ofFloatColor(1, 1, 1));
	keyLight.setSpecularColor(ofFloatColor(1, 1, 1));
	keyLight.rotate(45, ofVec3f(0, 1, 0));
	keyLight.rotate(-45, ofVec3f(1, 0, 0));
	keyLight.setPosition(ofVec3f(-18, 100, -122));

	//gui.setup();
	//gui.add(keylightpos.setup("KeyLightPosition", ofVec3f(0, 0, 0), ofVec3f(-200, -200, -200), ofVec3f(200, 200, 200)));

	fillLight.setup();
	fillLight.enable();
	fillLight.setSpotlight();
	fillLight.setScale(.05);
	fillLight.setSpotlightCutOff(15);
	fillLight.setAttenuation(2, .001, .001);
	fillLight.setAmbientColor(ofFloatColor(0.1, 0.1, 0.1));
	fillLight.setDiffuseColor(ofFloatColor(1, 1, 1));
	fillLight.setSpecularColor(ofFloatColor(1, 1, 1));
	fillLight.rotate(-10, ofVec3f(1, 0, 0));
	fillLight.rotate(-45, ofVec3f(0, 1, 0));
	fillLight.setPosition(ofVec3f(-81, 81, 94));
	//gui.add(filllightpos.setup("FillLightPosition", ofVec3f(0, 0, 0), ofVec3f(-200, -200, -200), ofVec3f(200, 200, 200)));

	rimLight.setup();
	rimLight.enable();
	rimLight.setSpotlight();
	rimLight.setScale(.05);
	rimLight.setSpotlightCutOff(30);
	rimLight.setAttenuation(.2, .001, .001);
	rimLight.setAmbientColor(ofFloatColor(.1, 0.1, .1));
	rimLight.setDiffuseColor(ofFloatColor(1, 1, 1));
	rimLight.setSpecularColor(ofFloatColor(1, 1, 1));
	rimLight.rotate(180, ofVec3f(0, 1, 0));
	rimLight.setPosition(ofVec3f(-4, 70, -200));
	//gui.add(rimLightpos.setup("RimLightPosition", ofVec3f(0, 0, 0), ofVec3f(-200, -200, -200), ofVec3f(200, 200, 200)));


//    cout <<burger.model.getNumMeshes();
//    ofVec3f min = burger.model.getSceneMin();
//    ofVec3f max = burger.model.getSceneMax();
//
//    cout << "min: " <<min.x <<", "<< min.y <<", "<<min.z<<endl;
//    cout << "max: " <<max.x <<", "<< max.y <<", "<<max.z<<endl;
//    a = Box(Vector3(min.x,min.y,min.z),Vector3(max.x,max.y,max.z));

}

//--------------------------------------------------------------
// incrementally update scene (animation)
//
void ofApp::update() {
    //levels = levelSlider;
	//keyLight.setPosition(keylightpos);
	//fillLight.setPosition(filllightpos);
	//rimLight.setPosition(rimLightpos);

    //particle.update();
    emitter.setPosition(burger.sys.particles[0].position);
    emitter.update();
    burger.update();
    collisionDetect();

	sidecampos = burger.getPosition() + ofVec3f(0, -3, 0);
	topcampos = burger.getPosition() + ofVec3f(0, 4, 0);
	if (tracking) {
		cam.setTarget(burger.getPosition());
		cam.lookAt(burger.getPosition());
	}
	else if (top) {
		cam.setPosition(topcampos);
		cam.lookAt(burger.getPosition() + ofVec3f(0, 0, 2));
	}
	else if (side) {
		cam.setPosition(sidecampos);
		cam.setTarget(burger.getPosition() + ofVec3f(0,-10,0));
	}
	else {
		cam.lookAt(burger.getPosition());
	}
    //    burgerBBox = meshBounds(burger.model.getMesh(0));

}
//--------------------------------------------------------------
void ofApp::draw(){
//	ofBackgroundGradient(ofColor(20), ofColor(0));   // pick your own backgroujnd
	ofBackground(ofColor::black);
//	cout << ofGetFrameRate() << endl;
    ofSetDepthTest(false);
    ofSetColor(255,255,255,255);
    background.draw(0,0, 1200,1200);
    //gui.draw();
    ofSetDepthTest(true);
    
	cam.begin();
	ofPushMatrix();

	//keyLight.draw();
	//fillLight.draw();
	//rimLight.draw();

	if (bWireframe) {                    // wireframe mode  (include axis)
		ofDisableLighting();
		ofSetColor(ofColor::slateGray);
		mars.drawWireframe();
		if (bRoverLoaded) {
			rover.drawWireframe();
			if (!bTerrainSelected) drawAxis(rover.getPosition());
		}
		if (bTerrainSelected) drawAxis(ofVec3f(0, 0, 0));
	}
	else {
		ofEnableLighting();              // shaded mode
		mars.drawFaces();

		if (bRoverLoaded) {
			rover.drawFaces();
			if (!bTerrainSelected) drawAxis(rover.getPosition());
		}
		if (bTerrainSelected) drawAxis(ofVec3f(0, 0, 0));
	}


	if (bDisplayPoints) {                // display points as an option    
		glPointSize(3);
		ofSetColor(ofColor::green);
		mars.drawVertices();
	}
    
    /*
	// highlight selected point (draw sphere around selected point)
	//
	if (bPointSelected) {
		ofSetColor(ofColor::blue);
		ofDrawSphere(selectedPoint, .1);
	}
	*/
	ofNoFill();
	ofSetColor(ofColor::white);
    drawBox(boundingBox);
    
    //ofVec3f p = burger.model.getPosition();
    //Vector3 pos = Vector3(p.x,p.y,p.z);
    //drawBox(burgerBBox);
    //drawBox(a);
    
    //draw to 5 levels
    //tree.draw(levels, currLevel);
    //tree.draw(15, currLevel);

 
    burger.draw();
    //particle.draw();
    emitter.draw();
    
    ofPopMatrix();
	cam.end();
}

// Draw an XYZ axis in RGB at world (0,0,0) for reference.
//
void ofApp::drawAxis(ofVec3f location) {

	ofPushMatrix();
	ofTranslate(location);

	ofSetLineWidth(1.0);

	// X Axis
	ofSetColor(ofColor(255, 0, 0));
	ofDrawLine(ofPoint(0, 0, 0), ofPoint(1, 0, 0));
	

	// Y Axis
	ofSetColor(ofColor(0, 255, 0));
	ofDrawLine(ofPoint(0, 0, 0), ofPoint(0, 1, 0));

	// Z Axis
	ofSetColor(ofColor(0, 0, 255));
	ofDrawLine(ofPoint(0, 0, 0), ofPoint(0, 0, 1));

	ofPopMatrix();
}


void ofApp::keyPressed(int key) {

	switch (key) {
	case 'C':
	case 'c':
		if (cam.getMouseInputEnabled()) cam.disableMouseInput();
		else cam.enableMouseInput();
		break;
	case 'F':
	case 'f':
		ofToggleFullscreen();
		break;
	case 'H':
	case 'h':
        break;
	case '1':
		cam.disableMouseInput();
		side = false;
		top = false;
		cam.setPosition(trackingpos);
		tracking = true;
		break;
	case '2':
		//top
		cam.disableMouseInput();
		tracking = false;
		side = false;
		cam.setPosition(topcampos);
		cam.lookAt(mars.getPosition());
		top = true;
		break;
	case '3':
		//side
		cam.disableMouseInput();
		tracking = false;
		top = false;
		cam.setPosition(sidecampos);
		cam.lookAt(cam.getPosition());
		side = true;
		break;
	case '4':
		//free
		cam.enableMouseInput();
		tracking = false;
		side = false;
		top = false;
		cam.setPosition(ofVec3f(50, 50, 50));
		cam.lookAt(mars.getPosition());
		break;
    case 'I':
    case 'i':
    {
        ofVec3f vel = burger.sys.particles[0].velocity;
        cout << "velocity: " << vel << endl;
        impulseForce->apply(-ofGetFrameRate() * vel);
        break;
    }
		break;
	case 'r':
		cam.reset();
		break;
	case 's':
		savePicture();
		break;
	case 't':
		setCameraTarget();
		break;
	case 'u':
		break;
	case 'v':
		togglePointsDisplay();
		break;
	case 'V':
		break;
	case 'w':
		toggleWireframeMode();
		break;
	case OF_KEY_ALT:
		cam.enableMouseInput();
		bAltKeyDown = true;
		break;
	case OF_KEY_CONTROL:
		bCtrlKeyDown = true;
		break;
	case OF_KEY_SHIFT:
            emitter.stop();
		break;
	case OF_KEY_DEL:
		break;
    
        case OF_KEY_UP:
            emitter.sys->reset();
            emitter.setRate(20);
            emitter.setVelocity(ofVec3f(0,-1,0));
            tf->add(ofVec3f(0,-.2,0));
            tf2->set(ofVec3f(0,0,0));
            gf->set(ofVec3f(0,-.1,0));
            emitter.start();
            if (!thrusterSound.isPlaying())
                thrusterSound.play();
            break;
        case OF_KEY_DOWN:
            emitter.sys->reset();
            emitter.setRate(20);
            emitter.setVelocity(ofVec3f(0,-1,0));
            tf->add(ofVec3f(0,.2,0));
            tf2->set(ofVec3f(0,-10,0));
            gf->set(ofVec3f(0,-.1,0));
            emitter.start();
            if (!thrusterSound.isPlaying())
                thrusterSound.play();
            break;
        case OF_KEY_LEFT:
            emitter.sys->reset();
            emitter.setRate(20);
            emitter.setVelocity(ofVec3f(-1,-2,0));
            tf->add(ofVec3f(.2,0,0));
            tf2->set(ofVec3f(-10,0,0));
            gf->set(ofVec3f(0,-.1,0));
            emitter.start();
            if (!thrusterSound.isPlaying())
                thrusterSound.play();
            break;
        case OF_KEY_RIGHT:
            emitter.sys->reset();
            emitter.setRate(20);
            emitter.setVelocity(ofVec3f(1,-2,0));
            tf->add(ofVec3f(-.2,0,0));
            tf2->set(ofVec3f(10,0,0));
            gf->set(ofVec3f(0,-.1,0));
            emitter.start();
            if (!thrusterSound.isPlaying())
                thrusterSound.play();
            break;
	default:
		break;
	}
}

void ofApp::toggleWireframeMode() {
	bWireframe = !bWireframe;
}

void ofApp::toggleSelectTerrain() {
	bTerrainSelected = !bTerrainSelected;
}

void ofApp::togglePointsDisplay() {
	bDisplayPoints = !bDisplayPoints;
}

void ofApp::keyReleased(int key) {

	switch (key) {
	
	case OF_KEY_ALT:
		cam.disableMouseInput();
		bAltKeyDown = false;
		break;
	case OF_KEY_CONTROL:
		bCtrlKeyDown = false;
		break;
	case OF_KEY_SHIFT:
		break;
        case OF_KEY_UP:
            emitter.setRate(0);
            emitter.setVelocity(ofVec3f(0,0,0));
            tf->set(ofVec3f(0,0,0));
            tf2->set(ofVec3f(0,0,0));
            thrusterSound.stop();
            emitter.sys->reset();
            break;
        case OF_KEY_DOWN:
            emitter.setRate(0);
            emitter.setVelocity(ofVec3f(0,0,0));
            tf->set(ofVec3f(0,0,0));
            tf2->set(ofVec3f(0,0,0));
            thrusterSound.stop();
            emitter.sys->reset();
            break;
        case OF_KEY_LEFT:
            emitter.setRate(0);
            emitter.setVelocity(ofVec3f(0,0,0));
            tf->set(ofVec3f(0,0,0));
            tf2->set(ofVec3f(0,0,0));
            thrusterSound.stop();
            emitter.sys->reset();
            break;
        case OF_KEY_RIGHT:
            emitter.setRate(0);
            emitter.setVelocity(ofVec3f(0,0,0));
            tf->set(ofVec3f(0,0,0));
            tf2->set(ofVec3f(0,0,0));
            thrusterSound.stop();
            emitter.sys->reset();
            break;
	default:
		break;

	}
}



//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
}


//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
    /*
    ofVec3f mouse(mouseX, mouseY);
    ofVec3f rayPoint = cam.screenToWorld(mouse);
    ofVec3f rayDir = rayPoint - cam.getPosition();
    rayDir.normalize();
    Ray ray = Ray(Vector3(rayPoint.x, rayPoint.y, rayPoint.z),
                  Vector3(rayDir.x, rayDir.y, rayDir.z));
    
    TreeNode temp;
    float searchStart = ofGetSystemTimeMicros();
    if (tree.intersect(ray, tree.root, temp)) {
        float searchEnd = ofGetSystemTimeMicros();
        cout << "Search Time in Microseconds: " << searchEnd - searchStart<< endl;
        selectedPoint= mars.getMesh(0).getVertex(temp.points[0]);
        bPointSelected = true;
    }
    else
        bPointSelected = false;
    */
}



//Return an array of points
//Referenced from Kevin Smith 2018 README file
int ofApp::getMeshPointsInBox(const ofMesh &mesh, const vector<int> &points, Box &box, vector<int> &pointsRtn) {
    int count = 0;
    
    for (int i = 0; i < points.size(); i++) {
        ofVec3f v = mesh.getVertex(points[i]);
        
        if (box.inside(Vector3(v.x, v.y, v.z))) {
            //cout << points[i] << endl;
            count++;
            pointsRtn.push_back(points[i]);
        }
    }
    return count;
}

//draw a box from a "Box" class  
//
void ofApp::drawBox(const Box &box) {
	Vector3 min = box.parameters[0];
	Vector3 max = box.parameters[1];
	Vector3 size = max - min;
	Vector3 center = size / 2 + min;
	ofVec3f p = ofVec3f(center.x(), center.y(), center.z());
	float w = size.x();
	float h = size.y();
	float d = size.z();
	ofDrawBox(p, w, h, d);
}

//  Subdivide a Box into eight(8) equal size boxes, return them in boxList;
//
void ofApp::subDivideBox8(const Box &box, vector<Box> & boxList) {
    Vector3 min = box.parameters[0];
    Vector3 max = box.parameters[1];
    Vector3 size = max - min;
    Vector3 center = size / 2 + min;
    float xdist = (max.x() - min.x()) / 2;
    float ydist = (max.y() - min.y()) / 2;
    float zdist = (max.z() - min.z()) / 2;
    Vector3 h = Vector3(0, ydist, 0);
    
    //  generate ground floor
    //
    Box b[8];
    b[0] = Box(min, center);
    b[1] = Box(b[0].min() + Vector3(xdist, 0, 0), b[0].max() + Vector3(xdist, 0, 0));
    b[2] = Box(b[1].min() + Vector3(0, 0, zdist), b[1].max() + Vector3(0, 0, zdist));
    b[3] = Box(b[2].min() + Vector3(-xdist, 0, 0), b[2].max() + Vector3(-xdist, 0, 0));
    
    boxList.clear();
    for (int i = 0; i < 4; i++)
        boxList.push_back(b[i]);
    
    // generate second story
    //
    for (int i = 4; i < 8; i++) {
        b[i] = Box(b[i - 4].min() + h, b[i - 4].max() + h);
        boxList.push_back(b[i]);
    }
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {
    //cout << "X: " << x << " Y: " << y << endl;
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//
//  Select Target Point on Terrain by comparing distance of mouse to 
//  vertice points projected onto screenspace.
//  if a point is selected, return true, else return false;
//
bool ofApp::doPointSelection() {

	ofMesh mesh = mars.getMesh(0);
	int n = mesh.getNumVertices();
	float nearestDistance = 0;
	int nearestIndex = 0;

	bPointSelected = false;

	ofVec2f mouse(mouseX, mouseY);
	vector<ofVec3f> selection;

	// We check through the mesh vertices to see which ones
	// are "close" to the mouse point in screen space.  If we find 
	// points that are close, we store them in a vector (dynamic array)
	//
	for (int i = 0; i < n; i++) {
		ofVec3f vert = mesh.getVertex(i);
		ofVec3f posScreen = cam.worldToScreen(vert);
		float distance = posScreen.distance(mouse);
		if (distance < selectionRange) {
			selection.push_back(vert);
			bPointSelected = true;
		}
	}

	//  if we found selected points, we need to determine which
	//  one is closest to the eye (camera). That one is our selected target.
	//
	if (bPointSelected) {
		float distance = 0;
		for (int i = 0; i < selection.size(); i++) {
			ofVec3f point =  cam.worldToCamera(selection[i]);

			// In camera space, the camera is at (0,0,0), so distance from 
			// the camera is simply the length of the point vector
			//
			float curDist = point.length(); 

			if (i == 0 || curDist < distance) {
				distance = curDist;
				selectedPoint = selection[i];
			}
		}
	}
	return bPointSelected;
}

// Set the camera to use the selected point as it's new target
//  
void ofApp::setCameraTarget() {

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
// setup basic ambient lighting in GL  (for now, enable just 1 light)
//
void ofApp::initLightingAndMaterials() {

	static float ambient[] =
	{ .5f, .5f, .5, 1.0f };
	static float diffuse[] =
	{ 1.0f, 1.0f, 1.0f, 1.0f };

	static float position[] =
	{5.0, 5.0, 5.0, 0.0 };

	static float lmodel_ambient[] =
	{ 1.0f, 1.0f, 1.0f, 1.0f };

	static float lmodel_twoside[] =
	{ GL_TRUE };


	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, position);

	glLightfv(GL_LIGHT1, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT1, GL_POSITION, position);


	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
	glLightModelfv(GL_LIGHT_MODEL_TWO_SIDE, lmodel_twoside);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
//	glEnable(GL_LIGHT1);
	glShadeModel(GL_SMOOTH);
} 

void ofApp::savePicture() {
	ofImage picture;
	picture.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
	picture.save("screenshot.png");
	cout << "picture saved" << endl;
}

//--------------------------------------------------------------
//
// support drag-and-drop of model (.obj) file loading.  when
// model is dropped in viewport, place origin under cursor
//
void ofApp::dragEvent(ofDragInfo dragInfo) {

	ofVec3f point;
	mouseIntersectPlane(ofVec3f(0, 0, 0), cam.getZAxis(), point);

	if (rover.loadModel(dragInfo.files[0])) {
		rover.setScaleNormalization(false);
		rover.setScale(.005, .005, .005);
		rover.setPosition(point.x, point.y, point.z);
		bRoverLoaded = true;
	}
	else cout << "Error: Can't load model" << dragInfo.files[0] << endl;
}

bool ofApp::mouseIntersectPlane(ofVec3f planePoint, ofVec3f planeNorm, ofVec3f &point) {
	ofVec2f mouse(mouseX, mouseY);
	ofVec3f rayPoint = cam.screenToWorld(mouse);
	ofVec3f rayDir = rayPoint - cam.getPosition();
	rayDir.normalize();
	return (rayIntersectPlane(rayPoint, rayDir, planePoint, planeNorm, point));
}

// return a Mesh Bounding Box for the entire Mesh
//
Box ofApp::meshBounds(const ofMesh & mesh) {
    int n = mesh.getNumVertices();
    ofVec3f v = mesh.getVertex(0);
    ofVec3f max = v;
    ofVec3f min = v;
    for (int i = 1; i < n; i++) {
        ofVec3f v = mesh.getVertex(i);
        
        if (v.x > max.x) max.x = v.x;
        else if (v.x < min.x) min.x = v.x;
        
        if (v.y > max.y) max.y = v.y;
        else if (v.y < min.y) min.y = v.y;
        
        if (v.z > max.z) max.z = v.z;
        else if (v.z < min.z) min.z = v.z;
    }
    return Box(Vector3(min.x, min.y, min.z), Vector3(max.x, max.y, max.z));
}

// Yukai Yang
// Taken from Kevin Smith's instructional video
void ofApp:: collisionDetect() {
    //cout<< "check"<<endl;
    Vector3 c = burgerBBox.center();
    contactPt = ofVec3f(c.x(),c.y() - burgerBBox.height()/2, c.z()) + burger.getPosition();
    ofVec3f vel = burger.sys.particles[0].velocity;
    
    if(vel.y > 0) return;
    TreeNode node;
    
    if(vel == ofVec3f(0,0,0)) return;
	
    if(tree.intersect (contactPt,tree.root,node)) {
        bCollision = true;
        cout << "collision" <<endl;
        
        // Impulse force
        ofVec3f norm = ofVec3f(0,1,0);
        ofVec3f f = (restitution + 1.0) * ((-vel.dot(norm))*norm);

        //impulseForce->apply(ofGetFrameRate() * -vel);
        tf->set(ofVec3f(0,0,0));
        tf2-> set(ofVec3f(0,0,0));
        gf->set(ofVec3f(0,0,0));
        emitter.setVelocity(ofVec3f(0,10,0));
        impulseForce->apply(ofGetFrameRate() * f);

//        cout << "velocity: " <<vel <<endl;
//        cout << "impulse: " << f <<endl;
//        cout << "framerate: "<< ofGetFrameRate()<<endl;
//        cout << "new vel: " << burger.sys.particles[0].velocity<<endl;
        emitter.stop();
    }
	
    
}

