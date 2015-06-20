//
//  TipHand.cpp
//  MusicalAdventure
//
//  Created by ampaUser on 4/30/15.
//  Copyright (c) 2015 ampagames. All rights reserved.
//

#include "TipHand.h"

using namespace cocos2d;

//--------------------------------------------------------------------------------------------------------
TipHand* TipHand::create( float _angle, float _duration, float _speed )
{
    TipHand* tipHand = new TipHand( _angle, _duration, _speed );
    if( tipHand->init() )
    {
        tipHand->autorelease();
        return tipHand;
    }
    else
        CC_SAFE_DELETE( tipHand );
    
    return nullptr;
}

//--------------------------------------------------------------------------------------------------------
TipHand::TipHand( float _angle, float _duration, float _speed ):
angle( _angle ),
duration( _duration ),
speed( _speed ),
handSprite( nullptr ),
handHintDirection( 0 ),
stopState( false )
{
}

//--------------------------------------------------------------------------------------------------------
TipHand::~TipHand()
{
}

//--------------------------------------------------------------------------------------------------------
bool TipHand::init()
{
    if( CCNode::init() == false )
        return false;
    
    
    // Create fly hand
    this->handSprite = CCSprite::create( "textures/scenes/minigame/L1_2_MusicBox2_MGScreen/hand.png" );
    this->handSprite->setRotation( this->angle );
    this->addChild( this->handSprite );
    
    // define direction
    if( this->angle == 0 )
        this->handHintDirection = 0;
    else if( this->angle == 90.0f )
        this->handHintDirection = 1;
    else if( this->angle == 180.0f )
        this->handHintDirection = 2;
    else
        this->handHintDirection = 3;
    
    
    return true;
}

//--------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------
void TipHand::start()
{
    this->stopState = false;
    
    // start animation
    this->callbackMoveForward();
    
    // set life time
    auto timer = CCDelayTime::create( this->duration );
    auto callFunc = CCCallFunc::create( CC_CALLBACK_0( TipHand::callbackStopping, this ) );
    
    CCSequence* sequence = nullptr;
    if( this->duration > 0 )
        sequence = CCSequence::create( timer, callFunc, nullptr );
    
    if( sequence )
        this->handSprite->runAction( sequence );
}

//--------------------------------------------------------------------------------------------------------
void TipHand::stop()
{
    this->callbackStopping();
}

//--------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------
void TipHand::callbackMoveForward()
{
    MoveTo* moveHand = nullptr;
    if( this->handHintDirection == 0 || this->handHintDirection == 2 )
        moveHand = CCMoveTo::create( this->speed, Vec2( handSprite->getPosition().x, handSprite->getPosition().y - 30 ) );
    else
        moveHand = CCMoveTo::create( this->speed, Vec2( handSprite->getPosition().x - 30, handSprite->getPosition().y ) );
    
    CallFunc *callFunc = CallFunc::create( CC_CALLBACK_0( TipHand::callbackMoveBack, this ) );
    
    Sequence* sequence = Sequence::create( moveHand, callFunc, nullptr );
    
    this->handSprite->runAction( sequence );
}

//--------------------------------------------------------------------------------------------------------
void TipHand::callbackMoveBack()
{
    MoveTo* moveHand = nullptr;
    if( this->handHintDirection == 0 || this->handHintDirection == 2 )
        moveHand = CCMoveTo::create( this->speed, Vec2( handSprite->getPosition().x, handSprite->getPosition().y + 30 ) );
    else
        moveHand = CCMoveTo::create( this->speed, Vec2( handSprite->getPosition().x + 30, handSprite->getPosition().y ) );
    
    CallFunc *callFunc = CallFunc::create( CC_CALLBACK_0( TipHand::callbackMoveForward, this ) );
    
    Sequence* sequence = Sequence::create( moveHand, callFunc, nullptr );
    
    this->handSprite->runAction( sequence );
}

//--------------------------------------------------------------------------------------------------------
void TipHand::callbackStopping()
{
    if( !this->stopState )
    {
        this->stopState = true;
        
        this->handSprite->stopAllActions();
        
        // fade out
        auto hideHand = CCFadeOut::create( 0.5f );
        this->handSprite->runAction( hideHand );
    }
}

//--------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------