#include "LevelManager.h"
#include "LevelFactory.h"
#include "AppController.h"

LevelManager* LevelManager::instance = 0;

//-----------------------------------------------------------------------------------------------------
LevelManager::LevelManager():
currentLevel( nullptr ),
replaceLocationName( level_id::NONE ),
levelTimer( 0.0f ),
levelTimerState( false ),
doOnEnterFirst( false ),
firstTouchHUD( false )
{
}

//-----------------------------------------------------------------------------------------------------
LevelManager::~LevelManager()
{
}

//-----------------------------------------------------------------------------------------------------
LevelManager* LevelManager::sharedInstance()
{
    if(!instance)
        instance = new LevelManager();
    
    return instance;
}

//-----------------------------------------------------------------------------------------------------
void LevelManager::releaseLevel()
{
    if(currentLevel)
    {
        GET_RUNNING_SCENE->stopAllActions();
        this->currentLevel->onExit();
        currentLevel->release();
        currentLevel = nullptr;
    }
}

//-----------------------------------------------------------------------------------------------------
void LevelManager::replaceLevel(level_id _level)
{
    this->replaceLocationName = _level;
    
    if( APPLICATION->isPaused() )
        this->update( 0 );
}

//-----------------------------------------------------------------------------------------------------
void LevelManager::replaceLevelToScene( scene_id _scene )
{
    this->releaseLevel();
    
    MANAGER_Scene->replaceScene( _scene );
}

//-----------------------------------------------------------------------------------------------------
void LevelManager::update( float _dt )
{
    if( this->replaceLocationName != level_id::NONE )
    {
        this->releaseLevel();
        
        this->currentLevel = LevelFactory::createLevel( this->replaceLocationName );
        this->replaceLocationName = level_id::NONE;
        
        MANAGER_Scene->replaceScene( scene_id::GAME, true );
        
        this->currentLevel->init();
        this->currentLevel->determinePlayerPosition();
        
        this->doOnEnterFirst = true;
        
        return;
    }
    
    
    if( this->currentLevel )
    {
        IScene* pCurrentScene = GET_RUNNING_SCENE;
        if( pCurrentScene && pCurrentScene->getSceneId() == scene_id::GAME )
        {
            if( pCurrentScene->isRunning() )
            {
                if( this->doOnEnterFirst )
                {
                    this->currentLevel->onEnter();
                    this->doOnEnterFirst = false;
                }
                
                // Updating.
                this->currentLevel->update( _dt );
                
                // Level timing.
                if( this->levelTimerState && this->firstTouchHUD )
                {
                    this->levelTimer += _dt;
                    MANAGER_Memory->getTempData()->currentLevelTimer = this->levelTimer;
                }
            }
        }
    }
}

//-----------------------------------------------------------------------------------------------------
void LevelManager::destroyInstance()
{
    delete instance;
    instance = NULL;
}

//-----------------------------------------------------------------------------------------------------