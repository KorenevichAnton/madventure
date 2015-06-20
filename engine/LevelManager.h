#ifndef __LEVEL_MANAGER__
#define __LEVEL_MANAGER__

#include "ILevel.h"
#include "LevelList.h"

//-----------------------------------------------------------------------------------------------------
/// Class for management of levels.
/** This class is singleton.
 */
class LevelManager
{
public:
    static LevelManager*                    sharedInstance();
    static void                             destroyInstance();

    /// External update.
    /** Updates takes from AppController. Do not call this method!
     */
    void                                    update( float _dt );
    
    
    // Current level-logic script
    ILevel*                                 getCurrentLevel() { return currentLevel; }
    
    // replacing between to locations (level and minigame for example)
    void                                    replaceLevel(level_id _level);
    // replacing from gamescene to another one
    void                                    replaceLevelToScene( scene_id _scene );
    // deleting current level
    void                                    releaseLevel();
    
    // timer controll on each level for statistics
    void                                    stopLevelTimer() { this->levelTimerState = false; this->firstTouchHUD = false; };
    void                                    resumeLevelTimer() { this->levelTimerState = true; };
    void                                    resetLevelTimer() { this->levelTimer = 0.0f; };
    
    // starting level timer on first move
    bool                                    getLevelTimerState() { return this->levelTimerState; };
    void                                    setFirstTouchHUD( bool _state ) { this->firstTouchHUD = _state; };
    
private:
    LevelManager();
    LevelManager( const LevelManager& ){};
    ~LevelManager();
    
    LevelManager& operator=( const LevelManager& ){ return *this; }

private:
    static LevelManager*    instance;
    
    bool                    doOnEnterFirst;
    
    ILevel*                 currentLevel;
    
    level_id                levelName;
    level_id                replaceLocationName;

    float                   levelTimer;
    bool                    levelTimerState;
    bool                    firstTouchHUD;  // state for starting timer after first touch on hud
};

//-----------------------------------------------------------------------------------------------------
#endif  // __LEVEL_MANAGER__