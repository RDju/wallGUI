#include "ofApp.h"

using namespace std;

//--------------------------------------------------------------
void ofApp::setup(){
	
	sender.setup( HOST, PORT );
	
	font.loadFont("OpenSans-Light.ttf",25);
	
	pageLevel=INTRO_PAGE;
	previousPageLevel=0;
	buttonNumber=-1;
	isButtonActioned = false;
	IDbuttonsCount = 0;
	IDmodulesCount = 0;
	
	gridSize.set( (float)(0.09*ofGetWidth()+WIDTH_BUTTONS), 0, (float)(ofGetWidth()-0.09*ofGetWidth()-WIDTH_BUTTONS), (float)(ofGetHeight()-0.15*ofGetHeight()-HEIGHT_BUTTONS));
	gridSetup();

	ofEnableAlphaBlending();
	
	//Buttons
	GUIbuttons.push_back(new Button("EXIT", IDbuttonsCount++, X_BUTTONS, ofGetHeight()-0.15*ofGetHeight() , -1, "EXIT", "EXIT"));
	
	for (int i=0; i<CHANNELSNUMBER; i++){
		GUIbuttons.push_back(new Button(channelsNames[i], IDbuttonsCount++, X_BUTTONS, Y_BUTTONS+i*(HEIGHT_BUTTONS+0.013*ofGetHeight()), CHANNELSELECT_PAGE, "STOP", "PLAY"));
	}
	
	GUIbuttons.push_back(new Button("CREATE WALL", IDbuttonsCount++, ofGetWidth()/2 - WIDTH_BUTTONS/2, ofGetHeight()/2 - HEIGHT_BUTTONS - 0.15*ofGetHeight() , MENU_PAGE, "CREATE WALL", "CREATE WALL"));
	GUIbuttons.push_back(new Button("CREATE CHANNEL", IDbuttonsCount++, ofGetWidth()/2 - WIDTH_BUTTONS/2, ofGetHeight()/2 , MENU_PAGE, "CREATE CHANNEL", "CREATE CHANNEL"));
	GUIbuttons.push_back(new Button("PLAY CHANNEL", IDbuttonsCount++, ofGetWidth()/2 - WIDTH_BUTTONS/2, ofGetHeight()/2 + HEIGHT_BUTTONS + 0.15*ofGetHeight(), MENU_PAGE, "PLAY CHANNEL", "PLAY CHANNEL"));
	
	GUIbuttons.push_back(new Button("RETURN", IDbuttonsCount++, 0.045*ofGetWidth() , ofGetHeight()-0.15*ofGetHeight(), -1, "RETURN", "RETURN"));
	
	GUIbuttons.push_back(new Button("screen13", IDbuttonsCount++, 0.045*ofGetWidth() , Y_BUTTONS, WALLCREATION_PAGE, "13' screen", "13' screen"));
	GUIbuttons.push_back(new Button("screen24", IDbuttonsCount++, 0.045*ofGetWidth() , Y_BUTTONS+HEIGHT_BUTTONS+0.013*ofGetHeight(), WALLCREATION_PAGE, "24' screen", "24' screen"));
	GUIbuttons.push_back(new Button("screen27", IDbuttonsCount++, 0.045*ofGetWidth() , Y_BUTTONS+2*(HEIGHT_BUTTONS+0.013*ofGetHeight()), WALLCREATION_PAGE, "27' screen", "27' screen"));
	
	
	
	//OSC message
	ofxOscMessage m;
	m.setAddress( "/INIT");
	sender.sendMessage( m );
}

