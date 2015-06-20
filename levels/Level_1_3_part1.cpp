//
//  Level_1_3_part1.cpp
//  MusicalAdventure
//
//  Created by madmin on 6/4/14.
//  Copyright (c) 2014 ampagames. All rights reserved.
//

#include "Level_1_3_part1.h"

#include "GameScene.h"

#include "HudScreen.h"

#include "GameObject.h"

#include "CompleteScreen.h"
#include "TryMoreScreen.h"
#include "DialogScreen.h"

#include "GraphicAnimationObject.h"

#include "L1_3_Quest_1_MGScreen.h"

using namespace cocos2d;
using namespace std;

//-----------------------------------------------------------------------------------------------------
Level_1_3_part1::Level_1_3_part1()
{
    level = level_id::LVL_1_3_PART1;
    nextLevel = level_id::LVL_1_BOSS;
    
    
    this->levelGameWorldPath = new char[50];
    sprintf(levelGameWorldPath, "%s%s", PHYS_LANDSCAPES_PATH , "landscape1_3_1.xml");
    this->levelGameObjectsPath = new char[50];
    sprintf(levelGameObjectsPath, "%s%s", PHYS_OBJECTS_PATH , "objects1_3_1.xml");
}

//-----------------------------------------------------------------------------------------------------
Level_1_3_part1::~Level_1_3_part1()
{
}

//-----------------------------------------------------------------------------------------------------
Level_1_3_part1* Level_1_3_part1::create()
{
    Level_1_3_part1* pLevel_1_3_part1 = new Level_1_3_part1();
    if(pLevel_1_3_part1)
    {
        return pLevel_1_3_part1;
    }
    else
    {
        delete pLevel_1_3_part1;
        return NULL;
    }
}

//-----------------------------------------------------------------------------------------------------
bool Level_1_3_part1::init()
{
    //set using screens
    this->pDialogScreen = dynamic_cast< DialogScreen* > ( MANAGER_Screen->getScreen( GET_RUNNING_SCENE, screen_id::DIALOG ) );
    this->pHudScreen = dynamic_cast< HudScreen* >( MANAGER_Screen->getScreen( GET_RUNNING_SCENE, screen_id::HUD) );
    
    this->aSource = MANAGER_Audio->createStaticSource();
    if( this->aSource )
        this->aSource->setAssociate( AudioSourceAssociate::SOUND );
    
    //Logic on enter level
    if( this->getLevelState( "musicbox_1_level_1_3_1_complete" ) == 1 )
    {
        this->hideGameObject( 900 );
        this->showGameObject( 923 );
    }
    //
    
    //removing taken coins
    vector< int > coinsToRemove = vLocalGetCoinIds;
    for( int i = 0 ; i < coinsToRemove.size() ; i++ )
        this->hideGameObject( coinsToRemove[i] );
    //removing killed mobs
    vector< int > mobsToRemove = vLocalKilledMobsIds;
    for( int i = 0 ; i < mobsToRemove.size() ; i++ )
        this->hideGameObject( mobsToRemove[i] );
    
    return true;
}

//-----------------------------------------------------------------------------------------------------
void Level_1_3_part1::onEnter()
{
    //level timer
    MANAGER_Level->resumeLevelTimer();


    
    //Player positioning
    //this->determinePlayerPosition();
    
    //on restart level from pause/defeat/win level loading
    this->restartLevel = this->level;
    
    //Add back music
    this->embiantSource->loadAudioFile( EMBIENT_BIRDS );
    this->embiantSource->loadSettings( EMBIENT_BIRDS );
    this->embiantSource->setLooping( true );
    this->embiantSource->play();
    this->embiantSource->setAssociate( AudioSourceAssociate::MUSIC );
    this->backMusicSource->loadAudioFile( MUSIC_LVL1 );
    this->backMusicSource->loadSettings( MUSIC_LVL1 );
    this->backMusicSource->setLooping( true );
    this->backMusicSource->play();
    this->backMusicSource->setAssociate( AudioSourceAssociate::MUSIC );
    
    this->embiantSource->setGain( MAX_VOLUME_EMBIENT_BIRDS );
    this->backMusicSource->setGain( MAX_VOLUME_EMBIENT_MUSIC );
    
   
    
    //location current area showing what music should play at the moment
    this->areaType = AREA_AIR_PLATFORMS;
    
    
    
    //Camera positionig according to player
    //pGameCamera->setPosition( pPlayer->getGameObject()->getPosition() );
    
    //level index for difficulty and complete screen
    MANAGER_Memory->getTempData()->currentLevelIndex = 5;
}

