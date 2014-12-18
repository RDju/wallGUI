#pragma once

#include <stdio.h>
#include <sstream> 
#include <cstring> 
#include <string.h>
#include <vector>



#include "ofMain.h"
#include "Button.h"
#include "Module.h"
#include "Channel.h"
#include "Menu.h"
#include "Wall.h"
#include "ofxAndroid.h"
#include "ofxOsc.h"
#include "ofxGui.h"
#include "ofxUI.h"


#define WALLNUMBER 1
#define PORT 9001
#define RECEIVEPORT 9003

#define X_BUTTONS 0.73*ofGetWidth()
#define Y_BUTTONS 0.17*ofGetHeight()

//pagesLevel
#define HOME_PAGE 1
#define WALLCREATION_PAGE 2
#define CHANNELSELECT_PAGE 3
#define AUTOMIX_PAGE 4
#define SEARCH_PAGE 5
#define CHANNELDISPLAY_PAGE 7
#define SETTINGS_PAGE 6

#define CHANNELSNUMBER 3

#define XGRID 
#define YGRID 


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
	
	void moodEvent(ofxUIEventArgs &e);
	

	void pause();
	void stop();
	void resume();
	void reloadTextures();

	bool backPressed();
	void okPressed();
	void cancelPressed();

	void OSCcatch();
	
	//bool isInVector(vector<int> pagesVector, int value);

	
	int pageLevel;
	int previousPageLevel;
	int buttonNumber;
	bool isButtonActioned;
	int activChannel;
	int IDbuttonsCount;
	
	Channel *channelSelected;
	Channel *tempChannel;
	bool isTempChannelCreated;
	int wallSelected;
	bool wantToSaveAutomix;
	
	vector<Button*> GUIbuttons;

	char stringFile[7];
	char* channelsNames[CHANNELSNUMBER] = {"CHANNEL 1",  "CHANNEL 2", "CHANNEL 3"};
	
	ofTrueTypeFont font;
	ofTrueTypeFont titleFont;
	ofImage background;
	vector<Channel*> demoChannels;
	vector<Channel*> myChannels;

	
	ofxUICanvas *guiChannelSettings;
	ofxUISlider *settingsSliders[5];
	ofxUIDropDownList *moodList;
	
	Menu *appMenu;
	Wall *appWall;
	
	void exit(); 
	
	//XML
	string wallURL = "http://192.168.1.14:8000/wallConf.txt";
	string myChannelsURL = "http://192.168.1.14:8000/myChannelsConf.txt";
	string demoChannelsURL = "http://192.168.1.14:8000/demoChannelsConf.txt"
	
private:
	vector<ofxOscSender> senders;
	ofxOscReceiver receiver;
	char* hosts[2] = {"192.168.1.17", "192.168.1.182"};
	
};
