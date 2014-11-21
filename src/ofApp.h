#include <stdio.h>
#include <sstream> 
#include <cstring> 
#include <string.h>
#include <vector>
#pragma once

#include "ofMain.h"
#include "Button.h"
#include "Module.h"
#include "ofxAndroid.h"
#include "ofxOsc.h"


#define HOST "192.168.0.33"
#define PORT 2323

#define X_BUTTONS 0.73*ofGetWidth()
#define Y_BUTTONS 0.17*ofGetHeight()

//pagesLevel
#define INTRO_PAGE 1
#define MENU_PAGE 2
#define WALLCREATION_PAGE 3
#define CHANNELCREATION_PAGE 4
#define CHANNELSELECT_PAGE 7

#define CHANNELSNUMBER 3

#define SCREEN_MAX_NUMBER 5

#define XGRID 
#define YGRID 




class ofApp : public ofxAndroidApp {
	
public:

	void setup();
	void gridSetup();
	void update();
	void draw();
	void keyPressed(int key);
	void keyReleased(int key);
	void windowResized(int w, int h);

	void touchDown(int x, int y, int id);
	void touchMoved(int x, int y, int id);
	void touchUp(int x, int y, int id);
	void touchDoubleTap(int x, int y, int id);
	void touchCancelled(int x, int y, int id);
	void swipe(ofxAndroidSwipeDir swipeDir, int id);

	void pause();
	void stop();
	void resume();
	void reloadTextures();

	bool backPressed();
	void okPressed();
	void cancelPressed();
	
	void drawGrid();
	void updateGridRepresentation();
	int rankInTouchOrder(int);
	
	//bool isInVector(vector<int> pagesVector, int value);

	
	int pageLevel;
	int previousPageLevel;
	int buttonNumber;
	bool isButtonActioned;
	int activChannel;
	int IDbuttonsCount;
	int IDmodulesCount;
	

	char stringFile[7];
	char* channelsNames[CHANNELSNUMBER] = {"CHANNEL 1",  "CHANNEL 2", "CHANNEL 3"};
	
	ofTrueTypeFont font;
	ofImage background;
	ofSoundPlayer ring;
	
	ofRectangle gridSize;
	ofRectangle gridLines[13];//Contient les coordonnées de chaque ligne de la grille (en relatif par rapport au xy de gridSize
	
	vector<int> touchOrder;
	int gridRepresentation[7][7];
	
	
	vector<Button*> GUIbuttons;
	vector<Module*> GUImodules;

private:
	ofxOscSender sender;
	
};
