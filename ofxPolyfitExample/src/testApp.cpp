#include "testApp.h"

float testApp::fundamental(float x)
{
    //This is the actual function that the
    //data pattern follows.
    //even though we generally expect
    //there to also be noise and bad data
    
    //feel free to change this function
    //and see how polyfit reacts
    
    return 400.0f + 0.5 * x - 0.001 * x * x;
}

//--------------------------------------------------------------
void testApp::setup(){
    
    ofBackground(80, 80, 80);
    ofSetWindowShape(WIDTH, HEIGHT);
    ofSetVerticalSync(true);
	ofEnableSmoothing();
    
    mouseX = WIDTH / 2.0f;
    
    nNoisyPoints = 50;
    nCrappyPoints = 20;
    meanNoiseOnValid = 20.0f;
    
    isAllocated = false;
    
    fit.init(2, 1, 1, BASIS_SHAPE_SQUARE);
    //ransac.init(2, 1, 1, BASIS_SHAPE_SQUARE);
    
    initialiseData();

}

void testApp::initialiseData()
{
    
    nPoints = nNoisyPoints + nCrappyPoints;
    dataSet.init(1, 1, nPoints);
    
    DataType *X = dataSet.getInput();
    DataType *Y = dataSet.getOutput();
    
    ////////////////////////
    // Noisy points
    ////////////////////////
    //
    float xStep = WIDTH / float(nNoisyPoints-1);
    
    for (int i = 0; i < nNoisyPoints; i++)
    {
        X[i] = i * xStep;
        Y[i] = fundamental(X[i]) + ofRandomf() * meanNoiseOnValid;
    }
    //
    ////////////////////////
    
    
    
    ////////////////////////
    // Crappy points
    ////////////////////////
    //
    xStep = WIDTH / float(nCrappyPoints-1);
    
    for (int i = 0; i < nCrappyPoints; i++)
    {
        X[i + nNoisyPoints] = i * xStep;
        Y[i + nNoisyPoints] = ofRandomuf() * HEIGHT;
    }
    //
    ////////////////////////
    
    
    ////////////////////////
    // Perform standard fit
    ////////////////////////
    //
    fit.correlate(dataSet);
    //ransac.RANSAC(X, Y, nPoints, 50, 0.1, 20.0f, 0.5f);
    //    
    ////////////////////////
}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){
    
    ofPushStyle();
    ofEnableSmoothing();
    
    ///////////////////////
    //  Draw fundamental
    ///////////////////////
    //
    ofPushStyle();
    ofSetLineWidth(8.0f);
    ofEnableAlphaBlending();
    ofSetColor(255, 255, 255, 50);
    ofNoFill();
    
    ofBeginShape();
    
    for (float x=0; x<WIDTH; x++)
        ofCurveVertex(x, fundamental(x));
    
    ofEndShape(false);
    
    ofPopStyle();
    //
    ///////////////////////
    
    
    
    ///////////////////////////
    //  Draw standard fit line
    ///////////////////////////
    //
    ofPushStyle();
    ofSetLineWidth(1.0f);
    ofSetColor(200, 200, 200);
    ofNoFill();
    
    ofBeginShape();
    
    for (float x=0; x<WIDTH; x++)
        ofCurveVertex(x, fit.evaluate(x));
    
    ofEndShape(false);
    
    ofPopStyle();
    //
    ///////////////////////

    
    /*
    ///////////////////////////
    //  Draw RANSAC fit line
    ///////////////////////////
    //
    ofPushStyle();
    ofSetLineWidth(1.0f);
    ofSetColor(255, 0, 0);
    ofNoFill();
    
    ofBeginShape();
    
    for (float x=0; x<WIDTH; x++)
        ofCurveVertex(x, ransac.evaluate(x));
    
    ofEndShape(false);
    
    ofPopStyle();
    //
    ///////////////////////
    */
    
    
    ///////////////////////
    //  Draw data points
    ///////////////////////
    //
    ofPushStyle();
    ofSetLineWidth(1.0f);
    ofFill();
    ofEnableAlphaBlending();
    ofSetCircleResolution(6);
    
    DataType *X = dataSet.getInput();
    DataType *Y = dataSet.getOutput();
    
    
    ofSetColor(150, 255, 150, 150);
    for (int i=0; i<nNoisyPoints; i++)
        ofCircle(X[i], Y[i], 5.0f);
    
    ofSetColor(255, 150, 150, 150);
    for (int i=nNoisyPoints; i<nCrappyPoints + nNoisyPoints; i++)
        ofCircle(X[i], Y[i], 5.0f);
    
    ofPopStyle();
    //
    ///////////////////////
    
    
    /*
    ///////////////////////////////////////
    //  Highlight selected consensus points
    ///////////////////////////////////////
    //
    ofPushStyle();
    ofNoFill();
    ofSetColor(255, 255, 255);
    ofSetLineWidth(1);
    ofSetCircleResolution(4);
    
    set<int>::iterator setIt;
    
    for (setIt = ransac.bestConsensus.begin(); setIt != ransac.bestConsensus.end(); setIt++)
        ofCircle(X[*setIt], Y[*setIt], 5);

    ofPopStyle();
    //
    ///////////////////////////////////////    
    */
    
    ///////////////////////
    //  Draw cursor line
    ///////////////////////
    //
    ofPushStyle();
    ofSetColor(255,255,255);
    ofSetLineWidth(1);
    
    ofLine(mouseX, 0, mouseX, HEIGHT);
    
    ofPopStyle();
    
    stringstream strLookups;
    
    int idxNoisyData = mouseX/WIDTH * float(nNoisyPoints-1);
    int idxCrappyData = mouseX/WIDTH * float(nCrappyPoints-1);
    
    ofPushStyle();
    ofNoFill();
    ofSetColor(255, 255, 255);
    ofSetLineWidth(1);
    ofSetCircleResolution(12);
    
    ofCircle(X[idxNoisyData], Y[idxNoisyData], 10);
    ofCircle(X[idxCrappyData + nNoisyPoints], Y[idxCrappyData + nNoisyPoints], 10);
    
    ofPopStyle();
    
    strLookups << "Base function [" << mouseX << "]: " << fundamental(mouseX) << "\n";
    
    strLookups << "Noisy data [" << X[idxNoisyData] << "]: " << Y[idxNoisyData] << "\n";
    
    strLookups << "Crappy data [" << X[nNoisyPoints + idxCrappyData] << "]: " << Y[nNoisyPoints + idxCrappyData] << "\n";    
    
    strLookups << "Standard fit [" << mouseX << "]: " << fit.evaluate(mouseX) << "\n";    
    
    //strLookups << "RANSAC fit [" << idxCrappyData << "]: " << ransac.evaluate(mouseX) << "\n";    
    
    ofDrawBitmapString(strLookups.str(), 10, 10);
    //
    ///////////////////////
    
    
    ofPopStyle();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    if (key ==' ')
        initialiseData();
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
    mouseX = x;
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
    mouseX = x;
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

