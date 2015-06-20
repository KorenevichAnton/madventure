#include "ILevel.h"

#include "AppController.h"
#include "GameObject.h"

#include "DialogScreen.h"
#include "HudScreen.h"

#include "GKHWrapperCpp.h"

using namespace std;
using namespace cocos2d;
using namespace physic;
using namespace ai;

//-----------------------------------------------------------------------------------------------------
ILevel::ILevel():
pGameWorld( nullptr ),
pPlayer( nullptr ),
pGameCamera( nullptr ),
nextLevel( level_id::NONE ),
level( level_id::NONE ),
areaType( AREA_NONE ),
levelGameWorldPath( nullptr ),
levelGameObjectsPath( nullptr ),
localPlayerCoins( 0 ),
winTimer( 0.0f ),
winTimerLimit( 0.0f ),
winState( false ),
localPlayerCoinsIntoPoints( 0 )
{
    this->vLocalGetCoinIds.clear();
    this->vLocalKilledMobsIds.clear();
}

//-----------------------------------------------------------------------------------------------------
void ILevel::setGameWorld( GameWorld* _pGameWorld )
{
    //creating back/embiant music
    this->embiantSource = MANAGER_Audio->createStreamingSource();
    this->backMusicSource = MANAGER_Audio->createStreamingSource();
    
    //set player data:
    //if player respauning on start level - reset player data
    if( MANAGER_Memory->getTempData()->cpPlayerHP == 0 )
    {
        //start at first on location (first check-point)
        MANAGER_Memory->getTempData()->resetPlayerData();
        //ADDING CORRECT LOCATION RESTART
        this->localPlayerCoinsIntoPoints = MANAGER_Memory->getTempData()->playerCoinsIntoPoints;
    }
    else
    {
        //start at check-point
        MANAGER_Memory->getTempData()->playerHP = MANAGER_Memory->getTempData()->cpPlayerHP;
        MANAGER_Memory->getTempData()->playerFireballAmmo = MANAGER_Memory->getTempData()->cpPlayerFireballAmmo;
        this->localPlayerCoins = MANAGER_Memory->getTempData()->playerCoins;
        //
        this->localPlayerCoinsIntoPoints = MANAGER_Memory->getTempData()->playerCoinsIntoPoints;
        //
        this->vLocalGetCoinIds = MANAGER_Memory->getTempData()->vGetCoinIds;
        this->vLocalKilledMobsIds = MANAGER_Memory->getTempData()->vKilledMobsIds;
    }
    
    //uploading all states of current game progress
    this->localLevelStates = MANAGER_Memory->getTempData()->globalLevelStates;
    
    this->pDialogScreen = dynamic_cast< DialogScreen* > ( MANAGER_Screen->getScreen( GET_RUNNING_SCENE, screen_id::DIALOG ) );
    this->pHudScreen = dynamic_cast< HudScreen* >( MANAGER_Screen->getScreen( GET_RUNNING_SCENE, screen_id::HUD) );
    
    this->pGameWorld = _pGameWorld;
    
    //searching for player gameobject and init pPlayer with player's ai
    const landscape worldMatrix = this->pGameWorld->getWorldMatrix();
    for(int i = 0; i < this->pGameWorld->getWorldWidth(); i++)
	{
		for(int j = 0; j < this->pGameWorld->getWorldHeight(); j++)
		{
			if(worldMatrix[i][j])
            {
                vector<GameObject*> gameObjectsList = worldMatrix[i][j]->getGameObjects();
                for(vector<GameObject*>::const_iterator it = gameObjectsList.begin(); it < gameObjectsList.end(); it++)
                {
                    IAiObject* aiObj = (*it)->getAi();
                    if(aiObj && aiObj->getType() == AI_PLAYER_OBJ)
                        this->pPlayer = dynamic_cast<AiPlayerObject*>(aiObj);
                }
            }
		}
	}
    
    this->pGameCamera = pGameWorld->getGameCamera();
}

//-----------------------------------------------------------------------------------------------------
GameObject* ILevel::getGameObjectById(int _id)
{
    for(int i = 0 ; i < pGameWorld->getWorldWidth() ; i++)
    {
        for(int j = 0 ; j < pGameWorld->getWorldHeight() ; j++)
        {
            for (std::vector<GameObject*>::const_iterator it = pGameWorld->getWorldMatrix()[i][j]->getGameObjects().begin(); it < pGameWorld->getWorldMatrix()[i][j]->getGameObjects().end(); it++)
            {
                if((*it)->getId() == _id)
                {
                    return (*it);
                }
            }
        }
    }
    return NULL;
}

//-----------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------
void ILevel::resetAllLevelStates()
{
    for( auto i = this->localLevelStates.begin() ; i != this->localLevelStates.end() ; ++i )
        (*i).second = false;
    this->localLevelStates.clear();
}

//-----------------------------------------------------------------------------------------------------
int ILevel::getLevelState( std::string _id ) const
{
    auto it = this->localLevelStates.find( _id );
    return ( it != this->localLevelStates.end() ) ? it->second : false;
}

//-----------------------------------------------------------------------------------------------------
void ILevel::setLevelState( string _id, int _state )
{
    this->localLevelStates[ _id ] = _state;
}

//-----------------------------------------------------------------------------------------------------
float ILevel::cameraSpeedCalc()
{
    float distance = pPlayer->getGameObject()->getPosition().getDistance( pGameCamera->getPosition() );
    return exp( -distance / CAMERA_FOLLOW_ACCELERATION ) * CAMERA_FOLLOW_SPEED;
}

//-----------------------------------------------------------------------------------------------------
void ILevel::uploadLevelStates()
{
    MANAGER_Memory->getTempData()->globalLevelStates = this->localLevelStates;
    
    GKHWrapperCpp gkh;
    gkh.submitScoreToCatagory( MANAGER_Memory->getTempData()->playerCoins, "com.myGame.myLearderBoard" );
}

//-----------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------
void ILevel::showGameObject(int _id)
{
    if( this->getGameObjectById( _id ) )
    {
        this->getGameObjectById(_id)->setActivity(true);
        this->pGameWorld->addGameObjectToActive(this->getGameObjectById(_id));
    }
}

//-----------------------------------------------------------------------------------------------------
void ILevel::hideGameObject(int _id)
{
    if( this->getGameObjectById( _id ) )
        this->getGameObjectById(_id)->setActivity(false);
}

//-----------------------------------------------------------------------------------------------------