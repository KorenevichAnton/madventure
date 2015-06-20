#ifndef __LEVEL_FACTORY__
#define __LEVEL_FACTORY__

#include "ILevel.h"
#include "LevelList.h"

//-----------------------------------------------------------------------------------------------------
/// Class factory of levels
/** IMPORTANT: Always if you add a new class of level, you need to add him in LevelList and LevelFactory!
 */
class LevelFactory
{
public:
	static ILevel*      createLevel( level_id _levelId );
    
private:
	LevelFactory(){};
	~LevelFactory(){};
};

//-----------------------------------------------------------------------------------------------------
#endif  // __LEVEL_FACTORY__