void ofApp::gridSetup(){

	/*for (int i = 1; i <= 6;i++){//verticales
		gridLines[i-1].set(i*1.7*GRID_RATIO, 0.03*ofGetHeight(), i*1.7*GRID_RATIO, gridSize.getHeight()-0.03*ofGetHeight());
	}
	for (int i = 0; i < 7;i++){//horizontales
		gridLines[i+6].set(0.03*ofGetWidth(), (i+1)*GRID_RATIO, gridSize.getWidth() - 0.06*ofGetWidth(), (i+1)*GRID_RATIO);
	}*/
	
	int decalage = 0;
	for (int i = 0; i < 6;i++){
		if (i==2 || i==3) decalage = 1;
		if(i==4 || i==5) decalage = 2;
		gridLines[i].set((i+4)*1.7*GRID_RATIO-decalage, 0.03*ofGetHeight(), (i+4)*1.7*GRID_RATIO-decalage, gridSize.getHeight()-0.03*ofGetHeight());
	}
	for (int i = 0; i < 7;i++){
		gridLines[i+6].set(gridSize.getX()+0.03*ofGetWidth(), (i+1)*GRID_RATIO, ofGetWidth() - 0.03*ofGetWidth(), (i+1)*GRID_RATIO);
	}
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
    			pageLevel = WALLCREATION_PAGE;
    			break;
    		case 6:
    			pageLevel = CHANNELSELECT_PAGE;
    			break;
    		case 7://return
    			switch (pageLevel){
    				case WALLCREATION_PAGE:
    				case CHANNELSELECT_PAGE:
    					pageLevel = MENU_PAGE;
    					break;
    			}
    			break;
    		case 8:
    		case 9:
    		case 10:
    			if (GUImodules.size() < SCREEN_MAX_NUMBER)
    				GUImodules.push_back(new Module(buttonNumber - 7, IDmodulesCount++));
    			if (GUImodules.size() == SCREEN_MAX_NUMBER){
    				for (int i=8; i<11; i++)
    					GUIbuttons[i]->setIsAvailable(false);
    			}
    			break;
    	}
    }
    isButtonActioned = false;
    
    
    //TODO: réagencer GUImodule + ID des modules après avoir réfléchi à comment le faire
    for (size_t i = 0; i<GUImodules.size(); i++){
    	if (GUImodules[i]->getIsDeleted()){
    		GUImodules.erase(GUImodules.begin()+i);
    		for (size_t j=i; j<GUImodules.size(); j++){
    			GUImodules[j]->setID(GUImodules[j]->getID()-1);
    		}
    		
    		for (int i=8; i<11; i++)
    					GUIbuttons[i]->setIsAvailable(true);
    	}
    }
    
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
    
    switch (pageLevel){
    	case WALLCREATION_PAGE:
    	
    		drawGrid();
    		
    		ofPushStyle();
    		ofFill();
    		ofSetColor(10);
    		ofRect(0.03*ofGetWidth(), 0.03*ofGetWidth(), WIDTH_BUTTONS+ 0.03*ofGetWidth(), ofGetHeight() - 2 * 0.045*ofGetHeight());
    		ofPopStyle();
    		
    		//ofPushMatrix();
			//ofTranslate(gridSize.getX(), gridSize.getY());
    		for (size_t i = 0; i<GUImodules.size();i++){
    			GUImodules[i]->draw();
    		}
    		//ofPopMatrix();
    		break;
    }
    
    for (size_t i=0; i < GUIbuttons.size(); i++){
    	if (GUIbuttons[i]->getAssociatedPages() == pageLevel || GUIbuttons[i]->getAssociatedPages()==-1){
    		GUIbuttons[i]->draw();
    	}
    }
	previousPageLevel = pageLevel;
}

void ofApp::drawGrid(){
	/*for (int i = 0; i < (gridSize.getWidth()/1.7*GRID_RATIO);i++){
		ofLine(i*1.7*GRID_RATIO, 0.03*ofGetWidth(), i*1.7*GRID_RATIO, gridSize.getHeight()-0.03*ofGetHeight());
	}
	for (int i = 0; i < (gridSize.getHeight()/GRID_RATIO);i++){
		ofLine(gridSize.getX()+0.03*ofGetWidth(), i*GRID_RATIO, ofGetWidth() - 0.03*ofGetWidth(), i*GRID_RATIO);
	}*/
	
	for (int i=0; i<13; i++){
		/*ofPushMatrix();
		ofTranslate(gridSize.getX(), gridSize.getY());*/
		
		ofPushStyle();
    	ofSetColor(200);
    	ofLine(gridLines[i].getX(), gridLines[i].getY(), gridLines[i].getWidth(), gridLines[i].getHeight());
    	ofPopStyle();
    	//ofPopMatrix();
	}
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
	
	if (pageLevel == WALLCREATION_PAGE){
		for (size_t i=0; i < GUImodules.size(); i++){
			GUImodules[i]->onTouchDown(x, y);
		}
	}
}

//--------------------------------------------------------------
void ofApp::touchMoved(int x, int y, int id){
	if (pageLevel == WALLCREATION_PAGE){
		for (size_t i=0; i < GUImodules.size(); i++){
			GUImodules[i]->onTouchMove(x, y);
		}
	}
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
	
	if (pageLevel == WALLCREATION_PAGE){
		for (size_t i=0; i < GUImodules.size(); i++){
			GUImodules[i]->onTouchUp(x, y);
		}
	}

	//Touche de l'écran par page
	ofxOscMessage m;
	switch (pageLevel) {
		case INTRO_PAGE:
			pageLevel = MENU_PAGE;
			
	        m.setAddress( "/STOP");
	        sender.sendMessage( m );
	        
			break;
	}
}

//--------------------------------------------------------------
void ofApp::touchDoubleTap(int x, int y, int id){
	if (pageLevel == WALLCREATION_PAGE){
		for (size_t i=0; i < GUImodules.size(); i++){
			GUImodules[i]->onDoubleClick(x, y);
		}
	}
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