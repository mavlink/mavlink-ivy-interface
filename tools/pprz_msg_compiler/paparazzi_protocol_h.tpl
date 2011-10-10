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
*	$Id: paparazzi_protocol_h.tpl $
*																*
****************************************************************/

// Includes
#ifndef __MY_PAPARAZZI_PROTOCOL_HEADER__
#define __MY_PAPARAZZI_PROTOCOL_HEADER__

#include "Fdc.h"
#include "MyStdLib.h"

#include <string>
#include <sstream>
#include <iterator>
#include <vector>
// included for itoa
#include <stdio.h>
#include <stdlib.h>

#if defined(WIN32)
#pragma pack(push)
#pragma pack(1)
#endif

#ifdef DEBUG
#undef DEBUG
#define DEBUG DEBUK
#endif

template <class Cb_Class>
class paparazzi_protocol
{
public:

	paparazzi_protocol(struct Fdc* s)
	{
<repeat name="init">
		memset(&this->%s, 0, sizeof(%s_pprz_struct));
</repeat>
		port = s;
	}

<repeat name="structs">
	struct %s_pprz_struct
	{
%s	} %s
#if defined(__GNUC__)
__attribute__((packed))
#endif
;

</repeat>

public:
<repeat name="send">
	void send_%s() { send( (void*) &this->%s, %d, %d ); }
</repeat>


<repeat name="to_ivy">
	std::string %s_to_ivy()
	{
		std::stringstream ivy;

		ivy << this->ac_id <<  " %s";
%s

		return ivy.str();
	}
</repeat>

	void split(const std::string input, std::vector<std::string> &results)
	{
		const string& delimiter = " ";
		const bool includeEmpties = true;

		int iPos = 0;
		int newPos = -1;
		int sizeS2 = (int)delimiter.size();
		int isize = (int)input.size();

		if( 
			( isize == 0 )
			||
			( sizeS2 == 0 )
		)
		{
			return;
		}

		vector<int> positions;

		newPos = input.find (delimiter, 0);

		if( newPos < 0 )
		{ 
			return; 
		}

		int numFound = 0;

		while( newPos >= iPos )
		{
			numFound++;
			positions.push_back(newPos);
			iPos = newPos;
			newPos = input.find (delimiter, iPos+sizeS2);
		}

		if( numFound == 0 )
		{
			return;
		}

		for( int i=0; i <= (int)positions.size(); ++i )
		{
			string s("");
			if( i == 0 ) 
			{ 
				s = input.substr( i, positions[i] ); 
			}
			int offset = positions[i-1] + sizeS2;
			if( offset < isize )
			{
				if( i == positions.size() )
				{
					s = input.substr(offset);
				}
				else if( i > 0 )
				{
					s = input.substr( positions[i-1] + sizeS2, 
						  positions[i] - positions[i-1] - sizeS2 );
				}
			}
			if( includeEmpties || ( s.size() > 0 ) )
			{
				results.push_back(s);
			}
		}
		return;
	}

<repeat name="print">
	void print_%s()
	{
		printf("Received msg '%%s' (%%d)\n","%s",%d);
%s	}
</repeat>



	int ivy_decode(const char* _msg)
	{
		std::stringstream vals;
		vals.str(_msg);
		std::string msg_name;
		std::vector<std::string> msg;
		split(_msg, msg);

		if (msg.size() < 2)
		{	
			E_printf("Msg Error: too short\n");
			return -1;
		}

		vals >> this->ac_id;
		vals >> msg_name;

<repeat name="ivy">
		if (msg_name.compare("%s") == 0)
		{
%s			print_%s();
			return %d;
		}
</repeat>
		return -2;
	}

	int receive()
	{
		// Read
		fdcRead(port);

		// Search for start-byte
		while ( (port->bytesread > 0) &&
				(port->buffer[0] != 0x99) )
		{
			fdcBufCrop(port,1);
		}

		// Check Length
		int l = (unsigned char) port->buffer[1]-4;
		if ((int)port->bytesread < (l+4))
			return 0;

		// Verify Checksum
		unsigned char ck_a;
		unsigned char ck_b;
		ck_a = port->buffer[1];
		ck_b = port->buffer[1];
		for (int i=0;i<l;i++)
		{
			ck_a += port->buffer[2+i];
			ck_b += ck_a;
		}
		if ((port->buffer[2+l] != ck_a) ||
			(port->buffer[3+l] != ck_b))
		{	
			E_printf("Decoding Checksum Error\n");
			E_printf("CK: %X %X %X %X \n",ck_a,ck_b,port->buffer[2+l],port->buffer[3+l]);
			//fdcBufCrop(port,l+4);
			//return -1;
		}

		ac_id = port->buffer[2];
		int msgid = port->buffer[3];
		switch (msgid)
		{
<repeat name="switcher">
		case %d:	// %s
			memcpy(&this->%s,&(port->buffer[4]),%d);
			print_%s();
			break;
</repeat>
		}
		fdcBufCrop(port,l+4);
		return msgid;
	}

private:
	void send(void *p, unsigned char t, int len)
	{
		unsigned char cs_a, cs_b;

		cs_a = (unsigned char) (len+2);
		cs_b = cs_a;

		buff[0] = 0x99;
		buff[1] = cs_a;
		buff[2] = ac_id;
		buff[3] = t;

		memcpy(&(buff[4]),p, len);

		for (int i=0;i<(len+2);i++)
		{
			cs_a += buff[2+len];
			cs_b += cs_a;
		}

		buff[4+len] = cs_a;
		buff[5+len] = cs_b;

		fdcWrite(port,(char*)buff,len+6);
	}

public:
	int ac_id;

<repeat name="cbfunc">
	inline void %s_set_interrupt(Cb_Class *_inst, void (Cb_Class::*f) (void*))
	{
		%s_Instance = _inst; %s_func_p = f;
	}
</repeat>

private:
	struct Fdc* port;
	unsigned char buff[264];

<repeat name="privdat">
	void (Cb_Class::*%s_func_p) (void*);
	Cb_Class *%s_Instance;
</repeat>



};

#if defined(WIN32)
#pragma pack(pop)
#endif


#endif