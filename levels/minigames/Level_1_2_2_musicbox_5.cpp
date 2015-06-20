//
//  Level_1_2_2_musicbox_5.cpp
//  MusicalAdventure
//
//  Created by ampa_games on 9/15/14.
//  Copyright (c) 2014 ampagames. All rights reserved.
//

#include "Level_1_2_2_musicbox_5.h"

#include "GraphicAnimationObject.h"

#include "HudScreen.h"

#include "GameObject.h"

#include "CompleteScreen.h"
#include "TryMoreScreen.h"
#include "DialogScreen.h"

#include "HintDurationSumScreen.h"

using namespace cocos2d;

//-----------------------------------------------------------------------------------------------------
Level_1_2_2_musicbox_5::Level_1_2_2_musicbox_5() :
aSource( nullptr ),
defeatAction( nullptr ),
winAction( nullptr )
{
    level = level_id::MG_1_2_2_MUSICBOX_5;
    
    this->levelGameWorldPath = new char[50];
    sprintf(levelGameWorldPath, "%s%s", PHYS_LANDSCAPES_PATH, "landscape1_2_musicbox_5.xml");
    this->levelGameObjectsPath = new char[50];
    sprintf(levelGameObjectsPath, "%s%s", PHYS_OBJECTS_PATH, "objects1_2_musicbox_5.xml");
}

//-----------------------------------------------------------------------------------------------------
Level_1_2_2_musicbox_5::~Level_1_2_2_musicbox_5()
{
    
}

//-----------------------------------------------------------------------------------------------------
Level_1_2_2_musicbox_5* Level_1_2_2_musicbox_5::create()
{
    Level_1_2_2_musicbox_5* pLevel_1_2_2_musicbox_5 = new Level_1_2_2_musicbox_5();
    if(pLevel_1_2_2_musicbox_5)
    {
        return pLevel_1_2_2_musicbox_5;
    }
    else
    {
        delete pLevel_1_2_2_musicbox_5;
        return NULL;
    }
}

//-----------------------------------------------------------------------------------------------------
bool Level_1_2_2_musicbox_5::init()
{
    //set using screens
    this->pDialogScreen = dynamic_cast< DialogScreen* > ( MANAGER_Screen->getScreen( GET_RUNNING_SCENE, screen_id::DIALOG ) );
    this->pHudScreen = dynamic_cast< HudScreen* >( MANAGER_Screen->getScreen( GET_RUNNING_SCENE, screen_id::HUD) );

    
    return true;
}

//-----------------------------------------------------------------------------------------------------
void Level_1_2_2_musicbox_5::onEnter()
{

    this->pHudScreen->hideSticker();
    
    //Player positioning
    //this->determinePlayerPosition();
    
    //on restart level from pause/defeat/win level loading
    this->restartLevel = level_id::LVL_1_2_PART2;
    
    //add sounds
    this->aSource = MANAGER_Audio->createStaticSource( "audio/scenes/test/blueNotes/zolotayaNota.wav" );
    if( this->aSource )
        this->aSource->setAssociate( AudioSourceAssociate::SOUND );
    
    this->embiantSource->setGain( MIN_VOLUME_EMBIENT_MUSIC );
    this->backMusicSource->setGain( MIN_VOLUME_EMBIENT_MUSIC );
    
    //exit musicbox animation
    graphic::GraphicAnimationObject* crystal = dynamic_cast<graphic::GraphicAnimationObject*>(this->getGameObjectById(900)->getGraphic());
    if( crystal )
        crystal->setScale( Vec2(1.5, 1.5) );
    
    //Logic on enter level level
    this->pDialogScreen->loadDialogFromXml(44, 0, 0);
    if( this->getLevelState( "atfirst_musicbox_3_level_1_2_2" ) != 1 )
    {
        //complete stat
        MANAGER_Memory->getTempData()->statSecretPlaces += 1;
        this->setLevelState( "atfirst_musicbox_3_level_1_2_2", 1 );
    }
    //
    //MANAGER_Memory->getTempData()->setLevelState( "MUSIC_BOX_RESTART", 1 );
    
    // Camera positioning
    //this->pGameCamera->setPosition( Vec2( 0, 0 ) );
}

//-----------------------------------------------------------------------------------------------------
void Level_1_2_2_musicbox_5::onExit()
{
    if( MANAGER_Memory->getTempData()->getLevelState( "SINGLE_MINIGAME_RESTART" ) == 0 )
        this->setLevelState( "SINGLE_MINIGAME", 0 );
    else
        MANAGER_Memory->getTempData()->setLevelState( "SINGLE_MINIGAME_RESTART", 1 );
    
    MANAGER_Memory->getTempData()->setLevelState( "MUSIC_BOX_RESTART", 0 );
    MANAGER_Level->stopLevelTimer();
    this->uploadLevelStates();
}

//-----------------------------------------------------------------------------------------------------
void Level_1_2_2_musicbox_5::release()
{
    MANAGER_Screen->removeAllScreens(GET_RUNNING_SCENE);
    delete []this->levelGameWorldPath;
    delete []this->levelGameObjectsPath;
    delete this;
}

