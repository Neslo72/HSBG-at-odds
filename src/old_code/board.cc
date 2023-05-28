
#include "board.h"



//////////// BOARD MANIPULATION ////////////



// Initialize board with nullptrs
Board::Board() 
{
    for(int i = 0; i < MAX_MINION; i++) 
    {
        minions[i] = nullptr;
        taunt_minions[i] = nullptr;
    }
    atk_idx = 0;
    num_minions = 0;
    num_taunt_minions = 0;
}


// Delete all minions on the board
Board::~Board() 
{
    for(int i = 0; i < MAX_MINION; i++) 
        if(minions[i] != nullptr)
            delete minions[i];
}


// // Add a minion to the board (last_idx)
// int Board::add_minion(Minion* to_add)
// {
//     if (num_minions >= MAX_MINION)
//         return -1;
    
//     minions[num_minions] = to_add;
//     return num_minions++;
// }


// Add minion to board at an index.  Remains compressed
int Board::add_minion_at(Minion* to_add, unsigned char idx) 
{
    if(num_minions >= MAX_MINION)
        return -1;

    if(idx > num_minions)  // Add to end if idx > num_minions
        idx = num_minions;

    // Shift all minions right of to_add's new index
    for(unsigned char i = num_minions; i > idx && i > 0; i--)
        minions[i] = minions[i - 1];
    
    if(to_add->is_taunt())  // Taunt minion quick storage
        add_taunt(to_add);
    
    minions[idx] = to_add;
    return num_minions++;
} 


// Get minion at idx
Minion* Board::get_minion_at(unsigned char idx)
{
    if(idx >= MAX_MINION)  // OOB
        return nullptr;

    return minions[idx];
}


// Get a random minion with a specified type
Minion* Board::get_random_minion(unsigned short _type) 
{
    if(num_minions == 0)
        return nullptr;  // edge case but necessary

    if(_type == 0)  // no type specified
        return minions[rand() % num_minions];

    unsigned char num_valid = 0;
    Minion* valid_minions[MAX_MINION];
    for(unsigned char i = 0; i < num_minions; i++)
    {
        if(minions[i]->get_type() & _type) 
        {
            valid_minions[num_valid] = minions[i];
            num_valid++;
        }
    }
    if(num_valid == 0)
        return nullptr;
    
    return valid_minions[rand() % num_valid];
}


// Remove and delete a minion from the board.  Remains compressed
int Board::remove_minion(Minion* to_remove)
{
    if(to_remove == nullptr)
        return -1;

    unsigned char idx = 0;
    for(; idx < MAX_MINION; idx++)  // Get to_remove's index
        if(minions[idx] == to_remove)
            break;

    if(idx >= num_minions)  // Not found || OOB
        return -1;
    
    // Shift remaining minions left of to_delete's old index
    for(unsigned char i = idx; i < MAX_MINION - 1; i++)
        minions[i] = minions[i + 1];

    minions[MAX_MINION - 1] = nullptr;
    num_minions--;

    if(to_remove->is_taunt())  // Taunt minion quick storage removal
        remove_taunt(to_remove);

    return idx;
}


// Remove and delete a minion from board
Minion* Board::remove_minion_at(unsigned char idx)
{
    if(idx > MAX_MINION || idx > num_minions - 1)
        return nullptr;
    if(minions[idx] == nullptr)
        return nullptr;
    
    // Shift remaining minions left of to_delete's old index
    Minion* to_delete = minions[idx];
    for(unsigned char i = idx; i < MAX_MINION - 1; i++)
        minions[i] = minions[i + 1];

    minions[MAX_MINION - 1] = nullptr;
    num_minions--;

    if(to_delete->is_taunt())  // Taunt minion quick storage removal
        remove_taunt(to_delete);

    return to_delete;
}


// Swap the position of two minions
bool Board::swap_minions(unsigned char src_idx, unsigned char dst_idx)
{
    if(src_idx >= MAX_MINION || dst_idx >= MAX_MINION)
        return false;

    if(minions[src_idx] == nullptr || minions[dst_idx] == nullptr)
        return false;

    Minion* tmp = minions[src_idx];
    minions[src_idx] = minions[dst_idx];
    minions[dst_idx] = tmp;
    return true;
}


// Check for minion on board
bool Board::minion_exists(Minion* minion)
{
    for(unsigned char i = 0; i < num_minions; i++)
    {
        if(minion == minions[i])
            return true;
    }
    return false;
}



//////////// EFFECTS PROCESSING ////////////



// Initialize effect queues
// Needs to be called on each board before each attack phase
void Board::init_effects()
{
    for(unsigned char i = 0; i < num_minions; i++)
    {
        unsigned short effect = minions[i]->get_effect();
        if(effect & SUMMON)
            summon_queue.push(minions[i]);
        if(effect & AVENGE)
            avenge_queue.push(minions[i]);
        if(effect & BEFORE_ATK)
            attack_queue.push(minions[i]);
    }
}


