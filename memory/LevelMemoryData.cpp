#include "LevelMemoryData.h"

using namespace std;
using namespace cocos2d;

//------------------------------------------------------------------------
LevelMemoryData::LevelMemoryData() :
gameLevelsProgress( level_state::LVL_STATE_CLOSED ),
levelPassingQuality( 0 ),
levelCoinsGoal( 0 ),
levelCoinsRecord( 0 ),
levelTimeGoal( 0 )
{

}

//------------------------------------------------------------------------
LevelMemoryData::~LevelMemoryData()
{
}

//------------------------------------------------------------------------

