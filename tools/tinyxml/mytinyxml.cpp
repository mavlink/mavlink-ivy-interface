
#include "mytinyxml.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

TiXmlNode* findormakechild(TiXmlNode *x, char *name)
{
	// printf("Find Child %s in node %p: \n",name,(void*)x);
	if (x)
	{
		if (x->FirstChild(name) != 0)
		{
			return x->FirstChild(name);
		}
		else
		{
			TiXmlElement tmp(name);
			return x->InsertEndChild(tmp);
		}
	}
	return 0;
}


// TinyXml safe accessor
const char *getvalue(TiXmlNode *x)
{
	if (x != NULL)
		return x->Value();
	else
		return "";
}

// TinyXml safe accessor
const int getattributeintvalue(TiXmlElement *x,char *name)
{
	if (x != 0)
	{
		if (x->Attribute(name) != NULL)
			return atoi(x->Attribute(name));
		else
			return 0;
	}
	else
		return 0;
}

// TinyXml safe accessor
const float getattributefloatvalue(TiXmlElement *x,char *name)
{
	if (x != 0)
	{
		if (x->Attribute(name) != NULL)
			return (const float) atof(x->Attribute(name));
		else
			return 0.0f;
	}
	else
		return 0.0f;
}

// TinyXml safe accessor
const char* getattributecharvalue(TiXmlElement *x,char *name)
{
	if (x != 0)
	{
		if (x->Attribute(name) != NULL)
			return (const char*) (x->Attribute(name));
		else
			return "";
	}
	else
		return "";
}


const int getchildintvalue(TiXmlNode *x,char *name)
{
//	printf("Get Child Int Value %s = %d\n",name,atoi(x->FirstChild(name)->Value()));
	return (const int) atoi(getchildcharvalue(x, name));
}
const float getchildfloatvalue(TiXmlNode *x,char *name)
{
	return (const float) atof(getchildcharvalue(x, name));
}
const char* getchildcharvalue(TiXmlNode *x,char *name)
{
	if (x != 0)
	{
		if (x->FirstChild(name) != NULL)
		{
			if (x->FirstChild(name)->FirstChild() != NULL)
				return (const char*) (x->FirstChild(name)->FirstChild()->Value());
			else
				return "";
		}
		else
		{
			return "";
		}
	}
	else
	{
		return "";
	}
}

void setchildintvalue(TiXmlNode *x,char *name, const int v)
{
	char tmp[255];
	sprintf(tmp,"%d",v);
	setchildcharvalue(x,name,tmp);
}
void setchildfloatvalue(TiXmlNode *x,char *name, const float v)
{
	char tmp[255];
	sprintf(tmp,"%f",v);
	setchildcharvalue(x,name,tmp);
}
void setchildcharvalue(TiXmlNode *x,char *name, const char* v)
{
	TiXmlNode *n;
	if (x->FirstChild(name) != NULL)
	{
		n = x->FirstChild(name);
	}
	else
	{
		TiXmlElement newnode(name);
		n = x->InsertEndChild(newnode);
	}
	
	setvalue(n, v);
}

void setvalue(TiXmlNode *n, const char* v)
{
	if (n == 0)
		return;

	if (n->FirstChild() != 0)
	{
		n->FirstChild()->SetValue(v);
	}
	else
	{
		TiXmlText txt(v);
		n->InsertEndChild(txt);
	}
}


const char *getname(TiXmlNode *x)
{
	if (x!= NULL)
	{
		if (x->FirstChild("name") != NULL)
		{
			if (x->FirstChild("name")->FirstChild() != NULL)
				return x->FirstChild("name")->FirstChild()->Value();
			else
				return "";
		}
		else
		{
			return "";
		}
	}
	else
	{
		return "";
	}
}

void setname(TiXmlNode *x, const char *name)
{
	if (x != NULL)
	{
		if (x->FirstChild("name") != NULL)
		{
			if (x->FirstChild("name")->FirstChild() != NULL)
			{
				x->FirstChild("name")->FirstChild()->SetValue(name);
			}
			else
			{
				TiXmlElement text( name );
				x->FirstChild("name")->InsertEndChild(text);
			}
		}
		else
		{
			TiXmlElement naam( "name" );
			TiXmlText text(	name );
			naam.InsertEndChild(text);
			x->InsertEndChild( naam );
		}
	}
}


// TinyXml safe accessor
const char *getattribute(TiXmlElement *x,char *name)
{
	if (x != 0)
	{
		if (x->Attribute(name) != NULL)
			return x->Attribute(name);
		else
			return "";
	}
	else
	{
		return "";
	}
}

