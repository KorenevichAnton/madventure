//
//  Level_1_boss.cpp
//  MusicalAdventure
//
//  Created by madmin on 6/25/14.
//  Copyright (c) 2014 ampagames. All rights reserved.
//

#include "Level_1_boss.h"

#include "GameScene.h"

#include "HudScreen.h"

#include "GameObject.h"
#include "AiBoss1Object.h"
#include "PhysDynamicBoss1Object.h"
#include "AiMovablePlatformObject.h"

#include "L1_3_Quest_1_MGScreen.h"
#include "L1_3_Quest_3_MGScreen.h"

#include "CompleteScreen.h"
#include "TryMoreScreen.h"
#include "DialogScreen.h"

#include "GraphicAnimationObject.h"
#include "AiNpcBatObject.h"

#define TIME_MOVE_BAT 1.0f

#define TARGET_POS_BAT_SOLDIER Vec2( 169, 38 )
#define TARGET_POS_BAT_BLIND Vec2( 266, 23 )
#define TARGET_POS_BAT_LEAVE_BOSS Vec2( 210, 25 )

using namespace physic;

//-----------------------------------------------------------------------------------------------------
Level_1_boss::Level_1_boss():
earthquakeTime(0.0f),
earthquakeLimit(2.0f),
earthquaker(0),
shadowValue(255),
bossObject(NULL),
startBossMG(false),
pBossMinigameScreen(NULL),
afterES(false),
cameraFollowPlayer(true),
cameraFollowPlayerY(false),
cameraAutoReplace(false),
cameraAutoReplaceRight( false ),
cameraAutoReplaceLeft( false )
{
    level = level_id::LVL_1_BOSS;
    
    this->levelGameWorldPath = new char[50];
    sprintf(levelGameWorldPath, "%s%s", PHYS_LANDSCAPES_PATH , "landscape_boss_1.xml");
    this->levelGameObjectsPath = new char[50];
    sprintf(levelGameObjectsPath, "%s%s", PHYS_OBJECTS_PATH , "objects_boss_1.xml");
}

//-----------------------------------------------------------------------------------------------------
Level_1_boss::~Level_1_boss()
{
}

//-----------------------------------------------------------------------------------------------------
Level_1_boss* Level_1_boss::create()
{
    Level_1_boss* pLevel_1_boss = new Level_1_boss();
    if(pLevel_1_boss)
    {
        return pLevel_1_boss;
    }
    else
    {
        delete pLevel_1_boss;
        return NULL;
    }
}

