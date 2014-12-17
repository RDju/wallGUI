#include <vector>
#include <string>
#include "ofxUI.h"

#define CHANNEL_IMAGE_WIDTH (ofGetWidth() - 3.0*10.0/100*ofGetWidth())/3.0
#define CHANNEL_IMAGE_HEIGHT (ofGetHeight() - HEIGHT_BUTTONS*2 - 2.0*1.0/9*ofGetHeight() - 1.0/15 * ofGetHeight())/2.0

class Channel{
	public:
		ofImage channelImage;
		
		string title;
		string creator;
		string price;
		string description;
		vector<string> tags;
		string tagsString;
		int ID;
		int rate;
		
		int imgVidRate, transitionSpeed, colorHarmony, sensorInput, sensorSensitivity;
		
		ofTrueTypeFont titleFont;
		ofTrueTypeFont infoFont;
		
		ofxUICanvas *guiDescription;
		ofxUITextArea *descriptionArea;
		
		
		Button *playButton;
		
		ofPoint tempPosition;
		
		bool automixImageFound;
		
		
		Channel()
		{
			creator = "user";
			rate = 0;
			automixImageFound = false;
			
			price = "free";
			tempPosition.set(0, 0);
			
			imgVidRate = 0;
			transitionSpeed = 0;
			colorHarmony = 0;
			sensorInput = 0;
			sensorSensitivity = 0;
			
			titleFont.loadFont("open-sansbold-italic.ttf", 15);
			titleFont.setEncoding(OF_ENCODING_ISO_8859_15);
			infoFont.loadFont("OpenSans-Light.ttf", 0.02*ofGetHeight());
		}
		
		
		Channel(string url, string title, string creator, int rate, int ID): title(title), creator(creator), rate(rate), ID(ID)
		{
			channelImage.loadImage(url);
			channelImage.resize(CHANNEL_IMAGE_WIDTH, CHANNEL_IMAGE_HEIGHT);
			titleFont.loadFont("open-sansbold-italic.ttf", 15);
			titleFont.setEncoding(OF_ENCODING_ISO_8859_15);
			infoFont.loadFont("OpenSans-Light.ttf", 0.02*ofGetHeight());
			price = "free";
			tempPosition.set(0, 0);
			
			imgVidRate = (rand()%4)*25;
			transitionSpeed = (rand()%4)*25;
			colorHarmony = (rand()%4)*25;
			sensorInput = rand()%1;
			sensorSensitivity = (rand()%4)*25;
			
			playButton = new Button ("play", ID, 0, 0, 1.0/10*ofGetWidth(), HEIGHT_BUTTONS-20, 3, "PLAY", "PLAY", 40);
						
			description = " Lorem ipsum dolor sit amet, consectetur adipiscing elit. Integer tempor a ipsum quis lobortis. Nunc nec orci condimentum lectus mollis suscipit. Nunc fringilla lorem ac maximus imperdiet. Aenean neque massa, laoreet ut lorem nec, ultrices eleifend nulla. Duis auctor arcu dolor, id fermentum leo accumsan et. Morbi ullamcorper, nunc eget elementum lobortis, massa justo feugiat dolor, eu porttitor nibh nulla nec arcu. Ut vitae purus aliquam, convallis mi et, ultricies sapien. Mauris elementum lectus ante, laoreet facilisis mi aliquam nec. Sed imperdiet, ex vel efficitur faucibus, sem nibh bibendum metus, a lobortis enim sem lobortis ex. Nulla facilisi. Morbi finibus augue sed enim congue, a porttitor enim elementum. Nullam nec sollicitudin dolor. Vivamus ornare efficitur nunc vel dignissim. Praesent semper orci in nisi viverra, ut maximus mauris dictum. Nunc lorem sem, volutpat ac orci ut, venenatis eleifend magna. Duis eget ornare nisl. Donec nunc felis, posuere non aliquet ut, accumsan aliquam sapien. Nunc semper suscipit mattis. Sed eu velit sapien. Integer fermentum eget turpis sit amet elementum. Lorem ipsum dolor sit amet, consectetur adipiscing elit. Curabitur felis velit, dignissim sit amet rhoncus vitae, consectetur at elit. Cras faucibus, felis ac tristique bibendum, velit erat dapibus tortor, quis faucibus nisl velit id ante. Duis varius nisi dolor, efficitur lacinia metus condimentum viverra. Interdum et malesuada fames ac ante ipsum primis in faucibus. Fusce et erat faucibus, egestas metus ut, vestibulum eros. Curabitur et urna fermentum massa iaculis dignissim at id arcu. Aliquam tincidunt sagittis libero, a accumsan ligula pulvinar et. ";
			tags.push_back("poulet");
			tags.push_back("arc-en-ciel");
			tags.push_back("jambon");
			
			for (int i = 0; i < tags.size(); i++){
				tagsString += tags[i] + ", ";
			}
			
			ofxUIColor cb = ofxUIColor(0, 0, 0, 0); //Background 
			ofxUIColor cb2 = ofxUIColor(40, 40, 40, 150); //BG liste déroulante
		    ofxUIColor co = ofxUIColor(255, 0, 0, 255); // ???
		    ofxUIColor coh = ofxUIColor(255, 255, 255, 255); //tour quand sélectionné
		    ofxUIColor cf = ofxUIColor(40, 40, 40, 255); //texte non sélectionné
		    ofxUIColor cfh = ofxUIColor(255, 255, 255, 255); //texte sélectionné + curseur
		    ofxUIColor cp = ofxUIColor(255, 255, 255, 255); // ???
		    ofxUIColor cpo = ofxUIColor(255, 0, 0, 255);
		    
			guiDescription = new ofxUICanvas(  50 + 2.0/5*ofGetWidth(), HEIGHT_BUTTONS+40+titleFont.getSize()*2, 3.0/5*ofGetWidth()-60, ofGetHeight() - 2*HEIGHT_BUTTONS+40);
			guiDescription->setFont("OpenSans-Light.ttf");
			guiDescription->setWidgetFontSize(OFX_UI_FONT_LARGE);
			guiDescription->setUIColors( cb, cb, coh, cf, cfh, cp, cpo );
			descriptionArea = guiDescription->addTextArea("description", description); 
			
			guiDescription->setVisible(false);
		}

