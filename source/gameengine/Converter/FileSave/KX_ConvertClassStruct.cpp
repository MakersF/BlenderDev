#ifdef WITH_FBT

#include "KX_ConvertClassStruct.h"
#include "KX_BGEdna.h"
#define CREATE_NEW NULL

void KX_ConvertClassStruct::notifyConverted(void* original, void* converted, FBTuint16 ID)
{
	/*if(checkUnique(original, ID)) return; 
	/*commented out because i assume that
	you don't notify data if it is already converted. You should check at the beginning of
	the conversion function if it is actually already been converted.*/

	std::vector<PointerIDList>::iterator it;
	PointerIDList ID_list;
	/*search for the list of that kind of ID*/
	for(it = converted_list.begin(); it!=converted_list.end(); ++it)
	{
		if( ( (PointerIDList) *it).ID== ID)
			break;
	}
	ID_list = ( PointerIDList) *it;
	
	if(it==converted_list.end())
	{
		/*it didn't find the correct list. Add it*/
		ID_list = PointerIDList(ID);
		converted_list.push_back(ID_list);
	}

	PointerCouple new_pointerCouple = PointerCouple();
	new_pointerCouple.converted = converted;
	new_pointerCouple.original = original;
	/* add the new couple to the list.*/
	ID_list.pointer_pairs.push_back(new_pointerCouple);

}

void* KX_ConvertClassStruct::checkUnique(void* original, FBTuint16 ID)
{
	std::vector<PointerIDList>::iterator it;
	/*search for the list of that kind of ID*/
	for(it = converted_list.begin(); it!=converted_list.end(); ++it)
	{
		if( ( (PointerIDList) *it).ID== ID)
			break;
	}
	
	if(it==converted_list.end())
		return NULL; /*the list doesn't exist, so no object have been registered so far*/

	PointerCoupleList::iterator pc_it;
	for(pc_it = ((PointerIDList) *it).pointer_pairs.begin(); pc_it != ((PointerIDList) *it).pointer_pairs.end(); ++pc_it)
	{
		if( ((PointerCouple) *pc_it).original == original)
			return ((PointerCouple) *pc_it).converted;
	}

	/*nothing found*/
	return NULL;
	
}

