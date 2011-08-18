#ifndef _KX_FILEINTERFACE
#define _KX_FILEINTERFACE

#include "fbtFile.h"
#include "KX_BGEdna.h"

class KX_FileInterface : public fbtFile
{
public:
        KX_FileInterface();
        virtual ~KX_FileInterface();


        fbtList m_scene;// all fbtList for each thing here!

        Bgedna::FileGlobal* m_fg;

        int save(const char* path, const int mode = PM_UNCOMPRESSED);
        
        void setIgnoreList(FBTuint32 *stripList) {m_stripList = stripList;}

protected:
        virtual int notifyData(void* p, const Chunk& id);
        virtual int initializeTables(fbtBinTables* tables);
        virtual bool skip(const FBTuint32& id);
        virtual int writeData(fbtStream* stream);

        FBTuint32* m_stripList;

        virtual void*   getFBT(void);
        virtual FBTsize getFBTlength(void);
};


#endif//_KX_FILEINTERFACE