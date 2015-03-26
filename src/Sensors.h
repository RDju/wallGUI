		
//#include "ofxOpenCv.h"	
#include "ofBitmapFont.h"	
		
	class Sensors{
		
		public :
		
			ofTrueTypeFont font;

			ofBaseVideoDraws * video;
		
			ofxAruco aruco;
			ofImage board;
			ofImage marker;
			bool isValidated, isSaved;
			ofImage wallImage;
			vector<ofPoint*> markersPos; 
			
			ofVideoGrabber grabber;
			ofxCvGrayscaleImage gray, prevGray, diff;
			ofxCvFloatImage diffFloat;
			ofxCvContourFinder contourFinder;
			int w, h, totalArea;
			int whitePix, whitePixPercent;
			int lightVal;
			bool isOn;
			ofPoint sizeMarker;
			ofPoint markerRotation;
			
			int	sampleRate;
	
			float * rAudio;
			
			ofSoundStream soundStream;
			
			float sum;
			int prevSum;
			
			Sensors(ofBaseApp *app){
			
				ofDisableArbTex();
				
				font.loadFont("OpenSans-Regular.ttf", 30);
				
				string boardName = "boardConfiguration.yml";
				
				grabber.setPixelFormat(OF_PIXELS_MONO);
				grabber.setDeviceID(0);
				grabber.initGrabber(640, 480);
				
				
				/************** Detection de marqueurs ****************/
				video=&grabber;
				
				aruco.setup("intrinsics.int", grabber.getWidth(), grabber.getHeight(), boardName);
				aruco.getBoardImage(board.getPixelsRef());
				board.update();
				
				ofEnableAlphaBlending();
				
				ofPixels pixels;
				ofBitmapStringGetTextureRef().readToPixels(pixels);
				ofSaveImage(pixels,"font.bmp");
				
				isValidated=false;
				isSaved=false;
	
				/********************************************************/
	
				
				w = grabber.getWidth();
				h = grabber.getHeight();
				
				totalArea = w*h;
				
				gray.allocate(w, h);
				gray.setUseTexture(false);
				prevGray.allocate(w, h);
				prevGray.setUseTexture(false);
				diff.allocate(w, h);
				diff.setUseTexture(false);
				diffFloat.allocate(w, h);
				diffFloat.setUseTexture(false);
				
				whitePix = 0;
				prevSum = 0;
				
				//audio stuff
				sampleRate = 44100;
				rAudio = new float[256];
			
				soundStream.setup(app, 2,2, sampleRate,256, 4);
				
				isOn = false;
			}
			
			void drawMarker(float size, const ofColor & color){
				ofDrawAxis(size);
				ofPushMatrix();
				ofPushStyle();
					ofSetRectMode(OF_RECTMODE_CENTER); 
					
					
					ofRotateX(90);
					
					ofNoFill();
					ofSetColor(ofColor::red);
					ofRect(0, 0, size, size);
					
					ofFill();
					ofSetColor(ofColor::red, 50);
					ofRect(0, 0, size, size);
					
					ofSetRectMode(OF_RECTMODE_CORNER); 

				ofPopStyle();
				ofPopMatrix();
			}
			
			void update(ofxOscSender &oscOUT){
			
				video->update();
				
				
				
				if(video->isFrameNew() && !isValidated && !isSaved){
					aruco.detectBoards(video->getPixelsRef());
					
					double matrix[16];
					if (aruco.getNumMarkers() > 0){
						aruco.markers[0].glGetModelViewMatrix(matrix);
						//ofLogNotice("aruco") << " Rot : " << matrix[0] << " " << matrix[1] << " " << matrix[2] << " " << matrix[4] << " " << matrix[5] << " " << matrix[6] << " " << matrix[8] << " " << matrix[9] << " " << matrix[10];
						//sleep();
					}
					
				} else if (isValidated && !isSaved) {
					markersPos.clear();
					double matrixRot[16];
					for(int i=0;i<aruco.getNumMarkers();i++){
						
						double matrix[16];
						aruco.markers[i].glGetModelViewMatrix(matrix);
						
						matrixRot[0] += matrix[0];
						matrixRot[1] += matrix[1];
						matrixRot[2] += matrix[2];
						matrixRot[4] += matrix[4];
						matrixRot[5] += matrix[5];
						matrixRot[6] += matrix[6];
						matrixRot[8] += matrix[8];
						matrixRot[9] += matrix[9];
						matrixRot[10] += matrix[10];
			
						markersPos.push_back(new ofPoint(matrix[12], matrix[13], matrix[14]));
						//changement de repère
						
						
						//ofLogNotice("aruco") << " Rot : " << matrix[0] << " " << matrix[1] << " " << matrix[2] << " " << matrix[4] << " " << matrix[5] << " " << matrix[6] << " " << matrix[8] << " " << matrix[9] << " " << matrix[10];
						
					}
					for (int i = 0; i< 16; i++){
						matrixRot[i] / aruco.getNumMarkers(); 
					}
					for(int i=0;i<aruco.getNumMarkers();i++){
					
						double matrix[16];
						aruco.markers[i].glGetModelViewMatrix(matrix);
						//markersPos[i]->x = matrix[12] * matrixRot[5] + matrix[13] * matrixRot[6] /*+  matrix[14] * matrixRot[2]*/;
						//markersPos[i]->y = matrix[12] * matrixRot[9] + matrix[13] * matrixRot[10] /*+  matrix[14] * matrixRot[6]*/;
					//	markersPos[i]->z = matrix[12] * matrixRot[8] + matrix[13] * matrixRot[9] +  matrix[14] * matrixRot[10];
					}
					triMarkers();
					isSaved = true;
				}
			
				if (isOn) {
					ofxOscMessage m;
					grabber.update();
				
					if(grabber.isFrameNew()){
						
						gray.setFromPixels(grabber.getPixels(),w, h);
						diff.absDiff(prevGray, gray);
						diffFloat = diff;
						diffFloat *=1.5;
						whitePixelsNumber(diffFloat);
						prevGray = gray;
						
						m.clear();
						m.setAddress( "/whitePixPercent");
						m.addIntArg(whitePixPercent);
						oscOUT.sendMessage( m );
					}
					
					//audio stuff
					sum = 0;
					for(int i=0;i<256;i++){
				         sum=sum+(rAudio[i]*70.0f);
				    }
					sum = (int)abs((sum/256)*100);
					
					
					if (sum != prevSum){
						m.clear();
						m.setAddress( "/SOUND");
						m.addIntArg(sum);
						oscOUT.sendMessage( m );
					}
					prevSum = sum;
				}
			}
			
			void draw(){
				
				if (!isValidated)
					video->draw(0,0, ofGetWidth(), ofGetHeight()/*960*/);
				else if (!isSaved)
					wallImage.draw(0, 0);
				
				
				if (!isSaved){
					for(int i=0;i<aruco.getNumMarkers();i++){
						aruco.begin(i);
						drawMarker(0.15,ofColor::white);
						aruco.end();
					}
				} else {
					for(int i=0;i<markersPos.size();i++){
						ofPushStyle();
						ofPushMatrix();
							ofTranslate(100, 100, 0);
							ofSetColor(0);
							ofFill();
							ofRect(markersPos[i]->x, markersPos[i]->y, sizeMarker.x-10, sizeMarker.y-10);
							ofSetColor(255);
							font.drawString(ofToString(aruco.markers[i].idMarker), markersPos[i]->x + 30, markersPos[i]->y +sizeMarker.y /2 );
						ofPopMatrix();
						ofPopStyle();
					}
				}
			}
			
		
			void whitePixelsNumber(ofxCvFloatImage image){
	
				whitePix = 0;
				unsigned char * pixels = image.getPixels();	 
				for(int i = 0; i < image.width*image.height; i++){
					if (pixels[i] > 20){  
						whitePix ++;  
					}  
				}  
				whitePixPercent = (float)whitePix / (image.width*image.height) *100;
			}

			void audioReceived(float * input,int bufferSize,int nChannels){
				if (isOn)
					for (int i = 0; i < bufferSize; i++)
						rAudio[i%256] = input[i*nChannels ] + input[i*nChannels +1] ;
			}
			
			void oscLight(int val){
				lightVal = val;
			}	
			
			void onTouchUp(){
				if (!isValidated){
					wallImage.grabScreen(0,0,ofGetWidth(),ofGetHeight());
					wallImage.saveImage("wallImageMarkers.png");
					wallImage.loadImage("wallImageMarkers.png");
				}
				isValidated = !isValidated;
				isSaved= false;
			}
			
			
			void triMarkers(){
			
			
				float xmin = 100.0;
				float ymin = 100.0;
				float xmax = -100.0;
				float ymax = -100.0;
				
				
				//on récupère les tailles min et max -> taille du wall
				ofPoint wallSize;
				for(int i=0;i<markersPos.size()/*aruco.getNumMarkers()*/;i++){
					if (xmin > markersPos[i]->x) xmin = markersPos[i]->x;
					if (ymin > markersPos[i]->y) ymin = markersPos[i]->y;
					if (xmax < markersPos[i]->x) xmax = markersPos[i]->x;
					if (ymax < markersPos[i]->y) ymax = markersPos[i]->y;
				}
				
				//On déplace l'origine en haut à gauche
				xmax -= xmin;
				ymax -= ymin;
				for(int i=0;i<markersPos.size()/*aruco.getNumMarkers()*/;i++){
					markersPos[i]->x -= xmin;
					markersPos[i]->y -= ymin;
				}
				
												//TODO: pas viable si une seule ligne ou colonne...
												//Récupérer taille en m d'un carré
												/*float xValue = -100;
												float yValue = -100;
												sizeMarker.set(ofGetWidth(), ofGetHeight(), 0);
												for(int i=0;i<markersPos.size();i++){
													if (xValue == -100) {
														xValue = markersPos[i]->x;//coordonnées du premier marqueur du tableau (arbitraire)
														yValue = markersPos[i]->y;
													} else {
														if (abs(markersPos[i]->y - yValue) < 0.1 && abs(markersPos[i]->x - xValue) > 0.1){//mm ligne, colonne différente
															if (abs(markersPos[i]->x - xValue) < sizeMarker.x) // le plus petit écart
																sizeMarker.x = abs(markersPos[i]->x - xValue);	
														}
														if (abs(markersPos[i]->y - yValue) > 0.1 && abs(markersPos[i]->x - xValue) < 0.1){//mm ligne, colonne différente
															if (abs(markersPos[i]->y - yValue) < sizeMarker.y)
																sizeMarker.y = abs(markersPos[i]->y - yValue);	
														}
													}
												}*/
				
				sizeMarker.x = 0.15;
				sizeMarker.y = 0.15;
				
				xmax += sizeMarker.x;
				ymax += sizeMarker.y;
				
				int column = round(xmax/ sizeMarker.x);
				int row = round(ymax / sizeMarker.y);
				//ofLogNotice("aruco") << "column : " << xmax/ sizeMarker.x << " , row : " << ymax / sizeMarker.y;
				
				
				//conversion m -> px
				for(int i=0;i<markersPos.size();i++){
					if (xmax > ymax){
						markersPos[i]->x = round((ofGetHeight()-200)*markersPos[i]->x / ymax);
						markersPos[i]->y = round((ofGetHeight()-200)*markersPos[i]->y / ymax);
					} else {
						markersPos[i]->x = round((ofGetWidth()-200)*markersPos[i]->x / xmax);
						markersPos[i]->y = round((ofGetWidth()-200)*markersPos[i]->y / xmax);
					}
				}
				if (xmax > ymax){
					sizeMarker.x = round((ofGetHeight()-200)*sizeMarker.x / ymax);
					sizeMarker.y = round((ofGetHeight()-200)*sizeMarker.y / ymax);
				} else {
					sizeMarker.x = round((ofGetWidth()-200)*sizeMarker.x / xmax);
					sizeMarker.y = round((ofGetWidth()-200)*sizeMarker.y / xmax);
				}
				
												/*int column = round((ofGetWidth()-100+sizeMarker.x) / sizeMarker.x);
												int row = round((ofGetHeight()-100+sizeMarker.y) / sizeMarker.y);
												ofLogNotice("aruco") << "column : " << column << " , row : " << row;*/
				
				
				//on aligne les marqueurs
				/*for(int i=0;i<markersPos.size();i++){
					for (int j=0; j < column; j++ ){
						if (abs(markersPos[i]->y - sizeMarker.y*j) < 50){
							markersPos[i]->y = sizeMarker.y*j;
						}
					}
					for (int j=0; j < row; j++ ){
						if (abs(markersPos[i]->x - sizeMarker.x*j) < 50){
							markersPos[i]->x = sizeMarker.x*j;
						}
					}
				}*/
				
				//TODO: calcul de la position dans une grille
				
			}
	};