#pragma once
#include "Damage.hpp"
#include "XMLDataStructures.hpp"

// Common functions useful to a number of different XMLReaders of different types (creatures, items, and so on)
// go here.
class XMLReader
{
  public:
    void parse_dice(Dice& dice, const XMLNode& dice_node);
    void parse_damage(Damage& damage, const XMLNode& damage_node);
};
