//
//  Level_1_1_part3.cpp
//  MusicalAdventure
//
//  Created by madmin on 6/4/14.
//  Copyright (c) 2014 ampagames. All rights reserved.
//

#include "Level_1_1_part3.h"

#include "GameScene.h"

#include "HudScreen.h"

#include "GameObject.h"

#include "L1_1_Quest_3_MGScreen.h"
#include "L1_1_Quest_4_MGScreen.h"
#include "L1_1_Quest_5_MGScreen.h"

#include "TestVoiceScreen.h"

#include "CompleteScreen.h"
#include "TryMoreScreen.h"
#include "DialogScreen.h"

#include "GraphicAnimationObject.h"

using namespace std;

//-----------------------------------------------------------------------------------------------------
Level_1_1_part3::Level_1_1_part3() :
cameraFollowPlayer( true ),
cameraFollowPlayerY( false ),
cameraAutoReplace( false),
cameraAutoReplaceLeft( false ),
cameraAutoReplaceRight( false )
{
    level = level_id::LVL_1_1_PART3;
    nextLevel = level_id::LVL_1_2_PART1;
    
    this->levelGameWorldPath = new char[50];
    sprintf(levelGameWorldPath, "%s%s", PHYS_LANDSCAPES_PATH , "landscape1_1_3.xml");
    this->levelGameObjectsPath = new char[50];
    sprintf(levelGameObjectsPath, "%s%s", PHYS_OBJECTS_PATH , "objects1_1_3.xml");
    
}
//-----------------------------------------------------------------------------------------------------
Level_1_1_part3::~Level_1_1_part3()
{
}

//-----------------------------------------------------------------------------------------------------
Level_1_1_part3* Level_1_1_part3::create()
{
    Level_1_1_part3* pLevel_1_1_part3 = new Level_1_1_part3();
    if(pLevel_1_1_part3)
    {
        return pLevel_1_1_part3;
    }
    else
    {
        delete pLevel_1_1_part3;
        return NULL;
    }
}

//-----------------------------------------------------------------------------------------------------
bool Level_1_1_part3::init()
{
    //set using screens
    this->pDialogScreen = dynamic_cast< DialogScreen* > ( MANAGER_Screen->getScreen( GET_RUNNING_SCENE, screen_id::DIALOG ) );
    this->pHudScreen = dynamic_cast< HudScreen* >( MANAGER_Screen->getScreen( GET_RUNNING_SCENE, screen_id::HUD) );
    
    this->aSource = MANAGER_Audio->createStaticSource();
    if( this->aSource )
        this->aSource->setAssociate( AudioSourceAssociate::SOUND );
    //
    
    //minigames screens
    MANAGER_Screen->addScreen( GET_RUNNING_SCENE, screen_id::MG_L1_1_QUEST_3 );
    MANAGER_Screen->detachScreen( GET_RUNNING_SCENE, screen_id::MG_L1_1_QUEST_3 );
    IScreen* mgScreen1 = MANAGER_Screen->getScreen( GET_RUNNING_SCENE, screen_id::MG_L1_1_QUEST_3 );
    mgScreen1->setPosition( 320, 800 );
    mgScreen1->setScale( this->pGameCamera->getDistance() );
    dynamic_cast< GameScene* >( GET_RUNNING_SCENE )->getForegroundLayer()->addChild( mgScreen1, 10 );
    L1_1_Quest_3_MGScreen* L1_3_Screen = dynamic_cast< L1_1_Quest_3_MGScreen* >( mgScreen1 );
    L1_3_Screen->hideMinigameInterface();
    //
    MANAGER_Screen->addScreen( GET_RUNNING_SCENE, screen_id::MG_L1_1_QUEST_4 );
    MANAGER_Screen->detachScreen( GET_RUNNING_SCENE, screen_id::MG_L1_1_QUEST_4 );
    IScreen* mgScreen2 = MANAGER_Screen->getScreen( GET_RUNNING_SCENE, screen_id::MG_L1_1_QUEST_4 );
    mgScreen2->setPosition( 10900, 260 );
    mgScreen2->setScale( this->pGameCamera->getDistance() );
    dynamic_cast < GameScene* >( GET_RUNNING_SCENE )->getForegroundLayer()->addChild( mgScreen2, 10);
    L1_1_Quest_4_MGScreen* L1_4_Screen = dynamic_cast< L1_1_Quest_4_MGScreen* >( mgScreen2 );
    L1_4_Screen->hideMinigameInterface();
    //
    MANAGER_Screen->loadScreen( GET_RUNNING_SCENE, screen_id::MG_L1_1_QUEST_5 );
    //MANAGER_Screen->detachScreen( GET_RUNNING_SCENE, screen_id::MG_L1_1_QUEST_5 );
    IScreen* mgScreen3 = MANAGER_Screen->getScreen( GET_RUNNING_SCENE, screen_id::MG_L1_1_QUEST_5 );
    mgScreen3->setPosition( 12050, 700 );
    mgScreen3->setScale( this->pGameCamera->getDistance() );
    dynamic_cast< GameScene* >( GET_RUNNING_SCENE )->getForegroundLayer()->addChild( mgScreen3, 10);
    L1_1_Quest_5_MGScreen* L1_5_Screen = dynamic_cast< L1_1_Quest_5_MGScreen* >(MANAGER_Screen->getScreen( GET_RUNNING_SCENE, screen_id::MG_L1_1_QUEST_5 ) );
    L1_5_Screen->hideMinigameInterface();
    //
    
    //logic on enter level
    // switch boxes position into win state
    if( this->getLevelState( "mg_1_level_1_1_3_complete" ) == 1 )
        dynamic_cast< L1_1_Quest_3_MGScreen* >( MANAGER_Screen->getScreen( GET_RUNNING_SCENE, screen_id::MG_L1_1_QUEST_3 ) )->setWinPos( true );
    
    //removing taken coins
    vector< int > coinsToRemove = vLocalGetCoinIds;
    for( int i = 0 ; i < coinsToRemove.size() ; i++ )
        this->hideGameObject( coinsToRemove[i] );
    //removing killed mobs
    vector< int > mobsToRemove = vLocalKilledMobsIds;
    for( int i = 0 ; i < mobsToRemove.size() ; i++ )
        this->hideGameObject( mobsToRemove[i] );
    
    //set logic if started single minigame
    if( this->getLevelState( "SINGLE_MINIGAME" ) == 1)
        this->singleMinigameLogic();
    
    
    return true;
}

