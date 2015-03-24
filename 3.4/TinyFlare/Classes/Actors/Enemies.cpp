//
//  Enemies.cpp
//  Geometry-Wars
//
//  Created by wang haibo on 15/3/9.
//
//

#include "Enemies.h"
#include "GameController.h"
#include "ParticleSystemHelper.h"
USING_NS_CC;
Enemy::Enemy()
{
    m_type = AT_ENEMY;
    m_EnemyType = ET_UNKNOWN;
}
Enemy::~Enemy()
{
}
void Enemy::update(float delta)
{
    if(m_curState == ActorState::AS_TRACK)
    {
        if(GameController::getInstance()->getPlayer())
        {
            Vec2 playerPos = GameController::getInstance()->getPlayerPos();
            Vec2 dir = playerPos - getPosition();
            setDirection(dir);
        }
        else
            setDirection(Vec2::ZERO);
    }
    
    float mainRotationZ = CC_RADIANS_TO_DEGREES(Vec2::angle(m_Direction, Vec2::UNIT_Y));
    if (m_Direction.x < 0)
        mainRotationZ = -mainRotationZ;
    setRotation(mainRotationZ);
    
    if(!m_bBounce)
    {
        if (m_Direction == Vec2::ZERO) {
            m_Velocity *= 0.99f;
            if(m_Velocity.length() <0.001f)
                m_Velocity = Vec2::ZERO;
        }
        else
        {
            m_Velocity += m_Direction*m_fAccel*delta;
            if(m_Velocity.length() >= m_fMaxSpeed)
            {
                m_Velocity.normalize();
                m_Velocity = m_Velocity*m_fMaxSpeed;
            }
        }
    }
    Vec2 pos = getPosition();
    Vec2 newPos = pos + m_Velocity;
    setPosition(newPos);
}
cocos2d::Color3B Enemy::getColor()
{
    if (m_pModel)
        return m_pModel->getColor();
    return Color3B::WHITE;
}
void Enemy::setColor(const cocos2d::Color3B& color)
{
    if(m_pModel)
        m_pModel->setColor(color);
}
void Enemy::beginTrack()
{
    m_fAccel = cocos2d::random(4.0f, 5.0f);
    m_fMaxSpeed = cocos2d::random(2.0f, 3.0f);
    setActorState(ActorState::AS_TRACK);
}
Circle::Circle()
{
    m_EnemyType = ET_CIRCLE;
    m_fAccel = 40.0f;
    m_fMaxSpeed = 10.0f;
}
Circle::~Circle()
{
}
void Circle::onEnterDead()
{
    ParticleSystemHelper::spawnExplosion(ExplosionType::ET_EXPLOSION_WHITE, getPosition());
}

/////ColorCircle
ColorCircle::ColorCircle()
{
    m_EnemyType = ET_CIRCLE_COLORED;
    m_fAccel = cocos2d::random(4.0f, 5.0f);
    m_fMaxSpeed = cocos2d::random(2.0f, 3.0f);
}
ColorCircle::~ColorCircle()
{
}
void ColorCircle::onEnterDead()
{
    ParticleSystemHelper::spawnExplosion(ExplosionType::ET_EXPLOSION_WHITE, getPosition());
    GameController::getInstance()->getEnemiesGenerator()->generateEnemiesByExplosion(Enemy::ET_CIRCLE, getPosition(), 2);
}