//-----------------------------------------------------------------------------------------------------
void Level_1_3_part1::onExit()
{
    MANAGER_Level->stopLevelTimer();
    this->uploadLevelStates();
}

//-----------------------------------------------------------------------------------------------------
void Level_1_3_part1::release()
{
    MANAGER_Screen->removeAllScreens(GET_RUNNING_SCENE);
    delete []this->levelGameWorldPath;
    delete []this->levelGameObjectsPath;
    delete this;
}

//-----------------------------------------------------------------------------------------------------
void Level_1_3_part1::update(float _dt)
{
    //Game camera following the player
    if(pGameWorld && pGameCamera)
    {
        //Following camera after player
        pGameCamera->moveCameraTo(pPlayer->getGameObject()->getPosition(), CAMERA_FOLLOW_SPEED);
        
        graphic::GraphicAnimationObject* waterfallAnimR1 = dynamic_cast<graphic::GraphicAnimationObject*>(this->getGameObjectById(6700)->getGraphic());
        waterfallAnimR1->setScale( Vec2( 1.5, 1.5) );
        
        graphic::GraphicAnimationObject* waterfallAnimR2 = dynamic_cast<graphic::GraphicAnimationObject*>(this->getGameObjectById(6701)->getGraphic());
        waterfallAnimR2->setScale( Vec2( 1.5, 1.5) );
        
        graphic::GraphicAnimationObject* waterfallAnimR3 = dynamic_cast<graphic::GraphicAnimationObject*>(this->getGameObjectById(6702)->getGraphic());
        waterfallAnimR3->setScale( Vec2( 1.5, 1.5) );
        
        graphic::GraphicAnimationObject* waterfallAnimR4 = dynamic_cast<graphic::GraphicAnimationObject*>(this->getGameObjectById(6705)->getGraphic());
        waterfallAnimR4->setScale( Vec2(-1.5, 1.5) );
        
        graphic::GraphicAnimationObject* waterfallAnimL2 = dynamic_cast<graphic::GraphicAnimationObject*>(this->getGameObjectById(6704)->getGraphic());
        waterfallAnimL2->setScale( Vec2( -1.5, 1.79) );
        
        //player hp checking
        if( MANAGER_Memory->getTempData()->playerHP == 0 )
        {
            MANAGER_Memory->getTempData()->playerHP--;
            GameObject* pObj = this->getGameObjectById( 777 );
            if( pObj )
            {
                graphic::GraphicAnimationObject* anim = dynamic_cast<graphic::GraphicAnimationObject*>(pObj->getGraphic());
                
                if( anim )
                    anim->setCallbackEndAnimation( graphic::sequence_mob::SM_DEATH, [this](){ this->onDefeat(); } );
            }
        }
    }
}

