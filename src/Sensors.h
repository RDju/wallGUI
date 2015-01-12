		
//#include "ofxOpenCv.h"		
		
	class Sensors{
		
		public :
			
			ofVideoGrabber grabber;
			ofxCvGrayscaleImage gray, prevGray, diff;
			ofxCvFloatImage diffFloat;
			ofxCvContourFinder contourFinder;
			int w, h, totalArea;
			int whitePix, whitePixPercent;
			int lightVal;
			bool isOn;
			
			int	sampleRate;
	
			float * rAudio;
			
			ofSoundStream soundStream;
			
			float sum;
			int prevSum;
			
			Sensors(ofBaseApp *app){
			
				ofDisableArbTex();
				
				grabber.setPixelFormat(OF_PIXELS_MONO);
				grabber.setDeviceID(1);
				grabber.initGrabber(640,480);
				
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
			
			
			void update(ofxOscSender &oscOUT){
				if (isOn) {
					ofLogNotice("sensors") << "update";
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
	};