//-----------------------------------------------------------------------------------------------------
bool Level_1_boss::init()
{
    //set using screens
    this->pDialogScreen = dynamic_cast< DialogScreen* > ( MANAGER_Screen->getScreen( GET_RUNNING_SCENE, screen_id::DIALOG ) );
    this->pHudScreen = dynamic_cast< HudScreen* >( MANAGER_Screen->getScreen( GET_RUNNING_SCENE, screen_id::HUD) );
    
    this->aSource = MANAGER_Audio->createStaticSource();
    if( this->aSource )
        this->aSource->setAssociate( AudioSourceAssociate::SOUND );
    
    //
    //minigames screens
    if( this->getLevelState( "mg_1_level_1_boss_complete" ) != 1 )
    {
        MANAGER_Screen->addScreen(GET_RUNNING_SCENE, screen_id::MG_L1_3_QUEST_1);
        MANAGER_Screen->detachScreen(GET_RUNNING_SCENE, screen_id::MG_L1_3_QUEST_1);
        MANAGER_Screen->getScreen(GET_RUNNING_SCENE, screen_id::MG_L1_3_QUEST_1);
        IScreen* mgScreen1 = MANAGER_Screen->getScreen(GET_RUNNING_SCENE, screen_id::MG_L1_3_QUEST_1);
        mgScreen1->setPosition(5150, 430);
        mgScreen1->setScale((this->pGameCamera->getDistance()));
        dynamic_cast<GameScene*>(GET_RUNNING_SCENE)->getForegroundLayer()->addChild(MANAGER_Screen->getScreen(GET_RUNNING_SCENE, screen_id::MG_L1_3_QUEST_1), 2010);
        dynamic_cast<L1_3_Quest_1_MGScreen*>( MANAGER_Screen->getScreen(GET_RUNNING_SCENE, screen_id::MG_L1_3_QUEST_1) )->hideMinigameInterface();
        
    }
    
    /*Add back music
     if( this->getLevelState( "minigame_boss_level_1_boss_complete" ) != 1 )
     {
     MANAGER_Screen->addScreen( GET_RUNNING_SCENE, screen_id::MG_BOSS1 );
     MANAGER_Screen->detachScreen( GET_RUNNING_SCENE, screen_id::MG_BOSS1 );
     MANAGER_Screen->getScreen( GET_RUNNING_SCENE, screen_id::MG_BOSS1 );
     IScreen* mgScreen2 = MANAGER_Screen->getScreen( GET_RUNNING_SCENE, screen_id::MG_BOSS1 );
     mgScreen2->setPosition( 7422, 250 );
     mgScreen2->setScale( this->pGameCamera->getDistance() );
     dynamic_cast<GameScene*>( GET_RUNNING_SCENE )->getForegroundLayer()->addChild( MANAGER_Screen->getScreen( GET_RUNNING_SCENE, screen_id::MG_BOSS1 ), 2090 );
     dynamic_cast<Boss1_MGScreen*>( MANAGER_Screen->getScreen(GET_RUNNING_SCENE, screen_id::MG_BOSS1) )->hideMinigameInterface();
     }
     */
    
    //pre-boss dark
    this->shadow = cocos2d::Layer::create();
    this->preShadow = cocos2d::CCSprite::create( "textures/scenes/world/effects/black_white.png" );
    this->preShadow->setAnchorPoint( Vec2( 0, 0 ) );
    this->preShadow->setPosition( Vec2( 0, 0) );
    this->preShadow->setScaleY( 2304 );
    this->shadow->addChild( preShadow, 2001 );
    this->afterShadow = CCSprite::create( "textures/scenes/world/effects/shadow_pixel.png" );
    this->afterShadow->setScaleX( 5120 );
    this->afterShadow->setScaleY( 2304 );
    this->afterShadow->setAnchorPoint( Vec2( 0.0, 0.0 ) );
    this->afterShadow->setPosition( Vec2( 1024, 0 ) );
    this->shadow->addChild( afterShadow, 2001 );
    
    if(this->shadow)
    {
        this->shadow->setPosition(Vec2(6 * 1024, 0));
        
        if(GET_RUNNING_SCENE->getSceneId() == scene_id::GAME)
            dynamic_cast<GameScene*>(GET_RUNNING_SCENE)->getForegroundLayer()->addChild(this->shadow, 2001);
    }
    //
    
    //
    //Logic on enter level
    //open cave before musicbox 1
    if( this->getLevelState( "open_cave_level_1_boss" ) == 1 )
    {
        this->hideGameObject( 533 );
        this->hideGameObject( 535 );
    }
    
    //after musicbox 1
    if( this->getLevelState( "musicbox_1_level_1_boss_complete" ) == 1 )
    {
        this->hideGameObject( 903 );
        this->showGameObject( 923 );
    }
    
    //after musicbox 2
    if( this->getLevelState( "musicbox_2_level_1_boss_complete" ) == 1 )
    {
        this->hideGameObject( 904 );
        this->showGameObject( 924 );
    }
    
    //soldiers
    if( this->getLevelState( "mg_1_level_1_boss_complete" ) == 1 )
    {
        this->hideGameObject( 901 );
        //animation g
        this->hideGameObject( 1500 );
        this->hideGameObject( 1501 );
        this->hideGameObject( 1502 );
        //sleeping g
        this->hideGameObject( 1605 );
        this->hideGameObject( 1606 );
        this->hideGameObject( 1607 );
        //woke up g
        this->showGameObject( 1600 );
        this->showGameObject( 1601 );
        this->showGameObject( 1602 );
    }
    else
    {
        if( APPLICATION->getDeviceModel() == "iPad" ||
           APPLICATION->getDeviceModel() == "iPad 2" )
        {
            //animation g
            this->hideGameObject( 1500 );
            this->hideGameObject( 1501 );
            this->hideGameObject( 1502 );
            //sleeping g
            this->showGameObject( 1605 );
            this->showGameObject( 1606 );
            this->showGameObject( 1607 );
            //woke up g
            this->hideGameObject( 1600 );
            this->hideGameObject( 1601 );
            this->hideGameObject( 1602 );
        }
        else
        {
            //animation g
            this->showGameObject( 1500 );
            this->showGameObject( 1501 );
            this->showGameObject( 1502 );
            //sleeping g
            this->hideGameObject( 1605 );
            this->hideGameObject( 1606 );
            this->hideGameObject( 1607 );
            //woke up g
            this->hideGameObject( 1600 );
            this->hideGameObject( 1601 );
            this->hideGameObject( 1602 );
        }
    }
    
    //minigame boss level complete
    if( this->getLevelState( "minigame_boss_level_1_boss_complete" ) == 1 )
    {
        // RAM - unloading
        this->showGameObject( 33001 );
        
        this->getGameObjectById( 1500 )->release();
        this->getGameObjectById( 1501 )->release();
        this->getGameObjectById( 1502 )->release();
        //sleeping g
        this->getGameObjectById( 1605 )->release();
        this->getGameObjectById( 1606 )->release();
        this->getGameObjectById( 1607 )->release();
        //woke up g
        this->getGameObjectById( 1600 )->release();
        this->getGameObjectById( 1601 )->release();
        this->getGameObjectById( 1602 )->release();
        
        if( this->getLevelState( "boss_dead_level_1_boss" ) != 1  )
        {
            //close room with boss
            this->showGameObject( 24 );
            
            //positioning camera for minigame
            this->cameraFollowPlayer = false;
            pGameCamera->moveCameraTo(Vec2(7 * 32 + 24, 20), 0);
            pGameCamera->setPosition(Vec2(7 * 32 + 24, 20));
            
            this->pDialogScreen->loadDialogFromXml( 98 );
        }
        else
        {
            // if we killed boss and fall in abuse
            //unshadow
            this->getGameObjectById( 101 )->getPhysic()->setActivity( false );
            
            this->preShadow->runAction( CCFadeTo::create( 0.1f, 0 ) );
            this->afterShadow->runAction( CCFadeTo::create( 0.1f, 0 ) );
            
            // hide right blocking wall (boss minigame and boss)
            this->hideGameObject( 25 );
        }
        
        //
        this->backMusicSource->loadAudioFile(MUSIC_WAR);
        this->backMusicSource->loadSettings( MUSIC_WAR );
        this->backMusicSource->setLooping(true);
        this->backMusicSource->play();
        this->backMusicSource->setAssociate( AudioSourceAssociate::MUSIC );
        
        this->areaType = AREA_WAR;
        //
    }
    
    //removing taken coins
    std::vector< int > coinsToRemove = vLocalGetCoinIds;
    for( int i = 0 ; i < coinsToRemove.size() ; i++ )
        this->hideGameObject( coinsToRemove[i] );
    //removing killed mobs
    std::vector< int > mobsToRemove = vLocalKilledMobsIds;
    for( int i = 0 ; i < mobsToRemove.size() ; i++ )
        this->hideGameObject( mobsToRemove[i] );
    
    //set logic if started single minigame
    if( this->getLevelState( "SINGLE_MINIGAME" ) == 1)
        this->singleMinigameLogic();
    
    return true;
}

