//
//  RunController.h
//  MazeJump
//
//  Created by wang haibo on 15/7/22.
//
//

#ifndef __MazeJump__RunController__
#define __MazeJump__RunController__

#include "cocos2d.h"
#include "TerrainLayer.h"
#include "Runner.h"
class RunController : public cocos2d::Ref
{
    RunController();
    ~RunController();
public:
    static RunController* getInstance();
    bool init(cocos2d::Layer* pMainLayer);
    void update(float delta);
    void destroy();
    
    TerrainLayer* getTerrainLayer() const { return m_pTerrainLayer; }
    cocos2d::Camera* getMainCamera() const { return m_pMainCamera; }
    Runner* getMainPlayer() const { return m_pMainPlayer; }
    
    int getInitDifficultLevel() const { return m_nInitDifficultLevel; }
    int getDifficultLevel();
    void setDifficultLevel(int difficult);

    int getMaxDifficultLevel();
    void setMaxDifficultLevel(int difficult);
    
    void cameraTrackPlayer();
    
    void gameOver();
private:
    bool initCloud();
    void updateCloud();
private:
    cocos2d::Layer*     m_pMainLayer;
    TerrainLayer*       m_pTerrainLayer;
    cocos2d::Camera*    m_pMainCamera;
    Runner*             m_pMainPlayer;
    cocos2d::Sprite3D*           m_pCloud1;
    cocos2d::Sprite3D*           m_pCloud2;
    cocos2d::Sprite3D*           m_pCloud3;
    
    int             m_nInitDifficultLevel;
    int             m_nDifficultLevel;
    int             m_nMaxReachDifficultLevel;
};

#endif /* defined(__MazeJump__RunController__) */