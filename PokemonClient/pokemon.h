#include <QString>
#include <QRandomGenerator>
#include <vector>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#define MAX_LEVEL 15
#define MAX_RAND 100
class Pokemon
{
public:
    Pokemon(const QString &name, int level, int exp, int attack, int defense, int hp, float speed)
        : name(name), level(level), exp(exp), attackPower(attack), defensePower(defense), healthPoints(hp), speedValue(speed){}
    int attack(Pokemon* targe,int randnumber);
    virtual void levelUp() = 0;
    virtual void attackway()= 0;
    QJsonObject toJsonObject() const;
public:
    QString name;
    int level=1;
    int exp=0;
    int attackPower=10;
    int defensePower=10;
    int healthPoints=50;
    float speedValue=0.1;
    QString attackWay;
};

class Attack : public Pokemon
{
public:
    Attack(const QString &name, int level, int exp, int attack, int defense, int hp, float speed)
        : Pokemon(name, level, exp, attack, defense, hp, speed) {}
    void levelUp () override;
};

class Hp : public Pokemon
{
public:
    Hp(const QString &name, int level, int exp, int attack, int defense, int hp, float speed)
        : Pokemon(name, level, exp, attack, defense, hp, speed) {}
    void levelUp () override;
};

class Defend : public Pokemon
{
public:
    Defend(const QString &name, int level, int exp, int attack, int defense, int hp, float speed)
        : Pokemon(name, level, exp, attack, defense, hp, speed) {}
    void levelUp () override;

};

class Speed : public Pokemon
{
public:
    Speed(const QString &name, int level, int exp, int attack, int defense, int hp, float speed)
        : Pokemon(name, level, exp, attack, defense, hp, speed) {}
    void levelUp () override;
};

// 示例子类实现
//Attack
class XiaoHuoLong : public Attack
{
public:
    XiaoHuoLong() : Attack("小火龙", 1, 0, 15, 8, 50, 0.1)
    {}
    void attackway() override;

private:  // 假设小火龙的攻击翻倍
};

class JieNiGui: public Attack
{
public:
    JieNiGui() : Attack("杰尼龟", 1, 0, 15, 8, 50, 0.1)
    {}
    void attackway() override;
};
class PiKaQiu: public Attack
{
public:
    PiKaQiu():  Attack("皮卡丘",1,0,15,8,50,0.1)
    {}
    void attackway() override;
};
//Hp
class MiaoWaZhongZi: public Hp
{
public:
    MiaoWaZhongZi(): Hp("妙蛙种子",1,0,10,8,60,0.1)
    {}
    void attackway() override;
};
class KaBiShou: public Hp
{
public:
    KaBiShou(): Hp("卡比兽",1,0,10,8,60,0.1)
    {}
    void attackway() override;
};
class GangQiu: public Hp
{
public:
    GangQiu(): Hp("钢球",1,0,10,8,60,0.1)
    {}
    void attackway() override;
};
//Defend
class DaYanShe:public Defend
{
public:
    DaYanShe(): Defend("大岩蛇",1,0,10,13,50,0.1)
    {}
    void attackway() override;
};
class JiLiDan:public Defend
{
public:
    JiLiDan(): Defend("吉利蛋",1,0,10,13,50,0.1)
    {}
    void attackway() override;
};
class KeDaYa:public Defend
{
public:
    KeDaYa(): Defend("可达鸭",1,0,10,13,50,0.1)
    {}
    void attackway() override;
};
//Speed
class ShanDianNiao:public Speed
{
public:
    ShanDianNiao():Speed("闪电鸟",1,0,10,8,50,0.2)
    {}
    void attackway() override;
};
class KuaiLong:public Speed
{
public:
    KuaiLong():Speed("快龙",1,0,10,8,50,0.2)
    {}
    void attackway() override;
};
class XiaoDaLa:public Speed
{
public:
    XiaoDaLa():Speed("小达拉",1,0,10,8,50,0.2)
    {}
    void attackway() override;
};
// 其他子类的实现类似

// 生成随机精灵的函数
std::vector<Pokemon*> getRandomPokemons(int num);
