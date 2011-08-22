#include "KX_Scene.h"
#include "KX_BGEdna.h"
#include "fbtTypes.h"

typedef enum DataType{
	SCENE = FBT_ID2('S', 'C'),
	CAMERA = FBT_ID2('C','M'),
	BUCKET_MANAGER = FBT_ID2('B','M'),
	FONT_OBJECT = FBT_ID2('F','O'),
	FRAME_SETTINGS = FBT_ID2('F','S'),
	GAME_OBJECT = FBT_ID2('G','O'),
	LIGHT_OBJECT = FBT_ID2('L','O')
} DataType;


class KX_ConvertClassStruct{

public:
	KX_ConvertClassStruct();
	virtual ~KX_ConvertClassStruct();

	void* convertClassToStruct(void* save_class, FBTuint16 ID);
	void* convertStructToClass(void* save_struct, FBTuint16 ID);

private:

	/*general function to convert from Clists to fbtDataList*/
	Bgedna::fbtDataList*					fbtListFromCList(CListValue* clist, FBTuint16 ID, void* (*func)(void*, FBTuint16));
	
	/*specific class to struct conversion methods.*/
	
	Bgedna::KX_SceneStruct*					convertScene(KX_Scene* scene);
	Bgedna::KX_CameraStruct*				convertCameraObject(KX_Camera* camera);
	Bgedna::RAS_BucketManagerStruct*		convertBucketManager(RAS_BucketManager* bucket_manager);
	Bgedna::KX_FontObjectStruct*			convertFont(KX_FontObject* font);
	Bgedna::RAS_FrameSettingsStruct*		convertFrameSettings(const RAS_FrameSettings frsets);
	Bgedna::KX_GameObjectStruct*			convertGameObject(KX_GameObject* gameobj);
	Bgedna::KX_LightObjectStruct*			convertLightObject(KX_LightObject* light);
	Bgedna::RAS_RectStruct*					convertRect(const RAS_Rect scene_viewport);
	Bgedna::SCA_TimeEventManagerStruct*		convertTimeEvManager(SCA_TimeEventManager* time_mng);
	Bgedna::KX_WorldInfoStruct*				convertWorldInfo(KX_WorldInfo* winfo);

	/*specific struct to class conversion methods*/

}