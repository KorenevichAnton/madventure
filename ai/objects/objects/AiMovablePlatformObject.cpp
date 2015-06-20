//
//  AiMovablePlatformObject.cpp
//  MusicalAdventure
//
//  Created by madmin on 5/18/14.
//  Copyright (c) 2014 ampagames. All rights reserved.
//

#include "AiMovablePlatformObject.h"
#include "GameObject.h"
#include "IPhysStaticObject.h"
#include "IPhysDynamicObject.h"

using namespace cocos2d;
using namespace tinyxml2;
using namespace ai;
using namespace physic;

//-----------------------------------------------------------------------------------------------------
AiMovablePlatformObject::AiMovablePlatformObject():
cyclic(false),
speed(0.0f), 
passedPoints(0),
beginPos(0,0),
movable(false)
{
    type = AI_MOVABLE_PLATFORM_OBJ;
    
    pGameObject = NULL;
}

//-----------------------------------------------------------------------------------------------------
AiMovablePlatformObject::~AiMovablePlatformObject()
{
}

//-----------------------------------------------------------------------------------------------------
AiMovablePlatformObject* AiMovablePlatformObject::create(XMLElement* _xmlAiObject)
{
    AiMovablePlatformObject* pAi = new AiMovablePlatformObject();
    if(pAi && pAi->initWithXml(_xmlAiObject))
        return pAi;
    else
    {
        delete pAi;
        return NULL;
    }
}

//-----------------------------------------------------------------------------------------------------
void AiMovablePlatformObject::release()
{
    delete this;
}

//-----------------------------------------------------------------------------------------------------
bool AiMovablePlatformObject::initWithXml(XMLElement* _xmlAiObject)
{
    if(_xmlAiObject)
    {
        XMLElement* wayElement = _xmlAiObject->FirstChildElement("way");
        if(wayElement)
        {
            this->cyclic = ( atoi(wayElement->Attribute("cyclic")) > 0 ) ? true : false;
            
            this->speed = atof(wayElement->Attribute("speed"));
        
            this->movable = ( atoi(wayElement->Attribute("move")) > 0) ? true : false;
            
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
void AiMovablePlatformObject::update(float _dt)
{
    if(this->pGameObject)
    {
        if(this->movable)
        {
            if( !this->cyclic && this->passedPoints == this->wayPoints.size() )
            {
                this->movable = false;
                return;
            }
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
            Vec2 globTargetPoint = Vec2( this->beginPos + targetPoint );
            Vec2 moveVec = Vec2( globTargetPoint - nowPos );
            float moveDist = moveVec.getLength();
            float moveStep = this->speed * _dt;
            
            if(moveDist >= moveStep)
            {
                Vec2 moveNormal = moveVec.getNormalized();
                this->changePosition( Vec2( moveNormal * moveStep ) );
            }
            else
                this->passedPoints++;
        }
        else
        {
            this->changePosition( Vec2::ZERO );
        }
    }
}

//-----------------------------------------------------------------------------------------------------
void AiMovablePlatformObject::changePosition( const Vec2 _position ) const
{
    IPhysObject* selfPhys = this->pGameObject->getPhysic();
    if( selfPhys )
    {
        if( selfPhys->getType() == phys_object_type::DYNAMIC )
        {
            dynamic_cast< IPhysDynamicObject* >( selfPhys )->setImpulseVector( _position );
        }
    }
}

//-----------------------------------------------------------------------------------------------------