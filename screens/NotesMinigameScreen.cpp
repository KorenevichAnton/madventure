#include "NotesMinigameScreen.h"
#include "AppController.h"

#include "HudScreen.h"
#include "MusicalNotebook.h"

#include "NotesInfoScreen.h"

using namespace cocos2d;
using namespace std;

//---------------------------------------------------------------------------------------------------------------------------------
NotesMinigameScreen* NotesMinigameScreen::create()
{
    NotesMinigameScreen* pRet = new NotesMinigameScreen();
    if(pRet && pRet->init())
        return pRet;
    else
    {
        delete pRet;
        pRet = NULL;
    }
    
	return NULL;
}

//---------------------------------------------------------------------------------------------------------------------------------
NotesMinigameScreen::NotesMinigameScreen():
startTimer(false),
noteShowDuration(0)
{
    selfId = screen_id::NOTESMINIGAME;
}

//---------------------------------------------------------------------------------------------------------------------------------
NotesMinigameScreen::~NotesMinigameScreen()
{
}

//---------------------------------------------------------------------------------------------------------------------------------
bool NotesMinigameScreen::init()
{
    //super init first
    if( !IScreen::init() )
        return false;
    
    //
    m_Notebook = MusicalNotebook::create();
    m_Notebook->setVisible(true);
    m_Notebook->setCountNotesPage(20);
    m_Notebook->setStartPosLayer( Vec2(0, m_Notebook->getStartPosLayer().y) );
    m_Notebook->setStartPosNote( Vec2(43, m_Notebook->getStartPosNote().y) );
    m_Notebook->getBody()->setPosition(Vec2(150, 550));
    this->addChild(m_Notebook);
    //
    PianoKey* pk1 = PianoKey::create("textures/scenes/minigame/PianoPlay/complete.png", 0, 0.0f, 0.0f, 4, 0, "audio/minigame/piano/piano_C.wav", 0, 1, "textures/scenes/world/level1/1_1/notes/pressedComplete.png", "textures/scenes/minigame/PianoPlay/completeNotActive.png");
    pk1->getBody()->setVisible(false);
    PianoKey* pk2 = PianoKey::create("textures/scenes/minigame/PianoPlay/complete.png", 2, 0.0f, 0.0f, 4, 0, "audio/minigame/piano/piano_D.wav", 0, 2, "textures/scenes/world/level1/1_1/notes/pressedComplete.png", "textures/scenes/minigame/PianoPlay/completeNotActive.png");
    pk2->getBody()->setVisible(false);
    PianoKey* pk3 = PianoKey::create("textures/scenes/minigame/PianoPlay/complete.png", 4, 0.0f, 0.0f, 4, 0, "audio/minigame/piano/piano_E.wav", 0, 3, "textures/scenes/world/level1/1_1/notes/pressedComplete.png", "textures/scenes/minigame/PianoPlay/completeNotActive.png");
    pk3->getBody()->setVisible(false);
    PianoKey* pk4 = PianoKey::create("textures/scenes/minigame/PianoPlay/complete.png", 5, 0.0f, 0.0f, 4, 0, "audio/minigame/piano/piano_F.wav", 0, 4, "textures/scenes/world/level1/1_1/notes/pressedComplete.png", "textures/scenes/minigame/PianoPlay/completeNotActive.png");
    pk4->getBody()->setVisible(false);
    PianoKey* pk5 = PianoKey::create("textures/scenes/minigame/PianoPlay/complete.png", 7, 0.0f, 0.0f, 4, 0, "audio/minigame/piano/piano_G.wav", 0, 5, "textures/scenes/world/level1/1_1/notes/pressedComplete.png", "textures/scenes/minigame/PianoPlay/completeNotActive.png");
    pk5->getBody()->setVisible(false);
    PianoKey* pk6 = PianoKey::create("textures/scenes/minigame/PianoPlay/complete.png", 9, 0.0f, 0.0f, 4, 0, "audio/minigame/piano/piano_A.wav", 0, 6, "textures/scenes/world/level1/1_1/notes/pressedComplete.png", "textures/scenes/minigame/PianoPlay/completeNotActive.png");
    pk6->getBody()->setVisible(false);
    PianoKey* pk7 = PianoKey::create("textures/scenes/minigame/PianoPlay/complete.png", 11, 0.0f, 0.0f, 4, 0, "audio/minigame/piano/piano_H.wav", 0, 7, "textures/scenes/world/level1/1_1/notes/pressedComplete.png", "textures/scenes/minigame/PianoPlay/completeNotActive.png");
    pk7->getBody()->setVisible(false);
    
    m_Notebook->setIndent(80);
    
    m_Notebook->addNote(pk1);
    m_Notebook->addNote(pk2);
    m_Notebook->addNote(pk3);
    m_Notebook->addNote(pk4);
    m_Notebook->addNote(pk5);
    m_Notebook->addNote(pk6);
    m_Notebook->addNote(pk7);
    
    //pk7->getBacklightBody()->setVisible(true);
    //pk7->getUnselected()->setVisible(false);
    
    /* Initialize a listener of touches.
    auto listener = EventListenerTouchAllAtOnce::create();
    listener->onTouchesBegan = CC_CALLBACK_2( NotesMinigameScreen::onTouchesBegan, this );
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority( listener, this );
    */
    IScreen::initTouches( Touch::DispatchMode::ALL_AT_ONCE );
    
    return true;
}

