#ifndef __ILEVEL__
#define __ILEVEL__

#include "LevelList.h"
#include "ScreenList.h"

#include "GameCamera.h"
#include "AiPlayerObject.h"
#include "IScene.h"

//-----------------------------------------------------------------------------------------------------
#define SOUND_BOSS_DEFEAT           "audio/scenes/locationSounds/bossDefeatSound.wav"
#define SOUND_HIDEN_LOCATION        "audio/scenes/locationSounds/hidenLocationSound.wav"
#define SOUND_DESTROYED_PLATFORM    "audio/scenes/locationSounds/destroyedPlatformSound.wav"
#define SOUND_TAKEN_NOTES           "audio/scenes/locationSounds/takenNotes"

#define EMBIENT_BIRDS               "audio/embients/birdSinging_bm.wav"
#define EMBIENT_DWARFS              "audio/embients/dwarfsWorkshop_bm.wav"

#define MUSIC_CAVE                  "audio/backMusic/caveMusic.wav"
#define MUSIC_ISLAND                "audio/backMusic/islandsMusic.wav"
#define MUSIC_LVL1                  "audio/backMusic/islandMusicLvl1.wav"
#define MUSIC_WAR                   "audio/backMusic/warMusic2.wav"
#define MUSIC_CASTLE                "audio/backMusic/castleMusic.wav"

#define FALLING_SOUND               "audio/player/fallingSound.wav"
#define COMPLETE_SOUND              "audio/player/completeLevelSound.wav"

#define MAX_VOLUME_EMBIENT_BIRDS 0.2f

#define MIN_VOLUME_EMBIENT_MUSIC 0.0f
#define MAX_VOLUME_EMBIENT_MUSIC 0.4f

//-----------------------------------------------------------------------------------------------------
/** embiant and back music depends on the are type */
enum area_type
{
    AREA_NONE = -1,
    AREA_AIR_PLATFORMS = 0,
    AREA_CAVE,
    AREA_CASTLE,
    AREA_WAR
};

/** different kinds of coins for some minigames
 * each kind has its properties in minigame
 */
enum PhysTriggerCoinKind
{
    COIN_DEFAULT = 0,
    COIN_HP,
    COIN_GOLD_HP,
    COIN_TASK,
    COIN_ROOM_CORRECT_FULL,
    COIN_ROOM_CORRECT_HALF,
    COIN_ROOM_CORRECT_QUARTER,
    COIN_ROOM_CORRECT_EIGHTH,
    COIN_ROOM_UNCORRECT,
    COIN_MUSIC_BOX_3_05,
    COIN_MUSIC_BOX_3_025,
    COIN_MUSIC_BOX_3_0125,
    COIN_MUSIC_BOX_4_00625,
    COIN_MUSIC_BOX_3_NONE,
    COIN_NOTE_1,
    COIN_NOTE_2,
    COIN_NOTE_3,
    COIN_NOTE_4,
    COIN_NOTE_5,
    COIN_NOTE_6,
    COIN_NOTE_7,
    COIN_BULLET
};

//-----------------------------------------------------------------------------------------------------
class DialogScreen;
class HudScreen;

class ILevel
{
public:
    virtual void                                    release() = 0;
    virtual void                                    update(float _dt) = 0;
    
    virtual bool                                    init() = 0;
    
    virtual void                                    onEnter() = 0;
    virtual void                                    onExit() = 0;
    
    virtual level_id                                getLevelName() const { return level; };
    
    virtual level_id                                getRestartLevel() const { return restartLevel; };
    
    virtual area_type                               getAreaType() const { return areaType; };
    
    // pathes for loading level landscapes and level objects
    const char*                                     getLevelGameWorldPath() const { return levelGameWorldPath; };
    const char*                                     getLevelGameObjectsPath() const { return levelGameObjectsPath; };
    
