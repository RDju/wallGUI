#include <stdio.h>
#include <sstream> 
#include <cstring> 
#include <string.h>
#include <vector>
#pragma once

#include "ofMain.h"
#include "Button.h"
#include "ofxAndroid.h"
#include "ofxOsc.h"


#define HOST "192.168.0.11"
#define PORT 2323


#define X_BUTTONS 936 //TODO: en %
#define Y_BUTTONS 129

#define CHANNELSNUMBER 3





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
	char* channelsNames[CHANNELSNUMBER] = {"CHANNEL 1",  "CHANNEL 2", "CHANNEL 3"};
	
	ofTrueTypeFont font;
	ofImage background;
	ofSoundPlayer ring;
	
	vector<Button*> GUIbuttons;

private:
	ofxOscSender sender;
	
};