//-----------------------------------------------------------------------------------------------------
void Level_1_boss::onEnter()
{
    //level timer
    MANAGER_Level->resumeLevelTimer();
    
    //Player positioning
    //this->determinePlayerPosition();
    
    //on restart level from pause/defeat/win level loading
    this->restartLevel = this->level;
    
    //Add back music
    if( this->getLevelState( "minigame_boss_level_1_boss_complete" ) != 1 )
    {
        this->embiantSource->loadAudioFile(EMBIENT_DWARFS);
        this->embiantSource->loadSettings( EMBIENT_DWARFS );
        this->embiantSource->setLooping(true);
        this->embiantSource->play();
        this->embiantSource->setAssociate(AudioSourceAssociate::MUSIC);
        
        this->backMusicSource->loadAudioFile(MUSIC_CAVE);
        this->backMusicSource->loadSettings( MUSIC_CAVE );
        this->backMusicSource->setLooping(true);
        this->backMusicSource->play();
        this->backMusicSource->setAssociate(AudioSourceAssociate::MUSIC);
        
        //location current area showing what music should play at the moment
        this->areaType = AREA_CAVE;
    }
    else
    {
        this->areaType = AREA_CASTLE;
    }
    
    if( this->getLevelState( "mg_1_level_1_boss_complete") == 1 )
    {
        this->backMusicSource->loadAudioFile( MUSIC_CASTLE );
        this->backMusicSource->loadSettings( MUSIC_CASTLE );
        this->backMusicSource->play();
        this->backMusicSource->setAssociate( AudioSourceAssociate::MUSIC );
    }
    
    this->embiantSource->setGain( 0.7 );
    this->backMusicSource->setGain( 0.7 );
    
    //Camera positionig according to player
    //if( this->cameraFollowPlayer )
    //    this->pGameCamera->setPosition( pPlayer->getGameObject()->getPosition() );
    
    //level index for difficulty and complete screen
    MANAGER_Memory->getTempData()->currentLevelIndex = 7;
    //FOR test boss
    //dynamic_cast<ai::AiBoss1Object*>( this->getGameObjectById( 1000 )->getAi() )->setStartBoss(true);
}

//-----------------------------------------------------------------------------------------------------
void Level_1_boss::onExit()
{
    if( MANAGER_Memory->getTempData()->getLevelState( "SINGLE_MINIGAME_RESTART" ) == 0 )
    {
        this->setLevelState( "SINGLE_MINIGAME", 0 );
        this->setLevelState( "sm_level_1_boss", 0 );
    }
    else
        MANAGER_Memory->getTempData()->setLevelState( "SINGLE_MINIGAME_RESTART", 1 );
    
    MANAGER_Level->stopLevelTimer();
    this->uploadLevelStates();
}

//-----------------------------------------------------------------------------------------------------
void Level_1_boss::release()
{
    MANAGER_Screen->removeAllScreens(GET_RUNNING_SCENE);
    delete []this->levelGameWorldPath;
    delete []this->levelGameObjectsPath;
    delete this;
}

