//
//  RunController.cpp
//  MazeJump
//
//  Created by wang haibo on 15/7/22.
//
//

#include "RunController.h"
#include "ui/CocosGUI.h"
#include "MainScene.h"
#include "storage/local-storage/LocalStorage.h"
USING_NS_CC;

RunController* g_pRunControllerInstance = nullptr;
RunController* RunController::getInstance()
{
    if( g_pRunControllerInstance == nullptr )
        g_pRunControllerInstance = new RunController();
    
    return g_pRunControllerInstance;
}
RunController::RunController()
{
    m_pMainLayer        = nullptr;
    m_pTerrainLayer     = nullptr;
    m_pMainCamera       = nullptr;
    m_pMainPlayer       = nullptr;
    m_pCloud1           = nullptr;
    m_pCloud2           = nullptr;
    m_pCloud3           = nullptr;
    m_nDifficultLevel   = 0;
    m_nInitDifficultLevel = 0;
    m_nMaxReachDifficultLevel = 0;
}
RunController::~RunController()
{
}
bool RunController::init(Layer* pMainLayer)
{
    if(pMainLayer == nullptr)
        return false;
    m_pMainLayer = pMainLayer;
    m_nInitDifficultLevel = getDifficultLevel();
    
    Skybox* m_pSkyBox = Skybox::create("sky4.png", "sky4.png", "sky4.png", "sky4.png", "sky4.png", "sky4.png");
    if(!m_pSkyBox)
        return false;
    m_pSkyBox->setScale(1000);
    m_pSkyBox->setCameraMask((unsigned short)CameraFlag::USER1);
    m_pSkyBox->setGlobalZOrder(-1);
    m_pMainLayer->addChild(m_pSkyBox);
    
    if(!initCloud())
        return false;
    
    m_pTerrainLayer = TerrainLayer::create();
    if(!m_pTerrainLayer)
        return false;
    m_pTerrainLayer->setCameraMask((unsigned short)CameraFlag::USER1);
    ///focus
    m_pTerrainLayer->setAnchorPoint(Vec2::ZERO);
    m_pMainLayer->addChild(m_pTerrainLayer);
    
    
    m_pMainPlayer = Runner::create();
    if(!m_pMainPlayer)
        return false;
    m_pMainPlayer->setCameraMask((unsigned short)CameraFlag::USER1);
    m_pTerrainLayer->addChild(m_pMainPlayer);
    m_pMainPlayer->setState(Runner::RS_IDLE);
    
    auto size = Director::getInstance()->getVisibleSize();
    m_pMainCamera = Camera::createPerspective(60, size.width/size.height, 1, 5000);
    if(!m_pMainCamera)
        return false;
    Vec3 camPos = Vec3(m_pTerrainLayer->getPositionX(),m_pTerrainLayer->getPositionY(),m_pMainPlayer->getPositionZ()) + Vec3(0,80.0f*cosf(M_PI/6.0f),80.0f*sinf(M_PI/6.0f));
    Vec3 lookAt = Vec3(m_pTerrainLayer->getPositionX(),m_pTerrainLayer->getPositionY(),m_pMainPlayer->getPositionZ()) + Vec3(0,0,-30);
    m_pMainCamera->setPosition3D(camPos);
    m_pMainCamera->lookAt(lookAt);
    m_pTerrainLayer->addChild(m_pMainCamera);
    m_pMainCamera->setCameraFlag(CameraFlag::USER1);
    
    AmbientLight* ambLight = AmbientLight::create(Color3B(150, 150, 150));
    m_pMainLayer->addChild(ambLight);
    DirectionLight* directionLight = DirectionLight::create(Vec3(-2, -4, -3), Color3B(158, 158, 158));
    m_pMainLayer->addChild(directionLight);
    
    cocos2d::ui::Button* button = cocos2d::ui::Button::create("button_retry_up.png",
                                                              "button_retry_down.png");
    button->setPosition(Vec2(size.width * 0.8f, size.height * 0.8f));
    button->setPressedActionEnabled(true);
    button->addClickEventListener([=](Ref* sender){
        auto scene = MainScene::createScene();
        Director::getInstance()->replaceScene(scene);
    });
    m_pMainLayer->addChild(button);
    
    return true;
}
void RunController::update(float delta)
{
    if(m_pTerrainLayer)
        m_pTerrainLayer->update(delta);
    if(m_pMainPlayer)
        m_pMainPlayer->update(delta);
}
void RunController::destroy()
{
    m_pMainLayer->removeAllChildren();
    m_pMainLayer = nullptr;
}
int RunController::getDifficultLevel()
{
    m_nDifficultLevel = Value(localStorageGetItem("LastReachDifficultLevel")).asInt();
    return m_nDifficultLevel;
}
void RunController::setDifficultLevel(int difficult)
{
    m_nDifficultLevel = difficult;
    localStorageSetItem("LastReachDifficultLevel", Value(m_nDifficultLevel).asString());
    CCLOG("Current Difficult Level %d", m_nDifficultLevel);
    if(m_nDifficultLevel > getMaxDifficultLevel())
        setMaxDifficultLevel(m_nDifficultLevel);
}
int RunController::getMaxDifficultLevel()
{
    m_nMaxReachDifficultLevel = Value(localStorageGetItem("MaxReachDifficultLevel")).asInt();
    return m_nMaxReachDifficultLevel;
}
void RunController::setMaxDifficultLevel(int difficult)
{
    m_nMaxReachDifficultLevel = difficult;
    localStorageSetItem("LastReachDifficultLevel", Value(m_nMaxReachDifficultLevel).asString());
    CCLOG("Max reach Difficult Level %d", m_nMaxReachDifficultLevel);
}
void RunController::cameraTrackPlayer()
{
    if(m_pMainPlayer && m_pMainCamera)
    {
        Vec3 camPos = Vec3(m_pTerrainLayer->getPositionX(),m_pTerrainLayer->getPositionY(),m_pMainPlayer->getPositionZ()) + Vec3(0,80.0f*cosf(M_PI/6.0f),80.0f*sinf(M_PI/6.0f));
        Vec3 targetLookAt = Vec3(m_pTerrainLayer->getPositionX(),m_pTerrainLayer->getPositionY(),m_pMainPlayer->getPositionZ()) + Vec3(0,0,-30);
        EaseSineOut* moveTo = EaseSineOut::create(MoveTo::create(0.4f, camPos));
        m_pMainCamera->runAction(moveTo);
        if(m_pMainCamera->getPositionZ() - m_pMainPlayer->getPositionZ() > 80.0f*sinf(M_PI/6.0f))
            updateCloud();
    }
}
void RunController::gameOver()
{
    CCLOG("gameOver");
    setDifficultLevel(0);
}
bool RunController::initCloud()
{
    m_pCloud1 = Sprite3D::create("cloud.c3b");
    if(!m_pCloud1)
        return false;
    m_pCloud1->setCameraMask((unsigned short)CameraFlag::USER1);
    m_pCloud1->setPositionZ(-200);
    m_pCloud1->setPositionY(-80);
    m_pCloud1->setScaleX(2.5f);
    m_pCloud1->setScaleY(2.5f);
    m_pCloud1->setScaleZ(2.5f);
    m_pCloud1->setForceDepthWrite(true);
    m_pMainLayer->addChild(m_pCloud1);
    
    m_pCloud2 = Sprite3D::create("cloud.c3b");
    if(!m_pCloud2)
        return false;
    m_pCloud2->setCameraMask((unsigned short)CameraFlag::USER1);
    m_pCloud2->setPositionZ(-400);
    m_pCloud2->setPositionY(-80);
    m_pCloud2->setOpacity(0);
    m_pCloud2->setScaleX(2.5f);
    m_pCloud2->setScaleY(2.5f);
    m_pCloud2->setScaleZ(2.5f);
    m_pCloud2->setRotation3D(Vec3(0,180,0));
    m_pCloud2->setForceDepthWrite(true);
    m_pMainLayer->addChild(m_pCloud2);
    
    m_pCloud3 = Sprite3D::create("cloud.c3b");
    if(!m_pCloud2)
        return false;
    m_pCloud3->setCameraMask((unsigned short)CameraFlag::USER1);
    m_pCloud3->setPositionZ(0);
    m_pCloud3->setPositionY(-80);
    m_pCloud3->setOpacity(0);
    m_pCloud3->setScaleX(2.5f);
    m_pCloud3->setScaleY(2.5f);
    m_pCloud3->setScaleZ(2.5f);
    m_pCloud3->setRotation3D(Vec3(0,180,0));
    m_pCloud3->setForceDepthWrite(true);
    m_pMainLayer->addChild(m_pCloud3);
    return true;
}
void RunController::updateCloud()
{
    if(m_pCloud1 && m_pCloud2 && m_pCloud3 && m_pMainPlayer)
    {
        if(m_pMainPlayer->getPositionZ() == m_pCloud1->getPositionZ() || m_pMainPlayer->getPositionZ() == m_pCloud1->getPositionZ() + m_pTerrainLayer->getCellBaseRadius()*2)
        {
            m_pCloud3->setPosition3D(m_pCloud2->getPosition3D() + Vec3(0,0,-200));
            m_pCloud3->setOpacity(0);
        }
        else if(m_pMainPlayer->getPositionZ() > m_pCloud1->getPositionZ() && m_pMainPlayer->getPositionZ() < m_pCloud3->getPositionZ())
        {
            int opacity =  m_pCloud2->getOpacity()+51;
            if(opacity>=255)
                opacity = 255;
            m_pCloud2->setOpacity(opacity);
        }

        if(m_pMainPlayer->getPositionZ() == m_pCloud2->getPositionZ() || m_pMainPlayer->getPositionZ() == m_pCloud2->getPositionZ() + m_pTerrainLayer->getCellBaseRadius()*2)
        {
            m_pCloud1->setPosition3D(m_pCloud3->getPosition3D() + Vec3(0,0,-200));
            m_pCloud1->setOpacity(0);
        }
        else if(m_pMainPlayer->getPositionZ() > m_pCloud2->getPositionZ() && m_pMainPlayer->getPositionZ() < m_pCloud1->getPositionZ())
        {
            int opacity =  m_pCloud3->getOpacity()+51;
            if(opacity>=255)
                opacity = 255;
            m_pCloud3->setOpacity(opacity);
        }
        
        if(m_pMainPlayer->getPositionZ() == m_pCloud3->getPositionZ() || m_pMainPlayer->getPositionZ() == m_pCloud3->getPositionZ() + m_pTerrainLayer->getCellBaseRadius()*2)
        {
            m_pCloud2->setPosition3D(m_pCloud1->getPosition3D() + Vec3(0,0,-200));
            m_pCloud2->setOpacity(0);
        }
        else if(m_pMainPlayer->getPositionZ() > m_pCloud3->getPositionZ() && m_pMainPlayer->getPositionZ() < m_pCloud2->getPositionZ())
        {
            int opacity =  m_pCloud1->getOpacity()+51;
            if(opacity>=255)
                opacity = 255;
            m_pCloud1->setOpacity(opacity);
        }
    }
}