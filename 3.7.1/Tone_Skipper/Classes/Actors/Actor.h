//
//  Actor.h
//  Tone_Skipper
//
//  Created by wang haibo on 15/8/26.
//
//

#ifndef __Tone_Skipper__Actor__
#define __Tone_Skipper__Actor__

#include "cocos2d.h"
#include "HueSprite.h"
class Actor : public cocos2d::Node
{
protected:
    Actor();
    virtual ~Actor();
public:
    typedef enum{
        RT_GROUND,
        RT_RAYCASTER,
        RT_UNKNOWN
    }
    RAYCAST_TYPE;
    typedef enum{
        TT_TIPS,
        TT_ROOM,
        TT_DOOR,
        TT_SECREAT,
        TT_UNKNOWN
    }
    TRIGGER_TYPE;
    typedef enum{
        UIT_LADDER,
        UIT_QUESTIONBOX,
        UIT_UNKNOWN
    }
    USABLE_ITEM_TYPE;
    static constexpr float Gravity = -10.0f;
    virtual void update(float delta){};
    
    virtual float getHue();
    virtual void setHue(float hue);
    
    void setFlipX(bool fliped);
    void setMaxXSpeed(float speed) { m_fMaxXSpeed = speed;}
    void setMaxYSpeed(float speed) { m_fMaxYSpeed = speed;}
    
    cocos2d::Vec2 getVelocity() const { return m_Velocity; }
    
    virtual bool loadModel() = 0;
    virtual bool loadAnimations() = 0;
    virtual void onLand() = 0;
    
    void showDebug(bool debug);
protected:
    HueSprite*  m_pSprite;
    
    float                       m_fMaxXSpeed;
    float                       m_fMaxYSpeed;
    cocos2d::Vec2               m_Velocity;
    bool                        m_bOnLand;
    bool                        m_bOnLadder;
    
    cocos2d::DrawNode*          m_pDebugDrawNode;
};

#endif /* defined(__Tone_Skipper__Actor__) */