//-----------------------------------------------------------------------------------------------------
void Level_1_boss::update(float _dt)
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
        {
            if( this->pPlayer->getGameObject()->getDirection() == game_object_direction::LEFT )
                this->pPlayer->moveLeft();
            else
                this->pPlayer->moveRight();
        }
        if( this->cameraAutoReplaceRight )
        {
            if( this->pPlayer->getGameObject()->getDirection() == game_object_direction::LEFT )
                this->pPlayer->moveLeft();
            else
                this->pPlayer->moveRight();
        }
        
        //TEMP : start boss
        if( this->getLevelState( "start_boss_level_1_boss" ) == 1 )
        {
            this->earthquakeTime += _dt;
            
            //Earth quakings
            if(this->earthquakeTime < this->earthquakeLimit)
            {
                if(this->earthquaker == 1)
                {
                    this->preShadow->runAction(CCFadeTo::create(3.0f, 0));
                    this->afterShadow->runAction(CCFadeTo::create(3.0f, 0));
                }
                earthquaker += 1;
                if(earthquaker % 2 == 0)
                    pGameCamera->setPosition(Vec2(pGameCamera->getPosition().x - 2, pGameCamera->getPosition().y));
                else
                    pGameCamera->setPosition(Vec2(pGameCamera->getPosition().x + 2, pGameCamera->getPosition().y));
            }
            else
            {
                this->showGameObject( 1000 );
                //
                this->setLevelState( "start_boss_level_1_boss", 2 );
            }
        }
        //TEMP : OR NOT????
        if( bossObject && this->getLevelState( "boss_dead_level_1_boss" ) != 1 )
        {
            ai::AiBoss1Object* pBossAi = dynamic_cast<ai::AiBoss1Object*>(bossObject->getAi());
            if( pBossAi )
            {
                if( pBossAi->getHp() > 0 )
                {
                    if( this->oldHpBoss != pBossAi->getHp() && 20 == pBossAi->getHp() * 100 / pBossAi->getFullHp())
                    {
                        this->hideGameObject( 100 );
                        IAudioSource *aSource = MANAGER_Audio->createStaticSource(SOUND_DESTROYED_PLATFORM);
                        if( aSource )
                        {
                            aSource->play();
                            aSource->setAssociate(AudioSourceAssociate::SOUND);
                        }
                    }
                }
                else
                {
                    graphic::GraphicAnimationObject* graphBoss = dynamic_cast<graphic::GraphicAnimationObject*>(bossObject->getGraphic());
                    
                    if(graphBoss &&
                       graphBoss->getState() == graphic::sequence_mob::SM_DEATH )
                    {
                        IAudioSource *aSource = MANAGER_Audio->createStaticSource(SOUND_BOSS_DEFEAT);
                        if( aSource )
                        {
                            aSource->play();
                            aSource->setAssociate(AudioSourceAssociate::SOUND);
                        }
                        
                        this->getGameObjectById( 101 )->getPhysic()->setActivity( false );
                        
                        this->cameraFollowPlayer = true;
                        //
                        this->embiantSource->changeGainTo( 0.0f, 1.0f );
                        this->backMusicSource->changeGainTo( 0.0f, 1.0f );
                        //
                        this->areaType = AREA_CAVE;
                        //
                        this->saveLevelStatData();
                        this->setLevelState( "boss_dead_level_1_boss", 1 );
                        this->embiantSource->setGain( 0.5f );
                        this->backMusicSource->setGain( 0.5f );
                        //coins add
                        this->localPlayerCoins += 5;
                        this->localPlayerCoinsIntoPoints += 5;
                        
                        this->pDialogScreen->loadDialogFromXml( 100 );
                    }
                }
                
                this->oldHpBoss = pBossAi->getHp();
            }
        }
        
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
bool Level_1_boss::onGetCoin(GameObject* _coin, std::string _key, PhysTriggerCoinKind _kind)
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
void Level_1_boss::onPortal(GameObject *_portal)
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
            
            MANAGER_Level->getCurrentLevel()->getEmbiantSource()->changeGainTo(0.0f, 2);
            MANAGER_Level->getCurrentLevel()->getBackMusicSource()->changeGainTo(0.0f, 2);
            
            auto defeatAction = Sequence::create( DelayTime::create( 2.0f ),
                                                  CallFunc::create( CC_CALLBACK_0( Level_1_boss::onDefeat, this ) ), nullptr );
            GET_RUNNING_SCENE->runAction( defeatAction );
            
            _portal->release();
        }
        
        //Location triggers-portals
        //open cave to musicbox 1
        if( _portal->getId() == 905 )
        {
            this->setLevelState( "open_cave_level_1_boss", 1 );
            //
            this->hideGameObject( 533 );
            this->hideGameObject( 535 );
        }
        //go musicbox 1
        if( _portal->getId() == 903 )
        {
            //level timer
            MANAGER_Level->stopLevelTimer();
            this->pHudScreen->hideTimer();
            
            this->saveLevelStatData();
            MANAGER_Level->replaceLevel( level_id::MG_1_3_1_MUSICBOX_2 );
        }
        //go musicbox 2
        if( _portal->getId() == 904 )
        {
            //level timer
            MANAGER_Level->stopLevelTimer();
            this->pHudScreen->hideTimer();
            
            this->saveLevelStatData();
            MANAGER_Level->replaceLevel( level_id::MG_1_3_1_MUSICBOX_3 );
        }
        //soldiers minigame
        if( _portal->getId() == 901 )
        {
            
            // The bat fly to soldier
            GameObject* objBat = this->getGameObjectById( 1700 );
            
            if( objBat )
            {
                ai::AiNpcBatObject* aiBat = dynamic_cast<ai::AiNpcBatObject*>( objBat->getAi() );
                
                if( aiBat )
                    aiBat->moveTo( TARGET_POS_BAT_SOLDIER, 10, [this](){ this->onBatPositionSoldier(); } );
                
                this->showGameObject( 1700 );
            }
            //
            
            //level timer
            MANAGER_Level->stopLevelTimer();
            this->pHudScreen->hideTimer();
            
            MANAGER_Level->getCurrentLevel()->getEmbiantSource()->changeGainTo(0.0f, 2);
            MANAGER_Level->getCurrentLevel()->getBackMusicSource()->changeGainTo(0.0f, 2);
            
            //
            this->pHudScreen->hideSticker();
            
            
            //positioning camera for minigame
            this->cameraFollowPlayer = false;
            pGameCamera->moveCameraTo( Vec2(170, 26), 0.01f, true );
            
            _portal->release();
        }
        //boss minigame
        if( _portal->getId() == 900 && this->getLevelState( "minigame_boss_level_1_boss_complete") != 1 )
        {
            
            //The bat fly to blinds
            GameObject* objBat = this->getGameObjectById( 1700 );
            
            if( objBat )
            {
                objBat->setPosition( Vec2( 212, 25 ) );
                
                ai::AiNpcBatObject* aiBat = dynamic_cast<ai::AiNpcBatObject*>( objBat->getAi() );
                
                if( aiBat )
                    aiBat->moveTo( TARGET_POS_BAT_BLIND, 10, [this](){ this->onBatPositionBlind(); } );
                
                this->showGameObject( 1700 );
            }
            //
            
            MANAGER_Level->getCurrentLevel()->getEmbiantSource()->changeGainTo(0.0f, 2);
            MANAGER_Level->getCurrentLevel()->getBackMusicSource()->changeGainTo(0.0f, 2);
            
            MANAGER_Level->stopLevelTimer();
            this->pHudScreen->hideTimer();
            //close room with boss
            this->showGameObject( 24 );
            
            //add boss-minigame
            this->pHudScreen->hideSticker();
            
            //positioning camera for minigame
            this->cameraFollowPlayer = false;
            pGameCamera->moveCameraTo(Vec2(7 * 32 + 24, 20), 0.01f, true );
            
            _portal->release();
        }
        //horn minigame
        if( _portal->getId() == 902 )
        {
            MANAGER_Level->stopLevelTimer();
            this->pHudScreen->hideTimer();
            
            this->pDialogScreen->loadDialogFromXml( 102 );
            this->pHudScreen->hideSticker();
            MANAGER_Screen->addScreen(GET_RUNNING_SCENE, screen_id::MG_L1_3_QUEST_3);
            
            this->embiantSource->changeGainTo( MIN_VOLUME_EMBIENT_MUSIC, 2);
            this->backMusicSource->changeGainTo( MIN_VOLUME_EMBIENT_MUSIC, 2);
            
            _portal->release();
        }
        
        //pre/post camera autoreplace
        if( _portal->getId() == 6005 )
        {
            if( this->pPlayer->getGameObject()->getPosition().x < 180 ||
               this->pPlayer->getGameObject()->getPosition().x > 210 )
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
                if( this->pPlayer->getGameObject()->getPosition().x < 190 ) //this->areaType == AREA_CAVE )
                    this->pGameCamera->moveCameraTo( Vec2( 170.0f, this->pPlayer->getGameObject()->getPosition().y ), CAMERA_FOLLOW_SPEED , true ); //this->pGameCamera->setPositionX( 170.0f );
                if( this->pPlayer->getGameObject()->getPosition().x > 194 ) // this->areaType == AREA_CASTLE )
                    this->pGameCamera->moveCameraTo( Vec2( 219.0f, this->pPlayer->getGameObject()->getPosition().y ), CAMERA_FOLLOW_SPEED , true ); //this->pGameCamera->setPositionX( 219.0f );
            }
        }
        
        //back music switch before guards
        if( _portal->getId() == 908 )
        {
            this->backMusicSource->loadAudioFile(MUSIC_CASTLE);
            this->backMusicSource->loadSettings( MUSIC_CASTLE );
            this->backMusicSource->play();
            this->backMusicSource->setAssociate( AudioSourceAssociate::MUSIC );
            _portal->release();
        }
        
        /*removing guards blocking cave
         if( _portal->getId() == 906 )
         {
         MANAGER_Screen->removeScreen( GET_RUNNING_SCENE, screen_id::MG_L1_3_QUEST_1 );
         //
         this->hideGameObject( 1500 );
         this->hideGameObject( 1501 );
         this->hideGameObject( 1502 );
         this->hideGameObject( 1503 );
         //
         this->hideGameObject( 1600 );
         this->hideGameObject( 1601 );
         this->hideGameObject( 1602 );
         this->hideGameObject( 1603 );
         //
         this->showGameObject( 907 );
         //set up boss.
         _portal->release();
         }
         */
    }
}

