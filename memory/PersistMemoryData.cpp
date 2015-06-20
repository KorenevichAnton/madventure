#include "PersistMemoryData.h"

//-----------------------------------------------------------------------------------------------------
PersistMemoryData::PersistMemoryData()
{
    this->levelSize = 16;
    this->gameLevelsProgress = new level_state[16];
    this->levelCoinsGoal = new int[16];
    this->levelCoinsRecord = new int[16];
    this->levelPassingQuality = new int[16];
    this->playerVoice = voice::VOICE_UNDEFINED;
    
    for( int i = 0 ; i < levelSize ; i++ )
    {
        this->gameLevelsProgress[i] = level_state::LVL_STATE_CLOSED;
        this->levelCoinsGoal[i] = 0;
        this->levelCoinsRecord[i] = 0;
        this->levelPassingQuality[i] = 0;
    }
}

//-----------------------------------------------------------------------------------------------------
PersistMemoryData::~PersistMemoryData()
{
    //delete[] this->levelCoinsRecord;
    //delete[] this->gameLevelsProgress;
    //delete[] this->levelPassingQuality;
    //delete[] this->levelCoinsGoal;
}

//-----------------------------------------------------------------------------------------------------