//-----------------------------------------------------------------------------------------------------
bool Level_1_3_part1::onGetCoin(GameObject* _coin, std::string _key, PhysTriggerCoinKind _kind)
{
    if( _kind == COIN_GOLD_HP )
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
void Level_1_3_part1::onPortal(GameObject *_portal)
{
    if(pGameWorld && pGameCamera)
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
            
            MANAGER_Level->getCurrentLevel()->getEmbiantSource()->changeGainTo(0.0f, 2);
            MANAGER_Level->getCurrentLevel()->getBackMusicSource()->changeGainTo(0.0f, 2);
            
            auto defeatAction = Sequence::create( DelayTime::create( 2.0f ),
                                                  CallFunc::create( CC_CALLBACK_0( Level_1_3_part1::onDefeat, this ) ), nullptr );
            GET_RUNNING_SCENE->runAction( defeatAction );
            
            _portal->release();
        }
        
        //Location triggers-portals
        if( _portal->getId() == 900 )
        {
            //level timer
            MANAGER_Level->stopLevelTimer();
            this->pHudScreen->hideTimer();
            
            this->saveLevelStatData();
            MANAGER_Level->replaceLevel( level_id::MG_1_3_1_MUSICBOX_1 );
        }
        if( _portal->getId() == 901 )
        {
            //level timer
            MANAGER_Level->stopLevelTimer();
            
            this->pHudScreen->hideSticker();
            this->pPlayer->stop();
            if( pPlayer->getGameObject()->getLocation() == game_object_location::GROUND )
            {
                MANAGER_Memory->getTempData()->setLevelState( "level_complete", 1 );
                
                auto winAction = Sequence::create( CCDelayTime::create( 2.0f ),
                                                   CCCallFunc::create( CC_CALLBACK_0( Level_1_3_part1::onVictory, this ) ), nullptr );
                GET_RUNNING_SCENE->runAction( winAction );
                
                this->pHudScreen->hideTimer();
                _portal->release();
            }
        }
        
    }
}

//-----------------------------------------------------------------------------------------------------
void Level_1_3_part1::onVictory()
{
    this->pHudScreen->hideSticker();
    this->saveLevelStatData();
    
    //ADDING CORRECT LOCATION RESTART
    MANAGER_Memory->getTempData()->locationCoinsIntoPoints = MANAGER_Memory->getTempData()->playerCoinsIntoPoints;
    
    MANAGER_Memory->getTempData()->vGetCoinIds.clear();
    MANAGER_Memory->getTempData()->vKilledMobsIds.clear();
    
    MANAGER_Memory->getTempData()->levelsProgressData[5]->gameLevelsProgress = level_state::LVL_STATE_COMPLETED;
    if( MANAGER_Memory->getTempData()->levelsProgressData[6]->gameLevelsProgress != level_state::LVL_STATE_COMPLETED )
        MANAGER_Memory->getTempData()->levelsProgressData[6]->gameLevelsProgress = level_state::LVL_STATE_DONATE_LOCK;
    MANAGER_Memory->getTempData()->setLevelStars( 5 );
    
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
void Level_1_3_part1::onDefeat()
{
    dynamic_cast<HudScreen*>(MANAGER_Screen->getScreen(GET_RUNNING_SCENE, screen_id::HUD))->hideSticker();
    dynamic_cast<TryMoreScreen*>(MANAGER_Screen->getScreen(GET_RUNNING_SCENE, screen_id::TRY_MORE))->show();
}

//-----------------------------------------------------------------------------------------------------
void Level_1_3_part1::determinePlayerPosition()
{
    //player first positioning
    MANAGER_Memory->getTempData()->playerPosition = Vec2( 5, 32.7 );
    
    //player second positioning ( after restart level from trymore screen )
    if( this->getLevelState( "musicbox_1_level_1_3_1_complete" ) == 1 )
        MANAGER_Memory->getTempData()->playerPosition = Vec2( 40, 9.1 );
    if( this->getLevelState( "musicbox_1_level_1_3_1_complete" ) == 2 )
        MANAGER_Memory->getTempData()->playerPosition = Vec2( 40, 9.1 );
    //set player position
    this->pPlayer->getGameObject()->setPosition( MANAGER_Memory->getTempData()->playerPosition );
    
    //add player gameobject to active obj list
    this->showGameObject( 777 );
    
    //Camera positionig according to player
    pGameCamera->setPosition( pPlayer->getGameObject()->getPosition() );
}

//-----------------------------------------------------------------------------------------------------
void Level_1_3_part1::onCheckPoint( GameObject* _checkPoint )
{
    
}

//-----------------------------------------------------------------------------------------------------
void Level_1_3_part1::saveLevelStatData()
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