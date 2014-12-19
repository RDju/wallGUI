#include "ofApp.h"
//#include <jni.h>

using namespace std;

//--------------------------------------------------------------
void ofApp::setup(){

	background.loadImage("bckgrimgWhite.png");
	//background.loadImage("http://192.168.1.14:8000/pict1.jpg");
	ofEnableAlphaBlending();
	font.loadFont("OpenSans-Regular.ttf", 15);
	titleFont.loadFont("open-sansbold-italic.ttf", 20);
	
	pageLevel=HOME_PAGE;
	previousPageLevel = 0;
	slide = 0;
	buttonNumber=-1;
	isButtonActioned = false;
	IDbuttonsCount = 20;
	IDmyChannelsCount = 50;
	wantToSaveAutomix = false;

	//channelSelected = none;
	isTempChannelCreated = false;
	
	appWall = new Wall();
	appWall->setup();
	
	appMenu = new Menu();
	appMenu->setup(appWall->names);
	
	//Channels demo Home page
	string channelNames[] = {"Jambon", "Les oiseaux", "Claude François", "Abstrait", "Des voitures", "Ez3kiel", "Picasso", "Pantera", "Bisounours", "Lapin", "Cthulhu", "Coquillages"};
	for (int i=0; i<12;i++){
		demoChannels.push_back(new Channel("channel"+ofToString(i)+".png", channelNames[i], "user", rand()%5, i));
		demoChannels[i]->playButton->setID(IDbuttonsCount++);
		
	}
	string myChannelNames[] = {"Réseaux", "Vacances", "Bouffe"};
	for (int i=0; i<3;i++){
		myChannels.push_back(new Channel("channel"+ofToString(i)+".png", myChannelNames[i], "user", rand()%5, i+6));
		myChannels[i]->playButton->setID(IDmyChannelsCount++);
	}
	
	//OSC message
	ofxOscMessage m;
	m.setAddress( "/INIT");
	for (int i = 0; i < WALLNUMBER; i++){
	 	ofxOscSender tempSender;
	 	tempSender.setup(hosts[i], PORT);
		senders.push_back(tempSender);
		senders[i].setup(hosts[i], PORT);
		senders[i].sendMessage( m );
	}
	sender.setup(hosts[0], PORT);
	sender.sendMessage( m );
	receiver.setup( RECEIVEPORT );
	
	//Buttons
	//Channel display
	GUIbuttons.push_back(new Button("back", 10, 25, ofGetHeight() - HEIGHT_BUTTONS*2 - 25, 1.0/2*2.0/5*ofGetWidth()-10, HEIGHT_BUTTONS , CHANNELDISPLAY_PAGE, "BACK", "BACK", 40));
	GUIbuttons.push_back(new Button("play", 11, 25+1.0/2*2.0/5*ofGetWidth(), ofGetHeight() - HEIGHT_BUTTONS*2 - 25, 1.0/2*2.0/5*ofGetWidth()-10, HEIGHT_BUTTONS , CHANNELDISPLAY_PAGE, "PAUSE", "PLAY", 40));
	
	//Automix
	GUIbuttons.push_back(new Button("preview", 12, 25, ofGetHeight() - HEIGHT_BUTTONS*2 - 25, 1.0/2*2.0/5*ofGetWidth()-10, HEIGHT_BUTTONS , AUTOMIX_PAGE, "PREVIEW", "PREVIEW", 40));
	GUIbuttons.push_back(new Button("save", 13, 25+1.0/2*2.0/5*ofGetWidth(), ofGetHeight() - HEIGHT_BUTTONS*2 - 25, 1.0/2*2.0/5*ofGetWidth()-10, HEIGHT_BUTTONS , AUTOMIX_PAGE, "SAVE", "SAVE", 40));
	GUIbuttons.push_back(new Button("random", 14, 25+2*(1.0/2*2.0/5*ofGetWidth()), ofGetHeight() - HEIGHT_BUTTONS*2 - 25, 1.0/2*2.0/5*ofGetWidth()-10, HEIGHT_BUTTONS , AUTOMIX_PAGE, "RANDOM", "RANDOM", 40));
	GUIbuttons.push_back(new Button("play", 15, ofGetWidth()-25-(1.0/2*2.0/5*ofGetWidth()-10), ofGetHeight() - HEIGHT_BUTTONS*2 - 25, 1.0/2*2.0/5*ofGetWidth()-10, HEIGHT_BUTTONS , AUTOMIX_PAGE, "PLAY", "PLAY", 40));
	
	//Automix validation
	GUIbuttons.push_back(new Button("back", 16, 25, ofGetHeight() - HEIGHT_BUTTONS*2 - 25, 1.0/2*2.0/5*ofGetWidth()-10, HEIGHT_BUTTONS , AUTOMIX_PAGE, "BACK", "BACK", 40));
	
	//Save wall
	
	
	//Settings GUI
	ofxUIColor cb = ofxUIColor(0, 0, 0, 125); //Background 
	ofxUIColor cbgui = ofxUIColor(0, 0, 0, 0);
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
	
	
	string slidersName[] = {"imgVidRate", "transitionSpeed", "colorHarmony", "sensor", "none"};
	for (int i = 0; i < 5; i++){
		settingsSliders[i] = guiChannelSettings->addSlider(slidersName[i], 0.0, 100.0, 0.0);
	}
	settingsSliders[4]->setVisible(false);
	
	vector<string> moodsNames;
	moodsNames.push_back("None");
	moodsNames.push_back("Retro");
	moodsNames.push_back("Psychedelic");
	moodsNames.push_back("Sleep");

	moodList = guiChannelSettings->addDropDownList("Select your mood", moodsNames, 1.0/3*ofGetWidth(), 0, 20);
	
	moodList ->setAllowMultiple(false);
	moodList ->setShowCurrentSelected(true); 
	moodList->setAutoClose(true);
	
	ofAddListener(guiChannelSettings->newGUIEvent, this, &ofApp::moodEvent);
	
	guiChannelSettings->setUIColors( cb, cbgui, coh, cf, cfh, cp, cpo );
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
	
	if (appMenu->wallListAction == -1) {
		pageLevel = WALLCREATION_PAGE;
		appWall->modules.clear();
		appWall->touchOrder.clear();
		appWall->IDmodulesCount = 0;
	} else if (appMenu->wallListAction != -2 && pageLevel == WALLCREATION_PAGE){
		appWall->XML2Wall(wallSelected);
	}
	//else if (appMenu->wallListAction != -2) wallSelected = appMenu->wallListAction;
	appMenu->wallListAction = -2;
	
	if (wallSelected == appMenu->wallList->toggles.size()-1 && pageLevel != WALLCREATION_PAGE && appMenu->wallList->getSelected().size() > 0){
		appMenu->wallList->clearSelected();
	}
	
	if (pageLevel != CHANNELSELECT_PAGE){
		for (int i = 0; i < demoChannels.size(); i++){
			demoChannels[i]->playButton->setPos(-1, -1);
		}
	}
	
	if (pageLevel != AUTOMIX_PAGE && isTempChannelCreated){
		tempChannel->guiTitle->setVisible(false);
		tempChannel->guiDescriptionEdit->setVisible(false);
		isTempChannelCreated = false;
	}	
	
	if (appWall->newWallCreated){
		appWall->newWallCreated = false;
		appMenu->updateListWall(appWall->names);
		appMenu->wallList->setSelected((int)appWall->names.size()-2);
	}
	
	
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
    		case WALLCREATION_PAGE:
    			pageLevel=buttonNumber;
    			if (wallSelected != -1 && wallSelected != appMenu->wallList->toggles.size() -1)
    				appWall->XML2Wall(wallSelected);
    			break;
    		case HOME_PAGE:
    		case CHANNELSELECT_PAGE:
    		case SETTINGS_PAGE:
    			pageLevel=buttonNumber;
    			break;
    			
    		case AUTOMIX_PAGE:
    			m.setAddress("/AUTOMIX/TAG");
    			m.addStringArg(appMenu->automixTextInput->getTextString());
	        	sender.sendMessage( m );
	        	
    			pageLevel=buttonNumber;
    			
    			if (appMenu->automixTextInput->isClicked()) appMenu->automixTextInput->unClick();
    			if (isTempChannelCreated) tempChannel->tagsTextInput->setTextString(appMenu->automixTextInput->getTextString());
    			
    			if(!isTempChannelCreated){
	    			tempChannel = new Channel(appMenu->automixTextInput->getTextString());
	    			isTempChannelCreated = true;
	    		} else {
	    			tempChannel->tagsTextInput->setTextString(appMenu->automixTextInput->getTextString());
	    			tempChannel->tagsTextInput->setVisible(true);
	    		}
	    		
	    		//appMenu->getButtons()[AUTOMIX_PAGE]->setIsTouched(false);//pour revenir à l'état normal
    			break;
    			
    		case SEARCH_PAGE:
    			if (appMenu->searchTextInput->isClicked()) appMenu->searchTextInput->unClick();
    			pageLevel=CHANNELSELECT_PAGE;
    			break;
    			
    		case 10://Back button 
    			if(previousPageLevel == AUTOMIX_PAGE) pageLevel = HOME_PAGE;
    			else pageLevel=previousPageLevel;
    			break;
    		
    		case 11://play channel button
    			m.setAddress( "/PLAY/CHANNEL"/*+ofToString(channelSelected)*/);
    			m.addIntArg(channelSelected->ID);
	        	sender.sendMessage( m );
    			break;
    			
    		case 12: 
    			//preview
    			break;
    		case 13: //save
    			if (!wantToSaveAutomix)
    				wantToSaveAutomix=true;
    			else {
    				wantToSaveAutomix = false; 
    				
    				isTempChannelCreated = false;
    				tempChannel->guiTags->setVisible(false);
					tempChannel->guiDescriptionEdit->setVisible(false);
					tempChannel->guiTitle->setVisible(false);
					
					
					//save in myChannels
					tempChannel->ID = myChannels.size();
					tempChannel->saveChannel(IDmyChannelsCount++);
					myChannels.push_back(tempChannel);
					channelSelected = myChannels[myChannels.size()-1];
					
					previousPageLevel = pageLevel;
					pageLevel = CHANNELDISPLAY_PAGE;
					
					
					
    				//TODO: save xml
    			}
    			break;
    			
    		case 14:
    			//random
    			for (int i = 0; i < 5; i++){
    				settingsSliders[i]->setValue((rand()%4) * 25);
    			}
    			moodList->setSelected((rand()%(moodList->toggles.size()-1))+1);
    			break;
    			
    		case 15:
    			//play automix
    			m.setAddress( "/AUTOMIX/PLAY");
    			m.addStringArg(/*appMenu->automixTextInput->getTextString()*/tempChannel->tagsString);
    			for (int i = 0; i < 4; i++){
    				m.addIntArg((int32_t)settingsSliders[i]->getValue());
    			}
    			if (moodList->getSelectedIndeces().size() > 0)
    				m.addIntArg((int32_t)moodList->getSelectedIndeces()[0]);
    			else m.addIntArg(-1);
	        	sender.sendMessage( m );
    			break;
    		case 16: //back automix
    			wantToSaveAutomix = false;
    			break;
    	}
    	if ((buttonNumber >=20 && buttonNumber <=40) || (buttonNumber >=50) ) {
    		pageLevel = CHANNELDISPLAY_PAGE;
    	}
    }
    isButtonActioned = false;
    appMenu->update(wallSelected);
    
    
    switch(pageLevel){
    	case AUTOMIX_PAGE:
    		
    		if (appMenu->automixTextInput->isClicked())
    			tempChannel->tagsTextInput->setVisible(false);
    		if (!wantToSaveAutomix){
	    		guiChannelSettings->setVisible(true);
	    		
	    	} else {
	    		guiChannelSettings->setVisible(false);
	    	}
	    	
    		break;
    	case WALLCREATION_PAGE:
    		appWall->update();
    	case HOME_PAGE:
    		
    		if (slide == 1){
    			if(demoChannels[6]->tempPosition.x > 75 ){
	    			for (int i = 0; i < demoChannels.size(); i++){
	    				int tempX = demoChannels[i]->tempPosition.x;
	    				demoChannels[i]->tempPosition.x = tempX - 50;
	    			}
	    		} 
	    		if (demoChannels[6]->tempPosition.x < 75) slide = 2;
    		} else if (slide == 3) {
    			
    			if(demoChannels[0]->tempPosition.x < 75 ){
	    			for (int i = 0; i < demoChannels.size(); i++){
	    				int tempX = demoChannels[i]->tempPosition.x;
	    				demoChannels[i]->tempPosition.x = tempX + 50;
	    			}
	    		} 
	    		if(demoChannels[0]->tempPosition.x > 75 ) slide = 0;
    		}
    		
    		if(slide == 2){
    			for (int i = 0; i < 6; i++){
    				if (i%2==0) demoChannels[i+6]->tempPosition.set((i/2)/3.0*ofGetWidth()+75, 100);
					else demoChannels[i+6]->tempPosition.set((i/2)/3.0*ofGetWidth()+75, 3.0/15*ofGetHeight()+1.0/3*ofGetHeight());
    			}
    			
    		} else if (slide == 0){
    			for (int i = 0; i < demoChannels.size(); i++){
    				if (i%2==0) demoChannels[i]->tempPosition.set((i/2)/3.0*ofGetWidth()+50, 100);
					else demoChannels[i]->tempPosition.set((i/2)/3.0*ofGetWidth()+50, 3.0/15*ofGetHeight()+1.0/3*ofGetHeight());
    			}
    		}
    		break;
    	case CHANNELSELECT_PAGE:
    		for(int i=0; i<4; i++){
    			if (i%2==0) demoChannels[i]->tempPosition.set((i/2 +1)/3.0*ofGetWidth()+50, HEIGHT_BUTTONS + 50 +titleFont.getSize());
				else demoChannels[i]->tempPosition.set((i/2+1)/3.0*ofGetWidth()+50, HEIGHT_BUTTONS + 50 +titleFont.getSize() + CHANNEL_IMAGE_HEIGHT + 80);
			}
			for(int i=4; i<12; i++){
				demoChannels[i]->tempPosition.set(-1, -1);
			}
    		break;
    }    
	
}



