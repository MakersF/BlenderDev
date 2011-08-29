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
	Bgedna::fbtDataList*					fbtListFromCList(CListValue* clist, FBTuint16 ID);
	
	/*specific class to struct conversion methods.*/
	
	Bgedna::KX_SceneStruct*					convertScene(KX_Scene* scene);
	Bgedna::KX_CameraStruct*				convertCameraObject(KX_Camera* camera);
	Bgedna::RAS_BucketManagerStruct*		convertBucketManager(RAS_BucketManager* bucket_manager);
	Bgedna::KX_FontObjectStruct*			convertFont(KX_FontObject* font);
	Bgedna::RAS_FrameSettingsStruct*		convertFrameSettings(const RAS_FrameSettings frsets);
	Bgedna::RAS_RectStruct*					convertRect(const RAS_Rect scene_viewport);
	Bgedna::SCA_TimeEventManagerStruct*		convertTimeEvManager(SCA_TimeEventManager* time_mng);
	Bgedna::KX_WorldInfoStruct*				convertWorldInfo(KX_WorldInfo* winfo);
	Bgedna::KX_GameObjectStruct*			convertGameObject(KX_GameObject* gameobj);
	Bgedna::KX_LightObjectStruct*			convertLightObject(KX_LightObject* light);
	Bgedna::RAS_MaterialBucketStruct*		convertMaterialBucket(RAS_MaterialBucket* material_bucket);
	Bgedna::RAS_IPolyMaterialStruct*		convertIPolyMaterial(RAS_IPolyMaterial* ipoly_material);
	Bgedna::RAS_MeshSlotStruct*				convertMeshSlot(RAS_MeshSlot* mesh_slot);

	/*specific struct to class conversion methods*/

	

	KX_Scene*								convertSceneStruct(Bgedna::KX_SceneStruct* scene_struct);
	KX_Camera*								convertCameraObjectStruct(Bgedna::KX_CameraStruct* camera_struct);
	RAS_BucketManager*						convertBucketManagerStruct(Bgedna::RAS_BucketManagerStruct* bucket_manager_struct);
	KX_FontObject*							convertFontStruct(Bgedna::KX_FontObjectStruct* font_struct);
	const RAS_FrameSettings					convertFrameSettingsStruct(Bgedna::RAS_FrameSettingsStruct* frsets_struct);
	const RAS_Rect							convertRectStruct(Bgedna::RAS_RectStruct* scene_viewport_struct);
	SCA_TimeEventManager*					convertTimeEvManagerStruct(Bgedna::SCA_TimeEventManagerStruct* time_mng_struct);
	KX_WorldInfo*							convertWorldInfoStruct(Bgedna::KX_WorldInfoStruct* winfo_struct);
	KX_GameObject*							convertGameObjectStruct(Bgedna::KX_GameObjectStruct* gameobj_struct);
	KX_LightObject*							convertLightObjectStruct(Bgedna::KX_LightObjectStruct* light_struct);
	RAS_MaterialBucket*						convertMaterialBucketStruct(Bgedna::RAS_MaterialBucketStruct* material_bucket_struct);
	RAS_IPolyMaterial*						convertIPolyMaterialStruct(Bgedna::RAS_IPolyMaterialStruct* ipoly_material_struct);
	RAS_MeshSlot*							convertMeshSlotStruct(Bgedna::RAS_MeshSlotStruct* mesh_slot_struct);

}