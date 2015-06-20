//
//  L1_1_Quest_2_4_MGScreen.cpp
//  MusicalAdventure
//
//  Created by madmin on 4/22/14.
//  Copyright (c) 2014 ampagames. All rights reserved.
//

#include "L1_1_Quest_2_4_MGScreen.h"
#include "MainMenuScene.h"
#include "LevelManager.h"

using namespace std;
using namespace cocos2d;
using namespace tinyxml2;

//---------------------------------------------------------------------------------------------------------------------------------
L1_1_Quest_2_4_MGScreen* L1_1_Quest_2_4_MGScreen::create()
{
    L1_1_Quest_2_4_MGScreen* pRet = new L1_1_Quest_2_4_MGScreen();
    if(pRet && pRet->init())
        return pRet;
    else
    {
        delete pRet;
        pRet = NULL;
    }
    
	return NULL;
}

//-----------------------------------------------------------------------------------------------------
L1_1_Quest_2_4_MGScreen::L1_1_Quest_2_4_MGScreen():
curQNode(NULL),
curQuastion(0),
lQuastion(NULL),
qCount(0),
timer(60.0f),
aSourceError( nullptr )
{
    this->selfId = screen_id::MG_L1_1_QUEST_2_4;
    qNodes.clear();
    answersButtons.clear();
}

//-----------------------------------------------------------------------------------------------------
L1_1_Quest_2_4_MGScreen::~L1_1_Quest_2_4_MGScreen()
{
    
}

//-----------------------------------------------------------------------------------------------------
bool L1_1_Quest_2_4_MGScreen::init()
{
    if(!IMiniGameScreen::init())
        return false;
    
    //Init touches
    IMiniGameScreen::initTouches( Touch::DispatchMode::ONE_BY_ONE );
    
    this->_touchListener->setEnabled( false );
    //
    
    if(initWithXML(0) == false)
        return false;
    
    this->minigameControl = MiniGameControls::create();
    this->addChild(this->minigameControl, 100000);
    this->minigameControl->configOnStartButton([this](){ this->onStartGame(); });
    this->minigameControl->configOnVictoryButton([this](){ this->onVictory() ; });
    
    this->testWindow = Sprite::create("textures/scenes/minigame/L1_1_Quest_2_4_MGScreen/testWindow.png");
    this->testWindow->setAnchorPoint( Vec2::ZERO );
    this->testWindow->setPosition( Vec2( 512.0f - this->testWindow->getContentSize().width / 2.0f, 384.0f - this->testWindow->getContentSize().height / 2.0f ) );
    //
    Sprite* timeField = Sprite::create( "textures/scenes/minigame/L1_1_Quest_2_4_MGScreen/fieldTime.png" );
    timeField->setAnchorPoint( Vec2::ZERO );
    timeField->setPosition( Vec2( 240, 590 ) );
    this->testWindow->addChild( timeField );
    //
    this->lTimer = Label::createWithTTF( "01:00", FONT_PATH, 28 );
    this->lTimer->setAnchorPoint( Vec2( 0, 0 ) );
    this->lTimer->setPosition( Vec2( 305, 620 ) );
    this->lTimer->setColor( Color3B::BLACK );
    this->testWindow->addChild( this->lTimer );
    //
    
    Sprite* questionField = Sprite::create("textures/scenes/minigame/L1_1_Quest_2_4_MGScreen/fieldQuestion.png");
    questionField->setAnchorPoint(Vec2(0, 0));
    questionField->setPosition(Vec2(120, 420));
    this->testWindow->addChild(questionField);
    
    //
    this->lQuastion = Label::createWithTTF( "", FONT_PATH, 28, Size( 450, 128 ) , TextHAlignment::CENTER );
    this->lQuastion->setAnchorPoint(Vec2(0, 0));
    this->lQuastion->setPosition(Vec2(140, 390));
    this->lQuastion->setColor( Color3B::BLACK);
    
    this->lQuastion1 = Label::createWithTTF("", FONT_PATH, 28);
    this->lQuastion1->setAnchorPoint(Vec2(0, 0));
    this->lQuastion1->setPosition(Vec2(150, 440));
    this->lQuastion1->setColor( Color3B::BLACK );
    
    this->testWindow->addChild( this->lQuastion );
    this->testWindow->addChild( this->lQuastion1 );

    
    
    Label* lbl = NULL;
    
    Sprite* buttonAnsw = NULL;
    
    for (int i = 0; i < 4; i++)
    {
        char* str = new char[100];
        sprintf(str, "textures/scenes/minigame/L1_1_Quest_2_4_MGScreen/fieldAnswer%d.png", i + 1);
        buttonAnsw = Sprite::create(str);
        buttonAnsw->setAnchorPoint(Vec2(0, 0));
        buttonAnsw->setPosition(Vec2(140, 45 + i * 90));
        lbl = Label::createWithTTF( "", FONT_PATH, 32 );
        lbl->setPosition(Vec2(buttonAnsw->getContentSize().width / 2, 50));
        lbl->setColor( Color3B::BLACK );
        
        buttonAnsw->addChild(lbl);
        this->testWindow->addChild(buttonAnsw);
        delete []str;
        answersLables.push_back(lbl);
        answersButtons.push_back(buttonAnsw);
    }
    this->showQuastion(curQuastion);
    
    this->addChild(this->testWindow);
    
    this->aSourceError = MANAGER_Audio->createStaticSource();
    if( this->aSourceError )
        this->aSourceError->setAssociate(AudioSourceAssociate::SOUND);
    
    return true;
}

