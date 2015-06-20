//
//  AiFlyingMobObject.cpp
//  MusicalAdventure
//
//  Created by madmin on 06/17/14.
//  Copyright (c) 2014 ampagames. All rights reserved.
//

#include "AiFlyingMobObject.h"
#include "GameObject.h"
#include "IPhysDynamicObject.h"

#include "PhysDynamicFlyingMobObject.h"
#include "PhysDynamicPlayerObject.h"

#include "AppController.h"

using namespace cocos2d;
using namespace tinyxml2;
using namespace ai;
using namespace physic;

//-----------------------------------------------------------------------------------------------------
AiFlyingMobObject::AiFlyingMobObject():
cyclic(false),
speed(0.0f),
passedPoints(0),
beginPos(0,0),
hp( 1 )
{
    type = AI_UNIT_OBJ;
    
    pGameObject = NULL;
}

//-----------------------------------------------------------------------------------------------------
AiFlyingMobObject::~AiFlyingMobObject()
{
}

//-----------------------------------------------------------------------------------------------------
AiFlyingMobObject* AiFlyingMobObject::create( XMLElement* _xmlAiObject)
{
    AiFlyingMobObject* pAi = new AiFlyingMobObject();
    if(pAi && pAi->initWithXml(_xmlAiObject))
        return pAi;
    else
        
    {
        delete pAi;
        return NULL;
    }
}

//-----------------------------------------------------------------------------------------------------
void AiFlyingMobObject::release()
{
    delete this;
}

//-----------------------------------------------------------------------------------------------------
bool AiFlyingMobObject::initWithXml(XMLElement* _xmlAiObject)
{
    if(_xmlAiObject)
    {
        XMLElement* wayElement = _xmlAiObject->FirstChildElement("way");
        if(wayElement)
        {
            this->cyclic = ( atoi(wayElement->Attribute("cyclic")) > 0 ) ? true : false;
            
            this->speed = atof(wayElement->Attribute("speed"));
            
            //fill wayPoints
            XMLElement* pointElement = wayElement->FirstChildElement("point");
            while(pointElement)
            {
                Vec2 point = Vec2( atof(pointElement->Attribute("x")),
                                    atof(pointElement->Attribute("y")) );
                
                this->wayPoints.push_back(point);
                
                pointElement = pointElement->NextSiblingElement("point");
            }
            
            return true;
        }
    }
    
    return false;
}

//-----------------------------------------------------------------------------------------------------
void AiFlyingMobObject::update(float _dt)
{
    if(this->pGameObject)
    {
        if(!this->cyclic && this->passedPoints == this->wayPoints.size())
            return;
        
        if( this->beginPos.equals( Vec2::ZERO ) )
            this->beginPos = this->getGameObject()->getPosition();
        
        
        Vec2 targetPoint = Vec2::ZERO;
        
        if(this->passedPoints != this->wayPoints.size())
        {
            if(this->passedPoints == this->wayPoints.size() + 1)
                this->passedPoints = 0;
            
            targetPoint = this->wayPoints.at(this->passedPoints);
        }
        
        Vec2 nowPos = this->getGameObject()->getPosition();
        Vec2 globTargetPoint = this->beginPos + targetPoint;
        Vec2 moveVec = Vec2(globTargetPoint.x - nowPos.x, 0);
        float moveDist = moveVec.getLength();
        float moveStep = this->speed * _dt;
        
        if(moveDist >= moveStep)
        {
            Vec2 moveNormal = moveVec.getNormalized();
            this->changePosition( moveNormal * moveStep );
        }
        else
            this->passedPoints++;
    }
}

//-----------------------------------------------------------------------------------------------------
void AiFlyingMobObject::changePosition(const Vec2 _position) const
{
    if(this->pGameObject)
    {
        IPhysObject* physObj = this->pGameObject->getPhysic();
        if(physObj && physObj->getType() == phys_object_type::DYNAMIC)
        {
            auto physDynamicObj = dynamic_cast<IPhysDynamicObject*>(physObj);
            physDynamicObj->setImpulseVector( _position );
        }
    }
}

//-----------------------------------------------------------------------------------------------------
void AiFlyingMobObject::onDamaged( GameObject* _contactObj )
{
    //pushing up player removing mob's physic
    IPhysDynamicObject* physDynamicObj = dynamic_cast< IPhysDynamicObject* >( _contactObj->getPhysic() );
    if( physDynamicObj )
    {
        PhysDynamicPlayerObject* pPhysPlayer = dynamic_cast< PhysDynamicPlayerObject* >( physDynamicObj );
        if( pPhysPlayer )
        {
            pPhysPlayer->setFreeFallSpeed( 0.0f );
            pPhysPlayer->setJumpForce( 20, true );
        }
    }
    
    MANAGER_Level->getCurrentLevel()->addLocalKilledMobsId( this->getGameObject()->getId() );
    
    this->hp = 0;
    
    //coins add
    MANAGER_Level->getCurrentLevel()->setLocalPlayerCoins( MANAGER_Level->getCurrentLevel()->getLocalPlayerCoins() + 2 );
    MANAGER_Level->getCurrentLevel()->setLocalPlayerCoinsIntoPoints( MANAGER_Level->getCurrentLevel()->getLocalPlayerCoinsIntoPoints() + 2 );
    
    this->pGameObject->getPhysic()->setActivity( false );
    this->setActivity( false );
}

//-----------------------------------------------------------------------------------------------------