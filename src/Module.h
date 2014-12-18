#include <vector>
#include <string>
#include "ofxUI.h"
#include "ofApp.h"


#define WIDTH_13 1.7
#define HEIGHT_13 1
#define WIDTH_24 1.7
#define HEIGHT_24 3
#define WIDTH_27 3.4
#define HEIGHT_27 2

#define GRID_RATIO 75

#define SCREEN_TYPE 0
#define LED_TYPE 1
#define LEDSCREEN_TYPE 2
#define TEXTURE_TYPE 3


class Module{
	private:
		int typeSize;
		int typeMat;
		int ID;
		ofPoint pos;
		ofPoint size;
		ofPoint touch;
		ofTrueTypeFont font;
		
		bool isTouched;
		bool isDeleted;
		bool isWellPlaced;
		
		ofColor color;
		
		int bordureX = 63;
		int bordureY = (((ofGetHeight()-200.0)/75.0 - (float)((int)((ofGetHeight()-200)/75)))*75 + 75) /2;
		
	
	public:
		ofPoint locationInGrid;
		bool isSelected;
	
	Module(int typeSize, int typeMat, int ID): typeSize(typeSize), ID(ID), typeMat(typeMat)
	{
		pos.set(bordureX, bordureY+50);//(0.1*ofGetWidth()+WIDTH_BUTTONS, ofGetHeight() - 0.15*ofGetHeight() - HEIGHT_BUTTONS, 0);
		touch.set(0, 0, 0);
		isTouched = false;
		isDeleted=false;
		isWellPlaced=true;
		isSelected=false;
		locationInGrid.set(-1, -1);
		
		font.loadFont("open-sansbold-italic.ttf", 12);
		
		switch(typeSize){
			case 0:
				size.set(WIDTH_13*75, HEIGHT_13*75);
				break;
			case 1:
				size.set(WIDTH_24*75, HEIGHT_24*75);
				break;
			case 2:
				size.set(WIDTH_27*75-1, HEIGHT_27*75);
				break;
		}
		
		switch(typeMat){
			case 0:
				color = ofColor(127, 127, 127);
				break;
			case 1:
				color = ofColor(157, 177, 250);
				break;
			case 2:
				color = ofColor(147, 217, 120);
				break;
			case 3:
				color = ofColor(184, 148, 193);
				break;
		}
		
	}
	
	void draw() {
		
		ofPushStyle();
			ofFill();
			ofSetColor(color);
			ofRect(pos.x, pos.y, size.x, size.y);
			
			if (!isWellPlaced){
				ofSetColor(255, 0, 0);
				ofLine(pos.x, pos.y, pos.x + size.x, pos.y + size.y);
			}
			
			ofNoFill();
			if (isSelected && ID != -1) ofSetColor(255, 0, 0);
			else ofSetColor(255);
			ofRect(pos.x, pos.y, size.x, size.y);
			
			ofSetColor(40);
			switch(typeMat){
				case SCREEN_TYPE:
					font.drawString("Screen", pos.x+5, pos.y+5 + font.getSize());
					break;
				case LED_TYPE:
					font.drawString("LED", pos.x+5, pos.y+5+ font.getSize());
					break;
				case LEDSCREEN_TYPE:
					font.drawString("LED screen", pos.x+5, pos.y+5+ font.getSize());
					break;
				case TEXTURE_TYPE:
					font.drawString("texture", pos.x+5, pos.y+5+ font.getSize());
					break;
			}
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
		if (( module->pos.x + module->size.x -10 <= this->pos.x &&  module->pos.y + module->size.y -10 <= this->pos.y)
			|| (module->pos.x +10 >= this->pos.x+this->size.x && module->pos.y + module->size.y -10 <= this->pos.y)
			|| (module->pos.x + module->size.x -10 <= this->pos.x && module->pos.y +10 >= this->pos.y + this->size.y)
			|| (module->pos.x +10 >= this->pos.x+this->size.x && module->pos.y +10 >= this->pos.y + this->size.y)
			
			|| (module->pos.x + module->size.x -10 <= this->pos.x-75*1.7)
			|| (module->pos.x +10 >= this->pos.x + this->size.x + 75*1.7)
			|| (module->pos.y + module->size.y -10 <= this->pos.y - 75*1.7)
			|| (module->pos.y +10 >= this->pos.y+this->size.y + 75 * 1.7))
				return true;
		else return false;
	}
	
	bool onTouchDown(int x, int y){
			if (x > pos.x && x < pos.x + size.x && y > pos.y && y < pos.y + size.y ) {
				isTouched = true;
				isSelected = true;
				touch.set(x-pos.x, y-pos.y);
				return true;
			}
		return false;
	}
	void onTouchUp(int x, int y){
		isTouched = false;
		if (!(x > pos.x && x < pos.x + size.x && y > pos.y && y < pos.y + size.y))
			isSelected = false;
	}
	void onTouchMove(int x, int y){
		//si dans le rectangle
		if (ID != -1){
			if ( x-touch.x > bordureX && x+(size.x-touch.x) < ofGetWidth()-bordureX && y-touch.y > bordureY+50 && y+(size.y-touch.y) < ofGetHeight()-150){ 
			  // ofLogNotice()
			   if (isTouched) {
			   
			   		if ( (int)abs(x-touch.x - bordureX) % (int)(1.7*GRID_RATIO) < 50 && (int)abs(y - touch.y - bordureY - 50) % GRID_RATIO < 50 ){
			   		
			   			pos.set(x-touch.x - ((int)(x-touch.x - bordureX) % int(1.7*GRID_RATIO)), (y-touch.y) - ((int)(y-touch.y - bordureY - 50) % GRID_RATIO));
			   			//locationInGrid.set((int)((x-touch.x- bordureX)/(GRID_RATIO*1.7)-4), (int)((y-touch.y- bordureY - 50)/GRID_RATIO));
			   		}
			   		/*else
			   			pos.set(x-touch.x,y-touch.y);*/
			   	}
			}
		}
	}
	
	void onDoubleClick(int x, int y){
		if (x > pos.x && x < pos.x + size.x && y > pos.y && y < pos.y + size.y )
			isDeleted=true;
	}
	
	
	void setTypeSize(int typeSize){
		this->typeSize=typeSize;
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
	void setIsDeleted(bool isDeleted){
		this->isDeleted=isDeleted;
	}
	int getID(){
		return ID;
	}
	int getTypeSize(){
		return typeSize;
	}
	int getTypeMat(){
		return typeMat;
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