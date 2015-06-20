//
//  MinigamesMenuScene.h
//  MusicalAdventure
//
//  Created by Anton Korenevich on 10/22/14.
//  Copyright (c) 2014 ampagames. All rights reserved.
//

#ifndef __MusicalAdventure__MinigamesMenuScene__
#define __MusicalAdventure__MinigamesMenuScene__

#include "IScene.h"
#include "AppController.h"

#include "IGUIInterface.h"

//---------------------------------------------------------------------------------------------------------------------------------
class MinigamesMenuScene: public IScene, public IGUIInterface
{
public:
    static MinigamesMenuScene*  create();
    
    virtual void                clean() override;
    
    void                        update(float _dt);
    
private:
    MinigamesMenuScene();
    ~MinigamesMenuScene();
    
    
    bool                        init();
    
    void                        initBackground();
    void                        initButtons();
    
    void                        onEnter();
    void                        onExit();
};

//---------------------------------------------------------------------------------------------------------------------------------

#endif /* defined(__MusicalAdventure__MinigamesMenuScene__) */
