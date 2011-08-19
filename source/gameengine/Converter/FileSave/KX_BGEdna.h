#ifndef _KX_BGEDNA
#define _KX_BGEDNA


#include <vector>
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

struct SceneList
	{
		SceneList*   next;
		SceneList*   prev;
		KX_SceneAttrs*	 data;
	};

/*fbt works with public attributes, so since all the attributes in KX_Scene are protected, i create a struct that will contains
all the attributes retrieved with the KX_Scene's pubblic methods. 
Note: make a class that convert a KX_Scene to Bgedna::KX_Scene and vice versa.*/
struct KX_SceneAttrs
{
	//put here all the variables that will contain KX_Scene's attributes that must be saved
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