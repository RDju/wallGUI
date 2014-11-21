#include "ofApp.h"

using namespace std;

//--------------------------------------------------------------
void ofApp::setup(){
	
	sender.setup( HOST, PORT );
	
	font.loadFont("OpenSans-Light.ttf",25);
	background.loadImage("bckgrimg.png");
	
	for (int i=0; i<6;i++){
		demoChannelImages.push_back(new ofImage("channel"+ofToString(i)+".png"));
		demoChannelImages[i]->resize(1.0/3*ofGetHeight(), 1.0/3*ofGetHeight());
	}
	
	pageLevel=HOME_PAGE;
	previousPageLevel=0;
	buttonNumber=-1;
	isButtonActioned = false;
	IDbuttonsCount = 0;
	IDmodulesCount = 0;
	
	gridSize.set( (float)(0.09*ofGetWidth()+WIDTH_BUTTONS), 0, (float)(ofGetWidth()-0.09*ofGetWidth()-WIDTH_BUTTONS), (float)(ofGetHeight()-0.15*ofGetHeight()-HEIGHT_BUTTONS));
	gridSetup();

	ofEnableAlphaBlending();
	
	//Buttons
	/*GUIbuttons.push_back(new Button("EXIT", IDbuttonsCount++, X_BUTTONS, ofGetHeight()-0.15*ofGetHeight(), WIDTH_BUTTONS, HEIGHT_BUTTONS , -1, "EXIT", "EXIT"));
	
	for (int i=0; i<CHANNELSNUMBER; i++){
		GUIbuttons.push_back(new Button(channelsNames[i], IDbuttonsCount++, X_BUTTONS, Y_BUTTONS+i*(HEIGHT_BUTTONS+0.013*ofGetHeight()), WIDTH_BUTTONS, HEIGHT_BUTTONS, CHANNELSELECT_PAGE, "STOP", "PLAY"));
	}
	
	GUIbuttons.push_back(new Button("CREATE WALL", IDbuttonsCount++, ofGetWidth()/2 - WIDTH_BUTTONS/2, ofGetHeight()/2 - HEIGHT_BUTTONS - 0.15*ofGetHeight() , WIDTH_BUTTONS, HEIGHT_BUTTONS, HOME_PAGE, "CREATE WALL", "CREATE WALL"));
	GUIbuttons.push_back(new Button("CREATE CHANNEL", IDbuttonsCount++, ofGetWidth()/2 - WIDTH_BUTTONS/2, ofGetHeight()/2 , WIDTH_BUTTONS, HEIGHT_BUTTONS, HOME_PAGE, "CREATE CHANNEL", "CREATE CHANNEL"));
	GUIbuttons.push_back(new Button("PLAY CHANNEL", IDbuttonsCount++, ofGetWidth()/2 - WIDTH_BUTTONS/2, ofGetHeight()/2 + HEIGHT_BUTTONS + 0.15*ofGetHeight(), WIDTH_BUTTONS, HEIGHT_BUTTONS, HOME_PAGE, "PLAY CHANNEL", "PLAY CHANNEL"));
	
	GUIbuttons.push_back(new Button("RETURN", IDbuttonsCount++, 0.045*ofGetWidth() , ofGetHeight()-0.15*ofGetHeight(), WIDTH_BUTTONS, HEIGHT_BUTTONS, -1, "RETURN", "RETURN"));
	
	GUIbuttons.push_back(new Button("screen13", IDbuttonsCount++, 0.045*ofGetWidth() , Y_BUTTONS, WIDTH_BUTTONS, HEIGHT_BUTTONS, WALLCREATION_PAGE, "13' screen", "13' screen"));
	GUIbuttons.push_back(new Button("screen24", IDbuttonsCount++, 0.045*ofGetWidth() , Y_BUTTONS+HEIGHT_BUTTONS+0.013*ofGetHeight(), WIDTH_BUTTONS, HEIGHT_BUTTONS, WALLCREATION_PAGE, "24' screen", "24' screen"));
	GUIbuttons.push_back(new Button("screen27", IDbuttonsCount++, 0.045*ofGetWidth() , Y_BUTTONS+2*(HEIGHT_BUTTONS+0.013*ofGetHeight()), WIDTH_BUTTONS, HEIGHT_BUTTONS, WALLCREATION_PAGE, "27' screen", "27' screen"));
	*/
	
	GUIbuttons.push_back(new Button("HOME", HOME_PAGE, 0, 0, 1.0/5*ofGetWidth(), HEIGHT_BUTTONS , -1, "HOME", "HOME"));
	GUIbuttons.push_back(new Button("EXIT", 0, 8.0/15*ofGetWidth(), 0, 1.0/15*ofGetWidth(), HEIGHT_BUTTONS , -1, "EXIT", "EXIT"));
	GUIbuttons.push_back(new Button("SETTINGS", SETTINGS_PAGE, 3.0/5*ofGetWidth(), 0, 1.0/15*ofGetWidth(), HEIGHT_BUTTONS , -1, "/", "/"));
	GUIbuttons.push_back(new Button("MY CHANNELS", CHANNELSELECT_PAGE, 0, ofGetHeight()-HEIGHT_BUTTONS, 1.0/3*ofGetWidth(), HEIGHT_BUTTONS , -1, "My channels", "My channels"));
	GUIbuttons.push_back(new Button("Automix", AUTOMIX_PAGE, 1.0/3*ofGetWidth(), ofGetHeight()-HEIGHT_BUTTONS, 1.0/3*ofGetWidth(), HEIGHT_BUTTONS , -1, "# Automix", "# Automix"));
	GUIbuttons.push_back(new Button("Search", SEARCH_PAGE, 2.0/3*ofGetWidth(), ofGetHeight()-HEIGHT_BUTTONS, 1.0/3*ofGetWidth(), HEIGHT_BUTTONS , -1, "#/@ Search", "#/@ Search"));
	
	ofxGuiSetBackgroundColor(0);
	ofxGuiSetFont("OpenSans-Light.ttf",0.033*ofGetHeight(),true,true);
	ofxGuiSetTextPadding(20);
	ofxGuiSetDefaultWidth(1.0/3*ofGetWidth());
	ofxGuiSetDefaultHeight(HEIGHT_BUTTONS);
	gui.setup("WALLS"); 
	gui.add(wall1.set("Wall kitchen"));
	gui.add(wall2.set("Wall living room"));
	
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
	
	for(int i=0; i<7; i++){
		for (int j=0; j<7; j++){
			gridRepresentation[i][j] = 0;
		}
	}
}

