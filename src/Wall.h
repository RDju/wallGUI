
#define X_BUTTONS 0.73*ofGetWidth()
#define Y_BUTTONS 0.17*ofGetHeight()

#define SCREEN_MAX_NUMBER 10

class Wall{

public:
	int IDmodulesCount;
	
	ofRectangle gridSize;
	ofRectangle gridLines[22];//Contient les coordonnées de chaque ligne de la grille (en relatif par rapport au xy de gridSize)
	vector<int> touchOrder;
	vector<ofPoint> splitableModules;
	int gridRepresentation[7][7];
	int IDbutton = 0;
	
	int bordureX; 
	int bordureY;
	
	bool isLibraryOpened;
	bool isButtonActioned;
	bool newWallCreated;
	
	vector<Module*> modules;
	vector<Module*> libraryModules;
	vector<Button*> wallButtons;
	
	ofxUICanvas *saveWallGUI;
	ofxUITextInput *saveWallTextInput;
	
	ofxXmlSettings modSettings;
    ofxXmlSettings mediaSettings;

    string xmlStructure;
	int lastTagNumber;
	
	bool drawMedia;
	
	vector<string> names;
	
	Wall(){}
	
	void setup(){
		IDmodulesCount = 0;
		isLibraryOpened = false;
		newWallCreated = false;
		
		gridSize.set( (float)(0.09*ofGetWidth()+WIDTH_BUTTONS), 0, (float)(ofGetWidth()-0.09*ofGetWidth()-WIDTH_BUTTONS), (float)(ofGetHeight()-0.15*ofGetHeight()-HEIGHT_BUTTONS));
		gridSetup();
		
		/*wallButtons.push_back(new Button("screen13", 0, 0.045*ofGetWidth() , Y_BUTTONS, WIDTH_BUTTONS, HEIGHT_BUTTONS, WALLCREATION_PAGE, "13' screen", "13' screen"));
		wallButtons.push_back(new Button("screen24", 1, 0.045*ofGetWidth() , Y_BUTTONS+HEIGHT_BUTTONS+0.013*ofGetHeight(), WIDTH_BUTTONS, HEIGHT_BUTTONS, WALLCREATION_PAGE, "24' screen", "24' screen"));
		wallButtons.push_back(new Button("screen27", 2, 0.045*ofGetWidth() , Y_BUTTONS+2*(HEIGHT_BUTTONS+0.013*ofGetHeight()), WIDTH_BUTTONS, HEIGHT_BUTTONS, WALLCREATION_PAGE, "27' screen", "27' screen"));*/
		
		wallButtons.push_back(new Button("library", IDbutton++, 25, ofGetHeight() - HEIGHT_BUTTONS*2 - 25, 1.0/2*2.0/5*ofGetWidth()-10, HEIGHT_BUTTONS , WALLCREATION_PAGE, "LIBRARY", "LIBRARY", 40));
		wallButtons.push_back(new Button("delete", IDbutton++, 25+1.0/2*2.0/5*ofGetWidth(), ofGetHeight() - HEIGHT_BUTTONS*2 - 25, 1.0/2*2.0/5*ofGetWidth()-10, HEIGHT_BUTTONS , WALLCREATION_PAGE, "DELETE", "DELETE", 40));
		wallButtons.push_back(new Button("speaker", IDbutton++, 25+2*(1.0/2*2.0/5*ofGetWidth()), ofGetHeight() - HEIGHT_BUTTONS*2 - 25, 1.0/2*2.0/5*ofGetWidth()-10, HEIGHT_BUTTONS , WALLCREATION_PAGE, "EXT SPEAKER", "EXT SPEAKER", 40));
		wallButtons.push_back(new Button("save", IDbutton++, ofGetWidth()-25-(1.0/2*2.0/5*ofGetWidth()-10), ofGetHeight() - HEIGHT_BUTTONS*2 - 25, 1.0/2*2.0/5*ofGetWidth()-10, HEIGHT_BUTTONS , WALLCREATION_PAGE, "SAVE", "SAVE", 40));
		
		
		for (int i = 0; i < 4; i++)
			for (int j = 0; j <3; j++)
				libraryModules.push_back(new Module(j, i, -1));
		
		/*names.push_back("WALL DE LA CUISINE");    
		names.push_back("WALL DU SALON");    
		names.push_back("WALL DE LA CHAMBRE");*/
		//names.push_back("CREATE A NEW WALL");
		
		getWallListName();
				
		ofxUIColor cb = ofxUIColor(0, 0, 0, 0); //Background 
		ofxUIColor cb2 = ofxUIColor(255,255,255,255); //BG liste déroulante
		ofxUIColor co = ofxUIColor(255, 0, 0, 255); // ???
		ofxUIColor coh = ofxUIColor(40, 40, 40, 255); //tour quand sélectionné
		ofxUIColor cf = ofxUIColor(40, 40, 40, 255); //texte non sélectionné
		ofxUIColor cfh = ofxUIColor(40, 40, 40, 255); //texte sélectionné + curseur
		ofxUIColor cp = ofxUIColor(255, 255, 255, 255); // ???
		ofxUIColor cpo = ofxUIColor(255, 0, 0, 255);
				
		saveWallGUI = new ofxUICanvas(30, ofGetHeight()/2+20, 2.0/8*ofGetWidth()-20, HEIGHT_BUTTONS);
		saveWallGUI->setFont("open-sansbold-italic.ttf");
		saveWallGUI->setWidgetFontSize(OFX_UI_FONT_LARGE);
		saveWallGUI->setUIColors( cb2, cb, coh, cf, cfh, cp, cpo );
		saveWallTextInput = saveWallGUI->addTextInput("Wall", "Wall title", -1); 
		saveWallTextInput->setAutoUnfocus(true);
		saveWallTextInput->setAutoClear(true);
		saveWallGUI->setVisible(false);
		
		//XML
		if( modSettings.loadFile("modules.xml") && mediaSettings.loadFile("media.xml") ){
			cout<<"mySettings.xml loaded!"<<endl;
		}else{
			cout<<"unable to load mySettings.xml check data/ folder"<<endl;
		}
		
	}
	
