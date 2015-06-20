//
//  Level_1_1_part1.cpp
//  MusicalAdventure
//
//  Created by madmin on 6/2/14.
//  Copyright (c) 2014 ampagames. All rights reserved.
//

#include "Level_1_1_part1.h"

#include "HudScreen.h"

#include "GameObject.h"

#include "PhysTriggerCoinObject.h"

#include "CompleteScreen.h"
#include "TryMoreScreen.h"
#include "DialogScreen.h"
#include "GraphicAnimationObject.h"

#include "L1_1_Quest_2_3_MGScreen.h"
#include "L1_1_Quest_1_4_MGScreen.h"

using namespace std;

//-----------------------------------------------------------------------------------------------------
Level_1_1_part1::Level_1_1_part1() :
pNotesMinigameScreen( nullptr ),
mgSevenCoinsCounter( 0 )
{
    pNotesMinigameScreen = NULL;
    
    level = level_id::LVL_1_1_PART1;
    nextLevel = level_id::LVL_1_1_PART2;
    
    this->levelGameWorldPath = new char[50];
    sprintf(levelGameWorldPath, "%s%s", PHYS_LANDSCAPES_PATH, "landscape1_1_1.xml");
    this->levelGameObjectsPath = new char[50];
    sprintf(levelGameObjectsPath, "%s%s", PHYS_OBJECTS_PATH, "objects1_1_1.xml");
}

//-----------------------------------------------------------------------------------------------------
Level_1_1_part1::~Level_1_1_part1()
{
}

//-----------------------------------------------------------------------------------------------------
Level_1_1_part1* Level_1_1_part1::create()
{
    Level_1_1_part1* pLevel_1_1_part1 = new Level_1_1_part1();
    if(pLevel_1_1_part1)
    {
        return pLevel_1_1_part1;
    }
    else
    {
        delete pLevel_1_1_part1;
        return NULL;
    }
}

