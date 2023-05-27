
#include "minion.h"

// Function to CREATE BEASTS RAWR
// Returns nulltpr on failure
Minion* beast_factory(std::string _name);

// Manasaber functions
void manasaber_deathrattle(Board *ally, Board* enemy, Minion *attacker, Minion* target);

// Scavenging Hyena
void scavenging_hyena_effect(Board *ally, Board* enemy, Minion *attacker, Minion* target);

// Humming Bird
void humming_bird_effect(Board *ally, Board* enemy, Minion *attacker, Minion* target);

// Leapfrogger 0__o
void leapfrogger_deathrattle(Board *ally, Board* enemy, Minion *attacker, Minion* target);

// Sewer Rat
void sewer_rat_deathrattle(Board *ally, Board* enemy, Minion *attacker, Minion* target);

// Banana Slamma
void bannana_slamma_effect(Board *ally, Board* enemy, Minion *attacker, Minion* target);

// Rat Pack
void rat_pack_deathrattle(Board *ally, Board* enemy, Minion *attacker, Minion* target);

// Rylak Metalhead
void rylack_metalhead_deathrattle(Board *ally, Board* enemy, Minion *attacker, Minion* target);

// Reanimating Rattler
// Does nothing in combat -- no functions

// Sly Raptor
void sly_raptor_deathrattle(Board *ally, Board* enemy, Minion *attacker, Minion* target);

// Bonemare
// Does nothing in combat -- no functions

// Mama bear
void mama_bear_effect(Board *ally, Board* enemy, Minion *attacker, Minion* target);

// Palescale Crocolisk
void palescale_effect(Board *ally, Board* enemy, Minion *attacker, Minion* target);
void palescale_deathrattle(Board *ally, Board* enemy, Minion *attacker, Minion* target);

// Sinrunner Blanchy
void blanchy_effect(Board *ally, Board* enemy, Minion *attacker, Minion* target);

// Felstomper
void felstomper_effect(Board *ally, Board* enemy, Minion *attacker, Minion* target);

// Ghastcoiler
void ghastcoiler_effect(Board *ally, Board* enemy, Minion *attacker, Minion* target);

// Octosari, Wrap God
void octosari_deathrattle(Board *ally, Board* enemy, Minion *attacker, Minion* target);