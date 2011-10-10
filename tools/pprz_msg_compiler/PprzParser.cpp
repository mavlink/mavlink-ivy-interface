/****************************************************************
*																*
*				------ SmartUAV Project ------					*
*																*
*	Aerospace Software and Technologies Institue	ASTI		*
*	Delft Cooperation for Intelligent Systems		DECIS		*
*	Delft University of Technology					TU Delft	*
*																*
*	M. Amelink - C. De Wagter									*
*																*
*	$Id: . $
*																*
****************************************************************/

#pragma warning( disable : 4018 ) // signed/unsigned mismatch
#pragma warning( disable : 4786 ) // truncated to 255


// Includes
#include "PprzParser.h"
#include "paparazzi_protocol_h.h"

#include "../tinyxml/mytinyxml.h"

#include <iostream>
#include <string>
#include <map>

void parsepprz(char* fileName)
{
	std::string fields;
	std::string prints;
	std::string ivys;
	std::string toivys;
	std::map<std::string,int>			typesize;
	std::map<std::string,std::string>	typetype;
	std::map<std::string,std::string>	typeprint;
	std::map<std::string,std::string>	typetoivystring;


	typetype["int8"] = "char";
	typetype["uint8"] = "unsigned char";
	typetype["int8[]"] = "char";
	typetype["uint8[]"] = "unsigned char";
	typetype["int16"] = "short";
	typetype["uint16"] = "unsigned short";
	typetype["int16[]"] = "short";
	typetype["uint16[]"] = "unsigned short";
	typetype["int32"] = "int";
	typetype["int32[]"] = "int";
	typetype["uint32[]"] = "int";
	typetype["uint32"] = "unsigned int";
	typetype["float"] = "float";
	typetype["double"] = "double";

	typetoivystring["int8"] = "(int)";
	typetoivystring["uint8"] = "(unsigned int)";
	typetoivystring["int8[]"] = "(int)";
	typetoivystring["uint8[]"] = "(int)";
	typetoivystring["int16"] = "";
	typetoivystring["uint16"] = "";
	typetoivystring["int16[]"] = "";
	typetoivystring["uint16[]"] = "";
	typetoivystring["int32"] = "";
	typetoivystring["int32[]"] = "";
	typetoivystring["uint32[]"] = "";
	typetoivystring["uint32"] = "";
	typetoivystring["float"] = "";
	typetoivystring["double"] = "";

	const int nb_values = 8;

	typesize["int8"] = 1;
	typesize["uint8"] = 1;
	typesize["int8[]"] = 1 * nb_values;
	typesize["uint8[]"] = 1 * nb_values;
	typesize["int16"] = 2;
	typesize["uint16"] = 2;
	typesize["int16[]"] = 2 * nb_values;
	typesize["uint16[]"] = 2 * nb_values;
	typesize["int32"] = 4;
	typesize["int32[]"] = 4;
	typesize["uint32[]"] = 4;
	typesize["uint32"] = 4;
	typesize["float"] = 4;
	typesize["double"] = 8;

	typeprint["int8"] = "d";
	typeprint["uint8"] = "u";
	typeprint["int8[]"] = "p";
	typeprint["uint8[]"] = "p";
	typeprint["int16"] = "d";
	typeprint["uint16"] = "u";
	typeprint["int16[]"] = "p";
	typeprint["uint16[]"] = "p";
	typeprint["int32"] = "d";
	typeprint["int32[]"] = "d";
	typeprint["uint32[]"] = "d";
	typeprint["uint32"] = "u";
	typeprint["float"] = "f";
	typeprint["double"] = "f";



	paparazzi_protocol_h.open("paparazzi_protocol.h");

	TiXmlDocument doc;
	bool Okay = doc.LoadFile( fileName );
	if ( !Okay )
	{
		std::cout << "Unable to open XML: " << fileName << std::endl;
	}

	TiXmlElement *node = doc.RootElement();

	if (node != 0)
	{
		// CLASS
		// std::cout << node->Value() << std::endl;
		for (TiXmlElement *e = node->FirstChildElement();e!= 0;e=e->NextSiblingElement())
		{
			// MESSAGE
			// std::cout << " - " << e->Attribute("name") << std::endl;
			for (TiXmlElement *m = e->FirstChildElement();m!=0;m=m->NextSiblingElement())
			{
				// FIELD
				fields.erase();
				prints.erase();
				ivys.erase();
				toivys.erase();
				int struct_size = 0;

				// std::cout << "   * " << getattributecharvalue(m,"name") << " (" << getattributeintvalue(m,"id")+getattributeintvalue(m,"ID") << ")" << std::endl;
				int i = 0;
				for (TiXmlElement *f = m->FirstChildElement();f!=0;f=f->NextSiblingElement())
				{
					// std::cout << "     > " << getattributecharvalue(f,"name") << " (" << getattributecharvalue(f,"type") << ")" << std::endl;
					fields.append("\t\t");
					fields.append(typetype[getattributecharvalue(f,"type")]);
					fields.append(" pprz_");
					fields.append(getattributecharvalue(f,"name"));
					fields.append(";	// ");
					fields.append(getattributecharvalue(f,"type"));
					fields.append("\n");

					prints.append("\t\tprintf(\"  %s = %");
					prints.append(typeprint[getattributecharvalue(f,"type")]);
					prints.append("\\n\", \"");
					prints.append(getattributecharvalue(f,"name"));
					prints.append("\", this->");
					prints.append(getattributecharvalue(m,"name"));
					prints.append(".pprz_");
					prints.append(getattributecharvalue(f,"name"));
					prints.append(");\n");
					
					ivys.append("\t\t\t");
					ivys.append(getattributecharvalue(m,"name"));
					ivys.append(".pprz_");
					ivys.append(getattributecharvalue(f,"name"));
					ivys.append(" = atof(msg[2+");
					char tmp[4];
					sprintf(tmp,"%d",i);
					i++;
					ivys.append(tmp);
					ivys.append("].c_str());\n");

					struct_size += typesize[getattributecharvalue(f,"type")];
				}

				if (strcmp(getattributecharvalue(e,"name"), "datalink") == 0)
				{
					for (TiXmlElement *f = m->FirstChildElement();f!=0;f=f->NextSiblingElement())
					{
//						if(~strcmp(getattributecharvalue(f,"type"), "char") || ~strcmp(getattributecharvalue(f,"type"), "unsigned char"))
						{
							toivys.append("\t\tivy << \" \" << ");
							toivys.append(typetoivystring[getattributecharvalue(f,"type")]);
							toivys.append("this->");
							toivys.append(getattributecharvalue(m,"name"));
							toivys.append(".pprz_");
							toivys.append(getattributecharvalue(f,"name"));
							toivys.append(";\n");	
							/*
							toivys.append("\t\titoa(this->");
							toivys.append(getattributecharvalue(m,"name"));
							toivys.append(".pprz_");
							toivys.append(getattributecharvalue(f,"name"));
							toivys.append(", buffer, 10);\n");
							toivys.append("\t\tivy << \" \" << buffer;\n");
							*/
						}
/*						else
						{
							toivys.append("\t\tivy << \" \" << this->");
							toivys.append(getattributecharvalue(m,"name"));
							toivys.append(".pprz_");
							toivys.append(getattributecharvalue(f,"name"));
							toivys.append(";\n");	
						}
*/					}
				}

				if (strcmp(getattributecharvalue(e,"name"), "telemetry") == 0)
				{
					paparazzi_protocol_h.add_switcher(	getattributeintvalue(m,"id")+getattributeintvalue(m,"ID"), 
														getattributecharvalue(m,"name"),
														getattributecharvalue(m,"name"),
														struct_size,
														getattributecharvalue(m,"name")
													 );

					paparazzi_protocol_h.add_print(		getattributecharvalue(m,"name"),
														getattributecharvalue(m,"name"),
														getattributeintvalue(m,"id")+getattributeintvalue(m,"ID"),
														prints.c_str()
												  );
					paparazzi_protocol_h.add_privdat(	getattributecharvalue(m,"name"),
														getattributecharvalue(m,"name")
													);
					paparazzi_protocol_h.add_cbfunc(	getattributecharvalue(m,"name"),
														getattributecharvalue(m,"name"),
														getattributecharvalue(m,"name")
												   );	

					paparazzi_protocol_h.add_ivy(		getattributecharvalue(m,"name"),
														ivys.c_str(),
														getattributecharvalue(m,"name"),
														getattributeintvalue(m,"id")+getattributeintvalue(m,"ID")
												);
				}

				if ( (strcmp(getattributecharvalue(e,"name"), "telemetry") == 0) ||
					 (strcmp(getattributecharvalue(e,"name"), "datalink") == 0)  )
				{
					paparazzi_protocol_h.add_structs(	getattributecharvalue(m,"name"),
														fields.c_str(),
														getattributecharvalue(m,"name") );
					paparazzi_protocol_h.add_init(	getattributecharvalue(m,"name"),	getattributecharvalue(m,"name")  );
				}

				if (strcmp(getattributecharvalue(e,"name"), "datalink") == 0)
				{
					paparazzi_protocol_h.add_send(		getattributecharvalue(m,"name"),
														getattributecharvalue(m,"name"),
														getattributeintvalue(m,"id")+getattributeintvalue(m,"ID"),
														struct_size);
					paparazzi_protocol_h.add_to_ivy(	getattributecharvalue(m,"name"),
														getattributecharvalue(m,"name"),
														toivys.c_str()
													);
				}				
			}
		}

	}
	else
	{
		std::cout << "Unable to load rootnode " << std::endl;
	}


	paparazzi_protocol_h.commit();
	std::cout << "papazazzi_protocol.h created.\n";
}
