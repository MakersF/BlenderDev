#include "KX_ConvertClassStruct.h"
#include <list>
#include "ListValue.h"

void* KX_ConvertClassStruct::convertClassToStruct(void* save_class, FBTuint16 ID)
{
	switch (ID) 
	{
		case FBT_ID2('S', 'C')://scene
				return (void*) convertScene((KX_Scene*) save_class);
/*
		case FBT_ID2('C','M')://camera
				return (void*) convertCamera((KX_Camera*) save_class);

		case FBT_ID2('B','M'): //bucket manager
				return (void*) convertBucketManager((RAS_BucketManager*) save_class);

		case FBT_ID2('F','O'): //font object
				return (void*) convertFont((KX_FontObject*) save_class);
		
		case FBT_ID2('F','S'): //frame settings
				return (void*) convertFrameSettings((const Ras_BucketManager) save_class);
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
	scene_struct->active_camera = convertCamera(scene->GetActiveCamera());
	scene_struct->bucket_manager = convertBucketManager(scene->GetBucketManager());
	scene_struct->camera_design_height = scene->GetCanvasDesignHeight();
	scene_struct->camera_design_width =  scene->GetCanvasDesignWidth();
	scene_struct->dbvt_culling = scene->GetDbvtCulling();
	scene_struct->dbvt_occlusion_res = scene->GetDbvtOcclusionRes();
	scene_struct->framing_type = convertFrameSettings(scene->GetFramingType());
	
	/*start converting the attributes in lists*/

	//convert all cameras and put into a DataList
	Bgedna::fbtDataList cameras = Bgedna::fbtDataList(FBT_ID2('C','M'));
	std::list<class KX_Camera*> camera_list = (std::list<class KX_Camera*>) *(scene->GetCameras());
	std::list<class KX_Camera*>::iterator cam_it = camera_list.begin();
	while(cam_it!= camera_list.end())
	{
		cameras.add_data(convertCamera(*cam_it));
		cam_it++;
	}
	scene_struct->cameras = cameras;
	delete(&cam_it);

	//convert all fonts and put into Datalist
	Bgedna::fbtDataList fonts = Bgedna::fbtDataList(FBT_ID2('F','N'));
	std::list<class KX_FontObject*> font_list = (std::list<class KX_FontObject*>) *(scene->GetFonts());
	std::list<class KX_FontObject*>::iterator fon_it = font_list.begin();
	while(fon_it != font_list.end() )
	{
		fonts.add_data(convertFont(*fon_it));
		fon_it++;
	}
	scene_struct->fonts = fonts;
	delete(&fon_it);
	

}

Bgedna::KX_CameraStruct* KX_ConvertClassStruct::convertCamera(KX_Camera* camera)
{

}

Bgedna::RAS_BucketManagerStruct* KX_ConvertClassStruct::convertBucketManager(RAS_BucketManager* bucket_manager)
{

}

Bgedna::KX_FontObjectStruct* KX_ConvertClassStruct::convertFont(KX_FontObject* font)
{

}

Bgedna::RAS_FrameSettingsStruct* KX_ConvertClassStruct::convertFrameSettings(const RAS_FrameSettings frsets)
{

}