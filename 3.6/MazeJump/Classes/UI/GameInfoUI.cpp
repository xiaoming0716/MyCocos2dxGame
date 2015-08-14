//
//  GameUI.cpp
//
//
//
//

#include "GameInfoUI.h"
#include "UtilityHelper.h"
#include "GameConst.h"
#include "MainScene.h"
#include "UIManager.h"
#include "GameConst.h"
#include "storage/local-storage/LocalStorage.h"
USING_NS_CC;

GameInfoUI* GameInfoUI::create()
{
    GameInfoUI *pRet = new(std::nothrow) GameInfoUI();
    if (pRet&& pRet->init() )
    {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return nullptr;
}

GameInfoUI::GameInfoUI()
{

}
GameInfoUI::~GameInfoUI()
{
}

bool GameInfoUI::init()
{

    auto size = Director::getInstance()->getVisibleSize();
    float scale = size.width /640.0f;
    
    disLayer = cocos2d::Layer::create();
    addChild(disLayer);
    
    ui::ImageView* glodView = ui::ImageView::create("ui_gold_bar.png");
    glodView->setPosition(Vec2(size.width*0.15, size.height*0.95));
    glodView->setScale(scale);
    addChild(glodView);
    
    goldTv = ui::Text::create(StringUtils::format("%d",Value(localStorageGetItem(USER_GOLD_NUM)).asInt()), FONT_FXZS, 25);
    goldTv->setPosition(Vec2(size.width*0.15+25*scale,size.height*0.95 ));
    goldTv->setScale(scale);
    addChild(goldTv);
    
    ui::ImageView* heartView = ui::ImageView::create("ui_heart_bar.png");
    heartView->setPosition(Vec2(size.width*0.13, size.height*0.95-glodView->getContentSize().height*scale - 5*scale));
    heartView->setScale(scale);
    addChild(heartView);
    
    heartTv = ui::Text::create(StringUtils::format("%d",Value(localStorageGetItem(USER_HEART_NUM)).asInt()), FONT_FXZS, 25);
    heartTv->setPosition(Vec2(size.width*0.13+22*scale, size.height*0.95-glodView->getContentSize().height*scale - 5*scale));
    heartTv->setScale(scale);
    addChild(heartTv);
    
    
    ui::ImageView* disImg = ui::ImageView::create("ui_distance.png");
    disImg->setPosition(Vec2(size.width*0.53, size.height*0.95));
    disImg->setScale(scale);
    disLayer->addChild(disImg);

    gameScoreTv = ui::Text::create(StringUtils::format(UtilityHelper::getLocalString("GAME_DISTANCE").c_str(),Value(localStorageGetItem(USER_LAST_LEVEL)).asInt()), FONT_FXZS, 25);
    gameScoreTv->setPosition(Vec2(size.width*0.48, size.height*0.95));
    gameScoreTv->setScale(scale);
    disLayer->addChild(gameScoreTv);
    




    return true;
}

 void GameInfoUI::onEnter()
{
    Layer::onEnter();
    

    Director::getInstance()->getEventDispatcher()->addCustomEventListener(EVENT_GOLD_CHANGE, std::bind(&GameInfoUI::onGoldChange, this, std::placeholders::_1));
    Director::getInstance()->getEventDispatcher()->addCustomEventListener(EVENT_HEART_CHANGE, std::bind(&GameInfoUI::onHeartChange, this, std::placeholders::_1));
    Director::getInstance()->getEventDispatcher()->addCustomEventListener(EVENT_MAX_DISTANCE_CHANGE, std::bind(&GameInfoUI::onMaxDistanceChange, this, std::placeholders::_1));

}
 void GameInfoUI::onExit()
{
    Layer::onExit();
    Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(EVENT_GOLD_CHANGE);
    Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(EVENT_HEART_CHANGE);
    Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(EVENT_MAX_DISTANCE_CHANGE);

    
}

void GameInfoUI::onhideEndPopup()
{
    
}
void GameInfoUI::onGoldChange(cocos2d::EventCustom* sender)
{
    if(goldTv)
        goldTv->setString(localStorageGetItem(USER_GOLD_NUM));
}
void GameInfoUI::onHeartChange(cocos2d::EventCustom* sender)
{
    if (heartTv)
        heartTv->setString(localStorageGetItem(USER_HEART_NUM));
}
void GameInfoUI::onMaxDistanceChange(cocos2d::EventCustom* sender)
{
    if (gameScoreTv)
        gameScoreTv->setString(StringUtils::format(UtilityHelper::getLocalString("GAME_DISTANCE").c_str(),Value(localStorageGetItem(USER_LAST_LEVEL)).asInt()));
}



