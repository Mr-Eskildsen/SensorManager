
#include <Arduino.h>

#include <functional>
#include "FS.h"

#define _TASK_STD_FUNCTION
#include <TaskScheduler.h>

#include <Sensor\BinarySensor.h>
#include <Web\InfoWebServer.h>






const char* wl_status_to_string(wl_status_t status) {
  switch (status) {
    case WL_NO_SHIELD: return "NO SHIELD";
    case WL_IDLE_STATUS: return "IDLE STATUS";
    case WL_NO_SSID_AVAIL: return "NO SSID  AVAIL";
    case WL_SCAN_COMPLETED: return "SCAN COMPLETED";
    case WL_CONNECTED: return "CONNECTED";
    case WL_CONNECT_FAILED: return "CONNECT FAILED";
    case WL_CONNECTION_LOST: return "CONNECTION LOST";
    case WL_DISCONNECTED: return "DISCONNECTED";
  }
}


InfoWebServer::InfoWebServer(uint16_t port) 
	: AsyncWebServer(port)
{

	pConfigSensorsTab = NULL;

	pRunner  = NULL;
	//TODO:: FIXC
	//pConfigSensorsTab = new InfoTabDropItem("sensors-config", "Configuration", NULL, NULL);
	
	tabItemCount = 0;
	lastUpdate = 0;

		for (int idx=0; idx<MAX_TAB_ITEMS; idx++)
			arrTabItems[idx] = NULL;
	

}

InfoWebServer::~InfoWebServer() 
{
	if (pRunner) {
		delete pRunner;
	}
}




void InfoWebServer::setup()
{
 	pinMode(D5, INPUT);     	
	Serial.println("[InfoWebServer] Starting....");
	
	
  if(!SPIFFS.begin()){
     Serial.println("An Error has occurred while mounting SPIFFS");
     return;
  }

	mapConnections = new SimpleMap<uint32_t, AsyncWebSocketClient *>([](uint32_t &a, uint32_t &b) -> int {
								if (a == b) return 0;      // a and b are equal
								else if (a > b) return 1;  // a is bigger than b
								else return -1;            // a is smaller than b
							});

	pConfigSensorsTab = new InfoTabDropItem("sensors-config", "Configuration", NULL, NULL);
	loadConfig();

    addTab(new InfoTabItem("status", "Status", InfoWebServer::buildStatusContents, this));
    addTab(new InfoTabItem("sensors", "Sensors", InfoWebServer::buildSensorsContents, this));
    if (pConfigSensorsTab) {
		addTab(pConfigSensorsTab);
	}
	addTab(new InfoTabItem("admin", "Administration", InfoWebServer::buildAdminContents, this));


	
	pWS = new AsyncWebSocket("/ws");
	pWS->onEvent(std::bind(&InfoWebServer::onWsEvent, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5, std::placeholders::_6));


	pRunner = new Scheduler();
	
	pRunner->init();
	Serial.println("[InfoWebServer] - Adding timer callback");
	std::function<void()> onTimerCallback = std::bind(&InfoWebServer::onTimer, this);
	pTask =  new Task(100, TASK_FOREVER, onTimerCallback);
	pRunner->addTask(*pTask);

	addHandler(pWS);


	// Configure the webserver
  	//rewrite("/", "/index.html");
  	on("/", HTTP_GET, std::bind(&InfoWebServer::onBuildPage, this, std::placeholders::_1));

	on("/favicon.ico", HTTP_GET, [](AsyncWebServerRequest *request)	{
		AsyncWebServerResponse *response = request->beginResponse_P(200, "image/x-icon", favicon_ico_gz, favicon_ico_gz_len);
		response->addHeader("Content-Encoding", "gzip");
		request->send(response);
	});

	on("/css/infoweb.css", HTTP_GET, [](AsyncWebServerRequest *request){
		request->send(SPIFFS, "/css/infoweb.css", "text/css"); 
	});
	
	on("/css/bootstrap-4.3.1.min.css", HTTP_GET, [](AsyncWebServerRequest *request){
		request->send(SPIFFS, "/css/bootstrap-4.3.1.min.css", "text/css"); 
	});

	on("/js/bootstrap-4.3.1.min.js", HTTP_GET, [](AsyncWebServerRequest *request){
		request->send(SPIFFS, "/js/bootstrap-4.3.1.min.js", "text/javascript");
	});
	
	on("/js/jquery-3.3.1.slim.min.js", HTTP_GET, [](AsyncWebServerRequest *request){
		request->send(SPIFFS, "/js/jquery-3.3.1.slim.min.js", "text/javascript");
	});
	
	on("/js/popper-1.4.7.min.js", HTTP_GET, [](AsyncWebServerRequest *request){
		request->send(SPIFFS, "/js/popper-1.4.7.min.js", "text/javascript");
	});



	onNotFound([](AsyncWebServerRequest *request){ request->send(404); });


	if(!SPIFFS.begin()){
    	Serial.println("[InfoWebServer] An Error has occurred while mounting SPIFFS");
     	return;
  	}	

	if (pRunner) {
		Serial.println("[InfoWebServer] Enable all tasks");
		pRunner->enableAll();
	}


	Serial.println("[InfoWebServer] 'setup' done");

}



