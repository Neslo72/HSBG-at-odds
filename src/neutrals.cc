
#include "minion_factories.h"

// DEATHRATTLE
// Selfless Hero : Give a random friendly minion Divine Shield
void selfless_deathrattle(Board *ally, Board *enemy, Minion *self, char death_idx, Minion *attacker)
{

}


void whelp_smuggler_effect(Board *ally, Board *enemy, Minion* self, Minion *target)
{

}


void greenthumb_effect(Board *ally, Board *enemy, Minion* self, Minion *target)
{

}

void straight_shot_effect(Board *ally, Board *enemy, Minion* self, Minion *target)
{

}

void tunnel_blaster_deathrattle(Board *ally, Board *enemy, Minion *self, char death_idx, Minion *attacker)
{

}

void kangors_deathrattle(Board *ally, Board *enemy, Minion *self, char death_idx, Minion *attacker)
{

}

void leeroy_deathrattle(Board *ally, Board *enemy, Minion *self, char death_idx, Minion *attacker)
{

}

void titus_effect(Board *ally, Board *enemy, Minion* self, Minion *target)
{

}

void mantid_queen_effect(Board *ally, Board *enemy, Minion* self, Minion *target)
{

}

void nadina_deathrattle(Board *ally, Board *enemy, Minion *self, char death_idx, Minion *attacker)
{

}

// Maybe do this in main ?? when targeting happens?
void zapp_effect(Board *ally, Board *enemy, Minion* self, Minion *target)
{

}




//////////// THE FACTORY ////////////



// Creates netural minions
Minion neutral_factory(std::string _name)
{

    Minion ret;

    // TAVERN 1
    if(_name == "Mistake")
    {
        // Type 0x7FFF sets ALL minion type bits
        ret = Minion(_name, false, 1, 0x7FFF, NO_EFFECT, 1, 3);
    }
    else if(_name == "Wrath Weaver")
    {
        ret = Minion(_name, false, 1, 0, AFTER | PLAY, 1, 4);
    }

    // TAVERN 2
    else if(_name == "Menagerie Mug")
    {
        ret = Minion(_name, false, 2, 0, BATTLECRY, 2, 2);
    }
    else if(_name == "Patient Scout")
    {
        // TODO Effect == SELL ??
        ret = Minion(_name, false, 2, 0, NO_EFFECT, 1, 1);
    }
    else if(_name == "Prophet of the Boar")
    {
        ret = Minion(_name, false, 2, 0, AFTER | PLAY, 2, 3);
    }
    else if(_name == "Selfless Hero")
    {
        ret = Minion(_name, false, 2, 0, NO_EFFECT, 2, 1);
        ret.set_deathrattle(selfless_deathrattle);
    }
    else if(_name == "Sparring Partner")
    {
        ret = Minion(_name, false, 2, 0, BATTLECRY, 3, 2);
        ret.toggle_taunt();
    }
    else if(_name == "Whelp Smuggler")
    {
        ret = Minion(_name, false, 2, 0, AFTER | STAT_GAIN, 2, 5);
        ret.set_effect(whelp_smuggler_effect);
    }

    // TAVERN 3
    else if(_name == "Budding Greenthumb")
    {
        ret = Minion(_name, false, 3, 0, AVENGE, 1, 4);
        ret.set_effect(greenthumb_effect);
    }
    else if(_name == "Faceless Disciple")
    {   
        ret = Minion(_name, false, 3, 0, BATTLECRY, 6, 4);
    }

    // TAVERN 4
    else if(_name == "Sindorei Straight Shot")
    {
        ret = Minion(_name, false, 4, 0, BEFORE | ATTACK, 3, 4);
        ret.toggle_divine();
        ret.toggle_windfury();
        ret.set_effect(straight_shot_effect);
    }
    else if(_name == "Strongshell Scavenger")
    {
        ret = Minion(_name, false, 4, 0, BATTLECRY, 2, 3);
    }
    else if(_name == "Treasure Seeker Elise")
    {
        // TODO Effect == AFTER | REFRESH ?? few applications
        ret = Minion(_name, false, 4, 0, NO_EFFECT, 5, 5);
    }
    else if(_name == "Tunnel Blaster")
    {
        ret = Minion(_name, false, 4, 0, DEATHRATTLE, 3, 7);
        ret.set_deathrattle(tunnel_blaster_deathrattle);
    }
    else if(_name == "Upbeat Duo")
    {
        ret = Minion(_name, false, 4, 0, NO_EFFECT, 4, 4);
    }
    else if(_name == "Vigilant Stoneborn")
    {
        ret = Minion(_name, false, 4, 0, BATTLECRY, 2, 6);
    }

    // TAVERN 5
    else if(_name == "Brann Bronzebeard")
    {
        // TODO battlecry huh?  MAYBE make special case
        ret = Minion(_name, false, 5, 0, ON_START, 2, 4);
    }
    else if(_name == "Drakkari Enchanter")
    {
        // TODO effect == AFTER | TURN ?  or special baron / brann case?
        ret = Minion(_name, false, 5, 0, NO_EFFECT, 1, 5);
    }
    else if(_name == "Kangor's Apprentice")
    {
        ret = Minion(_name, false, 5, 0, DEATHRATTLE, 3, 6);
        ret.set_deathrattle(kangors_deathrattle);
    }
    else if(_name == "Leeroy the Reckless")
    {
        ret = Minion(_name, false, 5, 0, DEATHRATTLE, 6, 2);
        ret.set_deathrattle(leeroy_deathrattle);
    }
    else if(_name == "Lightfang Enforcer")
    {
        // TODO effect == AFTER | TURN ?
        ret = Minion(_name, false, 5, 0, NO_EFFECT, 2, 2);
    }
    else if(_name == "Nomi, Kitchen Nightmare")
    {
        ret = Minion(_name, false, 5, 0, AFTER | PLAY, 4, 4);
    }
    else if(_name == "Titus Rivendare")
    {
        ret = Minion(_name, false, 5, 0, ON_START, 1, 7);
        ret.set_effect(titus_effect);
    }
    else if(_name == "Tortollan Blue Shell")
    {
        ret = Minion(_name, false, 5, 0, SELL, 4, 7);
    }

    // TAVERN 6
    else if(_name == "Mantid Queen")
    {
        ret = Minion(_name, false, 6, 0, ON_START, 5, 5);
        ret.set_effect(mantid_queen_effect);
    }
    else if(_name == "Nadina the Red")
    {
        ret = Minion(_name, false, 6, 0, DEATHRATTLE, 8, 4);
        ret.set_deathrattle(nadina_deathrattle);
    }
    else if(_name == "Orgozoa, the Tender")
    {
        ret = Minion(_name, false, 6, 0, NO_EFFECT, 3, 7);
    }
    else if(_name == "The Boogie Monster")
    {
        ret = Minion(_name, false, 6, 0, AFTER | PLAY, 3, 8);
    }
    else if(_name == "The Walking Fort")
    {
        ret = Minion(_name, false, 6, 0, NO_EFFECT, 4, 6);
    }
    else if(_name == "Uther the Lightbringer")
    {
        ret = Minion(_name, false, 6, 0, BATTLECRY, 5, 6);
    }
    else if(_name == "Zapp Slywick")
    {
        ret = Minion(_name, false, 6, 0, BEFORE | ATTACK, 8, 16);
        ret.set_effect(zapp_effect);
    }

    // ERROR
    else
    {
        printf("Requested neutral %s not found\n", _name.c_str());
    }
    return ret;
}
