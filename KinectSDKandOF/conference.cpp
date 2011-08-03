#include "conference.h"


struct conferenceManager {
		applicationState appState;
} conference;

void conference_init() {
	conference.appState = IDLE;
}
/*
void conference_update() {
printf(audio_direction);
}*/

/*
void conference_update() {
	if (conference.appState == IDLE) {
		// do nothing; everything should be blurry
	} else if (conference.appState == SPEAKING) {
		//focus in on the speaker
	}
}
*/

//void conferenceManager::focusRGB(BYTE* videoBuff, USHORT* depthBuff, BYTE * focusBuff, BYTE* blurBuff, KinectGrabber kinectGrabber) {
//void testApp::focusRGB(BYTE* videoBuff, USHORT* depthBuff, BYTE * focusBuff, BYTE* blurBuff) {
void focusRGB(BYTE* videoBuff, USHORT* depthBuff, BYTE * focusBuff, BYTE* blurBuff, KinectGrabber* kinectGrabber) {

	if (videoBuff && depthBuff) {
		LONG* pcolorx = new LONG();
		LONG* pcolory = new LONG();
		int max_index = DEPTH_WIDTH * DEPTH_HEIGHT * 4;

		for( int y = 0 ; y < DEPTH_HEIGHT ; y++ ){
			for( int x = 0 ; x < DEPTH_WIDTH ; x++ ) {

				kinectGrabber->Kinect_ColorFromDepth(x, y, pcolorx, pcolory);
				//NuiImageGetColorPixelCoordinatesFromDepthPixel(NUI_IMAGE_RESOLUTION_640x480, NULL, 
				//	LONG(x/2), LONG(y/2), m_depthBuffer[y*DEPTH_WIDTH + x] << 3, pcolorx, pcolory); 		
				int index = (y * DEPTH_WIDTH) + x;
				int	color_index = ((*pcolory*VIDEO_WIDTH) + *pcolorx);
				
				
				focusBuff[4*index + 0] = videoBuff[4*color_index + 0]; //focusBuff points to the r,g,b video stream
				focusBuff[4*index + 1] = videoBuff[4*color_index + 1];
				focusBuff[4*index + 2] = videoBuff[4*color_index + 2];
				focusBuff[4*index + 3] = 255;

				
				blurBuff[4*index + 0] = 0; //blurBuff points to the top layer with blur effect
				blurBuff[4*index + 1] = 0;
				blurBuff[4*index + 2] = 0;
				
				//if that pixel is not in the correct depth, make the focused image invisible (alpha = 0)
				//otherwise, set it so that it is visible (alpha = 255)
				int headPositionZ = kinectGrabber->headJoints_z;
				if (depthBuff[index] > headPositionZ + DEPTH_THRESHOLD  || depthBuff[index] < headPositionZ - DEPTH_THRESHOLD ) {
				//if (depthBuff[index] >  DEPTH_THRESHOLD  || depthBuff[index] < DEPTH_THRESHOLD ) {
					blurBuff[4*index + 3] = 255;
				} else {
					blurBuff[4*index + 3] = 0;
				}
			}
		}  
		free(pcolorx);
		free(pcolory);
	  }
}


/*
void testApp::highlightRGB(BYTE* videoBuff, USHORT* playerBuff, BYTE * highlightBuff, BYTE* overBuff) {
	  if (videoBuff && playerBuff) {
		LONG* pcolorx = new LONG();
		LONG* pcolory = new LONG();
		int max_index = DEPTH_WIDTH * DEPTH_HEIGHT * 4;

		for( int y = 0 ; y < DEPTH_HEIGHT ; y++ ){
			for( int x = 0 ; x < DEPTH_WIDTH ; x++ ) {
	
				g_kinectGrabber.Kinect_ColorFromDepth(x, y, pcolorx, pcolory);
				int index = (y * DEPTH_WIDTH) + x;
				int	color_index = ((*pcolory*VIDEO_WIDTH) + *pcolorx);
				//printf("depth (%d, %d)  color (%li, %li) \n", x, y, *pcolorx, *pcolory);
				//printf("index: %d color index: %d \n", index, color_index);
				
				//bool show = isInPlayerBound(index, playerBuff, max_index);

				highlightBuff[4*index + 0] = videoBuff[4*color_index + 0];
				highlightBuff[4*index + 1] = videoBuff[4*color_index + 1];
				highlightBuff[4*index + 2] = videoBuff[4*color_index + 2];
				highlightBuff[4*index + 3] = 255;
				overBuff[4*index + 0] = 0;
				overBuff[4*index + 1] = 0;
				overBuff[4*index + 2] = 0;
				//if that pixel does not belong to a player,  black it out
				//otherwise, leave its rgb values in tact
				if (playerBuff[index]) {
					overBuff[4*index + 3] = 0;
				} else {
					overBuff[4*index + 3] = 255;
				}
			}
		}  
		free(pcolorx);
		free(pcolory);
	  }
}
*/

