#ifndef __MusicalAdventure__LevelMemoryData__
#define __MusicalAdventure__LevelMemoryData__

#include "cocos2d.h"

//------------------------------------------------------------------------
enum class level_state
{
    LVL_STATE_CLOSED = 0,
    LVL_STATE_OPENED,
    LVL_STATE_COMPLETED,
    LVL_STATE_DONATE_LOCK
};

//------------------------------------------------------------------------
class LevelMemoryData
{
public:
    LevelMemoryData();
    ~LevelMemoryData();
    
public:
    // current game progress
    level_state                             gameLevelsProgress;
    
    // amount of stars getting on the end of the level
    int                                     levelPassingQuality;
    
    // amount of 100% coins on them level
    int                                     levelCoinsGoal;
    
    // time needed to pass level
    int                                     levelTimeGoal;
    
    // max amount of picked up coins
    int                                     levelCoinsRecord;
    
};

//------------------------------------------------------------------------
#endif /* defined(__MusicalAdventure__LevelMemoryData__) */
