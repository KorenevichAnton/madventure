#ifndef __MEMORY_MANAGER__
#define __MEMORY_MANAGER__

#include "cocos2d.h"
#include "PersistMemoryData.h"
#include "TempMemoryData.h"

class MemoryCatalogue;
class MemoryProfile;
class MemoryRecord;

//-----------------------------------------------------------------------------------------------------
class MemoryManager
{
public:
	static MemoryManager*               sharedInstance();
	static void                         destroyInstance();

    
    bool                                newProfile( const char* _key );
    bool                                deleteProfile( const char* _key );
    
	bool                                saveProfile();
	bool                                loadProfile( const char* _key );
    
    // uploading saved data to temp memory data
    void                                uploadPersistToTempData();
    
    // saving temp memory data before serialization
    void                                uploadTempToPersistData();
    
    std::string                         getCurrentProfile() const;
    
    const std::vector< std::string >&   getProfilesList() const;
    
    bool                                isProfileInList( const char* _key ) const;
    
    
    bool                                addRecord( const char* _name );
    bool                                removeRecord( const char* _name );
    
    std::string                         getCurrentRecord() const;
    bool                                setCurrentRecord( const char* _name );
    
    std::vector< std::string >          getRecordsList() const;
    
    bool                                isRecordInList( const char* _name ) const;
    
    
    PersistMemoryData*                  getPersistData() const;
    
    TempMemoryData*                     getTempData() const;
    
    
    
private:
	MemoryManager();
    MemoryManager( const MemoryManager& ){};
	~MemoryManager();
    
    MemoryManager& operator=( const MemoryManager& ){ return *this; }
   
    
private:
	static MemoryManager*       instance;
    
    MemoryCatalogue*            catalogue;
    MemoryProfile*              profile;
    MemoryRecord*               record;
    
    TempMemoryData*             tempData;
};

//-----------------------------------------------------------------------------------------------------
#endif	// __MEMORY_MANAGER__