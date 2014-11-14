#include <vector>

#define WIDTH_BUTTONS 315
#define HEIGHT_BUTTONS 50

class Button{
	private:
		ofPoint pos;
		vector<int> associatedPages;
		char* activName;
		char* passivName;
		bool isActiv, isTouched;
		int ID;
		
		
		
		
	public:
	ofTrueTypeFont font;
	
	
	Button(int ID, int x, int y, vector<int> associatedPages, char* activName, char* passivName): associatedPages(associatedPages), ID(ID)
	{
		pos.set(x, y, 0);
		isActiv = false;
	}
	
	
	void init() {
		font.loadFont("OpenSans-Light.ttf",25);
	}

	void update() {

	}
	
	void draw() {
		if (isTouched) ofFill();
		else ofNoFill();
		
		ofRect(pos.x, pos.y, WIDTH_BUTTONS, HEIGHT_BUTTONS);
		
		if (isActiv) font.drawString(activName,pos.x + WIDTH_BUTTONS/2-font.stringWidth(activName)/2,pos.y+font.getLineHeight()+HEIGHT_BUTTONS);
		else font.drawString(passivName,pos.x + WIDTH_BUTTONS/2-font.stringWidth(passivName)/2,pos.y+font.getLineHeight()+HEIGHT_BUTTONS);
	}
	
	void moveTo(int x, int y) {
		pos.set(x, y, 0);
	}
	
	
	
	void setPos(int x, int y){
		pos.set(x, y, 0);
	}
	void setActivName(char* activName){
		this->activName = activName;
	}
	void setPassivName(char* passivName){
		this->passivName = passivName;
	}
	void setAssociatedPages(vector<int> associatedPages){
		this->associatedPages = associatedPages;
	}
	void setIsActiv(bool isActiv){
		this->isActiv = isActiv;
	}


};