		void drawPreview(int x, int y){
			ofPushMatrix();
			ofPushStyle();
				ofSetColor(40);
				titleFont.drawString(title, x, y);
				ofSetColor(255);
				channelImage.resize(CHANNEL_IMAGE_WIDTH, CHANNEL_IMAGE_HEIGHT);
				channelImage.draw(x, y+5/*+titleFont.getSize()*/);
				ofSetColor(127);
				infoFont.drawString("By: " + creator, x, y/*+1.0/32*ofGetHeight()*/+CHANNEL_IMAGE_HEIGHT+ infoFont.getSize()+10);
				infoFont.drawString("Price: " + price, x, y/*+1.0/32*ofGetHeight()*/+CHANNEL_IMAGE_HEIGHT+ infoFont.getSize()*2 + 15);
				for (int i = 0; i < 5; i ++ ){
					if (rate>i) ofFill();
					else ofNoFill();
					
					ofSetColor(36, 37, 38);
					ofCircle(x+CHANNEL_IMAGE_WIDTH*2.0/3+i*infoFont.getSize()+i*0.5*infoFont.getSize(), y/*+1.0/32*ofGetHeight()*/+CHANNEL_IMAGE_HEIGHT+2*infoFont.getSize(), infoFont.getSize()/1.5);
				}
				
			tempPosition.set(x, y);
						
						
			ofPopStyle();
			ofPopMatrix();
		}
		