// Requeue a specific effect if a new minion is summoned
// I wrote this after init_effects, but that could be simplified now...
void Board::requeue_effect(unsigned short _effect)
{
    std::queue<Minion*> *queue;
    switch(_effect)  // setup the queue
    {
        case SUMMON:
            queue = &summon_queue;
            break;
        case AVENGE:
            queue = &avenge_queue;
            break;
        case BEFORE_ATK:
            queue = &attack_queue;
            break;
        default:
            return;
    }

    std::queue<Minion*> requeue;
    for(unsigned char i = 0; i < num_minions; i++)
    {
        if(minions[i]->get_effect() & _effect)
            requeue.push(minions[i]);
    }
    *queue = requeue;
}


// Process effects queue
// The mode changes which effects are processed
// m2 is defined by the effect being called:
//
// SUMMON FUNCITONS : m2 = summoned minion
// DEATHRATTE FUNCTIONS :  m2 = minion that attacked it
// AVENGE FUNCTIONS :  m2 = minion that died
// ATTACK FUNCTIONS :  m2 = minion that attacks
void Board::process_effect(unsigned short _effect, Board *enemy, Minion *m2)
{
    std::queue<Minion*> *queue;
    switch(_effect)  // setup the queue
    {
        case SUMMON:
            queue = &summon_queue;
            break;
        case AVENGE:
            queue = &avenge_queue;
            break;
        case BEFORE_ATK:
            queue = &attack_queue;
            break;
        default:
            return;
    }

    std::queue<Minion*> requeue;
    while(queue->size() > 0)  // Go through queued minion effects
    {
        Minion* curr = queue->front();
        if(minion_exists(curr)) 
        {
            curr->battle_effect(this, enemy, curr, m2);
            requeue.push(curr);
        }  
        queue->pop();
    }
    *queue = requeue;
}



///////////  MISC  ///////////


// This function could be way more complex without taunt_minions[]
// I decided on the taunt array since this funciton will likely be called alot
// so optimizing get_target over the add / remove functions for taunt_minions[]
Minion* Board::get_target()
{
    if(num_minions == 0)
        return nullptr;

    if(num_taunt_minions == 0) 
        return minions[rand() % num_minions];

    return taunt_minions[rand() % num_taunt_minions];
}


// Get a valid attacker minion
Minion* Board::get_attacker()
{
    if(num_minions == 0)
        return nullptr;

    if(atk_idx >= num_minions)
        atk_idx = 0;

    Minion* ret = minions[atk_idx];
    atk_idx = (atk_idx + 1) % num_minions;
    return ret;
}


// Get damage from remaining minions
unsigned char Board::get_damage(unsigned char player_tier)
{
    unsigned char total = player_tier;
    for(int i = 0; i < num_minions; i++)
    {
        total += minions[i]->get_tier();
    }
    return total;
}

// Prinout the board in text
void Board::print() 
{
    printf("BOARD: ");
    for(int i = 0; i < MAX_MINION; i++) 
    {
        if(minions[i] != nullptr)
            printf("(%d, %d) %s, ", minions[i]->battle_attack, minions[i]->battle_health, minions[i]->get_name().c_str());
    }
    printf("\n");
}


void Board::add_taunt(Minion* to_add) 
{
    taunt_minions[num_taunt_minions++];  // this should never go OOB, ignoring check
}

void Board::remove_taunt(Minion* to_remove)
{
    unsigned char idx = 0;
    for(; idx < num_taunt_minions; idx++)
        if(taunt_minions[idx] == to_remove)
            break;

    if(idx >= num_taunt_minions)
        return;

    Minion* last = taunt_minions[num_taunt_minions - 1];
    taunt_minions[idx] = last;
    taunt_minions[num_taunt_minions--] = nullptr;
}

// // Get the idex of the next valid minion
// // Returns -1 if no more valid minions
// int next_minion(int idx, Minion* minions[])
// {
//     unsigned char i = idx + 1;
//     for(; idx < MAX_MINION; i++) 
//     {
//         if(minions[i] != nullptr)
//             break;
//     }
//     return i;
// }

// // Compress empty nullptr spaces
// void Board::compress()
// {
//     for(unsigned char i = 0; i < MAX_MINION - 1; i++) 
//     {
//         if(minions[i] == nullptr)  // Identify slot to compress
//         { 
//             unsigned char next_idx = next_minion(i, minions);
//             if(next_idx >= MAX_MINION)  // No more valid minions
//                 return;
            
//             unsigned char shift_idx = i;
//             unsigned char shift_by = next_idx - i;
//             for(; shift_idx < MAX_MINION - shift_by; shift_idx++) // Perform shift
//                 minions[shift_idx] = minions[shift_idx + shift_by];

//             for(;shift_idx < MAX_MINION; shift_idx++)  // Fill remaining with nullptr
//                 minions[shift_idx] = nullptr;
//         }
//     }
// }