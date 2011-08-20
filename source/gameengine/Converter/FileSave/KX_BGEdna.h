#ifndef _KX_BGEDNA
#define _KX_BGEDNA


/*makefbt doesn't like public methods and typedef inside classes.
So in classes first define all private and protected methods and attribute,
then use "pub_methods:" to define public methods, and lately use "public:" to
define all the public attributes.
Remeber to bring typedef outside of classes.*/
#define pub_methods public 

namespace Bgedna {
/** \addtogroup Bgedna
*  @{
*/

/*fbt works with public attributes, so since all the attributes in KX_Scene are protected, i create a struct that will contains
all the attributes retrieved with the KX_Scene's pubblic methods. 
Note: make a class that convert a KX_Scene to Bgedna::KX_Scene and vice versa.*/
struct KX_SceneStruct
{
	//put here all the variables that will contain KX_Scene's attributes that must be saved
	KX_CameraStruct active_camera;
	RAS_BucketManagerStruct bucket_manager;
	fbtDataList cameras;
	unsigned int camera_design_height;
	unsigned int camera_design_width;
	bool dbvt_culling;
	int dbvt_occlusion_res;
	fbtDataList fonts;
	RAS_FrameSettingsStruct framing_type;

};

struct KX_CameraStruct
{

};

struct RAS_BucketManagerStruct
{

};

struct KX_FontObjectStruct
{

};

struct RAS_FrameSettingsStruct
{

};



/*data list related code*/

struct DataList
{
	DataList *next, *prev;
	void* data;
};

class fbtDataList
{
pub_methods:

	fbtDataList() : first(0), last(0) {}
	fbtDataList(unsigned short ID_code) : ID(ID_code) { fbtDataList(); }
	~fbtDataList() { clear(); }

	void clear(void) { first = last = 0; }

	void add_data(void* data)
	{
		DataList* datalist = new DataList();
		datalist->data = data;
		push_back(datalist);
	}

	void push_back(void* v)
	{
		DataList* datalist = ((DataList*)v);
		if (!datalist)
			return;

		datalist->prev = last;
		if (last)
			last->next = datalist;

		if (!first)
			first = datalist;

		last = datalist;
	}

	void remove(DataList* link)
	{
		if (!link)
			return;
		if (link->next)
			link->next->prev = link->prev;
		if (link->prev)
			link->prev->next = link->next;
		if (last == link)
			last = link->prev;
		if (first == link)
			first = link->next;
	}



public:

	DataList*   first;
	DataList*   last;
	unsigned short ID;
};

/*SceneList related data*/

struct SceneList
	{
		SceneList   *next;
		SceneList   *prev;
		KX_SceneStruct*	 data;
	};

class fbtSceneList
{
pub_methods:

	fbtSceneList() : first(0), last(0) {}
	~fbtSceneList() { clear(); }

	void clear(void) { first = last = 0; }

	void push_back(void* v)
	{
		SceneList* scenelist = ((SceneList*)v);
		if (!scenelist)
			return;

		scenelist->prev = last;
		if (last)
			last->next = scenelist;

		if (!first)
			first = scenelist;

		last = scenelist;
	}

	void remove(SceneList* link)
	{
		if (!link)
			return;
		if (link->next)
			link->next->prev = link->prev;
		if (link->prev)
			link->prev->next = link->next;
		if (last == link)
			last = link->prev;
		if (first == link)
			first = link->next;
	}



public:

	SceneList*   first;
	SceneList*   last;

};

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