Bgedna::fbtList* KX_ConvertClassStruct::CListTofbtList(CListValue* clist, FBTuint16 ID)
{
	
	Bgedna::fbtList* retlist = new Bgedna::fbtList(ID);

	switch (ID)
	{
		case GAME_OBJECT :
			for (int i=0; i < clist->GetCount(); ++i)
			{
				retlist->push_back( convertGameObject((KX_GameObject*) (clist+i), CREATE_NEW, true) );
			}
			break;

		case LIGHT_OBJECT :
			for (int i=0; i < clist->GetCount(); ++i)
			{

				retlist->push_back( convertLightObject((KX_LightObject*) (clist+i), CREATE_NEW, true) );
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
			{
				return (void*) convertScene((KX_Scene*) save_class, CREATE_NEW, true);
			}
/*
		case CAMERA:
				return (void*) convertCamera((KX_Camera*) save_class, CREATE_NEW, true);

		case BUCKET_MANAGER:
				return (void*) convertBucketManager((RAS_BucketManager*) save_class, CREATE_NEW, true);

		case FONT_OBJECT:
				return (void*) convertFont((KX_FontObject*) save_class, CREATE_NEW, true);
		
		case FRAME_SETTINGS:
				return (void*) convertFrameSettings((const Ras_BucketManager) save_class, CREATE_NEW, true);

		case GAME_OBJECT:
				return (void*) convertGameObject((KX_GameObject*) save_class, CREATE_NEW, true);

		case LIGHT_OBJECT:
				return (void*) convertLightObject((KX_LightObject*) save_class, CREATE_NEW, true);

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
	//if nothing is passed that means that you need to store nothing
	if(!scene)
		return NULL;

	if(!scene_struct)
	{
		scene_struct = new Bgedna::KX_SceneStruct();

		/*Why into this if statement? Because you enter in this if statement only if this is the most derived type class converting function.
		In fact to convert derived class you call the function that converts it, and so you don't want to notify or check if it already exist,
		since this is done in the original function. For example, if you want to convert a LightObject you'll call the GameObject convert function,
		and you don't want to register your LightObject into the GameObject list, and you already know that the GameObject list won't contain
		an already converted LightObject*/
		Bgedna::KX_SceneStruct* check_scene;
		check_scene = (Bgedna::KX_SceneStruct*) checkUnique((void*) scene, SCENE);
		if(check_scene != NULL)
			return check_scene;
	}

	/*convert all attributes that are simple data or structs*/
	scene_struct->active_camera = convertCameraObject(scene->GetActiveCamera(), CREATE_NEW, true);
	scene_struct->bucket_manager = convertBucketManager(scene->GetBucketManager(), CREATE_NEW, true);
	scene_struct->camera_design_height = scene->GetCanvasDesignHeight();
	scene_struct->camera_design_width =  scene->GetCanvasDesignWidth();
	scene_struct->dbvt_culling = scene->GetDbvtCulling();
	scene_struct->dbvt_occlusion_res = scene->GetDbvtOcclusionRes();
	scene_struct->framing_type = convertFrameSettings(scene->GetFramingType(), CREATE_NEW, true);
	scene_struct->pcamera = convertCameraObject(scene->GetpCamera(), CREATE_NEW, true);
	scene_struct->suspended_delta = scene->getSuspendedDelta();
	scene_struct->suspended_time = scene->getSuspendedTime();
	scene_struct->scene_viewport = convertRect(scene->GetSceneViewport(), CREATE_NEW, true);
	scene_struct->time_event_manager = convertTimeEvManager(scene->GetTimeEventManager(), CREATE_NEW, true);
	scene_struct->world_info = convertWorldInfo(scene->GetWorldInfo(), CREATE_NEW, true);

	STR_String string = scene->GetName();
	strncpy(scene_struct->name, string.Ptr(), 128);
	delete(&string);
	
	/*start converting the attributes in lists*/

	//convert all cameras and put into a DataList
	Bgedna::fbtList* cameras = new Bgedna::fbtList(CAMERA);
	std::list<class KX_Camera*> camera_list = (std::list<class KX_Camera*>) *(scene->GetCameras());
	std::list<class KX_Camera*>::iterator cam_it = camera_list.begin();
	while(cam_it!= camera_list.end())
	{
		cameras->push_back(convertCameraObject(*cam_it, CREATE_NEW, true));
		cam_it++;
	}
	scene_struct->cameras = *cameras;
	delete(&cam_it);
	delete(&camera_list);

	//convert all fonts and put into Datalist
	Bgedna::fbtList* fonts = new Bgedna::fbtList(FONT_OBJECT);
	std::list<class KX_FontObject*> font_list = (std::list<class KX_FontObject*>) *(scene->GetFonts());
	std::list<class KX_FontObject*>::iterator fon_it = font_list.begin();
	while(fon_it != font_list.end() )
	{
		fonts->push_back(convertFont(*fon_it, CREATE_NEW, true));
		fon_it++;
	}
	scene_struct->fonts = *fonts;
	delete(&fon_it);
	delete(&font_list);
	
	scene_struct->inactive_list = *(CListTofbtList(scene->GetInactiveList(), GAME_OBJECT));
	scene_struct->light_list = *(CListTofbtList(scene->GetLightList(), LIGHT_OBJECT));
	scene_struct->object_list = *(CListTofbtList(scene->GetObjectList(), GAME_OBJECT));
	
	/*need to find a workaround for this: can't save the whole scene, but it is used also outside of conversion...
	I'll create a struct that will contain all the needed things.
	I really think that the engine shouldn't require *ANYTHING* that isn't converted or is a blender strict type:
	imagine if someone writes a new converter for another format, s/he would need to create also a blender scene
	only because a few functions require it..
	Need a bit of time to look at it, let's do it later* / 
	scene->GetBlenderScene();
	*/

	/*rootParentList can contain all types of elements: game_objects, lights, cameras(even fonts are cvalues.. Quite everything is a cvalue..)
	Questions: How can i know what type is the element i'm processing?
	Can i recostruct a CListValue with only the CValue elements i save?* /
	scene->GetRootParentList();*/

	scene_struct->temp_obj_list = *(CListTofbtList(scene->GetTempObjectList(), GAME_OBJECT));

	/*IMPORTANT:
	All the data is actually stored into the KX_FileInterface lists. The conversion functions only return pointers to where the data is saved.
	This is done in order to save each element only once, and to avoid conversion loops in wich struct A contains struct B
	and struct B contains struct A.*/
	if(add_to_list)
	{
		notifyConverted((void*) scene, (void*) scene_struct, SCENE);
		m_finterface->m_scene.push_back(scene_struct);
		return (Bgedna::KX_SceneStruct*) m_finterface->m_scene.last;
	}
	return scene_struct;
}

KX_Scene* KX_ConvertClassStruct::convertSceneStruct(Bgedna::KX_SceneStruct* scene_struct, KX_Scene* scene)
{
	
}

Bgedna::KX_CameraStruct* KX_ConvertClassStruct::convertCameraObject(KX_Camera* camera, Bgedna::KX_CameraStruct* camera_struct, bool add_to_list)
{

}

KX_Camera* KX_ConvertClassStruct::convertCameraObjectStruct(Bgedna::KX_CameraStruct* camera_struct, KX_Camera* camera)
{

}

Bgedna::KX_FontObjectStruct* KX_ConvertClassStruct::convertFont(KX_FontObject* font, Bgedna::KX_FontObjectStruct* font_struct, bool add_to_list)
{
	
}

KX_FontObject* KX_ConvertClassStruct::convertFontStruct(Bgedna::KX_FontObjectStruct* font_struct, KX_FontObject*	font)
{

}

Bgedna::KX_LightObjectStruct* KX_ConvertClassStruct::convertLightObject(KX_LightObject* light, Bgedna::KX_LightObjectStruct* light_struct, bool add_to_list)
{

}

KX_LightObject* KX_ConvertClassStruct::convertLightObjectStruct(Bgedna::KX_LightObjectStruct* light_struct, KX_LightObject* light)
{

}

Bgedna::KX_GameObjectStruct* KX_ConvertClassStruct::convertGameObject(KX_GameObject* game_obj, Bgedna::KX_GameObjectStruct* game_obj_struct, bool add_to_list)
{

}

KX_GameObject* KX_ConvertClassStruct::convertGameObjectStruct(Bgedna::KX_GameObjectStruct* game_obj_struct, KX_GameObject* game_obj)
{

}

Bgedna::CValueStruct* KX_ConvertClassStruct::convertCValue(CValue* cvalue, Bgedna::CValueStruct* cvalue_struct = NULL , bool add_to_list = true)
{

}

CValue* KX_ConvertClassStruct::convertCValueStruct(Bgedna::CValueStruct* cvalue_struct, CValue* cvalue)
{

}

Bgedna::RAS_FrameSettingsStruct* KX_ConvertClassStruct::convertFrameSettings(RAS_FrameSettings* frsets, Bgedna::RAS_FrameSettingsStruct* frsets_struct, bool add_to_list)
{
	if(!frsets)
		return NULL;

	if(!frsets_struct)
	{
		Bgedna::RAS_FrameSettingsStruct* frsets_struct = new Bgedna::RAS_FrameSettingsStruct();

		
		Bgedna::RAS_FrameSettingsStruct* already_converted;
		already_converted = (Bgedna::RAS_FrameSettingsStruct*) checkUnique((void*) frsets_struct, FRAME_SETTINGS);
		if(already_converted != NULL)
			return already_converted;
	}
	
	frsets_struct->m_frame_type = frsets->FrameType();

	frsets_struct->bar[0] = frsets->BarRed();
	frsets_struct->bar[1] = frsets->BarGreen();
	frsets_struct->bar[2] = frsets->BarBlue();

	frsets_struct->m_design_aspect_height = frsets->DesignAspectHeight();
	frsets_struct->m_design_aspect_width = frsets->DesignAspectWidth();

	if(add_to_list)
	{
		notifyConverted((void*) &frsets, (void*) frsets_struct, FRAME_SETTINGS);
		m_finterface->m_frameSettings.push_back(frsets_struct);
		return (Bgedna::RAS_FrameSettingsStruct*) m_finterface->m_frameSettings.last;
	}
	return frsets_struct;

}

RAS_FrameSettings* KX_ConvertClassStruct::convertFrameSettingsStruct(Bgedna::RAS_FrameSettingsStruct* frsets_struct, RAS_FrameSettings* frsets)
{
	if(!frsets_struct)
		return NULL;

	if(!frsets)
		frsets= new RAS_FrameSettings( (RAS_FrameSettings.RAS_FrameType) frsets_struct->m_frame_type, 
														frsets_struct->bar[0], 
														frsets_struct->bar[1], 
														frsets_struct->bar[2], 
														frsets_struct->m_design_aspect_width,
														frsets_struct->m_design_aspect_height);
	return frsets;
}

Bgedna::RAS_RectStruct* KX_ConvertClassStruct::convertRect(RAS_Rect* scene_viewport, Bgedna::RAS_RectStruct* scene_viewport_struct, bool add_to_list)
{

	if(!scene_viewport)
		return NULL;

	if(!scene_viewport_struct)
	{
		scene_viewport_struct = new Bgedna::RAS_RectStruct();

		
		Bgedna::RAS_RectStruct* already_converted;
		already_converted = (Bgedna::RAS_RectStruct*) checkUnique((void*) scene_viewport_struct, RAS_RECT);
		if(already_converted != NULL)
			return already_converted;
	}

	scene_viewport_struct->m_x1 = scene_viewport->GetLeft();
	scene_viewport_struct->m_x2 = scene_viewport->GetRight();
	scene_viewport_struct->m_y1 = scene_viewport->GetBottom();
	scene_viewport_struct->m_y2 = scene_viewport->GetTop();

	if(add_to_list)
	{
		notifyConverted((void*) &scene_viewport, (void*) scene_viewport_struct, RAS_RECT);
		m_finterface->m_rasRect.push_back(scene_viewport_struct);
		return (Bgedna::RAS_RectStruct*) m_finterface->m_rasRect.last;
	}

	return scene_viewport_struct;
}

RAS_Rect* KX_ConvertClassStruct::convertRectStruct(Bgedna::RAS_RectStruct* scene_viewport_struct, RAS_Rect* scene_viewport)
{
	if(!scene_viewport_struct)
		return NULL;

	if(!scene_viewport)
			scene_viewport = new RAS_Rect(scene_viewport_struct->m_x1,
											scene_viewport_struct->m_y1,
											scene_viewport_struct->m_x2,
											scene_viewport_struct->m_y2);
	return scene_viewport;
}

Bgedna::SCA_TimeEventManagerStruct* KX_ConvertClassStruct::convertTimeEvManager(SCA_TimeEventManager* time_mng, Bgedna::SCA_TimeEventManagerStruct* time_mng_struct, bool add_to_list)
{

}

SCA_TimeEventManager* KX_ConvertClassStruct::convertTimeEvManagerStruct(Bgedna::SCA_TimeEventManagerStruct* time_mng_struct, SCA_TimeEventManager* time_mng)
{

}

Bgedna::RAS_BucketManagerStruct* KX_ConvertClassStruct::convertBucketManager(RAS_BucketManager* bucket_manager, Bgedna::RAS_BucketManagerStruct* bucket_manager_struct, bool add_to_list)
{
	if(!bucket_manager)
		return NULL;

	if(!bucket_manager_struct)
	{
		bucket_manager_struct = new Bgedna::RAS_BucketManagerStruct();

		Bgedna::RAS_BucketManagerStruct* already_converted;
		already_converted = (Bgedna::RAS_BucketManagerStruct*) checkUnique((void*) bucket_manager_struct, BUCKET_MANAGER);
		if(already_converted != NULL)
			return already_converted;
	}
	
	Bgedna::fbtList* solid_bucket_list_struct = new Bgedna::fbtList(MATERIAL_BUCKET);
	std::vector<class RAS_MaterialBucket*> solid_bucket_list = (std::vector<class RAS_MaterialBucket*>) (bucket_manager->GetSolidBuckets());
	std::vector<class RAS_MaterialBucket*>::iterator buk_it = solid_bucket_list.begin();
	while(buk_it != solid_bucket_list.end() )
	{
		solid_bucket_list_struct->push_back(convertMaterialBucket(*buk_it, CREATE_NEW, true));
		buk_it++;
	}
	bucket_manager_struct->solid_bucket_material_list = *solid_bucket_list_struct;
	delete(&solid_bucket_list);

	Bgedna::fbtList* alpha_bucket_list_struct = new Bgedna::fbtList(MATERIAL_BUCKET);
	std::vector<class RAS_MaterialBucket*> alpha_bucket_list = (std::vector<class RAS_MaterialBucket*>) (bucket_manager->GetAlphaBuckets());
	buk_it = alpha_bucket_list.begin();
	while(buk_it != alpha_bucket_list.end() )
	{
		alpha_bucket_list_struct->push_back(convertMaterialBucket(*buk_it, CREATE_NEW, true));
		buk_it++;
	}
	bucket_manager_struct->alpha_bucket_material_list = *alpha_bucket_list_struct;
	delete(&alpha_bucket_list);
	
	delete(&buk_it);

	if(add_to_list)
	{
		notifyConverted((void*) bucket_manager, (void*) bucket_manager_struct, BUCKET_MANAGER);
		m_finterface->m_bucketManager.push_back(bucket_manager_struct);
		return (Bgedna::RAS_BucketManagerStruct*) m_finterface->m_bucketManager.last;
	}

	return bucket_manager_struct;
}

RAS_BucketManager* KX_ConvertClassStruct::convertBucketManagerStruct(Bgedna::RAS_BucketManagerStruct* bucket_manager_struct, RAS_BucketManager* bucket_manager)
{
	if(!bucket_manager_struct)
		return NULL;

	if(!bucket_manager)
		bucket_manager = new RAS_BucketManager();

	std::vector<class RAS_MaterialBucket*> solid_list;
	Bgedna::List* bms;
	for( bms = bucket_manager_struct->solid_bucket_material_list.first; bms; bms = bms->next)
	{
		solid_list.push_back(convertMaterialBucketStruct((Bgedna::RAS_MaterialBucketStruct*) bms, CREATE_NEW));
	}
	bucket_manager->SetSolidBuckets(solid_list);

	std::vector<class RAS_MaterialBucket*> alpha_list;
	for(bms = bucket_manager_struct->alpha_bucket_material_list.first; bms; bms = bms->next)
	{
		alpha_list.push_back(convertMaterialBucketStruct((Bgedna::RAS_MaterialBucketStruct*) bms, CREATE_NEW));
	}
	bucket_manager->SetAlphaBuckets(alpha_list);
	
	delete(&bms);
	return bucket_manager;

}

Bgedna::RAS_MaterialBucketStruct* KX_ConvertClassStruct::convertMaterialBucket(RAS_MaterialBucket* material_bucket, Bgedna::RAS_MaterialBucketStruct* material_bucket_struct, bool add_to_list)
{
	if(!material_bucket)
		return NULL;

	if(!material_bucket_struct)
	{
		material_bucket_struct = new Bgedna::RAS_MaterialBucketStruct();

		
		Bgedna::RAS_MaterialBucketStruct* already_converted;
		already_converted = (Bgedna::RAS_MaterialBucketStruct*) checkUnique((void*) material_bucket_struct, MATERIAL_BUCKET);
		if(already_converted != NULL)
			return already_converted;
	}

	
	material_bucket_struct->isAlpha =  material_bucket->IsAlpha();
	material_bucket_struct->isSorted = material_bucket->IsZSort();
	material_bucket_struct->material = convertIPolyMaterial(material_bucket->GetPolyMaterial());

	Bgedna::fbtList* act_mesh_slot = new Bgedna::fbtList(MESH_SLOT);
	SG_DList::iterator<class RAS_MeshSlot> act_mesh_it(material_bucket->GetActiveMeshSlots());//assume this is the head of the element
	for(act_mesh_it.begin(); !act_mesh_it.end(); ++act_mesh_slot)
	{
		act_mesh_slot->push_back(convertMeshSlot(*act_mesh_it, CREATE_NEW, true));
	}
	material_bucket_struct->act_mesh_slot = *act_mesh_slot;

	Bgedna::fbtList* mesh_slot = new Bgedna::fbtList(MESH_SLOT);
	std::list<class RAS_MeshSlot>::iterator* mesh_it = &(material_bucket->msBegin());
	for(*mesh_it; *mesh_it!= material_bucket->msEnd(); ++mesh_it)
	{
		mesh_slot->push_back(convertMeshSlot((RAS_MeshSlot*)mesh_it, CREATE_NEW, true));
	}
	material_bucket_struct->mesh_slot = *mesh_slot;
	delete(&mesh_it);

	if(add_to_list)
	{
		notifyConverted((void*) material_bucket, (void*) material_bucket_struct, MATERIAL_BUCKET);
		m_finterface->m_materialBucket.push_back(material_bucket_struct);
		return (Bgedna::RAS_MaterialBucketStruct*) m_finterface->m_materialBucket.last;
	}
	return material_bucket_struct;

}

RAS_MaterialBucket*	KX_ConvertClassStruct::convertMaterialBucketStruct(Bgedna::RAS_MaterialBucketStruct* material_bucket_struct, RAS_MaterialBucket* material_bucket)
{
	if(!material_bucket_struct)
		return NULL;

	if(!material_bucket)
		material_bucket = new RAS_MaterialBucket(convertIPolyMaterialStruct(material_bucket_struct->material, CREATE_NEW));

	Bgedna::RAS_MeshSlotStruct* elem;
	for(elem = (Bgedna::RAS_MeshSlotStruct*) (material_bucket_struct->act_mesh_slot).first; elem != material_bucket_struct->act_mesh_slot.last; elem = (Bgedna::RAS_MeshSlotStruct*) elem->next)
		material_bucket->ActivateMesh(convertMeshSlotStruct(elem, CREATE_NEW));

	for(elem = (Bgedna::RAS_MeshSlotStruct*) material_bucket_struct->mesh_slot.first; elem != material_bucket_struct->mesh_slot.last; elem = (Bgedna::RAS_MeshSlotStruct*) elem->next)
		material_bucket->CopyMesh(convertMeshSlotStruct(elem, CREATE_NEW));

	return material_bucket;

}

Bgedna::KX_WorldInfoStruct*	KX_ConvertClassStruct::convertWorldInfo(KX_WorldInfo* winfo, Bgedna::KX_WorldInfoStruct* winfo_struct, bool add_to_list)
{
	if(!winfo)
		return NULL;

	if(!winfo_struct)
	{
		winfo_struct = new Bgedna::KX_WorldInfoStruct();

		
		Bgedna::KX_WorldInfoStruct* already_converted;
		already_converted = (Bgedna::KX_WorldInfoStruct*) checkUnique((void*) winfo_struct, WORLD_INFO);
		if(already_converted != NULL)
			return already_converted;
	}
	

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

	if(add_to_list)
	{
		notifyConverted((void*) winfo, (void*) winfo_struct, WORLD_INFO);
		m_finterface->m_worldInfo.push_back(winfo_struct);
		return (Bgedna::KX_WorldInfoStruct*) m_finterface->m_worldInfo.last;
	}

	return winfo_struct;
}

KX_WorldInfo* KX_ConvertClassStruct::convertWorldInfoStruct(Bgedna::KX_WorldInfoStruct* winfo_struct, KX_WorldInfo* winfo_origin)
{
	if(!winfo_struct)
		return NULL;

	BlenderWorldInfo* winfo;
	if(winfo_origin)
		winfo = (BlenderWorldInfo*) winfo_origin;
	else
		winfo = new BlenderWorldInfo(NULL, NULL);

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

Bgedna::RAS_IPolyMaterialStruct* KX_ConvertClassStruct::convertIPolyMaterial(RAS_IPolyMaterial* ipoly_material, Bgedna::RAS_IPolyMaterialStruct* ipoly_material_struct = NULL, bool add_to_list = true)
{
	if(!ipoly_material)
		return NULL;

	if(!ipoly_material_struct)
	{
		ipoly_material_struct = new Bgedna::RAS_IPolyMaterialStruct();

		
		Bgedna::RAS_IPolyMaterialStruct* already_converted;
		already_converted = (Bgedna::RAS_IPolyMaterialStruct*) checkUnique((void*) ipoly_material_struct, IPOLY_MATERIAL);
		if(already_converted != NULL)
			return already_converted;
	}

	

	STR_String copy_string = STR_String(ipoly_material->GetMaterialName());
	strncpy(ipoly_material_struct->m_materialname, copy_string.Ptr(), 128 );
	delete(copy_string);
	STR_String copy_string = STR_String(ipoly_material->GetTextureName());
	strncpy(ipoly_material_struct->m_texturename, copy_string.Ptr(), 128 );
	delete(copy_string);

	ipoly_material_struct->m_shininess = ipoly_material->m_shininess;
	ipoly_material->m_diffuse.getValue(ipoly_material_struct->m_diffuse);

	ipoly_material_struct->m_specularity = ipoly_material->m_specularity;
	ipoly_material->m_specular.getValue(ipoly_material_struct->m_specular);

	ipoly_material_struct->m_tile = ipoly_material->GetTile();
	ipoly_material_struct->m_tilexrep = ipoly_material->GetTileXRep();
	ipoly_material_struct->m_tileyrep = ipoly_material->GetTileYRep();

	ipoly_material_struct->m_drawingmode = ipoly_material->GetDrawingMode();
	ipoly_material_struct->m_transp = ipoly_material->GetTransp();

	ipoly_material_struct->m_alpha = ipoly_material->IsAlpha();
	ipoly_material_struct->m_zsort = ipoly_material->IsZSort();
	ipoly_material_struct->m_materialindex = ipoly_material->GetMaterialIndex();


	if(add_to_list)
	{
		notifyConverted((void*) ipoly_material, (void*) ipoly_material_struct, IPOLY_MATERIAL);
		m_finterface->m_iPolyMaterial.push_back(ipoly_material_struct);
		return (Bgedna::RAS_IPolyMaterialStruct*) m_finterface->m_iPolyMaterial.last;
	}

	return ipoly_material_struct;
}

RAS_IPolyMaterial* KX_ConvertClassStruct::convertIPolyMaterialStruct(Bgedna::RAS_IPolyMaterialStruct* ipoly_material_struct, RAS_IPolyMaterial* ipoly_material)
{
	if(!ipoly_material_struct)
		return NULL;

	if(!ipoly_material)
		ipoly_material = new RAS_IPolyMaterial( const STR_String(ipoly_material_struct->m_texturename),
												const STR_String(ipoly_material_struct->m_materialname),
												ipoly_material_struct->m_materialindex,
												ipoly_material_struct->m_tile,
												ipoly_material_struct->m_tilexrep,
												ipoly_material_struct->m_tileyrep,
												ipoly_material_struct->m_drawingmode,
												ipoly_material_struct->m_transp,
												ipoly_material_struct->m_alpha,
												ipoly_material_struct->m_zsort);
	return ipoly_material;

}

Bgedna::RAS_MeshSlotStruct* KX_ConvertClassStruct::convertMeshSlot(RAS_MeshSlot* mesh_slot, Bgedna::RAS_MeshSlotStruct* mesh_slot_struct = NULL, bool add_to_list = true)
{
	if(!mesh_slot)
		return NULL;

	if(!mesh_slot_struct)
	{
		mesh_slot_struct = new Bgedna::RAS_MeshSlotStruct();

		
		Bgedna::RAS_MeshSlotStruct* already_converted;
		already_converted = (Bgedna::RAS_MeshSlotStruct*) checkUnique((void*) mesh_slot_struct, MESH_SLOT);
		if(already_converted != NULL)
			return already_converted;
	}

	mesh_slot_struct->m_bCulled = mesh_slot->m_bCulled;
	mesh_slot_struct->m_bObjectColor = mesh_slot->m_bObjectColor;
	mesh_slot_struct->m_bVisible = mesh_slot->m_bVisible;
	mesh_slot_struct->m_clientObj = mesh_slot->m_clientObj;
	mesh_slot_struct->m_OpenGLMatrix = mesh_slot->m_OpenGLMatrix;
	mesh_slot->m_RGBAcolor.getValue(mesh_slot_struct->m_RGBAcolor);
	mesh_slot_struct->m_bucket = convertMaterialBucket(mesh_slot->m_bucket, CREATE_NEW, true);
	mesh_slot_struct->m_joinSlot = convertMeshSlot(mesh_slot->m_joinSlot, CREATE_NEW, true);
	mesh_slot_struct->m_bDisplayList = mesh_slot->m_DisplayList;
	
	RAS_MeshSlot::iterator it;
	mesh_slot->begin(it);
	mesh_slot_struct->m_startvertex = it.startvertex;
	mesh_slot_struct->m_startarray =  it.arraynum;
	mesh_slot_struct->m_endvertex = it.endvertex;

	/*m_endarray, m_startindex, m_endindex missing. I don't know how to retrieve from it.*/
	
	Bgedna::fbtList* display_list = new Bgedna::fbtList();
	for(it; !mesh_slot->end(it); mesh_slot->next(it))
		display_list->push_back(convertDisplayArray(it.array, CREATE_NEW, true));

	mesh_slot_struct->m_displayArrays = *display_list;
	/*here i really hope that checkUnique() will do his work :) */
	mesh_slot_struct->m_currentArray = convertDisplayArray(mesh_slot->CurrentDisplayArray(), CREATE_NEW, true);
	mesh_slot_struct->m_mesh = convertMeshObject(mesh_slot->m_mesh, CREATE_NEW, true);
	/*why setValue doesn't work?*/
	for(int x = 0;x<4;x++)
	{
		for(int y = 0; y<4; y++)
		{
			mesh_slot_struct->m_joinInvTransform[x][y] = mesh_slot->m_joinInvTransform.getElement(x,y);
		}
	}
	Bgedna::fbtList* joined_slots = new Bgedna::fbtList();
	list<RAS_MeshSlot*>::iterator slots_it = mesh_slot->m_joinedSlots.begin();
	for(slots_it; slots_it!=mesh_slot->m_joinedSlots.end(); slots_it++)
		/*here again checkUnique should do a good work*/
		joined_slots->push_back(convertMeshSlot((RAS_MeshSlot*)&slots_it, CREATE_NEW, true));

	mesh_slot_struct->m_joinedSlots = *joined_slots;
	mesh_slot_struct->m_pDeformer = convertDeformer(mesh_slot->m_pDeformer, CREATE_NEW, true);
	mesh_slot_struct->m_pDerivedMesh = convertDerivedMesh(mesh_slot->m_pDerivedMesh, CREATE_NEW, true);
	//don't know how to retrieve. Probably not usefull.
	mesh_slot_struct->m_displayArrays = NULL;


	if(add_to_list)
	{
		notifyConverted((void*) mesh_slot, (void*) mesh_slot_struct, MESH_SLOT);
		m_finterface->m_meshSlot.push_back(mesh_slot_struct);
		return (Bgedna::RAS_MeshSlotStruct*) m_finterface->m_meshSlot.last;
	}

	return mesh_slot_struct;
}

RAS_MeshSlot* KX_ConvertClassStruct::convertMeshSlotStruct(Bgedna::RAS_MeshSlotStruct* mesh_slot_struct, RAS_MeshSlot* mesh_slot)
{

}

Bgedna::RAS_DisplayArrayStruct*	KX_ConvertClassStruct::convertDisplayArray(RAS_DisplayArray* display_array, Bgedna::RAS_DisplayArrayStruct* display_array_struct = NULL, bool add_to_list = true)
{
	if(!display_array)
		return NULL;

	if(!display_array_struct)
	{
		display_array_struct = new Bgedna::RAS_DisplayArrayStruct();

		
		Bgedna::RAS_DisplayArrayStruct* already_converted;
		already_converted = (Bgedna::RAS_DisplayArrayStruct*) checkUnique((void*) display_array_struct, DISPLAY_ARRAY);
		if(already_converted != NULL)
			return already_converted;
	}

	display_array_struct->m_type = display_array->m_type;
	display_array_struct->m_users = display_array->m_users;

	Bgedna::fbtList* m_index = new Bgedna::fbtList();
	vector<unsigned short>::iterator *it;
	Bgedna::ushortList* ushortlist;
	for(*it = display_array->m_index.begin(); *it != display_array->m_index.end(); it++)
	{
		ushortlist = new Bgedna::ushortList();
		ushortlist->value = *((unsigned short*) it);
	}
	display_array_struct->m_index = *m_index;

	Bgedna::fbtList* vertex_list = new Bgedna::fbtList();
	vector<RAS_TexVert>::iterator* vert_it;
	for(*vert_it = display_array->m_vertex.begin(); *vert_it != display_array->m_vertex.end(); ++vert_it)
		vertex_list->push_back(convertTexVert( (RAS_TexVert* )vert_it, CREATE_NEW, true));
	display_array_struct->m_vertex = *vertex_list;
	

	if(add_to_list)
	{
		notifyConverted((void*) display_array, (void*) display_array_struct, DISPLAY_ARRAY);
		m_finterface->m_displayArray.push_back(display_array_struct);
		return (Bgedna::RAS_DisplayArrayStruct*) m_finterface->m_displayArray.last;
	}

	return display_array_struct;

}

RAS_DisplayArray* KX_ConvertClassStruct::convertDisplayArrayStruct(Bgedna::RAS_DisplayArrayStruct* display_array_struct, RAS_DisplayArray* display_array)
{

}

Bgedna::RAS_MeshObjectStruct* KX_ConvertClassStruct::convertMeshObject(RAS_MeshObject* mesh_object, Bgedna::RAS_MeshObjectStruct* mesh_object_struct = NULL, bool add_to_list = true)
{

}

RAS_MeshObject*	KX_ConvertClassStruct::convertMeshObjectStruct(Bgedna::RAS_MeshObjectStruct* mesh_object_struct, RAS_MeshObject* mesh_objeect)
{

}

Bgedna::RAS_DeformerStruct*	KX_ConvertClassStruct::convertDeformer(RAS_Deformer* ras_deformer, Bgedna::RAS_DeformerStruct* ras_deformer_struct = NULL, bool add_to_list = true)
{

}

RAS_Deformer* KX_ConvertClassStruct::convertDeformerStruct(Bgedna::RAS_DeformerStruct* ras_deformer_struct, RAS_Deformer* ras_deformer)
{

}

Bgedna::DerivedMeshStruct* KX_ConvertClassStruct::convertDerivedMesh(DerivedMesh* derived_mesh, Bgedna::DerivedMeshStruct* derived_mesh_struct = NULL, bool add_to_list = true)
{

}

DerivedMesh* KX_ConvertClassStruct::convertDerivedMeshStruct(Bgedna::DerivedMeshStruct* derived_mesh_struct, DerivedMesh* derived_mesh)
{

}

Bgedna::RAS_TexVertStruct* KX_ConvertClassStruct::convertTexVert(RAS_TexVert* tex_vert, Bgedna::RAS_TexVertStruct* tex_vert_struct = NULL, bool add_to_list = true)
{
	if(!tex_vert)
		return NULL;

	if(!tex_vert_struct)
	{
		tex_vert_struct = new Bgedna::RAS_TexVertStruct();

		
		Bgedna::RAS_TexVertStruct* already_converted;
		already_converted = (Bgedna::RAS_TexVertStruct*) checkUnique((void*) tex_vert_struct, TEXT_VERT);
		if(already_converted != NULL)
			return already_converted;
	}

	tex_vert_struct->m_flag = tex_vert->getFlag();
	memcpy(tex_vert_struct->m_normal ,tex_vert->getNormal(), sizeof(float)*3);
	tex_vert_struct->m_origindex = tex_vert->getOrigIndex();
	memcpy(&tex_vert_struct->m_rgba,tex_vert->getRGBA(), sizeof(unsigned int));
	tex_vert_struct->m_softBodyIndex = tex_vert->getSoftBodyIndex();
	memcpy(tex_vert_struct->m_tangent, tex_vert->getTangent(), sizeof(float)*4);
	tex_vert_struct->m_unit = tex_vert->getUnit();
	memcpy(tex_vert_struct->m_uv1 ,tex_vert->getUV1(), sizeof(float)*2 );
	memcpy(tex_vert_struct->m_uv2 ,tex_vert->getUV2(), sizeof(float)*2 );
	memcpy(tex_vert_struct->m_localxyz ,tex_vert->getXYZ(), sizeof(float)*3 );

	if(add_to_list)
	{
		notifyConverted((void*) tex_vert, (void*) tex_vert_struct, TEXT_VERT);
		m_finterface->m_texVert.push_back(tex_vert_struct);
		return (Bgedna::RAS_TexVertStruct*) m_finterface->m_texVert.last;
	}

	return tex_vert_struct;

}

RAS_TexVert* KX_ConvertClassStruct::convertTexVertStruct(Bgedna::RAS_TexVertStruct* tex_vert_struct, RAS_TexVert* tex_vert)
{
	if(!tex_vert_struct)
		return NULL;

	if(!tex_vert)
	{
		tex_vert = new RAS_TexVert(MT_Point3(tex_vert_struct->m_localxyz),
				MT_Point2(tex_vert_struct->m_uv1),
				MT_Point2(tex_vert_struct->m_uv2),
				MT_Vector4(tex_vert_struct->m_tangent),
				tex_vert_struct->m_rgba,
				MT_Vector3(tex_vert_struct->m_normal),
				tex_vert_struct->m_flag,
				tex_vert_struct->m_origindex);
	}

	return tex_vert;
}

#endif