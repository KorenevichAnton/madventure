//
//  Level_1_1_part2.cpp
//  MusicalAdventure
//
//  Created by ampa_games on 7/23/14.
//  Copyright (c) 2014 ampagames. All rights reserved.
//

#include "Level_1_1_part2.h"

#include "HudScreen.h"

#include "GameObject.h"

#include "CompleteScreen.h"
#include "TryMoreScreen.h"
#include "DialogScreen.h"

#include "GraphicAnimationObject.h"

using namespace cocos2d;
using namespace std;

//-----------------------------------------------------------------------------------------------------
Level_1_1_part2::Level_1_1_part2()
{
    level = level_id::LVL_1_1_PART2;
    nextLevel = level_id::LVL_1_1_PART3;
    
    this->levelGameWorldPath = new char[50];
    sprintf(levelGameWorldPath, "%s%s", PHYS_LANDSCAPES_PATH , "landscape1_1_2.xml");
    this->levelGameObjectsPath = new char[50];
    sprintf(levelGameObjectsPath, "%s%s", PHYS_OBJECTS_PATH , "objects1_1_2.xml");
}

//-----------------------------------------------------------------------------------------------------
Level_1_1_part2::~Level_1_1_part2()
{

}

//-----------------------------------------------------------------------------------------------------
Level_1_1_part2* Level_1_1_part2::create()
{
    Level_1_1_part2* pLevel_1_1_part2 = new Level_1_1_part2();
    if(pLevel_1_1_part2)
    {
        return pLevel_1_1_part2;
    }
    else
    {
        delete pLevel_1_1_part2;
        return NULL;
    }
}

//-----------------------------------------------------------------------------------------------------
bool Level_1_1_part2::init()
{
    //set using screens
    this->pDialogScreen = dynamic_cast< DialogScreen* > ( MANAGER_Screen->getScreen( GET_RUNNING_SCENE, screen_id::DIALOG ) );
    this->pHudScreen = dynamic_cast< HudScreen* >( MANAGER_Screen->getScreen( GET_RUNNING_SCENE, screen_id::HUD) );
    
    //Logic on enter level
    //
    
    //removing taken coins
    vector< int > coinsToRemove = vLocalGetCoinIds;
    for( int i = 0 ; i < coinsToRemove.size() ; i++ )
        this->hideGameObject( coinsToRemove[i] );
    //removing killed mobs
    vector< int > mobsToRemove = vLocalKilledMobsIds;
    for( int i = 0 ; i < mobsToRemove.size() ; i++ )
        this->hideGameObject( mobsToRemove[i] );
    
    //hide used check-points
    if( this->getLevelState( "level_1_1_2_cp_1" ) == 1 )
        this->hideGameObject( 970 );
    
    return true;
}

//-----------------------------------------------------------------------------------------------------
void Level_1_1_part2::onEnter()
{
    //level timer
    MANAGER_Level->resumeLevelTimer();
    
    //on restart level from pause/defeat/win level loading
    this->restartLevel = this->level;
    
    //Add back music
    this->areaType = AREA_AIR_PLATFORMS;
    this->embiantSource->loadAudioFile( EMBIENT_BIRDS );
    this->embiantSource->loadSettings( EMBIENT_BIRDS );
    this->embiantSource->setLooping( true );
    this->embiantSource->play();
    this->embiantSource->setAssociate( AudioSourceAssociate::MUSIC) ;
    this->backMusicSource->loadAudioFile( MUSIC_ISLAND );
    this->backMusicSource->loadSettings( MUSIC_ISLAND );
    this->backMusicSource->setLooping( true );
    this->backMusicSource->play();
    this->backMusicSource->setAssociate( AudioSourceAssociate::MUSIC );
    
    this->embiantSource->setGain( MAX_VOLUME_EMBIENT_BIRDS );
    this->backMusicSource->setGain( MAX_VOLUME_EMBIENT_MUSIC );
    
    //location current area showing what music should play at the moment
    this->areaType = AREA_AIR_PLATFORMS;
    
    this->aSource = MANAGER_Audio->createStaticSource();
    if( this->aSource )
        this->aSource->setAssociate( AudioSourceAssociate::SOUND );
    
    //level index for difficulty and complete screen
    MANAGER_Memory->getTempData()->currentLevelIndex = 1;
}

