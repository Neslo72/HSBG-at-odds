
#include "minion_factories.h"


// ALL BEASTS CAN BE REBORN
// NEED TO ADD DEATHRATTLE FOR EACH...?

// DEATHRATTLE
// Manasaber deathrattle : summon two 0/1 cublings with taunt
void manasaber_deathrattle(Board *ally, Board *enemy, Minion* self, char death_idx, Minion *attacker) 
{
    for(unsigned char i = 0; i < 2; i++)  // add 2 tokens
    {
        if(ally->get_num_minions() >= MAX_MINION)  // out of space
            return;

        Minion* token = ally->summon_minion_at("Cubling", true, 1, BEAST, NO_EFFECT, 0, 1, death_idx);
        token->toggle_taunt();
        ally->process_effect(WHENEVER | SUMMON, enemy, token);
        ally->process_effect(AFTER | SUMMON, enemy, token);
    }
}  // TODO add new minions to a summon queue? store this in the board class?


// AVENGE
// Scavenging Hyena : whenever a friendly beast dies, gain +2/+1
void scavenging_hyena_effect(Board *ally, Board *enemy, Minion* self, Minion *died) 
{
    if((died->get_type() & BEAST) == 0)
        return;

    self->battle_attack += 2;
    self->battle_health += 1;
}

// ON_START | WHENEVER | SUMMON
// Humming Bird : Your other beasts have +2 attack
void humming_bird_effect(Board *ally, Board *enemy, Minion *self, Minion *target)
{
    if((target->get_type() & BEAST) == 0)
        return;

    target->battle_attack += 2;
}

// DEATHRATTLE
// This card does not have an *official* deathrattle, but this is
// a clever way to rid the card's aura effect on the board
// Humming bird : Your other beasts have +2 attack
void humming_bird_deathrattle(Board *ally, Board *enemy, Minion *self, char death_idx, Minion* attacker)
{
    for(unsigned char i = 0; i < ally->get_num_minions(); i++)
    {
        Minion* target = ally->get_minion_at(i);
        if(target->get_type() & BEAST)
        {
            if(target->battle_attack < 2)
                target->battle_attack = 0;
            else 
                target->battle_attack -= 2;
        }
    }
}


// DEATHRATTLE
// Leapfrogger 0__o
void leapfrogger_deathrattle(Board *ally, Board *enemy, Minion *self, char death_idx, Minion *attacker)
{
    // TODO I do not want to deal with leap frogger yet...
    printf("Leapfrogger not done yet lmoa\n");
}


// AFTER | ATTACK
// Monstrous Macaw : After this attacks, trigger another friendly minion's deathrattle
void monstrous_macaw_effect(Board *ally, Board *enemy, Minion *self, Minion *target)
{
    Minion* to_trigger = ally->get_random_effect(DEATHRATTLE);
    char death_idx = ally->get_minion_idx(to_trigger);
    to_trigger->deathrattle(ally, enemy, to_trigger, death_idx, nullptr);
    // TODO be careful of nullptr in deathrattles where ATTACKER is used... (leeroy)
}


// DEATHRATTLE
// Sewer Rat : Summon a 2/3 turtle with taunt
void sewer_rat_deathrattle(Board *ally, Board *enemy, Minion *self, char death_idx, Minion *attacker)
{
    if(ally->get_num_minions() >= MAX_MINION)  // board space full
        return;

    Minion* token = ally->summon_minion_at("Half-Shell", true, 1, BEAST, NO_EFFECT, 2, 3, death_idx);
    token->toggle_taunt();
    ally->process_effect(WHENEVER | SUMMON, enemy, token);
    ally->process_effect(AFTER | SUMMON, enemy, token);
}


// SUMMON
// Banana Slamma : After you summon a beast in combat, double its stats
void bannana_slamma_effect(Board *ally, Board *enemy, Minion *self, Minion *target)
{
    if((target->get_type() & BEAST) == 0)
        return;

    target->battle_attack *= 2;
    target->battle_health *= 2;
}


// DEATHRATTLE
// Rat Pack : Summon a number of 1/1 rats equal to this minion's attack
void rat_pack_deathrattle(Board *ally, Board *enemy, Minion *self, char death_idx, Minion *attacker)
{
    unsigned char num_summons = self->battle_attack;
    while(num_summons > 0 && ally->get_num_minions() < MAX_MINION)
    {
        Minion *token = ally->summon_minion_at("Rat", true, 1, BEAST, NO_EFFECT, 1, 1, death_idx);
        ally->process_effect(WHENEVER | SUMMON, enemy, token);
        ally->process_effect(AFTER | SUMMON, enemy, token);
        num_summons--;
    }
}


