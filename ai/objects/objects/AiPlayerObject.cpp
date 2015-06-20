//
//  AiPlayerObject.cpp
//  MusicalAdventure
//
//  Created by pool on 09.07.14.
//  Copyright (c) 2014 ampagames. All rights reserved.
//

#include "AiPlayerObject.h"
#include "GameObject.h"
#include "IPhysDynamicObject.h"

#include "GameScene.h"

#include "IGraphicObject.h"
#include "PhysObjectFactory.h"
#include "AiObjectFactory.h"
#include "GraphicObject.h"

#include "AiShotObject.h"
#include "AiBoss1Object.h"
#include "HudScreen.h"

#include "GraphicAnimationBulletPlayerObject.h"
#include "PhysDynamicPlayerObject.h"

using namespace cocos2d;
using namespace tinyxml2;
using namespace ai;
using namespace physic;

//on damaged time - shows how long player wont get damage after losing one health point
#define PLAYER_DAMAGED_TIMER           2.0f

//-----------------------------------------------------------------------------------------------------
AiPlayerObject::AiPlayerObject():
shotObjectId( 100000 ),
damagedState( false ),
damagedTime( 0.0f ),
deadState( false )
{
    type = AI_PLAYER_OBJ;
    
    pGameObject = NULL;
}

//-----------------------------------------------------------------------------------------------------
AiPlayerObject::~AiPlayerObject()
{
}

//-----------------------------------------------------------------------------------------------------
AiPlayerObject* AiPlayerObject::create( XMLElement* _xmlAiObject )
{
    AiPlayerObject* pAi = new AiPlayerObject();
    if(pAi && pAi->initWithXml(_xmlAiObject))
        return pAi;
    else
    {
        delete pAi;
        return NULL;
    }
}

//-----------------------------------------------------------------------------------------------------
void AiPlayerObject::release()
{
    delete this;
}

//-----------------------------------------------------------------------------------------------------
bool AiPlayerObject::initWithXml( XMLElement* _xmlAiObject )
{
    if( _xmlAiObject )
        return true;
    
    return false;
}

//-----------------------------------------------------------------------------------------------------
void AiPlayerObject::update( float _dt )
{
    if( this->pGameObject )
    {
        if( this->pGameObject->getLocation() != game_object_location::SPACE &&
            this->pGameObject->getGraphic()->getActivity() == false )
            this->pGameObject->getGraphic()->setActivity( true );
        
        if( this->damagedState )
        {
            this->damagedTime += _dt;
            if( this->damagedTime > PLAYER_DAMAGED_TIMER )
            {
                this->damagedState = false;
                this->damagedTime = 0.0f;
            }
        }
        
        //checking dead state for player for trymore screen ( on level 1_2_1 used at first )
        if( MANAGER_Memory->getTempData()->playerHP <= 0 )
            this->deadState = true;
        else
            this->deadState = false;
    }
}

//-----------------------------------------------------------------------------------------------------
void AiPlayerObject::fire()
{
    if( MANAGER_Memory->getTempData()->playerFireballAmmo > 0 )
    {
        auto pPhysPlayer = dynamic_cast< PhysDynamicPlayerObject* >( this->getGameObject()->getPhysic() );
        auto pGameWorld = dynamic_cast< GameWorld* > ( this->pGameObject->getPhysic()->getGameWorld() );
        
        MANAGER_Memory->getTempData()->playerFireballAmmo--;
        XMLDocument objFile;
        if( objFile.LoadFile( GET_FULL_PATH( "xml/physWorld/objects/bulets.xml" ) ) == XML_NO_ERROR  )
        {
            XMLElement* objRootElement = objFile.FirstChildElement( "bulets" );
            if( objRootElement )
            {
                XMLElement* objElement = objRootElement->FirstChildElement( "object" );
                while( objElement )
                {
                    if( atoi( objElement->Attribute( "id" ) ) == 0 )
                    {
                        int sx = ( int )floor( pPhysPlayer->getPosition().x / 32 );
                        int sy = ( int )floor( pPhysPlayer->getPosition().y / 24 );
                        float px = ( int )floor( pPhysPlayer->getPosition().x - sx * 32 );
                        float py = ( int )floor( pPhysPlayer->getPosition().y - sy * 24 );
                        
                        //create game object and add him to square
                        auto pGameObject = GameObject::create( pGameWorld->getWorldMatrix()[ sx ][ sy ], objElement, Vec2( px, py ) );
                        pGameWorld->getWorldMatrix()[ sx ][ sy ]->addGameObject( pGameObject );
                        //
                        pGameObject->setPosition( Vec2( pGameObject->getPosition().x - 2, pGameObject->getPosition().y - 0.3 ) ); //0.3
                        //create and add physic to game object
                        XMLElement* objPhysElement = objElement->FirstChildElement( "physic" );
                        if( objPhysElement )
                        {
                            IPhysObject* physObj = PhysObjectFactory::createPhysObject( objPhysElement, pGameWorld );
                            pGameObject->setPhysic( physObj );
                            pGameObject->setId( shotObjectId );
                            
                            pGameObject->setDirection( this->getGameObject()->getDirection() );
     
                            shotObjectId++;
                        }
                        //
                        
                        //create and add graphic to game object
                        XMLElement* objGraphicElement = objElement->FirstChildElement( "graphic" );
                        if( objGraphicElement )
                        {
                            graphic::GraphicAnimationBulletPlayerObject* graphicObj = graphic::GraphicAnimationBulletPlayerObject::create( objGraphicElement );
                            graphicObj->setRepetition( true );
                            pGameObject->setGraphic( graphicObj );
                            
                            Layer* graphic = graphicObj->getGraphicLayer();
                            graphic->setAnchorPoint( Vec2::ZERO );
                            
                            dynamic_cast< GameScene* >( GET_RUNNING_SCENE )->getForegroundLayer()->addChild( graphic, 2001 );
                        }
                        //
                        
                        //create and add ai to game object
                        XMLElement* objAiElement = objElement->FirstChildElement( "ai" );
                        if( objAiElement )
                        {
                            ai::IAiObject* aiObj = ai::AiObjectFactory::createAiObject( objAiElement );
                            dynamic_cast< AiShotObject* >( aiObj )->setPlayersShot( true );
                            pGameObject->setAi( aiObj );
                        }
                        pGameWorld->addGameObjectToActive( pGameObject );
                        
                    }
                    objElement = objElement->NextSiblingElement( "object" );
                }
            }
        }
    }
}

