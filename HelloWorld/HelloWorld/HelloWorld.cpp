#include <iostream>
#include <sstream>
#include <list>
#include "LogInfoTool.h"
#include "RWLock.h"
#include "..\\Libs\\tinyxml.h"

#ifdef _DEBUG
#pragma comment(lib,"..\\Libs\\Debug\\TinyXml.lib")
#define LOG_FILE_PATH "../logs/shuanglongDebugInfo.log"
#else
#define LOG_FILE_PATH "MyLogs/shuanglongDebugInfo.log"
#pragma comment(lib,"..\\Libs\\Release\\TinyXml.lib")
#endif

using namespace std;

void ReadProtocolMapPanelConfig(string userID, LogInfoTool* pLog=NULL);
void WriteProtocolMapPanelConfig(string userID, const list<string> requestStr, LogInfoTool* pLog=NULL);
void addProtocolMapPanelSelectData(TiXmlDocument xmlDoc, const list<string> requestStr, TiXmlElement *pElement, LogInfoTool* pLog=NULL);
void addProtocolMapPanelUserData(TiXmlDocument xmlDoc, string userID, const list<string> requestStr, TiXmlElement *pRoot, LogInfoTool* pLog = NULL);

TiXmlElement* getUserElement(string userID, TiXmlElement* pRoot, LogInfoTool* pLog=NULL);
void ReadNpcMovableFlag(string userID, LogInfoTool* pLog = NULL);
void WriteNpcMovableFlag(string userID, const list<string> requestStr, LogInfoTool* pLog = NULL);

int main()
{
	FILE* logFile = fopen(LOG_FILE_PATH,"at+");
	LogInfoTool log;
	log.SetLogFile(logFile);

	//cout << "Set-Cookie:UserID=XYZ;\r\n";
	//cout << "Set-Cookie:Password=XYZ123;\r\n";
	//cout << "Set-Cookie:Domain=www.tutorialspoint.com;\r\n";
	//cout << "Set-Cookie:Path=/perl;\n";

	//cout << "<html>\n";
	//cout << "<head>\n";
	//cout << "<title>Cookies in CGI</title>\n";
	//cout << "</head>\n";
	//cout << "<body>\n";

	//cout << "Setting cookies" << endl;

	//cout << "<br/>\n";
	//cout << "</body>\n";
	//cout << "</html>\n";
	cout << "Content-type:text/html\r\n\r\n";

	// 客户端参数测试
	string requestMethod;
	char *pData=NULL;
	requestMethod = getenv("REQUEST_METHOD");
	if (requestMethod == "GET")
	{
		pData = getenv("QUERY_STRING");
	}
	else if (requestMethod == "POST")
	{
		char *tempLen = getenv("CONTENT_LENGTH");
		pData = new char[atoi(tempLen) + 1];
		cin >> pData;
	}

	// 请求参数转存
	string requestParam(pData);
	list<string> strParmList;
	istringstream istr(requestParam);
	int spanIndex = 0;
	string tempStr;
	
	log.WriteLogInfo(requestParam);
	if (requestParam.length() <= 0)
	{
		fclose(logFile);
		return 0;
	}

	do 
	{
		spanIndex = requestParam.find_first_of("&");
		if (spanIndex == string::npos)
		{
			tempStr = requestParam.substr(0, requestParam.length());
			requestParam = requestParam.erase(0, requestParam.length());
		} 
		else
		{
			tempStr = requestParam.substr(0, spanIndex);
			requestParam = requestParam.erase(0, spanIndex + 1);
		}
		strParmList.push_back(tempStr);

	} while (requestParam.length()>0 && spanIndex!=string::npos);

	strParmList.sort();

	// 处理编号
	string tempHandlerStr = strParmList.front();
	strParmList.pop_front();
	// 用户ID
	string tempUserIdStr = strParmList.front();
	strParmList.pop_front();

	log.WriteLogInfo(tempHandlerStr + string("  <-------->  ") + tempUserIdStr);
	spanIndex = tempHandlerStr.find_first_of("=");
	if (spanIndex != string::npos)
	{
		string handleIdxStr = tempHandlerStr.substr(spanIndex + 1, handleIdxStr.length() - spanIndex - 1);// 处理编号
		int handleIdx = atoi(handleIdxStr.c_str());

		spanIndex = tempUserIdStr.find_first_of("=");
		string userID = tempUserIdStr.substr(spanIndex + 1, tempUserIdStr.length() - spanIndex - 1);// 用户ID

		log.WriteLogInfo("============ Begin Switching ============");
		switch (handleIdx)
		{
		case 1://写 面板、地图、协议
			WriteProtocolMapPanelConfig(userID, strParmList, &log);
			break;
		case 2://读 面板、地图、协议
			ReadProtocolMapPanelConfig(userID, &log);
			break;
		case 3://写 移动场景NPC
			WriteNpcMovableFlag(userID, strParmList, &log);
			break;
		case 4://读 移动场景NPC
			ReadNpcMovableFlag(userID, &log);
			break;
		default:
			cout << "<!DOCTYPE html>\n"
				"<html>\n"
				"<head>\n"
				"<title>Welcome to CGI testing!</title>\n"
				"<style>\n"
				"body{\n"
				"color:#00ff00;\n"
				"background-color:#000000;\n"
				"}\n"
				"</style>\n"
				"</head>\n"
				"<body>\n"
				"<h1>Welcome to seer cgi test</h1>\n"
				"<p>If you see this page, you have transformed nothing to server.</p>\n"
				"</body>\n"
				"</html>";
			break;
		}
		log.WriteLogInfo("============  End Switching  ============");
	}

	fclose(logFile);
	return 0;
}

