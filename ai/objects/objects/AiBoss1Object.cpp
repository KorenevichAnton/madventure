//
//  AiBoss1Object.cpp
//  MusicalAdventure
//
//  Created by madmin on 6/25/14.
//  Copyright (c) 2014 ampagames. All rights reserved.
//

#include "AiBoss1Object.h"
#include "PhysDynamicBoss1Object.h"

#include "GameObject.h"
#include "IPhysDynamicObject.h"

#include "IGraphicObject.h"
#include "PhysObjectFactory.h"
#include "AiObjectFactory.h"
#include "GraphicObject.h"
#include "GameScene.h"
#include "IScene.h"

#include "AiShotObject.h"
#include "PhysDynamicPlayerObject.h"

#include "LevelManager.h"

#include "GraphicAnimationObject.h"

using namespace cocos2d;
using namespace tinyxml2;
using namespace ai;
using namespace physic;

//-----------------------------------------------------------------------------------------------------
AiBoss1Object::AiBoss1Object():
shotCount(8120),
shotDelay(2),
shotTimer(0),
canShot(false),
startBoss(false),
afterTouchState(false),
disableDmgTimeLimit( 2.5f ),
disableDmgTimer(0.0f),
hp( 0 ),
fullHp( 0 ),
beginPosition( Vec2::ZERO ),
targetPosition( 268, 24 ), //270 24
pathCounter( 0 )
{
    type = AI_BOSS1_OBJ;
    
    pGameObject = NULL;
}

//-----------------------------------------------------------------------------------------------------
AiBoss1Object::~AiBoss1Object()
{
}

//-----------------------------------------------------------------------------------------------------
AiBoss1Object* AiBoss1Object::create(XMLElement* _xmlAiObject)
{
    AiBoss1Object* pAi = new AiBoss1Object();
    if(pAi && pAi->initWithXml(_xmlAiObject))
        return pAi;
    else
    {
        delete pAi;
        return NULL;
    }
}

//-----------------------------------------------------------------------------------------------------
void AiBoss1Object::release()
{
    delete this;
}

//-----------------------------------------------------------------------------------------------------
bool AiBoss1Object::initWithXml(XMLElement* _xmlAiObject)
{
    if(_xmlAiObject)
    {
        //configuration
        XMLElement* configElement = _xmlAiObject->FirstChildElement("config");
        if(configElement)
        {
            hp = atof(configElement->Attribute("hp"));
            fullHp = atof(configElement->Attribute("hp"));
        }
        return true;
    }
    
    return false;
}

//-----------------------------------------------------------------------------------------------------
void AiBoss1Object::update(float _dt)
{
    if(startBoss)
    {
        if(this->pGameObject) //&& this->hp > 0)
        {
            //
            if(this->afterTouchState)
            {
                this->disableDmgTimer += _dt;
                if(this->disableDmgTimer > this->disableDmgTimeLimit)
                {
                    this->disableDmgTimer = 0.0f;
                    this->afterTouchState = false;
                }
            }
            //
            if( 75 > this->getHpRate() && this->pathCounter < 2 )
            {
                this->pathCounter = 2;
                this->setTargetBossPath();
            }
            if( 50 > this->getHpRate() && this->pathCounter < 4 )
            {
                this->pathCounter = 4;
                this->setTargetBossPath();
            }
            if( 30 > this->getHpRate() && this->pathCounter < 6 )
            {
                this->pathCounter = 6;
                this->setTargetBossPath();
            }
            if( this->getHp() <= 0 && this->pathCounter < 8)
            {
                this->pathCounter = 8;
                this->setTargetBossPath();
            }
                
            //Start shoot
            this->shotTimer += _dt;
            if(this->shotTimer >= this->shotDelay && this->canShot)
            {
                this->fire( _dt );
            }
            //boss moves
            Vec2 nowPos = this->getGameObject()->getPosition();
            Vec2 moveVec = Vec2( this->targetPosition - nowPos );
            float moveDist = moveVec.getLength();
            float moveStep = 10 * _dt;
            
            if(moveDist > moveStep)
            {
                Vec2 moveNormal = moveVec.getNormalized();
                this->changePosition( Vec2( moveNormal * moveStep ) );
            }
            else
            {
                this->setTargetBossPath();
            }
        }
    }
}

