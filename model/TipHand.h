//
//  TipHand.h
//  MusicalAdventure
//
//  Created by ampaUser on 4/30/15.
//  Copyright (c) 2015 ampagames. All rights reserved.
//

#ifndef __MusicalAdventure__TipHand__
#define __MusicalAdventure__TipHand__

#include <cocos2d.h>

//--------------------------------------------------------------------------------------------------------
class TipHand : public cocos2d::Node
{
public:
    static TipHand*                         create( float _angle, float _duration, float _speed );
    
    void                                    start();
    void                                    stop();
    
private:
    TipHand( float _angle, float _duration, float _speed );
    ~TipHand();
    
    bool                                    init() override;
    
    void                                    callbackMoveForward();
    void                                    callbackMoveBack();
    void                                    callbackStopping();
    
private:
    float                                   angle;
    float                                   duration;
    float                                   speed;
    
    int                                     handHintDirection;
    
    bool                                    stopState;
    
    cocos2d::Sprite*                        handSprite;
};

//--------------------------------------------------------------------------------------------------------
#endif /* defined(__MusicalAdventure__TipHand__) */