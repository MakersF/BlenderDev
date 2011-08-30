/*BGE_TestModule.cpp : Defines the exported functions for the DLL application.*/


#include "BGE_TestModule.h"
#include <iostream>
#include "FloatValue.h"
#include "MT_Vector3.h"
#include "MT_Scalar.h"

/* *msg = '1'; -> Everything fine
msg = "Error description"; -> error log that will be printed*/
extern "C" LIBEXPORT void bge_func(KX_KetsjiEngine* engine, KX_Scene* scene, KX_GameObject* object, SCA_IController* cont, char* msg)
{
	cout<<"I did it!\n";

	cout<<"Set linear velocity";
	object->addLinearVelocity(MT_Vector3(MT_Scalar(10),MT_Scalar(0),MT_Scalar(0)),false);

	cout<<"Spawn new object";
	KX_GameObject *new_spawn;
	new_spawn = (KX_GameObject*)scene->AddReplicaObject(object, object, 0 );
	const float pos = 5.0;
	new_spawn->NodeSetWorldPosition( MT_Point3(&pos) );

	cout<<"Set linear velocity to new object";
	new_spawn->ApplyForce(MT_Vector3(MT_Scalar(0),MT_Scalar(-4),MT_Scalar(0)),false);

	cout<<"Set a timebomb";
	scene->GetTempObjectList()->Add(object->AddRef());
	CValue *fval = new CFloatValue(10);
	object->SetProperty("::timebomb",fval);
	fval->Release();


	cout<<"Force a render";
	engine->Render();

	strcpy(msg,"Everything gone fine");
}
