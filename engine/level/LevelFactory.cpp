#include "LevelFactory.h"
// levels
#include "Level_1_1_part1.h"
#include "Level_1_1_part2.h"
#include "Level_1_1_part3.h"
#include "Level_1_2_part1.h"
#include "Level_1_2_part2.h"
#include "Level_1_3_part1.h"
#include "Level_1_boss.h"
// minigame levels
#include "Level_1_2_1_musicbox_1.h"
#include "Level_1_2_1_musicbox_2.h"
#include "Level_1_2_2_musicbox_3.h"
#include "Level_1_2_2_musicbox_4.h"
#include "Level_1_2_2_musicbox_5.h"
#include "Level_1_3_1_musicbox_1.h"
#include "Level_1_3_1_musicbox_2.h"
#include "Level_1_3_1_musicbox_3.h"

using namespace cocos2d;

//-----------------------------------------------------------------------------------------------------
ILevel* LevelFactory::createLevel( level_id _levelId )
{
	ILevel* pLevel = nullptr;
    
    switch( _levelId )
    {
        // levels
        case level_id::LVL_1_1_PART1:
            pLevel = Level_1_1_part1::create();
            break;
            
        case level_id::LVL_1_1_PART2:
            pLevel = Level_1_1_part2::create();
            break;
            
        case level_id::LVL_1_1_PART3:
            pLevel = Level_1_1_part3::create();
            break;
            
        case level_id::LVL_1_2_PART1:
            pLevel = Level_1_2_part1::create();
            break;
            
        case level_id::LVL_1_2_PART2:
            pLevel = Level_1_2_part2::create();
            break;
            
        case level_id::LVL_1_3_PART1:
            pLevel = Level_1_3_part1::create();
            break;
            
        case level_id::LVL_1_BOSS:
            pLevel = Level_1_boss::create();
            break;
            
            
        // minigame levels
        case level_id::MG_1_2_1_MUSICBOX_1:
            pLevel = Level_1_2_1_musicbox_1::create();
            break;
            
        case level_id::MG_1_2_1_MUSICBOX_2:
            pLevel = Level_1_2_1_musicbox_2::create();
            break;
            
        case level_id::MG_1_2_2_MUSICBOX_3:
            pLevel = Level_1_2_2_musicbox_3::create();
            break;
            
        case level_id::MG_1_2_2_MUSICBOX_4:
            pLevel = Level_1_2_2_musicbox_4::create();
            break;
            
        case level_id::MG_1_2_2_MUSICBOX_5:
            pLevel = Level_1_2_2_musicbox_5::create();
            break;
            
        case level_id::MG_1_3_1_MUSICBOX_1:
            pLevel = Level_1_3_1_musicbox_1::create();
            break;
            
        case level_id::MG_1_3_1_MUSICBOX_2:
            pLevel = Level_1_3_1_musicbox_2::create();
            break;
            
        case level_id::MG_1_3_1_MUSICBOX_3:
            pLevel = Level_1_3_1_musicbox_3::create();
            break;

            
        default:
            break;
    }
	
	return pLevel;
}

//-----------------------------------------------------------------------------------------------------