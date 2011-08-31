#if WITH_FBT
#ifndef _KX_CONVERTCLASSSTRUCT.CPP
#define _KX_CONVERTCLASSSTRUCT.CPP

#include "KX_ConvertClassStruct.h"
#include <list>
#include <vector>
#include "ListValue.h"
#include "STR_String.h"
#include "KX_GameObject.h"
#include "KX_FontObject.h"
#include "KX_Light.h"
#include "KX_Camera.h"
#include "KX_WorldInfo.h"
#include "BlenderWorldInfo.h"
#include "RAS_FramingManager.h"
#include "RAS_Rect.h"
#include "RAS_BucketManager.h"
#include "RAS_MaterialBucket.h"
#include "SG_Dlist.h"
#include "KX_FileInterface.h"

#define NEW NULL

Bgedna::fbtList* KX_ConvertClassStruct::fbtListFromCList(CListValue* clist, FBTuint16 ID)
{
	
	Bgedna::fbtList* retlist = new Bgedna::fbtList(ID);

	switch (ID)
	{
		case GAME_OBJECT :
			for (int i=0; i < clist->GetCount(); ++i)
			{
				retlist->push_back( convertGameObject((KX_GameObject*) (clist+i), NEW, true) );
			}
			break;

		case LIGHT_OBJECT :
			for (int i=0; i < clist->GetCount(); ++i)
			{

				retlist->push_back( convertLightObject((KX_LightObject*) (clist+i), NEW, true) );
			}
			break;

		default:
			break;
		
	}

	return retlist;

}

void* KX_ConvertClassStruct::convertClassToStruct(void* save_class, FBTuint16 ID)
{
	/*Why everithing is commented out? Because the conversion of "minor" types is handled inside the actual scene converter.
	I update this because maybe someday someone will need to convert a specific type.
	This function now only allow the conversion of the "main" types, the roots of the data to save( right now only scene, but
	i can discover that other things must be saved)*/
	switch (ID) 
	{
		case SCENE:
				return (void*) convertScene((KX_Scene*) save_class);
/*
		case CAMERA:
				return (void*) convertCamera((KX_Camera*) save_class);

		case BUCKET_MANAGER:
				return (void*) convertBucketManager((RAS_BucketManager*) save_class);

		case FONT_OBJECT:
				return (void*) convertFont((KX_FontObject*) save_class);
		
		case FRAME_SETTINGS:
				return (void*) convertFrameSettings((const Ras_BucketManager) save_class);

		case GAME_OBJECT:
				return (void*) convertGameObject((KX_GameObject*) save_class);

		case LIGHT_OBJECT:
				return (void*) convertLightObject((KX_LightObject*) save_class);

		to be updated
*/
		default:
			{
				break;
			}
	}

}

