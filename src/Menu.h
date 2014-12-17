#include <vector>
#include <string>
#include "ofxUI.h"
#include "ofApp.h"

#define HOME_PAGE 1
#define WALLCREATION_PAGE 2
#define CHANNELSELECT_PAGE 3
#define AUTOMIX_PAGE 4
#define SEARCH_PAGE 5
#define SETTINGS_PAGE 6

class Menu{
	public:
	
		Menu(){};
		void setup(){
			//Buttons
			menuButtons.push_back(new Button("HOME", HOME_PAGE, 0, 0, 1.0/4*ofGetWidth(), HEIGHT_BUTTONS , -1, "HOME", "HOME"));
			menuButtons.push_back(new Button("LOG OUT", 0,  15.0/28*ofGetWidth(), 0, 3.0/28*ofGetWidth(), HEIGHT_BUTTONS , -1, "LOG OUT", "LOG OUT"));
			menuButtons.push_back(new Button("SETTINGS", SETTINGS_PAGE, 18.0/28*ofGetWidth(), 0, 3.0/28*ofGetWidth(), HEIGHT_BUTTONS , -1, "SETTINGS", "SETTINGS"));
			
			menuButtons.push_back(new Button("MY CHANNELS", CHANNELSELECT_PAGE, 0, ofGetHeight()-HEIGHT_BUTTONS, 1.0/4*ofGetWidth(), HEIGHT_BUTTONS , -1, "My channels", "My channels"));
			//GUIbuttons.push_back(new Button("Automix", AUTOMIX_PAGE, 1.0/4*ofGetWidth(), ofGetHeight()-HEIGHT_BUTTONS, 3.0/8*ofGetWidth(), HEIGHT_BUTTONS , -1, "What @ or # is on your mind ?", "What @ or # is on your mind ?"));
			//GUIbuttons.push_back(new Button("Search", SEARCH_PAGE, 5.0/8*ofGetWidth(), ofGetHeight()-HEIGHT_BUTTONS, 3.0/8*ofGetWidth(), HEIGHT_BUTTONS , -1, "Search a # or a @", "Search a # or a @"));
			
			menuButtons.push_back(new Button("Automix go", AUTOMIX_PAGE, 9.0/16*ofGetWidth(), ofGetHeight()-HEIGHT_BUTTONS+10, 1.0/16*ofGetWidth()-10, HEIGHT_BUTTONS-20 , -1, "GO", "GO"));
			menuButtons.push_back(new Button("Search go", SEARCH_PAGE, 15.0/16*ofGetWidth(), ofGetHeight()-HEIGHT_BUTTONS+10, 1.0/16*ofGetWidth()-10, HEIGHT_BUTTONS-20 , -1, "GO", "GO"));
			
			//zones de texte
			
			ofxUIColor cb = ofxUIColor(0, 0, 0, 0); //Background 
			ofxUIColor cb2 = ofxUIColor(40, 40, 40, 200); //BG liste déroulante
		    ofxUIColor co = ofxUIColor(255, 0, 0, 255); // ???
		    ofxUIColor coh = ofxUIColor(255, 255, 255, 255); //tour quand sélectionné
		    ofxUIColor cf = ofxUIColor(255, 255, 255, 255); //texte non sélectionné
		    ofxUIColor cfh = ofxUIColor(255, 255, 255, 255); //texte sélectionné + curseur
		    ofxUIColor cp = ofxUIColor(255, 255, 255, 255); // ???
		    ofxUIColor cpo = ofxUIColor(255, 0, 0, 255);
		    
			
			guiAuto = new ofxUICanvas( 1.0/4*ofGetWidth()+10, ofGetHeight()-HEIGHT_BUTTONS*3.0/4, 2.0/8*ofGetWidth()-20, HEIGHT_BUTTONS);
			guiAuto->setFont("OpenSans-Light.ttf");
			guiAuto->setWidgetFontSize(OFX_UI_FONT_LARGE);
			guiAuto->setUIColors( cb, cb, coh, cf, cfh, cp, cpo );
			automixTextInput = guiAuto->addTextInput("Automix", "Automix", -1); 
			automixTextInput->setAutoUnfocus(true);
			automixTextInput->setAutoClear(true);
			
			//automixGui->setPosition(x,y);
			//automixTextInput->setAutoClear(true);
			
			guiSearch = new ofxUICanvas(5.0/8*ofGetWidth()+10, ofGetHeight()-HEIGHT_BUTTONS*3.0/4, 2.0/8*ofGetWidth()-20, HEIGHT_BUTTONS);
			guiSearch->setFont("OpenSans-Light.ttf");
			guiSearch->setWidgetFontSize(OFX_UI_FONT_LARGE);
			guiSearch->setUIColors( cb, cb, coh, cf, cfh, cp, cpo );
			searchTextInput = guiSearch->addTextInput("Search", "Search", -1);
			//searchTextInput->setAutoClear(true);
			
			//Menu déroulant
			guiMenuDeroulant = new ofxUICanvas(3.0/4*ofGetWidth()-5, HEIGHT_BUTTONS/2 - 15, 1.0/4*ofGetWidth(), HEIGHT_BUTTONS);
			guiMenuDeroulant->setFont("OpenSans-Light.ttf");
			guiMenuDeroulant->setWidgetFontSize(OFX_UI_FONT_LARGE);
			
			//guiMenuDeroulant->setDrawPadding(50);
			
		    vector<string> names;
		    names.push_back("WALL DE LA CUISINE");    
		    names.push_back("WALL DU SALON");    
		    names.push_back("WALL DE LA CHAMBRE");
		    names.push_back("CREATE A NEW WALL");
		    
			wallList = guiMenuDeroulant->addDropDownList("WALLS", names, 1.0/4*ofGetWidth(), 0, 20);
			wallList ->setAllowMultiple(false);
			wallList ->setShowCurrentSelected(true); 
			wallList->setAutoClose(true);
			ofAddListener(guiMenuDeroulant->newGUIEvent, this, &Menu::menuEvent);
			
			guiMenuDeroulant->setUIColors( cb2, cb, coh, cf, cfh, cp, cpo );
			
			wallListAction = -2;
		
		}
		