//-----------------------------------------------------------------------------------------------------
void Level_1_1_part3::onEnter()
{
    //level timer
    MANAGER_Level->resumeLevelTimer();
    
    //
    
    //location current area showing what music should play at the moment
    //this->areaType = AREA_CAVE;
    
    //on restart level from pause/defeat/win level loading
    this->restartLevel = this->level;
    
    //Add back music
    this->embiantSource->loadAudioFile( this->areaType == AREA_AIR_PLATFORMS ? EMBIENT_BIRDS : EMBIENT_DWARFS );
    this->embiantSource->loadSettings( this->areaType == AREA_AIR_PLATFORMS ? EMBIENT_BIRDS : EMBIENT_DWARFS );
    this->embiantSource->setLooping( true );
    this->embiantSource->play();
    this->embiantSource->setAssociate( AudioSourceAssociate::MUSIC );
    this->backMusicSource->loadAudioFile( this->areaType == AREA_AIR_PLATFORMS ? MUSIC_ISLAND : MUSIC_CAVE );
    this->backMusicSource->loadSettings( this->areaType == AREA_AIR_PLATFORMS ? MUSIC_ISLAND : MUSIC_CAVE );
    this->backMusicSource->setLooping( true );
    this->backMusicSource->play();
    this->backMusicSource->setAssociate( AudioSourceAssociate::MUSIC );

    this->embiantSource->setGain( MAX_VOLUME_EMBIENT_MUSIC );
    this->backMusicSource->setGain( MAX_VOLUME_EMBIENT_MUSIC );
    
    //Camera positionig according to player
    //if( this->cameraFollowPlayer )
        //pGameCamera->setPosition( pPlayer->getGameObject()->getPosition() );
    
    
    
    //level index for difficulty and complete screen
    MANAGER_Memory->getTempData()->currentLevelIndex = 2;
}

//-----------------------------------------------------------------------------------------------------
void Level_1_1_part3::onExit()
{
    if( MANAGER_Memory->getTempData()->getLevelState( "SINGLE_MINIGAME_RESTART" ) == 0 )
    {
        this->setLevelState( "SINGLE_MINIGAME", 0 );
        this->setLevelState( "sm_level_1_1_3", 0 );
    }
    else
        MANAGER_Memory->getTempData()->setLevelState( "SINGLE_MINIGAME_RESTART", 1 );
    
    MANAGER_Level->stopLevelTimer();
    
    this->uploadLevelStates();
}

//-----------------------------------------------------------------------------------------------------
void Level_1_1_part3::release()
{
    MANAGER_Screen->removeAllScreens(GET_RUNNING_SCENE);
    delete []this->levelGameWorldPath;
    delete []this->levelGameObjectsPath;
    delete this;
}

//-----------------------------------------------------------------------------------------------------
void Level_1_1_part3::update(float _dt)
{
    if(pGameWorld && pGameCamera)
    {
        //Following camera after player
        if( this->cameraFollowPlayerY )
            if( this->getLevelState( "SINGLE_MINIGAME" ) == 0 )
                pGameCamera->moveCameraTo( Vec2( pGameCamera->getPositionX(), pPlayer->getGameObject()->getPosition().y ), CAMERA_FOLLOW_SPEED );
        
        if( this->cameraFollowPlayer && !this->cameraFollowPlayerY )
            if( this->getLevelState( "SINGLE_MINIGAME" ) == 0 )
                pGameCamera->moveCameraTo( pPlayer->getGameObject()->getPosition(), CAMERA_FOLLOW_SPEED );
        
        this->cameraFollowPlayerY = false;
        
        if( this->cameraAutoReplaceLeft )
            this->pPlayer->moveLeft();
        if( this->cameraAutoReplaceRight )
            this->pPlayer->moveRight();
        
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
    }
}

