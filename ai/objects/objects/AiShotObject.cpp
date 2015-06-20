//
//  AiShotObject.cpp
//  MusicalAdventure
//
//  Created by korenevichanton@gmail.com on 7/2/14.
//  Copyright (c) 2014 ampagames. All rights reserved.
//

#include "AiShotObject.h"
#include "GameObject.h"
#include "IPhysDynamicObject.h"
//TEMP
#include "PhysDynamicShotObject.h"
#include "LevelManager.h"

using namespace cocos2d;
using namespace tinyxml2;
using namespace ai;
using namespace physic;

//-----------------------------------------------------------------------------------------------------
AiShotObject::AiShotObject():
cyclic(false),
speed(0.0f),
passedPoints(0),
beginPos(0,0)
{
    type = AI_SHOT_OBJ;
    
    pGameObject = NULL;
}

//-----------------------------------------------------------------------------------------------------
AiShotObject::~AiShotObject()
{
}

//-----------------------------------------------------------------------------------------------------
AiShotObject* AiShotObject::create(XMLElement* _xmlAiObject)
{
    AiShotObject* pAi = new AiShotObject();
    if(pAi && pAi->initWithXml(_xmlAiObject))
        return pAi;
    else
    {
        delete pAi;
        return NULL;
    }
}

//-----------------------------------------------------------------------------------------------------
void AiShotObject::release()
{
    delete this;
}

//-----------------------------------------------------------------------------------------------------
bool AiShotObject::initWithXml(XMLElement* _xmlAiObject)
{
    if(_xmlAiObject)
    {
        XMLElement* wayElement = _xmlAiObject->FirstChildElement("way");
        if(wayElement)
        {
            //this->speed = atof(wayElement->Attribute("speed");
            this->speed = 50.0f;
            return true;
        }
    }
    
    return false;
}

//-----------------------------------------------------------------------------------------------------
void AiShotObject::update(float _dt)
{
    if(this->pGameObject)
    {
        if(this->beginPos.equals(Vec2::ZERO))
            this->beginPos = this->getGameObject()->getPosition();
        
        
        Vec2 targetPoint = Vec2::ZERO;
        if(this->playersShot)
        {
            if(this->getGameObject()->getDirection() == game_object_direction::RIGHT)
                targetPoint = Vec2(40, 0);
            else if (this->getGameObject()->getDirection() == game_object_direction::LEFT)
                targetPoint = Vec2(-40, 0);
        }
        else
        {
            targetPoint = Vec2(0, -32);
        }
        Vec2 nowPos = this->getGameObject()->getPosition();
        Vec2 globTargetPoint = Vec2(this->beginPos + targetPoint);
        Vec2 moveVec = Vec2( globTargetPoint - nowPos );
        float moveDist = moveVec.getLength();
        float moveStep = this->speed * _dt;
        
        if(moveDist >= moveStep)
        {
            Vec2 moveNormal = moveVec.getNormalized();
            this->changePosition( Vec2( moveNormal * moveStep ) );
        }
        else
        {
            this->passedPoints++;
            this->getGameObject()->getPhysic()->getGameWorld()->removeGameObject( this->getGameObject() );
        }
    }
}

//-----------------------------------------------------------------------------------------------------
void AiShotObject::changePosition(const cocos2d::Vec2 _position) const
{
    if(this->pGameObject)
    {
        IPhysObject* physObj = this->pGameObject->getPhysic();
        if(physObj->getType() == phys_object_type::DYNAMIC)
        {
            IPhysDynamicObject* physDynamicObj = dynamic_cast<IPhysDynamicObject*>(physObj);
            physDynamicObj->setImpulseVector( _position );
        }
    }
}

//-----------------------------------------------------------------------------------------------------
void AiShotObject::onHitStaticObject( GameObject* _contactObj )
{
    this->getGameObject()->getPhysic()->getGameWorld()->removeGameObject( this->getGameObject() );
}

//-----------------------------------------------------------------------------------------------------
void AiShotObject::onHitDynamicObject( GameObject* _contactObj )
{
    auto pPhysDynamicContactObj = dynamic_cast< IPhysDynamicObject* >( _contactObj->getPhysic() );
    if( pPhysDynamicContactObj )
    {
        phys_dynamic_object_type subtype = pPhysDynamicContactObj->getSubType();
        if( subtype == phys_dynamic_object_type::PLAYER && !this->playersShot )
            this->getGameObject()->getPhysic()->getGameWorld()->removeGameObject( this->getGameObject() );
        if( ( subtype == phys_dynamic_object_type::BOSS1 ||
              subtype == phys_dynamic_object_type::FLYING_MOB ||
              subtype == phys_dynamic_object_type::WALKING_MOB ) &&
              this->playersShot )
        {
            this->getGameObject()->getPhysic()->getGameWorld()->removeGameObject( this->getGameObject() );
        }
        if( subtype == phys_dynamic_object_type::GROUND )
        {
            this->getGameObject()->getPhysic()->getGameWorld()->removeGameObject( this->getGameObject() );
        }
        if( subtype == phys_dynamic_object_type::CLOUD )
        {
            this->getGameObject()->getPhysic()->getGameWorld()->removeGameObject( this->getGameObject() );
        }
    }
}

//-----------------------------------------------------------------------------------------------------