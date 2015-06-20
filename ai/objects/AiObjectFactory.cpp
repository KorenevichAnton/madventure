//
//  AiObjectFactory.cpp
//  MusicalAdventure
//
//  Created by madmin on 5/18/14.
//  Copyright (c) 2014 ampagames. All rights reserved.
//

#include "AiObjectFactory.h"

#include "AiPlayerObject.h"
#include "AiWalkingMobObject.h"
#include "AiFlyingMobObject.h"
#include "AiMovablePlatformObject.h"
#include "AiBoss1Object.h"
#include "AiShotObject.h"
#include "AiNpcBatObject.h"

using namespace std;
using namespace tinyxml2;
using namespace ai;

//-----------------------------------------------------------------------------------------------------
IAiObject* AiObjectFactory::createAiObject(XMLElement* _xmlAiObject)
{
    IAiObject* aiObject = NULL;
    
    std::string type = _xmlAiObject->Attribute("type");
    if(type == (string)"movable")
    {
        aiObject = AiMovablePlatformObject::create(_xmlAiObject);
    }
    else if(type == (string)"walkingMob")
    {
        aiObject = AiWalkingMobObject::create(_xmlAiObject);
    }
    else if(type == (string)"boss1")
    {
        aiObject = AiBoss1Object::create(_xmlAiObject);
    }
    else if(type == (string)"shot")
    {
        aiObject = AiShotObject::create(_xmlAiObject);
    }
    else if(type == (string)"player")
    {
        aiObject = AiPlayerObject::create(_xmlAiObject);
    }
    else if(type == (string)"flyingMob")
    {
        aiObject = AiFlyingMobObject::create(_xmlAiObject);
    }
    else if(type == (string)"npcBat")
    {
        aiObject = AiNpcBatObject::create(_xmlAiObject);
    }
    
    return aiObject;
}

//-----------------------------------------------------------------------------------------------------
