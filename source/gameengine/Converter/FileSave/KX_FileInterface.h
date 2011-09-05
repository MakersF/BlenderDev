#ifdef WITH_FBT

#ifndef _KX_FILEINTERFACE_H
#define _KX_FILEINTERFACE_H

#include "fbtFile.h"
#include "KX_BGEdna.h"
#include "KX_ConvertClassStruct.h"

class KX_FileInterface : public fbtFile
{
public:
        KX_FileInterface();
        virtual ~KX_FileInterface();

		// all fbtList for each thing here!
		/*rembeber to add also in the fbtIdDB fbtData[]*/
		Bgedna::fbtList m_scene;
		Bgedna::fbtList m_frameSettings;
		Bgedna::fbtList m_rasRect;
		Bgedna::fbtList m_bucketManager;
		Bgedna::fbtList m_materialBucket;
		Bgedna::fbtList m_worldInfo;
		Bgedna::fbtList m_iPolyMaterial;
		Bgedna::fbtList m_meshSlot;
		Bgedna::fbtList m_displayArray;

        Bgedna::FileGlobal* m_fg;

        int save(const char* path, const int mode = PM_UNCOMPRESSED);
        
        void setIgnoreList(FBTuint32 *stripList) {m_stripList = stripList;}

protected:
        virtual int notifyData(void* p, const Chunk& id);
        virtual int initializeTables(fbtBinTables* tables);
        virtual bool skip(const FBTuint32& id);
        virtual int writeData(fbtStream* stream);

        FBTuint32* m_stripList;
		class KX_ConvertClassStruct* m_convertClass;

        virtual void*   getFBT(void);
        virtual FBTsize getFBTlength(void);
};


#endif//_KX_FILEINTERFACE_H
#endif