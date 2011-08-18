/*
 * Execute C Library 
 *
 * $Id: KX_SCA_CLibraryController.cpp 2011-08-03 11:17:25Z Makers_F $
 *
 * ***** BEGIN GPL LICENSE BLOCK *****
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * The Original Code is Copyright (C) 2001-2002 by NaN Holding BV.
 * All rights reserved.
 *
 * The Original Code is: all of this file.
 *
 * Contributor(s): none yet.
 *
 * ***** END GPL LICENSE BLOCK *****
 */

/** \file gameengine/Ketsji/SCA_CLibraryController.cpp
 *  \ingroup ketsji
 */

#include "KX_SCA_CLibraryController.h"
#include "SCA_LogicManager.h"
#include "SCA_ISensor.h"
#include "SCA_IActuator.h"
#include "PyObjectPlus.h"
#include "STR_String.h"
extern "C" {
#include "BLI_path_util.h"
#include "BKE_global.h"
#include "BKE_main.h"
#include "BLI_string.h"
}

// initialize static member variables
KX_SCA_CLibraryController* KX_SCA_CLibraryController::m_sCurrentController = NULL;

KX_SCA_CLibraryController::KX_SCA_CLibraryController(SCA_IObject* gameobj)
	: SCA_IController(gameobj),
	m_library(NULL),
	m_function(NULL),
	m_debug(false),
	m_modifiedPath(true),
	m_modifiedFunction(true)
{
	
}

KX_SCA_CLibraryController::~KX_SCA_CLibraryController()
{
	FreeCLibrary();
}

CValue* KX_SCA_CLibraryController::GetReplica()
{
	KX_SCA_CLibraryController* replica = new KX_SCA_CLibraryController(*this);
	// this will copy properties and so on...
	replica->ProcessReplica();
	return replica;
}

void KX_SCA_CLibraryController::SetModulePath(const STR_String& text)
{
	m_libraryPath = text;
	m_modifiedPath = true;
}

void KX_SCA_CLibraryController::SetFunctiontName(const STR_String& name)
{
	m_functionName = name;
	m_modifiedFunction = true;
}

STR_String KX_SCA_CLibraryController::ResolvePath(STR_String path)
{

	/*size of start path + size of abs path + extension dimension + 8 bonus char for safeness*/
#ifdef WIN32
	int totlen = (path.Length() + strlen(G.main->name) + 5 + 8);
	char *temp = (char*) malloc(sizeof(char)*totlen);
#else
	int totlen = (path.Length() + strlen(G.main->name) + 4 + 8);
	char *temp = (char*) malloc(sizeof(char)*totlen);
#endif

	strncpy(temp, path.Ptr(), totlen);
	/*make the pat absolute*/
	BLI_path_abs(temp, G.main->name);

	/*add the right extension*/
#ifdef WIN32
	BLI_replace_extension(temp, totlen, ".dll");
#else
	BLI_replace_extension(temp, totlen, ".so");
#endif

	STR_String retstring = STR_String(temp);
	free(temp);

	return retstring;
}


int KX_SCA_CLibraryController::LoadCLibrary()
{

	STR_String resolvedpath = ResolvePath(m_libraryPath);
	m_library = (*BLI_dynlib_open)(resolvedpath.Ptr());

	if(m_library!=NULL)
	{
		m_modifiedPath = false;
		return 1;
	}
	else
	{
		printf("Failed to load %s library.",resolvedpath.Ptr());
		ErrorPrint(NULL);
		return 0; 
	}
}

void KX_SCA_CLibraryController::FreeCLibrary()
{
	if(m_library)
		BLI_dynlib_close(m_library);
}

int KX_SCA_CLibraryController::LoadFunction()
{
	if(!m_library) 
	{	
		ErrorPrint("The library was not loaded!");
		return 0;
	}
	
	m_function = (moduleFunction)(*BLI_dynlib_find_symbol)(m_library, m_functionName.Ptr());
	
	static char* error_message;
	error_message = (*BLI_dynlib_get_error_as_string)(m_library);
	
	if(!error_message)
	{
		m_modifiedFunction = false;
		return 1;
	}
	else
	{
		printf("Error while loading the function: %s",error_message);
		ErrorPrint(NULL);
		return 0;
	}
}