//void testApp::adjustOver(int range, BYTE * overBuff) {
//	void conferenceManager::adjustOver(int range, BYTE * overBuff) {
	void adjustOver(int range, BYTE * overBuff) {

	BYTE * new_alpha_buff;
	new_alpha_buff = (BYTE*) malloc (DEPTH_WIDTH*DEPTH_HEIGHT*sizeof(BYTE));

	for( int y = 0 ; y < DEPTH_HEIGHT ; y++ ){
			for( int x = 0 ; x < DEPTH_WIDTH ; x++ ) {
				
				int index = (y*DEPTH_WIDTH) + x;
	
				int sum = 0;
				int divisors = 0;
				
				for (int m = 0; m < range; m ++) {
					int w_offset = m * DEPTH_WIDTH;
					for (int n = 0; n < range; n ++) {
						int new_index = index - w_offset - n;
						if (new_index > 0) {
							sum += overBuff[4*(new_index) +3];
							divisors ++;
						}
						
						new_index = index + w_offset + n;
						if (new_index <  DEPTH_WIDTH* DEPTH_HEIGHT) {
							sum += overBuff[4*(new_index) +3];
							divisors ++;
						}

						new_index = index - w_offset + n;
						if (new_index > 0 && new_index <  DEPTH_WIDTH* DEPTH_HEIGHT) {
							sum += overBuff[4*(new_index) +3];
							divisors ++;
						}
				
						new_index = index + w_offset - n;
						if (new_index > 0 && new_index <  DEPTH_WIDTH* DEPTH_HEIGHT) {
							sum += overBuff[4*(new_index) +3];
							divisors ++;
						}

					}
				}
				/*
				int new_index = index - DEPTH_WIDTH;
				if (new_index > 0) {
					sum += overBuff[4*(new_index) +3];
					divisors ++;
				}
				new_index = index + DEPTH_WIDTH;
				if (new_index < DEPTH_WIDTH * DEPTH_HEIGHT) {
					sum += overBuff[4*(new_index) +3];
					divisors ++;
				}
				new_index = index - 1;
				if (new_index > 0) {
					sum += overBuff[4*(new_index) +3];
					divisors ++;
				}
				new_index = index +1;
				if (new_index <  DEPTH_WIDTH* DEPTH_HEIGHT) {
					sum += overBuff[4*(new_index) +3];
					divisors ++;
				}
				*/
				new_alpha_buff[index] = (unsigned char)(sum / divisors);
			}
	}
	
	
	for( int y = 0 ; y < DEPTH_HEIGHT ; y++ ){
			for( int x = 0 ; x < DEPTH_WIDTH ; x++ ) {
				overBuff[4 * ((y*DEPTH_WIDTH) + x) + 3] = new_alpha_buff[(y*DEPTH_WIDTH) + x];
			}
	}

	free (new_alpha_buff);
}

/*
void testApp::adjustOver(int range, BYTE * overBuff) {
	// states:
	// 0 - have not seen a player pixel
	// 1 - scanning a player pixel
	// 2 - have not seen a player pixel, but no longer scanning a  player pixel
	int state = 0;

	for( int y = 0 ; y < DEPTH_HEIGHT ; y++ ){
			for( int x = 0 ; x < DEPTH_WIDTH ; x++ ) {
				
				int index = (y*DEPTH_WIDTH) + x;
	
				if (overBuff[4*index + 3] == 255) {
					
					/*for (int i = 0; i <= range; i++ ) {
						//if (y + i < DEPTH_HEIGHT)
						//	overBuff[(index + (DEPTH_WIDTH*i)) * 4 + 3] =  min(i * (255 / range), (int)overBuff[(index+ (DEPTH_WIDTH*i)) * 4 + 3]); 

						if (y-i > 0)
							overBuff[(index - (DEPTH_WIDTH*i)) * 4 + 3] =  min(i * (255 / range), (int)overBuff[(index- (DEPTH_WIDTH*i)) * 4 + 3]);
					*/
					/*
					if (state == 0) {
						//do blending on left
						for (int i = 0; i < range; i++ ) {
							if (x - i >= 0)
								overBuff[(index - i) * 4 + 3] = max(255 - i * (255 / range), (int)overBuff[(index- i) * 4 + 3]); 
						}
						state = 1;
					} else if (state == 1) {
						// the pixel is part of a person, make the overlay transparent
						overBuff[4*index + 3] = 0;
						state = 1;
					} else if (state == 2) {
						overBuff[4*index + 3] = 0;
						state = 1;
					}					
					
				} else if (overBuff[index*4 + 3] != 0) {
					
					if (state == 0) {
						// we haven't seen a person yet, keep the overlay
						state = 0;
					
					} else if (state == 1) {
						// blend on right
						for (int i = 0; i <= range; i++ ) {
							if (x + i <= DEPTH_WIDTH)
								overBuff[(index + i) * 4 + 3] = max(255 - i * (255 / range), (int)overBuff[(index + i) * 4 + 3]); 
						}
						state = 2;

					} else if (state == 2) {
						if (x == DEPTH_WIDTH - 1) {
							state = 0;
						} else {
							state = 2;
						}
					}
				}
				
			
			}
	}

}*/
