// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the BGE_MODULE_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// BGE_MODULE_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.

#include "externdef.h"

#include "KX_GameObject.h"
#include "KX_KetsjiEngine.h"
#include "KX_Scene.h"
#include "SCA_IController.h"

extern "C" {LIBEXPORT void bge_func(KX_KetsjiEngine* , KX_Scene* scene, KX_GameObject*, SCA_IController*, char*);}
