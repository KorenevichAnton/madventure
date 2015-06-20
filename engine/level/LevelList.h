#ifndef __LEVEL_LIST__
#define __LEVEL_LIST__

//-----------------------------------------------------------------------------------------------------
/// List of levels.
/** IMPORTANT: Always if you add a new class of level, you need to add him in LevelList and LevelFactory!
 */
enum class level_id
{
    NONE = 0,
    
    LVL_1_1_PART1,
    LVL_1_1_PART2,
    LVL_1_1_PART3,
    LVL_1_2_PART1,
    LVL_1_2_PART2,
    LVL_1_3_PART1,
    LVL_1_BOSS,
    
    // minigame levels
    MG_1_1_QUEST_1_1,
    MG_1_1_1_SEVEN_NOTES,
    MG_1_1_QUEST_1_3,
    MG_1_1_QUEST_1_2,
    MG_1_1_QUEST_1_4,
    MG_1_1_QUEST_2_1,
    MG_1_1_QUEST_2_2,
    MG_1_1_QUEST_2_3,
    MG_1_1_QUEST_2_4,
    MG_1_1_QUEST_3_1,
    MG_1_1_QUEST_3_2,
    MG_1_1_QUEST_3,
    MG_1_1_QUEST_4,
    MG_1_2_MUSICBOX_1,
    MG_1_2_MUSICBOX_2,
    MG_1_2_QUEST_7,
    MG_1_2_QUEST_8,
    MG_1_2_QUEST_9,
    MG_1_3_1_QUEST_1,
    MG_1_3_1_QUEST_2,
    MG_1_3_1_QUEST_3,
    MG_1_2_2_MUSICBOX_3,
    MG_1_2_2_MUSICBOX_4,
    MG_1_2_2_MUSICBOX_5,
    MG_1_2_1_MUSICBOX_1,
    MG_1_2_1_MUSICBOX_2,
    MG_1_3_1_MUSICBOX_1,
    MG_1_3_1_MUSICBOX_2,
    MG_1_3_1_MUSICBOX_3
};

//-----------------------------------------------------------------------------------------------------
#endif  // __LEVEL_LIST__