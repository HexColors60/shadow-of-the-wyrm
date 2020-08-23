#include "AttackNPCMagicDecision.hpp"
#include "Commands.hpp"
#include "CreatureUtils.hpp"
#include "NPCUseEquipItemDecisionStrategy.hpp"
#include "Spellbook.hpp"
#include "SpellShapeFactory.hpp"
#include "Wand.hpp"
#include "WeaponManager.hpp"

CommandPtr NPCUseEquipItemDecisionStrategy::decide(CreaturePtr creature, MapPtr map)
{
  CommandPtr use_cmd;

  if (creature != nullptr)
  {
    const std::list<ItemPtr>& items = creature->get_inventory()->get_items_ref();

    for (auto item : items)
    {
      if (item != nullptr)
      {
        ItemType itype = item->get_type();

        if (itype == ItemType::ITEM_TYPE_WEAPON)
        {
          use_cmd = get_equip_weapon_decision(creature, item);
        }
        else if (itype == ItemType::ITEM_TYPE_RING)
        {
          use_cmd = get_equip_ring_decision(creature, item);
        }
        else if (itype == ItemType::ITEM_TYPE_AMULET)
        {
          use_cmd = get_equip_amulet_decision(creature, item);
        }
        else if (itype == ItemType::ITEM_TYPE_AMMUNITION)
        {
          use_cmd = get_equip_ammunition_decision(creature, item);
        }
        else if (itype == ItemType::ITEM_TYPE_WAND)
        {
          MapPtr view_map = creature->get_decision_strategy()->get_fov_map();
          use_cmd = get_use_wand_decision(creature, item, view_map);
        }
        else if (itype == ItemType::ITEM_TYPE_SPELLBOOK)
        {
          use_cmd = get_use_book_decision(creature, item);
        }

        if (use_cmd != nullptr)
        {
          return use_cmd;
        }
      }
    }
  }

  return use_cmd;
}

CommandPtr NPCUseEquipItemDecisionStrategy::get_equip_weapon_decision(CreaturePtr creature, ItemPtr item)
{
  CommandPtr equip_cmd;

  if (creature != nullptr && item != nullptr && should_equip_weapon(creature, item))
  {
    equip_cmd = std::make_unique<InventoryCommand>(EquipmentWornLocation::EQUIPMENT_WORN_WIELDED, item);
  }

  return equip_cmd;
}

bool NPCUseEquipItemDecisionStrategy::should_equip_weapon(CreaturePtr creature, ItemPtr item)
{
  bool should_eq = false;

  WeaponManager wm;
  WeaponPtr weapon = std::dynamic_pointer_cast<Weapon>(item);
  WeaponPtr eq_weapon = wm.get_weapon(creature, AttackType::ATTACK_TYPE_MELEE_PRIMARY);
  Damage d = wm.get_damage(creature, AttackType::ATTACK_TYPE_MELEE_PRIMARY);

  if ((eq_weapon == nullptr && weapon != nullptr && (weapon->get_damage().avg() > d.avg())) ||
      (eq_weapon != nullptr && weapon != nullptr && weapon->get_status() != ItemStatus::ITEM_STATUS_CURSED && (weapon->get_damage().avg() > eq_weapon->get_damage().avg())))
  {   
    should_eq = true;
  }

  return should_eq;
}

CommandPtr NPCUseEquipItemDecisionStrategy::get_equip_ring_decision(CreaturePtr creature, ItemPtr item)
{
  CommandPtr equip_cmd;

  if (creature != nullptr && item != nullptr)
  {
    Equipment& eq = creature->get_equipment();

    EquipmentWornLocation f1 = EquipmentWornLocation::EQUIPMENT_WORN_LEFT_FINGER;
    EquipmentWornLocation f2 = EquipmentWornLocation::EQUIPMENT_WORN_RIGHT_FINGER;
    EquipmentWornLocation ewl = item->get_worn_location();

    if (ewl == f1 || ewl == f2)
    {
      bool f1_worn = eq.has_item(f1);
      bool f2_worn = eq.has_item(f2);

      if (!f1_worn)
      {
        equip_cmd = std::make_unique<InventoryCommand>(f1, item);
      }
      else if (!f2_worn)
      {
        equip_cmd = std::make_unique<InventoryCommand>(f2, item);
      }
    }
  }

  return equip_cmd;
}

CommandPtr NPCUseEquipItemDecisionStrategy::get_equip_amulet_decision(CreaturePtr creature, ItemPtr item)
{
  CommandPtr equip_cmd;

  if (creature != nullptr && item != nullptr)
  {
    EquipmentWornLocation ewl = item->get_worn_location();
    EquipmentWornLocation worn_loc = EquipmentWornLocation::EQUIPMENT_WORN_NECK;

    if (ewl == worn_loc && !creature->get_equipment().has_item(worn_loc))
    {
      equip_cmd = std::make_unique<InventoryCommand>(worn_loc, item);
    }
  }

  return equip_cmd;
}

CommandPtr NPCUseEquipItemDecisionStrategy::get_equip_ammunition_decision(CreaturePtr creature, ItemPtr item)
{
  CommandPtr equip_cmd;

  if (creature != nullptr && item != nullptr)
  {
    WeaponPtr ranged = std::dynamic_pointer_cast<Weapon>(creature->get_equipment().get_item(EquipmentWornLocation::EQUIPMENT_WORN_RANGED_WEAPON));
    ItemPtr ammo_item = creature->get_equipment().get_item(EquipmentWornLocation::EQUIPMENT_WORN_AMMUNITION);

    WeaponPtr ammo = std::dynamic_pointer_cast<Weapon>(item);
    WeaponManager wm;

    if (ammo_item == nullptr && wm.is_ranged_weapon_skill_type_compatible_with_ammunition(ranged, ammo))
    {
      equip_cmd = std::make_unique<InventoryCommand>(EquipmentWornLocation::EQUIPMENT_WORN_AMMUNITION, item);
    }
  }

  return equip_cmd;
}

CommandPtr NPCUseEquipItemDecisionStrategy::get_use_wand_decision(CreaturePtr creature, ItemPtr item, MapPtr view_map)
{
  CommandPtr use_cmd;
  WandPtr wand = std::dynamic_pointer_cast<Wand>(item);

  if (wand != nullptr && wand->get_charges().get_current() > 0 && wand->get_has_damage())
  {
    uint range = wand->get_range();
    AttackNPCMagicDecision anmd;
    Spell spell;
    SpellShape ss = SpellShapeFactory::create_spell_shape(wand->get_spell_shape_type(), wand->get_radius());

    spell.set_shape(ss);
    spell.set_damage(wand->get_damage());
    spell.set_range(wand->get_range());

    // If there are hostiles in range no non-hostiles in the same,
    // use the wand.
    std::pair<bool, Direction> dec_details = anmd.decide(creature, view_map, spell, creature->get_decision_strategy()->get_threats_ref().get_true_threats_without_level());

    if (dec_details.first)
    {
      use_cmd = std::make_unique<EvokeCommand>(wand->get_id(), dec_details.second);
    }
  }

  return use_cmd;
}

CommandPtr NPCUseEquipItemDecisionStrategy::get_use_book_decision(CreaturePtr creature, ItemPtr item)
{
  CommandPtr use_cmd;
  SpellbookPtr book = std::dynamic_pointer_cast<Spellbook>(item);

  if (book != nullptr && CreatureUtils::has_skill_for_spell(creature, book->get_spell_id()))
  {
    use_cmd = std::make_unique<ReadCommand>(book->get_id());
  }

  return use_cmd;
}