//-----------------------------------------------------------------------------------------------------
void Level_1_boss::onMinigameEnd( bool _isWin, screen_id _screen )
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
            dynamic_cast<L1_3_Quest_1_MGScreen*>(MANAGER_Screen->getScreen(GET_RUNNING_SCENE, screen_id::MG_L1_3_QUEST_1))->hideMinigameInterface();
            if( this->getLevelState( "sm_level_1_boss" ) == 2 )
                MANAGER_Screen->removeScreen(GET_RUNNING_SCENE, screen_id::MG_BOSS1);
            if( this->getLevelState( "sm_level_1_boss" ) == 3 )
                MANAGER_Screen->removeScreen(GET_RUNNING_SCENE, screen_id::MG_L1_3_QUEST_3);
        }
        return;
    }
    
    //soldiers minigame
    if( _screen == screen_id::MG_L1_3_QUEST_1 )
    {
        //
        this->hideGameObject( 1500 );
        this->hideGameObject( 1501 );
        this->hideGameObject( 1502 );
        this->hideGameObject( 1503 );
        //
        this->showGameObject( 1600 );
        this->showGameObject( 1601 );
        this->showGameObject( 1602 );
        this->showGameObject( 1603 );
        //
        
        MANAGER_Level->getCurrentLevel()->getEmbiantSource()->changeGainTo( 0.7, 2);
        MANAGER_Level->getCurrentLevel()->getBackMusicSource()->changeGainTo( 0.7, 2);
        
        MANAGER_Level->resumeLevelTimer();
        this->pHudScreen->showTimer();
        dynamic_cast<L1_3_Quest_1_MGScreen*>(MANAGER_Screen->getScreen(GET_RUNNING_SCENE, screen_id::MG_L1_3_QUEST_1))->hideMinigameInterface();
        this->saveLevelStatData();
        this->pHudScreen->showSticker();
        this->cameraFollowPlayer = true;
        //coins add
        this->localPlayerCoins += 3;
        this->localPlayerCoinsIntoPoints += 3;
    }
    //boss minigame
    if( _screen == screen_id::MG_BOSS1 )
    {
        MANAGER_Level->getCurrentLevel()->getEmbiantSource()->changeGainTo( 0.7, 2);
        MANAGER_Level->getCurrentLevel()->getBackMusicSource()->changeGainTo( 0.7, 2);
        this->setLevelState( "minigame_boss_level_1_boss_complete", 1 );
        this->saveLevelStatData();
        //
        this->pDialogScreen->loadDialogFromXml( 98 );
        //
        MANAGER_Screen->removeScreen(GET_RUNNING_SCENE, _screen);
        
        
        if(this->areaType != AREA_WAR)
        {
            this->backMusicSource->loadAudioFile(MUSIC_WAR);
            this->backMusicSource->loadSettings( MUSIC_WAR );
            this->backMusicSource->play();
            this->backMusicSource->setAssociate( AudioSourceAssociate::MUSIC );
            
            this->areaType = AREA_WAR;
        }
    }
    //horn minigame
    if( _screen == screen_id::MG_L1_3_QUEST_3 )
    {
        IAudioSource *aSource = MANAGER_Audio->createStaticSource(SOUND_BOSS_DEFEAT);
        if( aSource )
        {
            aSource->play();
            aSource->setAssociate(AudioSourceAssociate::SOUND);
        }
        
        this->pDialogScreen->loadDialogFromXml( 105 );
        MANAGER_Screen->removeScreen(GET_RUNNING_SCENE, screen_id::MG_L1_3_QUEST_3);
    }
}

