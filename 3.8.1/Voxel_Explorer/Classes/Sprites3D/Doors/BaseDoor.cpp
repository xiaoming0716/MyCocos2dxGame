//
//  BaseDoor.cpp
//  Voxel_Explorer
//
//  Created by wang haibo on 15/10/14.
//
//

#include "BaseDoor.hpp"
#include "BaseLevel.h"
#include "VoxelExplorer.h"
#include "SimpleAudioEngine.h"
#include "LevelResourceManager.h"
#include "RandomDungeon.hpp"
USING_NS_CC;
using namespace CocosDenshion;
const char* DOOR_MODEL_NAMES[] = {
    "DMN_STANDARD",
    "DMN_BOSS",
    "DMN_MAX"
};
BaseDoor::BaseDoor()
{
    m_DoorState = DS_UNKNOWN;
    m_DoorLastState = DS_UNKNOWN;
    m_pFakeDoor = 0;
    m_OpenType = DOT_STANDARD;
    
    m_bMagicLocked = false;
}
BaseDoor::~BaseDoor()
{
}

BaseDoor::DoorState BaseDoor::getDoorState() const
{
    return m_DoorState;
}
void BaseDoor::setDoorState(BaseDoor::DoorState state)
{
    if (m_DoorState == state)
        return;
    
    ///处理上一个状态退出逻辑
    switch (m_DoorState) {
        case BaseDoor::DS_CLOSED:
            onExitClosed();
            break;
        case BaseDoor::DS_OPENED:
            onExitOpened();
            break;
        case BaseDoor::DS_LOCKED:
            onExitLocked();
            break;
        case BaseDoor::DS_HIDE:
            onExitHide();
            break;
        default:
            break;
    }
    m_DoorLastState = m_DoorState;
    m_DoorState = state;
    ///处理下一个状态进入逻辑
    switch (m_DoorState) {
        case BaseDoor::DS_CLOSED:
            if(m_DoorLastState != BaseDoor::DS_HIDE)
                onEnterClosed();
            break;
        case BaseDoor::DS_OPENED:
            onEnterOpened();
            break;
        case BaseDoor::DS_LOCKED:
            onEnterLocked();
            break;
        case BaseDoor::DS_HIDE:
            onEnterHide();
            break;
        default:
            break;
    }
}
void BaseDoor::setActorDir( ActorDir dir )
{
    if(m_dir == dir)
        return;
    m_dir = dir;
    if(!m_pFakeDoor)
        return;
    switch (m_dir) {
        case AD_FORWARD:
            m_pFakeDoor->setRotation3D(Vec3(0,180,0));
            break;
        case AD_LEFT:
            m_pFakeDoor->setRotation3D(Vec3(0,-90,0));
            break;
        case AD_RIGHT:
            m_pFakeDoor->setRotation3D(Vec3(0,90,0));
            break;
        case AD_BACK:
            m_pFakeDoor->setRotation3D(Vec3(0,0,0));
            break;
        default:
            break;
    }
}
void BaseDoor::onEnterClosed()
{
    if(!m_pFakeDoor)
        return;
    if (m_dir == AD_FORWARD || m_dir == AD_BACK) {
        if(m_DoorLastState == BaseDoor::DS_UNKNOWN || m_DoorLastState == BaseDoor::DS_LOCKED)
        {
            m_pFakeDoor->setVisible(true);
            addTerrainTileFlag(TileInfo::USEABLE);
        }
        else
        {
            EaseSineOut* moveTo = EaseSineOut::create(MoveTo::create(0.5f, Vec3(m_pFakeDoor->getPositionX()+TerrainTile::CONTENT_SCALE, m_pFakeDoor->getPositionY(), m_pFakeDoor->getPositionZ())));
            m_pFakeDoor->setVisible(true);
            addTerrainTileFlag(TileInfo::USEABLE);
            m_pFakeDoor->runAction(moveTo);
        }
    }
    else if(m_dir == AD_LEFT || m_dir == AD_RIGHT)
    {
        if(m_DoorLastState == BaseDoor::DS_UNKNOWN || m_DoorLastState == BaseDoor::DS_LOCKED)
        {
            m_pFakeDoor->setVisible(true);
            addTerrainTileFlag(TileInfo::USEABLE);
        }
        else
        {
            EaseSineOut* moveTo = EaseSineOut::create(MoveTo::create(0.5f, Vec3(m_pFakeDoor->getPositionX(),m_pFakeDoor->getPositionY(), m_pFakeDoor->getPositionZ()-TerrainTile::CONTENT_SCALE)));
            m_pFakeDoor->setVisible(true);
            addTerrainTileFlag(TileInfo::USEABLE);
            m_pFakeDoor->runAction(moveTo);
        }
    }
    if(RandomDungeon::getInstance()->getCurrentDungeonNode()->isBossDepth() && m_DoorLastState == DoorState::DS_OPENED)
    {
        std::string soundName = LevelResourceManager::getInstance()->getDungeonSoundEffectRes("CLOSE_DOOR");
        SimpleAudioEngine::getInstance()->playEffect(soundName.c_str());
    }
}
void BaseDoor::onExitClosed()
{
}