//-----------------------------------------------------------------------------------------------------
bool Level_1_1_part1::init()
{
    //set using screens
    this->pDialogScreen = dynamic_cast< DialogScreen* > ( MANAGER_Screen->getScreen( GET_RUNNING_SCENE, screen_id::DIALOG ) );
    this->pHudScreen = dynamic_cast< HudScreen* >( MANAGER_Screen->getScreen( GET_RUNNING_SCENE, screen_id::HUD) );
    
    //Logic on enter level
    if( this->getLevelState( "treble_clef_mg_complete" ) == 1 )
    {
        this->hideGameObject( 907 );
        this->hideGameObject( 908 );
        this->showGameObject( 1005 );
        
        // Bridge down
        GameObject* obj = this->getGameObjectById( 1002 );
        if( obj )
        {
            graphic::GraphicAnimationObject* anim = dynamic_cast<graphic::GraphicAnimationObject*>( obj->getGraphic() );
            anim->setState( graphic::sequence_mob::SM_WAIT, false );
            anim->setViewPart( -1 );
            anim->setCurAnimFrameIndex( anim->getLastAnimIndex() );
        }
    }
    if( this->getLevelState( "treble_clef_mg_complete" ) != 1 && this->getLevelState( "treble_clef_mg" ) == 1 )
    {
        MANAGER_Level->stopLevelTimer();
        this->pHudScreen->hideTimer();
    }
    
    //hide used check-points
    if( this->getLevelState( "treble_clef_mg" ) == 1 )
    {
        this->hideGameObject( 970 );
        //ANIMATION OLDMAN
        GameObject* obj = this->getGameObjectById( 2000 );
        if( obj )
        {
            graphic::GraphicAnimationObject* anim = dynamic_cast<graphic::GraphicAnimationObject*>( obj->getGraphic() );
            anim->setState( graphic::sequence_mob::SM_WAIT );
        }
    }
    
    //checkpoint with seven notes minigame
    if( this->getLevelState( "seven_notsMiniGame_end" ) == 1 )
        this->hideGameObject( 971 );
    if( this->getLevelState( "treble_clef_mg_complete" ) == 1 )
        this->hideGameObject( 972 );
    if( this->getLevelState( "gates_minigames" ) == 1 )
    {
        this->hideGameObject( 973 );
        this->showGameObject( 1500 );
        this->hideGameObject( 1001 );
    }
    
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
void Level_1_1_part1::onEnter()
{
    //level timer
    MANAGER_Level->resumeLevelTimer();
    
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
    
    this->embiantSource->setGain( MAX_VOLUME_EMBIENT_BIRDS );//MAX_VOLUME_EMBIENT_MUSIC );
    this->backMusicSource->setGain( MAX_VOLUME_EMBIENT_MUSIC );
    
    this->aSource = MANAGER_Audio->createStaticSource();
    if( this->aSource )
        this->aSource->setAssociate( AudioSourceAssociate::SOUND );
    
    //ANIMATION OLDMAN
    GameObject* obj = this->getGameObjectById( 2000 );
    if( obj && this->getLevelState( "treble_clef_mg_complete" ) == 0 )
    {
        graphic::GraphicAnimationObject* anim = dynamic_cast<graphic::GraphicAnimationObject*>( obj->getGraphic() );
        anim->setState( graphic::sequence_mob::SM_ATTACK );
    }

    //location current area showing what music should play at the moment
    this->areaType = AREA_AIR_PLATFORMS;
    
    //level index for difficulty and complete screen
    MANAGER_Memory->getTempData()->currentLevelIndex = 0;
    
    MANAGER_Screen->loadScreen( GET_RUNNING_SCENE, screen_id::MG_L1_1_QUEST_1_1 );
    
    // Hint 0
    if( !this->getLevelState( "level_1_1_1_shown_hint_0" ) )
    {
        this->setLevelState( "level_1_1_1_shown_hint_0", 1 );
        MANAGER_Memory->getTempData()->setLevelState( "level_1_1_1_shown_hint_0", 1 );
        
        this->pHudScreen->showHandHint( Vec2( 100, 120 ), 90, -1, 0.4f );
    }
    else
    {
        MANAGER_Memory->getTempData()->setLevelState( "level_1_1_1_shown_hint_0", 0 );
    }
}

//-----------------------------------------------------------------------------------------------------
void Level_1_1_part1::onExit()
{
    if( MANAGER_Memory->getTempData()->getLevelState( "SINGLE_MINIGAME_RESTART" ) == 0 )
    {
        this->setLevelState( "SINGLE_MINIGAME", 0 );
        this->setLevelState( "sm_level_1_1_1", 0 );
    }
    else
        MANAGER_Memory->getTempData()->setLevelState( "SINGLE_MINIGAME_RESTART", 1 );
    
    MANAGER_Level->stopLevelTimer();
    
    this->uploadLevelStates();
}

//-----------------------------------------------------------------------------------------------------
void Level_1_1_part1::release()
{
    MANAGER_Screen->removeAllScreens(GET_RUNNING_SCENE);
    delete []this->levelGameWorldPath;
    delete []this->levelGameObjectsPath;
    delete this;
}

//-----------------------------------------------------------------------------------------------------
void Level_1_1_part1::update( float _dt )
{
    if(pGameWorld && pGameCamera)
    {
        //following camera after player
        if( this->getLevelState( "SINGLE_MINIGAME" ) == 0 )
            pGameCamera->moveCameraTo( pPlayer->getGameObject()->getPosition(), cameraSpeedCalc() );
        
        //run next level
        if( this->getLevelState( "go_next_level" ) == 1 )
        {
            this->setLevelState( "go_next_level", 0 );
            MANAGER_Level->replaceLevel( this->nextLevel );
        }
    }
}

//-----------------------------------------------------------------------------------------------------
bool Level_1_1_part1::onGetCoin( GameObject* _coin, std::string _key, PhysTriggerCoinKind _kind )
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
    
    //
    if( _coin->getId() == 800 )
    {
        if( this->getLevelState( "treble_clef_mg" ) != 1 )
            this->pHudScreen->showHandHint( Vec2( 80, 550 ), 0, 2, 0.4f );
    }
    
    //minigame with seven coins 
    if( _kind == COIN_TASK )
    {
        if( _key == "noteC" )
        {
            this->mgSevenCoinsCounter++;
            pNotesMinigameScreen->showNote1Border();
        }
        else if( _key == "noteD" )
        {
            this->mgSevenCoinsCounter++;
            pNotesMinigameScreen->showNote2Border();
        }
        else if( _key == "noteE" )
        {
            this->mgSevenCoinsCounter++;
            pNotesMinigameScreen->showNote3Border();
        }
        else if( _key == "noteF" )
        {
            this->mgSevenCoinsCounter++;
            pNotesMinigameScreen->showNote4Border();
        }
        else if( _key == "noteG" )
        {
            this->mgSevenCoinsCounter++;
            pNotesMinigameScreen->showNote5Border();
        }
        else if( _key == "noteA" )
        {
            this->mgSevenCoinsCounter++;
            pNotesMinigameScreen->showNote6Border();
        }
        else if( _key == "noteH" )
        {
            this->mgSevenCoinsCounter++;
            pNotesMinigameScreen->showNote7Border();
            
            //SINGLE MINIGAME MODE
            if( this->getLevelState( "SINGLE_MINIGAME" ) == 1 )
            {
                GameObject* pObj = this->getGameObjectById( 777 );
                if( pObj )
                {
                    graphic::GraphicAnimationObject* anim = dynamic_cast<graphic::GraphicAnimationObject*>(pObj->getGraphic());
                    
                    if( anim )
                    {
                        MANAGER_Memory->getTempData()->setLevelState( "level_complete", 1 );
                        anim->setCallbackEndAnimation( graphic::sequence_mob::SM_JOY, [this](){ this->onVictory(); } );
                        this->pHudScreen->hideSticker();
                    }
                }
            }
        }
        
        if( this->getLevelState( "seven_notsMiniGame_end" ) == 0 &&
           this->mgSevenCoinsCounter == 7 )
        {
            this->setLevelState( "seven_notsMiniGame_end", 1 );
        }
    }
    return true;
}