//-----------------------------------------------------------------------------------------------------
bool Level_1_1_part3::onGetCoin(GameObject* _coin, std::string _key, PhysTriggerCoinKind _kind)
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
void Level_1_1_part3::onPortal(GameObject *_portal)
{
    if( pGameWorld && pGameCamera && this->getLevelState( "SINGLE_MINIGAME" ) != 1 )
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
                                                    CallFunc::create( CC_CALLBACK_0( Level_1_1_part3::onDefeat, this ) ), nullptr );
            GET_RUNNING_SCENE->runAction( defeatAction );
            
            _portal->release();
        }
        
        //Location triggers-portals
        if( _portal->getId() == 900 && this->getLevelState( "mg_1_level_1_1_3_complete" ) != 1 )
        {
            MANAGER_Level->stopLevelTimer();
            this->pHudScreen->hideTimer();
            
            this->pDialogScreen->loadDialogFromXml( 20 );
            this->pHudScreen->hideSticker();
            _portal->release();
        }
        if( _portal->getId() == 901 && this->getLevelState( "mg_2_level_1_1_3_complete" ) != 1 )
        {
            MANAGER_Level->stopLevelTimer();
            this->pHudScreen->hideTimer();
            
            this->pDialogScreen->loadDialogFromXml( 23 );
            this->pHudScreen->hideSticker();
            //positioning camera for minigame
            this->cameraFollowPlayer = false;
            pGameCamera->moveCameraTo( Vec2(356.0f, 13.0f), 0.01f, true );
            
            _portal->release();
        }
        if( _portal->getId() == 903 && this->getLevelState( "mg_3_level_1_1_3_complete" ) != 1 )
        {
            this->embiantSource->changeGainTo( MIN_VOLUME_EMBIENT_MUSIC, 2 );
            this->backMusicSource->changeGainTo( MIN_VOLUME_EMBIENT_MUSIC, 2 );
            MANAGER_Level->stopLevelTimer();
            this->pHudScreen->hideTimer();
            
            this->pDialogScreen->loadDialogFromXml( 29 );
            this->pHudScreen->hideSticker();
            //positioning camera for minigame
            this->cameraFollowPlayer = false;
            pGameCamera->moveCameraTo( Vec2(392.0f, 37.0f), 0.01f, true );
            
            _portal->release();
        }
        //test voice screen added
        if( _portal->getId() == 904 && MANAGER_Memory->getTempData()->playerVoice == voice::VOICE_UNDEFINED )
        {
            this->embiantSource->changeGainTo( MIN_VOLUME_EMBIENT_MUSIC, 2 );
            this->backMusicSource->changeGainTo( MIN_VOLUME_EMBIENT_MUSIC, 2 );
            MANAGER_Screen->addScreen( GET_RUNNING_SCENE, screen_id::TEST_VOICE );
            if( dynamic_cast< TestVoiceScreen* >( MANAGER_Screen->getScreen( GET_RUNNING_SCENE, screen_id::TEST_VOICE ) ) )
            {
                TestVoiceScreen* pTestVoiceScreen = dynamic_cast< TestVoiceScreen* >( MANAGER_Screen->getScreen( GET_RUNNING_SCENE, screen_id::TEST_VOICE ) );
                pTestVoiceScreen->setCallbackTestCompleted( CC_CALLBACK_1( Level_1_1_part3::removeTestVoiceScreen, this ) );
            }
            
            this->pHudScreen->hideSticker();
            this->pHudScreen->hideTimer();
            MANAGER_Level->stopLevelTimer();
            
            _portal->release();
        }
        if( _portal->getId() == 902 )
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
        
        //pre/post camera autoreplace
        if( _portal->getId() == 6005 )
        {
            if( this->pPlayer->getGameObject()->getPosition().x < 120 ||
                this->pPlayer->getGameObject()->getPosition().x > 140 )
            {
                this->cameraFollowPlayer = true;
                this->cameraAutoReplace = false;
            }
            else
            {
                this->cameraFollowPlayer = false;
                this->cameraAutoReplace = true;
            }
            
            this->cameraFollowPlayerY = true;
            if( !this->cameraAutoReplace )
            {
                if( this->areaType == AREA_CAVE )
                    this->pGameCamera->moveCameraTo( Vec2( 104.0f, this->pPlayer->getGameObject()->getPosition().y ), CAMERA_FOLLOW_SPEED , true );//this->pGameCamera->setPositionX( 104.0f );
                if( this->areaType == AREA_AIR_PLATFORMS )
                    this->pGameCamera->moveCameraTo( Vec2( 151.0f, this->pPlayer->getGameObject()->getPosition().y ), CAMERA_FOLLOW_SPEED , true );//this->pGameCamera->setPositionX( 151.0f );
            }
        }
        if( _portal->getId() == 6006 )
        {
            if( this->pPlayer->getGameObject()->getPosition().x < 315 ||
                this->pPlayer->getGameObject()->getPosition().x > 331 )
            {
                this->cameraFollowPlayer = true;
                this->cameraAutoReplace = false;
            }
            else
            {
                this->cameraFollowPlayer = false;
                this->cameraAutoReplace = true;
            }
            
            this->cameraFollowPlayerY = true;
            if( !this->cameraAutoReplace )
            {
                if( this->areaType == AREA_AIR_PLATFORMS )
                    this->pGameCamera->moveCameraTo( Vec2( 299.0f, this->pPlayer->getGameObject()->getPosition().y ), CAMERA_FOLLOW_SPEED , true );//this->pGameCamera->setPositionX( 299.0f );
                if( this->areaType == AREA_CAVE )
                    this->pGameCamera->moveCameraTo( Vec2( 346.0f, this->pPlayer->getGameObject()->getPosition().y ), CAMERA_FOLLOW_SPEED , true );//this->pGameCamera->setPositionX( 346.0f );
            }
        }
    }
}

