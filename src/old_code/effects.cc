#include "effect.h"


Effect::Effect() 
{
    for(unsigned char i = 0; i < MAX_MINION; i++)
        effect_queue[i] = nullptr;
    start_idx = 0;
    end_idx = 0;
}

// There should never be more than 7 effects
void Effect::add_effect(Minion* to_add) 
{
    effect_queue[end_idx] = to_add;
    end_idx = (end_idx + 1) % MAX_MINION;
}


void Effect::remove_effect(Minion* to_remove)
{
    unsigned char i = start_idx;
    while(i != end_idx) 
    {
        if(effect_queue[i] == to_remove)  // locate to_remove
            break;
        i = (i + 1) % MAX_MINION;
    }

    if(effect_queue[i] == to_remove) 
    {
        while(i != end_idx)  // shift remaining over
        {
            effect_queue[i] = effect_queue[(i + 1) % MAX_MINION];
            i = (i + 1) % MAX_MINION;
        }
        effect_queue[i] = nullptr;  // remove the final
    }
}