#include "MemoryManager.h"
#include "AppController.h"

#include "MemoryCatalogue.h"
#include "MemoryProfile.h"
#include "MemoryRecord.h"

#include <fstream>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>


#define CATALOGUE_NAME          "memory"
#define CATALOGUE_EXTENSION     "ctg"
#define PROFILE_EXTENSION       "sav"

using namespace std;
using namespace cocos2d;

MemoryManager* MemoryManager::instance = nullptr;

//-----------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------
template< typename Сontent >
bool serialization( const Сontent* _content, const char* _fileName, const char* _fileExtension )
{
    std::string file = GET_DOCUMENTS_PATH + _fileName + "." + _fileExtension;
    std::ofstream ofs( file, std::ios::out | std::ios::binary );
    if( ofs.is_open() )
    {
        boost::archive::binary_oarchive archive( ofs );
        archive << *_content;
            
        ofs.close();
            
        return true;
    }
    
    return false;
}

//-----------------------------------------------------------------------------------------------------
template< typename Сontent >
bool deserialization( Сontent* _content, const char* _fileName, const char* _fileExtension )
{
    std::string file = GET_DOCUMENTS_PATH + _fileName + "." + _fileExtension;
    
    std::ifstream ifs( file, std::ios::in | std::ios::binary );
    if( ifs.is_open() )
    {
        boost::archive::binary_iarchive archive( ifs );
        archive >> *_content;
        
        ifs.close();
        
        return true;
    }

    return false;
}

//-----------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------
MemoryManager* MemoryManager::sharedInstance()
{
	if( !MemoryManager::instance )
		MemoryManager::instance = new MemoryManager();

    return MemoryManager::instance;
}

//-----------------------------------------------------------------------------------------------------
void MemoryManager::destroyInstance()
{
	if( MemoryManager::instance )
	{
		delete MemoryManager::instance;
		MemoryManager::instance = nullptr;
	}
}

//-----------------------------------------------------------------------------------------------------
MemoryManager::MemoryManager():
catalogue( nullptr ),
profile( nullptr ),
record( nullptr ),
tempData( nullptr )
{
    // initialize memory catalog from ROM if possible, otherwise use default catalog
    this->catalogue = MemoryCatalogue::create();
    if( deserialization( this->catalogue, CATALOGUE_NAME, CATALOGUE_EXTENSION ) )
    {
        // initialize current profile from ROM if possible
        this->profile = MemoryProfile::create( "" );
        if( !deserialization( this->profile, this->catalogue->getCurrentProfileKey().c_str(), PROFILE_EXTENSION ) )
        {
            this->profile->release();
            this->profile = nullptr;
        }
    }
    
    
    // initialize temporary memory data
    this->tempData = new TempMemoryData();
}

//-----------------------------------------------------------------------------------------------------
MemoryManager::~MemoryManager() 
{
    // release memory catalog
    this->catalogue->release();
    this->catalogue = nullptr;
    
    // release current profile
    if( this->profile )
    {
        this->profile->release();
        this->profile = nullptr;
    }
}

//-----------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------
bool MemoryManager::newProfile( const char* _key )
{
    if( _key && !this->catalogue->isProfileKeyInList( _key ) )
    {
        // backup old object of current profile
        MemoryProfile* tempProfile = this->profile;
    
        // create new object of current profile
        this->profile = MemoryProfile::create( _key );
        
        // save new file of current profile on ROM
        if( this->saveProfile() )
        {
            this->catalogue->addProfileKey( _key );
            this->catalogue->setCurrentProfileKey( _key );
            
            if( serialization( this->catalogue, CATALOGUE_NAME, CATALOGUE_EXTENSION ) )
            {
                // release old object of profile
                if( tempProfile )
                {
                    tempProfile->release();
                    tempProfile = nullptr;
                }
                
                // reset current record
                this->record = nullptr;
                
                log( "MEMORY_MANAGER: Creating new profile '%s' is success!", _key );
            
                return true;
            }
            
            this->catalogue->removeProfileKey( _key );
            this->catalogue->setCurrentProfileKey( tempProfile ? tempProfile->getKey() : "" );
            
            // remove new file of current profile from ROM
            this->deleteProfile( _key );
        }
        
        // release new object of current profile
        this->profile->release();
        this->profile = nullptr;
        
        // restore the old object of current profile
        this->profile = tempProfile;
    }
    
    log( "MEMORY_MANAGER: Creating new profile '%s' is failed!", _key );
    
    return false;
}

