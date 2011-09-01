/*
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

/** \file KX_SCA_CLibraryController.h
 *  \ingroup ketsji
 *  \brief Execute C Library
 */

#ifndef KX_CLIBRARYCONTROLLER_H
#define KX_CLIBRARYCONTROLLER_H

	      
#include "SCA_IController.h"
#include "SCA_LogicManager.h"
#include "KX_GameObject.h"
#include "KX_KetsjiEngine.h"
#include "KX_Scene.h"
#include "KX_PythonInit.h"

extern "C" {
#include "BLI_dynlib.h"
}


class KX_SCA_CLibraryController: public SCA_IController
{

	Py_Header;

public:

	KX_SCA_CLibraryController(SCA_IObject* gameobj);
	virtual ~KX_SCA_CLibraryController();
	virtual CValue* GetReplica();
	
	virtual void Trigger(SCA_LogicManager* logicmgr);
	
	void		SetModulePath(const STR_String& text);
	void		SetFunctiontName(const STR_String& name);
	void		SetDebug(bool debug) { m_debug = debug; }
	STR_String	GetModulePath() {return m_libraryPath; }
	STR_String	GetFunctionName() { return m_functionName; }

#ifdef WITH_PYTHON
	static const char* sPyGetCurrentController__doc__;
	PyObject* sPyGetCurrentController(PyObject *self);

#endif


protected:
	
	typedef void (*moduleFunction)(KX_KetsjiEngine* curEngine, KX_Scene* curScene, KX_GameObject* curObject, SCA_IController* curController, char* error_msg);

	bool					m_modifiedPath;
	bool					m_modifiedFunction;
	bool					m_debug;
	STR_String				m_libraryPath;
	STR_String				m_functionName;

	DynamicLibrary*			m_library;
	moduleFunction			m_function;
	
	static KX_SCA_CLibraryController* m_sCurrentController;
	int		LoadCLibrary();
	void	FreeCLibrary();
	int		LoadFunction();
	void	ErrorPrint(char* error_msg);
	STR_String ResolvePath(STR_String path);
	
	char					m_lastReturnCode[1024];
};


#endif
