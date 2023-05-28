#ifndef _BOARD_H_
#define _BOARD_H_

#include <queue>
#include <string.h>
#include "minion.h"

#define MAX_MINION 7


// Board class
class Board {

    public: 
        Board();
        ~Board() {};

        ///// BOARD MANIPULATION /////

        // Add minion to board at an index
        int add_minion_at(Minion* to_add, unsigned char idx);

        // Create (summon) a new minion at an index
        Minion* summon_minion_at(std::string _name, bool summon, unsigned short _tier, 
                                unsigned short _type, unsigned short _effect, 
                                int _attack, int _health, unsigned char idx);

        // Get minion at idx
        Minion* get_minion_at(unsigned char idx);

        // Get a random minion of a specified type
        Minion* get_random_type(unsigned short _type);

        // Get a random minion of a specified effect
        Minion* get_random_effect(unsigned short _effect);

        // Get the index of a minion on the board
        char get_minion_idx(Minion* minion);

        // Remove minion by pointer.  Returns removed index or -1 on error
        char remove_minion(Minion* to_remove);

        // Swap the position of two minions
        bool swap_minions(unsigned char src_idx, unsigned char dst_idx);

        ///// EFFECTS PROCESSING ///// 

        // Process avenge queue for board
        void process_effect(unsigned short _effect, Board* enemy, Minion* target);

        ///// MISC /////

        // Get a valid minion target, used by the enemy
        Minion* get_target();

        // Get a valid attacker minion
        // This is the ONLY function to increase atk_idx !!!
        Minion* get_attacker();

        // Get damage from remaining minions
        unsigned char get_damage(unsigned char player_tier);

        // num_minions getter
        unsigned char get_num_minions() { return num_minions; };

        // Print the board in text
        void print();

    private:

        // Board fields
        unsigned char atk_idx;
        unsigned char num_minions;
        Minion minions[MAX_MINION];

        // Store dead minions
        // std::vector<Minion*> graveyard;  // will I need this?  I think maybe so...
};

#endif