Bgedna::KX_SceneStruct* KX_ConvertClassStruct::convertScene(KX_Scene* scene, Bgedna::KX_SceneStruct* scene_struct, bool add_to_list)
{
	if(!scene_struct)
		Bgedna::KX_SceneStruct* scene_struct = new Bgedna::KX_SceneStruct();

	/*convert all attributes that are simple data or structs*/
	scene_struct->active_camera = *convertCameraObject(scene->GetActiveCamera());
	scene_struct->bucket_manager = *convertBucketManager(scene->GetBucketManager());
	scene_struct->camera_design_height = scene->GetCanvasDesignHeight();
	scene_struct->camera_design_width =  scene->GetCanvasDesignWidth();
	scene_struct->dbvt_culling = scene->GetDbvtCulling();
	scene_struct->dbvt_occlusion_res = scene->GetDbvtOcclusionRes();
	scene_struct->framing_type = *convertFrameSettings(scene->GetFramingType());
	scene_struct->pcamera = *convertCameraObject(scene->GetpCamera());
	scene_struct->suspended_delta = scene->getSuspendedDelta();
	scene_struct->suspended_time = scene->getSuspendedTime();
	scene_struct->scene_viewport = *convertRect(scene->GetSceneViewport());
	scene_struct->time_event_manager = *convertTimeEvManager(scene->GetTimeEventManager());
	scene_struct->world_info = *convertWorldInfo(scene->GetWorldInfo());

	STR_String string = scene->GetName();
	strncpy(scene_struct->name, string.Ptr(), 64);
	delete(&string);
	
	/*start converting the attributes in lists*/

	//convert all cameras and put into a DataList
	Bgedna::fbtDataList* cameras = new Bgedna::fbtDataList(CAMERA);
	std::list<class KX_Camera*> camera_list = (std::list<class KX_Camera*>) *(scene->GetCameras());
	std::list<class KX_Camera*>::iterator cam_it = camera_list.begin();
	while(cam_it!= camera_list.end())
	{
		cameras->add_data(convertCameraObject(*cam_it));
		cam_it++;
	}
	scene_struct->cameras = *cameras;
	delete(&cam_it);
	delete(&camera_list);

	//convert all fonts and put into Datalist
	Bgedna::fbtDataList* fonts = new Bgedna::fbtDataList(FONT_OBJECT);
	std::list<class KX_FontObject*> font_list = (std::list<class KX_FontObject*>) *(scene->GetFonts());
	std::list<class KX_FontObject*>::iterator fon_it = font_list.begin();
	while(fon_it != font_list.end() )
	{
		fonts->add_data(convertFont(*fon_it));
		fon_it++;
	}
	scene_struct->fonts = *fonts;
	delete(&fon_it);
	delete(&font_list);
	
	scene_struct->inactive_list = *(fbtListFromCList(scene->GetInactiveList(), GAME_OBJECT));
	scene_struct->light_list = *(fbtListFromCList(scene->GetLightList(), LIGHT_OBJECT));
	scene_struct->object_list = *(fbtListFromCList(scene->GetObjectList(), GAME_OBJECT));
	
	/*need to find a workaround for this: can't save the whole scene, but it is used also outside of conversion...* / 
	scene->GetBlenderScene();
	*/
	/*rootParentList can contain all types of elements: game_objects, lights, cameras... How can i know what type is the element i'm processing?
	Maybe i must find a way to directly save CValues. Other question: can i recostruct a CListValue with only the elements i save? If not, storing the additional
	infos required inside fbtDataList attributes or inside a DataLink attributes is absolutely a no problem(i'll create a fbtCValueList that will have the cvaluelist_infos attribute)
	and i can write a cvaluelistinfos struct to contain this data), but i don't know what to store.. 
	I think	when it will come to recostructing the scene from the saved data i'll know what i need.* /
	scene->GetRootParentList();*/
	scene_struct->temp_obj_list = *(fbtListFromCList(scene->GetTempObjectList(), GAME_OBJECT));

	/*IMPORTANT:
	All the data is actually stored into the KX_FileInterface lists. The conversion functions only return pointers to where the data is saved.
	This is done in order to save each element only once, and to avoid conversion loops in wich struct A contains struct B
	and struct B contains struct A.*/
	if(add_to_list)
	{
		m_finterface->m_scene.push_back(scene_struct);
		return (Bgedna::KX_SceneStruct*) m_finterface->m_scene.last;
	}
	return scene_struct;
}

KX_Scene* KX_ConvertClassStruct::convertSceneStruct(Bgedna::KX_SceneStruct* scene, KX_Scene* scene)
{
	
}

Bgedna::KX_CameraStruct* KX_ConvertClassStruct::convertCameraObject(KX_Camera* camera, Bgedna::KX_CameraStruct* camera_struct, bool add_to_list)
{

}

Bgedna::KX_FontObjectStruct* KX_ConvertClassStruct::convertFont(KX_FontObject* font, Bgedna::KX_FontObjectStruct* font_struct, bool add_to_list)
{
	
}

Bgedna::KX_LightObjectStruct* KX_ConvertClassStruct::convertLightObject(KX_LightObject* light, Bgedna::KX_LightObjectStruct* light_struct, bool add_to_list)
{

}

Bgedna::KX_GameObjectStruct* KX_ConvertClassStruct::convertGameObject(KX_GameObject* game_obj, Bgedna::KX_GameObjectStruct* game_obj_struct, bool add_to_list)
{

}

//continue correcting from here
Bgedna::RAS_FrameSettingsStruct* KX_ConvertClassStruct::convertFrameSettings(const RAS_FrameSettings frsets)
{
	Bgedna::RAS_FrameSettingsStruct* frsets_struct = new Bgedna::RAS_FrameSettingsStruct();
	
	frsets_struct->m_frame_type = frsets.FrameType();

	frsets_struct->bar[0] = frsets.BarRed();
	frsets_struct->bar[1] = frsets.BarGreen();
	frsets_struct->bar[2] = frsets.BarBlue();

	frsets_struct->m_design_aspect_height = frsets.DesignAspectHeight();
	frsets_struct->m_design_aspect_width = frsets.DesignAspectWidth();

	return frsets_struct;

}