void BaseDoor::onEnterOpened()
{
    if(!m_pFakeDoor)
        return;
    if (m_dir == AD_FORWARD || m_dir == AD_BACK) {
        if(m_DoorLastState == BaseDoor::DS_UNKNOWN)
        {
            m_pFakeDoor->setVisible(false);
            removeTerrainTileFlag(TileInfo::USEABLE);
        }
        else
        {
            EaseSineOut* moveTo = EaseSineOut::create(MoveTo::create(0.5f, Vec3(m_pFakeDoor->getPositionX()-TerrainTile::CONTENT_SCALE, m_pFakeDoor->getPositionY(), m_pFakeDoor->getPositionZ())));
            CallFunc* callback1 = CallFunc::create(CC_CALLBACK_0(BaseDoor::setVisible,m_pFakeDoor, false));
            CallFunc* callback2 = CallFunc::create(CC_CALLBACK_0(BaseDoor::removeTerrainTileFlag,this, TileInfo::USEABLE));
            Sequence* sequence = Sequence::create(moveTo, callback1, callback2, NULL);
            m_pFakeDoor->runAction(sequence);
        }
    }
    else if(m_dir == AD_LEFT || m_dir == AD_RIGHT)
    {
        if(m_DoorLastState == BaseDoor::DS_UNKNOWN)
        {
            m_pFakeDoor->setVisible(false);
            removeTerrainTileFlag(TileInfo::USEABLE);
        }
        else
        {
            EaseSineOut* moveTo = EaseSineOut::create(MoveTo::create(0.5f, Vec3(m_pFakeDoor->getPositionX(),m_pFakeDoor->getPositionY(), m_pFakeDoor->getPositionZ()+TerrainTile::CONTENT_SCALE)));
            CallFunc* callback1 = CallFunc::create(CC_CALLBACK_0(BaseDoor::setVisible,m_pFakeDoor, false));
            CallFunc* callback2 = CallFunc::create(CC_CALLBACK_0(BaseDoor::removeTerrainTileFlag,this, TileInfo::USEABLE));
            Sequence* sequence = Sequence::create(moveTo, callback1, callback2, NULL);
            m_pFakeDoor->runAction(sequence);
        }
    }
    std::string soundName = LevelResourceManager::getInstance()->getDungeonSoundEffectRes("OPEN_DOOR");
    SimpleAudioEngine::getInstance()->playEffect(soundName.c_str());
}
void BaseDoor::onExitOpened()
{
}

void BaseDoor::onEnterLocked()
{
    if(m_pFakeDoor)
    {
        if(m_DoorLastState == BaseDoor::DS_UNKNOWN)
            m_pFakeDoor->setColor(Color3B::ORANGE);
        else
        {
            EaseSineOut* fadeIn = EaseSineOut::create(TintTo::create(0.5f, Color3B::ORANGE));
            m_pFakeDoor->runAction(fadeIn);
        }
    }
}
void BaseDoor::onExitLocked()
{
    if(m_pFakeDoor)
    {
        EaseSineOut* fadeOut = EaseSineOut::create(TintTo::create(0.5f, Color3B::WHITE));
        m_pFakeDoor->runAction(fadeOut);
    }
    std::string soundName = LevelResourceManager::getInstance()->getDungeonSoundEffectRes("DOOR_UNLOCKED");
    SimpleAudioEngine::getInstance()->playEffect(soundName.c_str());
}
void BaseDoor::onEnterHide()
{
    EaseSineOut* fadeIn = EaseSineOut::create(FadeIn::create(0.5f));
    this->runAction(fadeIn);
}
void BaseDoor::onExitHide()
{
    EaseSineOut* fadeOut = EaseSineOut::create(FadeOut::create(0.5f));
    this->runAction(fadeOut);
}
void BaseDoor::setVisited(bool visited)
{
    unsigned int lightmask = getLightMask();
    if (visited)
        lightmask = lightmask | (unsigned int)LightFlag::LIGHT1;
    else
        lightmask = lightmask &~ (unsigned int)LightFlag::LIGHT1;
    setLightMask(lightmask);
    if(m_pFakeDoor)
    {
        if(m_DoorState != DS_OPENED)
            m_pFakeDoor->setVisible(visited);
        m_pFakeDoor->setLightMask(lightmask);
    }
}
std::string BaseDoor::getDesc()
{
    return VoxelExplorer::getInstance()->getCurrentLevel()->getTerrainTileInfoDesc(getPosInMap().x, getPosInMap().y);
}