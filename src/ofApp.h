#include <stdio.h>
#include <sstream> 
#include <cstring> 
#include <string.h>
#pragma once

#include "ofMain.h"
#include "ofxAndroid.h"
#include "ofxOsc.h"


#define HOST "192.168.0.23"
#define PORT 1234

#define CHANNELSNUMBER 3
#define X_CHANNELBUTTONS 936
#define Y_CHANNELBUTTONS 129
#define WIDTH_CHANNELBUTTONS 315
#define HEIGHT_CHANNELBUTTONS 50



class ofApp : public ofxAndroidApp {
	
public:

	void setup();
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

	
	int pageLevel;
	int previousPageLevel;
	int buttonNumber;
	int activChannel;
	

	char stringFile[7];
	char* channelsNames[CHANNELSNUMBER] = {"CALIBRATION",  "MARCEL DUCHAMP", "BIG BUCK BUNNY"};;
	
	ofTrueTypeFont font;
	ofImage background;
	ofSoundPlayer ring;

private:
	ofxOscSender sender;
	
};