//---------------------------------------------------------------------------------------------------------------------------------
void NotesMinigameScreen::onEnter()
{
    IScreen::onEnter();
    
    log("NotesMinigameScreen::onEnter");
}

//---------------------------------------------------------------------------------------------------------------------------------
void NotesMinigameScreen::onExit()
{
    IScreen::onExit();
    
    log("NotesMinigameScreen::onExit");
}

//---------------------------------------------------------------------------------------------------------------------------------
void NotesMinigameScreen::update(float _dt)
{
    
    
}

//---------------------------------------------------------------------------------------------------------------------------------
void NotesMinigameScreen::onTouchesBegan( const vector<Touch*>& _touches, Event* _event )
{
    Touch* touch = NULL;

    for (auto it = _touches.begin(); it != _touches.end(); ++it)
    {
        if (!(*it))
            break;
        
        touch = (Touch*)(*it);
        vector<PianoKey*> vec = m_Notebook->getNotes();
        Vec2 check = m_Notebook->getBody()->convertToNodeSpace(touch->getLocation());
        for (auto j = vec.begin(); j < vec.end(); j++)
        {
            if((*j)->getBody()->isVisible() && (*j)->containsPointUnselected(check))
            {
                //AudioManager::sharedInstance()->playSound((*j)->getSound().c_str(), false);
            }
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------
void NotesMinigameScreen::controlCallback1(Ref* _pSender)
{
    
    
}

//---------------------------------------------------------------------------------------------------------------------------------
void NotesMinigameScreen::controlCallback2(Ref* _pSender)
{
    
}

//---------------------------------------------------------------------------------------------------------------------------------
void NotesMinigameScreen::controlCallback3(Ref* _pSender)
{
    
}

//---------------------------------------------------------------------------------------------------------------------------------
void NotesMinigameScreen::showNote1Border()
{
    if( this )
    {
        NotesInfoScreen* notInfo = NotesInfoScreen::create( "textures/GUI/1024x768/notesInfo/posNotes/Do.png", L10N_STRING( 1301 ), "audio/minigame/piano/piano_C.wav" );
        this->addChild(notInfo);
        m_Notebook->getNotes()[0]->getBody()->setVisible(true);
        this->withBorder = true;
        dynamic_cast<HudScreen*>(MANAGER_Screen->getScreen(GET_RUNNING_SCENE, screen_id::HUD))->hideSticker();
    }
}

//---------------------------------------------------------------------------------------------------------------------------------
void NotesMinigameScreen::showNote2Border()
{
    NotesInfoScreen* notInfo = NotesInfoScreen::create( "textures/GUI/1024x768/notesInfo/posNotes/re.png",  L10N_STRING( 1302 ), "audio/minigame/piano/piano_D.wav" );
    this->addChild(notInfo);
    //m_Notebook->getNotes()[0]->getBacklightBody()->setVisible(false);
    m_Notebook->getNotes()[1]->getBody()->setVisible(true);
    this->withBorder = true;
    dynamic_cast<HudScreen*>(MANAGER_Screen->getScreen(GET_RUNNING_SCENE, screen_id::HUD))->hideSticker();
    /*m_Notebook->getNotes()[2]->getBacklightBody()->setVisible(false);
     m_Notebook->getNotes()[3]->getBacklightBody()->setVisible(false);
     m_Notebook->getNotes()[4]->getBacklightBody()->setVisible(false);
     m_Notebook->getNotes()[5]->getBacklightBody()->setVisible(false);
     m_Notebook->getNotes()[6]->getBacklightBody()->setVisible(false);
     //
     m_Notebook->getNotes()[0]->getUnselected()->setVisible(true);
     m_Notebook->getNotes()[1]->getUnselected()->setVisible(false);
     m_Notebook->getNotes()[2]->getUnselected()->setVisible(true);
     m_Notebook->getNotes()[3]->getUnselected()->setVisible(true);
     m_Notebook->getNotes()[4]->getUnselected()->setVisible(true);
     m_Notebook->getNotes()[5]->getUnselected()->setVisible(true);
     m_Notebook->getNotes()[6]->getUnselected()->setVisible(true);*/
}

//---------------------------------------------------------------------------------------------------------------------------------
void NotesMinigameScreen::showNote3Border()
{
    NotesInfoScreen* notInfo = NotesInfoScreen::create( "textures/GUI/1024x768/notesInfo/posNotes/mee.png",  L10N_STRING( 1303 ), "audio/minigame/piano/piano_E.wav" );
    this->addChild(notInfo);
    //m_Notebook->getNotes()[0]->getBacklightBody()->setVisible(false);
    //m_Notebook->getNotes()[1]->getBacklightBody()->setVisible(false);
    m_Notebook->getNotes()[2]->getBody()->setVisible(true);
    this->withBorder = true;
    dynamic_cast<HudScreen*>(MANAGER_Screen->getScreen(GET_RUNNING_SCENE, screen_id::HUD))->hideSticker();
    /*m_Notebook->getNotes()[3]->getBacklightBody()->setVisible(false);
     m_Notebook->getNotes()[4]->getBacklightBody()->setVisible(false);
     m_Notebook->getNotes()[5]->getBacklightBody()->setVisible(false);
     m_Notebook->getNotes()[6]->getBacklightBody()->setVisible(false);
     //
     m_Notebook->getNotes()[0]->getUnselected()->setVisible(true);
     m_Notebook->getNotes()[1]->getUnselected()->setVisible(true);
     m_Notebook->getNotes()[2]->getUnselected()->setVisible(false);
     m_Notebook->getNotes()[3]->getUnselected()->setVisible(true);
     m_Notebook->getNotes()[4]->getUnselected()->setVisible(true);
     m_Notebook->getNotes()[5]->getUnselected()->setVisible(true);
     m_Notebook->getNotes()[6]->getUnselected()->setVisible(true);*/
}

//---------------------------------------------------------------------------------------------------------------------------------
void NotesMinigameScreen::showNote4Border()
{
    NotesInfoScreen* notInfo = NotesInfoScreen::create( "textures/GUI/1024x768/notesInfo/posNotes/Fa.png",  L10N_STRING( 1304 ), "audio/minigame/piano/piano_F.wav" );
    this->addChild(notInfo);
    /*m_Notebook->getNotes()[0]->getBacklightBody()->setVisible(false);
     m_Notebook->getNotes()[1]->getBacklightBody()->setVisible(false);
     m_Notebook->getNotes()[2]->getBacklightBody()->setVisible(false);*/
    m_Notebook->getNotes()[3]->getBody()->setVisible(true);
    this->withBorder = true;
    dynamic_cast<HudScreen*>(MANAGER_Screen->getScreen(GET_RUNNING_SCENE, screen_id::HUD))->hideSticker();
    /*m_Notebook->getNotes()[4]->getBacklightBody()->setVisible(false);
     m_Notebook->getNotes()[5]->getBacklightBody()->setVisible(false);
     m_Notebook->getNotes()[6]->getBacklightBody()->setVisible(false);
     //
     m_Notebook->getNotes()[0]->getUnselected()->setVisible(true);
     m_Notebook->getNotes()[1]->getUnselected()->setVisible(true);
     m_Notebook->getNotes()[2]->getUnselected()->setVisible(true);
     m_Notebook->getNotes()[3]->getUnselected()->setVisible(false);
     m_Notebook->getNotes()[4]->getUnselected()->setVisible(true);
     m_Notebook->getNotes()[5]->getUnselected()->setVisible(true);
     m_Notebook->getNotes()[6]->getUnselected()->setVisible(true);*/
}

//---------------------------------------------------------------------------------------------------------------------------------
void NotesMinigameScreen::showNote5Border()
{
    NotesInfoScreen* notInfo = NotesInfoScreen::create( "textures/GUI/1024x768/notesInfo/posNotes/Sol.png",  L10N_STRING( 1305 ), "audio/minigame/piano/piano_G.wav" );
    this->addChild(notInfo);
    /*m_Notebook->getNotes()[0]->getBacklightBody()->setVisible(false);
     m_Notebook->getNotes()[1]->getBacklightBody()->setVisible(false);
     m_Notebook->getNotes()[2]->getBacklightBody()->setVisible(false);
     m_Notebook->getNotes()[3]->getBacklightBody()->setVisible(false);*/
    m_Notebook->getNotes()[4]->getBody()->setVisible(true);
    this->withBorder = true;
    dynamic_cast<HudScreen*>(MANAGER_Screen->getScreen(GET_RUNNING_SCENE, screen_id::HUD))->hideSticker();
    /*m_Notebook->getNotes()[5]->getBacklightBody()->setVisible(false);
     m_Notebook->getNotes()[6]->getBacklightBody()->setVisible(false);
     //
     m_Notebook->getNotes()[0]->getUnselected()->setVisible(true);
     m_Notebook->getNotes()[1]->getUnselected()->setVisible(true);
     m_Notebook->getNotes()[2]->getUnselected()->setVisible(true);
     m_Notebook->getNotes()[3]->getUnselected()->setVisible(true);
     m_Notebook->getNotes()[4]->getUnselected()->setVisible(false);
     m_Notebook->getNotes()[5]->getUnselected()->setVisible(true);
     m_Notebook->getNotes()[6]->getUnselected()->setVisible(true);*/
}

//---------------------------------------------------------------------------------------------------------------------------------
void NotesMinigameScreen::showNote6Border()
{
    NotesInfoScreen* notInfo = NotesInfoScreen::create( "textures/GUI/1024x768/notesInfo/posNotes/Lya.png",  L10N_STRING( 1306 ), "audio/minigame/piano/piano_A.wav" );
    this->addChild(notInfo);
    /*m_Notebook->getNotes()[0]->getBacklightBody()->setVisible(false);
     m_Notebook->getNotes()[1]->getBacklightBody()->setVisible(false);
     m_Notebook->getNotes()[2]->getBacklightBody()->setVisible(false);
     m_Notebook->getNotes()[3]->getBacklightBody()->setVisible(false);
     m_Notebook->getNotes()[4]->getBacklightBody()->setVisible(false);*/
    m_Notebook->getNotes()[5]->getBody()->setVisible(true);
    this->withBorder = true;
    dynamic_cast<HudScreen*>(MANAGER_Screen->getScreen(GET_RUNNING_SCENE, screen_id::HUD))->hideSticker();
    /*m_Notebook->getNotes()[6]->getBacklightBody()->setVisible(false);
     //
     m_Notebook->getNotes()[0]->getUnselected()->setVisible(true);
     m_Notebook->getNotes()[1]->getUnselected()->setVisible(true);
     m_Notebook->getNotes()[2]->getUnselected()->setVisible(true);
     m_Notebook->getNotes()[3]->getUnselected()->setVisible(true);
     m_Notebook->getNotes()[4]->getUnselected()->setVisible(true);
     m_Notebook->getNotes()[5]->getUnselected()->setVisible(false);
     m_Notebook->getNotes()[6]->getUnselected()->setVisible(true);*/
}

//---------------------------------------------------------------------------------------------------------------------------------
void NotesMinigameScreen::showNote7Border()
{
    NotesInfoScreen* notInfo = NotesInfoScreen::create( "textures/GUI/1024x768/notesInfo/posNotes/See.png",  L10N_STRING( 1307 ), "audio/minigame/piano/piano_H.wav" );
    this->addChild(notInfo);
    /*m_Notebook->getNotes()[0]->getBacklightBody()->setVisible(false);
     m_Notebook->getNotes()[1]->getBacklightBody()->setVisible(false);
     m_Notebook->getNotes()[2]->getBacklightBody()->setVisible(false);
     m_Notebook->getNotes()[3]->getBacklightBody()->setVisible(false);
     m_Notebook->getNotes()[4]->getBacklightBody()->setVisible(false);
     m_Notebook->getNotes()[5]->getBacklightBody()->setVisible(false);*/
    m_Notebook->getNotes()[6]->getBody()->setVisible(true);
    this->withBorder = true;
    dynamic_cast<HudScreen*>(MANAGER_Screen->getScreen(GET_RUNNING_SCENE, screen_id::HUD))->hideSticker();
    //
    /*m_Notebook->getNotes()[0]->getUnselected()->setVisible(true);
     m_Notebook->getNotes()[1]->getUnselected()->setVisible(true);
     m_Notebook->getNotes()[2]->getUnselected()->setVisible(true);
     m_Notebook->getNotes()[3]->getUnselected()->setVisible(true);
     m_Notebook->getNotes()[4]->getUnselected()->setVisible(true);
     m_Notebook->getNotes()[5]->getUnselected()->setVisible(true);
     m_Notebook->getNotes()[6]->getUnselected()->setVisible(false);*/
}

//---------------------------------------------------------------------------------------------------------------------------------
void NotesMinigameScreen::showNote1()
{
    m_Notebook->getNotes()[0]->getBody()->setVisible(true);
    m_Notebook->getNotes()[1]->getBody()->setVisible(false);
    m_Notebook->getNotes()[2]->getBody()->setVisible(false);
    m_Notebook->getNotes()[3]->getBody()->setVisible(false);
    m_Notebook->getNotes()[4]->getBody()->setVisible(false);
    m_Notebook->getNotes()[5]->getBody()->setVisible(false);
    m_Notebook->getNotes()[6]->getBody()->setVisible(false);
}

//---------------------------------------------------------------------------------------------------------------------------------
void NotesMinigameScreen::showNote2()
{
    m_Notebook->getNotes()[0]->getBody()->setVisible(false);
    m_Notebook->getNotes()[1]->getBody()->setVisible(true);
    m_Notebook->getNotes()[2]->getBody()->setVisible(false);
    m_Notebook->getNotes()[3]->getBody()->setVisible(false);
    m_Notebook->getNotes()[4]->getBody()->setVisible(false);
    m_Notebook->getNotes()[5]->getBody()->setVisible(false);
    m_Notebook->getNotes()[6]->getBody()->setVisible(false);
}

//---------------------------------------------------------------------------------------------------------------------------------
void NotesMinigameScreen::showNote3()
{
    m_Notebook->getNotes()[0]->getBody()->setVisible(false);
    m_Notebook->getNotes()[1]->getBody()->setVisible(false);
    m_Notebook->getNotes()[2]->getBody()->setVisible(true);
    m_Notebook->getNotes()[3]->getBody()->setVisible(false);
    m_Notebook->getNotes()[4]->getBody()->setVisible(false);
    m_Notebook->getNotes()[5]->getBody()->setVisible(false);
    m_Notebook->getNotes()[6]->getBody()->setVisible(false);
}

//---------------------------------------------------------------------------------------------------------------------------------
void NotesMinigameScreen::showNote4()
{
    m_Notebook->getNotes()[0]->getBody()->setVisible(false);
    m_Notebook->getNotes()[1]->getBody()->setVisible(false);
    m_Notebook->getNotes()[2]->getBody()->setVisible(false);
    m_Notebook->getNotes()[3]->getBody()->setVisible(true);
    m_Notebook->getNotes()[4]->getBody()->setVisible(false);
    m_Notebook->getNotes()[5]->getBody()->setVisible(false);
    m_Notebook->getNotes()[6]->getBody()->setVisible(false);
}

//---------------------------------------------------------------------------------------------------------------------------------
void NotesMinigameScreen::showNote5()
{
    m_Notebook->getNotes()[0]->getBody()->setVisible(false);
    m_Notebook->getNotes()[1]->getBody()->setVisible(false);
    m_Notebook->getNotes()[2]->getBody()->setVisible(false);
    m_Notebook->getNotes()[3]->getBody()->setVisible(false);
    m_Notebook->getNotes()[4]->getBody()->setVisible(true);
    m_Notebook->getNotes()[5]->getBody()->setVisible(false);
    m_Notebook->getNotes()[6]->getBody()->setVisible(false);
}

//---------------------------------------------------------------------------------------------------------------------------------
void NotesMinigameScreen::showNote6()
{
    m_Notebook->getNotes()[0]->getBody()->setVisible(false);
    m_Notebook->getNotes()[1]->getBody()->setVisible(false);
    m_Notebook->getNotes()[2]->getBody()->setVisible(false);
    m_Notebook->getNotes()[3]->getBody()->setVisible(false);
    m_Notebook->getNotes()[4]->getBody()->setVisible(false);
    m_Notebook->getNotes()[5]->getBody()->setVisible(true);
    m_Notebook->getNotes()[6]->getBody()->setVisible(false);
}

//---------------------------------------------------------------------------------------------------------------------------------
void NotesMinigameScreen::showNote7()
{
    m_Notebook->getNotes()[0]->getBody()->setVisible(false);
    m_Notebook->getNotes()[1]->getBody()->setVisible(false);
    m_Notebook->getNotes()[2]->getBody()->setVisible(false);
    m_Notebook->getNotes()[3]->getBody()->setVisible(false);
    m_Notebook->getNotes()[4]->getBody()->setVisible(false);
    m_Notebook->getNotes()[5]->getBody()->setVisible(false);
    m_Notebook->getNotes()[6]->getBody()->setVisible(true);
}

//---------------------------------------------------------------------------------------------------------------------------------
void NotesMinigameScreen::showHint()
{
    Sprite *pNoteSeries = Sprite::create("textures/scenes/minigame/PianoPlay/clearNotnyiSeries_03.png");
    m_Notebook->getNotnyiSeries()->setTexture(pNoteSeries->getTexture());
    m_Notebook->getNotnyiSeries()->setTextureRect(pNoteSeries->getTextureRect());
    
    m_Notebook->getNotes()[4]->getBody()->setVisible(true);
}

//---------------------------------------------------------------------------------------------------------------------------------
void NotesMinigameScreen::hideHint()
{
    Sprite *pNoteSeries = Sprite::create("textures/scenes/minigame/PianoPlay/clearNotnyiSeries.png");
    m_Notebook->getNotnyiSeries()->setTexture(pNoteSeries->getTexture());
    m_Notebook->getNotnyiSeries()->setTextureRect(pNoteSeries->getTextureRect());
    
    m_Notebook->getNotes()[4]->getBody()->setVisible(false);
}

//---------------------------------------------------------------------------------------------------------------------------------