//-----------------------------------------------------------------------------------------------------
void Level_1_1_part3::onMinigameEnd(bool _isWin, screen_id _screen)
{
    //if minigame ended as single
    if( this->getLevelState( "SINGLE_MINIGAME" ) == 1 )
    {
        GameObject* pObj = this->getGameObjectById( 777 );
        if( pObj )
        {
            graphic::GraphicAnimationObject* anim = dynamic_cast<graphic::GraphicAnimationObject*>(pObj->getGraphic());
            
            if( anim )
            {
                if( _isWin )
                {
                    MANAGER_Memory->getTempData()->setLevelState( "level_complete", 1 );
                    anim->setCallbackEndAnimation( graphic::sequence_mob::SM_JOY, [this](){ this->onVictory(); } );
                }
                else
                {
                    anim->setCallbackEndAnimation( graphic::sequence_mob::SM_WAIT, [this](){ this->onDefeat(); } );
                    anim->setState( graphic::sequence_mob::SM_WAIT );
                }
            }
            dynamic_cast< L1_1_Quest_5_MGScreen* >( MANAGER_Screen->getScreen( GET_RUNNING_SCENE, screen_id::MG_L1_1_QUEST_5 ) )->hideMinigameInterface();
            dynamic_cast< L1_1_Quest_4_MGScreen* >( MANAGER_Screen->getScreen( GET_RUNNING_SCENE, screen_id::MG_L1_1_QUEST_4 ) )->hideMinigameInterface();
            dynamic_cast< L1_1_Quest_3_MGScreen* >( MANAGER_Screen->getScreen( GET_RUNNING_SCENE, screen_id::MG_L1_1_QUEST_3 ) )->hideMinigameInterface();
        }
        return;
    }
    
    //first dwarfs mech minigame (longest trash)
    if( _screen == screen_id::MG_L1_1_QUEST_3 )
    {
        if( _isWin )
        {
            this->embiantSource->changeGainTo( 0.8f, 2 );//MAX_VOLUME_EMBIENT_MUSIC, 2 );
            this->backMusicSource->changeGainTo( MAX_VOLUME_EMBIENT_MUSIC, 2 );
            
            this->setLevelState( "mg_1_level_1_1_3_complete", 1 );
            dynamic_cast<L1_1_Quest_3_MGScreen*>(MANAGER_Screen->getScreen(GET_RUNNING_SCENE, screen_id::MG_L1_1_QUEST_3))->hideMinigameInterface();
            this->pDialogScreen->loadDialogFromXml( 22 );
            this->cameraFollowPlayer = true;
            this->saveLevelStatData();
            //coins add
            this->localPlayerCoins += 2;
            this->localPlayerCoinsIntoPoints += 2;
            //complete stat
            MANAGER_Memory->getTempData()->statBrokenMechs += 1;
        }
        else
        {
            //restart minigame (recreating minigame's screen)
            MANAGER_Screen->addScreen( GET_RUNNING_SCENE, screen_id::MG_L1_1_QUEST_3 );
            MANAGER_Screen->detachScreen( GET_RUNNING_SCENE, screen_id::MG_L1_1_QUEST_3 );
            IScreen* mgScreen1 = MANAGER_Screen->getScreen( GET_RUNNING_SCENE, screen_id::MG_L1_1_QUEST_3 );
            mgScreen1->setPosition( 320, 800 );
            mgScreen1->setScale( this->pGameCamera->getDistance() );
            dynamic_cast< GameScene* >( GET_RUNNING_SCENE )->getForegroundLayer()->addChild( mgScreen1, 10 );
            
            this->pDialogScreen->loadDialogFromXml( 27 );
        }
    }
    //second dwarfs mech
    if( _screen == screen_id::MG_L1_1_QUEST_4 )
    {
        if(_isWin)
        {
            MANAGER_Level->resumeLevelTimer();
            this->pHudScreen->showTimer();
            
            this->embiantSource->changeGainTo( 0.8f, 2 );//MAX_VOLUME_EMBIENT_MUSIC, 2 );
            this->backMusicSource->changeGainTo( MAX_VOLUME_EMBIENT_MUSIC, 2 );
            
            this->setLevelState( "mg_2_level_1_1_3_complete", 1 );
            dynamic_cast<L1_1_Quest_4_MGScreen*>(MANAGER_Screen->getScreen(GET_RUNNING_SCENE, screen_id::MG_L1_1_QUEST_4))->hideMinigameInterface();
            this->cameraFollowPlayer = true;
            this->saveLevelStatData();
            this->pHudScreen->showSticker();
            //coins add
            this->localPlayerCoins += 2;
            this->localPlayerCoinsIntoPoints += 2;
            //complete stat
            MANAGER_Memory->getTempData()->statBrokenMechs += 1;
        }
        else
        {
            this->embiantSource->changeGainTo( MIN_VOLUME_EMBIENT_MUSIC, 2 );
            this->backMusicSource->changeGainTo( MIN_VOLUME_EMBIENT_MUSIC, 2 );
            
            //restart minigame (recreating minigame's screen)
            MANAGER_Screen->addScreen( GET_RUNNING_SCENE, screen_id::MG_L1_1_QUEST_4 );
            MANAGER_Screen->detachScreen( GET_RUNNING_SCENE, screen_id::MG_L1_1_QUEST_4 );
            IScreen* mgScreen2 = MANAGER_Screen->getScreen( GET_RUNNING_SCENE, screen_id::MG_L1_1_QUEST_4 );
            mgScreen2->setPosition( 320, 800 );
            mgScreen2->setScale( this->pGameCamera->getDistance() );
            dynamic_cast< GameScene* >( GET_RUNNING_SCENE )->getForegroundLayer()->addChild( mgScreen2, 10 );
            
            this->pDialogScreen->loadDialogFromXml( 23 );
        }
    }
    //third dwarfs mech
    if( _screen == screen_id::MG_L1_1_QUEST_5 )
    {
        this->saveLevelStatData();
        this->embiantSource->changeGainTo( MAX_VOLUME_EMBIENT_MUSIC, 2 ); // 0.8f, 2 );//MAX_VOLUME_EMBIENT_MUSIC, 2 );
        this->backMusicSource->changeGainTo( MAX_VOLUME_EMBIENT_MUSIC, 2 );
        
        //camera fixing on game position
        this->cameraFollowPlayer = true;
        
        if( _isWin == false )
        {
            GameObject* obj = this->getGameObjectById( 2000 );
            
            if( obj )
            {
                obj->setPosition( Vec2(372, 14) );
                obj->setDirection( game_object_direction::LEFT );
                
                graphic::GraphicAnimationObject* anim = dynamic_cast<graphic::GraphicAnimationObject*>( obj->getGraphic() );
                
                if( anim )
                    anim->setState( graphic::sequence_mob::SM_WAIT, false );
                
                ILevel* curLvl = MANAGER_Level->getCurrentLevel();
                
                if( curLvl )
                    curLvl->showGameObject( 2000 );
            }
            
            this->pDialogScreen->loadDialogFromXml( 26 );
        }
        else
        {
            MANAGER_Level->resumeLevelTimer();
            this->pHudScreen->showTimer();
            
            this->pHudScreen->showSticker();
        }
        
        
        dynamic_cast< L1_1_Quest_5_MGScreen* >( MANAGER_Screen->getScreen( GET_RUNNING_SCENE, screen_id::MG_L1_1_QUEST_5 ) )->hideMinigameInterface();
        
        //coins add
        this->localPlayerCoins += 2;
        this->localPlayerCoinsIntoPoints += 2;
        //complete stat
        MANAGER_Memory->getTempData()->statBrokenMechs += 1;
    }
}