//--------------------------------------------------------------
void ofApp::update() {
	
    //Actions liées aux clic sur des boutons
    if (isButtonActioned){
    	ofxOscMessage m;
    	switch(buttonNumber){
    		case 0:
    			m.setAddress( "/EXIT");
				sender.sendMessage( m );
	
				ofApp::exit();
				std::exit(EXIT_SUCCESS);
				
    			break;
    		case HOME_PAGE:
    		case WALLCREATION_PAGE:
    		case CHANNELSELECT_PAGE:
    		case AUTOMIX_PAGE:
    		case SEARCH_PAGE:
    		case SETTINGS_PAGE:
    			pageLevel=buttonNumber;
    			break;
    			
    			
    			
    			
    		/*case 0://Exit
    			
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
    					pageLevel = HOME_PAGE;
    					break;
    			}
    			break;
    		case 8:
    		case 9:
    		case 10:
    			if (GUImodules.size() < SCREEN_MAX_NUMBER){
    				GUImodules.push_back(new Module(buttonNumber - 7, IDmodulesCount++));
    				touchOrder.push_back(IDmodulesCount-1);
    			}
    			if (GUImodules.size() == SCREEN_MAX_NUMBER){
    				for (int i=8; i<11; i++)
    					GUIbuttons[i]->setIsAvailable(false);
    			}
    			break;*/
    	}
    }
    isButtonActioned = false;
    
    //suppression des modules
    for (size_t i = 0; i<GUImodules.size(); i++){
    	if (GUImodules[i]->getIsDeleted()){
    		GUImodules.erase(GUImodules.begin()+i);//delete module 
    		for (size_t j=i; j<GUImodules.size(); j++){//decrement the ID of module following the one just deleted
    			GUImodules[j]->setID(GUImodules[j]->getID()-1);
    		}
    		IDmodulesCount--;
    		for (size_t j=0; j<touchOrder.size();j++){//remove the ID from the deleted module from touchOrder
    			if (touchOrder[j]==i){
    				touchOrder.erase(touchOrder.begin() + j);
    				for (size_t k=0; k<touchOrder.size();k++)
    					if (touchOrder[k] > i) touchOrder[k]--;
    				break;
    			}
    		}
    		
    		for (int i=8; i<11; i++)
    			GUIbuttons[i]->setIsAvailable(true);
    	}
    }
    
    //Detect collision
    for (int i = 0; i< GUImodules.size(); i++){
    	GUImodules[i]->setIsWellPlaced(true);
    }
    for (size_t i = 0; i< GUImodules.size(); i++){
    	for (size_t j = 0; j< GUImodules.size(); j++){
    		if(j!=i && GUImodules[i]->isCollision(GUImodules[j]) && rankInTouchOrder(i)>rankInTouchOrder(j))
    			GUImodules[i]->setIsWellPlaced(false);
    	}
    }
    //Alone module
    if (GUImodules.size() > 1){
	    for (size_t i = 0; i< GUImodules.size(); i++){
	    	if (GUImodules[i]->getPos().y < gridSize.getHeight()){
		    	bool alone=true;
		    	for (size_t j = 0; j< GUImodules.size(); j++){
		    		if (i!=j && !GUImodules[i]->isAlone(GUImodules[j])){
		    			alone = false;
		    		}
		    	}
		    	if(alone) GUImodules[i]->setIsWellPlaced(false);
		   	}
	    }
	}
    
    //updateGridRepresentation();
    
    //On change de background si besoin
    /*ostringstream convert; 
	convert << pageLevel;
	if (pageLevel!=previousPageLevel ){
		strcpy (stringFile, (convert.str()).c_str());
	  	strcat (stringFile,".png");
	   
	    background.loadImage(stringFile);
    }*/
}

