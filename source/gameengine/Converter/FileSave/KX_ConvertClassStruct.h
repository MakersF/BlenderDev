#include "KX_Scene.h"
#include "KX_BGEdna.h"
#include "fbtTypes.h"

class KX_ConvertClassStruct{

public:
	KX_ConvertClassStruct();
	virtual ~KX_ConvertClassStruct();

	void* convertClassToStruct(void* save_class, FBTuint16 ID);
	void* convertStructToClass(void* save_struct, FBTuint16 ID);

private:
	Bgedna::KX_SceneStruct* convertScene(KX_Scene* scene);
	Bgedna::KX_CameraStruct* convertCamera(KX_Camera* camera);
	Bgedna::RAS_BucketManagerStruct* convertBucketManager(RAS_BucketManager* bucket_manager);
	Bgedna::KX_FontObjectStruct* convertFont(KX_FontObject* font);
	Bgedna::RAS_FrameSettingsStruct* convertFrameSettings(const RAS_FrameSettings frsets);

}