	void update(){
		
		//suppression des modules
	    for (size_t i = 0; i<modules.size(); i++){
	    	if (modules[i]->getIsDeleted()){
	    		modules.erase(modules.begin()+i);//delete module 
	    		/*for (size_t j=i; j<modules.size(); j++){//decrement the ID of module following the one just deleted
	    			modules[j]->setID(modules[j]->getID()-1);
	    		}*/
	    		//IDmodulesCount--;
	    		for (size_t j=0; j<touchOrder.size();j++){//remove the ID from the deleted module from touchOrder
	    			if (touchOrder[j]==i){
	    				touchOrder.erase(touchOrder.begin() + j);
	    				for (size_t k=0; k<touchOrder.size();k++)
	    					if (touchOrder[k] > i) touchOrder[k]--;
	    				break;
	    			}
	    		}
	    		
	    		/*for (int i=8; i<11; i++)
	    			wallButtons[i]->setIsAvailable(true);*/
	    	}
	    }
	    
	    //Detect collision
	    for (int i = 0; i< modules.size(); i++){
	    	modules[i]->setIsWellPlaced(true);
	    }
	    /*for (size_t i = 0; i< modules.size(); i++){
	    	for (size_t j = 0; j< modules.size(); j++){
	    		if(j!=i && modules[i]->isCollision(modules[j]) && rankInTouchOrder(i)>rankInTouchOrder(j))
	    			modules[i]->setIsWellPlaced(false);
	    	}
	    }*/
	    //Alone module
	    if (modules.size() > 1){
		    for (size_t i = 0; i< modules.size(); i++){
		    	//if (modules[i]->getPos().y < gridSize.getHeight()){ // ???
			    	bool alone=true;
			    	for (size_t j = 0; j< modules.size(); j++){
			    		if (i!=j && !modules[i]->isAlone(modules[j])){
			    			alone = false;
			    		}
			    	}
			    	if(alone) modules[i]->setIsWellPlaced(false);
			    	else modules[i]->setIsWellPlaced(true);
			   	//}
		    }
		}
		
		//splitable modules
		splitableModules.clear();
		if (modules.size() > 1){
		    for (size_t i = 0; i< modules.size(); i++){
		    	for (size_t j = i+1; j< modules.size(); j++){
		    		if (modules[i]->getTypeSize()==1 && modules[j]->getTypeSize()==1 && modules[i]->getTypeMat() == modules[j]->getTypeMat() && modules[i]->getTypeMat()!=3 && modules[i]->isSplitable(modules[j]) ){
						splitableModules.push_back(ofPoint(i, j));
						//ofLogNotice() << i << " et " << j << " splitable";
		    		}
		    	}
			}
		}
		
		if (saveWallGUI->isVisible() && !saveWallTextInput->isClicked() ){
			saveWallGUI->setVisible(false);

			//TODO : save if new wall, modif if existing wall
			wall2XML(names.size()-1);
			
			ofLogNotice() << "create at id : " << names.size()-1;
			
			/*names.erase(names.end());
			names.push_back(saveWallTextInput->getTextString());
			names.push_back("CREATE A NEW WALL");*/
			
			getWallListName();
			newWallCreated = true;
		}
	}
	
