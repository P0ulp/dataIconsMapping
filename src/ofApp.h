#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxQuadWarp.h"
#include "ofxPostGlitch.h"


#define NUM_ICON 3

#define PIXEL_MM 3

class ofApp : public ofBaseApp {
    
public:
		
    void setup();
    void update();
    void draw();
    void exit();

    void keyPressed(int key);

private:
    void projectorPositionChanged(ofVec3f& projectorPosition);
    void projectorTiltChanged(float& projectorTilt);

    ofxQuadWarp warpers[NUM_ICON];
    string warpersPaths[NUM_ICON] = {"quadwarpA.xml","quadwarpB.xml","quadwarpC.xml"};
    
    ofImage imgs[NUM_ICON];
     string imgsPaths[NUM_ICON] = {"mappingA.png","mappingB.png","mappingC.png"};
    ofFbo fbo;
    
    ofCamera projector;
    
    // user interface
    ofxPanel gui;
    ofParameter<ofVec3f> projectorPosition;
    ofParameter<float> projectorTilt;
    ofParameter<float> bgColor;
    ofParameter<float> bgIconColor;
    
    bool isDebug;
    
    uint64_t lastMiracle;
    float timeToNextMiracle;
    int iconMiracle;
    const ofVec2f iconSize = ofVec2f(160,290);
    ofxPostGlitch    myGlitch;

};
