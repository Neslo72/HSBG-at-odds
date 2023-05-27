
#include "board.h"

//////////// BOARD MANIPULATION ////////////



// Initialize board with NAM minions
Board::Board() 
{
    for(int i = 0; i < MAX_MINION; i++) 
        minions[i] = Minion();  // initialize all to NAM
    
    atk_idx = MAX_MINION;  // set to max so it rollsback on get_attacker()
    num_minions = 0;
}


// Add minion to board at an index.  Remains compressed
// NOTE : This function could be more efficient with another 'board_pos[]'
// array that stores the positing of the minion in the minions array.  That
// way swap operations only change a byte of data instead of the entire minion
// object.  Remains to be seen if this would be a big optimization
// This optimization would be rather hard to keep track of tho...
int Board::add_minion_at(Minion* to_add, unsigned char idx) 
{
    if(num_minions >= MAX_MINION)
        return -1;

    if(idx > num_minions)
        idx = num_minions;

    for(unsigned char i = num_minions; i > idx; i--)  // shift minions right of idx
        minions[i] = minions[i - 1];

    // No changes to atk_idx, it will start again with the new summon

    minions[idx] = *to_add;
    return num_minions++;
} 


// Same as add_minion_at but creates the minion too
Minion* Board::summon_minion_at(std::string _name, bool _summon, unsigned short _tier, 
                                unsigned short _type, unsigned short _effect, 
                                int _attack, int _health, unsigned char idx)
{
    if(num_minions >= MAX_MINION)
        return nullptr;

    if(idx > num_minions)
        idx = num_minions;

    for(unsigned char i = num_minions; i > idx; i--)  // shift minions right of idx
        minions[i] = minions[i - 1];

    // Again, no changes to atk_idx

    minions[idx] = Minion(_name, _summon, _tier, _type, _effect, _attack, _health);
    num_minions++;
    return &minions[idx];
} 


// Get minion at idx
Minion* Board::get_minion_at(unsigned char idx)
{
    if(idx >= num_minions)  // OOB 
        return nullptr;

    return &minions[idx];
}


// Get a random minion with a specified type
// If you just want a fully random minion, specify no type (0)
Minion* Board::get_random_type(unsigned short _type) 
{
    if(_type == 0)  // no type specified, return random
        return &minions[rand() % num_minions];

    unsigned char num_valid = 0;
    unsigned char valid_minions[num_minions];
    for(unsigned char i = 0; i < num_minions; i++)
    {
        if(minions[i].get_type() & _type) 
        {
            valid_minions[num_valid] = i;
            num_valid++;
        }
    }
    if(num_valid == 0)
        return nullptr;
    
    return &minions[valid_minions[rand() % num_valid]];
}


// Get a random minion with a specified effect (i.e DEATHRATTLE)
Minion* Board::get_random_effect(unsigned short _effect) 
{
    unsigned char num_valid = 0;
    unsigned char valid_minions[num_minions];
    for(unsigned char i = 0; i < num_minions; i++)
    {
        if((minions[i].get_effect() & _effect) == _effect) 
        {
            valid_minions[num_valid] = i;
            num_valid++;
        }
    }
    if(num_valid == 0)
        return nullptr;
    
    return &minions[valid_minions[rand() % num_valid]];
}


// Check for minion on board
char Board::get_minion_idx(Minion* minion)
{
    for(unsigned char i = 0; i < num_minions; i++)
    {
        if(minions[i] == minion)
            return i;
    }
    return -1;
}


// Remove and delete a minion from the board.
char Board::remove_minion(Minion* to_remove)
{
    char idx = get_minion_idx(to_remove);

    if(idx < 0 || idx >= num_minions)  // OOB || not a minion
        return -1;
    
    // Shift remaining minions right of to_delete's old index
    for(unsigned char i = idx; i < num_minions - 1; i++)
        minions[i] = minions[i + 1];

    if(atk_idx >= idx)  // Adjust atk_idx with shifted minions
        atk_idx--;

    num_minions--;
    minions[num_minions] = Minion();  // Add NAM minion in newely empty slot
    return idx;
}


// Swap the position of two minions
// Currently unused
bool Board::swap_minions(unsigned char src_idx, unsigned char dst_idx)
{
    if(src_idx >= num_minions || dst_idx >= num_minions)
        return false;

    Minion tmp = minions[src_idx];
    minions[src_idx] = minions[dst_idx];
    minions[dst_idx] = tmp;
    return true;
}



//////////// EFFECTS PROCESSING ////////////



// Process effects queue
// The mode changes which effects are processed
// m2 is defined by the effect being called:
//
// SUMMON FUNCITONS : m2 = summoned minion
// DEATHRATTLE FUNCTIONS :  m2 = minion that attacked it
// AVENGE FUNCTIONS :  m2 = minion that died
// ATTACK FUNCTIONS :  m2 = minion that attacks
void Board::process_effect(unsigned short _effect, Board *enemy, Minion *m2)
{
    // It seems effects are processed left to right for original minions
    // then left to right for summoned minions.  This is still subject to change
    // if I find my effect ordering conclusion to be wrong...

    // Loop through original minions
    for(unsigned char i = 0; i < num_minions; i++)
        if(!minions[i].is_summon() && ((minions[i].get_effect() & _effect) == _effect))  // needs to have exact same effect
            minions[i].battle_effect(this, enemy, &minions[i], m2);

    // Loop through summons
    for(unsigned char i = 0; i < num_minions; i++)
        if(minions[i].is_summon() && ((minions[i].get_effect() & _effect) == _effect))  // needs to have exact same effect
            minions[i].battle_effect(this, enemy, &minions[i], m2);
}



///////////  MISC  ///////////


// Get a valid target from the board of minions
// There could be a way to optimize this if the taunt_idx[] was 
// stored with the board, but adds more confusing overhead...
Minion* Board::get_target()
{
    if(num_minions == 0)
        return nullptr;

    unsigned char num_taunt = 0;
    unsigned char taunt_idx[num_minions];
    for(unsigned char i = 0; i < num_minions; i++)
    {
        if(minions[i].is_taunt()) 
        {
            taunt_idx[num_taunt] = i;
            num_taunt++;
        }
    }

    if(num_taunt == 0)
        return &minions[rand() % num_minions];

    return &minions[taunt_idx[rand() % num_taunt]];
}


// Get a valid attacker minion
Minion* Board::get_attacker()
{
    atk_idx = (atk_idx + 1) % num_minions;
    return &minions[atk_idx];
}


// Get damage from remaining minions
unsigned char Board::get_damage(unsigned char player_tier)
{
    unsigned char total = player_tier;
    for(int i = 0; i < num_minions; i++)
    {
        total += minions[i].get_tier();
    }
    return total;
}

// Prinout the board in text
void Board::print() 
{
    printf("BOARD: ");
    for(int i = 0; i < num_minions; i++) 
    {
        if(minions[i].is_poisonous())
            printf("P");
        if(minions[i].is_divine())
            printf("D");
        if(minions[i].is_reborn())
            printf("R");
        if(minions[i].is_taunt())
            printf("T");
        printf("(%d, %d) %s, ", minions[i].battle_attack, minions[i].battle_health, minions[i].get_name().c_str());
    }
    printf("\n");
}