	void draw(){
	
		/*ofPushStyle();
    		ofFill();
    		ofSetColor(10);
    		ofRect(0.03*ofGetWidth(), 0.03*ofGetWidth(), WIDTH_BUTTONS+ 0.03*ofGetWidth(), ofGetHeight() - 2 * 0.045*ofGetHeight());
    	ofPopStyle();*/
    		
		gridDraw();
		
		for (size_t i=0; i < wallButtons.size(); i++){
				wallButtons[i]->draw();
		}
		
		for (size_t i = 0; i<modules.size();i++){
    		modules[touchOrder[i]]->draw();
    	}
	}
	
	void drawLibrary(){
		for(int i = 0; i < libraryModules.size(); i++){
			if ( libraryModules[i]->getTypeSize() == 2)
				libraryModules[i]->setPos(bordureX +libraryModules[i]->getTypeMat() * 2.5 *1.7*GRID_RATIO, bordureY + 50 + 5 * GRID_RATIO);
			else libraryModules[i]->setPos(bordureX +libraryModules[i]->getTypeMat() * 2.5 *1.7*GRID_RATIO, bordureY + 50 + libraryModules[i]->getTypeSize() *1.5*libraryModules[i]->getTypeSize() * GRID_RATIO);
			libraryModules[i]->draw();
		}
	}
	
	
	//**********************Touch****************************/
	