//-----------------------------------------------------------------------------------------------------
void Level_1_1_part1::onPortal(GameObject *_portal)
{
    //SINGLE MINIGAME MODE
    if( pGameCamera && pGameWorld )
    {
        if( this->getLevelState( "SINGLE_MINIGAME" ) == 1 && _portal->getId() >= 950 && _portal->getId() <= 964 )
        {
            GameObject* pObj = this->getGameObjectById( 777 );
            if( pObj )
            {
                graphic::GraphicAnimationObject* anim = dynamic_cast<graphic::GraphicAnimationObject*>(pObj->getGraphic());
                
                if( anim )
                {
                    //
                    this->pHudScreen->hideSticker();
                    
                    IAudioSource *aSource = MANAGER_Audio->createStaticSource(FALLING_SOUND);
                    if(aSource)
                    {
                        aSource->play();
                        aSource->setAssociate(AudioSourceAssociate::SOUND);
                    }
                    
                    this->embiantSource->changeGainTo( MIN_VOLUME_EMBIENT_MUSIC, 2 );
                    this->backMusicSource->changeGainTo( MIN_VOLUME_EMBIENT_MUSIC, 2 );
                    //
                    auto defeatAction = Sequence::create( DelayTime::create( 2.0f ),
                                                          CallFunc::create( CC_CALLBACK_0( Level_1_1_part1::onDefeat, this ) ), nullptr );
                    GET_RUNNING_SCENE->runAction( defeatAction );
                    
                    _portal->release();
                }
            }
        }
    }
    //
    if(pGameCamera && pGameWorld && this->getLevelState( "SINGLE_MINIGAME" ) != 1 )
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
                                                  CallFunc::create( CC_CALLBACK_0( Level_1_1_part1::onDefeat, this ) ), nullptr );
            GET_RUNNING_SCENE->runAction( defeatAction );

            _portal->release();
        }
        
        //Location triggers-portals
        if( _portal->getId() == 900 )
        {
            if( this->getLevelState( "treble_clef_mg_complete" ) != 1 && this->getLevelState( "seven_notsMiniGame_end" ) != 1 ) //this->getLevelState( "treble_clef_mg" ) != 1 )
            {
                MANAGER_Level->stopLevelTimer();
                this->pHudScreen->hideTimer();
                
                this->pHudScreen->hideSticker();
                this->setLevelState( "treble_clef_mg", 1 );
                this->pDialogScreen->loadDialogFromXml( 0 );
                _portal->release();
            }
        }
        if( _portal->getId() == 901 )
        {
            if( this->getLevelState( "seven_notsMiniGame_end" ) == 1 && this->getLevelState( "treble_clef_mg_complete" ) != 1 )
            {
                this->embiantSource->changeGainTo( MIN_VOLUME_EMBIENT_MUSIC, 2 );
                this->backMusicSource->changeGainTo( MIN_VOLUME_EMBIENT_MUSIC, 2 );
                
                this->pHudScreen->hideSticker();
                MANAGER_Screen->removeScreen( GET_RUNNING_SCENE, screen_id::NOTESMINIGAME );
                this->pDialogScreen->loadDialogFromXml( 5 );
                MANAGER_Screen->addScreen( GET_RUNNING_SCENE, screen_id::MG_L1_1_QUEST_1_3 );
                _portal->release();
            }
        }
        if( _portal->getId() == 902 )
        {
            if( this->getLevelState( "gates_minigames" ) != 1 )
            {
                MANAGER_Level->stopLevelTimer();
                this->pHudScreen->hideTimer();
                
                this->pHudScreen->hideSticker();
                this->setLevelState( "gates_minigames", 1 );
                MANAGER_Screen->addScreen( GET_RUNNING_SCENE, screen_id::MG_L1_1_QUEST_2_1 );
                this->pDialogScreen->loadDialogFromXml( 10 );
            }
        }
        if( _portal->getId() == 906 )
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
        if( _portal->getId() == 909 )
        {
            this->getGameObjectById( 1000 )->getPhysic()->setActivity( false );
        }
    }
}
    
