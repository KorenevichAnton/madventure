//
//  ShopScreen.cpp
//  MusicalAdventure
//
//  Created by ampaUser on 5/11/15.
//  Copyright (c) 2015 ampagames. All rights reserved.
//

#include "ShopScreen.h"
#include "AppController.h"

#include "LevelSelectionScene.h"

using namespace cocos2d;

#define FONT_COLOR_TITLE    Color3B( 90, 55, 20 )

//-----------------------------------------------------------------------------------------------
ShopScreen* ShopScreen::create()
{
    ShopScreen* pRet = new ShopScreen();
    if( pRet && pRet->init() )
        return pRet;
    else
    {
        delete  pRet;
        pRet = NULL;
    }
    
    return NULL;
}

//-----------------------------------------------------------------------------------------------
ShopScreen::ShopScreen() :
pBackgroundPanel( nullptr ),
pShopMenu( nullptr )
{
    this->selfId = screen_id::SHOP;
}

//-----------------------------------------------------------------------------------------------
ShopScreen::~ShopScreen()
{
    for( int i = 0 ; i < this->vDonateButtons.size() ; i++ )
    {
        DonateButtonInfo* dbf = ( DonateButtonInfo* )( vDonateButtons[i]->getUserData() );
        delete dbf;
        dbf = NULL;
        this->vDonateButtons[i]->setUserData( NULL );
    }
}

//-----------------------------------------------------------------------------------------------
bool ShopScreen::init()
{
    if( !IScreen::init() )
        return false;
    
    LevelSelectionScene* pLevelSelectionScene = dynamic_cast<LevelSelectionScene*>( GET_RUNNING_SCENE );
    if(pLevelSelectionScene)
    {
        pLevelSelectionScene->getLevelsMenu()->setEnabled(false);
        pLevelSelectionScene->getStaticLevelsMenu()->setEnabled(false);
    }
    
    Size winSize = Director::getInstance()->getWinSize();
    
    //background
    this->pBackgroundPanel = CCSprite::create( "shop/plashka.png" );
    this->pBackgroundPanel->setPosition( winSize / 2 );
    this->addChild( this->pBackgroundPanel );
    
    //plate
    Sprite* pPlate = Sprite::create( "shop/plate.png" );
    pPlate->setPosition( Vec2( this->pBackgroundPanel->getContentSize().width / 2 + 55, this->pBackgroundPanel->getContentSize().height - 40 ) );
    this->pBackgroundPanel->addChild( pPlate );
    
    this->initButtons();
    
    this->createDonatePlate( Vec2( 190.0f, 140.0f ), 500, 800 );
    this->createDonatePlate( Vec2( 190.0f, 310.0f ), 300, 500 );
    this->createDonatePlate( Vec2( 190.0f, 480.0f ), 100, 100 );
    
    this->createDonatePlate( Vec2( 520.0f, 140.0f ), 600, 1250 );
    this->createDonatePlate( Vec2( 520.0f, 310.0f ), 400, 650 );
    this->createDonatePlate( Vec2( 520.0f, 480.0f ), 200, 250 );
    
    return true;
}

//-----------------------------------------------------------------------------------------------
void ShopScreen::initButtons()
{
    Menu* exitMenu = Menu::create();
    exitMenu->setPosition( Vec2( 0.0f, 0.0f ) );
    this->pBackgroundPanel->addChild( exitMenu );
    
    this->pShopMenu = Menu::create();
    this->pShopMenu->setPosition( Vec2( 0.0f, 0.0f ) );
    this->pBackgroundPanel->addChild( this->pShopMenu, 1 );
    
    MenuItemImage* pCloseBtn = MenuItemImage::create( "btn/close.png", "btn/close_D.png", CC_CALLBACK_1( ShopScreen::onCloseBtn, this ) );
    pCloseBtn->setPosition( Vec2( pBackgroundPanel->getContentSize().width - 30, pBackgroundPanel->getContentSize().height - 60 ) );
    exitMenu->addChild( pCloseBtn );
}

//-----------------------------------------------------------------------------------------------
void ShopScreen::createDonatePlate( Vec2 _position, int _price, int _gameMoney )
{
    //donate background
    Sprite* donatePlate = Sprite::create("shop/buy_plashka.png");
    donatePlate->setPosition( _position );
    this->pBackgroundPanel->addChild( donatePlate );
    
    //donate price
    char priceStr[255];
    sprintf( priceStr, "%d %s", _price, L10N_STRING( 4001 ).c_str() );
    auto priceLabel = Label::createWithTTF( priceStr, FONT_PATH, 26 );
    priceLabel->setAnchorPoint( Vec2( 0.5, 0.5 ) );
    priceLabel->setPosition( Vec2( 85.0f, 125.0f ) );
    priceLabel->setColor( FONT_COLOR_TITLE );
    donatePlate->addChild( priceLabel, 1 );
    
    //donate game money
    char gameMoneyStr[255];
    sprintf( gameMoneyStr, "%d", _gameMoney );
    auto gameMoneyLabel = Label::createWithTTF( gameMoneyStr, FONT_PATH, 42 );
    gameMoneyLabel->setAnchorPoint( Vec2( 0.5, 0.5 ) );
    gameMoneyLabel->setPosition( Vec2( 145, 55 ) );
    gameMoneyLabel->setColor( FONT_COLOR_TITLE );
    donatePlate->addChild( gameMoneyLabel, 1 );
    
    //donate button
    MenuItem* donateBtn = this->createDonateButton( 0, _price, _gameMoney );
    donateBtn->setPosition( Vec2( _position.x + 97, _position.y - 19 ) );
    this->pShopMenu->addChild( donateBtn );
}

//-----------------------------------------------------------------------------------------------
MenuItem* ShopScreen::createDonateButton( int _index, int _price, int _gameMoney )
{
    Sprite* donateBtnNor = CCSprite::create( "btn/buy.png" );
    Sprite* donateBtnSel = CCSprite::create( "btn/buy_down.png" );
    
    MenuItemSprite* menuItem = CCMenuItemSprite::create( donateBtnNor, donateBtnSel, CC_CALLBACK_1( ShopScreen::onDonateBtn, this ) );
    
    //set donate data
    DonateButtonInfo* dbf = new DonateButtonInfo();
    dbf->index = this->vDonateButtons.size();
    dbf->price = _price;
    dbf->gameMoney = _gameMoney;
    menuItem->setUserData( dbf );
    this->vDonateButtons.push_back( menuItem );
    
    return menuItem;
}

//-----------------------------------------------------------------------------------------------
void ShopScreen::update( float _dt )
{
    
}

//-----------------------------------------------------------------------------------------------
void ShopScreen::onDonateBtn( Ref* _sender )
{
    DonateButtonInfo* dbf = ( DonateButtonInfo* )( ( ( MenuItem* )_sender )->getUserData() );
    MANAGER_Memory->getTempData()->playerMoney += dbf->gameMoney;
}


//-----------------------------------------------------------------------------------------------
void ShopScreen::onCloseBtn( Ref* _sender )
{
    LevelSelectionScene* pLevelSelectionScene = dynamic_cast<LevelSelectionScene*>( GET_RUNNING_SCENE );
    if(pLevelSelectionScene)
    {
        pLevelSelectionScene->getLevelsMenu()->setEnabled(true);
        pLevelSelectionScene->getStaticLevelsMenu()->setEnabled(true);
    }
    
    MANAGER_Screen->removeScreen( GET_RUNNING_SCENE, screen_id::SHOP );
}

//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------