	void touchDown(int x, int y){
	
		//---- Gestion des modules
		//Trie les modules par ordre de dernière sélection
		for (size_t i=0; i < modules.size(); i++){
			modules[touchOrder[i]]->onTouchDown(x, y);
			if (modules[touchOrder[i]]->getIsTouched()){
				touchOrder.push_back(touchOrder[i]);
				touchOrder.erase(touchOrder.begin()+i);

			}
		}
		//Ne laisse sélectionné que celui du dessus en cas de 2 modules superposés
		for (size_t i=0; i < modules.size(); i++){
			for (int j=0; j<modules.size();j++){
				if(i!=j && modules[j]->getIsTouched() && rankInTouchOrder(j)<rankInTouchOrder(i)){
						modules[i]->setIsTouched(false);
				}
			}
		}
		
		if (isLibraryOpened){
			for (int i = 0; i < libraryModules.size(); i++){
				if (libraryModules[i]->onTouchDown(x, y)){
					isLibraryOpened = false;
					
					modules.push_back(new Module(libraryModules[i]->getTypeSize(), libraryModules[i]->getTypeMat(), IDmodulesCount++));
					modules[/*IDmodulesCount-1*/ (int)modules.size()-1]->isSelected = true;
					touchOrder.push_back(modules.size()-1/*IDmodulesCount-1*/);
				}
			}
		}	
		
		//---- Gestion des boutons
		for (size_t i=0; i < wallButtons.size(); i++){
				wallButtons[i]->isTouchedDown(x, y);
		}
	}
	
	
	//-----------------------------------------
	void touchUp(int x, int y){
	
		for (size_t i=0; i < 4; i++){
			if (wallButtons[i]->isTouchedUp(x, y)){
				switch (wallButtons[i]->getID()){
					case 0: //library
						//on ouvre une fenêtre avec tous les modules
						/*ofLogNotice() << "number: " << (int)modules.size()-1;
						if ((int)modules.size()-1 < SCREEN_MAX_NUMBER)*/
							isLibraryOpened = true;
						break;
					case 1://delete
						//on suppr le module sélectionné
						if (moduleSelected() != -1)
							modules[moduleSelected()]->setIsDeleted(true);
						break;
					case 2://ext speaker
						//page musique
						break;
					case 3://save
						//TODO: if new wall -> empty else name wall
						saveWallGUI->setVisible(true);
						saveWallTextInput->click();
						break;
				}
			}
		}
		
		for (size_t i=0; i < modules.size(); i++){
			modules[i]->onTouchUp(x, y);
		}
	}
	
	void touchMoved(int x, int y){
		for (size_t i=0; i < modules.size(); i++){
			modules[i]->onTouchMove(x, y);
		}
	}
	
	
	/****************************Grid**************************/
	void gridSetup(){

		/*for (int i = 1; i <= 6;i++){//verticales
			gridLines[i-1].set(i*1.7*GRID_RATIO, 0.03*ofGetHeight(), i*1.7*GRID_RATIO, gridSize.getHeight()-0.03*ofGetHeight());
		}
		for (int i = 0; i < 7;i++){//horizontales
			gridLines[i+6].set(0.03*ofGetWidth(), (i+1)*GRID_RATIO, gridSize.getWidth() - 0.06*ofGetWidth(), (i+1)*GRID_RATIO);
		}*/
		
		int decalage = 0;
		/*for (int i = 0; i < 6;i++){
			if (i==2 || i==3) decalage = 1;
			if(i==4 || i==5) decalage = 2;
			gridLines[i].set((i+4)*1.7*GRID_RATIO-decalage, 0.03*ofGetHeight(), (i+4)*1.7*GRID_RATIO-decalage, gridSize.getHeight()-0.03*ofGetHeight());
		}
		for (int i = 0; i < 7;i++){
			gridLines[i+6].set(gridSize.getX()+0.03*ofGetWidth(), (i+1)*GRID_RATIO, ofGetWidth() - 0.03*ofGetWidth(), (i+1)*GRID_RATIO);
		}
		
		for(int i=0; i<7; i++){<z²²
			for (int j=0; j<7; j++){
				gridRepresentation[i][j] = 0;
			}
		}*/
		
		bordureX = (((ofGetWidth()-50)/(1.7*GRID_RATIO) - (float)((int)((ofGetWidth()-50)/(1.7*GRID_RATIO))))*1.7*GRID_RATIO + 50) /2; //63;
		bordureY = (((ofGetHeight()-200.0)/GRID_RATIO - (float)((int)((ofGetHeight()-200)/GRID_RATIO)))*GRID_RATIO +25 /*+GRID_RATIO*/) /2;
		for (int i = 0; i <13; i++){//verticales
			gridLines[i].set(bordureX + i*(int)(1.7*GRID_RATIO) /*- (i/2)*/, 50+bordureY-10, bordureX + i*(int)(1.7*GRID_RATIO) /*- (i/2)*/, ofGetHeight() - 120 - bordureY);
		}
		for (int i = 0; i < 9; i++){//horizontales
			gridLines[i+13].set(bordureX-10, 50+bordureY+i*GRID_RATIO, ofGetWidth() - bordureX+10, 50+bordureY+i*GRID_RATIO);
		}
	}
	