//-----------------------------------------------------------------------------------------------------
bool MemoryManager::deleteProfile( const char* _key )
{
    if( _key )
    {
        string file = GET_DOCUMENTS_PATH + _key + "." + PROFILE_EXTENSION;
        
        if( remove( file.c_str() ) == 0 )
        {
            this->catalogue->removeProfileKey( _key );
            serialization( this->catalogue, CATALOGUE_NAME, CATALOGUE_EXTENSION );

            log( "MEMORY_MANAGER: Deleting profile '%s' is success!", _key );
        
            return true;
        }
    }
    
    log( "MEMORY_MANAGER: Deleting profile '%s' is failed!", _key );
    
    return false;
}

//-----------------------------------------------------------------------------------------------------
bool MemoryManager::saveProfile()
{
    if( this->profile )
    {
        if( serialization( this->profile, this->profile->getKey().c_str(), PROFILE_EXTENSION ) )
        {
            log( "MEMORY_MANAGER: Saving profile '%s' is success!", this->profile->getKey().c_str() );
        
            return true;
        }
    }
    
    log( "MEMORY_MANAGER: Saving profile '%s' is failed!", this->profile ? this->profile->getKey().c_str() : "" );
    
    return false;
}

//-----------------------------------------------------------------------------------------------------
bool MemoryManager::loadProfile( const char* _key )
{
    if( _key && this->catalogue->isProfileKeyInList( _key ) )
    {
        // create new object of current profile
        MemoryProfile* tempProfile = MemoryProfile::create( _key );
        
        if( deserialization( tempProfile, _key, PROFILE_EXTENSION ) )
        {
            this->catalogue->setCurrentProfileKey( _key );
            
            if( serialization( this->catalogue, CATALOGUE_NAME, CATALOGUE_EXTENSION ) )
            {
                // remove old object of current profile
                if( this->profile )
                {
                    this->profile->release();
                    this->profile = nullptr;
                }
                
                // set new object of current profile
                this->profile = tempProfile;
                
                // reset current record
                this->record = nullptr;
                
                log( "MEMORY_MANAGER: Loading profile '%s' is success!", _key );
                
                return true;
            }
            
            this->catalogue->setCurrentProfileKey( this->profile ? this->profile->getKey() : "" );
        }
        
        // release temp object
        tempProfile->release();
        tempProfile = nullptr;
    }
    
    log( "MEMORY_MANAGER: Loading profile '%s' is failed!", _key );
    
    return false;
}

//-----------------------------------------------------------------------------------------------------
string MemoryManager::getCurrentProfile() const
{
    string key = "";
    
    if( this->profile )
        key = this->profile->getKey();
    
    return key;
}

//-----------------------------------------------------------------------------------------------------
const vector< string >& MemoryManager::getProfilesList() const
{
    return this->catalogue->getProfilesList();
}

//-----------------------------------------------------------------------------------------------------
bool MemoryManager::isProfileInList( const char* _key ) const
{
    return this->catalogue->isProfileKeyInList( _key );
}

//-----------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------
bool MemoryManager::addRecord( const char* _name )
{
    if( this->profile && _name && !this->isRecordInList( _name ) )
    {
        auto pRecord = MemoryRecord::create( _name );
        this->profile->addRecord( pRecord );
        
        log( "MEMORY_MANAGER: Adding record '%s' is success!", _name );
        
        return true;
    }
    
    log( "MEMORY_MANAGER: Adding record '%s' is failed!", _name );
    
    return false;
}

//-----------------------------------------------------------------------------------------------------
bool MemoryManager::removeRecord( const char* _name )
{
    if( this->profile && _name )
    {
        const vector< MemoryRecord* >& recordsList = this->profile->getRecordsList();
        for( auto it = recordsList.begin(); it != recordsList.end(); it++ )
        {
            if( ( *it )->getName() == _name )
            {
                this->profile->removeRecord( *it );
                
                log( "MEMORY_MANAGER: Removing record '%s' is success!", _name );
                
                return true;
            }
        }
    }
    
    log( "MEMORY_MANAGER: Removing record '%s' is failed!", _name );
    
    return false;
}

