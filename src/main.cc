#include <stdio.h>
#include <iostream>

#include "minion_factories.h"

#define MAX_DAMAGE 48
#define SIMULATIONS 100000

// Uncomment this for detailed board state printouts
// #define SIMULATIONS 1
// #define PRINTOUT

using namespace std;


// Make 1 attack on two input boards with the minions in combat
// ALLY ENEMY ATTACKER and TARGET will change for each process_effect 
// call depending on the effect to be processed!
void make_attack(Board* ally, Board* enemy, Minion* attacker, Minion* target) 
{
    // Before attack effect processing here
    ally->process_effect(BEFORE | ATTACK, enemy, target);

    // Prepare for attack by checking buff states
    int attacker_damage = attacker->battle_attack;
    int target_damage = target->battle_attack;

    // Poison (venemous) damage logic
    if(attacker->is_poisonous())
    {
        attacker_damage = 2147483648;  // int max :3
        attacker->toggle_poison();  // Venemous only allows 1 poison attack
    }
    if(target->is_poisonous())
    {
        target_damage = 2147483648;  // This could be problem for tracking / display?
        target->toggle_poison();  // Venemous only allows 1 poison attack
    }

    // Divine Sheild damage logic
    if(attacker->is_divine())
    {
        target_damage = 0;
        attacker->toggle_divine();
    }
    if(target->is_divine())
    {
        attacker_damage = 0;
        target->toggle_divine();
    }

    // ATTACK *happens* HERE (health trade)
    attacker->battle_health -= target_damage;
    target->battle_health -= attacker_damage;

    // Process after attack effects
    ally->process_effect(AFTER | ATTACK, enemy, target);

    // Process minion deathrattles + avenge + reborn
    if(attacker->battle_health <= 0) 
    {
        Minion dead_attacker = *attacker;  // copy for use after removal
        char death_idx = ally->remove_minion(attacker);
        //printf("ATTACKER %s DIED [%d] ", dead_attacker.get_name().c_str(), death_idx);
        dead_attacker.deathrattle(ally, enemy, &dead_attacker, death_idx, target);
        ally->process_effect(AVENGE, enemy, &dead_attacker);
        // TODO process REBORN
    }
    if(target->battle_health <= 0) 
    {
        Minion dead_target = *target;  // copy for use after removal
        char death_idx = enemy->remove_minion(target);
        //printf("TARGET %s DIED [%d] ", dead_target.get_name().c_str(), death_idx);
        dead_target.deathrattle(enemy, ally, &dead_target, death_idx, attacker);
        enemy->process_effect(AVENGE, ally, &dead_target);
    }
    // Summon processing calls happen IN DEATHRATTLES where summons happen!
    // must include both WHENEVER and AFTER cases for SUMMON !

    // One attack round complete!
}