//-----------------------------------------------------------------------------------------------------
void Level_1_1_part2::onExit()
{
    MANAGER_Level->stopLevelTimer();
    this->uploadLevelStates();
}

//-----------------------------------------------------------------------------------------------------
void Level_1_1_part2::release()
{
    MANAGER_Screen->removeAllScreens(GET_RUNNING_SCENE);
    delete []this->levelGameWorldPath;
    delete []this->levelGameObjectsPath;
    delete this;
}

//-----------------------------------------------------------------------------------------------------
void Level_1_1_part2::update(float _dt)
{
    if(pGameWorld && pGameCamera)
    {
        //following camera after player
        pGameCamera->moveCameraTo(pPlayer->getGameObject()->getPosition(), CAMERA_FOLLOW_SPEED);
    
        //player hp checking
        if( MANAGER_Memory->getTempData()->playerHP == 0 )
        {
            this->pHudScreen->hideSticker();
            MANAGER_Memory->getTempData()->playerHP--;
            GameObject* pObj = this->getGameObjectById( 777 );
            if( pObj )
            {
                graphic::GraphicAnimationObject* anim = dynamic_cast<graphic::GraphicAnimationObject*>(pObj->getGraphic());
                
                if( anim )
                    anim->setCallbackEndAnimation( graphic::sequence_mob::SM_DEATH, [this](){ this->onDefeat(); } );
            }
        }
        
        //run next level
        if( this->getLevelState( "go_next_level" ) == 1 )
        {
            this->setLevelState( "go_next_level", 0 );
            MANAGER_Level->replaceLevel( this->nextLevel );
        }
    }
}

//-----------------------------------------------------------------------------------------------------
bool Level_1_1_part2::onGetCoin(GameObject* _coin, std::string _key, PhysTriggerCoinKind _kind)
{
    //gold coins on each location
    if(_kind == COIN_GOLD_HP)
    {
        this->aSource->loadAudioFile( "audio/scenes/test/blueNotes/zolotayaNota.wav" );
        if( this->aSource )
        {
            this->aSource->loadSettings( "audio/scenes/test/blueNotes/zolotayaNota.wav" );
            this->aSource->play();
        }
    }
    
    return true;
}

//-----------------------------------------------------------------------------------------------------
void Level_1_1_part2::onPortal(GameObject *_portal)
{
    //falling in abyss
    if(_portal->getId() >= 950 && _portal->getId() <= 964)
    {
        this->pHudScreen->hideSticker();
        
        IAudioSource *aSource = MANAGER_Audio->createStaticSource(FALLING_SOUND);
        if(aSource)
        {
            aSource->play();
            aSource->setAssociate(AudioSourceAssociate::SOUND);
        }
        
        this->embiantSource->changeGainTo( MIN_VOLUME_EMBIENT_MUSIC, 2 );
        this->backMusicSource->changeGainTo( MIN_VOLUME_EMBIENT_MUSIC, 2 );
        
        auto defeatAction = Sequence::create( DelayTime::create( 2.0f ),
                                                CallFunc::create( CC_CALLBACK_0( Level_1_1_part2::onDefeat, this ) ), NULL );
        GET_RUNNING_SCENE->runAction( defeatAction );
        
        this->defeatState = true;
        
        _portal->release();
    }
    
    //Location triggers-portals
    if(_portal->getId() == 900)
    {
        this->pPlayer->stop();
        this->pHudScreen->resetSticker();
        this->pHudScreen->hideSticker();
        
        //level timer
        MANAGER_Level->stopLevelTimer();
        
        if( pPlayer->getGameObject()->getLocation() == game_object_location::GROUND )
        {
            MANAGER_Memory->getTempData()->setLevelState( "level_complete", 1 );
            
            GameObject* pObj = this->getGameObjectById( 777 );
            
            if( pObj )
            {
                graphic::GraphicAnimationObject* anim = dynamic_cast<graphic::GraphicAnimationObject*>(pObj->getGraphic());
                
                if( anim )
                {
                    anim->setCallbackEndAnimation( graphic::sequence_mob::SM_JOY, [this](){ this->onVictory(); } );
                }
            }
            
            this->pHudScreen->hideTimer();
            _portal->release();
        }
    }
}

