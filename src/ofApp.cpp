#include "ofApp.h"
//#include <jni.h>

using namespace std;

//--------------------------------------------------------------
void ofApp::setup(){

	background.loadImage("bckgrimgWhite.png");
	ofEnableAlphaBlending();
	font.loadFont("OpenSans-Light.ttf", 15);
	titleFont.loadFont("open-sansbold-italic.ttf", 20);
	
	pageLevel=HOME_PAGE;
	previousPageLevel=0;
	buttonNumber=-1;
	isButtonActioned = false;
	IDbuttonsCount = 20;
	IDmodulesCount = 0;
	//channelSelected = none;
	isTempChannelCreated = false;
	
	
	appMenu = new Menu();
	appMenu->setup();
	
	//Channels demo Home page
	string channelNames[] = {"Jambon", "Les oiseaux", "Claude François", "Abstrait", "Des voitures", "Ez3kiel"};
	for (int i=0; i<6;i++){
		demoChannels.push_back(new Channel("channel"+ofToString(i)+".png", channelNames[i], "user", rand()%5, i));
		demoChannels[i]->playButton->setID(IDbuttonsCount++);
		
	}
	string myChannelNames[] = {"Réseaux", "Vacances", "Bouffe"};
	for (int i=0; i<3;i++){
		myChannels.push_back(new Channel("channel"+ofToString(i)+".png", myChannelNames[i], "user", rand()%5, i+6));
		myChannels[i]->playButton->setID(IDbuttonsCount++);
	}
	
	gridSize.set( (float)(0.09*ofGetWidth()+WIDTH_BUTTONS), 0, (float)(ofGetWidth()-0.09*ofGetWidth()-WIDTH_BUTTONS), (float)(ofGetHeight()-0.15*ofGetHeight()-HEIGHT_BUTTONS));
	gridSetup();
	
	//OSC message
	sender.setup( HOST, PORT );
	ofxOscMessage m;
	m.setAddress( "/INIT");
	sender.sendMessage( m );
	
	receiver.setup( PORT+1 );
	
	
	//Buttons
	//Channel display
	GUIbuttons.push_back(new Button("back", 10, 25, ofGetHeight() - HEIGHT_BUTTONS*2 - 25, 1.0/2*2.0/5*ofGetWidth()-10, HEIGHT_BUTTONS , CHANNELDISPLAY_PAGE, "BACK", "BACK", 40));
	GUIbuttons.push_back(new Button("play", 11, 25+1.0/2*2.0/5*ofGetWidth(), ofGetHeight() - HEIGHT_BUTTONS*2 - 25, 1.0/2*2.0/5*ofGetWidth()-10, HEIGHT_BUTTONS , CHANNELDISPLAY_PAGE, "PAUSE", "PLAY", 40));
	
	//Automix
	GUIbuttons.push_back(new Button("preview", 12, 25, ofGetHeight() - HEIGHT_BUTTONS*2 - 25, 1.0/2*2.0/5*ofGetWidth()-10, HEIGHT_BUTTONS , AUTOMIX_PAGE, "PREVIEW", "PREVIEW", 40));
	GUIbuttons.push_back(new Button("save", 13, 25+1.0/2*2.0/5*ofGetWidth(), ofGetHeight() - HEIGHT_BUTTONS*2 - 25, 1.0/2*2.0/5*ofGetWidth()-10, HEIGHT_BUTTONS , AUTOMIX_PAGE, "SAVE", "SAVE", 40));
	GUIbuttons.push_back(new Button("random", 14, 25+2*(1.0/2*2.0/5*ofGetWidth()), ofGetHeight() - HEIGHT_BUTTONS*2 - 25, 1.0/2*2.0/5*ofGetWidth()-10, HEIGHT_BUTTONS , AUTOMIX_PAGE, "RANDOM", "RANDOM", 40));
	GUIbuttons.push_back(new Button("play", 15, ofGetWidth()-25-(1.0/2*2.0/5*ofGetWidth()-10), ofGetHeight() - HEIGHT_BUTTONS*2 - 25, 1.0/2*2.0/5*ofGetWidth()-10, HEIGHT_BUTTONS , AUTOMIX_PAGE, "PLAY", "PLAY", 40));
	
	//Settings GUI
	ofxUIColor cb = ofxUIColor(0, 0, 0, 125); //Background 
	ofxUIColor cb2 = ofxUIColor(40, 40, 40, 150); //BG liste déroulante
	ofxUIColor co = ofxUIColor(255, 0, 0, 255); // ???
	ofxUIColor coh = ofxUIColor(40, 40, 40, 255); //tour quand sélectionné
	ofxUIColor cf = ofxUIColor(255, 255, 255, 255); //texte non sélectionné
	ofxUIColor cfh = ofxUIColor(255, 255, 255, 255); //texte sélectionné + curseur
	ofxUIColor cp = ofxUIColor(255, 255, 255, 255); // ???
	ofxUIColor cpo = ofxUIColor(255, 0, 0, 255);
		    
	guiChannelSettings = new ofxUICanvas(50 + 2.0/5*ofGetWidth() + font.stringWidth("Sensor Sensitivity")+20, HEIGHT_BUTTONS+15+titleFont.getSize(), 3.0/5*ofGetWidth()-60-font.stringWidth("Sensor Sensitivity")-20, ofGetHeight() - 2*HEIGHT_BUTTONS+40);
	guiChannelSettings->setWidgetSpacing(30);
	
	guiChannelSettings->setFont("OpenSans-Light.ttf");
	guiChannelSettings->setWidgetFontSize(OFX_UI_FONT_LARGE);
	
	string slidersName[] = {"imgVidRate", "transitionSpeed", "colorHarmony", "sensorInput", "sensorSensitivity"};
	for (int i = 0; i < 5; i++){
		settingsSliders[i] = guiChannelSettings->addSlider(slidersName[i], 0.0, 100.0, 0.0);
	}
	
	vector<string> moodsNames;
	moodsNames.push_back("None");
	moodsNames.push_back("Retro");
	moodsNames.push_back("Psychedelic");
	moodsNames.push_back("Sleep");

	moodList = guiChannelSettings->addDropDownList("Select your mood", moodsNames, 1.0/4*ofGetWidth(), 0, 20);
	
	moodList ->setAllowMultiple(false);
	moodList ->setShowCurrentSelected(true); 
	moodList->setAutoClose(true);
	
	ofAddListener(guiChannelSettings->newGUIEvent, this, &ofApp::moodEvent);
	guiChannelSettings->setUIColors( cb, co, coh, cf, cfh, cp, cpo );
	guiChannelSettings->setVisible(false);
}

