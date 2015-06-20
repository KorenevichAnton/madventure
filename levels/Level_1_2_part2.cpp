//
//  Level_1_2_part2.cpp
//  MusicalAdventure
//
//  Created by madmin on 6/4/14.
//  Copyright (c) 2014 ampagames. All rights reserved.
//

#include "Level_1_2_part2.h"

#include "GameScene.h"

#include "HudScreen.h"

#include "GameObject.h"

#include "L1_2_Quest_7_MGScreen.h"
#include "L1_2_Quest_8_MGScreen.h"

#include "AiMovablePlatformObject.h"

#include "CompleteScreen.h"
#include "TryMoreScreen.h"
#include "DialogScreen.h"

using namespace std;

//-----------------------------------------------------------------------------------------------------
Level_1_2_part2::Level_1_2_part2() :
cameraFollowPlayer( true ),
firstEnter( true )
{
    level = level_id::LVL_1_2_PART2;
    nextLevel = level_id::LVL_1_3_PART1;

    
    this->levelGameWorldPath = new char[50];
    sprintf(levelGameWorldPath, "%s%s", PHYS_LANDSCAPES_PATH , "landscape1_2_2.xml");
    this->levelGameObjectsPath = new char[50];
    sprintf(levelGameObjectsPath, "%s%s", PHYS_OBJECTS_PATH , "objects1_2_2.xml");
}

//-----------------------------------------------------------------------------------------------------
Level_1_2_part2::~Level_1_2_part2()
{
}

//-----------------------------------------------------------------------------------------------------
Level_1_2_part2* Level_1_2_part2::create()
{
    Level_1_2_part2* pLevel_1_2_part2 = new Level_1_2_part2();
    if(pLevel_1_2_part2)
    {
        return pLevel_1_2_part2;
    }
    else
    {
        delete pLevel_1_2_part2;
        return NULL;
    }
}

