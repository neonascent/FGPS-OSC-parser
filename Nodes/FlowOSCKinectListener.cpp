/////////////////////////////////////////////////////////////////
// Copyright (C), RenEvo Software & Designs, 2008
// FGPlugin Source File
//
// FlowXmlDataNodes.cpp
//
// Purpose: Flowgraph nodes to dealing with data in Xml elements
//
// History:
//	- 8/23/08 : File created - KAK
/////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "base64.h"
#include <iostream>
#include <sstream>
#include "process.h"
#include "oscpkt.hh"
#include "Nodes/G2FlowBaseNode.h"

////////////////////////////////////////////////////
class CFlowOSCKinect_Listener : public CFlowBaseNode
{
	
	enum EInputPorts
	{
		EIP_Message,
		EIP_Filter, 
		EIP_JointName,
	};

	enum EOutputs
	{
		EOP_String1 = 0,
		EOP_String2,
		EOP_String3,
	};
	

	//OSC
	/** return the bytes of the osc packet (NULL if the construction of the packet has failed) */
	/** return the number of bytes of the osc packet -- will always be a
      multiple of 4 -- returns 0 if the construction of the packet has
      failed. */
	//char *

	oscpkt::PacketReader pr;

	//bool m_bEnabled;
	SActivationInfo m_actInfo;  // is this needed?  We already just use pActInfo

public:
	////////////////////////////////////////////////////
	CFlowOSCKinect_Listener(SActivationInfo *pActInfo)
	{
		// constructor
		//socketWorking = false;
		//m_bEnabled = false;
	}

	////////////////////////////////////////////////////
	void getArguments(string packetData, int packetSize, std::string Filter, std::string JointFilter) {
		

				 /*const std::string s = "ADP GmbH\nAnalyse Design & Programmierung\nGesellschaft mit beschränkter Haftung" ;

			  std::string encoded = base64_encode(reinterpret_cast<const unsigned char*>(s.c_str()), s.length());
			  std::string decoded = base64_decode(encoded);

			  std::cout << "encoded: " << encoded << std::endl;
			  std::cout << "decoded: " << decoded << std::endl;
			  */
		std::string encoded = packetData.c_str();
		std::string decoded = base64_decode(encoded);// does this handle null????

		// copy the null characters over, otherwise it ends the string
		const int messageLength = 1024;
		char message[messageLength];
		for (int i = 0; (i < decoded.length()) && (i < messageLength); i++) {
			message[i] = decoded[i];
		}

		string value = decoded.c_str();





		//packetData 
		pr.init(message, decoded.length());
		oscpkt::Message *msg;
		while (pr.isOk() && (msg = pr.popMessage()) != 0) {
			if(msg->match("/joint")) {

				// setup arg locations
				std::string listargs[3]= {"","",""};
				int count = 0;
				bool isCorrectJoint = false;
				bool isCorrectId = false;
				Filter="1";
				JointFilter="l_elbow";
				

				oscpkt::Message::ArgReader arg(msg->arg());
				while (arg.nbArgRemaining()) {  
					if (arg.isBlob()) {
						//std::vector<char> b; arg.popBlob(b); 
						//if (b.size()>1) { check(b.front() == 0x44 || fuzz); check(b.back() == 0x66 || fuzz); }
					} else if (arg.isBool()) {
						bool b; arg.popBool(b); 
					} else if (arg.isInt32()) {
						int i; arg.popInt32(i); std::stringstream ss (std::stringstream::in | std::stringstream::out); ss << i; std::string id = ss.str(); if(id.compare(Filter) == 0) { isCorrectId = true; }
					} else if (arg.isFloat()) {
						float f; arg.popFloat(f); std::stringstream ss (std::stringstream::in | std::stringstream::out); ss << f; listargs[count] = ss.str(); count++;
					} else if (arg.isDouble()) {
						double d; arg.popDouble(d); 
					} else if (arg.isStr()) {
						std::string s; arg.popStr(s); if(s.compare(JointFilter) == 0) { isCorrectJoint = true; }
					}
				}

				// if correct parts, then update
				if (isCorrectJoint && isCorrectId) {
					ActivateOutput(&m_actInfo, EOP_String1, (string)listargs[0].c_str());
					ActivateOutput(&m_actInfo, EOP_String2, (string)listargs[1].c_str());
					ActivateOutput(&m_actInfo, EOP_String3, (string)listargs[2].c_str());
				}


			} 

		} // end of  messages

	}

	////////////////////////////////////////////////////
	virtual ~CFlowOSCKinect_Listener(void)
	{
		// destructor
		
	}

	////////////////////////////////////////////////////
	virtual void Serialize(SActivationInfo *pActInfo, TSerialize ser)
	{
		
	}

	////////////////////////////////////////////////////
	virtual void GetConfiguration(SFlowNodeConfig& config)
	{
		// Define input ports here, in same order as EInputPorts
		static const SInputPortConfig inputs[] =
		{
			InputPortConfig<string>("Message", _HELP("Pipe OSC data in here")),
			InputPortConfig<string>("Filter", _HELP("The Skeleton Id eg. 1")),
			InputPortConfig<string>("JointFilter", _HELP("The OSC joint name looked for eg. l_elbow ")),
			{0}
		};

		// Define output ports here, in same oreder as EOutputPorts
		static const SOutputPortConfig outputs[] =
		{
			OutputPortConfig_Void("X", _HELP("X coordinate relative to kinect")),
			OutputPortConfig_Void("Y", _HELP("Y coordinate relative to kinect")),
			OutputPortConfig_Void("Z", _HELP("Z coordinate relative to kinect")),
			{0}
		};

		// Fill in configuration
		config.pInputPorts = inputs;
		config.pOutputPorts = outputs;
		config.sDescription = _HELP("Gets float value from OSC message (\0s needs to be converted to \1 in input string)");
		//config.SetCategory(EFLN_ADVANCED);
	}





	////////////////////////////////////////////////////
	virtual void ProcessEvent(EFlowEvent event, SActivationInfo *pActInfo)
	{
		switch (event)
		{
			case eFE_Initialize:
			{
				m_actInfo = *pActInfo;
			}
			break;

			case eFE_Activate:
			{
				if (IsPortActive(pActInfo, EIP_Message)) {
				
					
					//m_bEnabled = true;
					string message = GetPortString(pActInfo, EIP_Message);
					
					std::string Filter = GetPortString(pActInfo, EIP_Filter);
					std::string JointFilter = GetPortString(pActInfo, EIP_JointName);

					getArguments(message, message.length(), Filter, JointFilter);
				}
			}
			break;
		}
	}

	////////////////////////////////////////////////////
	virtual IFlowNodePtr Clone(SActivationInfo *pActInfo)
	{
		return new CFlowOSCKinect_Listener(pActInfo);
	}

	////////////////////////////////////////////////////
	virtual void GetMemoryStatistics(ICrySizer *s)
	{
		s->Add(*this);
	}


};


////////////////////////////////////////////////////
////////////////////////////////////////////////////

REGISTER_FLOW_NODE("OSC:KinectListener", CFlowOSCKinect_Listener);