void ofApp::moodEvent(ofxUIEventArgs &e){
	string name = e.widget->getName();
	if(name == "Select your mood")
	{
		ofxUIDropDownList *ddlist = (ofxUIDropDownList *) e.widget;
		vector<ofxUIWidget *> &selected = ddlist->getSelected(); 
	}
}

//--------------------------------------------------------------
void ofApp::update() {

	appMenu->update(wallSelected);
	
	OSCcatch();
	
	//On cache le canevas de la description des channels
	if (pageLevel!=CHANNELDISPLAY_PAGE){
    	for (size_t i=0; i < demoChannels.size(); i++)
    		if (demoChannels[i]->guiDescription->isVisible()) 
    				demoChannels[i]->guiDescription->setVisible(false);
    	for (size_t i=0; i < myChannels.size(); i++)
    		if (myChannels[i]->guiDescription->isVisible()) 
    				myChannels[i]->guiDescription->setVisible(false);
    }
    if (pageLevel != AUTOMIX_PAGE) {
    	guiChannelSettings->setVisible(false);
    	isTempChannelCreated = false;
    }
	
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
    		case SETTINGS_PAGE:
    			pageLevel=buttonNumber;
    			break;
    			
    		case AUTOMIX_PAGE:
    			if (appMenu->automixTextInput->isClicked()) appMenu->automixTextInput->unClick();
    			
    			m.setAddress("/AUTOMIX/TAG");
    			m.addStringArg(appMenu->automixTextInput->getTextString());
	        	sender.sendMessage( m );
	        	
    			pageLevel=buttonNumber;
    			break;
    			
    		case SEARCH_PAGE:
    			if (appMenu->searchTextInput->isClicked()) appMenu->searchTextInput->unClick();
    			pageLevel=CHANNELSELECT_PAGE;
    			break;
    			
    		case 10://Back button 
    			pageLevel=previousPageLevel;
    			break;
    		
    		case 11://play channel button
    			m.setAddress( "/PLAY/CHANNEL"/*+ofToString(channelSelected)*/);
    			m.addIntArg(channelSelected->getID());
	        	sender.sendMessage( m );
    			break;
    			
    		case 12: 
    			//preview
    			break;
    		case 13:
    			//save
    			break;
    			
    		case 14:
    			//random
    			for (int i = 0; i < 5; i++){
    				settingsSliders[i]->setValue((rand()%4) * 25);
    			}
    			break;
    			
    		case 15:
    			//play automix
    			m.setAddress( "/AUTOMIX/PLAY");
    			m.addStringArg(appMenu->automixTextInput->getTextString());
    			for (int i = 0; i < 5; i++)
    				m.addIntArg(settingsSliders[i]);
	        	sender.sendMessage( m );
    			break;
    			
    		/*case 10:
    			if (GUImodules.size() < SCREEN_MAX_NUMBER){
    				GUImodules.push_back(new Module(buttonNumber - 7, IDmodulesCount++));
    				touchOrder.push_back(IDmodulesCount-1);
    			}
    			if (GUImodules.size() == SCREEN_MAX_NUMBER){
    				for (int i=8; i<11; i++)
    					GUIbuttons[i]->setIsAvailable(false);
    			}infoFont.getSize()
    			break;*/
    			
    	}
    	if (buttonNumber >=20 && buttonNumber <=30) {
    		pageLevel = CHANNELDISPLAY_PAGE;
    	}
    }
    isButtonActioned = false;
    appMenu->update(wallSelected);
    
    
    switch(pageLevel){
    	case CHANNELSELECT_PAGE:
    		//set searchGui visible
    		//if menu.searchText != Search, set searchText=menu.searchText
    		break;
    	case AUTOMIX_PAGE:
    		guiChannelSettings->setVisible(true);
    		if(!isTempChannelCreated){
    			tempChannel = new Channel();
    			isTempChannelCreated = true;
    		}
    		break;
    
    }
    
    
    //********************************** WALL CREATION ***********************************************//
    //suppression des modules
    for (size_t i = 0; i<GUImodules.size(); i++){
    	if (GUImodules[i]->getIsDeleted()){
    		GUImodules.erase(GUImodules.begin()+i);//delete module 
    		for (size_t j=i; j<GUImodules.size(); j++){//decrement the ID of module following the one just deleted
    			GUImodules[j]->setID(GUImodules[j]->getID()-1);
    		}https://mail.google.com/mail/u/0/#inbox
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
	
	
	//************************************************************************//
    
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



//--------------------------------------------------------------
void ofApp::draw() {

    background.draw(0, 0);
    
    switch (pageLevel){
    	case HOME_PAGE:
    		for(int i=0; i<6; i++){
    			ofPushMatrix();
	    			//preview de channels
	    			if (i%2==0) demoChannels[i]->drawPreview((i/2)/3.0*ofGetWidth()+50, 100);
					else demoChannels[i]->drawPreview((i/2)/3.0*ofGetWidth()+50, 3.0/15*ofGetHeight()+1.0/3*ofGetHeight());
    			ofPopMatrix();
    		}
    		break;
    		
    	case CHANNELSELECT_PAGE:
    		//int spaceBetweenChannel = 20;
    		ofPushStyle();
    			ofSetColor(40);
    			ofLine(1.0/3*ofGetWidth()-20, HEIGHT_BUTTONS+20, 1.0/3*ofGetWidth()-20, ofGetHeight()-HEIGHT_BUTTONS-20);
    			titleFont.drawString("MY CHANNELS", 20, HEIGHT_BUTTONS + 20+titleFont.getSize());
    			for (int i = 0; i <3; i++){
    				myChannels[i]->drawMini(20, HEIGHT_BUTTONS + 20+titleFont.getSize()+ 30 + i*(font.getSize()+30));
    			}
    			titleFont.drawString("BOOKMARKS", 20 , ofGetHeight()/2+titleFont.getSize());
    			for (int i = 0; i < 4; i++){
    				demoChannels[i+2]->drawMini(20, ofGetHeight()/2+titleFont.getSize()+ 30 + i*(font.getSize()+30));
    			}
    			titleFont.drawString("SEARCH", 1.0/3*ofGetWidth()+20 , HEIGHT_BUTTONS + 20+titleFont.getSize());
    			
    			if ( appMenu->searchTextInput->getTextString()!= "Search"){// !(strcmp(appMenu->searchTextInput->getTextString(), "Search") == 0)){
    				font.drawString(appMenu->searchTextInput->getTextString(), 1.0/3*ofGetWidth()+40+titleFont.stringWidth("SEARCH"), HEIGHT_BUTTONS + 25+font.getSize());
    			}
    			
    			for(int i=0; i<4; i++){
    				if (i%2==0) demoChannels[i]->drawPreview((i/2 +1)/3.0*ofGetWidth()+50, HEIGHT_BUTTONS + 50 +titleFont.getSize());
					else demoChannels[i]->drawPreview((i/2+1)/3.0*ofGetWidth()+50, HEIGHT_BUTTONS + 50 +titleFont.getSize() + CHANNEL_IMAGE_HEIGHT + 80);
				}
				for(int i=4; i<6; i++){
					demoChannels[i]->tempPosition.set(-1, -1);
				}
				
				
    			
    			ofLine(1.0/3*ofGetWidth()+30+titleFont.stringWidth("SEARCH"), HEIGHT_BUTTONS +titleFont.getSize()+20, ofGetWidth()-20, HEIGHT_BUTTONS +titleFont.getSize()+20);
    			
    		ofPopStyle();
    	
    		break;
    		
    	case AUTOMIX_PAGE:
    		tempChannel->drawAutomixCreation(appMenu->automixTextInput->getTextString());
    		for (int i = 2; i < 6; i++){
    			GUIbuttons[i]->draw();
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
    		
    	case CHANNELDISPLAY_PAGE:
    		channelSelected->drawPage();
    		GUIbuttons[0]->draw();
    		GUIbuttons[1]->draw();
    		break;
    		
    }
    
    /*for (size_t i=0; i < menuButtons.size(); i++){
    	if (menuButtons[i]->getAssociatedPages() == pageLevel || GUIbuttons[i]->getAssociatedPages()==-1){
    		GUIbuttons[i]->draw();
    	}
    }*/
    
    appMenu->draw();
}

//--------------------------------------------------------------

void ofApp::OSCcatch(){

	ofxOscMessage rm; 
	if (isTempChannelCreated && receiver.hasWaitingMessages()){
		rm.clear();
		receiver.getNextMessage(&rm);
	
		if(rm.getAddress() == "/AUTOMIX/IMAGE"){
			tempChannel->automixImageFound = true;
			tempChannel->channelImage.loadImage(rm.getArgAsString(0));
		}
	}	
}

//--------------------------------------------------------------
void ofApp::touchDown(int x, int y, int id){//On met le doigt

	// Regarde si des boutons ont été sélectionnés
	
	//dans le menu
	for (size_t i=0; i < appMenu->getButtons().size(); i++){
		appMenu->getButtons()[i]->isTouchedDown(x, y);
	}
	
	//Channel buttons
	for (int i=0; i < demoChannels.size(); i++){
		demoChannels[i]->playButton->isTouchedDown(x, y);
	}
	
	for (int i=0; i < myChannels.size(); i++){
		myChannels[i]->playButton->isTouchedDown(x, y);
	}
	
	//globaux
	for (size_t i=0; i < GUIbuttons.size(); i++){
		//if(isInVector(GUIbuttons[i]->getAssociatedPages(), pageLevel)){
		if (GUIbuttons[i]->getAssociatedPages() == pageLevel || GUIbuttons[i]->getAssociatedPages()==-1){
			GUIbuttons[i]->isTouchedDown(x, y);
		}
	}
	
	if (pageLevel == WALLCREATION_PAGE){
		//Trie les modules par ordre de dernière sélection
		for (size_t i=0; i < GUImodules.size(); i++){
			GUImodules[touchOrder[i]]->onTouchDown(x, y);
			if (GUImodules[touchOrder[i]]->getIsTouched()){
				touchOrder.push_back(touchOrder[i]);
				touchOrder.erase(touchOrder.begin()+i);
			}
		}
		//Ne laisse sélectionné que celui du dessus en cas de 2 modules supperposés
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
	
	//Menu buttons
	for (size_t i=0; i < appMenu->getButtons().size(); i++){
		if (appMenu->getButtons()[i]->isTouchedUp(x, y)){
			buttonNumber = appMenu->getButtons()[i]->getID();
			isButtonActioned = true;
			continue;
		}
	}
	
	//Channel buttons
	for (int i=0; i < demoChannels.size(); i++){
		if (demoChannels[i]->playButton->isTouchedUp(x, y)){
			buttonNumber = demoChannels[i]->playButton->getID();
			channelSelected = demoChannels[i];
			isButtonActioned = true;
			previousPageLevel = pageLevel;
			continue;
		}
	}
	
	for (int i=0; i < myChannels.size(); i++){
		if (myChannels[i]->playButton->isTouchedUp(x, y)){
			buttonNumber = myChannels[i]->playButton->getID();
			channelSelected = myChannels[i];
			isButtonActioned = true;
			previousPageLevel = pageLevel;
			continue;
		}
	}
			
	//global buttons
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
	
	
	if (pageLevel == WALLCREATION_PAGE){//Vérifie si des modules sont touchés
		for (size_t i=0; i < GUImodules.size(); i++){
			GUImodules[i]->onTouchUp(x, y);
		}
	} else if (pageLevel == HOME_PAGE || pageLevel == CHANNELSELECT_PAGE){//vérifie le clic sur un channel
		for (size_t i = 0; i < demoChannels.size(); i++){
			if (demoChannels[i]->isTouched(x, y)){
				channelSelected = demoChannels[i];
				previousPageLevel = pageLevel;
				pageLevel = CHANNELDISPLAY_PAGE;
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::touchDoubleTap(int x, int y, int id){
	if (pageLevel == WALLCREATION_PAGE){
		for (size_t i=0; i < GUImodules.size(); i++){
			GUImodules[i]->onDoubleClick(x, y);//Not used
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

void ofApp::keyPressed(int key){
	
}

void ofApp::exit()
{
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
void ofApp::drawGrid(){

	for (int i=0; i<13; i++){
		ofPushStyle();
    	ofSetColor(200);
    	ofLine(gridLines[i].getX(), gridLines[i].getY(), gridLines[i].getWidth(), gridLines[i].getHeight());
    	ofPopStyle();
	}
}



//Argument : ID of the module
//return : order rank 
int ofApp::rankInTouchOrder(int value){
	for (size_t i = 0; i<touchOrder.size();i++){
		if (touchOrder[i]==value) return i;
	}
	return -1;
}

//fill an array corresponding to the grid with . if the case is empty and # if there is a module in the case.
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