//-----------------------------------------------------------------------------------------------------
void Level_1_1_part3::onDialogueEnd( int _id )
{
    if( _id == 20 )
    {
        this->embiantSource->changeGainTo( MIN_VOLUME_EMBIENT_MUSIC, 2 );
        this->backMusicSource->changeGainTo( MIN_VOLUME_EMBIENT_MUSIC, 2 );
        
        dynamic_cast<L1_1_Quest_3_MGScreen*>(MANAGER_Screen->getScreen(GET_RUNNING_SCENE, screen_id::MG_L1_1_QUEST_3))->showMinigameInterface();
        //camera fixing on game position
        this->cameraFollowPlayer = false;
        pGameCamera->moveCameraTo( Vec2(24, 37), 0.01f, true );
        
    }
    
    if( _id == 22 )
    {
        MANAGER_Level->resumeLevelTimer();
        this->pHudScreen->showTimer();
        
        this->pHudScreen->showSticker();
    }
    
    if( _id == 27 )
        this->pDialogScreen->loadDialogFromXml( 20 );
    
    if( _id == 23 )
    {
        this->embiantSource->changeGainTo( MIN_VOLUME_EMBIENT_MUSIC, 2 );
        this->backMusicSource->changeGainTo( MIN_VOLUME_EMBIENT_MUSIC, 2 );
        
        dynamic_cast<L1_1_Quest_4_MGScreen*>(MANAGER_Screen->getScreen(GET_RUNNING_SCENE, screen_id::MG_L1_1_QUEST_4))->showMinigameInterface();
    }

    if( _id == 26 )
    {
        MANAGER_Level->resumeLevelTimer();
        this->pHudScreen->showTimer();
        
        this->pHudScreen->showSticker();
    }
    
    if( _id == 29 )
    {
        this->embiantSource->changeGainTo( MIN_VOLUME_EMBIENT_MUSIC, 2 );
        this->backMusicSource->changeGainTo( MIN_VOLUME_EMBIENT_MUSIC, 2 );
        
        dynamic_cast< L1_1_Quest_5_MGScreen* >( MANAGER_Screen->getScreen( GET_RUNNING_SCENE, screen_id::MG_L1_1_QUEST_5 ) )->showMinigameInterface();
    }
}

