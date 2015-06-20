#ifndef __DIALOG_SCREEN__
#define __DIALOG_SCREEN__

#include "IScreen.h"
#include "IGUIInterface.h"

//---------------------------------------------------------------------------------------------------------------------------------
class DialogScreen: public IScreen
{
public:
	static DialogScreen*	create();
    
	void                    update(float _dt);

    
    bool                    loadDialogFromXml(int _id,
                                              int _minPageNum = 0,
                                              int _maxPageNum = 0,
                                              const char* _dialogImage = "",
                                              cocos2d::Vec2 _dialogImagePosition =  cocos2d::Vec2::ZERO,
                                              float _dialogImageScale = 1.0f);
    
    void                    setActionOnClose(const std::function<void()> &_functionCallback) { m_functionCallBackOnClose = _functionCallback; }
    
    
    void                    showDialog();
    void                    hideDialog();
    
    bool                    setNextDialog();
    bool                    isDialgEmpty() { return !dialogContented; };
    
    //callbacks
    void                    callbackPrevious(cocos2d::Ref* _pSender);
    void                    callbackNext(cocos2d::Ref* _pSender);
    void                    callbackClose(cocos2d::Ref* _pSender);

private:
    DialogScreen();
    ~DialogScreen();
    
    bool                    init();
    
    
private:
    IAudioSource*               aSource;  // for play a clicking sound of buttons
    
    bool                        onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    std::function<void()>       m_functionCallBackOnClose;
    
    int                         curDialogId;
    cocos2d::Sprite*            m_backDialog;
    
    int                         curPage;
    int                         minPageNum;
    int                         maxPageNum;
    
    std::vector<std::string>    pages;
    
    cocos2d::Vec2               dialogPos;
    float                       dialogScale;
    
    cocos2d::Label*             dialogueText;
    //
    const char*                 dialogImage;
    cocos2d::Vec2               dialogImagePosition;
    float                       dialogImageScale;
    //
    bool                        dialogContented;
};

//---------------------------------------------------------------------------------------------------------------------------------
#endif //__DIALOG_SCREEN__
