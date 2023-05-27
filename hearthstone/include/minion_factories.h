#include "minion.h"
#include "board.h"

// Standard deathrattle function
void standard_death(Board *ally, Board* enemy, Minion *attacker, Minion* target);

// Beast creator
Minion beast_factory(std::string _name);

// Murloc creator
Minion* murloc_factory(std::string _name);

// Mech creator
Minion* mech_factory(std::string _name);

// Demon creator
Minion* demon_factory(std::string _name);

// Dragon creator
Minion* dragon_factory(std::string _name);

// Elemental creator
Minion* elemental_factory(std::string _name);

// Pirate creator
Minion* pirate_factory(std::string _name);

// Quilboar creator
Minion* quilboar_factory(std::string _name);

// Naga creator
Minion* naga_factory(std::string _name);