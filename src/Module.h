
#define WIDTH_13 1.7
#define HEIGHT_13 1
#define WIDTH_24 1.7
#define HEIGHT_24 3
#define WIDTH_27 3.4
#define HEIGHT_27 2

#define GRID_RATIO 75


class Module{
	private:
		int type;
		int ID;
		ofPoint pos;
		ofPoint size;
		ofPoint touch;
		
		bool isTouched;
		bool isDeleted;
		bool isWellPlaced;
		
	
	public:
		ofPoint locationInGrid;
	
	Module(int type, int ID): type(type), ID(ID)
	{
		pos.set(0.1*ofGetWidth()+WIDTH_BUTTONS, ofGetHeight() - 0.15*ofGetHeight() - HEIGHT_BUTTONS, 0);
		touch.set(0, 0, 0);
		isTouched = false;
		isDeleted=false;
		isWellPlaced=true;
		locationInGrid.set(-1, -1);
		
		switch(type){
			case 1:
				size.set(WIDTH_13*75-1, HEIGHT_13*75);
				break;
			case 2:
				size.set(WIDTH_24*75-1, HEIGHT_24*75);
				break;
			case 3:
				size.set(WIDTH_27*75-1, HEIGHT_27*75);
				break;
		}
		
	}
	
	void draw() {
		
		ofPushStyle();
		ofFill();
		if (isWellPlaced) ofSetColor(127);
		else ofSetColor(127, 50, 50);
		ofRect(pos.x, pos.y, size.x, size.y);
		ofNoFill();
		ofSetColor(255);
		ofRect(pos.x, pos.y, size.x, size.y);
		ofPopStyle();
	}
	
	bool isCollision(Module *module){
		if (( module->pos.x + module->size.x <= this->pos.x)
			|| (module->pos.y + module->size.y <= this->pos.y)
			|| (module->pos.x >= this->pos.x+this->size.x)
			|| (module->pos.y >= this->pos.y+this->size.y)){
				return false;
		}		
		else {
			return true;
		}
	}
	bool isAlone(Module *module){
		if (( module->pos.x + module->size.x + 75*1.7 <= this->pos.x )
			|| (module->pos.y + module->size.y +75 <= this->pos.y)
			|| (module->pos.x - 75*1.7 >= this->pos.x+this->size.x)
			|| (module->pos.y  - 75 >= this->pos.y+this->size.y))
				return true;
		else return false;
	}
	
	void onTouchDown(int x, int y){
		if (x > pos.x && x < pos.x + size.x && y > pos.y && y < pos.y + size.y ) {
			isTouched = true;
			touch.set(x-pos.x, y-pos.y);
		}
	}
	void onTouchUp(int x, int y){
		isTouched = false;
	}
	void onTouchMove(int x, int y){
		//si dans le rectangle
		if ( x-touch.x > 0.09*ofGetWidth()+WIDTH_BUTTONS && x+(size.x-touch.x) < ofGetWidth() && y-touch.y > 0 && y+(size.y-touch.y) < ofGetHeight()-0.15*ofGetHeight()-HEIGHT_BUTTONS){
		   
		   if (isTouched) {
		   		if ( (int)abs(x-touch.x) % (int)(1.7*GRID_RATIO) < 50 && (int)abs(y - touch.y) % GRID_RATIO < 50 ){
		   			pos.set(x-touch.x - ((int)(x-touch.x) % int(1.7*GRID_RATIO))+2, (y-touch.y) - ((int)(y-touch.y) % GRID_RATIO));
		   			locationInGrid.set((int)((x-touch.x)/(GRID_RATIO*1.7)-4), (int)((y-touch.y)/GRID_RATIO));
		   			//ofLogNotice() << locationInGrid.x << " " << locationInGrid.y;
		   		}
		   		/*else
		   			pos.set(x-touch.x,y-touch.y);*/
		   	}
		}
	}
	
	void onDoubleClick(int x, int y){
		if (x > pos.x && x < pos.x + size.x && y > pos.y && y < pos.y + size.y )
			isDeleted=true;
	}
	
	
	void setType(int type){
		this->type=type;
	}
	void setID(int ID){
		this->ID=ID;
	}
	void setPos(int x, int y){
		pos.set(x, y, 0);
	}
	void setIsWellPlaced(bool isWellPlaced){
		this->isWellPlaced=isWellPlaced;
	}
	void setIsTouched(bool isTouched){
		this->isTouched=isTouched;
	}
	
	bool getIsDeleted(){
		return isDeleted;
	}
	int getID(){
		return ID;
	}
	int getType(){
		return type;
	}
	bool getIsTouched(){
		return isTouched;
	}
	bool getIsWellPlaced(){
		return isWellPlaced;
	}
	ofPoint getPos(){
		return pos;
	}


};