const RAS_FrameSettings KX_ConvertClassStruct::convertFrameSettingsStruct(Bgedna::RAS_FrameSettingsStruct* frsets_struct)
{
	const RAS_FrameSettings frsets= RAS_FrameSettings( (RAS_FrameSettings.RAS_FrameType) frsets_struct->m_frame_type, 
														frsets_struct->bar[0], 
														frsets_struct->bar[1], 
														frsets_struct->bar[2], 
														frsets_struct->m_design_aspect_width,
														frsets_struct->m_design_aspect_height);
	return frsets;
}

Bgedna::RAS_RectStruct* KX_ConvertClassStruct::convertRect(const RAS_Rect scene_viewport)
{
	Bgedna::RAS_RectStruct* scene_viewport_struct = new Bgedna::RAS_RectStruct();
	scene_viewport_struct->m_x1 = scene_viewport.GetLeft();
	scene_viewport_struct->m_x2 = scene_viewport.GetRight();
	scene_viewport_struct->m_y1 = scene_viewport.GetBottom();
	scene_viewport_struct->m_y2 = scene_viewport.GetTop();
	return scene_viewport_struct;
}

const RAS_Rect KX_ConvertClassStruct::convertRectStruct(Bgedna::RAS_RectStruct* scene_viewport_struct)
{
	const RAS_Rect scene_viewport = RAS_Rect(scene_viewport_struct->m_x1,
											scene_viewport_struct->m_y1,
											scene_viewport_struct->m_x2,
											scene_viewport_struct->m_y2);
	return scene_viewport;
}

Bgedna::SCA_TimeEventManagerStruct* KX_ConvertClassStruct::convertTimeEvManager(SCA_TimeEventManager* time_mng)
{

}

Bgedna::RAS_BucketManagerStruct* KX_ConvertClassStruct::convertBucketManager(RAS_BucketManager* bucket_manager)
{
	Bgedna::RAS_BucketManagerStruct* bucket_manager_struct = new Bgedna::RAS_BucketManagerStruct();
	
	Bgedna::fbtDataList* solid_bucket_list_struct = new Bgedna::fbtDataList(MATERIAL_BUCKET);
	std::vector<class RAS_MaterialBucket*> solid_bucket_list = (std::vector<class RAS_MaterialBucket*>) (bucket_manager->GetSolidBuckets());
	std::vector<class RAS_MaterialBucket*>::iterator buk_it = solid_bucket_list.begin();
	while(buk_it != solid_bucket_list.end() )
	{
		solid_bucket_list_struct->add_data(convertMaterialBucket(*buk_it));
		buk_it++;
	}
	bucket_manager_struct->solid_bucket_material_list = *solid_bucket_list_struct;
	delete(&solid_bucket_list);

	Bgedna::fbtDataList* alpha_bucket_list_struct = new Bgedna::fbtDataList(MATERIAL_BUCKET);
	std::vector<class RAS_MaterialBucket*> alpha_bucket_list = (std::vector<class RAS_MaterialBucket*>) (bucket_manager->GetAlphaBuckets());
	buk_it = alpha_bucket_list.begin();
	while(buk_it != alpha_bucket_list.end() )
	{
		alpha_bucket_list_struct->add_data(convertMaterialBucket(*buk_it));
		buk_it++;
	}
	bucket_manager_struct->alpha_bucket_material_list = *alpha_bucket_list_struct;
	delete(&alpha_bucket_list);
	
	delete(&buk_it);

	return bucket_manager_struct;
}

RAS_BucketManager* KX_ConvertClassStruct::convertBucketManagerStruct(Bgedna::RAS_BucketManagerStruct* bucket_manager_struct)
{
	RAS_BucketManager* bucket_manager = new RAS_BucketManager();
	std::vector<class RAS_MaterialBucket*> solid_list;
	Bgedna::DataList* bms;
	for( bms = bucket_manager_struct->solid_bucket_material_list.first; bms; bms = bms->next)
	{
		solid_list.push_back(convertMaterialBucketStruct((Bgedna::RAS_MaterialBucketStruct*) bms->data));
	}
	bucket_manager->SetSolidBuckets(solid_list);

	std::vector<class RAS_MaterialBucket*> alpha_list;
	for(bms = bucket_manager_struct->alpha_bucket_material_list.first; bms; bms = bms->next)
	{
		alpha_list.push_back(convertMaterialBucketStruct((Bgedna::RAS_MaterialBucketStruct*) bms->data));
	}
	bucket_manager->SetAlphaBuckets(alpha_list);
	
	delete(&bms);
	return bucket_manager;

}


