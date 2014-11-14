#include "ofApp.h"

using namespace std;

//--------------------------------------------------------------
void ofApp::setup(){
	
	sender.setup( HOST, PORT );
	
	font.loadFont("OpenSans-Light.ttf",25);
	
	pageLevel=1;
	previousPageLevel=0;
	buttonNumber=-1;
	isButtonActioned = false;
	IDcount = 0;

	ofEnableAlphaBlending();
	
	//Buttons
	GUIbuttons.push_back(new Button("EXIT", IDcount++, X_BUTTONS, ofGetHeight()-0.15*ofGetHeight() , -1, "EXIT", "EXIT"));
	
	for (int i=0; i<CHANNELSNUMBER; i++){
		GUIbuttons.push_back(new Button(channelsNames[i], IDcount++, X_BUTTONS, Y_BUTTONS+i*(HEIGHT_BUTTONS+0.013*ofGetHeight()), 7, "STOP", "PLAY"));
	}
	
	GUIbuttons.push_back(new Button("CREATE WALL", IDcount++, ofGetWidth()/2 - WIDTH_BUTTONS/2, ofGetHeight()/2 - HEIGHT_BUTTONS - 0.15*ofGetHeight() , 2, "CREATE WALL", "CREATE WALL"));
	GUIbuttons.push_back(new Button("CREATE CHANNEL", IDcount++, ofGetWidth()/2 - WIDTH_BUTTONS/2, ofGetHeight()/2 , 2, "CREATE CHANNEL", "CREATE CHANNEL"));
	GUIbuttons.push_back(new Button("PLAY CHANNEL", IDcount++, ofGetWidth()/2 - WIDTH_BUTTONS/2, ofGetHeight()/2 + HEIGHT_BUTTONS + 0.15*ofGetHeight(), 2, "PLAY CHANNEL", "PLAY CHANNEL"));
	
	GUIbuttons.push_back(new Button("RETURN", IDcount++, 0.045*ofGetWidth() , ofGetHeight()-0.15*ofGetHeight(), -1, "RETURN", "RETURN"));
	
	
	//OSC message
	ofxOscMessage m;
	m.setAddress( "/INIT");
	sender.sendMessage( m );
}

//--------------------------------------------------------------
void ofApp::update() {
	
    //Actions liées aux clic sur des boutons
    if (isButtonActioned){
    	ofxOscMessage m;
    	switch(buttonNumber){
    		case 0://Exit
    			
				m.setAddress( "/EXIT");
				sender.sendMessage( m );
	
				ofApp::exit();
				std::exit(EXIT_SUCCESS);
				
    			break;
    		case 1:
    		case 2:
    		case 3:
    			if (GUIbuttons[buttonNumber]->getIsActiv()){
	        		m.setAddress( "/PLAY/Chan"+ofToString(buttonNumber-1));
	        		sender.sendMessage( m );
	        	} else {
	        		m.setAddress( "/STOP" );
	        		sender.sendMessage( m );
	        	}
    			break;
    		case 4:
    			pageLevel = 3;
    			break;
    		case 6:
    			pageLevel = 7;
    			break;
    		case 7:
    			switch (pageLevel){
    				case 3:
    				case 7:
    					pageLevel = 2;
    					break;
    			}
    			break;
    	}
    }
    isButtonActioned = false;
    
    
    //On change de page si besoin
    ostringstream convert; 
	convert << pageLevel;  
	if (pageLevel!=previousPageLevel ){
		strcpy (stringFile, (convert.str()).c_str());
	  	strcat (stringFile,".png");
	   
	    background.loadImage(stringFile);
    }
}

//--------------------------------------------------------------
void ofApp::draw() {

    background.draw(0, 0);
    
    for (size_t i=0; i < GUIbuttons.size(); i++){
    	if (GUIbuttons[i]->getAssociatedPages() == pageLevel || GUIbuttons[i]->getAssociatedPages()==-1){
    		GUIbuttons[i]->draw();
    	}
    }
	previousPageLevel = pageLevel;
}

/*bool ofApp::isInVector(vector<int> pagesVector, int value){
	for (size_t i=0; i<pagesVector.size(); i++){
		if (pagesVector[i] == value) return true;
	}
	return false;
}*/

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
void ofApp::touchDown(int x, int y, int id){//On met le doigt

	for (size_t i=0; i < GUIbuttons.size(); i++){
		//if(isInVector(GUIbuttons[i]->getAssociatedPages(), pageLevel)){
		if (GUIbuttons[i]->getAssociatedPages() == pageLevel || GUIbuttons[i]->getAssociatedPages()==-1){
			GUIbuttons[i]->isTouchedDown(x, y);
		}
	}
}

//--------------------------------------------------------------
void ofApp::touchMoved(int x, int y, int id){
	
}

//--------------------------------------------------------------
void ofApp::touchUp(int x, int y, int id){//On enlève le doigt

	//Regarde si un bouton a été actionné et récupère l'ID si c'est le cas
	for (size_t i=0; i < GUIbuttons.size(); i++){
		if ((GUIbuttons[i]->getAssociatedPages() == pageLevel || GUIbuttons[i]->getAssociatedPages()==-1 )&& GUIbuttons[i]->isTouchedUp(x, y)){
			buttonNumber = GUIbuttons[i]->getID();
			isButtonActioned = true;
			continue;
		}
	}
	//Rend inactif tous les boutons autre que le nouvel actif
	if (isButtonActioned){
		for(size_t i=0; i < GUIbuttons.size(); i++){
			if (buttonNumber != GUIbuttons[i]->getID()){
				GUIbuttons[i]->setIsActiv(false);
			}
		}
	}

	//Touche de l'écran par page
	ofxOscMessage m;
	switch (pageLevel) {
		case 1:
			pageLevel = 2;
			
	        m.setAddress( "/STOP");
	        sender.sendMessage( m );
	        
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