#ifndef _BOARDSTATE_H_
#define _BOARDSTATE_H_

#include <vector>

#include "board.h"

// Boardstate should hold the state of the board... (duh)
class BoardState {

    public:
        BoardState();
        ~BoardState() {};

        // Attack enemy board
        void attack_once();

    private:

        unsigned char turn;

        // Player 1
        Board *board;
        unsigned char atk_idx;
        std::vector<Minion*> on_summon;
        std::vector<Minion*> on_attack;
        std::vector<Minion*> on_avenge;
};

#endif