//-----------------------------------------------------------------------------------------------------
void Level_1_boss::onDialogueEnd( int _id )
{
    if( _id == 95 )
    {
        // The bat leave boss fly
        GameObject* objBat = this->getGameObjectById( 1700 );
        
        if( objBat )
        {
            ai::AiNpcBatObject* aiBat = dynamic_cast<ai::AiNpcBatObject*>( objBat->getAi() );
            
            if( aiBat )
                aiBat->moveTo( TARGET_POS_BAT_LEAVE_BOSS, 20, [this](){ this->onBatPositionLeaveBoss(); } );
            
            this->showGameObject( 1700 );
        }
        //
    }
    
    if( _id == 91 )
    {
        this->setLevelState( "mg_1_level_1_boss_complete", 1 );
        //L1_3_Quest_1_MGScreen* guardsScreen = dynamic_cast< L1_3_Quest_1_MGScreen* >( MANAGER_Screen->getScreen( GET_RUNNING_SCENE, screen_id::MG_L1_3_QUEST_1 ) );
        //if( guardsScreen )
        //    guardsScreen->showHintHand();
    }
    //end dialogue after boss minigame
    if( _id == 98 )
    {
        MANAGER_Level->resumeLevelTimer();
        this->pHudScreen->showTimer();
        
        this->bossObject = this->getGameObjectById(1000);
        dynamic_cast<ai::AiBoss1Object*>(this->bossObject->getAi())->setStartBoss(true);
        
        ai::AiBoss1Object* pBossAi = dynamic_cast<ai::AiBoss1Object*>(bossObject->getAi());
        if( pBossAi )
            this->oldHpBoss = pBossAi->getHp();
        
        dynamic_cast<ai::AiMovablePlatformObject*>(this->getGameObjectById(100)->getAi())->setMovable(true);
        dynamic_cast<ai::AiMovablePlatformObject*>(this->getGameObjectById(101)->getAi())->setMovable(true);
        dynamic_cast<ai::AiMovablePlatformObject*>(this->getGameObjectById(102)->getAi())->setMovable(true);
        dynamic_cast<ai::AiMovablePlatformObject*>(this->getGameObjectById(103)->getAi())->setMovable(true);
        //dynamic_cast<ai::AiMovablePlatformObject*>(this->getGameObjectById(104)->getAi())->setMovable(true);
        
        //TEMP
        this->setLevelState( "start_boss_level_1_boss", 1 );
        this->pHudScreen->showSticker();
    }
    
    //Play castle music after kill boss
    if( _id == 100 )
    {
        this->backMusicSource->loadAudioFile(MUSIC_CASTLE);
        this->backMusicSource->loadSettings( MUSIC_CASTLE );
        this->backMusicSource->play();
        this->backMusicSource->changeGainTo( 0.7, 2.0f );
        this->backMusicSource->setAssociate( AudioSourceAssociate::MUSIC );
    }
    
    //horn minigame
    if( _id == 105 )
    {
        //level timer
        MANAGER_Level->stopLevelTimer();
        this->pHudScreen->hideTimer();
        
        this->pHudScreen->hideSticker();
        this->pPlayer->stop();
        
        //win animation
        //MANAGER_Memory->getTempData()->setLevelState( "level_complete", 1 );
        
        auto winAction = Sequence::create( DelayTime::create( 2.0f ),
                                           CallFunc::create( CC_CALLBACK_0( Level_1_boss::onVictory, this ) ), nullptr );
        GET_RUNNING_SCENE->runAction( winAction );
    }
}