void InfoWebServer::loadConfig()
{

	//TODO:: Dummy Load
	BinarySensor *pSensor1Config = new BinarySensor("pir1", "PIR Sensor 1", D5);
	BinarySensor *pSensor2Config = new BinarySensor("pir2", "PIR Sensor 2", D6);
	addSensor(new InfoTabItem(pSensor1Config));
	addSensor(new InfoTabItem(pSensor2Config));
//    server.addSensor(new InfoTabItem("pir", "PIR Sensor", sensorPir, NULL));
//    server.addSensor(new InfoTabItem("bme280", "BME280", sensorBME280, NULL));
	//delete pSensor1Config;
	//delete pSensor2Config;

}







void InfoWebServer::loop() {
	
	if (pRunner) {
		pRunner->execute();
	}
}


void InfoWebServer::onTimer()
{
	unsigned long curMillis = millis();
	//if(((lastUpdate+1000)<curMillis) && globalClient != NULL && globalClient->status() == WS_CONNECTED){
      String randomNumber = String(random(0,20));
      //globalClient->text(randomNumber);
	if( lastUpdate+5000<curMillis ){
		Serial.println("Updating from timer callback");
		for(int idx=0; idx<mapConnections->size();idx++) {
			AsyncWebSocketClient* conn = mapConnections->getData(idx);
			if (conn != NULL && conn->status() == WS_CONNECTED){
				int val = digitalRead(D5);
				if (val==0) {
					conn->text(String("Untriggered"));
				}
				else {
					conn->text(String("Triggered"));
				}
				//conn->text(randomNumber);
			}
		}

		lastUpdate = curMillis;
   }

}


void InfoWebServer::onWsEvent(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len){
 
  if(type == WS_EVT_CONNECT){
 
    //Serial.println("Websocket client connection received");
    Serial.print("Websocket client connection with id='");
    Serial.print(client->id());
    Serial.println("' received");
    
    //globalClient = client;
    //connectionHandlers.add(client);
    mapConnections->put(client->id(), client);
 
  } else if(type == WS_EVT_DISCONNECT){
    
    Serial.print("Websocket client connection with id='");
    Serial.print(client->id());
    Serial.println("' finished");
    //globalClient = NULL;

    //connectionHandlers.remove(client);
    mapConnections->remove(client->id());
  }
}

















void InfoWebServer::addSensor(InfoTabItem* pSensor) {
	if (pConfigSensorsTab) {
		pConfigSensorsTab->addDropItem(pSensor);
	}
}



//InfoWebPage& InfoWebServer::addPage(InfoWebPage* handler) {
void InfoWebServer::addTab(InfoTabItem* pItem) {
//	_pageHandlers.add(handler);
	if (tabItemCount<MAX_TAB_ITEMS) {
		arrTabItems[tabItemCount] = pItem;
		tabItemCount++;
	}
	//return *handler;
}

