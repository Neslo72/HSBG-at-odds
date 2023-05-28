
#include "minion.h"

static unsigned short _global_id = 1;


Minion::Minion(std::string _name, bool _summon, unsigned char _tier, unsigned short _type, 
                unsigned short _effect, int _attack, int _health) 
{
    // Identifiers
    name = _name;
    summon = _summon;
    tier = _tier;
    type = _type;
    effect = _effect;
    global_id = _global_id++;

    // Health and attack
    attack = _attack;
    health = _health;

    // Buff states
    poisonous = false;
    divine = false;
    windfury = false;
    taunt = false;
    reborn = false;

    // Combat fields
    battle_attack = _attack;
    battle_health = _health;
    battle_func = nullptr;
    deathrattle_func = nullptr;
}

Minion::Minion()
{
    type = NAM;  // Not A Minion
}

bool Minion::operator == (Minion *minion)
{
    if(minion->get_type() == NAM || minion->get_id() != global_id)
        return false;
    return true;
}

void Minion::set_effect(void (*func)(Board* ally, Board* enemy, Minion* self, Minion *target)) 
{
    battle_func = func;
}

void Minion::battle_effect(Board* ally, Board* enemy, Minion* self, Minion *target)
{
    if(battle_func == nullptr)
        return;
    
    battle_func(ally, enemy, self, target);
}

void Minion::set_deathrattle(void (*func)(Board* ally, Board* enemy, Minion* self, char death_idx, Minion *attacker)) 
{
    deathrattle_func = func;
}

void Minion::deathrattle(Board* ally, Board* enemy, Minion* self, char death_idx, Minion *attacker)
{
    if(deathrattle_func == nullptr)
        return;
    
    deathrattle_func(ally, enemy, self, death_idx, attacker);
}