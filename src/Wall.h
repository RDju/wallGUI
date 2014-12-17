
#define X_BUTTONS 0.73*ofGetWidth()
#define Y_BUTTONS 0.17*ofGetHeight()

#define SCREEN_MAX_NUMBER 10

class Wall{

public:
	int IDmodulesCount;
	
	ofRectangle gridSize;
	ofRectangle gridLines[18];//Contient les coordonnées de chaque ligne de la grille (en relatif par rapport au xy de gridSize)
	vector<int> touchOrder;
	int gridRepresentation[7][7];
	int IDbutton = 0;
	
	int bordureX; 
	int bordureY;
	
	bool isLibraryOpened;
	
	
	bool isButtonActioned;
	
	vector<Module*> modules;
	vector<Module*> libraryModules;
	vector<Button*> wallButtons;
	
	
	Wall(){}
	
	void setup(){
		IDmodulesCount = 0;
		isLibraryOpened = false;
		
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
		
		
		
	}
	
	void update(){
		
		//suppression des modules
	    for (size_t i = 0; i<modules.size(); i++){
	    	if (modules[i]->getIsDeleted()){
	    		ofLogNotice() << "passe 1"; 
	    		modules.erase(modules.begin()+i);//delete module 
	    		ofLogNotice() << "passe 2"; 
	    		for (size_t j=i; j<modules.size(); j++){//decrement the ID of module following the one just deleted
	    			modules[j]->setID(modules[j]->getID()-1);
	    			ofLogNotice() << "passe 3"; 
	    		}
	    		IDmodulesCount--;
	    		ofLogNotice() << "passe 4"; 
	    		for (size_t j=0; j<touchOrder.size();j++){//remove the ID from the deleted module from touchOrder
	    			if (touchOrder[j]==i){
	    				touchOrder.erase(touchOrder.begin() + j);
	    				for (size_t k=0; k<touchOrder.size();k++)
	    					if (touchOrder[k] > i) touchOrder[k]--;
	    				break;
	    			}
	    		}
	    		ofLogNotice() << "passe 5"; 
	    		
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
		    	if (modules[i]->getPos().y < gridSize.getHeight()){ // ???
			    	bool alone=true;
			    	for (size_t j = 0; j< modules.size(); j++){
			    		if (i!=j && !modules[i]->isAlone(modules[j])){
			    			alone = false;
			    		}
			    	}
			    	if(alone) modules[i]->setIsWellPlaced(false);
			    	else modules[i]->setIsWellPlaced(true);
			   	}
		    }
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
				libraryModules[i]->setPos(bordureX +libraryModules[i]->getTypeMat() * 2.5 *127, bordureY + 50 + 5 * 75);
			else libraryModules[i]->setPos(bordureX +libraryModules[i]->getTypeMat() * 2.5 *127, bordureY + 50 + libraryModules[i]->getTypeSize() *1.5*libraryModules[i]->getTypeSize() * 75);
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
					modules[IDmodulesCount-1]->isSelected = true;
					touchOrder.push_back(IDmodulesCount-1);
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
		
    	
    	//Creation de module
    	/*for (size_t i=0; i < wallButtons.size(); i++){
			if (wallButtons[i]->isTouchedUp(x, y)){
				if (modules.size() < SCREEN_MAX_NUMBER){
		    		modules.push_back(new Module(wallButtons[i]->getID(), IDmodulesCount++));
		    		touchOrder.push_back(IDmodulesCount-1);
		    	}
				if (modules.size() == SCREEN_MAX_NUMBER){
			    	for (int i=0; i<wallButtons.size(); i++)
		    			wallButtons[i]->setIsAvailable(false);
		    	}
				
			}
				
		}*/
		
		for (size_t i=0; i < 4; i++){
			if (wallButtons[i]->isTouchedUp(x, y)){
				switch (wallButtons[i]->getID()){
					case 0: //library
						//on ouvre une fenêtre avec tous les modules
						if (IDmodulesCount < SCREEN_MAX_NUMBER)
							isLibraryOpened = true;
						break;
					case 1://delete
						//on suppr le module sélectionné
						ofLogNotice() << "on click " << moduleSelected();
						if (moduleSelected() != -1)
							modules[moduleSelected()]->setIsDeleted(true);
						break;
					case 3://ext speaker
						//page musique
						break;
					case 4://save
						//on sauvegarde le wall
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
		
		for(int i=0; i<7; i++){
			for (int j=0; j<7; j++){
				gridRepresentation[i][j] = 0;
			}
		}*/
		
		bordureX = 63;
		bordureY = (((ofGetHeight()-200.0)/75.0 - (float)((int)((ofGetHeight()-200)/75)))*75 + 75) /2;
		for (int i = 0; i <10; i++){//verticales
			gridLines[i].set(bordureX + i*127, 50+bordureY-10, bordureX + i*127, ofGetHeight() - 150 - bordureY+10);
		}
		for (int i = 0; i < 7; i++){
			gridLines[i+10].set(bordureX-10, 50+bordureY+i*75, ofGetWidth() - bordureX+10, 50+bordureY+i*75);
		}
	}
	
	void gridDraw(){
		for (int i=0; i<18; i++){
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