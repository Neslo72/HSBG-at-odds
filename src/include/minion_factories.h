#ifndef _MINION_FACTORIES_H_
#define _MINION_FACTORIES_H_

#include "minion.h"
#include "board.h"

// Neutral creator
Minion neutral_factory(std::string _name);

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

#endif