#ifndef __MusicalAdventure__NotesMinigameScreen__
#define __MusicalAdventure__NotesMinigameScreen__

#include "IMiniGameScreen.h"
#include "MusicalNotebook.h"
#include "PianoKey.h"

//---------------------------------------------------------------------------------------------------------------------------------
class NotesMinigameScreen: public IMiniGameScreen
{
public:
    static NotesMinigameScreen*     create();
    
private:
    bool                            init();
    
    void                            update(float _dt);
    
    void                            onEnter();
    void                            onExit();
    
    NotesMinigameScreen();
    ~NotesMinigameScreen();
    
    //touch events
    void                            onTouchesBegan( const std::vector<cocos2d::Touch*>& _touches, cocos2d::Event* _event );
    
private:
    void                            controlCallback1(cocos2d::Ref* _pSender);
    void                            controlCallback2(cocos2d::Ref* _pSender);
    void                            controlCallback3(cocos2d::Ref* _pSender);
    
private:
    
    MusicalNotebook*                m_Notebook;
    
    int                             noteShowDuration;
    
public:
    const MusicalNotebook*          getNotebook() const { return m_Notebook; }
    
    void                            setNoteShowDuration(int _noteShowDuration) { noteShowDuration = _noteShowDuration; }
    const int                       getNoteShowDuration() { return noteShowDuration; }
    
    //FUCKING TEMP : for minigame on level 1_1_1
    bool                            startTimer;
    
    
    void                            setWithBorder(bool _state) { withBorder = _state; }
    bool                            getWithBorder() { return withBorder; }
    
    void                            showNote1Border();
    void                            showNote2Border();
    void                            showNote3Border();
    void                            showNote4Border();
    void                            showNote5Border();
    void                            showNote6Border();
    void                            showNote7Border();
    //
    void                            showNote1();
    void                            showNote2();
    void                            showNote3();
    void                            showNote4();
    void                            showNote5();
    void                            showNote6();
    void                            showNote7();
    //
    void                            showHint();
    void                            hideHint();
    //
private:
    bool                            withBorder;
};

//---------------------------------------------------------------------------------------------------------------------------------

#endif /* defined(__MusicalAdventure__NotesMinigameScreen__) */