void ReadProtocolMapPanelConfig(string userID, LogInfoTool* pLog)
{
	if (pLog != NULL)
	{
		pLog->WriteLogInfo("         inside read protocol map panel");
	}
	TiXmlDocument configXml("DebugToolConfig.xml");
	configXml.LoadFile();
	TiXmlElement *pRoot = configXml.RootElement();
	if (pRoot == NULL)
	{
		if (pLog != NULL)
		{
			pLog->WriteLogInfo("    what !!!   pRoot==NULL");
		}
		return;
	}

	TiXmlElement *pProtocol = pRoot->FirstChildElement("ProtocolMapPanel");
	while (pProtocol)
	{
		if (userID == string(pProtocol->Attribute("userID")))
		{
			break;
		}
		else
		{
			pProtocol = pProtocol->NextSiblingElement();
		}
	}

	if (pProtocol == NULL)
	{
		if (pLog != NULL)
		{
			pLog->WriteLogInfo(string("    userData is not exist  userID=")+userID);
		}
		return;
	}

	TiXmlElement *pElement = pProtocol->FirstChildElement();
	string tempStr = "p0=1";
	//stringstream ss;

	while (pElement)
	{
		tempStr.append("&p");
		tempStr.append(pElement->Attribute("id"));
		tempStr.append("=");
		tempStr.append(pElement->Attribute("status"));

		pElement = pElement->NextSiblingElement();
	}
	cout << tempStr.c_str();
}

void WriteProtocolMapPanelConfig(string userID, const list<string> requestStr, LogInfoTool* pLog)
{
	//if (pLog != NULL)
	//{
	//	pLog->WriteLogInfo("         inside write protocol map panel");
	//}
	TiXmlDocument configXml("DebugToolConfig.xml");
	configXml.LoadFile();
	TiXmlElement *pRoot = configXml.RootElement();
	if (pRoot == NULL)
	{
		return;
	}

	TiXmlElement *pProtocol = pRoot->FirstChildElement("ProtocolMapPanel");
	while (pProtocol)
	{
		if (userID == string(pProtocol->Attribute("userID")))
		{
			break;
		}
		else
		{
			pProtocol = pProtocol->NextSiblingElement();
		}
	}

	if (pProtocol != NULL)
	{
		addProtocolMapPanelSelectData(configXml, requestStr, pProtocol, pLog);
	} 
	else
	{
		addProtocolMapPanelUserData(configXml, userID, requestStr, pRoot, pLog);
	}

	configXml.SaveFile();
}