//-----------------------------------------------------------------------------------------------------
string MemoryManager::getCurrentRecord() const
{
    string name = "";
    
    if( this->record )
        name = this->record->getName();
    
    return name;
}

//-----------------------------------------------------------------------------------------------------
bool MemoryManager::setCurrentRecord( const char* _name )
{
    if( this->profile && _name )
    {
        const vector< MemoryRecord* >& recordsList = this->profile->getRecordsList();
        for( auto it = recordsList.begin(); it != recordsList.end(); it++ )
        {
            if( ( *it )->getName() == _name )
            {
                this->record = *it;
                
                log( "MEMORY_MANAGER: Setting record '%s' to current is success!", _name );
                
                return true;
            }
        }
    }
    
    log( "MEMORY_MANAGER: Setting record '%s' to current is failed!", _name );
    
    return false;
}

//-----------------------------------------------------------------------------------------------------
vector< string > MemoryManager::getRecordsList() const
{
    vector< string > tempList;
    
    if( this->profile )
    {
        const vector< MemoryRecord* >& recordsList = this->profile->getRecordsList();
        for( auto it = recordsList.begin(); it != recordsList.end(); it++ )
            tempList.push_back( ( *it )->getName() );
    }
    
    return tempList;
}

//-----------------------------------------------------------------------------------------------------
bool MemoryManager::isRecordInList( const char* _name ) const
{
    if( this->profile && _name )
    {
        const vector< MemoryRecord* >& recordsList = this->profile->getRecordsList();
        for( auto it = recordsList.begin(); it != recordsList.end(); it++ )
        {
            if( ( *it )->getName() == _name )
                return true;
        }
    }
    
    return false;
}

//-----------------------------------------------------------------------------------------------------
void MemoryManager::uploadPersistToTempData()
{
    PersistMemoryData* persistData = nullptr;
    
    if( this->tempData )
    {
        if( this->record )
        {
            persistData = this->record->getData();
            // uploading player voice
            this->tempData->playerVoice = persistData->playerVoice;
            
            for( int i = 0 ; i < persistData->levelSize ; i++ )
            {
                // uploading player game progress and coins statistics
                this->tempData->levelsProgressData[i]->gameLevelsProgress = persistData->gameLevelsProgress[i];
                //this->tempData->levelsProgressData[i]->levelCoinsGoal = persistData->levelCoinsGoal[i];
                this->tempData->levelsProgressData[i]->levelCoinsRecord = persistData->levelCoinsRecord[i];
                this->tempData->levelsProgressData[i]->levelPassingQuality = persistData->levelPassingQuality[i];
            }
        }
    }
}

//-----------------------------------------------------------------------------------------------------
void MemoryManager::uploadTempToPersistData()
{
    PersistMemoryData* persistData = nullptr;
    
    if( this->record )
    {
        persistData = this->record->getData();
        // uploading player voice
        persistData->playerVoice = this->tempData->playerVoice;
        
        for( int i = 0 ; i < persistData->levelSize ; i++ )
        {
            // uploading player game progress and coins statistics
            persistData->gameLevelsProgress[i] = this->tempData->levelsProgressData[i]->gameLevelsProgress;
            //persistData->levelCoinsGoal[i] = this->tempData->levelsProgressData[i]->levelCoinsGoal;
            persistData->levelCoinsRecord[i] = this->tempData->levelsProgressData[i]->levelCoinsRecord;
            persistData->levelPassingQuality[i] = this->tempData->levelsProgressData[i]->levelPassingQuality;
        }
    }
}

//-----------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------
PersistMemoryData* MemoryManager::getPersistData() const
{
    PersistMemoryData* persistData = nullptr;
    
    if( this->record )
        persistData = this->record->getData();
        
    return persistData;
}

//-----------------------------------------------------------------------------------------------------
TempMemoryData* MemoryManager::getTempData() const
{
    return this->tempData;
}

//-----------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------