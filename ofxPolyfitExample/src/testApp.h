#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"
#include "ofxPolyFit.h"

#include <sstream>

#define WIDTH 1000.0f
#define HEIGHT 500.0f

typedef double DataType;
class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
    
        ofxPolyFit_<DataType>	fit;
        pfitDataSet<DataType>	dataSet;
		ofxPolyFit_<DataType>	ransac;
        pfitDataSet<DataType>	dataSetRansac;
    
        float   fundamental(float x);
    
        void    initialiseData();
    
        int     nNoisyPoints;
        int     nCrappyPoints;
        int     nPoints;
    
        float   meanNoiseOnValid;
    
        bool	isAllocated;
    
        float   mouseX;

};

#endif