		void update(int& wallSelected){
			if (automixTextInput->isClicked()){
				guiAuto->setPosition(30, ofGetHeight()/2+20);
				menuButtons[AUTOMIX_PAGE]->setPos(30+2.0/8*ofGetWidth()-20, ofGetHeight()/2+20);
			} else {
				guiAuto->setPosition(1.0/4*ofGetWidth()+10, ofGetHeight()-HEIGHT_BUTTONS*3.0/4);
				menuButtons[AUTOMIX_PAGE]->setPos(9.0/16*ofGetWidth(), ofGetHeight()-HEIGHT_BUTTONS+10);
			}
			
			if (searchTextInput->isClicked()){
				guiSearch->setPosition(30, ofGetHeight()/2+20);
				menuButtons[SEARCH_PAGE]->setPos(30+2.0/8*ofGetWidth()-20, ofGetHeight()/2+20);
			} else {
				guiSearch->setPosition(5.0/8*ofGetWidth()+10, ofGetHeight()-HEIGHT_BUTTONS*3.0/4);
				menuButtons[SEARCH_PAGE]->setPos(15.0/16*ofGetWidth(), ofGetHeight()-HEIGHT_BUTTONS+10);
			}
			
			if (wallList->getSelectedIndeces().size() > 0)
				wallSelected = wallList->getSelectedIndeces()[0];
		}
	
		
		void draw(){
			 ofPushStyle();
			    ofSetColor(36, 37, 38);
			    ofFill();
			    ofRect(0, 0, ofGetWidth(), HEIGHT_BUTTONS);
			    ofRect(0, ofGetHeight()-HEIGHT_BUTTONS, ofGetWidth(), HEIGHT_BUTTONS);
			    
			    ofSetColor(255);
			    ofNoFill();
			    ofRect(0, 0, ofGetWidth(), HEIGHT_BUTTONS);
			    ofRect(0, ofGetHeight()-HEIGHT_BUTTONS, ofGetWidth(), HEIGHT_BUTTONS);
			    ofRect(1.0/4*ofGetWidth(), ofGetHeight()-HEIGHT_BUTTONS, 3.0/8*ofGetWidth(), HEIGHT_BUTTONS-1);
			    ofRect(5.0/8*ofGetWidth(), ofGetHeight()-HEIGHT_BUTTONS, 3.0/8*ofGetWidth()-1, HEIGHT_BUTTONS-1);
			    
			    ofLine(1.0/4*ofGetWidth()+10, ofGetHeight()- 1.0/4*HEIGHT_BUTTONS, 9.0/16*ofGetWidth()-10, ofGetHeight()- 1.0/4*HEIGHT_BUTTONS);
			    ofLine(5.0/8*ofGetWidth()+10, ofGetHeight()- 1.0/4*HEIGHT_BUTTONS, 15.0/16*ofGetWidth()-10, ofGetHeight()- 1.0/4*HEIGHT_BUTTONS);
			    
		    ofPopStyle();
		    
		    
		    	
		    
		    for (size_t i=0; i < menuButtons.size(); i++){
		    	menuButtons[i]->draw();
		    }
		    
		    //on passe le fond en noir
		    if (automixTextInput->isClicked() || searchTextInput->isClicked()){
		    	ofPushStyle();
		    		ofSetColor(36, 37, 38, 200);
			    	ofFill();
			    	ofRect(0, 0, ofGetWidth(), ofGetHeight() );
		    	ofPopStyle();
		    	
		    	menuButtons[SEARCH_PAGE]->draw();
		    	menuButtons[AUTOMIX_PAGE]->draw();
		    }
		    
		    
		}
		
		void menuEvent(ofxUIEventArgs &e){
		    string name = e.widget->getName();
		    if(name == "WALLS" && !((ofxUIDropDownList *)e.widget)->isOpen())
		    {
		    	
		        ofxUIDropDownList *ddlist = (ofxUIDropDownList *) e.widget;
		        vector<ofxUIWidget *> &selected = ddlist->getSelected(); 
		        
		       	if (ddlist->getSelectedIndice() == ddlist->toggles.size() -1)
		       		wallListAction = -1;
		       	else wallListAction = ddlist->getSelectedIndice();
		    }
		}
		
		vector<Button*> getButtons(){
			return menuButtons;
		}
		
	
		vector<Button*> menuButtons;
		ofxUICanvas *guiAuto;
		ofxUICanvas *guiSearch;
		ofxUICanvas *guiMenuDeroulant; 
		ofxUIDropDownList *wallList;
		ofxUITextInput *automixTextInput;
		ofxUITextInput *searchTextInput;
		
		int wallListAction;
	

};