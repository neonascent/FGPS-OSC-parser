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
class CFlowOSCArgument_Listener : public CFlowBaseNode
{
	
	enum EInputPorts
	{
		EIP_Message,
		EIP_Filter, 
	};

	enum EOutputs
	{
		EOP_String1 = 0,
		EOP_String2,
		EOP_String3,
		EOP_String4,
		EOP_String5,
	};
	

	//OSC
	/** return the bytes of the osc packet (NULL if the construction of the packet has failed) */
	/** return the number of bytes of the osc packet -- will always be a
      multiple of 4 -- returns 0 if the construction of the packet has
      failed. */
	//char *

	oscpkt::PacketReader pr;
	std::string Filter;

	//bool m_bEnabled;
	SActivationInfo m_actInfo;  // is this needed?  We already just use pActInfo

public:
	////////////////////////////////////////////////////
	CFlowOSCArgument_Listener(SActivationInfo *pActInfo)
	{
		// constructor
		//socketWorking = false;
		//m_bEnabled = false;
		Filter = "";
	}

	////////////////////////////////////////////////////
	void getArguments(string packetData, int packetSize) {
		

				 /*const std::string s = "ADP GmbH\nAnalyse Design & Programmierung\nGesellschaft mit beschränkter Haftung" ;

			  std::string encoded = base64_encode(reinterpret_cast<const unsigned char*>(s.c_str()), s.length());
			  std::string decoded = base64_decode(encoded);

			  std::cout << "encoded: " << encoded << std::endl;
			  std::cout << "decoded: " << decoded << std::endl;
			  */
		std::string encoded = packetData.c_str();
		std::string decoded = base64_decode(encoded);// does this handle null????

		// copy the null characters over, otherwise it ends the string
		char message[256];
		for (int i = 0; i < decoded.length(); i++) {
			message[i] = decoded[i];
		}

		string value = decoded.c_str();





		// setup arg locations
		std::string listargs[5]= {"","","","",""};
		int count = 0;



		//packetData 
		pr.init(message, decoded.length());
        oscpkt::Message *msg;
        while (pr.isOk() && (msg = pr.popMessage()) != 0) {


			oscpkt::Message::ArgReader arg(msg->arg());
			while (arg.nbArgRemaining()) {
			  // setup temporary string stream
			  std::stringstream ss (std::stringstream::in | std::stringstream::out);
	
			  if (arg.isBlob()) {
				//std::vector<char> b; arg.popBlob(b); 
				//if (b.size()>1) { check(b.front() == 0x44 || fuzz); check(b.back() == 0x66 || fuzz); }
			  } else if (arg.isBool()) {
				bool b; arg.popBool(b); ss << b; listargs[count] = ss.str();
			  } else if (arg.isInt32()) {
				int i; arg.popInt32(i); ss << i; listargs[count] = ss.str();
			  } else if (arg.isFloat()) {
				float f; arg.popFloat(f); ss << f; listargs[count] = ss.str();
			  } else if (arg.isDouble()) {
				double d; arg.popDouble(d); ss << d; listargs[count] = ss.str();
			  } else if (arg.isStr()) {
				std::string s; arg.popStr(s); listargs[count] = s;
			  }
			  count++;
			}

			
			ActivateOutput(&m_actInfo, EOP_String1, (string)listargs[0].c_str());
			ActivateOutput(&m_actInfo, EOP_String2, (string)listargs[1].c_str());
			ActivateOutput(&m_actInfo, EOP_String3, (string)listargs[2].c_str());
			ActivateOutput(&m_actInfo, EOP_String4, (string)listargs[3].c_str());
			ActivateOutput(&m_actInfo, EOP_String5, (string)listargs[4].c_str());

		}

		/*
          float iarg1;  
		  float iarg2;   
		  if (msg->match(Filter).popFloat(iarg1)) {
			if (msg->match(Filter).popFloat(iarg2)) {
				ActivateOutput(&m_actInfo, EOP_Float1, iarg1);
				ActivateOutput(&m_actInfo, EOP_Float2, iarg2);
				ActivateOutput(&m_actInfo, EOP_Updated, true);
			  //} 
		  }
		  //ActivateOutput(&m_actInfo, EOP_String, value);
		  return;
		}
		return;*/
	}

	////////////////////////////////////////////////////
	virtual ~CFlowOSCArgument_Listener(void)
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
			InputPortConfig<string>("Filter", _HELP("The OSC address being looked for eg. /1/fader1")),
			{0}
		};

		// Define output ports here, in same oreder as EOutputPorts
		static const SOutputPortConfig outputs[] =
		{
			OutputPortConfig_Void("Argument1", _HELP("Argument1")),
			OutputPortConfig_Void("Argument2", _HELP("Argument2")),
			OutputPortConfig_Void("Argument3", _HELP("Argument3")),
			OutputPortConfig_Void("Argument4", _HELP("Argument4")),
			OutputPortConfig_Void("Argument5", _HELP("Argument5")),
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
					
					Filter = GetPortString(pActInfo, EIP_Filter);

					getArguments(message, message.length());
				}
			}
			break;
		}
	}

	////////////////////////////////////////////////////
	virtual IFlowNodePtr Clone(SActivationInfo *pActInfo)
	{
		return new CFlowOSCArgument_Listener(pActInfo);
	}

	////////////////////////////////////////////////////
	virtual void GetMemoryStatistics(ICrySizer *s)
	{
		s->Add(*this);
	}


};


////////////////////////////////////////////////////
////////////////////////////////////////////////////

REGISTER_FLOW_NODE("OSC:ArgumentListener", CFlowOSCArgument_Listener);

