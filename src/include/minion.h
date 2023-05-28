#ifndef _MINION_H_
#define _MINION_H_

#include <string>

// To be used as binary flags later for minion types
#define NEUTRAL 1 << 0
#define BEAST 1 << 1
#define MURLOC 1 << 2
#define MECH 1 << 3
#define DEMON 1 << 4
#define DRAGON 1 << 5
#define ELEMENTAL 1 << 6
#define PIRATE 1 << 7
#define QUILBOAR 1 << 8
#define NAGA 1 << 9
#define UNDEAD 1 << 10
#define NAM 1 << 15  // NAM = Not a minion

// To be used as binary flags for battle_effect calls
#define NO_EFFECT 0
#define ON_START 1 << 0
#define BEFORE 1 << 1
#define WHENEVER 1 << 2
#define AFTER 1 << 3
#define SUMMON 1 << 4
#define DEATHRATTLE 1 << 5
#define AVENGE 1 << 6
#define ATTACK 1 << 7
#define REBORN 1 << 8


// NOTES:
// Deathrattle functions will NOT remove the minion from the board,
// this will likely be left to the make_attack() function

// FUNCTION DEFINITIONS
// SUMMON FUNCITONS : m1 = self minion, m2 = summoned minion
// DEATHRATTE FUNCTIONS : attacker, target
// AVENGE FUNCTIONS : m1 = self minion, m2 = minion that died
// ATTACK FUNCTIONS : m1 = self minion, m2 = minion that attacks
// AURA / ON START : m1 = self, m2 = minion to get effect
// more ???

class Board;  // Need to define board as a class for the battle functions

// Base minion class
class Minion {

    public:

        // Constructor
        Minion(std::string _name, bool _summon, unsigned char tier, 
                unsigned short _type, unsigned short _effect, int _damage, int _health);
        Minion();
        ~Minion() { };

        // Compares global id values
        bool operator == (Minion *minion);

        // Function that can do something in battle
        void set_effect(void (*func)(Board* ally, Board* enemy, Minion* self, Minion* target));
        void battle_effect(Board* ally, Board* enemy, Minion* self, Minion* target);

        // Function on deathrattle
        void set_deathrattle(void (*func)(Board* ally, Board* enemy, Minion* self, char death_idx, Minion* attacker));
        void deathrattle(Board* ally, Board* enemy, Minion* self, char death_idx, Minion* attacker);

        // Health getters and setters
        void set_health(int _health) { health = _health; };
        int get_health() { return health; };

        // Attack getters and setters
        void set_attack(int _attack) { attack = _attack; };
        int get_attack() { return attack; };

        // Identifier getters
        std::string get_name() { return name; };
        unsigned char get_tier() { return tier; };
        unsigned short get_type() { return type; };
        unsigned short get_effect() { return effect; };
        unsigned short get_id() { return global_id; };

        // Toggle poison / venemous
        void toggle_poison() { poisonous = !poisonous; };
        bool is_poisonous() { return poisonous; };

        // Toggle divine shield
        void toggle_divine() { divine = !divine; };
        bool is_divine() { return divine; };

        // Toggle taunt
        void toggle_taunt() { taunt = !taunt; };
        bool is_taunt() { return taunt; };

        // Toggle reborn
        void toggle_reborn() { reborn = !reborn; };
        bool is_reborn() { return reborn; };

        // Toggle windfury
        void toggle_windfury() { windfury = !windfury; };
        bool is_windfury() { return windfury; };

        // Getter for summon state
        bool is_summon() { return summon; };

        // Battle health made public for easy / quick changes
        int battle_attack;
        int battle_health;
        unsigned char avenge_count;

    private:  // mostly permanent buffs / minion states

        // Identifiers
        std::string name;  // Change to int id?  easier to store
        unsigned char tier;
        unsigned short type;
        unsigned short effect;
        unsigned short global_id;

        // Health and attack
        int attack;
        int health;

        // Buffs + other states
        bool poisonous;
        bool divine;
        bool windfury;
        bool taunt;
        bool reborn;
        bool summon;  // true = summoned minion

        // Function pointers to specific card features
        void (*battle_func)(Board* ally, Board* enemy, Minion* self, Minion* target);
        void (*deathrattle_func)(Board* ally, Board* enemy, Minion* self, char death_idx, Minion* attacker);
};

#endif