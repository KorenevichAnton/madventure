#include "DialogScreen.h"
#include "AppController.h"

#define FONT_SIZE_DIALOG    20

using namespace cocos2d;
using namespace tinyxml2;

//---------------------------------------------------------------------------------------------------------------------------------
DialogScreen* DialogScreen::create()
{
    DialogScreen* pRet = new DialogScreen();
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
DialogScreen::DialogScreen():
curDialogId(0),
curPage(0),
minPageNum(0),
maxPageNum(0),
m_backDialog(NULL),
dialogPos(0, 0),
dialogContented(false),
dialogImage(""),
dialogImagePosition(Vec2::ZERO)
{
    selfId = screen_id::DIALOG;
}

//---------------------------------------------------------------------------------------------------------------------------------
DialogScreen::~DialogScreen()
{
}

//---------------------------------------------------------------------------------------------------------------------------------
bool DialogScreen::init()
{
    //super init first
    if( !IScreen::init() )
        return false;

    /* PORT
    this->setTouchEnabled(true);
    this->setTouchMode(ccTouchesMode::kCCTouchesOneByOne);
    this->setTouchPriority(9);
    */
    
    // init player for button clicking sounds
    this->aSource = MANAGER_Audio->createStaticSource( "audio/gui/btn_press.wav", true );
    if( this->aSource )
        this->aSource->setAssociate( AudioSourceAssociate::SOUND );
    
    /* Initialize a listener of touches.
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2( DialogScreen::onTouchBegan, this );
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority( listener, this );
    */
    IScreen::initTouches( Touch::DispatchMode::ONE_BY_ONE );
    
    return true;
}

//---------------------------------------------------------------------------------------------------------------------------------
bool DialogScreen::loadDialogFromXml(int _id, int _minPageNum, int _maxPageNum, const char* _dialogImage, Vec2 _dialogImagePosition, float _dialogImageScale)
{
    if(strcmp(_dialogImage, ""))
    {
        this->dialogImage = _dialogImage;
        this->dialogImagePosition = _dialogImagePosition;
        this->dialogImageScale = _dialogImageScale;
    }
    
    XMLDocument dialoguesFile;
    if( dialoguesFile.LoadFile( GET_FULL_PATH("xml/dialogues/dialogues.xml") ) == XML_NO_ERROR )
    {
        XMLElement* rootElement = dialoguesFile.FirstChildElement("dialogues");
        if(rootElement)
        {
            XMLElement* dialogElement = rootElement->FirstChildElement("dialog");
            while(dialogElement)
            {
                if( atoi(dialogElement->Attribute("id") ) == _id)
                {
                    curDialogId = _id;
                    this->dialogPos.x = atof(dialogElement->Attribute("posX"));
                    this->dialogPos.y = atof(dialogElement->Attribute("posY"));
                    this->dialogScale = atof(dialogElement->Attribute("scale"));
                    
                    this->minPageNum = 0;
                    this->maxPageNum = 0;
                    
                    XMLElement* pageElement = dialogElement->FirstChildElement("page");
                    while(pageElement)
                    {
                        int id = atoi( pageElement->GetText() );
                        
                        if( SHOW_DIALOGUES_ID ) //FOR TEST
                            pages.push_back( dialogElement->Attribute("id") ); //FOR TEST
                        else
                            pages.push_back( L10N_STRING( id ) );

                        this->maxPageNum += 1;

                        
                        pageElement = pageElement->NextSiblingElement("page");
                    }
                    
                    this->maxPageNum -= 1;
                    
                    this->showDialog();
                    
                    return true;
                }
                
                dialogElement = dialogElement->NextSiblingElement("dialog");
            }
        }
    }
    
    
    return false;
}

//---------------------------------------------------------------------------------------------------------------------------------
void DialogScreen::update(float _dt)
{
}

//---------------------------------------------------------------------------------------------------------------------------------
bool DialogScreen::onTouchBegan(Touch *pTouch, Event *pEvent)
{
    //CCTouch* touch = dynamic_cast<CCTouch*>(_touches->anyObject());
    if(m_backDialog && m_backDialog->boundingBox().containsPoint( pTouch->getLocation() ))
    {
        this->callbackClose(this);
        //this->dialogContented = false;
        return true;
    }
    else
        return false;
}

//---------------------------------------------------------------------------------------------------------------------------------
void DialogScreen::showDialog()
{
    this->dialogContented = true;
    Layer* layer = Layer::create();
    layer->setPosition(Vec2::ZERO);
    this->addChild(layer);
    
    m_backDialog = CCSprite::create("textures/screens/dialogues/dialog.png");
    m_backDialog->setPosition( FRAME_CENTER );
    m_backDialog->setScale(dialogScale);
    layer->addChild(m_backDialog, 0);
    
    

    
    this->dialogueText = Label::createWithTTF( this->pages.at(0).c_str(),
                                       FONT_PATH,
                                       FONT_SIZE_DIALOG,
                                       Size(m_backDialog->getContentSize().width * (m_backDialog->getScale() * 0.8), m_backDialog->getContentSize().height * (m_backDialog->getScale() * 1.0)),
                                       TextHAlignment::CENTER,
                                       TextVAlignment::CENTER );
    
    this->dialogueText->setContentSize( Size(m_backDialog->getContentSize().width * (m_backDialog->getScale() * 0.8), m_backDialog->getContentSize().height * (m_backDialog->getScale() * 1.0)) );
    this->dialogueText->setColor( FONT_COLOR_TITLE );
    this->dialogueText->setAnchorPoint(Vec2(0.5, 0.5));
    this->dialogueText->setPosition(this->dialogPos);
    layer->addChild(this->dialogueText, 10);
    
    
    MenuItemImage* btnClose = MenuItemImage::create("btn/close.png", "btn/close_D.png", CC_CALLBACK_1( DialogScreen::callbackClose, this ) );
    btnClose->setPosition(Vec2( m_backDialog->getContentSize().width / 1.02f, m_backDialog->getContentSize().height / 1.08f ) );
    btnClose->setScale(1.0f / m_backDialog->getScale());
    
    Menu* menu = Menu::create(btnClose, nullptr);
    menu->setPosition(Vec2(0, 0));
    m_backDialog->addChild(menu, 10);
    
    /* PORT
    menu->setTouchPriority( 0 );
    */
    
    if(strcmp(this->dialogImage, ""))
    {
        this->dialogueText->setPosition(Vec2(this->dialogueText->getPosition().x, this->dialogueText->getPosition().y + 30));
        Sprite* pDialogImage = Sprite::create(this->dialogImage);
        pDialogImage->setPosition(dialogImagePosition);
        pDialogImage->setScale(this->dialogImageScale);
        m_backDialog->addChild(pDialogImage, 22);
    }
    
}

//---------------------------------------------------------------------------------------------------------------------------------
void DialogScreen::hideDialog()
{
    m_backDialog = NULL;
    pages.clear();
    this->removeAllChildren();
}

//---------------------------------------------------------------------------------------------------------------------------------
void DialogScreen::callbackPrevious(cocos2d::Ref* _pSender)
{
    if(this->curPage - 1 >= this->minPageNum)
    {
        this->curPage--;
        this->dialogueText->setString( this->pages.at(this->curPage).c_str() );
    }
}

//---------------------------------------------------------------------------------------------------------------------------------
void DialogScreen::callbackNext(cocos2d::Ref* _pSender)
{
    if(this->curPage + 1 <= this->maxPageNum)
    {
        this->curPage++;
        this->dialogueText->setString( this->pages.at(this->curPage).c_str() );
    }
}

//---------------------------------------------------------------------------------------------------------------------------------
bool DialogScreen::setNextDialog()
{
    if(this->curPage + 1 <= this->maxPageNum)
    {
        this->curPage++;
        this->dialogueText->setString( this->pages.at(this->curPage).c_str() );
        return true;
    }
    else
    {
        return false;
    }
}

//---------------------------------------------------------------------------------------------------------------------------------
void DialogScreen::callbackClose(cocos2d::Ref* _pSender)
{
    if( this->aSource )
        this->aSource->play();
    
    if(setNextDialog())
        return;
    else
    {
        this->curPage = 0;
        this->hideDialog();
        this->dialogContented = false;
        
        //call logic from level, done after dialogue end
        MANAGER_Level->getCurrentLevel()->onDialogueEnd( this->curDialogId );
    }
}

//---------------------------------------------------------------------------------------------------------------------------------