	void gridDraw(){
		for (int i=0; i<22; i++){
			ofPushStyle();
	    	ofSetColor(40);
	    	ofLine(gridLines[i].getX(), gridLines[i].getY(), gridLines[i].getWidth(), gridLines[i].getHeight());
	    	ofPopStyle();
		}
	}
	
	//fill an array corresponding to the grid with . if the case is empty and # if there is a module in the case.
	void wallRepresentation(){
		for(int i=0; i<7; i++){
			for (int j=0; j<7; j++){
				gridRepresentation[i][j] = 0;
			}
		}
		for(int i=0; i<modules.size();i++){
		
			//gridRepresentation[(int)(modules[i]->locationInGrid.x)][(int)(modules[i]->locationInGrid.y)] = 1;
			
			/*if (modules[i]->getType()==2){
				gridRepresentation[(int)(modules[i]->locationInGrid.x)][(int)(modules[i]->locationInGrid.y)+1]=1;
				gridRepresentation[(int)(modules[i]->locationInGrid.x)][(int)(modules[i]->locationInGrid.y)+2]=1;
			} else if (modules[i]->getType()==3){
				gridRepresentation[(int)(modules[i]->locationInGrid.x)][(int)(modules[i]->locationInGrid.y)+1]=1;
				gridRepresentation[(int)(modules[i]->locationInGrid.x)+1][(int)(modules[i]->locationInGrid.y)]=1;
				gridRepresentation[(int)(modules[i]->locationInGrid.x)+1][(int)(modules[i]->locationInGrid.y)+1]=1;
			}*/
		}
	}
	
	
	//-------------------------XML--------------------------------------//
	