//-----------------------------------------------------------------------------------------------------
void Level_1_1_part1::onMinigameEnd(bool _isWin, screen_id _screen)
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
        }
        MANAGER_Screen->removeScreen( GET_RUNNING_SCENE, _screen );
        return;
    }
    
    //Bridge minigames
    if( _screen == screen_id::MG_L1_1_QUEST_1_1 )
    {
        MANAGER_Screen->removeScreen( GET_RUNNING_SCENE, screen_id::MG_L1_1_QUEST_1_1 );
        this->pDialogScreen->loadDialogFromXml( 4 );
        this->pNotesMinigameScreen->showHint();
        this->pNotesMinigameScreen->getNotebook()->showTrebleClef();
        //coins add
        this->localPlayerCoins += 1;
        this->localPlayerCoinsIntoPoints += 1;
    }
    if( _screen == screen_id::MG_L1_1_QUEST_1_3 )
    {
        MANAGER_Screen->removeScreen( GET_RUNNING_SCENE, screen_id::MG_L1_1_QUEST_1_3 );
        MANAGER_Screen->addScreen( GET_RUNNING_SCENE, screen_id::MG_L1_1_QUEST_2_3 );
        this->pDialogScreen->loadDialogFromXml( 6 );
        //coins add
        this->localPlayerCoins += 1;
        this->localPlayerCoinsIntoPoints += 1;
    }
    if( _screen == screen_id::MG_L1_1_QUEST_2_3 )
    {
        this->embiantSource->changeGainTo( MAX_VOLUME_EMBIENT_BIRDS, 2 );//MAX_VOLUME_EMBIENT_MUSIC, 2 );
        this->backMusicSource->changeGainTo( MAX_VOLUME_EMBIENT_MUSIC, 2 );
        
        MANAGER_Screen->removeScreen( GET_RUNNING_SCENE, screen_id::MG_L1_1_QUEST_2_3 );
        this->pDialogScreen->loadDialogFromXml( 8 );
        //hide blocking wall before bridge
        this->hideGameObject( 907 );
        this->hideGameObject( 908 );
        this->showGameObject( 1005 );
        this->setLevelState( "treble_clef_mg_complete", 1 );
        //ANIMATION BRIDGE
        ///coins add
        this->localPlayerCoins += 1;
        this->localPlayerCoinsIntoPoints += 1;
    }
    
    //Gates minigames
    if( _screen == screen_id::MG_L1_1_QUEST_2_1 )
    {
        MANAGER_Screen->removeScreen( GET_RUNNING_SCENE, screen_id::MG_L1_1_QUEST_2_1 );
        MANAGER_Screen->addScreen( GET_RUNNING_SCENE, screen_id::MG_L1_1_QUEST_2_2 );
        this->pDialogScreen->loadDialogFromXml( 12 );
    }
    if( _screen == screen_id::MG_L1_1_QUEST_2_2 )
    {
        MANAGER_Screen->removeScreen( GET_RUNNING_SCENE, screen_id::MG_L1_1_QUEST_2_2 );
        MANAGER_Screen->addScreen( GET_RUNNING_SCENE, screen_id::MG_L1_1_QUEST_1_4 );
        this->pDialogScreen->loadDialogFromXml( 13 );
    }
    if( _screen == screen_id::MG_L1_1_QUEST_1_4 )
    {
        MANAGER_Screen->removeScreen( GET_RUNNING_SCENE, screen_id::MG_L1_1_QUEST_1_4 );
        this->pDialogScreen->loadDialogFromXml( 14 );
    }
    if( _screen== screen_id::MG_L1_1_QUEST_2_4 )
    {
        if( _isWin )
        {
            this->embiantSource->changeGainTo( 0.5f, 2);//MAX_VOLUME_EMBIENT_MUSIC, 2 );
            this->backMusicSource->changeGainTo( MAX_VOLUME_EMBIENT_MUSIC, 2 );
            
            this->pDialogScreen->loadDialogFromXml( 15 );
            MANAGER_Screen->removeScreen( GET_RUNNING_SCENE, screen_id::MG_L1_1_QUEST_2_4 );
            this->getGameObjectById( 902 )->release();
            //coins add
            this->localPlayerCoins += 5;
            this->localPlayerCoinsIntoPoints += 5;
            
            this->hideGameObject( 1001 );
            this->showGameObject( 1500 );
        }
        else
        {
            this->setLevelState( "gates_minigames", 0 );
            MANAGER_Screen->removeScreen( GET_RUNNING_SCENE, screen_id::MG_L1_1_QUEST_2_4 );
        }
    }
}