//-----------------------------------------------------------------------------------------------------
void Level_1_boss::onSwitchArea(GameObject* _switcher)
{
    if(_switcher->getId() == 6000)
    {
        if(this->pPlayer->getGameObject()->getPosition().x < _switcher->getPosition().x)
        {
            if( this->areaType != AREA_CAVE )
            {
                this->pHudScreen->resetSticker();
                //
                this->areaType = AREA_CAVE;
                this->cameraAutoReplace = true;
                this->cameraAutoReplaceLeft = true;
                this->pGameCamera->moveCameraTo(Vec2(170, pGameCamera->getPositionY()), 0.3f, true );
                
                auto cameraActiomn = Sequence::create( DelayTime::create( 0.2f ),
                                                       CallFunc::create( CC_CALLBACK_0( Level_1_boss::followPlayerCameraY, this ) ), nullptr );
                GET_RUNNING_SCENE->runAction( cameraActiomn );
            }
        }
        else
        {
            if(this->areaType != AREA_CASTLE)
            {
                this->pHudScreen->resetSticker();
                //
                //
                this->areaType = AREA_CASTLE;
                this->cameraAutoReplace = true;
                this->cameraAutoReplaceRight = true;
                this->pGameCamera->moveCameraTo(Vec2(218.4, pGameCamera->getPositionY()), 0.3f, true );
                //
                auto cameraActiomn = Sequence::create( DelayTime::create( 0.2f ),
                                                       CallFunc::create( CC_CALLBACK_0( Level_1_boss::followPlayerCameraY, this ) ), nullptr );
                GET_RUNNING_SCENE->runAction( cameraActiomn );
            }
        }
    }
    if(_switcher->getId() == 6001)
    {
        if(this->pPlayer->getGameObject()->getPosition().x < _switcher->getPosition().x)
        {
            this->cameraFollowPlayerY = false;
            this->cameraAutoReplace = false;
        }
        else
        {
            this->cameraFollowPlayerY = true;
            this->cameraAutoReplace = false;
            this->cameraFollowPlayer = true;
        }
    }
    if(_switcher->getId() == 6002)
    {
        if(this->pPlayer->getGameObject()->getPosition().x < _switcher->getPosition().x)
        {
            this->cameraFollowPlayerY = true;
            this->cameraAutoReplace = false;
        }
        else
        {
            this->cameraFollowPlayerY = false;
            this->cameraAutoReplace = false;
            this->cameraFollowPlayer = true;
        }
    }
    if(_switcher->getId() == 6003)
    {
        if(this->pPlayer->getGameObject()->getPosition().x < _switcher->getPosition().x)
        {
            if(this->areaType != AREA_CAVE)
            {
                this->backMusicSource->loadAudioFile(MUSIC_CAVE);
                this->backMusicSource->loadSettings( MUSIC_CAVE );
                this->backMusicSource->play();
                this->backMusicSource->setAssociate( AudioSourceAssociate::MUSIC );
                
                this->areaType = AREA_CAVE;
            }
        }
        else
        {
            if(this->areaType != AREA_CASTLE)
            {
                this->backMusicSource->loadAudioFile(MUSIC_CASTLE);
                this->backMusicSource->loadSettings( MUSIC_CASTLE );
                this->backMusicSource->play();
                this->backMusicSource->setAssociate( AudioSourceAssociate::MUSIC );
                
                this->areaType = AREA_CASTLE;
            }
        }
    }
}

//-----------------------------------------------------------------------------------------------------
void Level_1_boss::onVictory()
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
        
        MANAGER_Memory->getTempData()->levelsProgressData[6]->gameLevelsProgress = level_state::LVL_STATE_COMPLETED;
        if( MANAGER_Memory->getTempData()->levelsProgressData[7]->gameLevelsProgress != level_state::LVL_STATE_COMPLETED )
            MANAGER_Memory->getTempData()->levelsProgressData[7]->gameLevelsProgress = level_state::LVL_STATE_DONATE_LOCK;
        MANAGER_Memory->getTempData()->setLevelStars( 6 );
        
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
void Level_1_boss::onDefeat()
{
    this->pHudScreen->hideSticker();
    if( this->getLevelState( "SINGLE_MINIGAME" ) == 1 )
        MANAGER_Scene->replaceScene( scene_id::MINIGAMES_SELECTION );
    else
        dynamic_cast<TryMoreScreen*>(MANAGER_Screen->getScreen(GET_RUNNING_SCENE, screen_id::TRY_MORE))->show();
}

//-----------------------------------------------------------------------------------------------------
void Level_1_boss::unShadow()
{
    if(shadowValue < 0)
        shadowValue = 0;
    else
        shadowValue -= 65.0f;
    this->preShadow->runAction(CCFadeTo::create(3.0f, shadowValue));
    this->afterShadow->runAction(CCFadeTo::create(3.0f, shadowValue));
}

//-----------------------------------------------------------------------------------------------------
void Level_1_boss::followPlayerCameraY()
{
    this->cameraFollowPlayerY = true;
    this->cameraAutoReplace = false;
    this->cameraAutoReplaceLeft = false;
    this->cameraAutoReplaceRight = false;
    this->pHudScreen->showSticker();
    
    // RAM - unloading
    this->showGameObject( 33001 );
    
    //SOLDIERS
    MANAGER_Screen->removeScreen(GET_RUNNING_SCENE, screen_id::MG_L1_3_QUEST_1);
    
    this->getGameObjectById( 1500 )->release();
    this->getGameObjectById( 1600 )->release();
    this->getGameObjectById( 1501 )->release();
    this->getGameObjectById( 1601 )->release();
    this->getGameObjectById( 1502 )->release();
    this->getGameObjectById( 1602 )->release();
    
    MANAGER_Screen->addScreen( GET_RUNNING_SCENE, screen_id::MG_BOSS1 );
    MANAGER_Screen->detachScreen( GET_RUNNING_SCENE, screen_id::MG_BOSS1 );
    MANAGER_Screen->getScreen( GET_RUNNING_SCENE, screen_id::MG_BOSS1 );
    IScreen* mgScreen2 = MANAGER_Screen->getScreen( GET_RUNNING_SCENE, screen_id::MG_BOSS1 );
    mgScreen2->setPosition( 7422, 250 );
    mgScreen2->setScale( this->pGameCamera->getDistance() );
    dynamic_cast<GameScene*>( GET_RUNNING_SCENE )->getForegroundLayer()->addChild( MANAGER_Screen->getScreen( GET_RUNNING_SCENE, screen_id::MG_BOSS1 ), 2090 );
    dynamic_cast<Boss1_MGScreen*>( MANAGER_Screen->getScreen(GET_RUNNING_SCENE, screen_id::MG_BOSS1) )->hideMinigameInterface();
}