	void wall2XML(int wallID){
	
	
	//---------------WORK IN PROGRESS : save several walls -----------------------//
		/*bool wallFound = false;
		if (modSettings.load("modules.xml")){
			int numTagsWALL = modSettings.getNumTags("WALL");
			for (int i = 0; i < numTagsWALL; i++){
	    		modSettings.pushTag("WALL", i);
	    			if (modSettings.getValue("ID", i) == wallID){
				    				wallFound = true;
				    				ofLogNotice() << "trouvé";
				    				//on remplace les modifs
				    				modSettings.setValue("NAME", saveWallTextInput->getTextString());
				    				modSettings.pushTag("SPLIT");
				    					int numTagsS = modSettings.getNumTags("S");
					    				for (int i=0; i < splitableModules.size(); i++){
					    					if (i < numTagsS){
					    						modSettings.pushTag("S", i);
											    	modSettings.setValue("A", splitableModules[i].x);
											    	modSettings.setValue("B", splitableModules[i].y);
										    	modSettings.popTag();
					    					} else {
					    						modSettings.addTag("S");
										   		modSettings.pushTag("S", i);
										   			modSettings.addValue("A", splitableModules[i].x);
											    	modSettings.addValue("B", splitableModules[i].y);
					    						modSettings.popTag();
					    					}
					    				}
									modSettings.popTag();//("SPLIT")
									
									int numTagsMODULES = modSettings.getNumTags("MODULES");
									for(int i = 0; i<modules.size(); i++){
										if (i >= numTagsMODULES)
									        lastTagNumber = modSettings.addTag("MODULES");
									    modSettings.pushTag("MODULES", i);
									    modSettings.setValue("ID", modules[i]->getID(), 0);
									    modSettings.setValue("SIZE", modules[i]->getTypeSize(), 0);
										modSettings.setValue("TYPE", modules[i]->getTypeMat(), 0);
										modSettings.addTag("POS");
										modSettings.pushTag("POS");
										    modSettings.setValue("X", modules[i]->getPos().x, 0);
										    modSettings.setValue("Y", modules[i]->getPos().y, 0);
										modSettings.popTag();
										modSettings.addTag("POSGRID");
										modSettings.pushTag("POSGRID");
										    ofPoint tmpPosGrid = modules[i]->getPosInGrid();
										    modSettings.setValue("X", tmpPosGrid.x, 0);
										    modSettings.setValue("Y", tmpPosGrid.y, 0);
										modSettings.popTag(); 
									    modSettings.popTag();
								    }
	    			}
	    		modSettings.popTag();//("WALL")
	    	}		
		}
		
		if (!wallFound){
			//on créé
		
			lastTagNumber	= modSettings.addTag("WALL");
	    	ofLogNotice() << "on créé : " << lastTagNumber;
	    	
		    modSettings.pushTag("WALL", wallID);
		    modSettings.addValue("ID", wallID);
		    modSettings.addValue("NAME", saveWallTextInput->getTextString());
		    modSettings.addTag("SPLIT");
			modSettings.pushTag("SPLIT");
		    for (int i=0; i < splitableModules.size(); i++){
		    	modSettings.addTag("S");
			   	modSettings.pushTag("S");
			    	
			    	modSettings.addValue("A", splitableModules[i].x);
			    	modSettings.addValue("B", splitableModules[i].y);
		    	modSettings.popTag();
		    }
			modSettings.popTag();
			
		    for(int i = 0; i<modules.size(); i++){
		
		        lastTagNumber = modSettings.addTag("MODULES");
		        modSettings.pushTag("MODULES", i);
		        	modSettings.setValue("ID", modules[i]->getID(), 0);
		        	modSettings.setValue("SIZE", modules[i]->getTypeSize(), 0);
			        modSettings.setValue("TYPE", modules[i]->getTypeMat(), 0);
			        modSettings.addTag("POS");
			        modSettings.pushTag("POS");
			        	modSettings.setValue("X", modules[i]->getPos().x, 0);
			        	modSettings.setValue("Y", modules[i]->getPos().y, 0);
			        modSettings.popTag();
			        modSettings.addTag("POSGRID");
			        modSettings.pushTag("POSGRID");
			        	ofPoint tmpPosGrid = modules[i]->getPosInGrid();
			        	modSettings.setValue("X", tmpPosGrid.x, 0);
			        	modSettings.setValue("Y", tmpPosGrid.y, 0);
			        modSettings.popTag(); 
		        modSettings.popTag();
		    }
		    modSettings.popTag();
		}
		
		//ofSetDataPathRoot("");
	   	//modSettings.save("/storage/emulated/0/Android/data/cc.openframeworks.wallGUI2/files/modules.xml");
		//modSettings.save("file://192.168.1.17:8000/modules.xml");
		modSettings.save("modules.xml");*/
		//---------------------------------------------------------------------
		
		modSettings.clear();
	
	    lastTagNumber	= modSettings.addTag("WALL");
	    
	    modSettings.pushTag("WALL", 0);
	    modSettings.setValue("ID", (int)names.size()-1, 0);
	    modSettings.setValue("NAME", saveWallTextInput->getTextString());
	    modSettings.addTag("SPLIT");
		modSettings.pushTag("SPLIT");
	    for (int i=0; i < splitableModules.size(); i++){
	    	modSettings.addTag("S");
		   	modSettings.pushTag("S");
		    	
		    	modSettings.addValue("A", splitableModules[i].x);
		    	modSettings.addValue("B", splitableModules[i].y);
	    	modSettings.popTag();
	    }
		modSettings.popTag();
		
	    for(int i = 0; i<modules.size(); i++){
	
	        lastTagNumber = modSettings.addTag("MODULES");
	        modSettings.pushTag("MODULES", i);
	        	modSettings.setValue("ID", modules[i]->getID(), 0);
	        	modSettings.setValue("SIZE", modules[i]->getTypeSize(), 0);
		        modSettings.setValue("TYPE", modules[i]->getTypeMat(), 0);
		        modSettings.addTag("POS");
		        modSettings.pushTag("POS");
		        	modSettings.setValue("X", modules[i]->getPos().x, 0);
		        	modSettings.setValue("Y", modules[i]->getPos().y, 0);
		        modSettings.popTag();
		        modSettings.addTag("POSGRID");
		        modSettings.pushTag("POSGRID");
		        	ofPoint tmpPosGrid = modules[i]->getPosInGrid();
		        	modSettings.setValue("X", tmpPosGrid.x, 0);
		        	modSettings.setValue("Y", tmpPosGrid.y, 0);
		        modSettings.popTag(); 
	        modSettings.popTag();
	    }
	    modSettings.popTag();
		//save at : /storage/emulated/0/Android/data/cc.openframeworks.wallGUI2/files/modules.xml
		
		//EDDY
		//ofSetDataPathRoot("");
	   	//modSettings.save("/storage/emulated/0/Android/data/cc.openframeworks.wallGUI2/files/modules.xml");
		//ofLogNotice() << modSettings.save("file://192.168.1.17:8000/modules.xml");
		modSettings.save("modules.xml");
	}
	