Bgedna::RAS_MaterialBucketStruct* KX_ConvertClassStruct::convertMaterialBucket(RAS_MaterialBucket* material_bucket)
{
	Bgedna::RAS_MaterialBucketStruct* material_bucket_stuct = new Bgedna::RAS_MaterialBucketStruct();
	material_bucket_stuct->isAlpha =  material_bucket->IsAlpha();
	material_bucket_stuct->isSorted = material_bucket->IsZSort();
	material_bucket_stuct->material = *convertIPolyMaterial(material_bucket->GetPolyMaterial());

	Bgedna::fbtDataList* act_mesh_slot = new Bgedna::fbtDataList(MESH_SLOT);
	SG_DList::iterator<class RAS_MeshSlot> act_mesh_slot(material_bucket->GetActiveMeshSlots());//assume this is the head of the element
	for(act_mesh_slot.begin(); !act_mesh_slot.end(); ++act_mesh_slot)
	{
		act_mesh_slot->add_data(convertMeshSlot(*act_mesh_it));
	}
	material_bucket_stuct->act_mesh_slot = *act_mesh_slot;

	Bgedna::fbtDataList* mesh_slot = new Bgedna::fbtDataList(MESH_SLOT);
	std::list<class RAS_MeshSlot>::iterator mesh_it = material_bucket->msBegin();
	for(mesh_it; mesh_it!= material_bucket->msEnd(); ++mesh_it)
	{
		mesh_slot->add_data(convertMeshSlot(*mesh_it));
	}
	material_bucket_stuct->mesh_slot = *mesh_slot;
	delete(&mesh_it);

}

RAS_MaterialBucket*	KX_ConvertClassStruct::convertMaterialBucketStruct(Bgedna::RAS_MaterialBucketStruct* material_bucket_struct)
{

}

Bgedna::KX_WorldInfoStruct*	KX_ConvertClassStruct::convertWorldInfo(KX_WorldInfo* winfo)
{
	Bgedna::KX_WorldInfoStruct* winfo_struct = new Bgedna::KX_WorldInfoStruct();

	winfo_struct->ambient_color[2] =  winfo->getAmbientColorBlue();
	winfo_struct->ambient_color[1] =  winfo->getAmbientColorGreen();
	winfo_struct->ambient_color[0] =  winfo->getAmbientColorRed();

	winfo_struct->back_color[2] = winfo->getBackColorBlue();
	winfo_struct->back_color[1] = winfo->getBackColorGreen();
	winfo_struct->back_color[0] = winfo->getBackColorRed();

	winfo_struct->has_world = winfo->hasWorld();

	if(winfo->hasMist())
	{
		winfo_struct->has_mist = true;
		winfo_struct->mist_distance = winfo->getMistDistance();
		winfo_struct->mist_start = winfo->getMistStart();

		winfo_struct->mist_color[2] = winfo->getMistColorBlue();
		winfo_struct->mist_color[1] = winfo->getMistColorGreen();
		winfo_struct->mist_color[0] = winfo->getMistColorRed();

	}
	else
		winfo_struct->has_mist = false;

	winfo_struct->has_world = winfo->hasWorld();

	return winfo_struct;
}

KX_WorldInfo* KX_ConvertClassStruct::convertWorldInfoStruct(Bgedna::KX_WorldInfoStruct* winfo_struct)
{
	BlenderWorldInfo* winfo = new BlenderWorldInfo(NULL, NULL);
	winfo->setBackColor(winfo_struct->back_color[0],winfo_struct->back_color[1],winfo_struct->back_color[2]);
	if(winfo_struct->has_world)
	{
		winfo->setHasWorld(true);
		if(winfo_struct->has_mist)
		{	
			winfo->setHasMist(true);
			winfo->setMistDistance(winfo_struct->mist_distance);
			winfo->setMistStart(winfo_struct->mist_start);
			winfo->setMistColorBlue(winfo_struct->mist_color[2]);
			winfo->setMistColorGreen(winfo_struct->mist_color[1]);
			winfo->setMistColorRed(winfo_struct->mist_color[0]);
		}

		else
		{
			winfo->setHasMist(false);
			winfo->setMistDistance(0.0);
			winfo->setMistStart(0.0);
			winfo->setMistColorBlue(0);
			winfo->setMistColorGreen(0);
			winfo->setMistColorRed(0);
		}
	}
	else
		winfo->setHasWorld(false);

	return winfo;

}
#endif //_KX_CONVERTCLASSSTRUCT.CPP
#endif