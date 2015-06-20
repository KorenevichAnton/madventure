//
//  Level_1_2_part1.cpp
//  MusicalAdventure
//
//  Created by madmin on 6/4/14.
//  Copyright (c) 2014 ampagames. All rights reserved.
//

#include "Level_1_2_part1.h"

#include "GameScene.h"

#include "HudScreen.h"

#include "GameObject.h"

#include "AiMovablePlatformObject.h"

#include "CompleteScreen.h"
#include "TryMoreScreen.h"
#include "DialogScreen.h"

#include "GraphicAnimationPlayerObject.h"

using namespace cocos2d;
using namespace std;

//-----------------------------------------------------------------------------------------------------
Level_1_2_part1::Level_1_2_part1() :
startTimer(false),
atFirst(false),
lastPlatformPosition(Vec2::ZERO),
platformCounter(0),
stopQueue(false),
currentCoinType(COIN_NOTE_NONE),
currentCoin(nullptr),
canShowBL(false),
taskCoin( COIN_ROOM_CORRECT_QUARTER ),
cameraFollowPlayer(true),
pCompasScreen( nullptr )
{
    level = level_id::LVL_1_2_PART1;
    nextLevel = level_id::LVL_1_2_PART2;
    

    this->levelGameWorldPath = new char[50];
    sprintf(levelGameWorldPath, "%s%s", PHYS_LANDSCAPES_PATH , "landscape1_2_1.xml");
    this->levelGameObjectsPath = new char[50];
    sprintf(levelGameObjectsPath, "%s%s", PHYS_OBJECTS_PATH , "objects1_2_1.xml");
    
}

//-----------------------------------------------------------------------------------------------------
Level_1_2_part1::~Level_1_2_part1()
{
}

//-----------------------------------------------------------------------------------------------------
Level_1_2_part1* Level_1_2_part1::create()
{
    Level_1_2_part1* pLevel_1_2_part1 = new Level_1_2_part1();
    if(pLevel_1_2_part1)
    {
        return pLevel_1_2_part1;
    }
    else
    {
        delete pLevel_1_2_part1;
        return NULL;
    }
}

