#include "Actor.h"
#include "StudentWorld.h"
#include "GameConstants.h"
#include <cmath>
using namespace std;

bool Actor::isAlive() const
{
    return m_lifeStatus;
}
void Actor::SetDead()
{
    m_lifeStatus = false;
    return;
}
void Actor::setCollisionAvoid(int num)
{
    if (num == 0)
        m_collisionAvoidance = true;
    else if (num == 1)
        m_collisionAvoidance = false;
    return;
}
int Actor::getGRSpeed()
{
    GhostRacer* m_ghostracer = getWorld()->getGR();
    int GRSpeed = m_ghostracer->getVertSpeed();
    return GRSpeed;
}
Actor::Actor(StudentWorld* stwd, int imageID, double startX, double startY, int dir, double size, int depth)
    :GraphObject(imageID, startX, startY, dir, size, depth)
{
    m_world = stwd;
    m_lifeStatus = true;
    m_hp = 10; 
    m_collisionAvoidance = false;
}
BorderLine::BorderLine(StudentWorld* stwd, int imageID, double startX, double startY, int dir, double size,
    int depth)
    :Actor(stwd, imageID, startX, startY, dir, size, depth)
{
    setVertSpeed(-4);
    setCollisionAvoid(1);
}
void BorderLine::doSomething()
{
    if(!isAlive())
        return;
    setVertSpeed(-4 - getGRSpeed());
    int new_y = getY() + getVertSpeed();
    int new_x = getX() + 0;
    moveTo(new_x, new_y);
    if( new_x < 0 || new_x > VIEW_WIDTH ||  new_y < 0 || new_y > VIEW_HEIGHT)
    {
        SetDead();
    }
    return;
}

