#include "webRender.h"

void WebRender::setupWebcore(){
	awe_webcore_initialize_default();

    webView = awe_webcore_create_webview(WEB_WIDTH, WEB_HEIGHT, false);
	//awe_webview* webView = awe_webcore_create_webview(WIDTH, HEIGHT, false);
    awe_string* url_str = awe_string_create_from_ascii(URL, strlen(URL));

    awe_webview_load_url(webView, url_str, awe_string_empty(), awe_string_empty(), awe_string_empty());

    // Wait for or WebView to finish loading
    while(awe_webview_is_loading_page(webView))
    {
        awe_webcore_update();
    }

	renderBuffer = awe_webview_render(webView);

	

	pixelBuffer = new unsigned char [WEB_WIDTH*WEB_HEIGHT*4];
	rightPixelBuffer = new unsigned char [WEB_WIDTH*WEB_HEIGHT*3];
	texColor.allocate(WEB_WIDTH,WEB_HEIGHT,GL_RGB);
}

void WebRender::convertBGRAToRGB(unsigned char* bgraBuffer, unsigned char* rgbBuffer, int width, int height){
	for(int i = 0; i < width * height; i++)
    {
        rgbBuffer[i * 3 + 2] = bgraBuffer[i * 4 + 0];
        rgbBuffer[i * 3 + 1] = bgraBuffer[i * 4 + 1];
        rgbBuffer[i * 3 + 0] = bgraBuffer[i * 4 + 2];
    }
}

void WebRender::updateWebcore(){
	Sleep(50);
    awe_webcore_update();
	rPixelBuffer = awe_renderbuffer_get_buffer(renderBuffer);
	pixelBuffer = const_cast<unsigned char *> (rPixelBuffer);
	convertBGRAToRGB(pixelBuffer,rightPixelBuffer,WEB_WIDTH, WEB_HEIGHT);
	texColor.loadData(rightPixelBuffer, WEB_WIDTH, WEB_HEIGHT, GL_RGB);	
}

void WebRender::drawWebcore(){
	texColor.draw(1,1,400,300);
}

void WebRender:: injectMouseMoved(int x, int y){
	awe_webview_inject_mouse_move (webView,x,y); 
}
void WebRender::injectMouseDragged(int x, int y){
	awe_webview_inject_mouse_move (webView,x,y); 
}
void WebRender::injectMousePressed(){
	awe_webview_inject_mouse_down (webView,AWE_MB_LEFT);
}
void WebRender::injectMouseReleased(){
	awe_webview_inject_mouse_up (webView,AWE_MB_LEFT);
}