//-----------------------------------------------------------------------------------------------------
void Level_1_1_part1::onDialogueEnd( int _id )
{
    if( _id == 0 )
    {
        //delay for oldman animation
        auto animationAction = Sequence::create( DelayTime::create( 2.0f ),
                                                 CallFunc::create( CC_CALLBACK_0( Level_1_1_part1::callPostAnimation, this ) ), nullptr );
        GET_RUNNING_SCENE->runAction( animationAction );
        //
        this->addNotesMinigame();
    }
    if( _id == 1 )
    {
        this->pDialogScreen->loadDialogFromXml( 2 );
        //load
        this->pHudScreen->showHandHint( Vec2( 500, 600 ), 0, 1.5f, 0.4f );
    }
    if( _id == 2 )
    {
        //ANIMATION OLDMAN
        GameObject* obj = this->getGameObjectById( 2000 );
        if( obj )
        {
            graphic::GraphicAnimationObject* anim = dynamic_cast<graphic::GraphicAnimationObject*>( obj->getGraphic() );
            anim->setState( graphic::sequence_mob::SM_WAIT );
        }
        //
        
        MANAGER_Screen->attachScreen( GET_RUNNING_SCENE, screen_id::MG_L1_1_QUEST_1_1 );
    }
    
    if( _id == 4 )
    {
        this->embiantSource->changeGainTo( MIN_VOLUME_EMBIENT_MUSIC, 2 );
        this->backMusicSource->changeGainTo( MIN_VOLUME_EMBIENT_MUSIC, 2 );
        
        this->pNotesMinigameScreen->hideHint();
        this->pNotesMinigameScreen->getNotebook()->showTrebleClef();
        this->pHudScreen->showSticker();
    }
    
    if( _id == 6 )
    {
        dynamic_cast< L1_1_Quest_2_3_MGScreen* >( MANAGER_Screen->getScreen( GET_RUNNING_SCENE, screen_id::MG_L1_1_QUEST_2_3 ) )->showHintHand();
    }
    
    if( _id == 8 )
    {
        //Graphics
        GameObject* obj = this->getGameObjectById( 2000 );
        if( obj )
        {
            graphic::GraphicAnimationObject* anim = dynamic_cast<graphic::GraphicAnimationObject*>( obj->getGraphic() );
            anim->setState( graphic::sequence_mob::SM_WAIT );
        }
        obj = this->getGameObjectById( 1002 );
        if( obj )
        {
            graphic::GraphicAnimationObject* anim = dynamic_cast<graphic::GraphicAnimationObject*>( obj->getGraphic() );
            anim->setState( graphic::sequence_mob::SM_WAIT, false );
            anim->setViewPart( -1 );
        }
        //
        MANAGER_Level->resumeLevelTimer();
        this->pHudScreen->showTimer();
        
        this->pHudScreen->showSticker();
    }
    
    if( _id == 12 )
    {
        this->embiantSource->changeGainTo( MIN_VOLUME_EMBIENT_MUSIC, 2 );
        this->backMusicSource->changeGainTo( MIN_VOLUME_EMBIENT_MUSIC, 2 );
        
        this->pHudScreen->showHandHint( Vec2( 820, 600 ), 0, 1.5f, 0.4f );
    }
    
    if( _id == 13 )
    {
        dynamic_cast< L1_1_Quest_1_4_MGScreen* >( MANAGER_Screen->getScreen( GET_RUNNING_SCENE, screen_id::MG_L1_1_QUEST_1_4 ) )->showHintHand();
    }
    
    if( _id == 14 )
    {
        MANAGER_Screen->addScreen( GET_RUNNING_SCENE, screen_id::MG_L1_1_QUEST_2_4 );
    }
    if( _id == 15 )
    {
        this->pHudScreen->showSticker();
        MANAGER_Level->resumeLevelTimer();
        this->pHudScreen->showTimer();
    }
}

