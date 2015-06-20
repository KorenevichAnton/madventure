//
//  ShopScreen.h
//  MusicalAdventure
//
//  Created by ampaUser on 5/11/15.
//  Copyright (c) 2015 ampagames. All rights reserved.
//

#ifndef __MusicalAdventure__ShopScreen__
#define __MusicalAdventure__ShopScreen__

#include "IScreen.h"

//---------------------------------------------------------------------------------------
class ShopScreen: public IScreen
{
public:
    static ShopScreen*              create();
    
    virtual void                    update( float _dt ) override;
    
private:
    ShopScreen();
    ~ShopScreen();
    
    bool                            init();
    void                            initButtons();
    
    void                            createDonatePlate( cocos2d::Vec2 _position, int _price, int _gameMoney );
    
    
private:
    void                            onCloseBtn( cocos2d::Ref* _sender );
    void                            onDonateBtn( cocos2d::Ref* _sender );
    
    struct DonateButtonInfo
    {
        int             index;
        int             price;
        int             gameMoney;
    };
    
    cocos2d::MenuItem*            createDonateButton( int _index, int _price, int _gameMoney );
    
private:
    cocos2d::Sprite*                      pBackgroundPanel;
    cocos2d::Menu*                        pShopMenu;
    std::vector<cocos2d::MenuItem*>       vDonateButtons;
};

//---------------------------------------------------------------------------------------
#endif /* defined(__MusicalAdventure__ShopScreen__) */