// Main
int main(int argc, char* argv[]) 
{
    // Main setsup the player's boards then runs a number of simulations based
    // upon the SIMULATIONS define.  If PRINTOUT is defined, the board
    // state will be printed after each attack -- Keep SIMULATIONS low if 
    // PRINTITOUT is defined.

    // Randomize seed for now
    // Set to fixed number for detailed testing
    srand(time(NULL));

    // Testing 1 round randomness between some beasts
    Minion p1_minions[MAX_MINION];
    Minion p2_minions[MAX_MINION];

    Board p1_init;
    Board p2_init;

    // Stats setup
    unsigned int ties = 0;
    unsigned int p1_wins = 0;
    unsigned int p2_wins = 0;
    unsigned int p1_damage[MAX_DAMAGE] = { 0 };
    unsigned int p2_damage[MAX_DAMAGE] = { 0 };

    // p1 minion generation
    p1_minions[0] = beast_factory("Rat Pack");
    p1_minions[1] = beast_factory("Rat Pack");
    p1_minions[2] = beast_factory("Sewer Rat");
    p1_minions[2].toggle_taunt();
    p1_minions[3] = beast_factory("Sewer Rat");
    p1_minions[3].toggle_taunt();
    p1_minions[4] = beast_factory("Palescale Crocolisk");
    p1_minions[5] = beast_factory("Palescale Crocolisk");
    for(int i = 0; i < 5; i++)
    {
        p1_init.add_minion_at(p1_minions + i, i);
    }

    // p2 minion generation
    p2_minions[0] = neutral_factory("Treasure Seeker Elise");
    p2_minions[1] = beast_factory("Sewer Rat");
    p2_minions[1].toggle_taunt();
    p2_minions[2] = beast_factory("Sewer Rat");
    p2_minions[2].toggle_taunt();
    p2_minions[3] = beast_factory("Mama Bear");
    p2_minions[4] = beast_factory("Mama Bear");
    p2_minions[5] = beast_factory("Banana Slamma");
    for(int i = 0; i < 5; i++)
    {
        p2_init.add_minion_at(p2_minions + i, i);
    }


    // Run simulations
    // This can be turned into a function to utilize multithreading later on...
    for(int i = 0; i < SIMULATIONS; i++) 
    {
        Board p1 = p1_init;
        Board p2 = p2_init;
       
        Board* ally;
        Board* enemy;

        // Decide who starts : 
        // The player with more minions will go first
        // If both players have the same # of minions, then it is random
        int dif = p1.get_num_minions() - p2.get_num_minions();
        if(dif > 0) 
        {
            //printf("p1 starts!  p1 == evens\n");
            ally = &p1;
            enemy = &p2;
        }
        else if(dif < 0)
        {
            //printf("p2 starts!  p1 == odds\n");
            ally = &p2;
            enemy = &p1;
        }
        else 
        {
            if(rand() % 2)
            {
                //printf("p1 starts!  p1 == evens\n");
                ally = &p1;
                enemy = &p2;
            }
            else
            {
                //printf("p2 starts!  p1 == odds\n");
                ally = &p2;
                enemy = &p1;
            }
        }

        #ifdef PRINTOUT
            if(ally == &p1)
                printf("p1 starts!  p1 == evens\n");
            else
                printf("p2 starts!  p1 == odds\n");
            int counter = 0;
        #endif

        ////////////  COMBAT LOOP  ////////////

        while(1)
        {
            // Setup minions
            Minion* attacker = ally->get_attacker();  // Zapp case here?
            Minion* target = enemy->get_target();

            #ifdef PRINTOUT
                p2.print();
                p1.print();
                printf("Attack %d : ATTACKER %s [%d] TARGET %s [%d]\n", 
                        counter, attacker->get_name().c_str(), ally->get_minion_idx(attacker),
                        target->get_name().c_str(), enemy->get_minion_idx(target));
                counter++;
            #endif

            // Make attack
            make_attack(ally, enemy, attacker, target);
            if(p1.get_num_minions() <= 0 || p2.get_num_minions() <= 0)
                break;

            if(ally->get_minion_idx(attacker) != -1 && attacker->is_windfury())  // windfury
            {
                target = enemy->get_target();
                make_attack(ally, enemy, attacker, target);
                if(p1.get_num_minions() <= 0 || p2.get_num_minions() <= 0)
                    break;
            }

            // Swap who is attacking
            Board* tmp = ally;
            ally = enemy;
            enemy = tmp;
        }

        ////////////  END COMBAT  ////////////

        #ifdef PRINTOUT
            printf("FINAL BOARD STATE: \n");
            p2.print();
            p1.print();
            if(p1.get_num_minions() == 0 && p2.get_num_minions() == 0)
            {
                ties++;
                printf("Draw\n");
            }
            else if(p2.get_num_minions() == 0) 
            {
                p1_wins++;
                p1_damage[p1.get_damage(1)]++;
                printf("P1 wins!  P2 takes %d damage\n", p1.get_damage(5));
            }
            else 
            {
                p2_wins++;
                p2_damage[p2.get_damage(1)]++;
                printf("P2 wins!  P1 takes %d damage\n", p2.get_damage(5));
            }
        #else
            if(p1.get_num_minions() == 0 && p2.get_num_minions() == 0)
            {
                ties++;
            }
            else if(p2.get_num_minions() == 0) 
            {
                p1_wins++;
                p1_damage[p1.get_damage(1)]++;
            }
            else 
            {
                p2_wins++;
                p2_damage[p2.get_damage(1)]++;
            }
        #endif
    }

    printf("End of simulation\n");

    // Percentages
    float tie_percent = ((float) ties) / SIMULATIONS * 100;
    float p1_percent = ((float) p1_wins) / SIMULATIONS * 100;
    float p2_percent = ((float) p2_wins) / SIMULATIONS * 100;

    // Damage averages setup
    float p1_avg_damage = 0;
    float p2_avg_damage = 0;

    // Damage quartiles setup
    unsigned int p1_loop_total = 0;
    unsigned int p2_loop_total = 0;
    char p1_first_quartile = -1;
    char p1_third_quartile = -1;
    char p2_first_quartile = -1;
    char p2_third_quartile = -1;

    // Loop through damage tallies to calc averages and quartiles
    for(unsigned char i = 0; i < MAX_DAMAGE; i++)
    {
        p1_avg_damage += i * p1_damage[i];
        p2_avg_damage += i * p2_damage[i];

        // Definetly a better way out there to calculate quartiles...
        // This shouldn't matter too much since this loop barely runs
        p1_loop_total += p1_damage[i];
        p2_loop_total += p2_damage[i];
        if(p1_first_quartile == -1 && p1_loop_total > p1_wins / 4)
            p1_first_quartile = i;
        if(p2_first_quartile == -1 && p2_loop_total > p2_wins / 4)
            p2_first_quartile = i;
        if(p1_third_quartile == -1 && p1_loop_total > (p1_wins / 4) * 3)
            p1_third_quartile = i;
        if(p2_third_quartile == -1 && p2_loop_total > (p2_wins / 4) * 3)
            p2_third_quartile = i;
    }

    // Avoid div by 0 for averages
    if(p1_wins)
        p1_avg_damage /= p1_wins;
    else
        p1_avg_damage = 0;
    if(p2_wins)
        p2_avg_damage /= p2_wins;
    else 
        p2_avg_damage = 0;

    // Statistical results printout
    printf("Ties: %d, P1 Wins: %d, P2 Wins: %d\n", ties, p1_wins, p2_wins);
    printf("Percentages:  Tie [%4.3f] P1 Wins [%4.3f] P2 Wins [%4.3f]\n", tie_percent, p1_percent, p2_percent);
    printf("P1 avg damage upon win: [%4.3f]  P2 avg damage upon win: [%4.3f]\n", p1_avg_damage, p2_avg_damage);
    printf("P1 damage quartiles [%d - %d]  P2 damage quartiles [%d - %d]\n",
            p1_first_quartile, p1_third_quartile, p2_first_quartile, p2_third_quartile);
    return 0;
}