		void drawPage(){
			channelImage.resize(2.0/5*ofGetWidth(), 2.0/5*ofGetHeight());
			channelImage.draw(20, HEIGHT_BUTTONS+20);
			ofPushStyle();
			ofSetColor(40, 40, 40);
			titleFont.drawString(title + " @ ", 25, HEIGHT_BUTTONS+20+2.0/5*ofGetHeight()+infoFont.getSize()+5);
			infoFont.drawString( creator, 25+titleFont.stringWidth(title + " @ :"), HEIGHT_BUTTONS+20+2.0/5*ofGetHeight()+infoFont.getSize()+5);
			
			for (int i = 0; i < 5; i ++ ){
				if (rate>i) ofFill();
				else ofNoFill();
					
				ofSetColor(36, 37, 38);
				ofCircle(20+2.0/5*ofGetWidth()-100+i*infoFont.getSize()+i*0.5*infoFont.getSize(), HEIGHT_BUTTONS+20+2.0/5*ofGetHeight()+infoFont.getSize()/1.5 / 2 + 10, infoFont.getSize()/1.5);
			}
			
			titleFont.drawString("# ", 25, HEIGHT_BUTTONS+20+2.0/5*ofGetHeight()+infoFont.getSize()+50);
			infoFont.drawString(tagsString, 25 + titleFont.stringWidth("# :"), HEIGHT_BUTTONS+20+2.0/5*ofGetHeight()+infoFont.getSize()+50);
			
			titleFont.drawString("Price: ", 25, HEIGHT_BUTTONS+20+2.0/5*ofGetHeight()+infoFont.getSize()+100);
			infoFont.drawString(price, 25 + titleFont.stringWidth("Price : "), HEIGHT_BUTTONS+20+2.0/5*ofGetHeight()+infoFont.getSize()+100);
			
			titleFont.drawString("Suscribe now - Bookmark now ", 25, HEIGHT_BUTTONS+20+2.0/5*ofGetHeight()+infoFont.getSize()+150);
			
			titleFont.drawString("Description", 50 + 2.0/5*ofGetWidth(), HEIGHT_BUTTONS+20+titleFont.getSize());
			guiDescription->setVisible(true);
			
			ofPopStyle();
		}
		
		void drawMini(int x, int y){
			playButton->setPos(x, y);
			playButton->draw();
			infoFont.drawString(title, x+ 1.0/9*ofGetWidth() + 10, y+infoFont.getSize()+10);
		}
		
		void drawAutomixCreation(string tags){
			if (!automixImageFound) {
    			ofPushStyle();
    				ofSetColor(40, 40, 40, 255);
    				ofFill();
    				ofRect(20, HEIGHT_BUTTONS+20, 2.0/5*ofGetWidth(), 2.0/5*ofGetHeight());
    			ofPopStyle();
    		} else {
    			channelImage.resize(2.0/5*ofGetWidth(), 2.0/5*ofGetHeight());
    			channelImage.draw(20, HEIGHT_BUTTONS+20);
    		}
    		
    		ofPushStyle();
    			ofSetColor(40);
    			titleFont.drawString("#", 25, HEIGHT_BUTTONS+20+2.0/5*ofGetHeight()+infoFont.getSize()+50);
				infoFont.drawString(tags, 25 + titleFont.stringWidth("# :"), HEIGHT_BUTTONS+20+2.0/5*ofGetHeight()+infoFont.getSize()+50);
			
				titleFont.drawString("SETTINGS", 50 + 2.0/5*ofGetWidth(), HEIGHT_BUTTONS+20+titleFont.getSize());
				infoFont.drawString ("Photo / Video", 50 + 2.0/5*ofGetWidth(), HEIGHT_BUTTONS+20+titleFont.getSize()+45);
				infoFont.drawString ("Transition Speed", 50 + 2.0/5*ofGetWidth(), HEIGHT_BUTTONS+20+titleFont.getSize()+100);
				infoFont.drawString ("Color Harmony", 50 + 2.0/5*ofGetWidth(), HEIGHT_BUTTONS+20+titleFont.getSize()+155);
				infoFont.drawString ("Sensor Input", 50 + 2.0/5*ofGetWidth(), HEIGHT_BUTTONS+20+titleFont.getSize()+210);
				infoFont.drawString ("Sensor Sensitivity", 50 + 2.0/5*ofGetWidth(), HEIGHT_BUTTONS+20+titleFont.getSize()+265);
			
				titleFont.drawString("MOODS", 50 + 2.0/5*ofGetWidth(), HEIGHT_BUTTONS+20+titleFont.getSize()+350);
			ofPopStyle();
			
		}
		
		void drawAutomixValidation(){
		
		
		}
		
		bool isTouched(int x, int y){
			if (tempPosition.x !=-1 && x>tempPosition.x && x < tempPosition.x+CHANNEL_IMAGE_WIDTH && y > tempPosition.y && y < tempPosition.y + CHANNEL_IMAGE_HEIGHT + 2*infoFont.getSize()) {
				return true;
			}
			return false;
			
		}
};