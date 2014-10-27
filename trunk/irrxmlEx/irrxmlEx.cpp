// irrxmlEx.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <irrXML.h>
#include <string>
using namespace irr;
using namespace io;

#pragma comment(lib, "Irrlicht.lib")

void main()
{
	// create the reader using one of the factory functions
	IrrXMLReader* xml = createIrrXMLReader("config.xml");

	// strings for storing the data we want to get out of the file
	std::string modelFile;
	std::string messageText;
	std::string caption;

	// parse the file until end reached

	while (xml && xml->read())
	{
		switch (xml->getNodeType())
		{
		case EXN_TEXT:
			// in this xml file, the only text which
			// occurs is the messageText
			messageText = xml->getNodeData();
			break;
		case EXN_ELEMENT:
			if (!strcmp("model", xml->getNodeName()))
			{
				modelFile = xml->getAttributeValue("file");
				printf("file : %s\n", modelFile.c_str());
			}				
			else
			if (!strcmp("messageText", xml->getNodeName()))
			{
				caption = xml->getAttributeValue("caption");
				printf("caption : %s\n", caption.c_str());
			}
				
			break;
		}
	}

	// delete the xml parser after usage
	delete xml;
}