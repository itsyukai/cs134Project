#pragma once

#include "ofMain.h"
#include  "ofxAssimpModelLoader.h"
#include "box.h"
#include "ray.h"
#include "Octree.h"
#include "ofxGui.h"
/*
class TreeNode {
public:
    
    void setup();
    void update();
    void draw();
    
    bool bSelected;
    
    //TreeNode *parent;
    vector<TreeNode> children;
    //vector<int> points;
    vector<int> points;
    Box box;
};
*/
class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
    void createTree(TreeNode &node);
    int getMeshPointsInBox(const ofMesh & mesh, const vector<int> & points, Box & box, vector<int> & pointsRtn);
    void drawTree(TreeNode &node, int levels, int currLevel);
    
		void drawAxis(ofVec3f);
		void initLightingAndMaterials();
		void savePicture();
		void toggleWireframeMode();
		void togglePointsDisplay();
		void toggleSelectTerrain();
		void setCameraTarget();
		bool  doPointSelection();
		void drawBox(const Box &box);
		Box meshBounds(const ofMesh &);
		void subDivideBox8(const Box &b, vector<Box> & boxList);
		bool mouseIntersectPlane(ofVec3f planePoint, ofVec3f planeNorm, ofVec3f &point);

		ofEasyCam cam;
		ofxAssimpModelLoader mars, rover;
		ofLight light;
		Box boundingBox;
		vector<Box> level1, level2, level3, level4, level5, level6;
    
    
		bool bAltKeyDown;
		bool bCtrlKeyDown;
		bool bWireframe;
		bool bDisplayPoints;
		bool bPointSelected;
		
		bool bRoverLoaded;
		bool bTerrainSelected;
	
		ofVec3f selectedPoint;
		ofVec3f intersectPoint;
    
    //TreeNode root;
    Octree tree;
    int levels;
    int currLevel;

    ofxIntSlider levelSlider;
    ofxPanel gui;
    
		const float selectionRange = 4.0;
};