void KX_SCA_CLibraryController::ErrorPrint(char* error_msg)
{
	if(error_msg)
		printf("%s - object '%s', controller '%s', library '%s', function '%s'\n", error_msg, GetParent()->GetName().Ptr(), GetName().Ptr(), m_libraryPath.Ptr(), m_functionName.Ptr());
	else
		printf(" - object '%s', controller '%s', library '%s', function '%s'\n", GetParent()->GetName().Ptr(), GetName().Ptr(), m_libraryPath.Ptr(), m_functionName.Ptr());
}

void KX_SCA_CLibraryController::Trigger(SCA_LogicManager* logicmgr)
{
	
	m_sCurrentController = this;
	m_sCurrentLogicManager = logicmgr;

	if(!m_libraryPath.Compare("") || !m_functionName.Compare(""))
	{
		ErrorPrint("Expected a library path and a function name, '' received instead.");
		m_sCurrentController = NULL;
		return;
	}
	
	/*If debug is true, reload the module each time*/
	if(m_debug)
	{
		/*Free the memory of the previous library*/
		this->FreeCLibrary();
		if(!this->LoadCLibrary()) return;
		if(!this->LoadFunction()) return;
	}
	/*If the path has been changed, reload the library*/
	if(m_modifiedPath)
	{
		this->FreeCLibrary();
		if(!this->LoadCLibrary()) return;
		/*Since the library is reloaded, the function's address may change*/
		if(!this->LoadFunction()) return;
	}
	/*If the function name has been changed, reload the function*/
	if(m_modifiedFunction)
		if(!this->LoadFunction()) return;
	
	
	m_lastReturnCode[0] = NULL;
	
	/*the actual execution of the function*/
	(this->m_function)(KX_GetActiveEngine(), KX_GetActiveScene(), (KX_GameObject*) this->GetParent(), (SCA_IController*) this, m_lastReturnCode);
	
	switch(*m_lastReturnCode)
	{
		case NULL:
		{
			ErrorPrint("An error occourred while executing the library function");
			break;
		}
		case '1':
			/*everything went fine*/
			break;
		default:
		{
			/*Print the error string given by the library function*/
			ErrorPrint(m_lastReturnCode);
			break;
		}
	}
	m_sCurrentController = NULL;
}

#ifdef WITH_PYTHON

/* warning, self is not the SCA_CLibraryController, its a PyObjectPlus_Proxy */

PyObject* KX_SCA_CLibraryController::sPyGetCurrentController(PyObject *self)
{
	if(m_sCurrentController==NULL)
	{
		PyErr_SetString(PyExc_SystemError, "bge.logic.getCurrentController(), this function is being run outside the python controllers context, or blenders internal state is corrupt.");
		return NULL;
	}
	return m_sCurrentController->GetProxy();
}

const char* KX_SCA_CLibraryController::sPyGetCurrentController__doc__ = "getCurrentController()";

PyTypeObject KX_SCA_CLibraryController::Type = {
	PyVarObject_HEAD_INIT(NULL, 0)
	"KX_SCA_CLibraryController",
	sizeof(PyObjectPlus_Proxy),
	0,
	py_base_dealloc,
	0,
	0,
	0,
	0,
	py_base_repr,
	0,0,0,0,0,0,0,0,0,
	Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
	0,0,0,0,0,0,0,
	Methods,
	0,
	0,
	&SCA_IController::Type,
	0,0,0,0,0,0,
	py_base_new
};

PyMethodDef KX_SCA_CLibraryController::Methods[] = {
	{NULL,NULL} //Sentinel
};

PyAttributeDef KX_SCA_CLibraryController::Attributes[] = {
	KX_PYATTRIBUTE_BOOL_RW("debug", KX_SCA_CLibraryController, m_debug),
	KX_PYATTRIBUTE_STRING_RW("module_path", 0, 63, false, KX_SCA_CLibraryController, m_libraryPath),
	KX_PYATTRIBUTE_STRING_RW("function_name", 0, 31, false, KX_SCA_CLibraryController, m_functionName),
	{NULL}

};

#endif