//-----------------------------------------------------------------------------------------------------
bool Level_1_2_part1::init()
{
    //set using screens
    this->pDialogScreen = dynamic_cast< DialogScreen* > ( MANAGER_Screen->getScreen( GET_RUNNING_SCENE, screen_id::DIALOG ) );
    this->pHudScreen = dynamic_cast< HudScreen* >( MANAGER_Screen->getScreen( GET_RUNNING_SCENE, screen_id::HUD) );
    
    MANAGER_Screen->addScreen(GET_RUNNING_SCENE, screen_id::COMPAS, 121);
    this->pCompasScreen = dynamic_cast< CompasScreen* >( MANAGER_Screen->getScreen( GET_RUNNING_SCENE, screen_id::COMPAS ) );
    this->pCompasScreen->hideTask();
    //
    //location current area showing what music should play at the moment
    this->areaType = AREA_CAVE;
    
    this->aSource = MANAGER_Audio->createStaticSource();
    if( this->aSource )
        this->aSource->setAssociate( AudioSourceAssociate::SOUND );
    
    //Logic on enter level
    if( this->getLevelState( "first_room_level_1_2_1" ) )
    {
        //first lift set position
        this->getGameObjectById( 105 )->setPosition( Vec2( 14, 115 ) );
    }
    if( this->getLevelState( "second_room_level_1_2_1" ) )
    {
        //second lift set position
        this->getGameObjectById( 198 )->setPosition( Vec2( 5, 174 ) );
    }
    if( this->getLevelState( "third_room_level_1_2_1" ) )
    {
        //third lift set position
        this->getGameObjectById( 199 )->setPosition( Vec2( 5, 233 ) );
    }
    if( this->getLevelState( "musicbox_1_level_1_2_1_complete" ) == 1 )
    {
        //on first musicbox victory
        this->showGameObject( 70 );
        this->showGameObject( 71 );
        this->showGameObject( 72 );
        //
        this->hideGameObject( 73 );
        this->hideGameObject( 74 );
        this->hideGameObject( 75 );
        //
        this->hideGameObject( 900 );
    }
    if( this->getLevelState( "musicbox_2_level_1_2_1_complete" ) == 1 )
    {
        //on second musicbox victory
        this->hideGameObject( 901 );
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
void Level_1_2_part1::onEnter()
{
    srand(time(NULL));

    //level timer
    MANAGER_Level->resumeLevelTimer();
    //
    
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
    
    //location current area showing what music should play at the moment
    
    //
    this->setLevelState( "level_1_2_1_death", 0 );
    
    //level index for difficulty and complete screen
    MANAGER_Memory->getTempData()->currentLevelIndex = 3;
}

//-----------------------------------------------------------------------------------------------------
void Level_1_2_part1::onExit()
{
    MANAGER_Level->stopLevelTimer();
    this->uploadLevelStates();
}

//-----------------------------------------------------------------------------------------------------
void Level_1_2_part1::release()
{
    MANAGER_Screen->removeAllScreens(GET_RUNNING_SCENE);
    delete []this->levelGameWorldPath;
    delete []this->levelGameObjectsPath;
    delete this;
}

//-----------------------------------------------------------------------------------------------------
void Level_1_2_part1::update( float _dt )
{
    //KOSTIL'
    if( pGameWorld && pGameCamera && MANAGER_Memory->getTempData()->playerHP != -1 )
    {
        //game camera following the player
        if( this->cameraFollowPlayer )
            if( this->getLevelState( "SINGLE_MINIGAME" ) == 0 )
                pGameCamera->moveCameraTo(pPlayer->getGameObject()->getPosition(), CAMERA_FOLLOW_SPEED);
        
        //platforms backlight
        if(!this->canShowBL)
        {
            this->hideGameObject(2030);
            this->hideGameObject(2031);
            this->hideGameObject(2032);
            this->hideGameObject(2033);
        }
        if(this->currentCoinType == COIN_NOTE_RIGHT && this->canShowBL)
        {
            this->showRightBacklight(currentCoin);
            
            //auto hideBLAction = CCSequence::create( CCDelayTime::create( 1.0f ),
            //                                       CCCallFunc::create( this, callfunc_selector( Level_1_2_part1::hideBackLight ) ), nullptr );
            //GET_RUNNING_SCENE->runAction( hideBLAction );
        }
        else if(this->currentCoinType == COIN_NOTE_WRONG && this->canShowBL)
        {
            this->showWrongBacklight(currentCoin);
            //auto hideBLAction = CCSequence::create( CCDelayTime::create( 0.5f ),
            //                                       CCCallFunc::create( this, callfunc_selector( Level_1_2_part1::hideBackLight ) ), nullptr );
            //GET_RUNNING_SCENE->runAction( hideBLAction );
        }
        //player hp checking
        if( this->pPlayer->isDead() )//MANAGER_Memory->getTempData()->playerHP == 0 )
        {
            
            //this->setLevelState( "level_1_2_1_death", 1 );
            this->pPlayer->stop();
            this->pHudScreen->resetSticker();
            //MANAGER_Memory->getTempData()->playerHP--;
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
bool Level_1_2_part1::onGetCoin(GameObject* _coin, std::string _key, PhysTriggerCoinKind _kind)
{
    if(pGameWorld && pGameCamera)
    {
        if(this->pPlayer->getGameObject()->getLocation() != game_object_location::GROUND)
        {
            return false;
        }
        
        if(!dynamic_cast<CompasScreen*>(MANAGER_Screen->getScreen(GET_RUNNING_SCENE, screen_id::COMPAS))->isActive())
            return false;
        
        if(this->lastPlatformPosition.x == _coin->getPosition().x && this->lastPlatformPosition.y == _coin->getPosition().y)
        {
            return false;
        }
        
        this->canShowBL = true;
        
        auto hideBLAction = Sequence::create( DelayTime::create( 1.0f ),
                                              CallFunc::create( CC_CALLBACK_0( Level_1_2_part1::hideBackLight, this ) ), nullptr );
        hideBLAction->setTag( 1 );
        GET_RUNNING_SCENE->runAction( hideBLAction );
        
        //reset task for last room with platforms
        // - seting state: 0 - when touched coin;
        //                 1 - when touched 911 portal ( reset portal );
        this->setLevelState( "level_1_2_1_platforms_reseter", 0 );
        
        this->lastPlatformPosition = _coin->getPosition();
        
        if(this->taskCoin == _kind)
        {
            this->currentCoin = _coin;
            this->currentCoinType = COIN_NOTE_RIGHT;
        }
        else
        {
            this->currentCoin = _coin;
            this->currentCoinType = COIN_NOTE_WRONG;
            
            this->pPlayer->stop();
            this->pHudScreen->resetSticker();
            
            MANAGER_Memory->getTempData()->playerHP-- ;
            if(MANAGER_Memory->getTempData()->playerHP > 0)
            {
                /*Play sound dmg*/
                if( this->aSource->loadAudioFile( "audio/player/gettingDmgSound.wav" ) )
                {
                    this->aSource->setAssociate( AudioSourceAssociate::SOUND );
                    this->aSource->play();
                }
                
                pPlayer->stop();
            }
            else
                this->pHudScreen->hideSticker();
        }
        
        if(_key == "full")
        {
            MANAGER_Memory->getTempData()->setLevelState( "level_1_2_1_room_task", 0 );
            this->taskCoin = COIN_ROOM_CORRECT_FULL;
            this->setNewPlatformTask(COIN_ROOM_CORRECT_FULL);
        }
        if(_key == "half")
        {
            MANAGER_Memory->getTempData()->setLevelState( "level_1_2_1_room_task", 1 );
            this->taskCoin = COIN_ROOM_CORRECT_HALF;
            this->setNewPlatformTask(COIN_ROOM_CORRECT_HALF);
        }
        if(_key == "quarter")
        {
            MANAGER_Memory->getTempData()->setLevelState( "level_1_2_1_room_task", 2 );
            this->taskCoin = COIN_ROOM_CORRECT_QUARTER;
            this->setNewPlatformTask(COIN_ROOM_CORRECT_QUARTER);
        }
        if(_key == "eighth")
        {
            MANAGER_Memory->getTempData()->setLevelState( "level_1_2_1_room_task", 3 );
            this->taskCoin = COIN_ROOM_CORRECT_EIGHTH;
            this->setNewPlatformTask(COIN_ROOM_CORRECT_EIGHTH);
        }
        
        //
        if( _coin->getId() == 50062 ||
           _coin->getId() == 50063 ||
           _coin->getId() == 50066 )
        {
            this->taskCoin = (rand() % 100 > 50) ? COIN_ROOM_CORRECT_FULL : COIN_ROOM_CORRECT_HALF;;
            this->setNewPlatformTask( this->taskCoin );
        }
    }
    return false;
}

//-----------------------------------------------------------------------------------------------------
void Level_1_2_part1::setNewPlatformTask(PhysTriggerCoinKind _coinKind)
{
    if(_coinKind == COIN_ROOM_CORRECT_FULL)
        pCompasScreen->setDirection(0);
    if(_coinKind == COIN_ROOM_CORRECT_HALF)
        pCompasScreen->setDirection(90);
    if(_coinKind == COIN_ROOM_CORRECT_QUARTER)
        pCompasScreen->setDirection(180);
    if(_coinKind == COIN_ROOM_CORRECT_EIGHTH)
        pCompasScreen->setDirection(270);
}

//-----------------------------------------------------------------------------------------------------
void Level_1_2_part1::hideBackLight()
{
    this->canShowBL = false;
}

//-----------------------------------------------------------------------------------------------------
void Level_1_2_part1::onHideCompas()
{
    this->pCompasScreen->hideTask();
}

//-----------------------------------------------------------------------------------------------------
void Level_1_2_part1::showRightBacklight(GameObject* _coin)
{
    //PLATFORM POSITION
    this->lastPlatformPosition = _coin->getPosition();
    this->getGameObjectById(2030)->setPosition(Vec2(_coin->getPosition().x, _coin->getPosition().y - 2));
    this->hideGameObject(2031);
    this->showGameObject(2030);
    //CLOUD POSITION
    if(_coin->getPosition().x > 60 || _coin->getPosition().y > 220)
    {
        this->getGameObjectById(2030)->setPosition(Vec2(_coin->getPosition().x, _coin->getPosition().y - 0.5));
        //
        this->getGameObjectById(2032)->setPosition(Vec2(_coin->getPosition().x, _coin->getPosition().y - 1.5));
        this->hideGameObject(2033);
        this->showGameObject(2032);
    }
}

//-----------------------------------------------------------------------------------------------------
void Level_1_2_part1::showWrongBacklight(GameObject *_coin)
{
    //PLATFORM POSITION
    //this->lastPlatformPosition = _coin->getPosition();
    this->getGameObjectById(2031)->setPosition(Vec2(_coin->getPosition().x, _coin->getPosition().y - 2));
    this->hideGameObject(2030);
    this->showGameObject(2031);
    //CLOUD POSITION
    if(_coin->getPosition().x > 60 || _coin->getPosition().y > 220)
    {
        this->getGameObjectById( 2031 )->setPosition( Vec2( _coin->getPosition().x, _coin->getPosition().y - 0.5 ));
        //
        this->getGameObjectById(2033)->setPosition(Vec2(_coin->getPosition().x, _coin->getPosition().y - 1.5 ));
        this->hideGameObject(2032);
        this->showGameObject(2033);
    }
}

//-----------------------------------------------------------------------------------------------------
void Level_1_2_part1::onPortal(GameObject *_portal)
{
    if(pGameWorld && pGameCamera)
    {
        //falling in abyss
        if( _portal->getId() >= 950 && _portal->getId() <= 964 )
        {
            this->pHudScreen->hideSticker();
            
            if( this->aSource->loadAudioFile( FALLING_SOUND ) )
                this->aSource->play();
            
            this->embiantSource->changeGainTo( MIN_VOLUME_EMBIENT_MUSIC, 2.0f );
            this->backMusicSource->changeGainTo( MIN_VOLUME_EMBIENT_MUSIC, 2.0f );
            
            auto defeatAction = Sequence::create( DelayTime::create( 2.0f ),
                                                    CallFunc::create( CC_CALLBACK_0( Level_1_2_part1::onDefeat, this ) ), nullptr );
            GET_RUNNING_SCENE->runAction( defeatAction );
            
            _portal->release();
        }
        
        if( _portal->getId() == 159 && MANAGER_Memory->getTempData()->playerHP > 0 )
        {
            this->pHudScreen->hideSticker();
            
            this->embiantSource->changeGainTo( MIN_VOLUME_EMBIENT_MUSIC, 2.0f );
            this->backMusicSource->changeGainTo( MIN_VOLUME_EMBIENT_MUSIC, 2.0f );
            
            //GAVNO?
            //auto defeatAction = CCSequence::create( CCDelayTime::create( 2.0f ),
            //                                       CCCallFunc::create( this, callfunc_selector( Level_1_2_part1::onDefeat ) ), nullptr );
            //GET_RUNNING_SCENE->runAction( defeatAction );
            
            MANAGER_Memory->getTempData()->playerHP = 0;
            
        }
        
        //Location triggers-portals
        //stop camera by falling in abyss
        if( _portal->getId() == 908 )
        {
            this->cameraFollowPlayer = false;
        }
        //on first hint, lift up
        if( _portal->getId() == 940 )
        {
            MANAGER_Level->stopLevelTimer();
            this->pHudScreen->hideSticker();
            this->pDialogScreen->loadDialogFromXml( 30 );
            _portal->release();
        }
        //first room
        if( _portal->getId() == 941 )
        {
            if( this->getLevelState( "first_room_level_1_2_1" ) != 1 )
            {
                this->setLevelState( "first_room_level_1_2_1", 1 );
                this->saveLevelStatData();
                this->pDialogScreen->loadDialogFromXml( 33 );
            }
            
            //platforms algo
            this->pCompasScreen->showTask();
            
            MANAGER_Memory->getTempData()->setLevelState( "level_1_2_1_room_task", 2 );
            this->taskCoin = COIN_ROOM_CORRECT_QUARTER;
            this->setNewPlatformTask(COIN_ROOM_CORRECT_QUARTER);
            this->lastPlatformPosition = Vec2::ZERO;
        }
        //on second lift up
        if( _portal->getId() == 197 )
        {
            dynamic_cast<ai::AiMovablePlatformObject*>(this->getGameObjectById(198)->getAi())->setMovable(true);
            this->pHudScreen->hideSticker();
        }
        //second room
        if( _portal->getId() == 942 )
        {
            if( this->getLevelState( "second_room_level_1_2_1" ) != 1 )
            {
                this->setLevelState( "second_room_level_1_2_1", 1 );
                this->saveLevelStatData();
                this->pHudScreen->showSticker();
            }
            //platforms algo
            this->pCompasScreen->showTask();
            MANAGER_Memory->getTempData()->setLevelState( "level_1_2_1_room_task", 0 );
            this->taskCoin = COIN_ROOM_CORRECT_FULL;
            this->setNewPlatformTask(COIN_ROOM_CORRECT_FULL);
            this->lastPlatformPosition = Vec2::ZERO;
        }
        //third room
        if( _portal->getId() == 949 )
        {
            dynamic_cast<ai::AiMovablePlatformObject*>(this->getGameObjectById(199)->getAi())->setMovable(true);
            this->pHudScreen->hideSticker();
        }
        //on third lift up
        if( _portal->getId() == 943 )
        {
            if( this->getLevelState( "third_room_level_1_2_1" ) != 1 )
            {
                this->setLevelState( "third_room_level_1_2_1", 1 );
                this->saveLevelStatData();
                this->pHudScreen->showSticker();
            }
            //platforms algo
            this->pCompasScreen->showTask();
            MANAGER_Memory->getTempData()->setLevelState( "level_1_2_1_room_task", 3 );
            this->taskCoin = COIN_ROOM_CORRECT_EIGHTH;
            this->setNewPlatformTask(COIN_ROOM_CORRECT_EIGHTH);
            this->lastPlatformPosition = Vec2::ZERO;
        }
        //after third room - hide platform task
        if( _portal->getId() == 910 )
        {
            this->pCompasScreen->hideTask();
        }
        //dialogue - first meeting with music-box
        if( _portal->getId() == 912 && this->getLevelState( "dialogue_musicbox_level_1_2_1" ) != 1 )
        {
            this->setLevelState( "dialogue_musicbox_level_1_2_1", 1 );
            this->pDialogScreen->loadDialogFromXml( 40 );
            this->pHudScreen->hideSticker();
        }
        //first musicbox
        if( _portal->getId() == 900 )
        {
            this->saveLevelStatData();
            MANAGER_Level->replaceLevel( level_id::MG_1_2_1_MUSICBOX_1 );
        }
        //second musicbox
        if( _portal->getId() == 901 )
        {
            //level timer
            MANAGER_Level->stopLevelTimer();
            this->pHudScreen->hideTimer();
            
            this->saveLevelStatData();
            MANAGER_Level->replaceLevel( level_id::MG_1_2_1_MUSICBOX_2 );
        }
        //on fourth room
        if( _portal->getId() == 944 )
        {
            this->lastPlatformPosition = Vec2::ZERO;
            
            MANAGER_Memory->getTempData()->setLevelState( "level_1_2_1_room_task", 0 );
            this->taskCoin = COIN_ROOM_CORRECT_FULL;
            this->setNewPlatformTask(COIN_ROOM_CORRECT_FULL);
            
            this->pCompasScreen->showTask();
            this->setLevelState( "fourth_room_level_1_2_1", 1 );
            this->saveLevelStatData();
        }
        //duration sum - change hint screen
        if( _portal->getId() == 915 )
        {
            MANAGER_Screen->addScreen( GET_RUNNING_SCENE, screen_id::SCREEN_HINT_NOTES_CALC );
            MANAGER_Memory->getTempData()->setLevelState( "1_2_1_second_hint", 1 );
            _portal->release();
        }
        //get random tasks on new platform's level
        if( _portal->getId() == 911 && this->getLevelState( "level_1_2_1_platforms_reseter" ) == 0 )
        {
            this->lastPlatformPosition = Vec2::ZERO;
            this->setLevelState( "level_1_2_1_platforms_reseter", 1 );
            this->taskCoin = (rand() % 100 > 50) ? COIN_ROOM_CORRECT_FULL : COIN_ROOM_CORRECT_HALF;
            this->setNewPlatformTask( this->taskCoin );
            //_portal->release();
        }
        if( _portal->getId() == 914 && this->getLevelState( "level_1_2_1_platforms_reseter" ) == 0 )
        {
            this->lastPlatformPosition = Vec2::ZERO;
            this->setLevelState( "level_1_2_1_platforms_reseter", 1 );
            this->taskCoin = (rand() % 100 > 50) ? COIN_ROOM_CORRECT_FULL : COIN_ROOM_CORRECT_HALF;;
            this->setNewPlatformTask( this->taskCoin );
            //_portal->release();
        }
        //go level 1_2_2
        if( _portal->getId() == 913 )
        {
            //level timer
            MANAGER_Level->stopLevelTimer();
            
            this->pCompasScreen->hideTask();
            this->pPlayer->stop();
            
            MANAGER_Memory->getTempData()->setLevelState( "level_complete", 1 );
            
            GameObject* pObj = this->getGameObjectById( 777 );
            if( pObj )
            {
                graphic::GraphicAnimationObject* anim = dynamic_cast<graphic::GraphicAnimationObject*>(pObj->getGraphic());
                
                if( anim )
                    anim->setCallbackEndAnimation( graphic::sequence_mob::SM_JOY, [this](){ this->onVictory(); } );
            }
            
            this->pHudScreen->hideSticker();
            _portal->release();
        }
        
        //Hide compas screen
        if( _portal->getId() == 916 )
        {
            auto compasHiderAction = Sequence::create( DelayTime::create( 1.0f ),
                                                         CallFunc::create( CC_CALLBACK_0( Level_1_2_part1::onHideCompas, this ) ), nullptr );
            GET_RUNNING_SCENE->runAction( compasHiderAction );
        }
    }
}

//-----------------------------------------------------------------------------------------------------
void Level_1_2_part1::onDialogueEnd( int _id )
{
    //on lift
    if( _id == 30 )
        this->pDialogScreen->loadDialogFromXml( 32 );
    
    //after first close first hint (lift)
    if( _id == 32 )
    {
        MANAGER_Screen->addScreen( GET_RUNNING_SCENE, screen_id::HINT );
        /* SET
        if( this->getLevelState( "first_lift_level_1_2_1" ) != 1 )
        {
            this->setLevelState( "first_lift_level_1_2_1", 1 );
            dynamic_cast<ai::AiMovablePlatformObject*>(this->getGameObjectById(105)->getAi())->setMovable(true);
        }
        else
        {
            this->pHudScreen->showSticker();
        }
         */
    }
    
    //on end first room dialogue
    if( _id == 0 )
    {
        if( this->getLevelState( "first_lift_level_1_2_1" ) != 1 )
        {
            this->setLevelState( "first_lift_level_1_2_1", 1 );
            dynamic_cast<ai::AiMovablePlatformObject*>(this->getGameObjectById(105)->getAi())->setMovable(true);
        }
        else
        {
            this->pHudScreen->showSticker();
        }
        MANAGER_Level->resumeLevelTimer();
        //this->pHudScreen->showSticker();
    }
    
    //on end dialogue before first musicbox
    if( _id == 40 )
        this->pHudScreen->showSticker();
    
    //adding hint screen with notes after dialogue
    if( _id == 33 )
    {
        // show first hint
        this->pHudScreen->showHandHint( Vec2( 520, 610 ), 0, 1.5f, 0.4f );
        
        // show second hint with delay
        auto secondHintAction = Sequence::create( DelayTime::create( 2.0f ),
                                                    CallFunc::create( CC_CALLBACK_0( Level_1_2_part1::showSecondHint, this ) ), nullptr );
        GET_RUNNING_SCENE->runAction( secondHintAction );
        
        // show third hint with delay
        auto thirdHintAction = Sequence::create( DelayTime::create( 4.0f ),
                                                   CallFunc::create( CC_CALLBACK_0( Level_1_2_part1::showThirdHint, this ) ) , nullptr );
        GET_RUNNING_SCENE->runAction( thirdHintAction );
        
        this->pHudScreen->showSticker();
    }
}

//-----------------------------------------------------------------------------------------------------
void Level_1_2_part1::showSecondHint()
{
    this->pHudScreen->showHandHint( Vec2( 820, 600 ), 0, 1.5f, 1.0f );
}

//-----------------------------------------------------------------------------------------------------
void Level_1_2_part1::showThirdHint()
{
    //this->pHudScreen->showHandHint( Vec2( 470, 520 ), 90, 1.5f );
    auto handTip = TipHand::create( 90, 1.5f, 1.0f );
    handTip->setPosition( Vec2( 21, 124 ) * CELL_SIZE );
    handTip->setScale( this->pGameCamera->getDistance() );
    dynamic_cast< GameScene* >( GET_RUNNING_SCENE )->getForegroundLayer()->addChild( handTip, 2002 );
    handTip->start();
}

//-----------------------------------------------------------------------------------------------------
void Level_1_2_part1::onVictory()
{
    dynamic_cast<HudScreen*>(MANAGER_Screen->getScreen(GET_RUNNING_SCENE, screen_id::HUD))->hideSticker();
    this->saveLevelStatData();
    
    //ADDING CORRECT LOCATION RESTART
    MANAGER_Memory->getTempData()->locationCoinsIntoPoints = MANAGER_Memory->getTempData()->playerCoinsIntoPoints;
    
    MANAGER_Memory->getTempData()->vGetCoinIds.clear();
    MANAGER_Memory->getTempData()->vKilledMobsIds.clear();
    
    MANAGER_Memory->getTempData()->levelsProgressData[3]->gameLevelsProgress = level_state::LVL_STATE_COMPLETED;
    if( MANAGER_Memory->getTempData()->levelsProgressData[4]->gameLevelsProgress != level_state::LVL_STATE_COMPLETED )
        MANAGER_Memory->getTempData()->levelsProgressData[4]->gameLevelsProgress = level_state::LVL_STATE_DONATE_LOCK;
    MANAGER_Memory->getTempData()->setLevelStars( 3 );
    
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
void Level_1_2_part1::onDefeat()
{
    dynamic_cast<HudScreen*>(MANAGER_Screen->getScreen(GET_RUNNING_SCENE, screen_id::HUD))->hideSticker();
    dynamic_cast<TryMoreScreen*>(MANAGER_Screen->getScreen(GET_RUNNING_SCENE, screen_id::TRY_MORE))->show();
}

//-----------------------------------------------------------------------------------------------------
void Level_1_2_part1::determinePlayerPosition()
{
    //player first positioning
    MANAGER_Memory->getTempData()->playerPosition = Vec2( 5, 80.2 );
    
    //player second positioning ( after restart level from trymore screen )
    if( this->getLevelState( "first_room_level_1_2_1" ) == 1 )
        MANAGER_Memory->getTempData()->playerPosition = Vec2( 19, 120.2 );
    if( this->getLevelState( "second_room_level_1_2_1" ) == 1 )
        MANAGER_Memory->getTempData()->playerPosition = Vec2( 19, 178 );
    if( this->getLevelState( "third_room_level_1_2_1" ) == 1 )
        MANAGER_Memory->getTempData()->playerPosition = Vec2( 8.5, 234.7 );
    if( this->getLevelState( "musicbox_1_level_1_2_1_complete" ) == 2 ) // mbox 1 defeat
        MANAGER_Memory->getTempData()->playerPosition = Vec2( 90, 215.2 );
    if( this->getLevelState( "musicbox_1_level_1_2_1_complete" ) == 1 ) // mbox 1 victory
        MANAGER_Memory->getTempData()->playerPosition = Vec2( 73, 199.3 );
    if( this->getLevelState( "musicbox_2_level_1_2_1_complete" ) == 2 ) // mbox 2 defeat
        MANAGER_Memory->getTempData()->playerPosition = Vec2( 96, 198 );
    if( this->getLevelState( "musicbox_2_level_1_2_1_complete" ) == 1 ) // mbox 2 victory
        MANAGER_Memory->getTempData()->playerPosition = Vec2( 105, 197.8 );
    if( this->getLevelState( "fourth_room_level_1_2_1" ) == 1 )
        MANAGER_Memory->getTempData()->playerPosition = Vec2( 107, 169.7 );
        
    //set player position
    this->pPlayer->getGameObject()->setPosition( MANAGER_Memory->getTempData()->playerPosition );
    
    //add player gameobject to active obj list
    this->showGameObject( 777 );
    
    //Camera positionig according to player
    this->cameraFollowPlayer = true;
    pGameCamera->setPosition( pPlayer->getGameObject()->getPosition() );

}

//-----------------------------------------------------------------------------------------------------
void Level_1_2_part1::onCheckPoint( GameObject* _checkPoint )
{
    
}

//-----------------------------------------------------------------------------------------------------
void Level_1_2_part1::saveLevelStatData()
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