String InfoWebServer::buildSensorsContents(void *object)
{
	String response = "";
	InfoWebServer* pThis = (InfoWebServer*)object;
	pThis->addLine(response, "<table class=\"table\" cellspacing=\"0\">");
	pThis->addLine(response, "  <thead>");
	pThis->addLine(response, "  </thead>");
    
	
	pThis->addLine(response, "  <tbody>");

		pThis->addLine(response, "    <tr class=\"infoweb-group\">");
		
		pThis->append(response, "      <td colspan=2 class=\"infoweb-group\">A Sample</td>");
		pThis->addLine(response, "    </tr>");

		pThis->addLine(response, "    <tr>");
		pThis->addLine(response, "      <td>TODO:: Some text</td>");
		pThis->addLine(response, "      <td id=\"my-sampledata\"></td>");
		pThis->addLine(response, "    </tr>");

	
	for (uint32_t idx=0;idx<pThis->getSensorCount();idx++) {
		InfoTabItem*pSensor = pThis->GetSensorAt(idx);
		pThis->addLine(response, "    <tr class=\"infoweb-group\">");
		
		pThis->append(response, "      <td colspan=2 class=\"infoweb-group\">");
		pThis->append(response, pSensor->getDescription());
		pThis->addLine(response, "</td>");
		pThis->addLine(response, "    </tr>");

	//ROW 2
		pThis->addLine(response, "    <tr>");
		pThis->addLine(response, "      <td>TODO:: Some text</td>");
		pThis->addLine(response, "      <td></td>");
		pThis->addLine(response, "    </tr>");
	}
	pThis->addLine(response, "  </tbody>");
	pThis->addLine(response, "</table>");
	

	return response;
}

String InfoWebServer::buildStatusContents(void *object)
{
	String response = "";
	InfoWebServer* pThis = (InfoWebServer*)object;
	pThis->addLine(response, "<table class=\"table\" cellspacing=\"0\">");
	pThis->addLine(response, "  <thead>");
/*    
	pThis->addLine(response, "    <tr>");
	pThis->addLine(response, "      <th>Attribute</th>");
	pThis->addLine(response, "      <th>Value</th>");
	pThis->addLine(response, "    </tr>");
	*/
	pThis->addLine(response, "  </thead>");
    
	
	pThis->addLine(response, "  <tbody>");

	pThis->addLine(response, "    <tr class=\"infoweb-group\">");
	pThis->addLine(response, "      <td colspan=2 class=\"infoweb-group\">WIFI</td>");
	pThis->addLine(response, "    </tr>");

	//ROW 1
	pThis->addLine(response, "    <tr>");
	pThis->addLine(response, "      <td>WIFI SSID</td>");
	if (WiFi.isConnected())
		pThis->addLine(response, "      <td>" + String(WiFi.SSID()) + "</td>");
	else 
		pThis->addLine(response, "      <td><NOT CONNECTED></td>");
	pThis->addLine(response, "    </tr>");
	

	//ROW 2
	pThis->addLine(response, "    <tr>");
	pThis->addLine(response, "      <td>IP address</td>");
	pThis->addLine(response, "      <td>" + WiFi.localIP().toString() + "</td>");
	pThis->addLine(response, "    </tr>");
	
	//ROW 3
	pThis->addLine(response, "    <tr>");
	pThis->addLine(response, "      <td>WIFI Status</td>");
	pThis->addLine(response, "      <td>" + String(wl_status_to_string(WiFi.status())) + "  [" + String(WiFi.status()) + "]</td>");
	pThis->addLine(response, "    </tr>");

	pThis->addLine(response, "    <tr class=\"infoweb-group\">");
	pThis->addLine(response, "      <td colspan=2 class=\"infoweb-group\">ESP</td>");
	pThis->addLine(response, "    </tr>");

	//ROW 4
	pThis->addLine(response, "    <tr>");
	pThis->addLine(response, "      <td>FreeSkecthSpace</td>");
	pThis->addLine(response, "      <td>" + String(ESP.getFreeSketchSpace()) + "</td>");
	pThis->addLine(response, "    </tr>");



	response += "  <tr>";
	response += "    <td>Last Reset reason</td>";
	response += "    <td>" + String(ESP.getResetReason()) + "</td> ";
	response += "  </tr>";

	response += "  <tr>";
	response += "    <td>Sketch Size</td>";
	response += "    <td>" + String(ESP.getSketchSize()) + "</td> ";
	response += "  </tr>";

	response += "  <tr>";
	response += "    <td>Free Heap (Actual)</td>";
	response += "    <td>" + String(ESP.getFreeHeap()) + "</td> ";
	response += "  </tr>";


	pThis->addLine(response, "  </tbody>");
	pThis->addLine(response, "</table>");
	return response;
}



String  InfoWebServer::buildAdminContents(void *object)
{
	return "<h3>TODO:: Load data for #admin panel instead of dummy data</h3>";

}



void InfoWebServer::onBuildPage(AsyncWebServerRequest *request) {
	request->send(200, F("text/html"), buildPage());
}


