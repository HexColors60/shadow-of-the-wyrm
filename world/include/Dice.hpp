#pragma once
#include <string>
#include "common.hpp"

namespace boost
{
  namespace serialization
  {
    class access;
  }
}

class Dice
{
  public:
    Dice();
    Dice(const uint new_num_dice, const uint new_dice_sides, const int modifier);
    virtual ~Dice();

    int max() const;
    
    void set_num_dice(const uint new_num_dice);
    uint get_num_dice() const;

    void set_dice_sides(const uint new_dice_sides);
    uint get_dice_sides() const;

    void set_modifier(const int new_modifier);
    int get_modifier() const;

    // Convert to a string (for UI use, etc)
    std::string str() const;

  protected:
    uint num_dice;
    uint dice_sides;
    int modifier;
    
  private:
    friend class boost::serialization::access;
    
    template<typename Archive>
    void serialize(Archive& ar, const unsigned int version)
    {
      ar & num_dice & dice_sides & modifier;
    }
};