// DEATHRATTLE
// Rylak Metalhead : Trigger the battlecries of adjacent minions
void rylack_metalhead_deathrattle(Board *ally, Board *enemy, Minion *self, char death_idx, Minion *attacker)
{
    printf("Rylack Metalhead: Battlecries not implemented :(((\n");
    // TODO: implement battlecries?
}


// Reanimating Rattler
// Does nothing in combat


// DEATHRATTLE
// Sly Raptor : Summon another random beast.  Set its stats to 7/7
void sly_raptor_deathrattle(Board *ally, Board *enemy, Minion *self, char death_idx, Minion *attacker)
{
    return;  // TODO random feature needs to be added
    // Minion *summon = beast_factory("Random");  // we do not have a random feature yet
    // summon->battle_attack = 7;
    // summon->battle_health = 7;
    // ally->process_effect(SUMMON, enemy, summon);

    // Idea : create boards of each minion type, then get a random minion from there??
    // just a thought
}


// Bonemare
// Does nothing in combat


// SUMMON
// Mama bear : Whenever you summon a beast, give it +4/+4
void mama_bear_effect(Board *ally, Board *enemy, Minion *self, Minion *target) 
{
    if((target->get_type() & BEAST) == 0)
        return;

    target->battle_attack += 4;
    target->battle_health += 4;
}


// AVENGE (2)
// Palescale Crocolisk : Give another friendly beast +6/+6
void palescale_effect(Board *ally, Board *enemy, Minion *self, Minion *target)
{
    if(self->avenge_count < 2) 
    {
        self->avenge_count++;
        return;
    }

    self->avenge_count = 0;

    Minion* to_buff = ally->get_random_type(BEAST);
    if(to_buff == nullptr)
        return;
    
    to_buff->battle_attack += 6;
    to_buff->battle_health += 6;
}


// DEATHRATTLE
// Palescale Crocolisk : Give another friendly beast +6/+6
void palescale_deathrattle(Board *ally, Board *enemy, Minion *self, char death_idx, Minion *attacker)
{
    Minion* to_buff = ally->get_random_type(BEAST);
    if(to_buff == nullptr)
        return;
    
    to_buff->battle_attack += 6;
    to_buff->battle_health += 6;
}


// REBORN
// Sinrunner Blanchy : This is reborn with full health and enchantments
void blanchy_effect(Board *ally, Board *enemy, Minion *self, Minion *attacker)
{
    // TODO not sure how to handle this special case ...
    printf("Sinrunner Blanchy effect not yet implemented\n");
}


// SUMMON
// Felstomper : After you summon a minion in combat, give your minions +3 attack
void felstomper_effect(Board *ally, Board *enemy, Minion *self, Minion *attacker)
{
    for(unsigned char i = 0; i < ally->get_num_minions(); i++)
    {
        Minion *to_buff = ally->get_minion_at(i);
        to_buff->battle_attack += 3;
    }
}


// DEATHRATTLE
// Ghastcoiler : Summon 2 random deathrattle minions
void ghastcoiler_deathrattle(Board *ally, Board *enemy, Minion *self, char death_idx, Minion *attacker)
{
    // TODO get random deathrattles, how do?
    printf("Ghastcoiler : not yet implemented");
}


// WHENEVER | SUMMON
// Ocotosari : Summon a 2/2 tentacle (it gains +2/+2 permanently after you summon a minion in combat!)
void octosari_effect(Board *ally, Board *enemy, Minion *self, Minion *target)
{
    self->avenge_count++;  // this will be used as a summon counter, ignore the name...
}


// DEATHRATTLE
// Ocotosari : Summon a 2/2 tentacle (it gains +2/+2 permanently after you summon a minion in combat!)
void octosari_deathrattle(Board *ally, Board *enemy, Minion *self, char death_idx, Minion *attacker)
{
    int idx = ally->remove_minion(self);
    if(idx < 0)
        return;
    
    unsigned int token_stats = 2 + (self->avenge_count * 2);  // avenge count is summon count here
    Minion* token = ally->summon_minion_at("Tentacle of Octosari", true, 1, BEAST, NO_EFFECT, token_stats, token_stats, death_idx);
    ally->process_effect(WHENEVER | SUMMON, enemy, token);
    ally->process_effect(AFTER | SUMMON, enemy, token);
}



////////////  THE FACTORY  ////////////