String InfoWebServer::buildPage()
{
	String response = "";
	
	addLine(response, "<!DOCTYPE html>");

	addLine(response, "<html>");
	addLine(response, "<head>");
	addLine(response, "<style>");

	response.concat(buildStyleSection());

	addLine(response, "</style>");

	response.concat(buildHeadSection());

	addLine(response, "</head>\r\n");
	addLine(response, "<body>");
	addLine(response, "<section id=\"tabs\" class=\"infoweb-tab\">");
	addLine(response, "<div class=\"container\">");
	//response.concat("<h4>Hello World</h4>");
	response.concat(buildTabNavigation());
	
	response.concat(buildTabContentSection());
	
	addLine(response, "</div>");
	addLine(response, "</section>");
	addLine(response, "</body>");

	response.concat(buildScriptSection());
	
	
	addLine(response, "</html>");
	return response;
}

String InfoWebServer::buildHeadSection()
{
	String response = "";

	addLine(response, "<meta charset=\"utf-8\">");
	addLine(response, "<meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\">");
	addLine(response, "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
	addLine(response, "<meta name=\"description\" content=\"\">");
	addLine(response, "<meta name=\"author\" content=\"\">");
	
	addLine(response, "<!-- InfoWeb CSS -->");
	addLine(response, "<link rel=\"stylesheet\" href=\"css/infoweb.css\">" );
	
	addLine(response, "<!-- Boot strapp 4.x -->");
	addLine(response, "<link rel=\"stylesheet\" href=\"css/bootstrap-4.3.1.min.css\" >");
	

	addLine(response, "<script src=\"js/jquery-3.3.1.slim.min.js\"></script>");
	addLine(response, "<script src=\"js/popper-1.4.7.min.js\"></script>");
	addLine(response, "<script src=\"js/bootstrap-4.3.1.min.js\"></script>");
	
	//addLine(response, "<link rel=\"stylesheet\" href=\"https://stackpath.bootstrapcdn.com/bootstrap/4.3.1/css/bootstrap.min.css\" integrity=\"sha384-ggOyR0iXCbMQv3Xipma34MD+dH/1fQ784/j6cY/iJTQUOhcWr7x9JvoRxT2MZw1T\" crossorigin=\"anonymous\">");

	//addLine(response, "<script src=\"https://code.jquery.com/jquery-3.3.1.slim.min.js\" integrity=\"sha384-q8i/X+965DzO0rT7abK41JStQIAqVgRVzpbzo5smXKp4YfRvH+8abtTE1Pi6jizo\" crossorigin=\"anonymous\"></script>");
	//addLine(response, "<script src=\"https://cdnjs.cloudflare.com/ajax/libs/popper.js/1.14.7/umd/popper.min.js\" integrity=\"sha384-UO2eT0CpHqdSJQ6hJty5KVphtPhzWj9WO1clHTMGa3JDZwrnQq4sF86dIHNDz0W1\" crossorigin=\"anonymous\"></script>");
	//addLine(response, "<script src=\"https://stackpath.bootstrapcdn.com/bootstrap/4.3.1/js/bootstrap.min.js\" integrity=\"sha384-JjSmVgyd0p3pXB1rRibZUAYoIIy6OrQ6VrjIEaFf/nJGzIxFDsf4x0xIM+B07jRM\" crossorigin=\"anonymous\"></script>");
	return response;

}
String InfoWebServer::generateTabDropItem(InfoTabItem* pItem) 
{
	String item = "";
	if (pItem!=NULL) {
		append(item, "<a data-toggle=\"tab\" class=\"dropdown-item\" href=\"#");
		append(item, pItem->getId());
		append(item, "\">");
		append(item, pItem->getDescription());
		addLine(item, "</a>");
	}
	return item;

}

String InfoWebServer::generateTabItem(InfoTabItem* pItem, bool isActive) 
{
	String item = "";
	if (pItem!=NULL) {
		append(item, "<li class=\"nav-item\"><a href=\"#");
		append(item, pItem->getId());
		
		append(item, "\" data-toggle=\"tab\" class=\"nav-link");
		if (isActive) {
		 	append(item, " active");
		}
		append(item, "\">");

		append(item, pItem->getDescription());
		addLine(item, "</a></li>");
	}
	return item;
}

String InfoWebServer::buildTabNavigation()
{
	String response = "";
	response.concat("<div id=\"menu-content\">\r\n");
	response.concat("<ul  class=\"nav nav-tabs nav-justified\">\r\n");



	bool active = true;
	//for(LinkedList<InfoTabItem *, LinkedListNode>::ConstIterator tabItemIt = _pageHandlers.begin(); tabItemIt!=_pageHandlers.end(); tabItemIt++)
	for(int idx=0; idx<tabItemCount; idx++) {
		
		InfoTabItem* pTabItem = arrTabItems[idx];
		if (pTabItem!=NULL) {

			if (pTabItem->isDropItem()) {
				
 				response.concat("<li class=\"nav-item dropdown\">\r\n");

				response.concat("<a class=\"nav-link dropdown-toggle\" id=\"btnSensors\" data-toggle=\"dropdown\" href=\"#\" role=\"button\" aria-haspopup=\"true\" aria-expanded=\"false\">");
				response.concat(pTabItem->getDescription());
				response.concat("</a>\r\n");

				response.concat("<div class=\"dropdown-menu\">\r\n");

				response.concat("<h6 class=\"dropdown-header\">");
				response.concat(pTabItem->getDescription());
				response.concat("</h6>\r\n");

				response.concat("<div class=\"dropdown-divider\"></div>\r\n");

				for(int idxSub = 0; idxSub<pTabItem->GetDropItemCount();idxSub++) {
					response.concat(generateTabDropItem(pTabItem->GetDropItemAt(idxSub)));
				}
				
				response.concat("</div>\r\n");
				response.concat("</li>\r\n");
			}
			else {
				response.concat(generateTabItem(pTabItem, active));
			}
			active = false;
		}
	
	}
    response.concat("</ul>\r\n");
	response.concat("</div>\r\n");
	return response;
}




String InfoWebServer::buildTabContentSection()
{
	String response = "";
	bool active = true;
	addLine(response, "");
	addLine(response, "<!-- CONTENTS ON TABPAGES -->");
	addLine(response, "<div id=\"my-tab-content\" class=\"tab-content\">");

	for(int idx=0; idx<tabItemCount; idx++) {
		
		InfoTabItem* pItem = arrTabItems[idx];
		addLine(response, "");
		addLine(response, buildSpecificTabContent(pItem, active));
		addLine(response, "");
		active = false;
		if (pItem->isDropItem()) {
			for(int idx1=0; idx1<pItem->GetDropItemCount(); idx1++) {
				addLine(response, "");
				addLine(response, buildSpecificTabContent(pItem->GetDropItemAt(idx1), active));
				addLine(response, "");
			}
		}
	}

	addLine(response, "</div>");
            

	return response;
}

String InfoWebServer::buildSpecificTabContent(InfoTabItem* pItem, bool isActive)
{
	String item = "";
	append(item, "<div class=\"tab-pane container fade");
	if (isActive)
		append(item, " show active\"");
	append(item, "\" id=\"");
	append(item, pItem->getId());
	addLine(item, "\">");

	addLine(item, pItem->getContents());

	addLine(item, "</div>");
	return item;
}

String InfoWebServer::buildStyleSection()
{
	return "";
}

String InfoWebServer::buildContentSection() {
	String response = "";
	return response;
}

String InfoWebServer::buildScriptSection()
{
		String response = "";
	addLine(response, "<script>");

	addLine(response, "// Update menu with selected sensor name");
    addLine(response, "$(\".dropdown-menu a\").click(function(){");
    addLine(response, "$(\"#btnSensors\").text($(this).text());");
    addLine(response, "});");



	addLine(response, "</script>");	
	
	
	addLine(response, "<script type = \"text/javascript\">");
	append(response, "  var ws = new WebSocket(\"ws://");
	append(response, WiFi.localIP().toString() );
	addLine(response, "/ws\");");
 
    addLine(response, "  ws.onopen = function() {");
	addLine(response, "    window.alert(\"Connected\");");
	addLine(response, "};");
 
	addLine(response, "ws.onmessage = function(evt) {");
	addLine(response, "    document.getElementById(\"my-sampledata\").innerHTML  = \"temperature: \" + evt.data + \" C\";");
	addLine(response, "};");
	
	addLine(response, "</script");
	return response;
}



void InfoWebServer::addLine(String& html, String line)
{

	html.concat(line);
	html.concat("\r\n");
}

void InfoWebServer::append(String& html, String line)
{
	html.concat(line);
}

