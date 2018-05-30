#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxQuadWarp.h"
#include "ofxPostGlitch.h"


#define NUM_ICON 3

class ofApp : public ofBaseApp {
    
public:
		
    void setup();
    void update();
    void draw();
    void exit();

    void keyPressed(int key);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);

private:

    ofxQuadWarp warpers[NUM_ICON];
    string warpersPaths[NUM_ICON] = {"quadwarpA.xml","quadwarpB.xml","quadwarpC.xml"};
    
    ofImage imgs[NUM_ICON];
    ofPoint initPoints[NUM_ICON][4];
    string imgsPaths[NUM_ICON] = {"mappingA.png","mappingB.png","mappingC.png"};
    ofFbo fbo;
    
    // user interface
    ofxPanel gui;
    ofParameter<float> bgColor;
    ofParameter<float> bgIconColor;
    
    bool isDebug;
    
    uint64_t lastMiracle;
    float timeToNextMiracle;
    int iconMiracle;
    const ofVec2f iconSize = ofVec2f(160,290);
    ofxPostGlitch    myGlitch;

};
