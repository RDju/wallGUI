#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	
	sender.setup( HOST, PORT );
	
	font.loadFont("OpenSans-Light.ttf",25);
	
	/*channelsNames[0] = "CALIBRATION";
	channelsNames[1] = "MARCEL DUCHAMP";
	channelsNames[2] = "DEEP THROAD";*/
	
	
	pageLevel=1;
	previousPageLevel=0;
	buttonNumber=-1;
	activChannel=-1;

	ofEnableAlphaBlending();
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
	    		font.drawString(channelsNames[i-1], 30 ,   Y_CHANNELBUTTONS+font.getLineHeight() +(i-1)*(HEIGHT_CHANNELBUTTONS+10));
	    		
	    		//tour bouton
	    		if (i==buttonNumber) ofFill();
	    		else ofNoFill();
	    		ofRect(X_CHANNELBUTTONS, Y_CHANNELBUTTONS + (i-1)*(HEIGHT_CHANNELBUTTONS+10), WIDTH_CHANNELBUTTONS, HEIGHT_CHANNELBUTTONS );
	    		
	    		//Play / stop
	    		if (i == activChannel)
	    			font.drawString("STOP",X_CHANNELBUTTONS+WIDTH_CHANNELBUTTONS/2-font.stringWidth("STOP")/2,   Y_CHANNELBUTTONS+font.getLineHeight() +(i-1)*(HEIGHT_CHANNELBUTTONS+10));
	    		else font.drawString("PLAY",X_CHANNELBUTTONS+WIDTH_CHANNELBUTTONS/2-font.stringWidth("PLAY")/2,Y_CHANNELBUTTONS+font.getLineHeight()+(i-1)*(HEIGHT_CHANNELBUTTONS+10));
	    	
	    	}
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
				if (x > X_CHANNELBUTTONS && x < X_CHANNELBUTTONS + WIDTH_CHANNELBUTTONS && y > Y_CHANNELBUTTONS + (i-1)*(HEIGHT_CHANNELBUTTONS +10) && y < Y_CHANNELBUTTONS + (i-1)*(HEIGHT_CHANNELBUTTONS +10) + HEIGHT_CHANNELBUTTONS)
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
	buttonNumber = -1;
	switch (pageLevel) {
		case 1:
			pageLevel = 7;
			break;
			
		case 7:
			for (int i=1; i<=CHANNELSNUMBER; i++){
				if (x > X_CHANNELBUTTONS && x < X_CHANNELBUTTONS + WIDTH_CHANNELBUTTONS && y > Y_CHANNELBUTTONS + (i-1)*(HEIGHT_CHANNELBUTTONS +10) && y < Y_CHANNELBUTTONS + (i-1)*(HEIGHT_CHANNELBUTTONS +10) + HEIGHT_CHANNELBUTTONS){
					 if (activChannel!=i){
					 
						 ofxOscMessage m;
	        			 m.setAddress( "/WALL/PLAY/Chan" );
	        			 m.addIntArg(i);
	        			 sender.sendMessage( m );
	        			 
	        			 activChannel=i;
	        		} else {
	        			 ofxOscMessage m;
	        			 m.setAddress( "/WALL/STOP/Chan" );
	        			 m.addIntArg(i);
	        			 sender.sendMessage( m );
	        			 activChannel=-1;
	        		}
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