//
//  DistortablePlane.cpp
//  ofxPolyFitExample-VideoDistort
//

#include "DistortablePlane.h"

DistortablePlane::DistortablePlane() :
radius(15.0f) {
	
	this->source = 0;
	this->shift = false;
	
	fit.init(2, 2, 2, BASIS_SHAPE_SQUARE);
	
	calibration.init(2, 2);	
	calibration.resize(9);
	selection = calibration.begin();
	
	this->setBounds( ofRectangle(100, 100, 300, 300) );
	this->setCalibrateMode(false);
	this->setGridResolution(10);
	
	ofAddListener(ofEvents().mousePressed, this, &DistortablePlane::mousePressed);
    ofAddListener(ofEvents().mouseDragged, this, &DistortablePlane::mouseDragged);
	ofAddListener(ofEvents().keyPressed, this, &DistortablePlane::keyPressed);
	ofAddListener(ofEvents().keyReleased, this, &DistortablePlane::keyReleased);
}

void DistortablePlane::draw() {
	if (this->source != 0) {
		source->getTextureReference().bind();
		mesh.draw();
		source->getTextureReference().unbind();
	}
	
	if (this->calibrateMode) {
		ofPushStyle();
		ofNoFill();
		ofEnableAlphaBlending();
		
		ofSetColor(255, 255, 255, 50);
		mesh.drawWireframe();
		ofSetColor(255, 255, 255);
		
		pfitDataPointf point;
		for (point = calibration.begin(); point != calibration.end(); ++point) {
			
			if (selection == point)
				ofSetLineWidth(3.0f);
			else
				ofSetLineWidth(1.0f);
			
			ofCircle(point.getOutput()[0], point.getOutput()[1], radius);
		}
		ofPopStyle();
	}
}

void DistortablePlane::setSource(ofBaseHasTexture &source) {
	this->source = &source;
	defaultBounds.width = source.getTextureReference().getWidth();
	defaultBounds.height = source.getTextureReference().getHeight();
	this->updateGrid();
}

void DistortablePlane::setCalibrateMode(const bool calibrateMode) {
	this->calibrateMode = calibrateMode;
}

bool DistortablePlane::getCalibrateMode() const {
	return this->calibrateMode;
}

bool DistortablePlane::toggleCalibrateMode() {
	this->calibrateMode ^= true;
}

void DistortablePlane::setGridResolution(int resolution) {
	
	if (resolution < 2) {
		ofLogError("DistortablePlane") << "Resolution must be 2 or greater";
		return;
	}
	
	this->gridResolution = resolution;
	this->updateGrid();
}

void DistortablePlane::resetCalibration() {
	
	float xstep = this->defaultBounds.width / 2;
	float ystep = this->defaultBounds.height / 2;

	pfitDataPointf point = calibration.begin();
	for (int y = 0; y < 3; y++) {
		for (int x = 0; x < 3; x++) {
			point.getInput()[0] = x / 2.0f;
			point.getInput()[1] = y / 2.0f;
			
			point.getOutput()[0] = defaultBounds.x + x * xstep;
			point.getOutput()[1] = defaultBounds.y + y * ystep;
			
			++point;
		}
	}	
}

void DistortablePlane::nextHandle() {
	++selection;
	if (selection == calibration.end())
		selection = calibration.begin();
}

void DistortablePlane::updateGrid() {
	
	if (source == 0) {
		ofLogError("DistortablePlane") << "Cannot update the grid without a source ofBaseHasTexture attached";
		return;
	}
	
	mesh.clear();
	
	float resByGridX = source->getTextureReference().getWidth() / (gridResolution - 1);
	float resByGridY = source->getTextureReference().getHeight() / (gridResolution - 1);
	
	fit.correlate(calibration);
	pfitDataPointf point(2, 2);

	for (int y = 0; y < gridResolution; y++) {
		for (int x = 0; x < gridResolution; x++) {
			point.getInput()[0] = x / (gridResolution - 1.0f);
			point.getInput()[1] = y / (gridResolution - 1.0f);
			
			fit.evaluate(point);
			
			mesh.addVertex( ofVec3f(point.getOutput()[0],
									point.getOutput()[1],
									0.0f) );
			mesh.addTexCoord(ofVec2f(resByGridX * x, resByGridY * y));
		}
	}
	
	for (int y = 0; y < gridResolution - 1; y++) {
		for (int x = 0; x < gridResolution - 1; x++) {
			mesh.addIndex( x + gridResolution * y);
			mesh.addIndex( x + 1 + gridResolution * y);
			mesh.addIndex( x + gridResolution * (y + 1));
			
			mesh.addIndex( x + 1 + gridResolution * y);
			mesh.addIndex( x + 1 + gridResolution * (y + 1));
			mesh.addIndex( x + gridResolution * (y + 1));
		}
	}
}

ofVec2f & DistortablePlane::getHandle() {
	return * (ofVec2f*) this->selection.getOutput();
}

void DistortablePlane::keyPressed(ofKeyEventArgs &args) {
	if (this->calibrateMode == false)
		return;
	
	float step = this->shift ? 10.0f : 1.0f;
	
	switch(args.key) {
		case OF_KEY_SHIFT:
			this->shift = true;
			break;
			
		case OF_KEY_LEFT:
			this->getHandle().x-= step;
			break;
		case OF_KEY_RIGHT:
			this->getHandle().x+= step;
			break;
		case OF_KEY_UP:
			this->getHandle().y-= step;
			break;
		case OF_KEY_DOWN:
			this->getHandle().y+= step;
			break;
			
		case '\t':
			this->nextHandle();
			break;
		case 'r':
			this->resetCalibration();
			break;		
	}
	
	this->updateGrid();
}

void DistortablePlane::keyReleased(ofKeyEventArgs &args) {
	if (args.key == OF_KEY_SHIFT)
		this->shift = false;
}

void DistortablePlane::mousePressed(ofMouseEventArgs &args) {
	if (this->calibrateMode == false)
		return;
	
	pfitDataPointf point;
	
	for(point = calibration.begin(); point != calibration.end(); ++point) {
		ofVec2f mouse(args.x, args.y);
		ofVec2f target(point.getOutput()[0], point.getOutput()[1]);
		float distance = (mouse - target).length();
		
		if (distance < radius) {
			hit = true;
			selection = point;
			startDragDiff = target - mouse;
			return;
		}
	}
	
	hit = false;
}

void DistortablePlane::mouseDragged(ofMouseEventArgs &args) {
	if (this->calibrateMode == false)
		return;
	
	if (hit) {
		selection.getOutput()[0] = args.x + startDragDiff.x;
		selection.getOutput()[1] = args.y + startDragDiff.y;
		updateGrid();
	}
}

void DistortablePlane::load(string filename) {
	this->calibration.load(filename);
	this->updateGrid();
}

void DistortablePlane::save(string filename) {
	this->calibration.save(filename);
}

void DistortablePlane::setBounds(const ofRectangle & bounds) {
	this->defaultBounds = bounds;
	this->resetCalibration();
}