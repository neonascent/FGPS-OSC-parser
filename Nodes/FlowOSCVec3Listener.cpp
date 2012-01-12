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
#include "process.h"
#include "oscpkt.hh"
#include "Nodes/G2FlowBaseNode.h"

////////////////////////////////////////////////////
class CFlowOSCVec3_Listener : public CFlowBaseNode
{
	
	enum EInputPorts
	{
		EIP_Message,
		EIP_Filter, 
	};

	enum EOutputs
	{
		EOP_Updated = 0,
		EOP_Vec3,
		EOP_String,
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
	CFlowOSCVec3_Listener(SActivationInfo *pActInfo)
	{
		// constructor
		//socketWorking = false;
		//m_bEnabled = false;
		Filter = "";
	}

	////////////////////////////////////////////////////
	void getArguments(string packetData, int packetSize) {
		// '/1/fader1   ,f  ?Q'
		/*char string[] = {'\57', '\61', '\57', '\146', '\141', '\144', '\145',
						 '\162', '\61', '\000', '\000', '\000', '\054', '\146', '\000', '\000', '\77', '\121', '\000', '\000'};*/
		// change \1 back to \0
		


		char message[256];
		for (int i = 0; i < packetSize; i++) {
			if (packetData[i] < 2) {
				message[i] = '\0';
			} else {
				message[i] = packetData[i];
			}
		}


		std::string r = message;
				string value = r.c_str();

		//packetData 
		pr.init(message, packetSize);
        oscpkt::Message *msg;
        while (pr.isOk() && (msg = pr.popMessage()) != 0) {
          float iarg1;  
		  float iarg2;  
		  float iarg3;  
		  if (msg->match(Filter).popFloat(iarg1)) {
			if (msg->match(Filter).popFloat(iarg2)) {
				if (msg->match(Filter).popFloat(iarg3).isOkNoMoreArgs()) {
					ActivateOutput(&m_actInfo, EOP_Vec3, Vec3(iarg1, iarg2, iarg3));
					ActivateOutput(&m_actInfo, EOP_Updated, true);
				}
			  } 
		  }
		  ActivateOutput(&m_actInfo, EOP_String, value);
		}
	}

	////////////////////////////////////////////////////
	virtual ~CFlowOSCVec3_Listener(void)
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
			OutputPortConfig_Void("Updated", _HELP("Triggered when updated")), 
			OutputPortConfig<Vec3>("Vec3", _HELP("Vec3 argument")),
			OutputPortConfig_Void("String", _HELP("Vec3 argument")),
			{0}
		};

		// Fill in configuration
		config.pInputPorts = inputs;
		config.pOutputPorts = outputs;
		config.sDescription = _HELP("Gets Vec3 value from OSC message (\0s needs to be converted to \1 in input string)");
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

									
					/*ActivateOutput(&m_actInfo, EOP_Vec3, (Vec3)message.length());
					ActivateOutput(&m_actInfo, EOP_Int, message.length());




					ActivateOutput(&m_actInfo, EOP_String, value);
					 ActivateOutput(&m_actInfo, EOP_Updated, true);*/
					getArguments(message, message.length());
				}
			}
			break;

		}
	}

	////////////////////////////////////////////////////
	virtual IFlowNodePtr Clone(SActivationInfo *pActInfo)
	{
		return new CFlowOSCVec3_Listener(pActInfo);
	}

	////////////////////////////////////////////////////
	virtual void GetMemoryStatistics(ICrySizer *s)
	{
		s->Add(*this);
	}


};


////////////////////////////////////////////////////
////////////////////////////////////////////////////

REGISTER_FLOW_NODE("OSC:Vec3Listener", CFlowOSCVec3_Listener);
