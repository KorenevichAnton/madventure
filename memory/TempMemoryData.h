#ifndef __TEMP_MEMORY_DATA__
#define __TEMP_MEMORY_DATA__

#include "cocos2d.h"
#include "AudioDefinitions.h"
#include "LevelMemoryData.h"
#include "LevelList.h"

//-----------------------------------------------------------------------------------------------------
class TempMemoryData
{
public:
    TempMemoryData();
    ~TempMemoryData();
 
    
public:
    //donate
    int                                     playerMoney;
    
    //player data
    int                                     playerHP;
    int                                     playerFireballAmmo;
    cocos2d::Vec2                           playerPosition;
    int                                     playerCoins;
    
    //complete screen stat fields
    int                                     statBrokenMechs;
    int                                     statMusicTasks;
    int                                     statSleepingChars;
    int                                     statSecretPlaces;
    int                                     statTotalScore;
    
    //check point player data
    int                                     cpPlayerHP;
    int                                     cpPlayerFireballAmmo;
    int                                     cpPlayerCoins;
    
    //location save data ( timer and coins into points for level)
    int                                     locationCoinsIntoPoints;
    
    //level index for complete and difficulty screen
    int                                     currentLevelIndex;
    
    //level timer
    float                                   currentLevelTimer;
    
    //coins witch going in points
    int                                     playerCoinsIntoPoints;
    
    //coins and mobs for hide after restart level without reset-states
    std::vector< int >                      vGetCoinIds;
    std::vector< int >                      vKilledMobsIds;
    
    //reset player data according to game complexity
    void                                    resetPlayerData();
    
    //Level data
    int                                     getLevelState( std::string _id ) const;
    void                                    setLevelState( std::string _id, int _state );
    void                                    resetAllLevelStates();
    
    //global game progress states
    std::map< std::string, int >            globalLevelStates;
 
    //complexity levels
    //0 - "student"
    //1 - "already can"
    //2 - "maestro"
    int                                     complexityLevel;
    int                                     currentLevelStars;
    
    // type of voice of player
    voice                                   playerVoice;
    
    // consider amount of stars according to coins record on the level
    void                                    setLevelStars( int _id );
    
    // levels data
    LevelMemoryData*                        levelsProgressData[ 16 ];
    
    //FIELDS FOR TEST
    bool                                    gMod;
    bool                                    debugDraw;
    
};

//-----------------------------------------------------------------------------------------------------
#endif	//__TEMP_MEMORY_DATA__