//-----------------------------------------------------------------------------------------------------
void Level_1_1_part3::onSwitchArea(GameObject* _switcher)
{
    if(_switcher->getId() == 6000)
    {
        if(this->pPlayer->getGameObject()->getPosition().x < _switcher->getPosition().x)
        {
            if(this->areaType != AREA_CAVE)
            {
                //this->pPlayer->stop();
                this->pHudScreen->resetSticker();
                //this->pHudScreen->hideSticker();
                //
                this->embiantSource->loadAudioFile(EMBIENT_DWARFS);
                this->embiantSource->loadSettings( EMBIENT_DWARFS );
                this->embiantSource->play();
                this->embiantSource->setAssociate( AudioSourceAssociate::MUSIC );
                
                this->backMusicSource->loadAudioFile(MUSIC_CAVE);
                this->backMusicSource->loadSettings( MUSIC_CAVE );
                this->backMusicSource->play();
                this->backMusicSource->setAssociate( AudioSourceAssociate::MUSIC );
                
                this->areaType = AREA_CAVE;
                this->cameraAutoReplaceLeft = true;
                this->pGameCamera->moveCameraTo(Vec2(104, pGameCamera->getPositionY()), 0.3f, true );
                
                auto cameraActiomn = Sequence::create( DelayTime::create( 0.2f ),
                                                         CallFunc::create( CC_CALLBACK_0( Level_1_1_part3::followPlayerCameraY, this ) ), nullptr );
                GET_RUNNING_SCENE->runAction( cameraActiomn );
                //
            }
        }
        else
        {
            if(this->areaType != AREA_AIR_PLATFORMS)
            {
                //this->pPlayer->stop();
                this->pHudScreen->resetSticker();
                //this->pHudScreen->hideSticker();
                //
                this->embiantSource->loadAudioFile(EMBIENT_BIRDS);
                this->embiantSource->loadSettings( EMBIENT_BIRDS );
                this->embiantSource->play();
                this->embiantSource->setAssociate( AudioSourceAssociate::MUSIC );
                this->embiantSource->setGain( MAX_VOLUME_EMBIENT_BIRDS );
                
                this->backMusicSource->loadAudioFile(MUSIC_ISLAND);
                this->backMusicSource->loadSettings( MUSIC_ISLAND );
                this->backMusicSource->play();
                this->backMusicSource->setAssociate( AudioSourceAssociate::MUSIC );
                
                this->areaType = AREA_AIR_PLATFORMS;
                this->cameraAutoReplaceRight = true;
                this->pGameCamera->moveCameraTo(Vec2(151, pGameCamera->getPositionY()), 0.3f, true);
                
                //
                auto cameraActiomn = Sequence::create( DelayTime::create( 0.2f ),
                                                       CallFunc::create( CC_CALLBACK_0( Level_1_1_part3::followPlayerCameraY, this ) ), nullptr );
                GET_RUNNING_SCENE->runAction( cameraActiomn );
                //
            }
        }
    }
    
    if(_switcher->getId() == 6001)
    {
        if(this->pPlayer->getGameObject()->getPosition().x < _switcher->getPosition().x)
        {
            if(this->areaType != AREA_AIR_PLATFORMS)
            {
                //this->pPlayer->stop();
                this->pHudScreen->resetSticker();
                //this->pHudScreen->hideSticker();
                //
                this->embiantSource->loadAudioFile(EMBIENT_BIRDS);
                this->embiantSource->loadSettings( EMBIENT_BIRDS );
                this->embiantSource->play();
                this->embiantSource->setAssociate( AudioSourceAssociate::MUSIC );
                this->embiantSource->setGain( MAX_VOLUME_EMBIENT_BIRDS );
                
                this->backMusicSource->loadAudioFile(MUSIC_ISLAND);
                this->backMusicSource->loadSettings( MUSIC_ISLAND );
                this->backMusicSource->play();
                this->backMusicSource->setAssociate( AudioSourceAssociate::MUSIC );
                
                this->areaType = AREA_AIR_PLATFORMS;
                this->cameraAutoReplaceLeft = true;
                this->pGameCamera->moveCameraTo(Vec2(299, pGameCamera->getPositionY()), 0.3f, true );
                //
                auto cameraActiomn = Sequence::create( DelayTime::create( 0.2f ),
                                                         CallFunc::create( CC_CALLBACK_0( Level_1_1_part3::followPlayerCameraY, this ) ), nullptr );
                GET_RUNNING_SCENE->runAction( cameraActiomn );
                //
            }
        }
        else
        {
            if(this->areaType != AREA_CAVE)
            {
                //this->pPlayer->stop();
                this->pHudScreen->resetSticker();
                //this->pHudScreen->hideSticker();
                //
                this->embiantSource->loadAudioFile(EMBIENT_DWARFS);
                this->embiantSource->loadSettings( EMBIENT_DWARFS );
                this->embiantSource->play();
                this->embiantSource->setAssociate( AudioSourceAssociate::MUSIC );
                
                this->backMusicSource->loadAudioFile(MUSIC_CAVE);
                this->backMusicSource->loadSettings( MUSIC_CAVE );
                this->backMusicSource->play();
                this->backMusicSource->setAssociate( AudioSourceAssociate::MUSIC );
                
                this->areaType = AREA_CAVE;
                this->cameraAutoReplaceRight = true;
                this->pGameCamera->moveCameraTo(Vec2(346, pGameCamera->getPositionY()), 0.3f, true );
                //
                auto cameraActiomn = Sequence::create( DelayTime::create( 0.3f ),
                                                       CallFunc::create( CC_CALLBACK_0( Level_1_1_part3::followPlayerCameraY, this ) ), nullptr );
                GET_RUNNING_SCENE->runAction( cameraActiomn );
                //
            }
        }
    }
}