	void XML2Wall(int wallID){
	
	    modules.clear();
	    touchOrder.clear();

	    ofFile tempXML;
	    ofBuffer dataBuffer;
	    
	    tempXML.open(ofToDataPath("temp.xml"), ofFile::ReadWrite, false);
	    dataBuffer = ofLoadURL("http://192.168.1.13:8000/test2.xml").data;
	    ofBufferToFile("temp.xml", dataBuffer);
	    
	    modSettings.load("temp.xml");
	    tempXML.remove();
	    
		
	    int numTagsWALL = modSettings.getNumTags("WALL");
	    
	    for (int i = 0; i < numTagsWALL; i++){
	    	modSettings.pushTag("WALL", i);
	    	//if (modSettings.getValue("ID", i) == wallID){
	    		//modSettings.pushTag("WALL");
		    		int numTagsMODULES = modSettings.getNumTags("MODULES");
		    		for (int j = 0; j < numTagsMODULES; j++){
		    			modSettings.pushTag("MODULES", j);
		    			
		    				int tmpSize;
		    				tmpSize = modSettings.getValue("SIZE", j);
		    				int tmpType;
		    				tmpType = modSettings.getValue("TYPE", j);
		    				
		    				
		    				ofPoint tmpPos;
		    				modSettings.pushTag("POS");
			        			tmpPos.x = modSettings.getValue("X", j);
			        			tmpPos.y = modSettings.getValue("Y", j);
			        		modSettings.popTag();
			        		
			        		int tmpID;
			        		tmpID = modSettings.getValue("ID", j);
			        		if (IDmodulesCount < tmpID) IDmodulesCount = tmpID;
			        		
			        		modules.push_back(new Module(tmpSize, tmpType, /*IDmodulesCount++*/ tmpID));
		    				modules[j]->setPos(tmpPos.x, tmpPos.y);
		    				touchOrder.push_back(j);
		        			
		    			modSettings.popTag();
		    			
		    			
		    		}
		    	//modSettings.popTag();
	    	//}
	    	modSettings.popTag();
	    }
	    modSettings.popTag();
	}
	
	void getWallListName(){
		
		modSettings.load("modules.xml");
		
		names.clear();
		int numTagsWALL = modSettings.getNumTags("WALL");
	    for (int i = 0; i < numTagsWALL; i++){
	    	modSettings.pushTag("WALL", i);
	    		names.push_back((string)modSettings.getValue("NAME", "", i));
	    		ofLogNotice() << "name : " << (string)modSettings.getValue("NAME", "", i);
	    	modSettings.popTag();
	    }
	    names.push_back("CREATE A NEW WALL");
	}
	    
	//-------------------
	
	int moduleSelected(){
		for (int i = 0; i < modules.size(); i++){
			if(modules[i]->isSelected) 
				return i;
		}
		return -1;
	}
	
	
	//Argument : ID of the module
	//return : order rank 
	int rankInTouchOrder(int value){
		for (size_t i = 0; i<touchOrder.size();i++){
			if (touchOrder[i]==value) return i;
		}
		return -1;
	}
	
	
	
};