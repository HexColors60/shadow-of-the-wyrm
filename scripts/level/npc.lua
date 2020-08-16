require('level')

-- NPCs get an extra d6 HP and AP per level, and get those stats restored.
local function npc_gain_hp_ap(creature_id)
  local lvl_hp = RNG_range(1,6)
  local lvl_ap = RNG_range(1,6)

  local base_hp = get_creature_base_hp(creature_id)
  local base_ap = get_creature_base_ap(creature_id)

  local cur_hp = get_creature_current_hp(creature_id)
  local cur_ap = get_creature_current_ap(creature_id)

  -- NPCs get their HP and AP restored when they level.
  -- Life's not fair.
  base_hp = base_hp + lvl_hp
  base_ap = base_ap + lvl_ap

  set_creature_base_hp(creature_id, base_hp)
  set_creature_current_hp(creature_id, base_hp)
  set_creature_base_ap(creature_id, base_ap)
  set_creature_current_ap(creature_id, base_ap)
end

-- Creatures get an extra d2 damage per level
local function npc_gain_damage(creature_id)
  local ndice, dsides, modifier = get_creature_base_damage(creature_id)
  dsides = dsides + 2
  set_creature_base_damage(creature_id, ndice, dsides, modifier)
end

local function npc_level_gain_fn(creature_id, level)
  if level > 1 then
    npc_gain_hp_ap(creature_id)
    npc_gain_damage(creature_id)
  
    add_npc_level_message(creature_id)
  end
end

local npc_lvl_fn = npc_level_gain_fn

level.set_misc_gain_level_fn("_npc", npc_lvl_fn)