//-----------------------------------------------------------------------------------------------------
void Level_1_1_part3::onVictory()
{
    this->pHudScreen->hideSticker();
    if( this->getLevelState( "SINGLE_MINIGAME" ) == 1 )
        MANAGER_Scene->replaceScene( scene_id::MINIGAMES_SELECTION );
    else
    {
        this->saveLevelStatData();
        
        //ADDING CORRECT LOCATION RESTART
        MANAGER_Memory->getTempData()->locationCoinsIntoPoints = MANAGER_Memory->getTempData()->playerCoinsIntoPoints;
        
        MANAGER_Memory->getTempData()->vGetCoinIds.clear();
        MANAGER_Memory->getTempData()->vKilledMobsIds.clear();
        
        MANAGER_Memory->getTempData()->levelsProgressData[2]->gameLevelsProgress = level_state::LVL_STATE_COMPLETED;
        if( MANAGER_Memory->getTempData()->levelsProgressData[3]->gameLevelsProgress != level_state::LVL_STATE_COMPLETED )
            MANAGER_Memory->getTempData()->levelsProgressData[3]->gameLevelsProgress = level_state::LVL_STATE_DONATE_LOCK;
        MANAGER_Memory->getTempData()->setLevelStars( 2 );
        
        MANAGER_Memory->uploadTempToPersistData();
        MANAGER_Memory->saveProfile();
        
        CompleteScreen* _pCompleteScreen = dynamic_cast< CompleteScreen* >( MANAGER_Screen->getScreen( GET_RUNNING_SCENE, screen_id::COMPLETE ) );
        if( _pCompleteScreen )
        {
            _pCompleteScreen->setCompletedLevel( MANAGER_Memory->getTempData()->currentLevelIndex );
            _pCompleteScreen->show();
        }
    }
}

//-----------------------------------------------------------------------------------------------------
void Level_1_1_part3::onDefeat()
{
    this->pHudScreen->hideSticker();
    if( this->getLevelState( "SINGLE_MINIGAME" ) == 1 )
        MANAGER_Scene->replaceScene( scene_id::MINIGAMES_SELECTION );
    else
        dynamic_cast<TryMoreScreen*>(MANAGER_Screen->getScreen(GET_RUNNING_SCENE, screen_id::TRY_MORE))->show();
}

//-----------------------------------------------------------------------------------------------------
void Level_1_1_part3::determinePlayerPosition()
{
    //player first positioning
    MANAGER_Memory->getTempData()->playerPosition = Vec2( 5, 31.7 ); //5, 31.7
    
    //player second positioning ( after restart level from trymore screen )
    if( this->getLevelState( "mg_1_level_1_1_3_complete" ) == 1 )
        MANAGER_Memory->getTempData()->playerPosition = Vec2( 34, 30.9 );
    //cp 1 camera auto-replace
    if( this->getLevelState( "level_1_1_3_cp_1") == 1 )
    {
        MANAGER_Memory->getTempData()->playerPosition = Vec2( 131, 29.2 );
        this->cameraFollowPlayer = false;
        this->followPlayerCameraY();
    }
    //cp 2 camera auto-replace
    if( this->getLevelState( "level_1_1_3_cp_2") == 1 )
    {
        MANAGER_Memory->getTempData()->playerPosition = Vec2( 324, 31.7 );
        this->cameraFollowPlayer = false;
        this->followPlayerCameraY();
    }
    //
    if( this->getLevelState( "mg_2_level_1_1_3_complete" ) == 1)
        MANAGER_Memory->getTempData()->playerPosition = Vec2( 362, 13 );
    if( this->getLevelState( "mg_3_level_1_1_3_complete" ) == 1)
        MANAGER_Memory->getTempData()->playerPosition = Vec2( 404, 29 );
    
    //set player position
    this->pPlayer->getGameObject()->setPosition( MANAGER_Memory->getTempData()->playerPosition );
    
    //add player gameobject to active obj list
    this->showGameObject( 777 );
    
    this->areaType = AREA_CAVE;
    
    //camera positioning
    if( this->getLevelState( "level_1_1_3_cp_1" ) == 1 )
    {
        this->pGameCamera->setPosition( Vec2( 151, 28 ) );
        this->setLevelState( "level_1_1_3_cp_1", 0 );
        this->areaType = AREA_AIR_PLATFORMS;
    }
    if( this->getLevelState( "level_1_1_3_cp_2" ) == 1 )
    {
        this->pGameCamera->setPosition( Vec2( 346, 32 ) );
        this->setLevelState( "level_1_1_3_cp_2", 0 );
        this->areaType = AREA_CAVE;
    }
    
    //Camera positionig according to player
    if( this->cameraFollowPlayer )
        pGameCamera->setPosition( pPlayer->getGameObject()->getPosition() );
}

