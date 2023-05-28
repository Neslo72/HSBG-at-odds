#ifndef _BOARD_H_
#define _BOARD_H_

#include <queue>
#include "minion.h"

#define MAX_MINION 7


// Board class, try to keep this small for recursion (if it indeed works)
// Current size == 64  (LOL this will no longer be small :))) )
class Board {

    public: 
        Board();
        ~Board();

        ///// BOARD MANIPULATION /////

        // // Add minion to board
        // int add_minion(Minion* to_add);

        // Add minion to board at an index
        int add_minion_at(Minion* to_add, unsigned char idx);

        // Get minion at idx
        Minion* get_minion_at(unsigned char idx);

        // Get a random minion of a specified type
        Minion* get_random_minion(unsigned short _type);

        // Remove minion by pointer.  Returns removed index
        int remove_minion(Minion* to_remove);

        // Remove minion by index.  Returns pointer to minion removed
        Minion* remove_minion_at(unsigned char idx);

        // Swap the position of two minions
        bool swap_minions(unsigned char src_idx, unsigned char dst_idx);

        // Check for minion on board
        bool minion_exists(Minion* minion);

        ///// EFFECTS PROCESSING ///// 

        // Initialize effect queues
        void init_effects();

        // requeue an effect if a new minion is summoned
        void requeue_effect(unsigned short _effect);

        // Process avenge queue for board
        void process_effect(unsigned short _effect, Board* enemy, Minion* target);

        ///// MISC /////

        // Get a valid minion target, used by the enemy
        Minion* get_target();

        // Get a valid attacker minion
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
        unsigned char num_taunt_minions;
        Minion* minions[MAX_MINION];
        Minion* taunt_minions[MAX_MINION];  // Taunt quick storage
        Minion minion_storage[MAX_MINION];

        // Effect flags / trackers
        // unsigned char num_summons;
        // unsigned char acked_summons;

        // Effects queues
        std::queue<Minion*> summon_queue;
        std::queue<Minion*> avenge_queue;
        std::queue<Minion*> attack_queue;

        // Add minion to taunt quick tracker
        void add_taunt(Minion* to_add);

        // Remove minion from taunt quick tracker
        void remove_taunt(Minion* to_remove);

        // Store dead minions
        // std::vector<Minion*> graveyard;  // will I need this?  I think maybe so...
        // How else to tell most recent minion death type?
        // This could be made efficient with a most recent death type field???
};

#endif