//-----------------------------------------------------------------------------------------------------
void AiBoss1Object::setTargetBossPath()
{
    //first path
    if( this->pathCounter == 0 )
    {
        //this->targetPosition = ccp( 230, 24 );15, 40
        this->targetPosition = Vec2( 230, 25 );
        this->pathCounter = 1;
    }
    else if( this->pathCounter == 1 )
    {
        this->targetPosition = Vec2( 268, 25 );//270, 2430, 42
        this->pathCounter = 0;
    }
    //second path
    else if ( this->pathCounter == 2 )
    {
        this->targetPosition = Vec2( 230, 11 );
        this->pathCounter = 3;
    }
    else if ( this->pathCounter == 3 )
    {
        this->targetPosition = Vec2( 268, 11 );
        this->pathCounter = 2;
    }
    //third path
    else if( this->pathCounter == 4 )
    {
        this->canShot = true;
        this->targetPosition = Vec2( 268, 28 );
        this->pathCounter = 5;
    }
    else if( this->pathCounter == 5 )
    {
        this->targetPosition = Vec2( 230, 28 );
        this->pathCounter = 4;
    }
    //fourth path
    else if( this->pathCounter == 6 )
    {
        this->canShot = false;
        this->targetPosition = Vec2( 237, 16 );
        this->pathCounter = 7;
    }
    else if( this->pathCounter == 7 )
    {
        this->targetPosition = Vec2( 235, 34 );
        this->pathCounter = 6;
    }
    //dead path
    else if ( this->pathCounter == 8 )
    {
        this->targetPosition = Vec2( 235, 17 );
        this->pathCounter = 9;
    }
}

//-----------------------------------------------------------------------------------------------------
void AiBoss1Object::getTargetBossPath()
{
    
}

//-----------------------------------------------------------------------------------------------------
void AiBoss1Object::fire(float _dt)
{
    this->shotTimer = 0;
    this->shotCount++;
    
    graphic::GraphicAnimationObject* animBoss = dynamic_cast<graphic::GraphicAnimationObject*>( this->getGameObject()->getGraphic() );

    animBoss->setCallbackEndAnimation( graphic::sequence_mob::SM_ATTACK, [this](){ this->createShoot(); } );
}

//-----------------------------------------------------------------------------------------------------
void AiBoss1Object::createShoot()
{
    XMLDocument objFile;
    if( objFile.LoadFile( GET_FULL_PATH("xml/physWorld/objects/bulets.xml") ) == XML_NO_ERROR  )
    {
        XMLElement* objRootElement = objFile.FirstChildElement("bulets");
        if(objRootElement)
        {
            XMLElement* objElement = objRootElement->FirstChildElement("object");
            while(objElement)
            {
                if(atoi(objElement->Attribute("id")) == 1)
                {
                    int sx = (int)floor(this->getGameObject()->getPhysic()->getPosition().x / 32);
                    int sy = (int)floor(this->getGameObject()->getPhysic()->getPosition().y / 24);
                    float px = (int)floor(this->getGameObject()->getPhysic()->getPosition().x - sx * 32 +
                                          ( this->getGameObject()->getDirection() == game_object_direction::RIGHT ? 2 : 0 ) );
                    float py = (int)floor(this->getGameObject()->getPhysic()->getPosition().y - 2 - sy * 24 - 1 );
                    
                    //create game object and add him to square
                    GameObject* pGameObject = GameObject::create(this->getGameObject()->getPhysic()->getGameWorld()->getWorldMatrix()[sx][sy], objElement, Vec2(px, py) );
                    this->getGameObject()->getPhysic()->getGameWorld()->getWorldMatrix()[sx][sy]->addGameObject(pGameObject);
                    //s
                    //create and add physic to game object
                    XMLElement* objPhysElement = objElement->FirstChildElement("physic");
                    if(objPhysElement)
                    {
                        IPhysObject* physObj = PhysObjectFactory::createPhysObject(objPhysElement, this->getGameObject()->getPhysic()->getGameWorld());
                        pGameObject->setPhysic(physObj);
                        pGameObject->setId(shotCount);
                        
                    }
                    //
                    
                    //create and add graphic to game object
                    XMLElement* objGraphicElement = objElement->FirstChildElement("graphic");
                    if(objGraphicElement)
                    {
                        graphic::IGraphicObject* graphicObj = graphic::GraphicObject::create(objGraphicElement);
                        pGameObject->setGraphic(graphicObj);
                        
                        Layer* graphic = graphicObj->getGraphicLayer();
                        graphic->setAnchorPoint(Vec2::ZERO);
                        dynamic_cast<GameScene*>( GET_RUNNING_SCENE )->getForegroundLayer()->addChild(graphic, 200);
                        if( this->getGameObject()->getDirection() == game_object_direction::RIGHT )
                        {
                            pGameObject->getGraphic()->getGraphicLayer()->setScaleX( -1 );
                        }
                    }
                    //
                    
                    //create and add ai to game object
                    XMLElement* objAiElement = objElement->FirstChildElement("ai");
                    if(objAiElement)
                    {
                        ai::IAiObject* aiObj = ai::AiObjectFactory::createAiObject(objAiElement);
                        dynamic_cast<AiShotObject*>(aiObj)->setPlayersShot(false);
                        pGameObject->setAi(aiObj);
                    }
                    this->getGameObject()->getPhysic()->getGameWorld()->addGameObjectToActive(pGameObject);
                    
                }
                objElement = objElement->NextSiblingElement("object");
            }
        }
    }
}

