#ifndef _KX_BGEDNA
#define _KX_BGEDNA
// Generated from a Blender(256) file.


#include <vector>

/*why this?
#ifdef near
#undef near
#endif
#ifdef far
#undef far
#endif*/


namespace Bgedna {
/** \addtogroup Bgedna
*  @{
*/

	//all structs here
struct FileGlobal;
struct KX_Scene;

typedef std::vector<KX_Scene*> KX_SceneList;
typedef std::vector<class RAS_MaterialBucket*> BucketList;

typedef struct RAS_BucketManager {
	
	BucketList m_SolidBuckets;
	BucketList m_AlphaBuckets;
	
	struct sortedmeshslot;
	struct backtofront;
	struct fronttoback;

} RAS_BucketManager;

struct CullingInfo {
		int m_layer;
	};

typedef struct KX_Scene {

#ifdef WITH_PYTHON
	PyObject*	m_attr_dict;
	PyObject*	m_draw_call_pre;
	PyObject*	m_draw_call_post;
#endif

	RAS_BucketManager*	m_bucketmanager;
	CListValue*			m_tempObjectList;
	CListValue*			m_euthanasyobjects;
	CListValue*			m_objectlist;
	CListValue*			m_parentlist;
	CListValue*			m_lightlist;
	CListValue*			m_inactivelist;
	SG_QList			m_sghead;		
	list<class KX_Camera*>       m_cameras;
	list<class KX_FontObject*>       m_fonts;
	SCA_LogicManager*		m_logicmgr;
	SCA_KeyboardManager*	m_keyboardmgr;
	SCA_MouseManager*		m_mousemgr;
	SCA_TimeEventManager*	m_timemgr;
	class KX_BlenderSceneConverter*		m_sceneConverter;
	class PHY_IPhysicsEnvironment*		m_physicsEnvironment;
	bool m_isclearingZbuffer;
	STR_String	m_sceneName;
	KX_WorldInfo* m_worldinfo;
	NG_NetworkDeviceInterface*	m_networkDeviceInterface;
	NG_NetworkScene* m_networkScene;
	SG_Node* m_rootnode;
	KX_Camera* m_active_camera;
	CTR_Map	<CTR_HashedPtr, void*> m_map_gameobject_to_replica;
	std::vector<KX_GameObject*>	m_logicHierarchicalGameObjects;
	std::set<CValue*>	m_groupGameObjects;
	SCA_ISystem* m_kxsystem;
	int	m_ueberExecutionPriority;
	bool m_suspend;
	float m_activity_box_radius;
	bool m_activity_culling;
	bool m_dbvt_culling;
	int m_dbvt_occlusion_res;
	RAS_FrameSettings m_frame_settings;
	RAS_Rect m_viewport;
	double				m_suspendedtime;
	double				m_suspendeddelta;
	struct Scene* m_blenderScene;
	RAS_2DFilterManager m_filtermanager;
	static SG_Callbacks	m_callbacks;

#ifdef WITH_PYTHON

	static PyMappingMethods	Mapping;
	static PySequenceMethods	Sequence;
#endif
} KX_Scene;

struct FileGlobal
{
    char subvstr[4];
    short subversion;
    int revision;
    char filename[240];
};

/** @}*/
}
#endif//_KX_BGEDNA
#ifndef _KX_BGEDNA
#define _KX_BGEDNA
// Generated from a Blender(256) file.


#include <vector>

/*why this?
#ifdef near
#undef near
#endif
#ifdef far
#undef far
#endif*/


namespace Bgedna {
/** \addtogroup Bgedna
*  @{
*/

	//all structs here
struct FileGlobal;
struct KX_Scene;

typedef std::vector<KX_Scene*> KX_SceneList;
typedef std::vector<class RAS_MaterialBucket*> BucketList;

typedef struct RAS_BucketManager {
	
	BucketList m_SolidBuckets;
	BucketList m_AlphaBuckets;
	
	struct sortedmeshslot;
	struct backtofront;
	struct fronttoback;

} RAS_BucketManager;

struct CullingInfo {
		int m_layer;
	};

typedef struct KX_Scene {

#ifdef WITH_PYTHON
	PyObject*	m_attr_dict;
	PyObject*	m_draw_call_pre;
	PyObject*	m_draw_call_post;
#endif

	RAS_BucketManager*	m_bucketmanager;
	CListValue*			m_tempObjectList;
	CListValue*			m_euthanasyobjects;
	CListValue*			m_objectlist;
	CListValue*			m_parentlist;
	CListValue*			m_lightlist;
	CListValue*			m_inactivelist;
	SG_QList			m_sghead;		
	list<class KX_Camera*>       m_cameras;
	list<class KX_FontObject*>       m_fonts;
	SCA_LogicManager*		m_logicmgr;
	SCA_KeyboardManager*	m_keyboardmgr;
	SCA_MouseManager*		m_mousemgr;
	SCA_TimeEventManager*	m_timemgr;
	class KX_BlenderSceneConverter*		m_sceneConverter;
	class PHY_IPhysicsEnvironment*		m_physicsEnvironment;
	bool m_isclearingZbuffer;
	STR_String	m_sceneName;
	KX_WorldInfo* m_worldinfo;
	NG_NetworkDeviceInterface*	m_networkDeviceInterface;
	NG_NetworkScene* m_networkScene;
	SG_Node* m_rootnode;
	KX_Camera* m_active_camera;
	CTR_Map	<CTR_HashedPtr, void*> m_map_gameobject_to_replica;
	std::vector<KX_GameObject*>	m_logicHierarchicalGameObjects;
	std::set<CValue*>	m_groupGameObjects;
	SCA_ISystem* m_kxsystem;
	int	m_ueberExecutionPriority;
	bool m_suspend;
	float m_activity_box_radius;
	bool m_activity_culling;
	bool m_dbvt_culling;
	int m_dbvt_occlusion_res;
	RAS_FrameSettings m_frame_settings;
	RAS_Rect m_viewport;
	double				m_suspendedtime;
	double				m_suspendeddelta;
	struct Scene* m_blenderScene;
	RAS_2DFilterManager m_filtermanager;
	static SG_Callbacks	m_callbacks;

#ifdef WITH_PYTHON

	static PyMappingMethods	Mapping;
	static PySequenceMethods	Sequence;
#endif
} KX_Scene;

struct FileGlobal
{
    char subvstr[4];
    short subversion;
    int revision;
    char filename[240];
};

/** @}*/
}
#endif//_KX_BGEDNA