//--------------------------------------------------------------
void ofApp::draw() {

    background.draw(0, 0);
    
    switch (pageLevel){
    	case HOME_PAGE:
    		//if (slide == 0){
	    		for(int i=0; i<12; i++){
	    			ofPushMatrix();
	    			ofPushStyle();
		    			//preview de channels
		    			
		    			demoChannels[i]->drawPreview();
		    			/*if (i%2==0) demoChannels[i]->drawPreview((i/2)/3.0*ofGetWidth()+50, 100);
						else demoChannels[i]->drawPreview((i/2)/3.0*ofGetWidth()+50, 3.0/15*ofGetHeight()+1.0/3*ofGetHeight());*/
						
						if (slide == 0){
							ofSetColor(255);
							ofFill();
							ofCircle(ofGetWidth()-40, ofGetHeight()/2, 30 );
							ofSetCircleResolution(3);
							ofSetColor(40);
							ofCircle(ofGetWidth()-40, ofGetHeight()/2, 25 );
						} else if (slide == 2) {
							ofSetColor(255);
							ofFill();
							ofCircle(40, ofGetHeight()/2, 30 );
							ofSetCircleResolution(3);
							ofSetColor(40);
							ofPushMatrix();
								ofRotate(180, 0, 0, 1);
								ofCircle(40, ofGetHeight()/2, 25 );
							ofPopMatrix();
						}
						
					ofPopStyle();
	    			ofPopMatrix();
	    		}
    		/*} else {
    			int boucle = 0;
    			while(demoChannels[6]->tempPosition.x > 25){
					for (int i = 0; i < demoChannels.size(); i ++ ){
						//demoChannels[i]->tempPosition.set();
						//demoChannels[i]->drawPreview(demoChannels[i]->tempPosition.x - 10, demoChannels[i]->tempPosition.y);
						
						if (i%2==0) demoChannels[i]->drawPreview((i/2)/3.0*ofGetWidth()+50 - i * 10, 100);
						else demoChannels[i]->drawPreview((i/2)/3.0*ofGetWidth()+50 - i * 10, 3.0/15*ofGetHeight()+1.0/3*ofGetHeight());
					}
					ofSleepMillis(10);
					boucle ++;
				}
			}*/
    		break;
    		
    	case CHANNELSELECT_PAGE:
    		//int spaceBetweenChannel = 20;
    		ofPushStyle();
    			ofSetColor(40);
    			ofLine(1.0/3*ofGetWidth()-20, HEIGHT_BUTTONS+20, 1.0/3*ofGetWidth()-20, ofGetHeight()-HEIGHT_BUTTONS-20);
    			titleFont.drawString("MY CHANNELS", 20, HEIGHT_BUTTONS + 20+titleFont.getSize());
    			for (int i = 0; i <myChannels.size(); i++){
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
    				demoChannels[i]->drawPreview();
    				/*if (i%2==0) demoChannels[i]->drawPreview((i/2 +1)/3.0*ofGetWidth()+50, HEIGHT_BUTTONS + 50 +titleFont.getSize());
					else demoChannels[i]->drawPreview((i/2+1)/3.0*ofGetWidth()+50, HEIGHT_BUTTONS + 50 +titleFont.getSize() + CHANNEL_IMAGE_HEIGHT + 80);*/
				}
				/*for(int i=4; i<12; i++){
					demoChannels[i]->tempPosition.set(-1, -1);
				}*/
				
				
    			
    			ofLine(1.0/3*ofGetWidth()+30+titleFont.stringWidth("SEARCH"), HEIGHT_BUTTONS +titleFont.getSize()+20, ofGetWidth()-20, HEIGHT_BUTTONS +titleFont.getSize()+20);
    			
    		ofPopStyle();
    	
    		break;
    		
    	case AUTOMIX_PAGE:
    		if (!wantToSaveAutomix){
	    		tempChannel->drawAutomixCreation(/*appMenu->automixTextInput->getTextString()*/);
	    		for (int i = 2; i < 6; i++){
	    			GUIbuttons[i]->draw();
	    		}
	    	} else {
	    		tempChannel->drawAutomixValidation();
	    		GUIbuttons[3]->draw();
	    		GUIbuttons[6]->draw();
	    	}
    		break;
    		
    	case WALLCREATION_PAGE:
    		
    		
    		if (!appWall->isLibraryOpened)
    			appWall->draw();
    		else appWall->drawLibrary();
    		
    		break;
    		
    	case CHANNELDISPLAY_PAGE:
    		channelSelected->drawPage();
    		GUIbuttons[0]->draw();
    		GUIbuttons[1]->draw();
    		break;
    		
    }
    
    /*for (size_t i=0; i < GUIbuttons.size(); i++){
    	if (GUIbuttons[i]->getAssociatedPages() == pageLevel || GUIbuttons[i]->getAssociatedPages()==-1){
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
	
		if(rm.getAddress() == "/SNAPSHOT"){
			tempChannel->automixImageFound = true;
			tempChannel->channelImage.loadImage(rm.getArgAsString(0));
			tempChannel->imageUrl = rm.getArgAsString(0);
			ofLogNotice() << "URL : " << rm.getArgAsString(0);
		}
	}	
}

//--------------------------------------------------------------
void ofApp::touchDown(int x, int y, int id){//On met le doigt

		// Regarde si des boutons ont été sélectionnés
		
		//dans le menu
		for (size_t i=0; i < appMenu->getButtons().size(); i++){
			if (appMenu->getButtons()[i]->isTouchedDown(x, y)){
				//On touchDown pour devancer l'annulation de l'activation de la zone de texte
				buttonNumber = appMenu->getButtons()[i]->getID();
				isButtonActioned = true;
				appMenu->getButtons()[AUTOMIX_PAGE]->isTouchedUp(x, y);//pour revenir à l'état normal
			}
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
			appWall->touchDown(x, y);
		} else if (pageLevel == HOME_PAGE){
			if (slide == 0){
				if (x > ofGetWidth() - 40 && y > ofGetHeight() / 2 - 15 && y < ofGetHeight() / 2 +15){
					slide = 1;
				}
			} else if (slide == 2){
				if (x < 50 && y > ofGetHeight() / 2 - 15 && y < ofGetHeight() / 2 +15){
					slide = 3;
				}
			}
		}
}

//--------------------------------------------------------------
void ofApp::touchMoved(int x, int y, int id){
	if (pageLevel == WALLCREATION_PAGE){
		appWall->touchMoved(x, y);
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
			if (pageLevel == CHANNELSELECT_PAGE && demoChannels[i]->playButton->isTouchedUp(x, y) && y > 50 && x > 0){
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
		
	if (!appMenu->wallList->isOpen() && !appMenu->automixTextInput->isClicked() && !appMenu->searchTextInput->isClicked()){
		
		
		if (pageLevel == HOME_PAGE || pageLevel == CHANNELSELECT_PAGE){//vérifie le clic sur un channel
			for (size_t i = 0; i < demoChannels.size(); i++){
				if (demoChannels[i]->isTouched(x, y)){
					channelSelected = demoChannels[i];
					previousPageLevel = pageLevel;
					pageLevel = CHANNELDISPLAY_PAGE;
				}
			}
		} else if (pageLevel == WALLCREATION_PAGE){//Vérifie si des modules sont touchés
			appWall->touchUp(x, y);
		}
	}
}

//--------------------------------------------------------------
void ofApp::touchDoubleTap(int x, int y, int id){
	/*if (pageLevel == WALLCREATION_PAGE){
		for (size_t i=0; i < GUImodules.size(); i++){
			GUImodules[i]->onDoubleClick(x, y);//Not used
		}
	}*/
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
	
	switch(key){
		case 10://return
			buttonNumber = AUTOMIX_PAGE;
			isButtonActioned = true;
			break;
	}
}

void ofApp::exit()
{
}


//--------------------------------------------------------------


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