/////Triangle
Triangle::Triangle()
{
    m_EnemyType = ET_TRIANGLE;
    m_fAccel = cocos2d::random(4.0f, 5.0f);
    m_fMaxSpeed = cocos2d::random(2.0f, 3.0f);
}
Triangle::~Triangle()
{
}
void Triangle::onEnterDead()
{
    ParticleSystemHelper::spawnExplosion(ExplosionType::ET_EXPLOSION_YELLOW, getPosition());
}
void Triangle::beginTrack()
{
    m_fAccel = cocos2d::random(8.0f, 10.0f);
    m_fMaxSpeed = cocos2d::random(4.0f, 6.0f);
    ScaleTo* scaleTo1 = ScaleBy::create(0.2f, 0.8f);
    ScaleTo* scaleTo2 = ScaleBy::create(0.2f, 1.25f);
    Sequence* sequence = Sequence::createWithTwoActions(scaleTo1, scaleTo2);
    RepeatForever* repeatForever = RepeatForever::create(sequence);
    runAction(repeatForever);
    setActorState(ActorState::AS_TRACK);
}

ColorTriangle::ColorTriangle()
{
    m_EnemyType = ET_TRIANGLE_COLORED;
    m_fTrackTime = 0.0f;
    m_fChargeTime = 0.0f;
    m_fIdleTime = 0.0f;
}
ColorTriangle::~ColorTriangle()
{
}
void ColorTriangle::update(float delta)
{
    if(m_curState == AS_TRACK)
    {
        m_fTrackTime += delta;
        float dist = GameController::getInstance()->getPlayerPos().distance(getPosition());
        if(m_fTrackTime >= 5.0f && dist >= GameController::getInstance()->getBoundSize().width*0.15f)
        {
            m_fTrackTime = 0.0f;
            setActorState(ActorState::AS_IDLE);
        }
        if(GameController::getInstance()->getPlayer())
        {
            Vec2 playerPos = GameController::getInstance()->getPlayerPos();
            Vec2 dir = playerPos - getPosition();
            setDirection(dir);
        }
        else
            setDirection(Vec2::ZERO);
    }
    else if(m_curState == ActorState::AS_IDLE)
    {
        m_fIdleTime += delta;
        float dist = GameController::getInstance()->getPlayerPos().distance(getPosition());
        if(m_fIdleTime >= 2.0f)
        {
            m_fIdleTime = 0.0f;
            if(dist >= GameController::getInstance()->getBoundSize().width*0.1f)
                setActorState(ActorState::AS_CHARGE);
            else
                setActorState(ActorState::AS_TRACK);
        }
    }
    else if(m_curState == ActorState::AS_CHARGE)
    {
        m_fChargeTime += delta;
        if(m_fChargeTime >= 1.0f)
        {
            m_fChargeTime = 0.0f;
            setActorState(ActorState::AS_TRACK);
        }
    }
    
    //if(m_curState != ActorState::AS_CHARGE)
    {
        float mainRotationZ = CC_RADIANS_TO_DEGREES(Vec2::angle(m_Direction, Vec2::UNIT_Y));
        if (m_Direction.x < 0)
            mainRotationZ = -mainRotationZ;
        setRotation(mainRotationZ);
    }
    
    if(!m_bBounce)
    {
        if (m_Direction == Vec2::ZERO) {
            m_Velocity *= 0.99f;
            if(m_Velocity.length() <0.001f)
                m_Velocity = Vec2::ZERO;
        }
        else
        {
            m_Velocity += m_Direction*m_fAccel*delta;
            if(m_Velocity.length() >= m_fMaxSpeed)
            {
                m_Velocity.normalize();
                m_Velocity = m_Velocity*m_fMaxSpeed;
            }
        }
    }
    if(m_curState != ActorState::AS_IDLE)
    {
        Vec2 pos = getPosition();
        Vec2 newPos = pos + m_Velocity;
        setPosition(newPos);
    }
}
void ColorTriangle::onEnterCharge()
{
    Vec2 dir = GameController::getInstance()->getPlayerPos() - getPosition();
    dir.normalize();
    setDirection(dir);
    m_fAccel = 100.0f;
    m_fMaxSpeed = 20.0f;
}
void ColorTriangle::onExitCharge()
{
    setDirection(Vec2::ZERO);
    m_fAccel = cocos2d::random(4.0f, 5.0f);
    m_fMaxSpeed = cocos2d::random(2.0f, 3.0f);
}