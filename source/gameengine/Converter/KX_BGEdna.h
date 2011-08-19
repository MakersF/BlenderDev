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

#define BGE_PUBBLIC public

namespace Bgedna {
/** \addtogroup Bgedna
*  @{
*/

	//all structs here
struct FileGlobal;

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