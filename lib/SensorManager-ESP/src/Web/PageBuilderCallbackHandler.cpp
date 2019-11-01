#include <Arduino.h>

//#include "IOTAppStory.h"
#include <ESPAsyncWebServer.h>

#include <Web\PageBuilderCallbackHandler.h>
	

PageBuilderCallbackHandler::PageBuilderCallbackHandler(String id, String description, PageBuildCallback callback, void *object)
{

	//server = srv;
	_id = id;
	_description = description;
	contentBuildCallback = callback;
	_object = object;

}





String PageBuilderCallbackHandler::buildStyleSection_NA()
{
	String response = "\r\n";
		
	response.concat("body{ font - family: Arial; }\r\n");
	response.concat("\r\n");
	response.concat("/* Style the tab */\r\n");
	response.concat(".tab {\r\n");
	response.concat("  overflow: hidden;\r\n");
	response.concat("  border: 1px solid #ccc;\r\n");
	response.concat("  background - color: #f1f1f1;\r\n");
	response.concat("}\r\n");

	response.concat("\r\n");
	response.concat("/* Style the buttons inside the tab */\r\n");
	response.concat(".tab button{ \r\n");
	response.concat("  background - color: inherit;\r\n");
	response.concat("  float: left;\r\n");
	response.concat("  border: none;\r\n");
	response.concat("  outline: none;\r\n");
	response.concat("  cursor: pointer;\r\n");
	response.concat("  padding: 14px 16px;\r\n");
	response.concat("  transition: 0.3s;\r\n");
	response.concat("  font - size: 17px;\r\n");
	response.concat("}\r\n");

	response.concat("\r\n");
	response.concat("/* Change background color of buttons on hover */\r\n");
	response.concat(".tab button : hover{ \r\n");
	response.concat("  background - color: #ddd;\r\n");
	response.concat("}\r\n");

	response.concat("\r\n");
	response.concat("/* Create an active/current tablink class */\r\n");
	response.concat(".tab button.active{ \r\n");
	response.concat("  background - color: #ccc;\r\n");
	response.concat("}\r\n");

	response.concat("\r\n");
	response.concat("/* Style the tab content */\r\n");
	response.concat(".tabcontent{ \r\n");
	response.concat("  display: none;\r\n");
	response.concat("  padding: 6px 12px;\r\n");
	response.concat("  border: 1px solid #ccc;\r\n");
	response.concat("  border - top: none;\r\n");
	response.concat("}\r\n");
	response.concat("\r\n");

	return response;
}

String PageBuilderCallbackHandler::buildNavigationPanel_HEST() {
	String response = "\r\n";
	response.concat("<div class=\"tab\">\r\n");
	response.concat("	<button class=\"tablinks\" onclick=\"openCity(event, \'London\')\" id = \"defaultOpen\">London</button>\r\n");
	response.concat("	<button class=\"tablinks\" onclick=\"openCity(event, \'Paris\')\">Paris</button>\r\n");
	response.concat("	<button class=\"tablinks\" onclick=\"openCity(event, \'Tokyo\')\">Tokyo</button>\r\n");
	response.concat("</div>\r\n");

	response.concat("\r\n");
	response.concat("<div id=\"London\" class=\"tabcontent\">\r\n");
	response.concat("	<h3>London</h3>\r\n");
	response.concat("	<p>London is the capital city of England.</p>\r\n");
	response.concat("</div>\r\n");

	response.concat("\r\n");
	response.concat("<div id=\"Paris\" class = \"tabcontent\">\r\n");
	response.concat("	<h3>Paris</h3>\r\n");
	response.concat("	<p>Paris is the capital of France.</p>\r\n");
	response.concat("</div>\r\n");

	response.concat("\r\n");
	response.concat("<div id=\"Tokyo\" class=\"tabcontent\">\r\n");
	response.concat("   <h3>Tokyo</h3>\r\n");
	response.concat("	<p>Tokyo is the capital of Japan.</p>\r\n");
	response.concat("</div>\r\n");
	return response;
}


String PageBuilderCallbackHandler::buildScriptSection_HEST()
{
	String response = "\r\n";
	response.concat("<script>\r\n");
	response.concat("	function openCity(evt, cityName) {\r\n");
	response.concat("	var i, tabcontent, tablinks;\r\n");
	response.concat("	tabcontent = document.getElementsByClassName(\"tabcontent\");\r\n");
	response.concat("	for (i = 0; i < tabcontent.length; i++) {\r\n");
	response.concat("		tabcontent[i].style.display=\"none\";\r\n");
	response.concat("	}\r\n");
	response.concat("	tablinks = document.getElementsByClassName(\"tablinks\");\r\n");
	response.concat("	for (i = 0; i < tablinks.length; i++) {\r\n");
	response.concat("		tablinks[i].className = tablinks[i].className.replace(\"active\", \"\");\r\n");
	response.concat("	}\r\n");
	response.concat("	document.getElementById(cityName).style.display = \"block\";\r\n");
	response.concat("	evt.currentTarget.className += \"active\";\r\n");
	response.concat("}\r\n");

	response.concat("\r\n");
	response.concat("// Get the element with id=\"defaultOpen\" and click on it\r\n");
	response.concat("document.getElementById(\"defaultOpen\").click();\r\n");
	response.concat("</script>\r\n");

	return response;
}
