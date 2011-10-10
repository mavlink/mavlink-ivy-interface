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
*	$Id: loadsim.h 322 2006-06-30 22:27:24Z cdewagter $
*																*
****************************************************************/


#ifndef my_tinyxml_header
#define my_tinyxml_header


#include "tinyxml.h"

//!	\defgroup myxml Own TinyXML Extras

//! @{


TiXmlNode* findormakechild(TiXmlNode *x, char *name);

// TinyXml safe accessor
const int getattributeintvalue(TiXmlElement *x,char *name);
const float getattributefloatvalue(TiXmlElement *x,char *name);
const char* getattributecharvalue(TiXmlElement *x,char *name);

void setattributeintvalue(TiXmlElement *x,char *name, const int);
void setattributefloatvalue(TiXmlElement *x,char *name, const float);
void setattributecharvalue(TiXmlElement *x,char *name, const char*);

const int getchildintvalue(TiXmlNode *x,char *name);
const float getchildfloatvalue(TiXmlNode *x,char *name);
const char* getchildcharvalue(TiXmlNode *x,char *name);

void setchildintvalue(TiXmlNode *x,char *name, const int);
void setchildfloatvalue(TiXmlNode *x,char *name, const float);
void setchildcharvalue(TiXmlNode *x,char *name, const char*);

//! TinyXml safe accessor
const char* getvalue(TiXmlNode *x);
void setvalue(TiXmlNode *n,const char* v);

const char *getname(TiXmlNode *x);
void setname(TiXmlNode *x, const char *name);

//! TinyXml safe accessor
const char *getattribute(TiXmlElement *x,char *name);
//void setattribute(TiXmlElement *x,char *name, char *value);

//!	@}

#endif

