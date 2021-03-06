#include "ResistanceTextKeys.hpp"
#include "StringTable.hpp"

using namespace std;

// Resistances
ResistanceTextKeys::ResistanceTextKeys()
{
}

ResistanceTextKeys::~ResistanceTextKeys()
{
}

string ResistanceTextKeys::get_resistance_for_damage_type(const DamageType dt)
{
  string res;

  switch (dt)
  {
    case DamageType::DAMAGE_TYPE_SLASH:
      res = RESISTANCE_SLASH;
      break;
    case DamageType::DAMAGE_TYPE_PIERCE:
      res = RESISTANCE_PIERCE;
      break;
    case DamageType::DAMAGE_TYPE_POUND:
      res = RESISTANCE_POUND;
      break;
    case DamageType::DAMAGE_TYPE_HEAT:
      res = RESISTANCE_HEAT;
      break;
    case DamageType::DAMAGE_TYPE_COLD:
      res = RESISTANCE_COLD;
      break;
    case DamageType::DAMAGE_TYPE_ACID:
      res = RESISTANCE_ACID;
      break;
    case DamageType::DAMAGE_TYPE_POISON:
      res = RESISTANCE_POISON;
      break;
    case DamageType::DAMAGE_TYPE_HOLY:
      res = RESISTANCE_HOLY;
      break;
    case DamageType::DAMAGE_TYPE_SHADOW:
      res = RESISTANCE_SHADOW;
      break;
    case DamageType::DAMAGE_TYPE_ARCANE:
      res = RESISTANCE_ARCANE;
      break;
    case DamageType::DAMAGE_TYPE_LIGHTNING:
      res = RESISTANCE_LIGHTNING;
      break;
    case DamageType::DAMAGE_TYPE_NULL:
    default:
      break;
  }

  res = StringTable::get(res);
  return res;
}

const string ResistanceTextKeys::RESISTANCE_RESISTANCES    = "RESISTANCE_RESISTANCES";
const string ResistanceTextKeys::RESISTANCE_SLASH          = "RESISTANCE_SLASH";
const string ResistanceTextKeys::RESISTANCE_ABRV_SLASH     = "RESISTANCE_ABRV_SLASH";
const string ResistanceTextKeys::RESISTANCE_GAIN_SLASH     = "RESISTANCE_GAIN_SLASH";
const string ResistanceTextKeys::RESISTANCE_LOSE_SLASH     = "RESISTANCE_LOSE_SLASH";
const string ResistanceTextKeys::RESISTANCE_POUND          = "RESISTANCE_POUND";
const string ResistanceTextKeys::RESISTANCE_ABRV_POUND     = "RESISTANCE_ABRV_POUND";
const string ResistanceTextKeys::RESISTANCE_GAIN_POUND     = "RESISTANCE_GAIN_POUND";
const string ResistanceTextKeys::RESISTANCE_LOSE_POUND     = "RESISTANCE_LOSE_POUND";
const string ResistanceTextKeys::RESISTANCE_PIERCE         = "RESISTANCE_PIERCE";
const string ResistanceTextKeys::RESISTANCE_ABRV_PIERCE    = "RESISTANCE_ABRV_PIERCE";
const string ResistanceTextKeys::RESISTANCE_GAIN_PIERCE    = "RESISTANCE_GAIN_PIERCE";
const string ResistanceTextKeys::RESISTANCE_LOSE_PIERCE    = "RESISTANCE_LOSE_PIERCE";
const string ResistanceTextKeys::RESISTANCE_HEAT           = "RESISTANCE_HEAT";
const string ResistanceTextKeys::RESISTANCE_ABRV_HEAT      = "RESISTANCE_ABRV_HEAT";
const string ResistanceTextKeys::RESISTANCE_GAIN_HEAT      = "RESISTANCE_GAIN_HEAT";
const string ResistanceTextKeys::RESISTANCE_LOSE_HEAT      = "RESISTANCE_LOSE_HEAT";
const string ResistanceTextKeys::RESISTANCE_COLD           = "RESISTANCE_COLD";
const string ResistanceTextKeys::RESISTANCE_ABRV_COLD      = "RESISTANCE_ABRV_COLD";
const string ResistanceTextKeys::RESISTANCE_GAIN_COLD      = "RESISTANCE_GAIN_COLD";
const string ResistanceTextKeys::RESISTANCE_LOSE_COLD      = "RESISTANCE_LOSE_COLD";
const string ResistanceTextKeys::RESISTANCE_ACID           = "RESISTANCE_ACID";
const string ResistanceTextKeys::RESISTANCE_ABRV_ACID      = "RESISTANCE_ABRV_ACID";
const string ResistanceTextKeys::RESISTANCE_GAIN_ACID      = "RESISTANCE_GAIN_ACID";
const string ResistanceTextKeys::RESISTANCE_LOSE_ACID      = "RESISTANCE_LOSE_ACID";
const string ResistanceTextKeys::RESISTANCE_POISON         = "RESISTANCE_POISON";
const string ResistanceTextKeys::RESISTANCE_ABRV_POISON    = "RESISTANCE_ABRV_POISON";
const string ResistanceTextKeys::RESISTANCE_GAIN_POISON    = "RESISTANCE_GAIN_POISON";
const string ResistanceTextKeys::RESISTANCE_LOSE_POISON    = "RESISTANCE_LOSE_POISON";
const string ResistanceTextKeys::RESISTANCE_HOLY           = "RESISTANCE_HOLY";
const string ResistanceTextKeys::RESISTANCE_ABRV_HOLY      = "RESISTANCE_ABRV_HOLY";
const string ResistanceTextKeys::RESISTANCE_GAIN_HOLY      = "RESISTANCE_GAIN_HOLY";
const string ResistanceTextKeys::RESISTANCE_LOSE_HOLY      = "RESISTANCE_LOSE_HOLY";
const string ResistanceTextKeys::RESISTANCE_SHADOW         = "RESISTANCE_SHADOW";
const string ResistanceTextKeys::RESISTANCE_ABRV_SHADOW    = "RESISTANCE_ABRV_SHADOW";
const string ResistanceTextKeys::RESISTANCE_GAIN_SHADOW    = "RESISTANCE_GAIN_SHADOW";
const string ResistanceTextKeys::RESISTANCE_LOSE_SHADOW    = "RESISTANCE_LOSE_SHADOW";
const string ResistanceTextKeys::RESISTANCE_ARCANE         = "RESISTANCE_ARCANE";
const string ResistanceTextKeys::RESISTANCE_ABRV_ARCANE    = "RESISTANCE_ABRV_ARCANE";
const string ResistanceTextKeys::RESISTANCE_GAIN_ARCANE    = "RESISTANCE_GAIN_ARCANE";
const string ResistanceTextKeys::RESISTANCE_LOSE_ARCANE    = "RESISTANCE_LOSE_ARCANE";
const string ResistanceTextKeys::RESISTANCE_LIGHTNING      = "RESISTANCE_LIGHTNING";
const string ResistanceTextKeys::RESISTANCE_ABRV_LIGHTNING = "RESISTANCE_ABRV_LIGHTNING";
const string ResistanceTextKeys::RESISTANCE_GAIN_LIGHTNING = "RESISTANCE_GAIN_LIGHTNING";
const string ResistanceTextKeys::RESISTANCE_LOSE_LIGHTNING = "RESISTANCE_LOSE_LIGHTNING";
const string ResistanceTextKeys::RESISTANCES_EXPLANATION   = "RESISTANCES_EXPLANATION";
