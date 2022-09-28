#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
using namespace std;

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class StudentWorld;
class Actor : public GraphObject
{
    public:
    Actor(StudentWorld* stwd, int imageID, double startX, double startY, int dir = 0, double size = 1.0, int depth = 0);
    virtual ~Actor() {};
    virtual void doSomething() = 0;
    virtual void ifSprayed(){
        return;
    }
    StudentWorld* getWorld() {
        return m_world;
    }
    bool isAlive() const;
    void SetDead();
    void setCollisionAvoid(int num);
    bool isCollisionAvoid(){
        return m_collisionAvoidance;
    }
    int getGRSpeed();
    int getVertSpeed() const{
        return m_vertspeed;
    }
    int setVertSpeed(int newspeed) {
        m_vertspeed = newspeed;
        return m_vertspeed;
    }
    int getHp() const{
        return m_hp;
    }
    int setHp(int newhp){
        m_hp = newhp;
        return m_hp;
    }
    void setSprayable(){
        m_isSprayable = true;
    }
    bool isSprayable(){
        return m_isSprayable;
    }
    int getHorSpeed() const {
        return m_horspeed;
    }
    void setHorSpeed(int newspeed) {
        m_horspeed = newspeed;
        return;
    }
    private:
    StudentWorld* m_world;
    bool m_lifeStatus;
    bool m_isSprayable = false;
    int m_hp;
    int m_vertspeed;
    bool m_collisionAvoidance;
    int m_horspeed;
};
class Goodie : public Actor
{
    public:
    Goodie(StudentWorld* stwd, int imageID, double startX, double startY, int dir = 0, double size = 1.0 , int depth = 1);
    virtual ~Goodie() {};
    virtual void doSomething();
    virtual void effect() = 0;
    virtual void ifSprayed();
    virtual void rotate() = 0;
    private:
};
class Pedestrian : public Actor
{
    public:
    Pedestrian(StudentWorld* stwd, int imageID, double startX, double startY, double size, int dir = 0, int depth = 0);
    virtual ~Pedestrian() {};
    void setMoveplan(int newplan){
        m_moveplan = newplan;
        return;
    }
    int getMoveplan(){
        return m_moveplan;
    }
    void moveAndPickPlan();
    private:
    int m_moveplan;
};
class BorderLine : public Actor
{
    public:
    BorderLine(StudentWorld* stwd, int imageID, double startX, double startY, int dir = 0, double size = 2.0,
    int depth = 2);
    virtual ~BorderLine() {};
    virtual void doSomething();
    private:
};
class GhostRacer : public Actor
{
    public:
    GhostRacer(StudentWorld* stwd, int imageID = IID_GHOST_RACER, double startX = 128.0, double startY = 32.0,
    int dir = 90, double size = 4.0, int depth = 0);
    virtual ~GhostRacer(){};
    virtual void doSomething();
    int getShotCount(){
        return m_shotcount;
    }
    void setShotCount(int add){
        m_shotcount += add;
    }
    void GRMove(int dir);
    void hitOilSlick();
    private:
    int m_shotcount;
};
class LostSoul : public Goodie
{
    public:
    LostSoul(StudentWorld* stwd, double startX, double startY, int imageID = IID_SOUL_GOODIE, int dir = 0,
    double size = 4.0, int depth = 2);
    virtual ~LostSoul(){};
    virtual void effect();
    virtual void rotate();
    private:
};
class HolyProjectile : public Actor
{
    public:
    HolyProjectile(StudentWorld* stwd, double startX, double startY, int dir, int imageID = IID_HOLY_WATER_PROJECTILE,
    double size = 1.0, int depth = 1);
    virtual ~HolyProjectile(){};
    virtual void doSomething();
    private:
    int m_maxTravelDis = 160;
};
class HolyGoodie : public Goodie
{
    public:
    HolyGoodie(StudentWorld* stwd, double startX, double startY, int imageID = IID_HOLY_WATER_GOODIE, int dir = 90,
    double size = 2.0, int depth = 2);
    virtual ~HolyGoodie(){};
    virtual void effect();
    virtual void rotate(){return;}
    private:
};
class HealGoodie : public Goodie
{
    public:
    HealGoodie(StudentWorld* stwd, double startX, double startY, int imageID = IID_HEAL_GOODIE, int dir = 0, 
    double size = 1.0, int depth = 2);
    virtual ~HealGoodie() {};
    virtual void effect();
    virtual void rotate() {return;}
    private:
};
class OilSlick : public Goodie
{
    public:
    OilSlick(StudentWorld* stwd, double startX, double startY, int imageID = IID_OIL_SLICK, int dir = 0,
    double size = randInt(2,5), int depth = 2);
    virtual ~OilSlick() {};
    virtual void effect();
    virtual void rotate(){return;}
    private:
};
class HumanPed : public Pedestrian
{
    public:
    HumanPed(StudentWorld* stwd, double startX, double startY, int imageID = IID_HUMAN_PED, int dir = 0,
    double size = 2.0, int depth = 0);
    virtual ~HumanPed() {};
    virtual void doSomething();
    virtual void ifSprayed();
    private:
};
class ZombiePed : public Pedestrian
{
    public:
    ZombiePed(StudentWorld* stwd, double startX, double startY, int imageID = IID_ZOMBIE_PED, int dir = 0,
    double size = 3.0, int depth = 0);
    virtual ~ZombiePed() {};
    virtual void doSomething();
    virtual void ifSprayed();
    private:
    int m_ticksToGrunt;
};
class ZombieCab : public Actor
{
    public:
    ZombieCab(StudentWorld* stwd, double startX, double startY, int imageID = IID_ZOMBIE_CAB, int dir = 90,
    double size = 4.0, int depth = 0);
    virtual ~ZombieCab() {};
    virtual void doSomething();
    virtual void ifSprayed();
    int getMovePlan(){
        return m_moveplan;
    }
    void setMovePlan(int newplan){
        m_moveplan = newplan;
        return;
    }
    void setDefault(int def){
        m_defaultVS = def;
    }
    private:
    bool m_damagedGR;
    bool m_changed;
    int m_moveplan;
    int m_defaultVS;
};
#endif // ACTOR_H_