// Create various beasts RAWR
// TODO : Maybe change the string ID system to a number ID system, would be quicker / better
Minion beast_factory(std::string _name) 
{

    Minion ret;

    // TAVERN 1
    if(_name == "Manasaber") 
    {
        ret = Minion(_name, false, 1, BEAST, DEATHRATTLE, 4, 1);
        ret.set_deathrattle(manasaber_deathrattle);
    }
    else if(_name == "Scavenging Hyena") 
    {
        ret = Minion(_name, false, 1, BEAST, AVENGE, 2, 2);
        ret.set_effect(scavenging_hyena_effect);
    }

    // TAVERN 2
    else if(_name == "Humming Bird")
    {
        ret = Minion(_name, false, 2, BEAST, ON_START | SUMMON, 0, 3);
        ret.set_deathrattle(humming_bird_deathrattle);
        ret.set_effect(humming_bird_effect);
    }
    else if (_name == "Leapfrogger")  // NOT DONE
    {
        ret = Minion(_name, false, 2, BEAST, DEATHRATTLE, 3, 3);
        ret.set_deathrattle(leapfrogger_deathrattle);
    }   
    else if (_name == "Sewer Rat")
    {
        ret = Minion(_name, false, 2, BEAST, DEATHRATTLE, 3, 2);
        ret.set_deathrattle(sewer_rat_deathrattle);
    }

    // TAVERN 3
    else if (_name == "Monstrous Macaw")
    {
        ret = Minion(_name, false, 3, BEAST, AFTER | ATTACK, 5, 3);
        ret.set_effect(monstrous_macaw_effect);
    }
    else if (_name == "Rat Pack")
    {
        ret = Minion(_name, false, 3, BEAST, DEATHRATTLE, 2, 2);
        ret.set_deathrattle(rat_pack_deathrattle);
    }
    else if(_name == "Rylack Metalhead")  // NOT DONE
    {
        ret = Minion(_name, false, 3, BEAST, DEATHRATTLE, 3, 4);
        ret.toggle_taunt();
        ret.set_deathrattle(rylack_metalhead_deathrattle);
    }

    // TAVERN 4
    else if(_name == "Banana Slamma")
    {
        ret = Minion(_name, false, 4, BEAST, AFTER | SUMMON, 2, 5);
        ret.set_effect(bannana_slamma_effect);
    }
    else if(_name == "Reanimating Rattler")
    {
        ret = Minion(_name, false, 4, BEAST, NO_EFFECT, 4, 3);  // could change effect to end of turn?
    }
    else if(_name == "Sly Raptor")  // NOT DONE
    {
        ret = Minion(_name, false, 4, BEAST, DEATHRATTLE, 1, 3);
        ret.set_deathrattle(sly_raptor_deathrattle);
    }

    // TAVERN 5
    else if(_name == "Bonemare")
    {
        ret = Minion(_name, false, 5, BEAST | UNDEAD, NO_EFFECT, 5, 5);
    }
    else if (_name == "Mama Bear")
    {
        ret = Minion(_name, false, 5, BEAST, WHENEVER | SUMMON, 4, 4);
        ret.set_effect(mama_bear_effect);
    }
    else if(_name == "Palescale Crocolisk")
    {
        ret = Minion(_name, false, 5, BEAST, AVENGE | DEATHRATTLE, 4, 5);
        ret.set_deathrattle(palescale_deathrattle);
        ret.set_effect(palescale_effect);
    }
    else if(_name == "Sinrunner Blanchy")  // NOT DONE
    {
        ret = Minion(_name, false, 5, BEAST | UNDEAD, REBORN, 3, 3);
        ret.set_effect(blanchy_effect);
    }

    // TAVERN 6
    else if(_name == "Felstomper")
    {
        ret = Minion(_name, false, 6, BEAST | DEMON, AFTER | SUMMON, 3, 7);
        ret.set_effect(felstomper_effect);
    }
    else if(_name == "Ghastcoiler")  // NOT DONE
    {
        ret = Minion(_name, false, 6, BEAST, DEATHRATTLE, 7, 7);
        ret.set_deathrattle(ghastcoiler_deathrattle);
    }
    else if(_name == "Octosari")
    {
        ret = Minion(_name, false, 6, BEAST, AFTER | SUMMON | DEATHRATTLE, 6, 7);
        ret.set_deathrattle(octosari_deathrattle);
        ret.set_effect(octosari_effect);
    }

    // ERROR
    else 
    {
        printf("Requested beast %s not found\n", _name.c_str());
    }
    return ret;
}