    ai::AiPlayerObject*                             getPlayer() const { return pPlayer; };
    GameCamera*                                     getGameCamera() const { return pGameCamera; };
    void                                            setGameWorld( GameWorld* _pGameWorld );
    GameWorld*                                      getGameWorld() const { return pGameWorld; };

    // player data for statistics counting
    int                                             getLocalPlayerCoins() const { return localPlayerCoins; };
    void                                            setLocalPlayerCoins( int _localPlayerCoins ) { localPlayerCoins = _localPlayerCoins; };

    int                                             getLocalPlayerCoinsIntoPoints() { return this->localPlayerCoinsIntoPoints; }
    void                                            setLocalPlayerCoinsIntoPoints( int _localPlayerCoinsIntoPoints ) { this->localPlayerCoinsIntoPoints = _localPlayerCoinsIntoPoints; };
    
    // coins and mobs for release on restart level from checkpoint
    void                                            addLocalGetCoinsId( int _id ) { this->vLocalGetCoinIds.push_back( _id ); };
    void                                            addLocalKilledMobsId( int _id ) { this->vLocalKilledMobsIds.push_back( _id ); };
    
    virtual void                                    onVictory() = 0;
    virtual void                                    onDefeat() = 0;
    
    virtual bool                                    onGetCoin( GameObject* _coin, std::string _key, PhysTriggerCoinKind _kind ) = 0;
    virtual void                                    onPortal( GameObject* _portal ) = 0;
    virtual void                                    onCheckPoint( GameObject* _checkPoint ) = 0;
    virtual void                                    onMinigameEnd( bool _isWin, screen_id _screen ) = 0;
    virtual void                                    onSwitchArea( GameObject* _switcher ) = 0;
    virtual void                                    onDialogueEnd( int _id ) = 0;
    virtual void                                    saveLevelStatData() = 0;
    
    //positioning player on enter by states
    virtual void                                    determinePlayerPosition() {};
    
    //objects tampering
    GameObject*                                     getGameObjectById(int _id);
    void                                            showGameObject(int _id);
    void                                            hideGameObject(int _id);
    
    IAudioSource*                                   getEmbiantSource() { return embiantSource; };
    IAudioSource*                                   getBackMusicSource() { return backMusicSource; };
    
    //reseting all logic level states (local)
    void                                            resetAllLevelStates();
    
protected:
    ILevel();
    ~ILevel(){};
    
    //Level data
    int                                             getLevelState( std::string _id ) const;
    void                                            setLevelState( std::string _id, int _state );
    
    //uploading current local level states to global in TempMemoryData
    void                                            uploadLevelStates();
  
    //calc time for moving camera
    float                                           cameraSpeedCalc();
    
protected:
    GameWorld*                          pGameWorld;
    ai::AiPlayerObject*                 pPlayer;
    GameCamera*                         pGameCamera;
    
    // most usable screens in logic
    DialogScreen*                       pDialogScreen;
    HudScreen*                          pHudScreen;
    
    //level id's for replacing on next level / restarting current level and getting current one
    level_id                            restartLevel;
    level_id                            nextLevel;
    level_id                            level;
    
    //current level area showing which music/embiant should play
    area_type                           areaType;
    
    char*                               levelGameWorldPath;
    char*                               levelGameObjectsPath;
    
    // back music / embiant on each level
    IAudioSource*                       embiantSource;
    IAudioSource*                       backMusicSource;
    
    //current level states, level's progress 
    std::map< std::string, int >        localLevelStates;
    
    int                                 localPlayerCoinsIntoPoints;
    int                                 localPlayerCoins;
    
    // coins and mobs for release on restart level from checkpoint
    std::vector< int >                  vLocalGetCoinIds;
    std::vector< int >                  vLocalKilledMobsIds;

    
    //timer for joy delay / win game / defeat delay
    float                               winTimer;
    float                               winTimerLimit;
    bool                                winState;
    
    float                               defeatTimer;
    float                               defeatTimerLimit;
    bool                                defeatState;
};

//-----------------------------------------------------------------------------------------------------
#endif  // __ILEVEL__