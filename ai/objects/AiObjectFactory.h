//
//  AiObjectFactory.h
//  MusicalAdventure
//
//  Created by madmin on 5/18/14.
//  Copyright (c) 2014 ampagames. All rights reserved.
//

#ifndef __MusicalAdventure__AiObjectFactory__
#define __MusicalAdventure__AiObjectFactory__

#include "IAiObject.h"

//-----------------------------------------------------------------------------------------------------
namespace ai
{
    
class AiObjectFactory
{
public:
    static IAiObject*	createAiObject( tinyxml2::XMLElement* _xmlAiObject );
    
private:
    AiObjectFactory(){};
    ~AiObjectFactory(){};
};
    
}   //namespase ai

//-----------------------------------------------------------------------------------------------------
#endif /* defined(__MusicalAdventure__AiObjectFactory__) */