//-----------------------------------------------------------------------------------------------------
void Level_1_boss::determinePlayerPosition()
{
    //player first positioning
    MANAGER_Memory->getTempData()->playerPosition = Vec2( 5, 31.6 );
    
    //player second positioning ( after restart level from trymore screen )
    if( this->getLevelState( "musicbox_1_level_1_boss_complete" ) == 1 )
        MANAGER_Memory->getTempData()->playerPosition = Vec2( 25, 13.8 );
    if( this->getLevelState( "musicbox_1_level_1_boss_complete" ) == 2 )
        MANAGER_Memory->getTempData()->playerPosition = Vec2( 25, 13.8 );
    if( this->getLevelState( "musicbox_2_level_1_boss_complete" ) == 1)
        MANAGER_Memory->getTempData()->playerPosition = Vec2( 67, 11 );
    if( this->getLevelState( "musicbox_2_level_1_boss_complete" ) == 2 )
        MANAGER_Memory->getTempData()->playerPosition = Vec2( 67, 11 );
    if( this->getLevelState( "mg_1_level_1_boss_complete" ) == 1 )
        MANAGER_Memory->getTempData()->playerPosition = Vec2( 161, 29.8 );
    if( this->getLevelState( "minigame_boss_level_1_boss_complete" ) == 1 )
        MANAGER_Memory->getTempData()->playerPosition = Vec2( 234, 10.1 );
    
    //set player position
    this->pPlayer->getGameObject()->setPosition( MANAGER_Memory->getTempData()->playerPosition );
    
    //add player gameobject to active obj list
    this->showGameObject( 777 );
    
    //Camera positionig according to player
    if( this->cameraFollowPlayer )
        this->pGameCamera->setPosition( pPlayer->getGameObject()->getPosition() );
}

//-----------------------------------------------------------------------------------------------------
void Level_1_boss::onCheckPoint( GameObject* _checkPoint )
{
    
}

//-----------------------------------------------------------------------------------------------------
void Level_1_boss::singleMinigameLogic()
{
    //player positioning, logic adding (screen with minigame)
    if( this->getLevelState( "sm_level_1_boss" ) == 1 )
    {
        MANAGER_Memory->getTempData()->playerPosition = Vec2( 160, 29 );
        //
        dynamic_cast<L1_3_Quest_1_MGScreen*>(MANAGER_Screen->getScreen(GET_RUNNING_SCENE, screen_id::MG_L1_3_QUEST_1))->showMinigameInterface();
        
        //positioning camera for minigame
        this->cameraFollowPlayer = false;
        //pGameCamera->moveCameraTo(Vec2(170, 26), 0);
        pGameCamera->setPosition(Vec2(170, 26));
        //
        this->pHudScreen->hideSticker();
    }
    if( this->getLevelState( "sm_level_1_boss" ) == 2 )
    {
        MANAGER_Memory->getTempData()->playerPosition = Vec2( 235, 13 );
        //
        MANAGER_Screen->addScreen(GET_RUNNING_SCENE, screen_id::MG_BOSS1);
        this->preShadow->runAction(CCFadeTo::create(3.0f, 0));
        this->afterShadow->runAction(CCFadeTo::create(3.0f, 0));
        //
        this->pHudScreen->hideSticker();
    }
    if( this->getLevelState( "sm_level_1_boss" ) == 3 )
    {
        MANAGER_Memory->getTempData()->playerPosition = Vec2( 364, 44 );
        //
        MANAGER_Screen->addScreen(GET_RUNNING_SCENE, screen_id::MG_L1_3_QUEST_3);
        this->preShadow->runAction(CCFadeTo::create(3.0f, 0));
        this->afterShadow->runAction(CCFadeTo::create(3.0f, 0));
        //
        this->pHudScreen->hideSticker();
    }
    //
    this->pPlayer->getGameObject()->setPosition( MANAGER_Memory->getTempData()->playerPosition );
    //add player gameobject to active obj list
    this->showGameObject( 777 );
}

//-----------------------------------------------------------------------------------------------------
void Level_1_boss::saveLevelStatData()
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
void Level_1_boss::onBatPositionSoldier()
{
    dynamic_cast< DialogScreen* > ( MANAGER_Screen->getScreen( GET_RUNNING_SCENE, screen_id::DIALOG ) )->loadDialogFromXml( 91 );
    dynamic_cast< L1_3_Quest_1_MGScreen* >( MANAGER_Screen->getScreen( GET_RUNNING_SCENE, screen_id::MG_L1_3_QUEST_1 ) )->showMinigameInterface();
}

//-----------------------------------------------------------------------------------------------------
void Level_1_boss::onBatPositionBlind()
{
    Boss1_MGScreen* pScreen = dynamic_cast< Boss1_MGScreen* >( MANAGER_Screen->getScreen( GET_RUNNING_SCENE, screen_id::MG_BOSS1 ) );
    if( pScreen )
        pScreen->showMinigameInterface();
    
    this->pDialogScreen->loadDialogFromXml( 95 );
}

//-----------------------------------------------------------------------------------------------------
void Level_1_boss::onBatPositionLeaveBoss()
{
    this->hideGameObject( 1700 );
}

//-----------------------------------------------------------------------------------------------------