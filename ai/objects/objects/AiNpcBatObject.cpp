//
//  AiNpcBatObject.cpp
//  MusicalAdventure
//
//  Created by madmin on 6/25/14.
//  Copyright (c) 2014 ampagames. All rights reserved.
//

#include "AiNpcBatObject.h"

#include "GameObject.h"

#include "DialogScreen.h"
#include "L1_3_Quest_1_MGScreen.h"

using namespace cocos2d;
using namespace tinyxml2;
using namespace ai;
using namespace physic;

//-----------------------------------------------------------------------------------------------------
AiNpcBatObject::AiNpcBatObject():
beginPosition( Vec2::ZERO ),
targetPosition( Vec2::ZERO ),
speed( 8.0f ),
functionCallBackOnMoveTo( nullptr )
{
    type = AI_NPC_BAT_OBJ;
    
    pGameObject = NULL;
}

//-----------------------------------------------------------------------------------------------------
AiNpcBatObject::~AiNpcBatObject()
{
}

//-----------------------------------------------------------------------------------------------------
AiNpcBatObject* AiNpcBatObject::create(XMLElement* _xmlAiObject)
{
    AiNpcBatObject* pAi = new AiNpcBatObject();
    if(pAi && pAi->initWithXml(_xmlAiObject))
        return pAi;
    else
    {
        delete pAi;
        return NULL;
    }
}

//-----------------------------------------------------------------------------------------------------
void AiNpcBatObject::release()
{
    delete this;
}

//-----------------------------------------------------------------------------------------------------
bool AiNpcBatObject::initWithXml(XMLElement* _xmlAiObject)
{
    if( _xmlAiObject )
    {
        //configuration

        return true;
    }
    
    return false;
}

//-----------------------------------------------------------------------------------------------------
void AiNpcBatObject::update(float _dt)
{
    if( this->isStart )
    {
        if( this->pGameObject )
        {
            Vec2 nowPos = this->pGameObject->getPosition();
            
            //Set state anim run if have target point
            if( nowPos.x != this->targetPosition.x ||
                nowPos.y != this->targetPosition.y
              )
            {
                graphic::GraphicAnimationObject* graphicBat = dynamic_cast<graphic::GraphicAnimationObject*>( this->pGameObject->getGraphic() );
                
                if( graphicBat && graphicBat->getState() != graphic::sequence_mob::SM_RUN )
                    graphicBat->setState( graphic::sequence_mob::SM_RUN );
            }
            //
            
            if( this->pGameObject->getPosition().x < this->targetPosition.x )
                this->pGameObject->setDirection( game_object_direction::LEFT );
            else
                this->pGameObject->setDirection( game_object_direction::RIGHT );
            
            // Logic movement the bat
            Vec2 moveVec = this->targetPosition - nowPos;
            float moveDist = moveVec.getLength();
            float moveStep = this->speed * _dt;
            
            if( moveDist > moveStep )
            {
                Vec2 moveNormal = moveVec.getNormalized();
                this->pGameObject->setPosition( nowPos + ( moveNormal * moveStep ) );
                
                nowPos = this->pGameObject->getPosition();
                moveVec = this->targetPosition - nowPos;
                moveDist = moveVec.getLength();
                
                
                if( moveDist < moveStep )
                {
                    this->pGameObject->setPosition( this->targetPosition );
                    this->isStart = false;
                    
                    if( this->functionCallBackOnMoveTo )
                        this->functionCallBackOnMoveTo();
                    
                    // Set state to wait if we arrived at the scene
                    if( this->pGameObject )
                    {
                        graphic::GraphicAnimationObject* graphicBat = dynamic_cast<graphic::GraphicAnimationObject*>( this->pGameObject->getGraphic() );
                        
                        if( graphicBat && graphicBat->getState() != graphic::sequence_mob::SM_WAIT )
                            graphicBat->setState( graphic::sequence_mob::SM_WAIT );
                    }
                }
            }
        }
    }
}

//-----------------------------------------------------------------------------------------------------
void AiNpcBatObject::moveTo( Vec2 _targetPosition, float _speed, const std::function<void()> &_functionCallback )
{
    this->functionCallBackOnMoveTo = _functionCallback;
    this->targetPosition = _targetPosition;
    this->speed = _speed;
    
    this->isStart = true;
}

//-----------------------------------------------------------------------------------------------------