//-----------------------------------------------------------------------------------------------------
bool Level_1_2_part2::init()
{
    //set using screens
    this->pDialogScreen = dynamic_cast< DialogScreen* > ( MANAGER_Screen->getScreen( GET_RUNNING_SCENE, screen_id::DIALOG ) );
    this->pHudScreen = dynamic_cast< HudScreen* >( MANAGER_Screen->getScreen( GET_RUNNING_SCENE, screen_id::HUD) );
    
    //minigames screens
    MANAGER_Screen->addScreen( GET_RUNNING_SCENE, screen_id::MG_L1_2_QUEST_7 );
    MANAGER_Screen->detachScreen( GET_RUNNING_SCENE, screen_id::MG_L1_2_QUEST_7 );
    IScreen* mgScreen1 = MANAGER_Screen->getScreen( GET_RUNNING_SCENE, screen_id::MG_L1_2_QUEST_7 );
    mgScreen1->setPosition( 5050, 640 );
    mgScreen1->setScale( ( this->pGameCamera->getDistance() ) );
    dynamic_cast< GameScene* >( GET_RUNNING_SCENE )->getForegroundLayer()->addChild( mgScreen1 , 10 );
    dynamic_cast<L1_2_Quest_7_MGScreen*>(MANAGER_Screen->getScreen(GET_RUNNING_SCENE, screen_id::MG_L1_2_QUEST_7))->hideMinigameInterface();
    //
    MANAGER_Screen->addScreen( GET_RUNNING_SCENE, screen_id::MG_L1_2_QUEST_8 );
    MANAGER_Screen->detachScreen( GET_RUNNING_SCENE, screen_id::MG_L1_2_QUEST_8 );
    IScreen* mgScreen2 = MANAGER_Screen->getScreen( GET_RUNNING_SCENE, screen_id::MG_L1_2_QUEST_8 );
    mgScreen2->setPosition( 8300, 650 );
    mgScreen2->setScale( ( this->pGameCamera->getDistance() ) );
    dynamic_cast< GameScene* >( GET_RUNNING_SCENE )->getForegroundLayer()->addChild( mgScreen2 , 10 );
    dynamic_cast<L1_2_Quest_8_MGScreen*>(MANAGER_Screen->getScreen(GET_RUNNING_SCENE, screen_id::MG_L1_2_QUEST_8))->hideMinigameInterface();
    
    this->aSource = MANAGER_Audio->createStaticSource();
    if( this->aSource )
        this->aSource->setAssociate( AudioSourceAssociate::SOUND );
    
    //location current area showing what music should play at the moment
    this->areaType = AREA_CAVE;
    
    //Logic on enter level
    if( this->getLevelState( "musicbox_1_level_1_2_2_complete" ) == 1 )
    {
        this->hideGameObject( 902 );
        this->showGameObject( 922 );
    }
    if( this->getLevelState( "musicbox_2_level_1_2_2_complete" ) == 1)
    {
        this->hideGameObject( 903 );
        this->showGameObject( 923 );
    }
    if( this->getLevelState( "musicbox_3_level_1_2_2_complete" ) == 1)
    {
        this->hideGameObject( 906 );
        this->showGameObject( 926 );
    }
    //
    if( this->getLevelState( "mg_1_level_1_2_2_complete" ) == 1 )
    {
        this->hideGameObject( 900 );
        dynamic_cast<L1_2_Quest_7_MGScreen*>(MANAGER_Screen->getScreen(GET_RUNNING_SCENE, screen_id::MG_L1_2_QUEST_7))->setWinPos( true );
    }
    
    if( this->getLevelState( "mg_2_level_1_2_2_complete" ) == 1 )
    {
        this->hideGameObject( 901 );
        dynamic_cast<L1_2_Quest_8_MGScreen*>(MANAGER_Screen->getScreen(GET_RUNNING_SCENE, screen_id::MG_L1_2_QUEST_8))->setWinPos( true );
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
    
    //set logic if started single minigame
    if( this->getLevelState( "SINGLE_MINIGAME" ) == 1)
        this->singleMinigameLogic();
    
    return true;
}

//-----------------------------------------------------------------------------------------------------
void Level_1_2_part2::onEnter()
{
    //level timer
    MANAGER_Level->resumeLevelTimer();
    
    //Player positioning
    //this->determinePlayerPosition();
    
    //on restart level from pause/defeat/win level loading
    this->restartLevel = this->level;
    
    //Add back music
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
    
    this->embiantSource->setGain( MAX_VOLUME_EMBIENT_MUSIC );
    this->backMusicSource->setGain( MAX_VOLUME_EMBIENT_MUSIC );

    
    //Camera positionig according to player
    //if( this->cameraFollowPlayer )
    //    pGameCamera->setPosition( pPlayer->getGameObject()->getPosition() );
    
    //level index for difficulty and complete screen
    MANAGER_Memory->getTempData()->currentLevelIndex = 4;
}

//-----------------------------------------------------------------------------------------------------
void Level_1_2_part2::onExit()
{
    if( MANAGER_Memory->getTempData()->getLevelState( "SINGLE_MINIGAME_RESTART" ) == 0 )
    {
        this->setLevelState( "SINGLE_MINIGAME", 0 );
        this->setLevelState( "sm_level_1_2_2", 0 );
    }
    else
        MANAGER_Memory->getTempData()->setLevelState( "SINGLE_MINIGAME_RESTART", 1 );
    
    MANAGER_Level->stopLevelTimer();
    this->uploadLevelStates();
}

//-----------------------------------------------------------------------------------------------------
void Level_1_2_part2::release()
{
    MANAGER_Screen->removeAllScreens(GET_RUNNING_SCENE);
    delete []this->levelGameWorldPath;
    delete []this->levelGameObjectsPath;
    delete this;
}

//-----------------------------------------------------------------------------------------------------
void Level_1_2_part2::update(float _dt)
{
    if(pGameWorld && pGameCamera)
    {
        if( this->firstEnter )
        {
            this->firstEnter = false;
        }
        //Following camera after player
        if( this->cameraFollowPlayer )
            if( this->getLevelState( "SINGLE_MINIGAME" ) == 0 )
                pGameCamera->moveCameraTo(pPlayer->getGameObject()->getPosition(), CAMERA_FOLLOW_SPEED);
    
        if( this->getLevelState( "level_1_2_2_movable_platform_wait" ) == 1)
        {
            if(this->getGameObjectById( 606 )->getPosition().y <= 16.6 )
            {
                this->embiantSource->changeGainTo( MIN_VOLUME_EMBIENT_MUSIC, 2.0f );
                this->backMusicSource->changeGainTo( MIN_VOLUME_EMBIENT_MUSIC, 2.0f );
                
                this->setLevelState( "level_1_2_2_movable_platform_wait", 2 );
                this->hideGameObject( 606 );
                this->showGameObject( 603 );
                this->pDialogScreen->loadDialogFromXml( 65 );
                
                MANAGER_Screen->addScreen( GET_RUNNING_SCENE, screen_id::MG_L1_2_QUEST_10 );
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
bool Level_1_2_part2::onGetCoin(GameObject* _coin, std::string _key, PhysTriggerCoinKind _kind)
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
void Level_1_2_part2::onPortal(GameObject *_portal)
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
            
            this->embiantSource->changeGainTo( MIN_VOLUME_EMBIENT_MUSIC, 2.0f );
            this->backMusicSource->changeGainTo( MIN_VOLUME_EMBIENT_MUSIC, 2.0f );
            
            auto defeatAction = Sequence::create( DelayTime::create( 2.0f ),
                                                  CallFunc::create( CC_CALLBACK_0( Level_1_2_part2::onDefeat, this ) ), nullptr );
            GET_RUNNING_SCENE->runAction( defeatAction );
            
            _portal->release();
        }
        
        //Location triggers-portals
        //music box 1
        if( _portal->getId() == 902 )
        {
            //level timer
            MANAGER_Level->stopLevelTimer();
            this->pHudScreen->hideTimer();
            
            this->saveLevelStatData();
            MANAGER_Level->replaceLevel( level_id::MG_1_2_2_MUSICBOX_3 );
        }
        //music box 2
        if( _portal->getId() == 903 )
        {
            //level timer
            MANAGER_Level->stopLevelTimer();
            this->pHudScreen->hideTimer();
            
            this->saveLevelStatData();
            MANAGER_Level->replaceLevel( level_id::MG_1_2_2_MUSICBOX_4 );
        }
        //music box 3
        if( _portal->getId() == 906)
        {
            //level timer
            MANAGER_Level->stopLevelTimer();
            this->pHudScreen->hideTimer();
            
            this->saveLevelStatData();
            MANAGER_Level->replaceLevel( level_id::MG_1_2_2_MUSICBOX_5 );
        }
        //minigame 1
        if( _portal->getId() == 900 )
        {
            MANAGER_Level->stopLevelTimer();
            this->pHudScreen->hideTimer();
            //level save/states
            this->setLevelState( "level_1_2_2_minigame_1", 1 );
            this->saveLevelStatData();
            
            this->pDialogScreen->loadDialogFromXml( 53 );
            this->pHudScreen->hideSticker();

            dynamic_cast<L1_2_Quest_7_MGScreen*>(MANAGER_Screen->getScreen(GET_RUNNING_SCENE, screen_id::MG_L1_2_QUEST_7))->showMinigameInterface();
            //positioning camera for minigame
            this->cameraFollowPlayer = false;
            pGameCamera->moveCameraTo(Vec2(174.0f, 32.0f), 0.01, true);

            _portal->release();
        }
        //minigame 2
        if( _portal->getId() == 901 )
        {
            MANAGER_Level->stopLevelTimer();
            this->pHudScreen->hideTimer();
            //level save/states
            this->setLevelState( "level_1_2_2_minigame_1", 1 );
            this->saveLevelStatData();
            
            this->pDialogScreen->loadDialogFromXml( 55 );
            this->pHudScreen->hideSticker();
            
            //decrease sound
            this->embiantSource->changeGainTo( MIN_VOLUME_EMBIENT_MUSIC, 2.0f );
            this->backMusicSource->changeGainTo( MIN_VOLUME_EMBIENT_MUSIC, 2.0f );
            //
            dynamic_cast<L1_2_Quest_8_MGScreen*>(MANAGER_Screen->getScreen(GET_RUNNING_SCENE, screen_id::MG_L1_2_QUEST_8))->showMinigameInterface();
            //positioning camera for minigame
            this->cameraFollowPlayer = false;
            pGameCamera->moveCameraTo(Vec2(275.0f, 28.0f), 0.01, true);
            
            _portal->release();
        }
        //minigames 3/4
        if( _portal->getId() == 905 )
        {
            MANAGER_Level->stopLevelTimer();
            this->pHudScreen->hideTimer();
            //decrease sound
            this->embiantSource->changeGainTo( MIN_VOLUME_EMBIENT_MUSIC, 2.0f );
            this->backMusicSource->changeGainTo( MIN_VOLUME_EMBIENT_MUSIC, 2.0f );
            
            this->pDialogScreen->loadDialogFromXml( 58 );
            this->pHudScreen->hideSticker();
            MANAGER_Screen->addScreen( GET_RUNNING_SCENE, screen_id::MG_L1_2_QUEST_9 );
            
            _portal->release();
        }
        if( _portal->getId() == 907 )
        {
            //looking for platform between minigmae 3 and 4,
            //when its get up - going down till starting minigame 4
            if( this->getLevelState( "mg_4_level_1_2_2_complete" ) != 1 )
                this->setLevelState("level_1_2_2_movable_platform_wait", 1 );
        }
        //go level 1_3_1
        if( _portal->getId() == 904 )
        {
            this->pPlayer->stop();
            this->pHudScreen->resetSticker();
            this->pHudScreen->hideSticker();
            
            //level timer
            MANAGER_Level->stopLevelTimer();
            
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
void Level_1_2_part2::onMinigameEnd(bool _isWin, screen_id _screen)
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
            if( this->getLevelState( "sm_level_1_2_2" ) == 3 )
            {
                MANAGER_Screen->removeScreen( GET_RUNNING_SCENE, screen_id::MG_L1_2_QUEST_9 );
            }
            dynamic_cast<L1_2_Quest_7_MGScreen*>(MANAGER_Screen->getScreen(GET_RUNNING_SCENE, screen_id::MG_L1_2_QUEST_7))->hideMinigameInterface();
            dynamic_cast<L1_2_Quest_8_MGScreen*>(MANAGER_Screen->getScreen(GET_RUNNING_SCENE, screen_id::MG_L1_2_QUEST_8))->hideMinigameInterface();
        }
        return;
    }
    
    if( _screen == screen_id::MG_L1_2_QUEST_7 )
    {
        
        this->pDialogScreen->loadDialogFromXml( 54 );
        dynamic_cast<L1_2_Quest_7_MGScreen*>(MANAGER_Screen->getScreen(GET_RUNNING_SCENE, screen_id::MG_L1_2_QUEST_7))->hideMinigameInterface();
        this->cameraFollowPlayer = true;
        
        this->setLevelState( "mg_1_level_1_2_2_complete", 1 );
        //coins add
        this->localPlayerCoins += 3;
        this->localPlayerCoinsIntoPoints += 3;
        //complete stat
        MANAGER_Memory->getTempData()->statBrokenMechs += 1;
    }
    if( _screen == screen_id::MG_L1_2_QUEST_8 )
    {
        //increase sound
        this->embiantSource->changeGainTo( MAX_VOLUME_EMBIENT_MUSIC, 2.0f );
        this->backMusicSource->changeGainTo( MAX_VOLUME_EMBIENT_MUSIC, 2.0f );
        
        this->pDialogScreen->loadDialogFromXml( 56 );
        dynamic_cast<L1_2_Quest_8_MGScreen*>(MANAGER_Screen->getScreen(GET_RUNNING_SCENE, screen_id::MG_L1_2_QUEST_8))->hideMinigameInterface();
        this->cameraFollowPlayer = true;
        //
        
        this->setLevelState( "mg_2_level_1_2_2_complete", 1 );
        //coins add
        this->localPlayerCoins += 3;
        this->localPlayerCoinsIntoPoints += 3;
        //complete stat
        MANAGER_Memory->getTempData()->statBrokenMechs += 1;
    }
    //minigame 3
    if( _screen == screen_id::MG_L1_2_QUEST_9 )
    {
        if( _isWin )
        {
            //increase sound
            this->embiantSource->changeGainTo( MAX_VOLUME_EMBIENT_MUSIC, 2.0f );
            this->backMusicSource->changeGainTo( MAX_VOLUME_EMBIENT_MUSIC, 2.0f );
            
            dynamic_cast<ai::AiMovablePlatformObject*>(this->getGameObjectById(606)->getAi())->setMovable(true);
            this->showGameObject( 907 );
            
            MANAGER_Screen->removeScreen( GET_RUNNING_SCENE, _screen );
            //
            this->setLevelState( "mg_3_level_1_2_2_complete", 1 );
            //coins add
            this->localPlayerCoins += 3;
            this->localPlayerCoinsIntoPoints += 3;
        }
        else
        {
            MANAGER_Screen->removeScreen( GET_RUNNING_SCENE, _screen );
            MANAGER_Screen->addScreen( GET_RUNNING_SCENE, screen_id::MG_L1_2_QUEST_9 );
        }
    }
    //minigame 4
    if( _screen == screen_id::MG_L1_2_QUEST_10 )
    {
        if( _isWin )
        {
            //increase sound
            this->embiantSource->changeGainTo( MAX_VOLUME_EMBIENT_MUSIC, 2.0f );
            this->backMusicSource->changeGainTo( MAX_VOLUME_EMBIENT_MUSIC, 2.0f );
            
            this->pDialogScreen->loadDialogFromXml( 67 );
            MANAGER_Screen->removeScreen(GET_RUNNING_SCENE, _screen);
            //
            this->setLevelState( "mg_4_level_1_2_2_complete", 1 );
            //coins add
            this->localPlayerCoins += 3;
            this->localPlayerCoinsIntoPoints += 3;
        }
        else
        {
            this->pDialogScreen->loadDialogFromXml( 66 );
            MANAGER_Screen->removeScreen(GET_RUNNING_SCENE, _screen);
        }
    }
}

//-----------------------------------------------------------------------------------------------------
void Level_1_2_part2::onDialogueEnd( int _id )
{
    //minigame 1 show hint screen
    if( _id == 53 )
        MANAGER_Screen->addScreen( GET_RUNNING_SCENE, screen_id::HINT_NOTES_DURATION );
    //MANAGER_Screen->addScreen( GET_RUNNING_SCENE, screen_id::HINT );
    
    //minigame 1 dialog end
    if( _id == 54 )
    {
        this->pHudScreen->showSticker();
        MANAGER_Level->resumeLevelTimer();
        this->pHudScreen->showTimer();
    }
    //minigame 2 dialog end
    if( _id == 56 )
    {
        this->pHudScreen->showSticker();
        MANAGER_Level->resumeLevelTimer();
        this->pHudScreen->showTimer();
    }
    //minigame 4 dialog end
    if( _id == 66 || _id == 67 )
        dynamic_cast<ai::AiMovablePlatformObject*>(this->getGameObjectById(603)->getAi())->setMovable(true);
}

//-----------------------------------------------------------------------------------------------------
void Level_1_2_part2::onVictory()
{
    if( this->getLevelState( "SINGLE_MINIGAME" ) == 1 )
        MANAGER_Scene->replaceScene( scene_id::MINIGAMES_SELECTION );
    else
    {
        this->saveLevelStatData();
        
        //ADDING CORRECT LOCATION RESTART
        MANAGER_Memory->getTempData()->locationCoinsIntoPoints = MANAGER_Memory->getTempData()->playerCoinsIntoPoints;
        
        MANAGER_Memory->getTempData()->vGetCoinIds.clear();
        MANAGER_Memory->getTempData()->vKilledMobsIds.clear();
        
        MANAGER_Memory->getTempData()->levelsProgressData[4]->gameLevelsProgress = level_state::LVL_STATE_COMPLETED;
        if( MANAGER_Memory->getTempData()->levelsProgressData[5]->gameLevelsProgress != level_state::LVL_STATE_COMPLETED )
            MANAGER_Memory->getTempData()->levelsProgressData[5]->gameLevelsProgress = level_state::LVL_STATE_DONATE_LOCK;
        MANAGER_Memory->getTempData()->setLevelStars( 4 );
        
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
void Level_1_2_part2::onDefeat()
{
    this->pHudScreen->hideSticker();
    if( this->getLevelState( "SINGLE_MINIGAME" ) == 1 )
        MANAGER_Scene->replaceScene( scene_id::MINIGAMES_SELECTION );
    else
        dynamic_cast<TryMoreScreen*>(MANAGER_Screen->getScreen(GET_RUNNING_SCENE, screen_id::TRY_MORE))->show();
}

//-----------------------------------------------------------------------------------------------------
void Level_1_2_part2::determinePlayerPosition()
{
    //player first positioning
    MANAGER_Memory->getTempData()->playerPosition = Vec2( 14, 7.7 );
    
    //player second positioning ( after restart level from trymore screen )
    if( this->getLevelState( "musicbox_1_level_1_2_2_complete" ) == 1 )
        MANAGER_Memory->getTempData()->playerPosition = Vec2( 96.7, 45.4 );
    if( this->getLevelState( "musicbox_1_level_1_2_2_complete" ) == 2 )
        MANAGER_Memory->getTempData()->playerPosition = Vec2( 96.7, 45.4 );
    
    //minigame 1 cp
    if( this->getLevelState( "mg_1_level_1_2_2_complete" ) == 1 )
        MANAGER_Memory->getTempData()->playerPosition = Vec2( 153, 25.7 );
    
    if( this->getLevelState( "musicbox_2_level_1_2_2_complete" ) == 1)
        MANAGER_Memory->getTempData()->playerPosition = Vec2( 205, 41.2 );
    if( this->getLevelState( "musicbox_2_level_1_2_2_complete" ) == 2 )
        MANAGER_Memory->getTempData()->playerPosition = Vec2( 205, 41.2 );
    if( this->getLevelState( "musicbox_3_level_1_2_2_complete" ) == 1)
        MANAGER_Memory->getTempData()->playerPosition = Vec2( 255, 6.2 );
    if( this->getLevelState( "musicbox_3_level_1_2_2_complete" ) == 2 )
        MANAGER_Memory->getTempData()->playerPosition = Vec2( 255, 6.2 );
    
    //minigame 2 cp
    if( this->getLevelState( "mg_2_level_1_2_2_complete" ) == 1 )
        MANAGER_Memory->getTempData()->playerPosition = Vec2( 253.5, 25.7 );
    
    //set player position
    this->pPlayer->getGameObject()->setPosition( MANAGER_Memory->getTempData()->playerPosition );

    //add player gameobject to active obj list
    this->showGameObject( 777 );
    
    if( this->cameraFollowPlayer )
        pGameCamera->setPosition( pPlayer->getGameObject()->getPosition() );
}

//-----------------------------------------------------------------------------------------------------
void Level_1_2_part2::onCheckPoint( GameObject* _checkPoint )
{
    
}

//-----------------------------------------------------------------------------------------------------
void Level_1_2_part2::singleMinigameLogic()
{
    //player positioning, logic adding (screen with minigame)
    if( this->getLevelState( "sm_level_1_2_2" ) == 1 )
    {
        MANAGER_Memory->getTempData()->playerPosition = Vec2( 153, 27 );
        //
        this->embiantSource->changeGainTo( MIN_VOLUME_EMBIENT_MUSIC, 2.0f );
        this->backMusicSource->changeGainTo( MIN_VOLUME_EMBIENT_MUSIC, 2.0f );
        //
        dynamic_cast<L1_2_Quest_7_MGScreen*>(MANAGER_Screen->getScreen(GET_RUNNING_SCENE, screen_id::MG_L1_2_QUEST_7))->showMinigameInterface();
        //positioning camera for minigame
        this->cameraFollowPlayer = false;
        //pGameCamera->moveCameraTo(Vec2(174.0f, 32.0f), 0);
        pGameCamera->setPosition(Vec2(174.0f, 32.0f));
        //
        this->pHudScreen->hideSticker();
    }
    if( this->getLevelState( "sm_level_1_2_2" ) == 2 )
    {
        MANAGER_Memory->getTempData()->playerPosition = Vec2( 257, 26 );
        //
        this->embiantSource->changeGainTo( MIN_VOLUME_EMBIENT_MUSIC, 2.0f );
        this->backMusicSource->changeGainTo( MIN_VOLUME_EMBIENT_MUSIC, 2.0f );
        //
        dynamic_cast<L1_2_Quest_8_MGScreen*>(MANAGER_Screen->getScreen(GET_RUNNING_SCENE, screen_id::MG_L1_2_QUEST_8))->showMinigameInterface();
        //positioning camera for minigame
        this->cameraFollowPlayer = false;
        //pGameCamera->moveCameraTo(Vec2(275.0f, 28.0f), 0);
        pGameCamera->setPosition(Vec2(275.0f, 28.0f));
        //
        this->pHudScreen->hideSticker();
    }
    if( this->getLevelState( "sm_level_1_2_2" ) == 3 )
    {
        MANAGER_Memory->getTempData()->playerPosition = Vec2( 312, 20 );
        //
        this->embiantSource->changeGainTo( MIN_VOLUME_EMBIENT_MUSIC, 2.0f );
        this->backMusicSource->changeGainTo( MIN_VOLUME_EMBIENT_MUSIC, 2.0f );
        
        MANAGER_Screen->addScreen( GET_RUNNING_SCENE, screen_id::MG_L1_2_QUEST_9 );
        //
        this->pHudScreen->hideSticker();
    }
    //
    this->pPlayer->getGameObject()->setPosition( MANAGER_Memory->getTempData()->playerPosition );
    //add player gameobject to active obj list
    this->showGameObject( 777 );
}

//-----------------------------------------------------------------------------------------------------
void Level_1_2_part2::saveLevelStatData()
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