//-----------------------------------------------------------------------------------------------------
void Level_1_1_part1::callPostAnimation()
{
    //adding staff for minigame with seven notes and to show treble clef
    MANAGER_Screen->addScreen( GET_RUNNING_SCENE, screen_id::NOTESMINIGAME );
    pNotesMinigameScreen = dynamic_cast< NotesMinigameScreen* >( MANAGER_Screen->getScreen(GET_RUNNING_SCENE, screen_id::NOTESMINIGAME ) );
    pNotesMinigameScreen->getNotebook()->hideTrebleClef();
    
    this->pDialogScreen->loadDialogFromXml( 1 );
}

//-----------------------------------------------------------------------------------------------------
void Level_1_1_part1::onVictory()
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
        
        MANAGER_Memory->getTempData()->levelsProgressData[0]->gameLevelsProgress = level_state::LVL_STATE_COMPLETED;
        if( MANAGER_Memory->getTempData()->levelsProgressData[1]->gameLevelsProgress != level_state::LVL_STATE_COMPLETED )
            MANAGER_Memory->getTempData()->levelsProgressData[1]->gameLevelsProgress = level_state::LVL_STATE_OPENED;
        MANAGER_Memory->getTempData()->setLevelStars( 0 );
        
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
void Level_1_1_part1::onDefeat()
{
    this->pHudScreen->hideSticker();
    if( this->getLevelState( "SINGLE_MINIGAME" ) == 1 )
        MANAGER_Scene->replaceScene( scene_id::MINIGAMES_SELECTION );
    else
        dynamic_cast<TryMoreScreen*>(MANAGER_Screen->getScreen(GET_RUNNING_SCENE, screen_id::TRY_MORE))->show();
}