//-----------------------------------------------------------------------------------------------------
void AiPlayerObject::moveLeft()
{
    PhysDynamicPlayerObject* pPhysPlayer = dynamic_cast< PhysDynamicPlayerObject* >( this->getGameObject()->getPhysic() );
    
    float force = pPhysPlayer->getMoveForce();
    float speed = 0.0f;
    
    if( force > 0 )
        speed = PLAYER_MOVE_MIN_FORCE * ( -1.0f );
    else
        speed = PLAYER_MOVE_MAX_FORCE * ( -1.0f );
    
    pPhysPlayer->setMoveForce( speed );
}

//------------------------------------------------------------------------------------------------------
void AiPlayerObject::moveRight()
{
    PhysDynamicPlayerObject* pPhysPlayer = dynamic_cast< PhysDynamicPlayerObject* >( this->getGameObject()->getPhysic() );
    
    float force = pPhysPlayer->getMoveForce();
    float speed = 0.0f;
    
    if( force < 0 )
        speed = PLAYER_MOVE_MIN_FORCE;
    else
        speed = PLAYER_MOVE_MAX_FORCE;
    
    pPhysPlayer->setMoveForce( speed );
}

//-----------------------------------------------------------------------------------------------------
void AiPlayerObject::jump()
{
    PhysDynamicPlayerObject* pPhysPlayer = dynamic_cast< PhysDynamicPlayerObject* >( this->getGameObject()->getPhysic() );
    
    if( pPhysPlayer )
        pPhysPlayer->setJumpForce( PLAYER_JUMP_FORCE );
}

//-----------------------------------------------------------------------------------------------------
void AiPlayerObject::stop()
{
    PhysDynamicPlayerObject* pPhysPlayer = dynamic_cast< PhysDynamicPlayerObject* >( this->getGameObject()->getPhysic() );
    pPhysPlayer->setMoveForce( 0.0f );
}

//-----------------------------------------------------------------------------------------------------
void AiPlayerObject::onDamaged( GameObject* _contactObj )
{
    if( !this->damagedState )
    {
        if( _contactObj->getPhysic()->getType() == phys_object_type::DYNAMIC )
        {
            //check if shoting player - dont deal player damage
            auto pPhysDynamicObj = dynamic_cast< IPhysDynamicObject* >( _contactObj->getPhysic() );
            
            if( pPhysDynamicObj->getSubType() == phys_dynamic_object_type::SHOT )
            {
                auto pAiShot = dynamic_cast< ai::AiShotObject* >( _contactObj->getAi() );
                if( pAiShot )
                    if( pAiShot->getPlayersShot() )
                        return;
            }
            
            //check if bosses below zero - dont deal player damage
            if( pPhysDynamicObj->getSubType() == phys_dynamic_object_type::BOSS1 )
            {
                auto pAiBoss1 = dynamic_cast< ai::AiBoss1Object* >( _contactObj->getAi() );
                if( pAiBoss1 )
                    if( pAiBoss1->getHp() <= 0 || pAiBoss1->isTouchable() )
                        return;
            }
        }
        
        IScreen* pHudScreen = MANAGER_Screen->getScreen( GET_RUNNING_SCENE, screen_id::HUD );
        if( pHudScreen )
            dynamic_cast< HudScreen* >( pHudScreen )->resetSticker();
        
        MANAGER_Memory->getTempData()->playerHP--;
        this->stop();
        this->damagedState = true;
    }
}

//-----------------------------------------------------------------------------------------------------