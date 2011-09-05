#ifdef WITH_FBT

#ifndef _KX_CONVERTCLASSSTRUCT
#define _KX_CONVERTCLASSSTRUCT

#include "KX_Scene.h"
#include "KX_BGEdna.h"
#include "fbtTypes.h"
#include <vector>

#include <list>
#include "MT_Vector3.h"
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
#include "MT_Matrix4x4.h"

enum DataType{
	SCENE = FBT_ID2('S', 'C'),
	CAMERA = FBT_ID2('C','M'),
	BUCKET_MANAGER = FBT_ID2('B','M'),
	FONT_OBJECT = FBT_ID2('F','O'),
	FRAME_SETTINGS = FBT_ID2('F','S'),
	GAME_OBJECT = FBT_ID2('G','O'),
	LIGHT_OBJECT = FBT_ID2('L','O'),
	MATERIAL_BUCKET = FBT_ID2('M','B'),
	MESH_SLOT = FBT_ID2('M','S'),
	RAS_RECT = FBT_ID2('R','R'),
	WORLD_INFO = FBT_ID2('W','I'),
	IPOLY_MATERIAL = FBT_ID2('I','P'),
	DISPLAY_ARRAY = FBT_ID2('D','A')
};


class KX_ConvertClassStruct{
public:	

	class PointerCouple
	{
	public:
		void* original;
		void* converted;
	};
	
	typedef std::vector<PointerCouple> PointerCoupleList;
	
	class PointerIDList
	{
	public:
		PointerIDList();
		PointerIDList(unsigned int id) {ID = id;};
		unsigned int ID;
		PointerCoupleList pointer_pairs;
	};

public:
	KX_ConvertClassStruct();
	KX_ConvertClassStruct(class KX_FileInterface* file_interface) { m_finterface = file_interface;};
	virtual ~KX_ConvertClassStruct();

	void* convertClassToStruct(void* save_class, FBTuint16 ID);
	void* convertStructToClass(void* save_struct, FBTuint16 ID);

private:
	/*attributes*/

	class KX_FileInterface* m_finterface;
	/*it contains a different list of PointerCouple for each different IDs.
	It is used to check if the elements have been already converted*/
	std::vector<PointerIDList> converted_list;

	/*methods*/

	/*check if the element has be already converted. If so, it returns the pointer to the converted element,
	if not return NULL*/
	void*									checkUnique(void* original, FBTuint16 ID);
	void									notifyConverted(void* original, void* converted, FBTuint16 ID);
	/*general function to convert from Clists to fbtDataList*/
	Bgedna::fbtList*					CListTofbtList(CListValue* clist, FBTuint16 ID);
	
	/*specific class to struct conversion methods.*/
	
	Bgedna::KX_SceneStruct*					convertScene(KX_Scene* scene, Bgedna::KX_SceneStruct* scene_struct = NULL, bool add_to_list = true);
	Bgedna::KX_CameraStruct*				convertCameraObject(KX_Camera* camera, Bgedna::KX_CameraStruct* camera_struct = NULL, bool add_to_list = true);
	Bgedna::RAS_BucketManagerStruct*		convertBucketManager(RAS_BucketManager* bucket_manager, Bgedna::RAS_BucketManagerStruct* bucket_manager_struct = NULL, bool add_to_list = true);
	Bgedna::KX_FontObjectStruct*			convertFont(KX_FontObject* font, Bgedna::KX_FontObjectStruct* font_struct = NULL, bool add_to_list = true);
	Bgedna::RAS_FrameSettingsStruct*		convertFrameSettings(const RAS_FrameSettings frsets, Bgedna::RAS_FrameSettingsStruct* frsets_struct = NULL, bool add_to_list = true);
	Bgedna::RAS_RectStruct*					convertRect(const RAS_Rect scene_viewport, Bgedna::RAS_RectStruct* scene_viewport_struct = NULL, bool add_to_list = true);
	Bgedna::SCA_TimeEventManagerStruct*		convertTimeEvManager(SCA_TimeEventManager* time_mng, Bgedna::SCA_TimeEventManagerStruct* time_mng_struct = NULL, bool add_to_list = true);
	Bgedna::KX_WorldInfoStruct*				convertWorldInfo(KX_WorldInfo* winfo, Bgedna::KX_WorldInfoStruct* world_info_struct = NULL, bool add_to_list = true);
	Bgedna::KX_GameObjectStruct*			convertGameObject(KX_GameObject* game_obj, Bgedna::KX_GameObjectStruct* game_obj_struct = NULL, bool add_to_list = true);
	Bgedna::KX_LightObjectStruct*			convertLightObject(KX_LightObject* light, Bgedna::KX_LightObjectStruct* light_struct = NULL, bool add_to_list = true);
	Bgedna::RAS_MaterialBucketStruct*		convertMaterialBucket(RAS_MaterialBucket* material_bucket, Bgedna::RAS_MaterialBucketStruct* material_bucket_struct = NULL, bool add_to_list = true);
	Bgedna::RAS_IPolyMaterialStruct*		convertIPolyMaterial(RAS_IPolyMaterial* ipoly_material, Bgedna::RAS_IPolyMaterialStruct* ipoly_material_struct = NULL, bool add_to_list = true);
	Bgedna::RAS_MeshSlotStruct*				convertMeshSlot(RAS_MeshSlot* mesh_slot, Bgedna::RAS_MeshSlotStruct* mesh_slot_struct = NULL, bool add_to_list = true);
	Bgedna::RAS_DisplayArrayStruct*			convertDisplayArray(RAS_DisplayArray* display_array, Bgedna::RAS_DisplayArrayStruct* display_array_struct = NULL, bool add_to_list = true);
	Bgedna::RAS_MeshObjectStruct*			convertMeshObject(RAS_MeshObject* mesh_object, Bgedna::RAS_MeshObjectStruct* mesh_object_struct = NULL, bool add_to_list = true);
	Bgedna::RAS_DeformerStruct*				convertDeformer(RAS_Deformer* ras_deformer, Bgedna::RAS_DeformerStruct* ras_deformer_struct = NULL, bool add_to_list = true);
	Bgedna::DerivedMeshStruct*				convertDerivedMesh(DerivedMesh* derived_mesh, Bgedna::DerivedMeshStruct* derived_mesh_struct = NULL, bool add_to_list = true);


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
	RAS_DisplayArray*						convertDisplayArrayStruct(Bgedna::RAS_DisplayArrayStruct* display_array_struct, RAS_DisplayArray* display_array);
	RAS_MeshObject*							convertMeshObjectStruct(Bgedna::RAS_MeshObjectStruct* mesh_object_struct, RAS_MeshObject* mesh_objeect);
	RAS_Deformer*							convertDeformerStruct(Bgedna::RAS_DeformerStruct* ras_deformer_struct, RAS_Deformer* ras_deformer);
	DerivedMesh*							convertDerivedMeshStruct(Bgedna::DerivedMeshStruct* derived_mesh_struct, DerivedMesh* derived_mesh);

}
#endif //_KX_CONVERTCLASSSTRUCT
#endif