//-----------------------------------------------------------------------------------------------------
void Level_1_1_part3::onCheckPoint( GameObject* _checkPoint )
{
    if( _checkPoint->getId() == 970 )
    {
        this->setLevelState( "level_1_1_3_cp_1", 1 );
        
        this->saveLevelStatData();
        
        _checkPoint->release();
    }
    if( _checkPoint->getId() == 971 )
    {
        this->setLevelState( "level_1_1_3_cp_2", 1 );
        
        this->saveLevelStatData();
        
        _checkPoint->release();
    }
}

//-----------------------------------------------------------------------------------------------------
void Level_1_1_part3::singleMinigameLogic()
{
    //player positioning, logic adding (screen with minigame)
    if( this->getLevelState( "sm_level_1_1_3" ) == 1 )
    {
        MANAGER_Memory->getTempData()->playerPosition = Vec2( 17, 33 );
        //
        this->embiantSource->changeGainTo( MIN_VOLUME_EMBIENT_MUSIC, 2 );
        this->backMusicSource->changeGainTo( MIN_VOLUME_EMBIENT_MUSIC, 2 );
        
        dynamic_cast<L1_1_Quest_3_MGScreen*>(MANAGER_Screen->getScreen(GET_RUNNING_SCENE, screen_id::MG_L1_1_QUEST_3))->showMinigameInterface();
        //camera fixing on game position
        this->cameraFollowPlayer = false;
        //pGameCamera->moveCameraTo( Vec2(320 / 32, 896 / 24), 0 );
        pGameCamera->setPosition( Vec2(320 / 32, 896 / 24) );
        //
        this->pHudScreen->hideSticker();
    }
    if( this->getLevelState( "sm_level_1_1_3" ) == 2 )
    {
        MANAGER_Memory->getTempData()->playerPosition = Vec2( 351, 15 );
        //
        this->embiantSource->changeGainTo( MIN_VOLUME_EMBIENT_MUSIC, 2 );
        this->backMusicSource->changeGainTo( MIN_VOLUME_EMBIENT_MUSIC, 2 );
        
        dynamic_cast<L1_1_Quest_4_MGScreen*>(MANAGER_Screen->getScreen(GET_RUNNING_SCENE, screen_id::MG_L1_1_QUEST_4))->showMinigameInterface();
        //camera fixing on game position
        this->cameraFollowPlayer = false;
       // pGameCamera->moveCameraTo(Vec2(356.0f, 13.0f), 0);
        pGameCamera->setPosition(Vec2(356.0f, 13.0f));
        //
        this->pHudScreen->hideSticker();
    }
    if( this->getLevelState( "sm_level_1_1_3" ) == 3 )
    {
        MANAGER_Memory->getTempData()->playerPosition = Vec2( 400, 28 );
        //
        this->embiantSource->changeGainTo( MIN_VOLUME_EMBIENT_MUSIC, 2 );
        this->backMusicSource->changeGainTo( MIN_VOLUME_EMBIENT_MUSIC, 2 );
        
        dynamic_cast< L1_1_Quest_5_MGScreen* >( MANAGER_Screen->getScreen( GET_RUNNING_SCENE, screen_id::MG_L1_1_QUEST_5 ) )->showMinigameInterface();
        //camera fixing on game position
        this->cameraFollowPlayer = false;
        //pGameCamera->moveCameraTo(Vec2(392.0f, 37.0f), 0);
        pGameCamera->setPosition(Vec2(392.0f, 37.0f));
        //
        this->pHudScreen->hideSticker();
    }
    //
    this->pPlayer->getGameObject()->setPosition( MANAGER_Memory->getTempData()->playerPosition );
    //add player gameobject to active obj list
    this->showGameObject( 777 );
}

//-----------------------------------------------------------------------------------------------------
void Level_1_1_part3::removeTestVoiceScreen( Ref* _sender )
{
    MANAGER_Screen->removeScreen( GET_RUNNING_SCENE, screen_id::TEST_VOICE );
    this->embiantSource->changeGainTo( MAX_VOLUME_EMBIENT_MUSIC, 2 );
    this->backMusicSource->changeGainTo( MAX_VOLUME_EMBIENT_MUSIC, 2 );
    
    this->pHudScreen->showSticker();
    MANAGER_Level->resumeLevelTimer();
    this->pHudScreen->showTimer();
}

//-----------------------------------------------------------------------------------------------------
void Level_1_1_part3::followPlayerCameraY()
{
    this->cameraAutoReplaceRight = false;
    this->cameraAutoReplaceLeft = false;
    //this->pHudScreen->showSticker();
}

//-----------------------------------------------------------------------------------------------------
void Level_1_1_part3::saveLevelStatData()
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