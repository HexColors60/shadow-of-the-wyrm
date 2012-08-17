#pragma once
#include "Weapon.hpp"

class Ammunition : public RangedWeapon
{
  public:
    Ammunition();
    ~Ammunition();
    
    virtual EquipmentWornLocation get_worn_location() const;
    
    // TODO: Add a function to let me figure out if the ammunition works with the ranged weapon...probbaly put this in
    // MeleeWeapon, and delete the Ammunition class.
    
    Item* clone();
    
  protected:
    // Whether the ammunition requires a ranged weapon.  Some (arrows, bolts) do.
    // Others (clubs, rocks) don't.
    bool requires_ranged_weapon;
};

typedef boost::shared_ptr<Ammunition> AmmunitionPtr;
