#include "GameObject.h"
#include "GameSquare.h"
#include "GameWorld.h"

using namespace cocos2d;
using namespace tinyxml2;
using namespace physic;
using namespace graphic;
using namespace ai;

//-----------------------------------------------------------------------------------------------------
GameObject* GameObject::create( GameSquare* _parentSquare, XMLElement* _objectXML, const Vec2 _position )
{
    GameObject* pObject = new GameObject();
    if( pObject && pObject->initWithXML( _objectXML ) )
    {
        pObject->pParentSquare = _parentSquare;
        pObject->position = pObject->pParentSquare->getPositionInCells() + _position;
        return pObject;
    }
    else
    {
        delete pObject;
        return nullptr;
    }
}

//-----------------------------------------------------------------------------------------------------
void GameObject::release()
{
    // Remove this object from world.
    this->pParentSquare->removeGameObject( this );
    this->pParentSquare = nullptr;
    
    // Release physic module.
    if( this->pPhysic )
    {
        this->pPhysic->release();
        this->pPhysic = nullptr;
    }
    
    // Release graphic module.
    if( this->pGraphic )
    {
        this->pGraphic->release();
        this->pGraphic = nullptr;
    }
    
    // Release ai module.
    if( this->pAi )
    {
        this->pAi->release();
        this->pAi = nullptr;
    }
    
    delete this;
}

//-----------------------------------------------------------------------------------------------------
GameObject::GameObject():
id( 0 ),
pPhysic( nullptr ),
pGraphic( nullptr ),
pAi( nullptr ),
pParentSquare( nullptr ),
activity( true ),
position( Vec2::ZERO ),
location( game_object_location::NONE ),
direction( game_object_direction::RIGHT )
{
}

//-----------------------------------------------------------------------------------------------------
GameObject::~GameObject()
{
}

//-----------------------------------------------------------------------------------------------------
bool GameObject::initWithXML( XMLElement* _objectXML )
{
    if( _objectXML )
    {
        this->id = atoi( _objectXML->Attribute( "id" ) );
        this->activity = ( atoi( _objectXML->Attribute( "active" ) ) > 0 ) ? true : false;
        
        return true;
    }
    
    return false;
}

//-----------------------------------------------------------------------------------------------------
void GameObject::update( float _dt )
{
    // Dynamic reload of modules.
    this->dynamicReload();
    
    // Updating modules.
    if( this->activity )
    {
        if( this->pPhysic && this->pPhysic->getActivity() )
            this->pPhysic->update( _dt );
        
        if( this->pAi && this->pAi->getActivity() )
            this->pAi->update( _dt );
        
        if( this->pGraphic && this->pGraphic->getActivity() )
            this->pGraphic->update( _dt );
    }
}

//-----------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------
void GameObject::preloadResources()
{
    // Preload a graphic resources.
    if( this->pGraphic )
    {
        if( this->pGraphic->getReloadState() == reload_state::NONE )
            this->pGraphic->preloadGraphic();
    }
}

//-----------------------------------------------------------------------------------------------------
void GameObject::releaseResources()
{
    // Release a graphic resources.
    if( this->pGraphic )
    {
        if( this->pGraphic->getReloadState() != reload_state::NONE )
            this->pGraphic->releaseGraphic();
    }
}

//-----------------------------------------------------------------------------------------------------
void GameObject::attachResources()
{
    // Attach a graphic resources.
    if( this->pGraphic )
    {
        if( this->activity && this->pGraphic->getActivity() )
        {
            if( this->pGraphic->getReloadState() != reload_state::ATTACHED )
                this->pGraphic->attachGraphic();
        }
    }
}

//-----------------------------------------------------------------------------------------------------
void GameObject::detachResources()
{
    // Detach a graphic resources.
    if( this->pGraphic )
    {
        if( this->pGraphic->getReloadState() == reload_state::ATTACHED )
            this->pGraphic->detachGraphic();
    }
}

//-----------------------------------------------------------------------------------------------------
void GameObject::dynamicReload()
{
    // Reload graphic module.
    if( this->pGraphic )
    {
        if( this->activity && this->pGraphic->getActivity() )
        {
            if( this->pGraphic->getReloadState() != reload_state::ATTACHED  )
                this->pGraphic->attachGraphic();
        }
        else
        {
            if( this->pGraphic->getReloadState() == reload_state::ATTACHED )
                this->pGraphic->detachGraphic();
        }
    }
}

//-----------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------
void GameObject::setPhysic( IPhysObject* _pPhysic )
{
    this->pPhysic = _pPhysic;
    
    if( _pPhysic )
        this->pPhysic->setGameObject( this );
}

//-----------------------------------------------------------------------------------------------------
void GameObject::setGraphic( IGraphicObject* _pGraphic )
{
    this->pGraphic = _pGraphic;
    
    if( _pGraphic )
        this->pGraphic->setGameObject( this );
}

//-----------------------------------------------------------------------------------------------------
void GameObject::setAi( IAiObject* _pAi )
{
    this->pAi = _pAi;
    
    if( _pAi )
        this->pAi->setGameObject( this );
}

//-----------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------
void GameObject::setPosition( const Vec2& _position )
{
    this->position = _position;
    
    if( this->pGraphic )
        this->pGraphic->setPosition( _position );
    
    if( this->pPhysic )
        this->pPhysic->setPosition( _position );
}

//-----------------------------------------------------------------------------------------------------
void GameObject::onPhysicChangePosition( const Vec2& _position )
{
    if( !this->position.equals( _position ) )
    {
        this->position = _position;
        
        if( this->pGraphic )
            this->pGraphic->setPosition( _position );
    }
}

//-----------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------