GhostRacer::GhostRacer(StudentWorld* stwd, int imageID, double startX, double startY,
    int dir, double size, int depth)
    :Actor(stwd, imageID, startX, startY, dir, size, depth)
{
    setVertSpeed(0);
    m_shotcount = 10;
    setHp(100);
    setCollisionAvoid(0);
}
void GhostRacer::doSomething()
{
    if (getHp() <= 0)
        return;
    if (getX() <= ROAD_CENTER - ROAD_WIDTH/2)
    {
        if (getDirection() > 90)
        {
            setHp(getHp() - 10);
            if (getHp() <= 0)
            {
                getWorld()->resetBonus();
                getWorld()->decLives();
                SetDead();
                getWorld()->playSound(SOUND_PLAYER_DIE);
            }
            setDirection(82);
            getWorld()->playSound(SOUND_VEHICLE_CRASH);
        }
    }
    if (getX() >= ROAD_CENTER + ROAD_WIDTH/2)
    {
        if (getDirection() < 90)
        {
            setHp(getHp() - 10);
            if (getHp() <= 0)
            {
                getWorld()->resetBonus();
                getWorld()->decLives();
                SetDead();
                getWorld()->playSound(SOUND_PLAYER_DIE);
            }
            setDirection(98);
            getWorld()->playSound(SOUND_VEHICLE_CRASH);
        }
    }
    int ch;
    if(getWorld()->getKey(ch))
    {
        switch(ch)
        {
            case KEY_PRESS_SPACE:{
                if(m_shotcount > 0){
                    getWorld()->shootHoly(this);
                    m_shotcount--;
                }
                GRMove(getDirection());
                break;
            }
            case KEY_PRESS_LEFT:{
                if (getDirection() < 114)
                    setDirection(getDirection()+8);
                GRMove(getDirection());
                break;
            }
            case KEY_PRESS_RIGHT:{
                if (getDirection() > 66)
                    setDirection(getDirection()-8);
                GRMove(getDirection());
                break;
            }
            case KEY_PRESS_UP:{
                if(getVertSpeed() < 5)
                    setVertSpeed(getVertSpeed() + 1);
                GRMove(getDirection());
                break;
            }
            case KEY_PRESS_DOWN:{
                if(getVertSpeed() > -1)
                    setVertSpeed(getVertSpeed() - 1);
                GRMove(getDirection());
                break;
            }
        }
    }
    else
    {
        GRMove(getDirection());
    }
}
void GhostRacer::GRMove(int dir)
{
    long double curdir = dir;
    double pi = 3.14159265359;
    long double delta_x = cos((curdir*pi)/180) * 4;
    if(curdir == 90)
        return;
    else
        moveTo(getX() + delta_x, getY());
}
void GhostRacer::hitOilSlick()
{
    int change = randInt(5,20);
    int ccorc = randInt(0,1);
    if (ccorc == 0) //counterclockwise
    {
        setDirection(getDirection() + change);
        if (getDirection() > 120)
            setDirection(120);
        return;
    }
    if(ccorc == 1) //clockwise
    {
        setDirection(getDirection() - change);
        if(getDirection() < 60)
            setDirection(60);
        return;
    }
    return;
}
Goodie::Goodie(StudentWorld* stwd, int imageID, double startX, double startY, int dir, double size, int depth)
    : Actor(stwd, imageID, startX, startY, dir, size, depth)
{
    setVertSpeed(-4);
    setCollisionAvoid(1);
}
void Goodie::doSomething()
{
    if(!isAlive())
        return;
    setVertSpeed(-4 - getGRSpeed());
    int new_y = getY() + getVertSpeed();
    int new_x = getX() + 0;
    moveTo(new_x, new_y);
    if( new_x < 0 || new_x > VIEW_WIDTH ||  new_y < 0 || new_y > VIEW_HEIGHT)
    {
        SetDead();
        return;
    }
    if(getWorld()->doTheyOverlap(this)){
        effect();
        return;
    }
    rotate();
}
void Goodie::ifSprayed()
{
    if(isSprayable())
    {
        SetDead();
        return;
    }
    else
        return;
}
Pedestrian::Pedestrian(StudentWorld* stwd, int imageID, double startX, double startY, double size, int dir, int depth)
    : Actor(stwd, imageID, startX, startY, dir, size, depth)
{
    setHorSpeed(0);
    setMoveplan(0);
    setCollisionAvoid(0);
}
void Pedestrian::moveAndPickPlan()
{
    setVertSpeed(-4 - getGRSpeed());
    int new_y = getY() + getVertSpeed();
    int new_x = getX() + getHorSpeed();
    moveTo(new_x, new_y);
    if( new_x < 0 || new_x > VIEW_WIDTH ||  new_y < 0 || new_y > VIEW_HEIGHT)
    {
        SetDead();
    }
    if (getMoveplan() <= 0)
    {
        int x = randInt(-3,3);
        while (x == 0)
        {
            x = randInt(-3,3);
        }
        setHorSpeed(x);
        setMoveplan(randInt(4,32));
        if(getHorSpeed() > 0)
            setDirection(0);
        else if (getHorSpeed() < 0)
            setDirection(180);
    }
}
LostSoul::LostSoul(StudentWorld* stwd, double startX, double startY, int imageID, int dir, double size,
    int depth)
    : Goodie(stwd, imageID, startX, startY, dir, size, depth)
{
    setVertSpeed(-4);
    setCollisionAvoid(1);
}
void LostSoul::effect()
{
    getWorld()->decSouls();
    SetDead();
    getWorld()->playSound(SOUND_GOT_SOUL);
    getWorld()->increaseScore(100);
    return;
}
void LostSoul::rotate()
{
    setDirection(getDirection() - 10);
    return;
}
HolyProjectile::HolyProjectile(StudentWorld* stwd, double startX, double startY, int dir, int imageID,
    double size, int depth)
    : Actor(stwd, imageID, startX, startY, dir, size, depth)
{
}
void HolyProjectile::doSomething()
{
    if(!isAlive())
        return;
    if (getWorld()->ifSprayHits(this))
    {
        SetDead();
        return;
    }
    moveForward(SPRITE_HEIGHT);
    int new_y = getY();
    int new_x = getX();
    m_maxTravelDis -= SPRITE_HEIGHT;
    if( new_x < 0 || new_x > VIEW_WIDTH ||  new_y < 0 || new_y > VIEW_HEIGHT)
    {
        SetDead();
    }
    if(m_maxTravelDis <= 0){
        SetDead();
    }
}
HolyGoodie::HolyGoodie(StudentWorld* stwd, double startX, double startY, int imageID, int dir,
    double size, int depth)
    : Goodie(stwd, imageID, startX, startY, dir, size, depth)
{
    setSprayable();
}
void HolyGoodie::effect()
{
    GhostRacer* m_ghostracer = getWorld()->getGR();
    m_ghostracer->setShotCount(10);
    SetDead();
    getWorld()->playSound(SOUND_GOT_GOODIE);
    getWorld()->increaseScore(50);
}
HealGoodie::HealGoodie(StudentWorld* stwd, double startX, double startY, int imageID, int dir,
    double size, int depth)
    : Goodie(stwd, imageID, startX, startY, dir, size, depth)
{
    setSprayable();
}
void HealGoodie::effect()
{
    GhostRacer* m_ghostracer = getWorld()->getGR();
    getWorld()->changeGRhealth(10);
    if(m_ghostracer->getHp() > 100)
        m_ghostracer->setHp(100);
    SetDead();
    getWorld()->playSound(SOUND_GOT_GOODIE);
    getWorld()->increaseScore(250);
}
OilSlick::OilSlick(StudentWorld* stwd, double startX, double startY, int imageID, int dir,
    double size, int depth)
    : Goodie(stwd, imageID, startX, startY, dir, size, depth)
{
}
void OilSlick::effect()
{
    getWorld()->playSound(SOUND_OIL_SLICK);
    GhostRacer* m_ghostracer = getWorld()->getGR();
    m_ghostracer->hitOilSlick();
}
HumanPed::HumanPed(StudentWorld* stwd, double startX, double startY, int imageID, int dir,
    double size, int depth)
    : Pedestrian(stwd, imageID, startX, startY, size, dir, depth)
{
    setSprayable();
    setVertSpeed(-4);
    setHp(2);
}
void HumanPed::doSomething()
{
    if (!isAlive())
        return;
    if(getWorld()->doTheyOverlap(this))
    {
        getWorld()->deathCollision();
        return;
    }
    moveAndPickPlan();
    setMoveplan(getMoveplan() - 1);
    if (getMoveplan() > 0)
        return;
}
void HumanPed::ifSprayed()
{
    setHorSpeed(getHorSpeed() * -1);
    if (getDirection() == 0)
        setDirection(180);
    if (getDirection() == 180)
        setDirection(0);
    getWorld()->playSound(SOUND_PED_HURT);
}
ZombiePed::ZombiePed(StudentWorld* stwd, double startX, double startY, int imageID, int dir,
    double size, int depth)
    : Pedestrian(stwd, imageID, startX, startY, size, dir, depth)
{
    setHp(2);
    setSprayable();
    setVertSpeed(-4);
    m_ticksToGrunt = 0;
}
void ZombiePed::doSomething()
{
    if (!isAlive())
        return;
    GhostRacer* m_ghostracer = getWorld()->getGR();
    if(getWorld()->doTheyOverlap(this))
    {
        getWorld()->changeGRhealth(-5);
        getWorld()->playSound(SOUND_PED_DIE);
        SetDead();
        return;
    }
    double distance = getX()-m_ghostracer->getX();
    if(abs(distance) < 30)
    {
        setDirection(270);
        if(distance < 0)
            setHorSpeed(1);
        else if(distance > 0)
            setHorSpeed(-1);
        else if (distance == 0)
            setHorSpeed(0);
        m_ticksToGrunt--;
        if(m_ticksToGrunt <= 0)
        {
            getWorld()->playSound(SOUND_ZOMBIE_ATTACK);
            m_ticksToGrunt = 20;
        }
    }
    moveAndPickPlan();
    if(getMoveplan() > 0)
    {
        setMoveplan(getMoveplan() - 1);
    }
}
void ZombiePed::ifSprayed()
{
    setHp(getHp() - 1);
    if (getHp() <= 0)
    {
        SetDead();
        getWorld()->playSound(SOUND_PED_DIE);
        if(!(getWorld()->doTheyOverlap(this)))
        {
            int x = randInt(0,5);
            if (x == 0)
            {
                getWorld()->addHealGoodie(this);
            }
        }
        getWorld()->increaseScore(150);
        return;
    }
    else{
        getWorld()->playSound(SOUND_PED_HURT);
        return;
    }
}
ZombieCab::ZombieCab(StudentWorld* stwd, double startX, double startY, int imageID, int dir, double size, int depth)
    : Actor(stwd, imageID, startX, startY, dir, size, depth)
{
    setHp(3);
    m_damagedGR = false;
    m_changed = false;
    setCollisionAvoid(0);
    setHorSpeed(0); 
    setSprayable();
}
void ZombieCab::doSomething()
{
    if(!isAlive())
        return;
    if(getWorld()->doTheyOverlap(this))
    {
        if(m_damagedGR == false)
        {
            getWorld()->playSound(SOUND_VEHICLE_CRASH);
            getWorld()->changeGRhealth(-20);
            if(getX() <= getWorld()->getGR()->getX())
            {
                setHorSpeed(-5);
                setDirection(120 + randInt(0,19));
            }
            else if(getX() > getWorld()->getGR()->getX())
            {
                setHorSpeed(5);
                setDirection(60 - randInt(0,19));
            }
            m_damagedGR = true;
        }
    }
    setVertSpeed(m_defaultVS - getGRSpeed());
    setHorSpeed(getHorSpeed());
    int new_y = getY() + getVertSpeed();
    int new_x = getX() + getHorSpeed();
    moveTo(new_x, new_y);
    if( new_x < 0 || new_x > VIEW_WIDTH ||  new_y < 0 || new_y > VIEW_HEIGHT)
    {
        SetDead();
        return;
    }
    if ((getWorld()->actorInFrontBehind(this) == 1) && (getVertSpeed() > getGRSpeed())) //checks in front and gr speed
    {
        if (m_changed == false)
        {
            setDefault(m_defaultVS * .5);
            m_changed = true;
        }
    }
    else if ((getWorld()->actorInFrontBehind(this) == 0) && (getVertSpeed() <= getGRSpeed()))
    {
        if (m_changed == false)
        {
            setDefault(m_defaultVS * 2);
            m_changed = true;
        }
    }
    m_moveplan--;
    if(m_moveplan > 0)
        return;
    else
    {
        setMovePlan(randInt(4,32));
        setDefault(m_defaultVS + randInt(-2,2));
    }
}
void ZombieCab::ifSprayed()
{
    setHp(getHp() - 1);
    if(getHp() <= 0)
    {
        SetDead();
        getWorld()->playSound(SOUND_VEHICLE_DIE);
        int x = randInt(0,5);
        if(x == 0)
        {
            getWorld()->addOilSlick(this);
        }
        getWorld()->increaseScore(200);
        return;
    }
    getWorld()->playSound(SOUND_VEHICLE_HURT);
}
// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp
