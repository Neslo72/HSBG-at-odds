#ifndef _EFFECT_H_
#define _EFFECT_H_

#include "minion.h"  

// This is redeclared in board.h...
#define MAX_MINION 7

class Board;

class Effect {

    public: 
        Effect();
        ~Effect() {};

        void add_effect(Minion* to_add);
        void remove_effect(Minion* to_remove);

    private:
        Minion* effect_queue[MAX_MINION];
        unsigned char start_idx;
        unsigned char end_idx;
};


#endif