#pragma once

#include <stdio.h>
#include <sstream> 
#include <cstring> 
#include <string.h>
#include <vector>

#include "ofxAndroid.h"
#include "ofxOsc.h"
#include "ofxGui.h"
#include "ofxUI.h"
#include "ofxHTTP.h"
#include "ofxOpenCv.h"	
#include "ofxAruco.h"


#include "ofMain.h"
#include "Button.h"
#include "Module.h"
#include "Channel.h"
#include "Menu.h"
#include "Wall.h"
#include "Sensors.h"


//#define WALLNUMBER 1 
#define PORT 9999
#define RECEIVEPORT 9010
#define HTTPPORT 8000

//default size of buttons
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
	void batteryInfo();

	void OSCcatch();
	void loadChannelXml();

	void audioReceived(	float * input,int bufferSize,int nChannels);
	//bool isInVector(vector<int> pagesVector, int value);

	
	int pageLevel;
	int previousPageLevel;
	int slide; //printed page of demo channel on home page
	int buttonNumber;
	bool isButtonActioned;
	int IDbuttonsCount;
	int IDmyChannelsCount;
	
	Channel *channelSelected;
	Channel *tempChannel;
	bool isTempChannelCreated;
	int wallSelected;
	bool wantToSaveAutomix;
	
	vector<Button*> GUIbuttons;

	char stringFile[7];
	
	ofTrueTypeFont font;
	ofTrueTypeFont titleFont;
	ofImage background;
	vector<Channel*> demoChannels;
	int demoChannelNumber;
	vector<Channel*> myChannels;
	
	ofxXmlSettings modSettings;

	
	ofxUICanvas *guiChannelSettings;
	ofxUISlider *settingsSliders[5];
	ofxUIDropDownList *moodList;
	
	Menu *appMenu;
	Wall *appWall;
	Sensors *appSensors;
	
	void exit(); 
	
	
	
	/*jclass javaClass;
	jobject javaObject;*/
	
	int batteryCapacity;
	int batteryStatus;
	int batteryTimeToFull;
	int batteryTimeToEmpty;
	
private:
	vector<ofxOscSender> senders;
	ofxOscReceiver receiver;
	ofxOscSender sender;
	
	//IP of each wall server (only one wall working for the moment)
	string hosts[2] = {"192.168.1.10", "192.168.1.182"};
	
	string pathToServer = "http://" + hosts[0] + ":" + ofToString(HTTPPORT) + "/";
	
	//XML TODO:use it
	string wallURL = pathToServer + "wall.xml";
	string myChannelsURL = pathToServer + "myChannels.xml";
	string demoChannelsURL = pathToServer + "demoChannels.xml";
	
};
