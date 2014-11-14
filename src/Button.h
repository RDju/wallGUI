#include <vector>
#include <string>

#define WIDTH_BUTTONS 0.245*ofGetWidth()
#define HEIGHT_BUTTONS 0.065*ofGetHeight()

class Button{
	private:
		char* name;
		ofPoint pos;
		int associatedPages;
		string activName;
		string passivName;
		bool isActiv, isTouched;
		int ID;
		ofTrueTypeFont font;
		
		
		
	public:
	
	
	
	Button(char* name, int ID, int x, int y, int associatedPages, string activName, string passivName): name(name), associatedPages(associatedPages), ID(ID), activName(activName), passivName(passivName)
	{
		pos.set(x, y, 0);
		isActiv = false;
		isTouched = false;
		font.loadFont("OpenSans-Light.ttf", 0.033*ofGetHeight());
	}

	void update() {

	}
	
	void draw() {
		if (isTouched) ofFill();
		else ofNoFill();
		//ofSetColor(0,0,0,100);
		ofRect(pos.x, pos.y, WIDTH_BUTTONS, HEIGHT_BUTTONS);
		
		if (isActiv) {
			 font.drawString(activName, pos.x + WIDTH_BUTTONS/2-font.stringWidth(activName)/2, pos.y+font.getLineHeight());
		} else {
			font.drawString(passivName, pos.x + WIDTH_BUTTONS/2-font.stringWidth(passivName)/2, pos.y+font.getLineHeight());
		}
	}
	
	void moveTo(int x, int y) {
		pos.set(x, y, 0);
	}
	
	bool isTouchedUp(int x, int y){
		if (x>pos.x && x < pos.x+WIDTH_BUTTONS && y > pos.y && y < pos.y+HEIGHT_BUTTONS) {
			if (isTouched) isActiv = !isActiv;
			isTouched = false;
			return true;
		} else {
			isTouched = false;
			return false;
		}
	}
	bool isTouchedDown(int x, int y){
		if (x>pos.x && x < pos.x+WIDTH_BUTTONS && y > pos.y && y < pos.y+HEIGHT_BUTTONS) {
			isTouched = true;
			return true;
		} else {
			isTouched = false;
			return false;
		}
	}
	
	
	
	void setPos(int x, int y){
		pos.set(x, y, 0);
	}
	void setActivName(string activName){
		this->activName = activName;
	}
	void setPassivName(string passivName){
		this->passivName = passivName;
	}
	void setAssociatedPages(int associatedPages){
		this->associatedPages = associatedPages;
	}
	void setIsActiv(bool isActiv){
		this->isActiv = isActiv;
	}
	
	int getAssociatedPages(){
		return associatedPages;
	}
	int getID(){
		return ID;
	}
	bool getIsActiv(){
		return isActiv;
	}


};
