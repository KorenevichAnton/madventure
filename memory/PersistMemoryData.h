#ifndef __PERSIST_MEMORY_DATA__
#define __PERSIST_MEMORY_DATA__

#include "cocos2d.h"
#include "AudioDefinitions.h"
#include "LevelMemoryData.h"

#include <boost/serialization/vector.hpp>
#include <boost/serialization/array.hpp>

//-----------------------------------------------------------------------------------------------------
class PersistMemoryData
{
public:
    PersistMemoryData();
    ~PersistMemoryData();
    
    template< class Archive >
    void serialize( Archive& _archive, const unsigned int _version )
    {
        _archive & this->levelSize;
        _archive & boost::serialization::make_array< level_state >( this->gameLevelsProgress, this->levelSize );
        _archive & boost::serialization::make_array< int >( this->levelCoinsGoal, this->levelSize );
        _archive & boost::serialization::make_array< int >( this->levelCoinsRecord, this->levelSize );
        _archive & boost::serialization::make_array< int >( this->levelPassingQuality, this->levelSize );
        _archive & this->playerVoice;
    }
    
public:
    int                                     levelSize;
    
    // current game progress
    level_state*                            gameLevelsProgress;
    
    // amount of 100% coins on them level
    int*                                    levelCoinsGoal;
    
    // max amount of picked up coins
    int*                                    levelCoinsRecord;
    
    // amount of stars getting on the end of the level
    int*                                    levelPassingQuality;
    
    // type of voice of player
    voice                                   playerVoice;
};

//-----------------------------------------------------------------------------------------------------
#endif	//__PERSIST_MEMORY_DATA__