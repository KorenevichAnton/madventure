//
//  MinigamesMenuScene.cpp
//  MusicalAdventure
//
//  Created by Anton Korenevich on 10/22/14.
//  Copyright (c) 2014 ampagames. All rights reserved.
//

#include "MinigamesMenuScene.h"
#include "Clipping.h"

using namespace std;
using namespace cocos2d;

//---------------------------------------------------------------------------------------------------------------------------------
MinigamesMenuScene* MinigamesMenuScene::create()
{
    MinigamesMenuScene* pRet = new MinigamesMenuScene();
    if(pRet && pRet->init())
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        CC_SAFE_DELETE(pRet);
        return NULL;
    }
}

//---------------------------------------------------------------------------------------------------------------------------------
MinigamesMenuScene::MinigamesMenuScene()
{
    selfId = scene_id::MINIGAMES_SELECTION;
}

//---------------------------------------------------------------------------------------------------------------------------------
MinigamesMenuScene::~MinigamesMenuScene()
{
    MANAGER_Screen->removeAllScreens( this );
}

//---------------------------------------------------------------------------------------------------------------------------------
bool MinigamesMenuScene::init()
{
    if( !IScene::init() )
        return false;
    
    initBackground();
    initButtons();
    
    return true;
}

//---------------------------------------------------------------------------------------------------------------------------------
void MinigamesMenuScene::onEnter()
{
    IScene::onEnter();
    log("MinigamesMenuScene::onEnter");
    
    ///Reset timer / statistics
    MANAGER_Level->resetLevelTimer();
    MANAGER_Memory->getTempData()->resetPlayerData();
    MANAGER_Memory->getTempData()->resetAllLevelStates();
    //ADDING CORRECT LOCATION RESTART
    MANAGER_Memory->getTempData()->locationCoinsIntoPoints = 0;
    MANAGER_Memory->getTempData()->playerCoinsIntoPoints = 0;
    if( MANAGER_Level->getCurrentLevel() )
        MANAGER_Level->getCurrentLevel()->resetAllLevelStates();
    
    MANAGER_Screen->addScreen(GET_RUNNING_SCENE, screen_id::MINIGAMES_MENU);
}

//---------------------------------------------------------------------------------------------------------------------------------
void MinigamesMenuScene::onExit()
{
    IScene::onExit();
    log("MinigamesMenuScene::onExit");
}

//---------------------------------------------------------------------------------------------------------------------------------
void MinigamesMenuScene::clean()
{
    MANAGER_Screen->removeAllScreens( this );
}

//---------------------------------------------------------------------------------------------------------------------------------
void MinigamesMenuScene::update(float _dt)
{
}

//---------------------------------------------------------------------------------------------------------------------------------
void MinigamesMenuScene::initBackground()
{
    
}

//---------------------------------------------------------------------------------------------------------------------------------
void MinigamesMenuScene::initButtons()
{
    
}

//---------------------------------------------------------------------------------------------------------------------------------