//Argument : ID of the module
//return : order rank 
int ofApp::rankInTouchOrder(int value){
	for (size_t i = 0; i<touchOrder.size();i++){
		if (touchOrder[i]==value) return i;
	}
	return -1;
}

//fill an arraw corresponding to the grid with . if the case is empty and # if there is a module in the case.
void ofApp::updateGridRepresentation(){
	for(int i=0; i<7; i++){
		for (int j=0; j<7; j++){
			gridRepresentation[i][j] = 0;
		}
	}
	for(int i=0; i<GUImodules.size();i++){
	
	
		//ofLogNotice() << "lol :" << GUImodules[i]->locationInGrid.x;
		//gridRepresentation[(int)(GUImodules[i]->locationInGrid.x)][(int)(GUImodules[i]->locationInGrid.y)] = 1;
		
		
		
		
		/*if (GUImodules[i]->getType()==2){
			gridRepresentation[(int)(GUImodules[i]->locationInGrid.x)][(int)(GUImodules[i]->locationInGrid.y)+1]=1;
			gridRepresentation[(int)(GUImodules[i]->locationInGrid.x)][(int)(GUImodules[i]->locationInGrid.y)+2]=1;
		} else if (GUImodules[i]->getType()==3){
			gridRepresentation[(int)(GUImodules[i]->locationInGrid.x)][(int)(GUImodules[i]->locationInGrid.y)+1]=1;
			gridRepresentation[(int)(GUImodules[i]->locationInGrid.x)+1][(int)(GUImodules[i]->locationInGrid.y)]=1;
			gridRepresentation[(int)(GUImodules[i]->locationInGrid.x)+1][(int)(GUImodules[i]->locationInGrid.y)+1]=1;
		}*/
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

    background.draw(0, 0);
    ofPushStyle();
	    ofSetColor(0);
	    ofFill();
	    ofRect(0, 0, ofGetWidth(), HEIGHT_BUTTONS);
	    ofRect(0, ofGetHeight()-HEIGHT_BUTTONS, ofGetWidth(), HEIGHT_BUTTONS);
	    
	    ofPushMatrix();
	    	ofTranslate(2.0/3*ofGetWidth()-10, -10);
	    	gui.draw();
	    ofPopMatrix();
	    ofSetColor(255);
	    ofNoFill();
	    ofRect(0, 0, ofGetWidth(), HEIGHT_BUTTONS);
	    ofRect(0, ofGetHeight()-HEIGHT_BUTTONS, ofGetWidth(), HEIGHT_BUTTONS);
    ofPopStyle();
    
    
    switch (pageLevel){
    	case HOME_PAGE:
    		
    		for(int i=0; i<6; i++){
    			ofPushMatrix();
    			
    			if (i%2==0){
    				demoChannelImages[i]->draw((i/2+1)*100+i/2*1.0/3*ofGetHeight(), 100);
    			} else {
    				demoChannelImages[i]->draw((i/2+1)*100+i/2*1.0/3*ofGetHeight(), 3.0/15*ofGetHeight()+1.0/3*ofGetHeight());
    			}
    			
    			ofPopMatrix();
    		}
    		
    		break;
    	case WALLCREATION_PAGE:
    	
    		drawGrid();
    		
    		ofPushStyle();
    		ofFill();
    		ofSetColor(10);
    		ofRect(0.03*ofGetWidth(), 0.03*ofGetWidth(), WIDTH_BUTTONS+ 0.03*ofGetWidth(), ofGetHeight() - 2 * 0.045*ofGetHeight());
    		ofPopStyle();
    		
    		for (size_t i = 0; i<GUImodules.size();i++){
    			GUImodules[touchOrder[i]]->draw();
    		}
    		break;
    }
    
    for (size_t i=0; i < GUIbuttons.size(); i++){
    	if (GUIbuttons[i]->getAssociatedPages() == pageLevel || GUIbuttons[i]->getAssociatedPages()==-1){
    		GUIbuttons[i]->draw();
    	}
    }
	previousPageLevel = pageLevel;
}
//--------------------------------------------------------------
void ofApp::drawGrid(){

	for (int i=0; i<13; i++){
		ofPushStyle();
    	ofSetColor(200);
    	ofLine(gridLines[i].getX(), gridLines[i].getY(), gridLines[i].getWidth(), gridLines[i].getHeight());
    	ofPopStyle();
	}
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
			GUImodules[touchOrder[i]]->onTouchDown(x, y);
			if (GUImodules[touchOrder[i]]->getIsTouched()){
				//for (int j=0; j<touchOrder.size();j++){
					//if (touchOrder[j]==i) {
						touchOrder.push_back(touchOrder[i]);
						touchOrder.erase(touchOrder.begin()+i);
						
						//break;
					//}
				//}
			}
		}
		for (size_t i=0; i < GUImodules.size(); i++){
			for (int j=0; j<GUImodules.size();j++){
					ofLogNotice() << rankInTouchOrder(j) << " " << rankInTouchOrder(i);
					if(i!=j && GUImodules[j]->getIsTouched() && rankInTouchOrder(j)<rankInTouchOrder(i)){
						ofLogNotice() << "you're not alone";
						GUImodules[i]->setIsTouched(false);
					}
			}
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

//check if a number is in the vector
/*bool ofApp::isInVector(vector<int> pagesVector, int value){
	for (size_t i=0; i<pagesVector.size(); i++){
		if (pagesVector[i] == value) return true;
	}
	return false;
}*/

//--------------------------------------------------------------

void ofApp::keyPressed(int key){}
void ofApp::keyReleased(int key){}
void ofApp::windowResized(int w, int h){}
void ofApp::touchCancelled(int x, int y, int id){}
void ofApp::swipe(ofxAndroidSwipeDir swipeDir, int id){}
void ofApp::pause(){}
void ofApp::stop(){}
void ofApp::resume(){}
void ofApp::reloadTextures(){}
bool ofApp::backPressed(){
	return false;
}
void ofApp::okPressed(){}
void ofApp::cancelPressed(){}