//-----------------------------------------------------------------------------------------------------
void Level_1_1_part1::addNotesMinigame()
{
    if( this->getGameObjectById( 808 ) )
        this->hideGameObject( 808 );
    
    this->showGameObject( 306 );
    this->showGameObject( 307 );
    this->showGameObject( 308 );
    this->showGameObject( 309 );
    this->showGameObject( 310 );
    this->showGameObject( 311 );
    this->showGameObject( 312 );
    
    this->showGameObject( 21 );
    this->showGameObject( 22 );
    this->showGameObject( 23 );
    this->showGameObject( 24 );
    this->showGameObject( 25 );
    this->showGameObject( 26 );
    this->showGameObject( 27 );
}

//-----------------------------------------------------------------------------------------------------
void Level_1_1_part1::determinePlayerPosition()
{
    //Player first positioning
    MANAGER_Memory->getTempData()->playerPosition = Vec2( 14, 8.7 );
    
    //Player second positioning ( after restart level from trymore screen )
    if( this->getLevelState( "treble_clef_mg" ) == 1 )
        MANAGER_Memory->getTempData()->playerPosition = Vec2( 93, 15.3 );
        
    if( this->getLevelState( "gates_minigames" ) == 1 )
        MANAGER_Memory->getTempData()->playerPosition = Vec2( 323, 11.2 );
    
    //set player position
    this->pPlayer->getGameObject()->setPosition( MANAGER_Memory->getTempData()->playerPosition );
    
    //set player active
    this->showGameObject( 777 );
    
    pGameCamera->setPosition( pPlayer->getGameObject()->getPosition() );
}

//-----------------------------------------------------------------------------------------------------
void Level_1_1_part1::onCheckPoint( GameObject* _checkPoint )
{
    if( ( _checkPoint->getId() == 970 && this->getLevelState( "treble_clef_mg" ) == 1 ) ||
        ( _checkPoint->getId() == 971 && this->getLevelState( "seven_notsMiniGame_end" ) == 1 ) ||
        ( _checkPoint->getId() == 972 && this->getLevelState( "treble_clef_mg_complete" ) == 1 ) ||
        ( _checkPoint->getId() == 973 && this->getLevelState( "gates_minigames" ) == 1 ) )
    {
        this->saveLevelStatData();
        _checkPoint->release();
    }
}

