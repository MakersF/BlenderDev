#include "KX_ConvertClassStruct.h"
#include <list>
#include "ListValue.h"
#include "STR_String.h"

/*C++ doesnìt allow to pass methods as function pointers, so i define a free function that call the right method.
An alternative could be to perform the check in fbtListFromCList and have a "for..." for each different type.
That should be even faster and have less overhead, but i think in this way code is more easy to read, maintain and change*/
void* convertFunction(void* data, FBTuint16 ID)
{
	switch (ID)
	{
		case GAME_OBJECT :
			return (void*) convertGameObject((KX_GameObject*) data);

		case LIGHT_OBJECT :
			return (void*) convertLightObject((KX_LightObject*) data);
		
	}
}

Bgedna::fbtDataList* KX_ConvertClassStruct::fbtListFromCList(CListValue* clist, FBTuint16 ID, void* (*func)(void*, FBTuint16))
{
	Bgedna::fbtDataList* retlist = new Bgedna::fbtDataList(ID);

	for (int i=0; i < clist->GetCount(); ++i)
	{
		//cast the pointer of the cvalue to void*, and pass it to the callback function, that will convert it)
		retlist->add_data( func( (void*) (clist+i) , ID) );
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
*/
		default:
			{
				break;
			}
	}

}

Bgedna::KX_SceneStruct* KX_ConvertClassStruct::convertScene(KX_Scene* scene)
{
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
	
	scene_struct->inactive_list = *(fbtListFromCList(scene->GetInactiveList(), GAME_OBJECT, convertFunction));
	scene_struct->light_list = *(fbtListFromCList(scene->GetLightList(), LIGHT_OBJECT, convertFunction));
	scene_struct->object_list = *(fbtListFromCList(scene->GetObjectList(), GAME_OBJECT, convertFunction));
	
	/*rootParentList can contain all types of elements: game_objects, lights, cameras... How can i know what type is the element i'm processing?
	Maybe i must find a way to directly save CValues. Other question: can i recostruct a CListValue with only the elements i save? If not, storing the additional
	infos required inside fbtDataList attributes or inside a DataLink attributes is absolutely a no problem(i'll create a fbtCValueList that will have the cvaluelist_infos attribute)
	and i can write a cvaluelistinfos struct to contain this data), but i don't know what to store.. 
	I think	when it will come to recostructing the scene from the saved data i'll know what i need.* /
	scene->GetRootParentList();*/
	scene_struct->temp_obj_list = *(fbtListFromCList(scene->GetTempObjectList(), GAME_OBJECT, convertFunction));

}

Bgedna::KX_CameraStruct* convertCameraObject(KX_Camera* camera)
{
	Bgedna::KX_CameraStruct* camera = new Bgedna::KX_CameraStruct();
}

Bgedna::RAS_BucketManagerStruct* convertBucketManager(RAS_BucketManager* bucket_manager)
{

}

Bgedna::KX_FontObjectStruct* convertFont(KX_FontObject* font)
{

}

Bgedna::RAS_FrameSettingsStruct* convertFrameSettings(const RAS_FrameSettings frsets)
{

}

Bgedna::KX_GameObjectStruct* convertGameObject(KX_GameObject* gameobj)
{

}

Bgedna::KX_LightObjectStruct* convertLightObject(KX_LightObject* light)
{

}

Bgedna::RAS_RectStruct* convertRect(const RAS_Rect scene_viewport)
{

}

Bgedna::SCA_TimeEventManagerStruct* convertTimeEvManager(SCA_TimeEventManager* time_mng)
{

}

Bgedna::KX_WorldInfoStruct*	convertWorldInfo(KX_WorldInfo* winfo)
{

}