//---------------------------------------------------------------------------------------------------------------------------------
void L1_1_Quest_2_4_MGScreen::update(float _dt)
{
    if(this->_touchListener->isEnabled())
    {
        this->timer -= _dt;
        char* str = new char[5];
        if((int)this->timer < 10)
            sprintf(str, "00:0%d", (int)this->timer);
        else
            sprintf(str, "00:%d", (int)this->timer);
        lTimer->setString(str);
        if(this->timer <= 0)
        {
            this->onDefeat();
        }
        delete []str;
    }
}

//---------------------------------------------------------------------------------------------------------------------------------
bool L1_1_Quest_2_4_MGScreen::onTouchBegan( cocos2d::Touch *_pTouch, cocos2d::Event *_pEvent )
{
    return true;
}

//---------------------------------------------------------------------------------------------------------------------------------
void L1_1_Quest_2_4_MGScreen::onTouchEnded( cocos2d::Touch *_pTouch, cocos2d::Event *_pEvent )
{
    Vec2 lPoint = this->testWindow->convertTouchToNodeSpace( _pTouch );
    
    int j = 0;
    bool isRight = false;
    bool isPressButton = false;
    
    for(std::vector<Sprite*>::iterator i = answersButtons.begin() ; i < answersButtons.end() ; i++)
    {
        if((*i)->boundingBox().containsPoint(lPoint))
        {
            isPressButton = true;
            if(curQNode->getRightAnswer() == j)
            {
                isRight = true;
                if(qCount > curQuastion + 1)
                {
                    curQuastion+=1;
                    showQuastion(curQuastion);
                    this->minigameControl->showCorrect();
                }
                else
                {
                    this->minigameControl->showCorrect();
                    this->onVictory();
                }
            }
        }
        j++;
    }
    
    if(isPressButton && isRight == false)
    {
        this->minigameControl->showWrong();
        if( this->aSourceError )
        {
            if ( this->aSourceError->loadAudioFile( "audio/minigame/L1_1_Quest_2_4_MGScreen/error.wav" ) )
            {
                this->aSourceError->loadSettings( "audio/minigame/L1_1_Quest_2_4_MGScreen/error.wav" );
                this->aSourceError->play();
            }
        }
        
        this->onDefeat();
    }
}

//-----------------------------------------------------------------------------------------------------
bool L1_1_Quest_2_4_MGScreen::initWithXML(int _id)
{
    qNodes.clear();
    qCount = 0;
    
    XMLDocument testFile;
	if( testFile.LoadFile( FileUtils::getInstance()->fullPathForFilename( "xml/minigame/qTest.xml" ).c_str() ) == XML_NO_ERROR )
	{
        
        XMLElement* rootElement = testFile.FirstChildElement("qTest");
		if(rootElement)
		{
            XMLElement* testElement = rootElement->FirstChildElement("test");
            while (testElement)
            {
                if(atoi(testElement->Attribute("id")) == 0)
                {
                    XMLElement* quastionElement = testElement->FirstChildElement("quastion");
                    while (quastionElement)
                    {
                        QNode* qNode = QNode::create(quastionElement);
                        qNodes.push_back(qNode);
                        quastionElement = quastionElement->NextSiblingElement("quastion");
                        //
                        qCount++;
                    }
                }
                testElement = testElement->NextSiblingElement("test");
            }
            
            return true;
        }
    }
    return false;
}

//---------------------------------------------------------------------------------------------------------------------------------
void L1_1_Quest_2_4_MGScreen::onVictory()
{
    IMiniGameScreen::onVictory();

    MANAGER_Level->getCurrentLevel()->onMinigameEnd(true, screen_id::MG_L1_1_QUEST_2_4);
}

//-----------------------------------------------------------------------------------------------------
void L1_1_Quest_2_4_MGScreen::onDefeat()
{
    IMiniGameScreen::onDefeat();
    MANAGER_Level->getCurrentLevel()->onMinigameEnd(false, screen_id::MG_L1_1_QUEST_2_4);
}

//-----------------------------------------------------------------------------------------------------
bool L1_1_Quest_2_4_MGScreen::showQuastion(int _id)
{
    //if(_id == 1)
    //{
    //    lQuastion->setString("На какой строчки нотного стана пишется скрипичный ключ");
        //lQuastion1->setString("    ");
    //}
    //else
        lQuastion->setString(qNodes[_id]->getQText().c_str());
    
    std::vector<std::string> vAnswers = qNodes[_id]->getVAnswers();
    
    int j = 0;
    for(std::vector<std::string>::iterator i = vAnswers.begin() ; i < vAnswers.end() ; i++)
    {
        std::string str = (*i);

        (*(answersLables.begin() + j))->setString(str.c_str());
        j++;
            
    }
    curQNode = qNodes[_id];
    return true;
}

//---------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------