//-----------------------------------------------------------------------------------------------------
void Level_1_2_2_musicbox_5::update(float _dt)
{
    //Game camera following the player
    //pGameCamera->setPosition(pPlayer->getGameObject()->getPosition());
    if(pGameWorld && pGameCamera)
    {
        if( this->getLevelState( "mbox_3_go_to_1_2_2" ) == 1 )
        {
            this->setLevelState( "mbox_3_go_to_1_2_2", 0 );
            MANAGER_Level->replaceLevel( level_id::LVL_1_2_PART2);
        }
    }
}

//-----------------------------------------------------------------------------------------------------
bool Level_1_2_2_musicbox_5::onGetCoin(GameObject* _coin, std::string _key, PhysTriggerCoinKind _kind)
{
    //gold coins on each location
    if( _kind == COIN_GOLD_HP )
    {
        if( this->aSource )
            this->aSource->play();
    }
    return true;
}

//-----------------------------------------------------------------------------------------------------
void Level_1_2_2_musicbox_5::onPortal(GameObject *_portal)
{
    if(pGameWorld && pGameCamera)
    {
        if(_portal->getId() == 900)
        {
            this->pHudScreen->hideSticker();
            this->pPlayer->stop();
            
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
                
                _portal->release();
            }
        }
    }
}

//-----------------------------------------------------------------------------------------------------
void Level_1_2_2_musicbox_5::onDialogueEnd( int _id )
{
    if( _id == 44 )
        MANAGER_Screen->addScreen(GET_RUNNING_SCENE, screen_id::MG_L1_2_MUSICBOX_5);
}

//-----------------------------------------------------------------------------------------------------
void Level_1_2_2_musicbox_5::onMinigameEnd(bool _isWin, screen_id _screen)
{
    if( _isWin )
    {
        this->pHudScreen->showSticker();
        MANAGER_Screen->removeScreen(GET_RUNNING_SCENE, _screen);
        
        this->getGameObjectById(20)->setActivity(true);
        this->pGameWorld->addGameObjectToActive(this->getGameObjectById(20));
        this->getGameObjectById(21)->setActivity(true);
        this->pGameWorld->addGameObjectToActive(this->getGameObjectById(21));
        this->getGameObjectById(22)->setActivity(true);
        this->pGameWorld->addGameObjectToActive(this->getGameObjectById(22));
        this->getGameObjectById(23)->setActivity(true);
        this->pGameWorld->addGameObjectToActive(this->getGameObjectById(23));
        this->getGameObjectById(26)->setActivity(true);
        this->pGameWorld->addGameObjectToActive(this->getGameObjectById(26));
        this->getGameObjectById(900)->setActivity(true);
        this->pGameWorld->addGameObjectToActive(this->getGameObjectById(900));
        
        this->getGameObjectById(1200)->setActivity(true);
        this->pGameWorld->addGameObjectToActive(this->getGameObjectById(1200));
        
        this->getGameObjectById(4500)->setActivity(true);
        this->pGameWorld->addGameObjectToActive(this->getGameObjectById(4500));
    }
    else
    {
        this->pHudScreen->hideSticker();
        this->pPlayer->stop();
        
        this->defeatAction = Sequence::create( DelayTime::create( 2.0f ),
                                               CallFunc::create( CC_CALLBACK_0( Level_1_2_2_musicbox_5::onDefeat, this ) ), nullptr );
        GET_RUNNING_SCENE->runAction( this->defeatAction );
        
    }
}

//-----------------------------------------------------------------------------------------------------
void Level_1_2_2_musicbox_5::onVictory()
{
    if( this->getLevelState( "SINGLE_MINIGAME" ) == 0 )
    {
        this->setLevelState( "mbox_3_go_to_1_2_2", 1 );
        this->setLevelState( "musicbox_3_level_1_2_2_complete", 1 );
        this->saveLevelStatData();
    }
    else
    {
        MANAGER_Scene->replaceScene( scene_id::MINIGAMES_SELECTION );
    }
}

//-----------------------------------------------------------------------------------------------------
void Level_1_2_2_musicbox_5::onDefeat()
{
    if( this->getLevelState( "SINGLE_MINIGAME" ) == 0 )
    {
        this->setLevelState( "mbox_3_go_to_1_2_2", 1 );
        this->setLevelState( "musicbox_3_level_1_2_2_complete", 2 );
        this->saveLevelStatData();
    }
    else
    {
        MANAGER_Scene->replaceScene( scene_id::MINIGAMES_SELECTION );
    }
}

//-----------------------------------------------------------------------------------------------------
void Level_1_2_2_musicbox_5::determinePlayerPosition()
{
    //player first positioning
    MANAGER_Memory->getTempData()->playerPosition = Vec2( 22, 8 );
    
    //set player position
    this->pPlayer->getGameObject()->setPosition( MANAGER_Memory->getTempData()->playerPosition );
    
    // Camera positioning
    this->pGameCamera->setPosition( Vec2( 0, 0 ) );
}

//-----------------------------------------------------------------------------------------------------
void Level_1_2_2_musicbox_5::saveLevelStatData()
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