//-----------------------------------------------------------------------------------------------------
void Level_1_1_part2::onVictory()
{
    dynamic_cast<HudScreen*>(MANAGER_Screen->getScreen(GET_RUNNING_SCENE, screen_id::HUD))->hideSticker();
    //this->setLevelState( "go_next_level", 1 );
    this->saveLevelStatData();
    
    this->saveLevelStatData();
    
    //ADDING CORRECT LOCATION RESTART
    MANAGER_Memory->getTempData()->locationCoinsIntoPoints = MANAGER_Memory->getTempData()->playerCoinsIntoPoints;
    
    MANAGER_Memory->getTempData()->vGetCoinIds.clear();
    MANAGER_Memory->getTempData()->vKilledMobsIds.clear();
    
    MANAGER_Memory->getTempData()->levelsProgressData[1]->gameLevelsProgress = level_state::LVL_STATE_COMPLETED;
    if( MANAGER_Memory->getTempData()->levelsProgressData[2]->gameLevelsProgress != level_state::LVL_STATE_COMPLETED )
        MANAGER_Memory->getTempData()->levelsProgressData[2]->gameLevelsProgress = level_state::LVL_STATE_OPENED;
    MANAGER_Memory->getTempData()->setLevelStars( 1 );
    
    MANAGER_Memory->uploadTempToPersistData();
    MANAGER_Memory->saveProfile();
    
    CompleteScreen* _pCompleteScreen = dynamic_cast< CompleteScreen* >( MANAGER_Screen->getScreen( GET_RUNNING_SCENE, screen_id::COMPLETE ) );
    if( _pCompleteScreen )
    {
        _pCompleteScreen->setCompletedLevel( MANAGER_Memory->getTempData()->currentLevelIndex );
        _pCompleteScreen->show();
    }
}

//-----------------------------------------------------------------------------------------------------
void Level_1_1_part2::onDefeat()
{
    dynamic_cast<HudScreen*>(MANAGER_Screen->getScreen(GET_RUNNING_SCENE, screen_id::HUD))->hideSticker();
    dynamic_cast<TryMoreScreen*>(MANAGER_Screen->getScreen(GET_RUNNING_SCENE, screen_id::TRY_MORE))->show();
}

//-----------------------------------------------------------------------------------------------------
void Level_1_1_part2::determinePlayerPosition()
{
    //Player first positioning
    MANAGER_Memory->getTempData()->playerPosition = Vec2( 10, 15.2 );
    
    if( this->getLevelState( "level_1_1_2_cp_1" ) == 1 )
        MANAGER_Memory->getTempData()->playerPosition = Vec2( 120, 16.2 );
    
    //set player position
    this->pPlayer->getGameObject()->setPosition( MANAGER_Memory->getTempData()->playerPosition );
    //add player gameobject to active obj list
    this->showGameObject( 777 );
    
    pGameCamera->setPosition(pPlayer->getGameObject()->getPosition());
}

//-----------------------------------------------------------------------------------------------------
void Level_1_1_part2::onCheckPoint( GameObject* _checkPoint )
{
    if( _checkPoint->getId() == 970 )
    {
        this->setLevelState( "level_1_1_2_cp_1", 1 );
        this->saveLevelStatData();
        
        _checkPoint->release();
    }
}

//-----------------------------------------------------------------------------------------------------
void Level_1_1_part2::saveLevelStatData()
{
    //save player data as check point data
    MANAGER_Memory->getTempData()->cpPlayerHP = MANAGER_Memory->getTempData()->playerHP;
    MANAGER_Memory->getTempData()->cpPlayerFireballAmmo = MANAGER_Memory->getTempData()->playerFireballAmmo;
    //saving coins for geting points on the end of level
    MANAGER_Memory->getTempData()->playerCoinsIntoPoints = this->localPlayerCoinsIntoPoints;
    //saving local coins state
    MANAGER_Memory->getTempData()->playerCoins = this->localPlayerCoins;
    MANAGER_Memory->getTempData()->vGetCoinIds = this->vLocalGetCoinIds;
    MANAGER_Memory->getTempData()->vKilledMobsIds = this->vLocalKilledMobsIds;
}

//-----------------------------------------------------------------------------------------------------