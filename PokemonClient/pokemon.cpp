#include "pokemon.h"
#include <QDebug>
QJsonObject Pokemon:: toJsonObject() const {
    QJsonObject obj;
    obj["name"]=name;
    obj["level"] = level;
    obj["exp"]=exp;
    obj["attackPower"] = attackPower;
    obj["defensePower"] = defensePower;
    obj["healthPoints"] = healthPoints;
    obj["speedValue"] = speedValue;
    return obj;
}

std::vector<Pokemon*> getRandomPokemons(int num)
{
    std::vector<Pokemon*> pokemons;
    static Pokemon* pokemonList[] = {
        new XiaoHuoLong(),
        new JieNiGui(),
        new PiKaQiu(),
        new MiaoWaZhongZi(),
        new KaBiShou(),
        new GangQiu(),
        new DaYanShe(),
        new JiLiDan(),
        new KeDaYa(),
        new ShanDianNiao(),
        new KuaiLong(),
        new XiaoDaLa()
    };
    int count = sizeof(pokemonList) / sizeof(Pokemon*);
    for (int i = 0; i < num; ++i) {
        int index = QRandomGenerator::global()->generate() % count;
        pokemons.push_back(pokemonList[index]);
    }
    return pokemons;
}

int Pokemon::attack(Pokemon* target,int randnumber) {
    int damage;
    if (randnumber < target->speedValue*100 ) {
        qDebug()<<target->name<<"闪避伤害"<<randnumber;
        damage = 0;
    }
    // 实现攻击逻辑，例如计算伤害
    else
    {
        damage = this->attackPower*2 - target->defensePower;
        if(damage<=0)
            damage = 1;
    }
    return damage;
}

void Attack::levelUp(){
    if(this->level < MAX_LEVEL)
    {
        while(this->exp>=this->level*10)
        {
            this->attackPower+=level*4;
            this->healthPoints+=level*8;
            this->defensePower+=level*3;
            this->speedValue+=0.02;
            this->exp -= this->level*10;
            this->level+=1;
        }
    }
}

void Hp::levelUp(){
    if(this->level < MAX_LEVEL)
    {
        while(this->exp>=this->level*10)
        {
            this->attackPower+=level*3;
            this->healthPoints+=level*12;
            this->defensePower+=level*4;
            this->speedValue+=0.01;
            this->exp -= this->level*10;
            this->level+=1;
        }
    }
}

void Defend::levelUp(){
    if(this->level < MAX_LEVEL)
    {
        while(this->exp>=this->level*10)
        {
            this->attackPower+=level*3;
            this->healthPoints+=level*9;
            this->defensePower+=level*5;
            this->speedValue+=0.013;
            this->exp -= this->level*10;
            this->level+=1;
        }
    }
}

void Speed::levelUp(){
    if(this->level < MAX_LEVEL)
    {
        while(this->exp>=this->level*10)
        {
            this->attackPower+=level*4;
            this->defensePower+=level*3;
            this->healthPoints+=level*6;
            this->speedValue+=0.025;
            this->exp -= this->level*10;
            this->level+=1;
        }
    }
}


void XiaoHuoLong::attackway()
{
    attackWay="火焰攻击";
}

void JieNiGui::attackway()
{
    attackWay="水系攻击";
}

void PiKaQiu::attackway()
{
    attackWay="闪电攻击";
}

void MiaoWaZhongZi::attackway()
{
    attackWay="草系攻击";
}

void KaBiShou::attackway()
{
    attackWay="物理攻击";
}

void GangQiu::attackway()
{
    attackWay="物理攻击";
}

void DaYanShe::attackway()
{
    attackWay="土系攻击";
}

void JiLiDan::attackway()
{
    attackWay="物理攻击";
}

void KeDaYa::attackway()
{
    attackWay="水系攻击";
}

void ShanDianNiao::attackway()
{
    attackWay="闪电攻击";
}

void KuaiLong::attackway()
{
    attackWay="土系攻击";
}

void XiaoDaLa::attackway()
{
    attackWay="物理攻击";
}