//-----------------------------------------------------------------------------------------------------
void Level_1_1_part1::singleMinigameLogic()
{
    //player positioning, logic adding (screen with minigame)
    if( this->getLevelState( "sm_level_1_1_1" ) == 1 )
    {
        MANAGER_Memory->getTempData()->playerPosition = Vec2( 93, 17 );
        //
        MANAGER_Screen->addScreen( GET_RUNNING_SCENE, screen_id::MG_L1_1_QUEST_1_1 );
        this->embiantSource->changeGainTo( MIN_VOLUME_EMBIENT_MUSIC, 2 );
        this->backMusicSource->changeGainTo( MIN_VOLUME_EMBIENT_MUSIC, 2 );
        //
        this->pHudScreen->hideSticker();
    }
    if( this->getLevelState( "sm_level_1_1_1" ) == 2 )
    {
        MANAGER_Memory->getTempData()->playerPosition = Vec2( 93, 17 );
        
        this->embiantSource->changeGainTo( MIN_VOLUME_EMBIENT_MUSIC, 2 );
        this->backMusicSource->changeGainTo( MIN_VOLUME_EMBIENT_MUSIC, 2 );
        //
        //adding staff for minigame with seven notes and to show treble clef
        MANAGER_Screen->addScreen( GET_RUNNING_SCENE, screen_id::NOTESMINIGAME );
        pNotesMinigameScreen = dynamic_cast< NotesMinigameScreen* >( MANAGER_Screen->getScreen(GET_RUNNING_SCENE, screen_id::NOTESMINIGAME ) );
        pNotesMinigameScreen->getNotebook()->hideTrebleClef();

        this->addNotesMinigame();
        
        this->pHudScreen->showSticker();
    }
    if( this->getLevelState( "sm_level_1_1_1" ) == 3 )
    {
        MANAGER_Memory->getTempData()->playerPosition = Vec2( 93, 17 );
        //
        MANAGER_Screen->addScreen( GET_RUNNING_SCENE, screen_id::MG_L1_1_QUEST_1_3 );
        this->embiantSource->changeGainTo( MIN_VOLUME_EMBIENT_MUSIC, 2 );
        this->backMusicSource->changeGainTo( MIN_VOLUME_EMBIENT_MUSIC, 2 );
        //
        this->pHudScreen->hideSticker();
    }
    if( this->getLevelState( "sm_level_1_1_1" ) == 4 )
    {
        MANAGER_Memory->getTempData()->playerPosition = Vec2( 93, 17 );
        //
        MANAGER_Screen->addScreen( GET_RUNNING_SCENE, screen_id::MG_L1_1_QUEST_2_3 );
        this->embiantSource->changeGainTo( MIN_VOLUME_EMBIENT_MUSIC, 2 );
        this->backMusicSource->changeGainTo( MIN_VOLUME_EMBIENT_MUSIC, 2 );
        //
        this->pHudScreen->hideSticker();
    }
    //GATES
    if( this->getLevelState( "sm_level_1_1_1" ) == 5 )
    {
        MANAGER_Memory->getTempData()->playerPosition = Vec2( 323, 14 );
        //
        MANAGER_Screen->addScreen( GET_RUNNING_SCENE, screen_id::MG_L1_1_QUEST_2_1 );
        this->embiantSource->changeGainTo( MIN_VOLUME_EMBIENT_MUSIC, 2 );
        this->backMusicSource->changeGainTo( MIN_VOLUME_EMBIENT_MUSIC, 2 );
        //
        this->pHudScreen->hideSticker();
    }
    if( this->getLevelState( "sm_level_1_1_1" ) == 6 )
    {
        MANAGER_Memory->getTempData()->playerPosition = Vec2(  323, 14  );
        //
        MANAGER_Screen->addScreen( GET_RUNNING_SCENE, screen_id::MG_L1_1_QUEST_2_2 );
        this->embiantSource->changeGainTo( MIN_VOLUME_EMBIENT_MUSIC, 2 );
        this->backMusicSource->changeGainTo( MIN_VOLUME_EMBIENT_MUSIC, 2 );
        //
        this->pHudScreen->hideSticker();
    }
    if( this->getLevelState( "sm_level_1_1_1" ) == 7 )
    {
        MANAGER_Memory->getTempData()->playerPosition = Vec2(  323, 14  );
        //
        MANAGER_Screen->addScreen( GET_RUNNING_SCENE, screen_id::MG_L1_1_QUEST_1_4 );
        this->embiantSource->changeGainTo( MIN_VOLUME_EMBIENT_MUSIC, 2 );
        this->backMusicSource->changeGainTo( MIN_VOLUME_EMBIENT_MUSIC, 2 );
        //
        this->pHudScreen->hideSticker();
    }
    if( this->getLevelState( "sm_level_1_1_1" ) == 8 )
    {
        MANAGER_Memory->getTempData()->playerPosition = Vec2(  323, 14  );
        //
        MANAGER_Screen->addScreen( GET_RUNNING_SCENE, screen_id::MG_L1_1_QUEST_2_4 );
        this->embiantSource->changeGainTo( MIN_VOLUME_EMBIENT_MUSIC, 2 );
        this->backMusicSource->changeGainTo( MIN_VOLUME_EMBIENT_MUSIC, 2 );
        //
        this->pHudScreen->hideSticker();
    }
    //
    this->pPlayer->getGameObject()->setPosition( MANAGER_Memory->getTempData()->playerPosition );
    //add player gameobject to active obj list
    this->showGameObject( 777 );
}

//-----------------------------------------------------------------------------------------------------
void Level_1_1_part1::saveLevelStatData()
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