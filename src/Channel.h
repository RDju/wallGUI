#include <string>

#define CHANNEL_IMAGE_WIDTH (ofGetWidth() - 3.0*10.0/100*ofGetWidth())/3.0
#define CHANNEL_IMAGE_HEIGHT (ofGetHeight() - HEIGHT_BUTTONS*2 - 2.0*1.0/9*ofGetHeight() - 1.0/15 * ofGetHeight())/2.0

class Channel{
	public:
		ofImage channelImage;
		string title;
		string creator;
		string price;
		string description;
		int ID;
		int rate;
		
		ofTrueTypeFont titleFont;
		ofTrueTypeFont infoFont;
		
		
		Channel(string url, string title, string creator, int rate): title(title), creator(creator), rate(rate)
		{
			rate = 2;
			channelImage.loadImage(url);
			channelImage.resize(CHANNEL_IMAGE_WIDTH, CHANNEL_IMAGE_HEIGHT);
			titleFont.loadFont("OpenSans-Light.ttf", 0.033*ofGetHeight());
			infoFont.loadFont("OpenSans-Light.ttf", 0.02*ofGetHeight());
			price = "free";
		}

		void drawPreview(int x, int y){
			ofPushMatrix();
			ofPushStyle();
				ofSetColor(40);
				titleFont.drawString(title, x, y);
				ofSetColor(255);
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
						
						
			ofPopStyle();
			ofPopMatrix();
		}
		
		void drawPage(){
			
		}
};