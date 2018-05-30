#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
    ofSetFrameRate(60);
    ofSetVerticalSync(true);
    
    int space = 100;
    int w = iconSize.x;
    int h = iconSize.y;
    int offsetH = ofGetHeight()/2;
    int offsetW = (ofGetWidth()-(NUM_ICON*w+(NUM_ICON-1)*space))/2;

    fbo.allocate(w, h,GL_RGBA,8);
    
    myGlitch.setup(&fbo);
    myGlitch.setFx(OFXPOSTGLITCH_CUTSLIDER, true);

    for(int i=0; i<NUM_ICON; i++){
        initPoints[i][0] = ofPoint(i*(space+w)+offsetW,offsetH-h/2);
        initPoints[i][1] = ofPoint(i*(space+w)+w+offsetW, offsetH-h/2);
        initPoints[i][2] = ofPoint(i*(space+w)+offsetW,offsetH+h/2);
        initPoints[i][3] = ofPoint(i*(space+w)+w+offsetW,offsetH+h/2);
        
        warpers[i].setSourceRect(ofRectangle(0, 0, w, h));
        warpers[i].setTopLeftCornerPosition(initPoints[i][0]);
        warpers[i].setTopRightCornerPosition(initPoints[i][1]);
        warpers[i].setBottomLeftCornerPosition(initPoints[i][2]);
        warpers[i].setBottomRightCornerPosition(initPoints[i][3]);
        
        warpers[i].setup();
        warpers[i].setAnchorSize(5);
        warpers[i].load(warpersPaths[i]); // reload last saved changes.
        
        imgs[i].load(imgsPaths[i]);

    }
   
    
    
    // set up user interface so we can tweak the projection
    gui.setup();
    gui.setSize(400, gui.getHeight());
    gui.add(bgColor.set("BackGround Adjust", 0.f, 0.f, 255.f));
    gui.add(bgIconColor.set("BackGround Icon Adjust", 0.f, 0.f, 255.f));
    
    // load the settings from the previous time we ran the application
    gui.loadFromFile("settings.xml");
    gui.setWidthElements(400);
    
    lastMiracle = ofGetElapsedTimeMillis();
    timeToNextMiracle =  ofRandom(1000,2000);
    iconMiracle = -1;
    
    isDebug = false;
    //ofToggleFullscreen();
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
            imgs[i].draw(0, 0,iconSize.x,iconSize.y);
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
        gui.draw();
        ofShowCursor();
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

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}
