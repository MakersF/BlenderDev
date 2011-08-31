#if WITH_FBT

#ifndef _KX_CONVERTCLASSSTRUCT.H
#define _KX_CONVERTCLASSSTRUCT.H

#include "KX_Scene.h"
#include "KX_BGEdna.h"
#include "fbtTypes.h"

enum DataType{
	SCENE = FBT_ID2('S', 'C'),
	CAMERA = FBT_ID2('C','M'),
	BUCKET_MANAGER = FBT_ID2('B','M'),
	FONT_OBJECT = FBT_ID2('F','O'),
	FRAME_SETTINGS = FBT_ID2('F','S'),
	GAME_OBJECT = FBT_ID2('G','O'),
	LIGHT_OBJECT = FBT_ID2('L','O'),
	MATERIAL_BUCKET = FBT_ID2('M','B'),
	MESH_SLOT = FBT_ID2('M','S')
};


class KX_ConvertClassStruct{

public:
	KX_ConvertClassStruct();
	KX_ConvertClassStruct(class KX_FileInterface* file_interface) { m_finterface = file_interface;};
	virtual ~KX_ConvertClassStruct();

	void* convertClassToStruct(void* save_class, FBTuint16 ID);
	void* convertStructToClass(void* save_struct, FBTuint16 ID);

private:
	/*attributes*/

	class KX_FileInterface* m_finterface;

	/*methods*/
	/*general function to convert from Clists to fbtDataList*/
	Bgedna::fbtList*					fbtListFromCList(CListValue* clist, FBTuint16 ID);
	
	/*specific class to struct conversion methods.*/
	
	Bgedna::KX_SceneStruct*					convertScene(KX_Scene* scene, Bgedna::KX_SceneStruct* scene_struct, bool add_to_list);
	Bgedna::KX_CameraStruct*				convertCameraObject(KX_Camera* camera, Bgedna::KX_CameraStruct* camera_struct, bool add_to_list);
	Bgedna::RAS_BucketManagerStruct*		convertBucketManager(RAS_BucketManager* bucket_manager, Bgedna::RAS_BucketManagerStruct* bucket_manager_struct, bool add_to_list);
	Bgedna::KX_FontObjectStruct*			convertFont(KX_FontObject* font, Bgedna::KX_FontObjectStruct* font_struct, bool add_to_list);
	Bgedna::RAS_FrameSettingsStruct*		convertFrameSettings(const RAS_FrameSettings frsets, Bgedna::RAS_FrameSettingsStruct* frsets_struct, bool add_to_list);
	Bgedna::RAS_RectStruct*					convertRect(const RAS_Rect scene_viewport, Bgedna::RAS_RectStruct* scene_viewport_struct, bool add_to_list);
	Bgedna::SCA_TimeEventManagerStruct*		convertTimeEvManager(SCA_TimeEventManager* time_mng, Bgedna::SCA_TimeEventManagerStruct* time_mng_struct, bool add_to_list);
	Bgedna::KX_WorldInfoStruct*				convertWorldInfo(KX_WorldInfo* winfo, Bgedna::KX_WorldInfoStruct* world_info_struct, bool add_to_list);
	Bgedna::KX_GameObjectStruct*			convertGameObject(KX_GameObject* game_obj, Bgedna::KX_GameObjectStruct* game_obj_struct, bool add_to_list);
	Bgedna::KX_LightObjectStruct*			convertLightObject(KX_LightObject* light, Bgedna::KX_LightObjectStruct* light_struct, bool add_to_list);
	Bgedna::RAS_MaterialBucketStruct*		convertMaterialBucket(RAS_MaterialBucket* material_bucket, Bgedna::RAS_MaterialBucketStruct* material_bucket_struct, bool add_to_list);
	Bgedna::RAS_IPolyMaterialStruct*		convertIPolyMaterial(RAS_IPolyMaterial* ipoly_material, Bgedna::RAS_IPolyMaterialStruct* ipoly_material_struct, bool add_to_list);
	Bgedna::RAS_MeshSlotStruct*				convertMeshSlot(RAS_MeshSlot* mesh_slot, Bgedna::RAS_MeshSlotStruct* mesh_slot_struct, bool add_to_list);

	/*specific struct to class conversion methods*/

	

	KX_Scene*								convertSceneStruct(Bgedna::KX_SceneStruct* scene_struct, KX_Scene* scene);
	KX_Camera*								convertCameraObjectStruct(Bgedna::KX_CameraStruct* camera_struct, KX_Camera* camera);
	RAS_BucketManager*						convertBucketManagerStruct(Bgedna::RAS_BucketManagerStruct* bucket_manager_struct, RAS_BucketManager* bucket_manager);
	KX_FontObject*							convertFontStruct(Bgedna::KX_FontObjectStruct* font_struct, KX_FontObject*	font);
	RAS_FrameSettings*						convertFrameSettingsStruct(Bgedna::RAS_FrameSettingsStruct* frsets_struct, RAS_FrameSettings* frsets);
	RAS_Rect*								convertRectStruct(Bgedna::RAS_RectStruct* scene_viewport_struct, RAS_Rect* scene_viewport);
	SCA_TimeEventManager*					convertTimeEvManagerStruct(Bgedna::SCA_TimeEventManagerStruct* time_mng_struct, SCA_TimeEventManager* time_mng);
	KX_WorldInfo*							convertWorldInfoStruct(Bgedna::KX_WorldInfoStruct* winfo_struct, KX_WorldInfo* winfo);
	KX_GameObject*							convertGameObjectStruct(Bgedna::KX_GameObjectStruct* game_obj_struct, KX_GameObject* game_obj);
	KX_LightObject*							convertLightObjectStruct(Bgedna::KX_LightObjectStruct* light_struct, KX_LightObject* light);
	RAS_MaterialBucket*						convertMaterialBucketStruct(Bgedna::RAS_MaterialBucketStruct* material_bucket_struct, RAS_MaterialBucket* material_bucket);
	RAS_IPolyMaterial*						convertIPolyMaterialStruct(Bgedna::RAS_IPolyMaterialStruct* ipoly_material_struct, RAS_IPolyMaterial* ipoly_material);
	RAS_MeshSlot*							convertMeshSlotStruct(Bgedna::RAS_MeshSlotStruct* mesh_slot_struct, RAS_MeshSlot* mesh_slot);

}
#endif //_KX_CONVERTCLASSSTRUCT.H
#endif