//-----------------------------------------------------------------------------------------------------
void AiBoss1Object::changePosition(const cocos2d::Vec2 _position) const
{
    if(this->pGameObject)
    {
        IPhysObject* physObj = this->pGameObject->getPhysic();
        if( physObj->getType() == phys_object_type::DYNAMIC )
        {
            auto selfPhysDynamicObj = dynamic_cast< IPhysDynamicObject* >( physObj );
            selfPhysDynamicObj->setImpulseVector( _position );
            
            if( _position.x > 0 )
                selfPhysDynamicObj->flipX( true );
            else
                selfPhysDynamicObj->flipX( false );
        }
    }
}

//-----------------------------------------------------------------------------------------------------
void AiBoss1Object::onDamaged( GameObject *_contactObj )
{
    if( this->hp > 0 )
    {
        if( _contactObj->getPhysic()->getType() == phys_object_type::DYNAMIC )
        {
            auto pPhysDynamicObj =  dynamic_cast< IPhysDynamicObject*>( _contactObj->getPhysic() );
            if( pPhysDynamicObj->getSubType() == phys_dynamic_object_type::SHOT )
            {
                auto pAiShot = dynamic_cast< ai::AiShotObject* >( _contactObj->getAi() );
                if( pAiShot )
                {
                    if( !this->afterTouchState && pAiShot->getPlayersShot() )
                    {
                        if( this->hp > 0 )
                        {
                            hp--;
                            this->afterTouchState = true;
                        }
                    }
                }
            }
            else
            {
                if(!this->afterTouchState)
                {
                    if( pPhysDynamicObj )
                    {
                        auto pPhysPlayer = dynamic_cast< PhysDynamicPlayerObject* >( pPhysDynamicObj );
                        if( pPhysPlayer )
                        {
                            pPhysPlayer->setFreeFallSpeed( 0.0f );
                            pPhysPlayer->setJumpForce( 20, true );
                            _contactObj->setPosition( Vec2( _contactObj->getPosition().x, _contactObj->getPosition().y + 1 ) );
                            if(this->hp > 0)
                            {
                                hp--;
                                this->afterTouchState = true;
                            }
                        }
                    }
                }
            }
        }
    }
    if( this->hp == 0 )
    {
        MANAGER_Level->getCurrentLevel()->hideGameObject( 24 );
        MANAGER_Level->getCurrentLevel()->hideGameObject( 25 );
        MANAGER_Level->getCurrentLevel()->hideGameObject( 26 );
        MANAGER_Level->getCurrentLevel()->hideGameObject( 27 );
    }
}

//-----------------------------------------------------------------------------------------------------