#include "ofApp.h"

using namespace std;

//--------------------------------------------------------------
void ofApp::setup(){
	
	sender.setup( HOST, PORT );
	
	font.loadFont("OpenSans-Light.ttf",25);
	
	pageLevel=1;
	previousPageLevel=0;
	buttonNumber=-1;
	activChannel=-1;

	ofEnableAlphaBlending();
	
	//Buttons
	vector<int> tempVector(1, 7);
	Button *tempButton = new Button(1, 100, 100, tempVector, "POULET", "FRITE");
	GUIbuttons.push_back(tempButton);
	
	
	
	ofxOscMessage m;
	m.setAddress( "/INIT");
	sender.sendMessage( m );
	
	
	
}

//--------------------------------------------------------------
void ofApp::update() {
	ostringstream convert; 
	convert << pageLevel;  
	
	if (pageLevel!=previousPageLevel && (pageLevel == 1 || pageLevel == 7) ){
		strcpy (stringFile, (convert.str()).c_str());
	  	strcat (stringFile,".png");
	   
	    background.loadImage(stringFile);
    }
}

//--------------------------------------------------------------
void ofApp::draw() {
	
    background.draw(0, 0);
    
	switch(pageLevel){
	    ofFill();
		ofSetColor(0,0,0,100);
		    
	    case 1:
	    	break;
	    case 7:
	    
	    	for (int i=1; i<=CHANNELSNUMBER; i++){
	    	
	    		//Nom des chaines
	    		font.drawString(channelsNames[i-1], 30 ,   Y_BUTTONS+font.getLineHeight() +(i-1)*(HEIGHT_BUTTONS+10));
	    		
	    		//tour bouton
	    		if (i==buttonNumber) ofFill();
	    		else ofNoFill();
	    		ofRect(X_BUTTONS, Y_BUTTONS + (i-1)*(HEIGHT_BUTTONS+10), WIDTH_BUTTONS, HEIGHT_BUTTONS );
	    		
	    		//Play / stop
	    		if (i == activChannel)
	    			font.drawString("STOP",X_BUTTONS+WIDTH_BUTTONS/2-font.stringWidth("STOP")/2,   Y_BUTTONS+font.getLineHeight() +(i-1)*(HEIGHT_BUTTONS+10));
	    		else font.drawString("PLAY",X_BUTTONS+WIDTH_BUTTONS/2-font.stringWidth("PLAY")/2,Y_BUTTONS+font.getLineHeight()+(i-1)*(HEIGHT_BUTTONS+10));
	    	
	    	}
	    		//tour bouton
	    		if (buttonNumber==4) ofFill();
	    		else ofNoFill();
	    		ofRect(X_BUTTONS, ofGetHeight()-100 , WIDTH_BUTTONS, HEIGHT_BUTTONS );
	    		
	    		font.drawString("EXIT",X_BUTTONS+WIDTH_BUTTONS/2-font.stringWidth("EXIT")/2, ofGetHeight()-100 + font.getLineHeight());
	    		
	    	break;
	    
	}
		
	previousPageLevel = pageLevel;
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::touchDown(int x, int y, int id){
	
	switch (pageLevel) {
		case 7:
			for (int i=1; i <= CHANNELSNUMBER; i++){
				if (x > X_BUTTONS && x < X_BUTTONS + WIDTH_BUTTONS && y > Y_BUTTONS + (i-1)*(HEIGHT_BUTTONS +10) && y < Y_BUTTONS + (i-1)*(HEIGHT_BUTTONS +10) + HEIGHT_BUTTONS)
						buttonNumber=i;
			}
			break;
	}
}

//--------------------------------------------------------------
void ofApp::touchMoved(int x, int y, int id){
	
}

//--------------------------------------------------------------
void ofApp::touchUp(int x, int y, int id){

	ofxOscMessage m;
	buttonNumber = -1;
	switch (pageLevel) {
		case 1:
			pageLevel = 7;
			
			
	        m.setAddress( "/STOP");
	        sender.sendMessage( m );
	        
			break;
			
		case 7:
			for (int i=1; i<=CHANNELSNUMBER; i++){
				if (x > X_BUTTONS && x < X_BUTTONS + WIDTH_BUTTONS && y > Y_BUTTONS + (i-1)*(HEIGHT_BUTTONS +10) && y < Y_BUTTONS + (i-1)*(HEIGHT_BUTTONS +10) + HEIGHT_BUTTONS){
					 if (activChannel!=i){
					 	
	        			 m.setAddress( "/PLAY/Chan"+ofToString(i-1));
	        			 sender.sendMessage( m );
	        			 activChannel=i;
	        			 
	        		} else {
	        		
	        			 m.setAddress( "/STOP" );
	        			 sender.sendMessage( m );
	        			 activChannel=-1;
	        		}
				}
				
				//Exit app
				if (x > X_BUTTONS && x < X_BUTTONS + WIDTH_BUTTONS && y>ofGetHeight()-100 && y < ofGetHeight()-100 + HEIGHT_BUTTONS){
					buttonNumber=4;
					
					ofxOscMessage m;
					m.setAddress( "/EXIT");
					sender.sendMessage( m );
	
	
					ofApp::exit();
					//std::exit(EXIT_SUCCESS);
					OF_EXIT_APP(0);  
					
				}
			}
			break;
	}
}

//--------------------------------------------------------------
void ofApp::touchDoubleTap(int x, int y, int id){

}

//--------------------------------------------------------------
void ofApp::touchCancelled(int x, int y, int id){

}

//--------------------------------------------------------------
void ofApp::swipe(ofxAndroidSwipeDir swipeDir, int id){

}

//--------------------------------------------------------------
void ofApp::pause(){

}

//--------------------------------------------------------------
void ofApp::stop(){

}

//--------------------------------------------------------------
void ofApp::resume(){

}

//--------------------------------------------------------------
void ofApp::reloadTextures(){

}

//--------------------------------------------------------------
bool ofApp::backPressed(){
	return false;
}

//---------

void ofApp::okPressed(){

}

void ofApp::cancelPressed(){

}