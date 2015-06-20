#include "TempMemoryData.h"

#include "Config.h" // TEMP

using namespace std;
using namespace cocos2d;

//-----------------------------------------------------------------------------------------------------
TempMemoryData::TempMemoryData() :
playerHP( 3 ),
playerFireballAmmo( 3 ),
playerCoins( 0 ),
playerPosition( Vec2::ZERO ),
cpPlayerHP( 0 ),
cpPlayerCoins( 0 ),
cpPlayerFireballAmmo( 0 ),
complexityLevel( 0 ),
currentLevelTimer( 0.0f ),
playerCoinsIntoPoints( 0 ),
playerVoice( VOICE_UNDEFINED ),
currentLevelStars( 0 ),
statBrokenMechs( 0 ),
statMusicTasks( 0 ),
statSleepingChars( 0 ),
statSecretPlaces( 0 ),
statTotalScore( 0 ),
currentLevelIndex( (int)level_id::NONE ),
locationCoinsIntoPoints( 0 ),
playerMoney( 1000 )
{
    //TEST
    this->gMod = GOD_MODE;
    this->debugDraw = DEBUG_DRAWING;
    this->vGetCoinIds.clear();
    this->vKilledMobsIds.clear();
    
    //init levels memory-progress data 
    for( int i = 0 ; i < 16 ; i++ )
        this->levelsProgressData[i] = new LevelMemoryData();
    
    // init amount of 100% coins on each level
    this->levelsProgressData[0]->levelCoinsGoal = 27;           //level 1-1-1
    this->levelsProgressData[1]->levelCoinsGoal = 34;           //level 1-1-2
    this->levelsProgressData[2]->levelCoinsGoal = 29;           //level 1-1-3
    this->levelsProgressData[3]->levelCoinsGoal = 26;           //level 1-2-1
    this->levelsProgressData[4]->levelCoinsGoal = 56;           //level 1-2-2
    this->levelsProgressData[5]->levelCoinsGoal = 58;           //level 1-3-1
    this->levelsProgressData[6]->levelCoinsGoal = 26;           //level 1-boss
    
    // init time needed to pass each level
    this->levelsProgressData[0]->levelTimeGoal = 55;           //level 1-1-1
    this->levelsProgressData[1]->levelTimeGoal = 55;           //level 1-1-2
    this->levelsProgressData[2]->levelTimeGoal = 57;           //level 1-1-3
    this->levelsProgressData[3]->levelTimeGoal = 120;          //level 1-2-1
    this->levelsProgressData[4]->levelTimeGoal = 95;           //level 1-2-2
    this->levelsProgressData[5]->levelTimeGoal = 85;           //level 1-3-1
    this->levelsProgressData[6]->levelTimeGoal = 120;          //level 1-boss
    
    //temp start
    this->levelsProgressData[0]->gameLevelsProgress = level_state::LVL_STATE_OPENED;
}

//-----------------------------------------------------------------------------------------------------
TempMemoryData::~TempMemoryData()
{
    for( int i = 0 ; i < 16 ; i++ )
    {
        delete this->levelsProgressData[i];
    }
}

//-----------------------------------------------------------------------------------------------------
void TempMemoryData::resetPlayerData()
{
    if( this->complexityLevel == 0 )
    {
        this->playerHP = 5;
        this->playerFireballAmmo = 3;
    }
    if( this->complexityLevel == 1 )
    {
        this->playerHP = 3;
        this->playerFireballAmmo = 3;
    }
    if( this->complexityLevel == 2 )
    {
        this->playerHP = 1;
        this->playerFireballAmmo = 3;
    }
    //ADDING CORRECT LOCATION RESTART
    //this->playerCoinsIntoPoints = 0;
    
    // reset player data
    this->playerCoins = 0;
    this->cpPlayerFireballAmmo = 0;
    this->cpPlayerHP = 0;
    // reset player stat
    this->statBrokenMechs = 0;
    this->statMusicTasks = 0;
    this->statSecretPlaces = 0;
    this->statSleepingChars = 0;
    this->statTotalScore = 0;
    // reset level index
    this->currentLevelIndex = 0;
}

//-----------------------------------------------------------------------------------------------------
void TempMemoryData::setLevelStars( int _id )
{
    //points coef
    int complexityCoef = 0;
    int timerCoins = 0;
    
    if( this->complexityLevel == 0 )
        complexityCoef = 2;
    else if( this->complexityLevel == 1 )
        complexityCoef = 3;
    else
        complexityCoef = 5;
    if( this->currentLevelTimer < this->levelsProgressData[_id]->levelTimeGoal )
    {
        timerCoins = ( this->levelsProgressData[_id]->levelTimeGoal - this->currentLevelTimer );	 // * timerCoef;
    }
    //updating record
    if( this->levelsProgressData[_id]->levelCoinsRecord < ( this->playerCoinsIntoPoints + timerCoins ) * complexityCoef )
    {
        this->levelsProgressData[_id]->levelCoinsRecord = ( this->playerCoinsIntoPoints + timerCoins ) * complexityCoef;
        //setting total score for complete_level_screen
        this->statTotalScore = levelsProgressData[_id]->levelCoinsRecord;
        
        //level passing quality counting
        int levelPassingPercentage = ( 100 * this->playerCoinsIntoPoints ) / levelsProgressData[ _id ]->levelCoinsGoal;
        
        //considering star's amount on the end of the level according to percent ( TODO : BALANCE )
        if( levelPassingPercentage >= 90 )
            this->levelsProgressData[ _id ]->levelPassingQuality = 3;
        else if( levelPassingPercentage >= 60 )
            this->levelsProgressData[ _id ]->levelPassingQuality = 2;
        else
            this->levelsProgressData[ _id ]->levelPassingQuality = 1;
        
        this->currentLevelStars = this->levelsProgressData[ _id ]->levelPassingQuality;
    }
    else
    {
        //setting total score for complete_level_screen
        this->statTotalScore = ( this->playerCoinsIntoPoints + timerCoins ) * complexityCoef;
    }
}

//-----------------------------------------------------------------------------------------------------
void TempMemoryData::resetAllLevelStates()
{
    for( auto i = this->globalLevelStates.begin() ; i != this->globalLevelStates.end() ; ++i )
        (*i).second = false;
    this->globalLevelStates.clear();
}

//-----------------------------------------------------------------------------------------------------
int TempMemoryData::getLevelState( std::string _id ) const
{
    auto it = this->globalLevelStates.find( _id );
    return ( it != this->globalLevelStates.end() ) ? it->second : false;
}

//-----------------------------------------------------------------------------------------------------
void TempMemoryData::setLevelState( string _id, int _state )
{
    this->globalLevelStates[ _id ] = _state;
}

//-----------------------------------------------------------------------------------------------------