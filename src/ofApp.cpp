#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
    ofSetFrameRate(60);
    ofSetVerticalSync(true);
    
    int space = 100*PIXEL_MM;
    int w = iconSize.x*PIXEL_MM;
    int h = iconSize.y*PIXEL_MM;
    int offset = (w*3+space*2)/2;

    fbo.allocate(w, h,GL_RGBA,8);
    
    myGlitch.setup(&fbo);
    myGlitch.setFx(OFXPOSTGLITCH_CUTSLIDER, true);

    for(int i=0; i<NUM_ICON; i++){
        warpers[i].setSourceRect(ofRectangle(0, 0, w, h));
        warpers[i].setTopLeftCornerPosition(ofPoint(0,0));
        warpers[i].setTopRightCornerPosition(ofPoint(w, 0));
        warpers[i].setBottomLeftCornerPosition(ofPoint(0,h));
        warpers[i].setBottomRightCornerPosition(ofPoint(w,h));
        
        warpers[i].setPosition((i*(space+w))-offset, -h/2);
        
        warpers[i].setup();
        warpers[i].setAnchorSize(30);
        warpers[i].load(warpersPaths[i]); // reload last saved changes.
        
        imgs[i].load(imgsPaths[i]);

    }
   
    
    projector.setPosition(0, 0, 0);
    //projector.setFov(18.79f);
    projector.setFov(29.03f);
    projector.lookAt(ofVec3f(0.f, 0.f, 0.f));
    
    // add functions to be called when the projector position and tilt are changed
    // and the boxAngle in relation to the camera
    projectorPosition.addListener(this, &ofApp::projectorPositionChanged);
    projectorTilt.addListener(this, &ofApp::projectorTiltChanged);
    
    // set up user interface so we can tweak the projection
    gui.setup();
    gui.setSize(400, gui.getHeight());
    gui.add(projectorTilt.set("projectorTilt", 0.f, -45.f, 95.f));
    gui.add(projectorPosition.set("projectorPosition",
                                  ofVec3f(0.f, 0.f, 0.f),
                                  ofVec3f(-2000.f, -2000.f, 700.f),
                                  ofVec3f(2000.f, 2000.f, 6000.f)));
    gui.add(bgColor.set("BackGround Adjust", 0.f, 0.f, 255.f));
    gui.add(bgIconColor.set("BackGround Icon Adjust", 0.f, 0.f, 255.f));
    
    // load the settings from the previous time we ran the application
    gui.loadFromFile("settings.xml");
    gui.setWidthElements(400);
    
    for(int i=0; i<NUM_ICON ; i++){
        warpers[i].setCamera(projector);
    }
    
    lastMiracle = ofGetElapsedTimeMillis();
    timeToNextMiracle =  ofRandom(1000,2000);
    iconMiracle = -1;
    
    isDebug = false;
    ofToggleFullscreen();
}

//--------------------------------------------------------------
void ofApp::update()
{

}

//--------------------------------------------------------------
void ofApp::draw() {
    ofBackground(ofColor(bgColor));
    ofSetColor(255);
    ofHideCursor();
    
    //======================== draw image into fbo.
    
    
    projector.begin();
    ofEnableDepthTest();
    
    
    if(ofGetElapsedTimeMillis()-lastMiracle > timeToNextMiracle && iconMiracle == -1){
        iconMiracle = ofRandom(3.);
        lastMiracle = ofGetElapsedTimeMillis();
    }
    
    if(ofGetElapsedTimeMillis()-lastMiracle > 120 && iconMiracle != -1){
        iconMiracle = -1;
        lastMiracle = ofGetElapsedTimeMillis();
        timeToNextMiracle = ofRandom(10000.,20000.);
        if(isDebug){
            timeToNextMiracle = ofRandom(1000.,2000.);
        }
    }
    
    
    
    for(int i=0; i<NUM_ICON ; i++){
        fbo.begin();
        ofClear(ofColor(bgIconColor));
        if(i == iconMiracle){
            imgs[i].draw(0, 0,iconSize.x*PIXEL_MM,iconSize.y*PIXEL_MM);
        }
        fbo.end();
        myGlitch.generateFx();
        
        //======================== get our quad warp matrix.
        ofMatrix4x4 mat = warpers[i].getMatrix();
        
        //======================== use the matrix to transform our fbo.
        ofPushMatrix();
        ofMultMatrix(mat);
        fbo.draw(0, 0);
        ofPopMatrix();
        if(isDebug){
            //======================== draw quad warp ui.
            ofDisableDepthTest();
            ofSetColor(0,255,255);
            warpers[i].drawQuadOutline();
            
            ofSetColor(0,255,0);
            warpers[i].drawCorners();
            
            ofSetColor(255,255,0);
            warpers[i].drawHighlightedCorner();
            
            ofSetColor(255,0,0);
            warpers[i].drawSelectedCorner();
            
            ofSetColor(255,255,255);
        }
    }
    if(isDebug){
        ofDrawAxis(500);
        ofShowCursor();
    }
    projector.end();
    ofDisableDepthTest();
    if(isDebug){
        gui.draw();
    }
}

void ofApp::exit() {
    for(int i=0; i<NUM_ICON ; i++){
        warpers[i].save(warpersPaths[i]);
    }
    gui.saveToFile("settings.xml");
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

    if(key == 'h' || key == 'H') {
        for(int i=0; i<NUM_ICON ; i++){
            warpers[i].toggleShow();
        }
    }
    
    if(key == 'l' || key == 'L') {
        for(int i=0; i<NUM_ICON ; i++){
            warpers[i].load(warpersPaths[i]);
        }
    }
    
    if(key == 's' || key == 'S') {
        for(int i=0; i<NUM_ICON ; i++){
            warpers[i].save(warpersPaths[i]);
        }
    }
    if(key == 'd' || key == 'D') {
        isDebug = !isDebug;
    }
    if(key == 'f' || key == 'F') {
        ofToggleFullscreen();
    }
    
}

void ofApp::projectorPositionChanged(ofVec3f& projectorPosition)
{
    projector.setPosition(projectorPosition*PIXEL_MM);
    for(int i=0; i<NUM_ICON ; i++){
        warpers[i].setCamera(projector);
    }
}

void ofApp::projectorTiltChanged(float& projectorTilt)
{
    // get the current orientation
    ofVec3f orientation = projector.getOrientationEuler();
    
    // change the tilt (x rotation)
    //projector.setOrientation(ofVec3f(projectorTilt, orientation.y, orientation.z));
    projector.setOrientation(ofVec3f(projectorTilt, orientation.y, orientation.z));
    for(int i=0; i<NUM_ICON ; i++){
        warpers[i].setCamera(projector);
    }
}