void addProtocolMapPanelSelectData(TiXmlDocument xmlDoc, const list<string> requestStr, TiXmlElement *pProtocol, LogInfoTool* pLog)
{
	//if (pLog != NULL)
	//{
	//	pLog->WriteLogInfo("         inside addProtocolMapPanelSelectData() ");
	//}

	TiXmlElement *pElement = pProtocol->FirstChildElement();
	list<string>::const_iterator cit = requestStr.begin();
	stringstream ssConvert;
	string tempStr;
	string id;
	string value;
	int spanIndex = string::npos;

	while (cit != requestStr.end())
	{
		spanIndex = cit->find_first_of("=");
		if (spanIndex != string::npos)
		{
			id = cit->substr(1, spanIndex - 1);
			int newID = atoi(id.c_str()) - 1;
			ssConvert << newID;
			ssConvert >> id;
			ssConvert.clear();
			value = cit->substr(spanIndex + 1, cit->length() - spanIndex - 1);
			//if (pLog != NULL)
			//{
			//	pLog->WriteLogInfo(string("         #3  id ==") + id);
			//}
		}

		while (pElement)
		{
			//if (pLog != NULL)
			//{
			//	pLog->WriteLogInfo(string("         #2  ")+id+string(" == ")+string(pElement->Attribute("id")));
			//}

			if (id == string(pElement->Attribute("id")))
			{
				pElement->SetAttribute("status", value.c_str());
				break;
			}

			pElement = pElement->NextSiblingElement();
		}

		//if (pLog != NULL)
		//{
		//	pLog->WriteLogInfo("  out of while ..............");
		//}

		if (pElement == NULL)
		{
			//if (pLog != NULL)
			//{
			//	pLog->WriteLogInfo(string("   pElement==NULL    ") + string(pProtocol->Value()));
			//}
			TiXmlElement addNode("select");
			addNode.SetAttribute("id", id.c_str());
			addNode.SetAttribute("name", "");
			addNode.SetAttribute("status", value.c_str());
			pProtocol->InsertEndChild(addNode);
		}

		cit++;
	}
}

void addProtocolMapPanelUserData(TiXmlDocument xmlDoc, string userID, const list<string> requestStr, TiXmlElement *pRoot, LogInfoTool* pLog)
{
	//if (pLog != NULL)
	//{
	//	pLog->WriteLogInfo("         inside addProtocolMapPanelUserData() ");
	//}
	// 添加新用户节点
	TiXmlElement addUserNode("ProtocolMapPanel");
	addUserNode.SetAttribute("userID", userID.c_str());
	pRoot->InsertEndChild(addUserNode);

	addProtocolMapPanelSelectData(xmlDoc, requestStr, &addUserNode, pLog);
}

TiXmlElement* getUserElement(string userID, TiXmlElement *pRoot, LogInfoTool* pLog)
{
	TiXmlElement* pUserElement = pRoot->FirstChildElement("ProtocolMapPanel");
	while (pUserElement)
	{
		if (userID == string(pUserElement->Attribute("userID")))
		{
			break;
		}
		else
		{
			pUserElement = pUserElement->NextSiblingElement();
		}
	}

	return pUserElement;
}

void ReadNpcMovableFlag(string userID, LogInfoTool* pLog)
{
	if (pLog != NULL)
	{
		pLog->WriteLogInfo("         inside ReadNpcMovableFlag() ");
	}

	TiXmlDocument configXml("DebugToolConfig.xml");
	configXml.LoadFile();
	TiXmlElement *pRoot = configXml.RootElement();
	if (pRoot == NULL)
	{
		return;
	}

	TiXmlElement *pProtocol = getUserElement(userID, pRoot, pLog);
	if (pProtocol != NULL)
	{
		const char *pValue = pProtocol->Attribute("NpcMovable");
		if (pValue != NULL)
		{
			string tempStr = "p0=4&p1=";
			tempStr.append(pValue);
			cout << tempStr.c_str();
		}
	}
}

void WriteNpcMovableFlag(string userID, const list<string> requestStr, LogInfoTool* pLog /* = NULL */)
{
	if (pLog != NULL)
	{
		pLog->WriteLogInfo("         inside WriteNpcMovableFlag() ");
	}

	TiXmlDocument configXml("DebugToolConfig.xml");
	configXml.LoadFile();

	TiXmlElement *pRoot = configXml.RootElement();
	if (pRoot == NULL)
	{
		return;
	}

	TiXmlElement *pProtocol = getUserElement(userID, pRoot, pLog);
	
	if (pProtocol == NULL)
	{
		list<string> requestStr;
		addProtocolMapPanelUserData(configXml, userID, requestStr, pRoot, pLog);
	}

	if (requestStr.empty())
	{
		return;
	}

	string tempValue = requestStr.front();
	int spanIndex = tempValue.find_first_of("=");
	if (spanIndex != string::npos)
	{
		tempValue = tempValue.substr(spanIndex + 1, tempValue.length() - spanIndex - 1);
		pProtocol->SetAttribute("NpcMovable", tempValue.c_str());
		configXml.SaveFile();
	}
}