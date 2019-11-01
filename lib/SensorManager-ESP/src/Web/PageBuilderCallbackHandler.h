#ifndef PageBuilderCallbackHandler_h
#define PageBuilderCallbackHandler_h

//#include "IOTAppStory.h"
//#include <ESPAsyncWebServer.h>               // https://github.com/me-no-dev/ESPAsyncWebServer
#include <Web\TabbedAsyncDecl.h>
#include <Web\InfoWebServer.h>


//typedef std::function<void(AsyncWebServerRequest *)>	WebRequestHandler;



class PageBuilderCallbackHandler {
private:
	String _id;
	String _description;
	void *_object;
	PageBuildCallback contentBuildCallback;
public:
	PageBuilderCallbackHandler(String id, String description, PageBuildCallback callback, void *object);

	
	String getId() { return _id; };
	String getDescription() { return _description; };
	
	String getContents() { return contentBuildCallback(_object); };

private:	
	String buildStyleSection_NA();
	String buildNavigationPanel_HEST();
	String buildScriptSection_HEST();
	
};
#endif //PageBuilderCallbackHandler_h
