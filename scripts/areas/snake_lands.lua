require('areas')

local snake_lands = {}

-- Sithrin is the ancestral home of the snakelings.  Its inhabitants are 
-- always hostile, but it also contains the key to Zaeda's tower.
local sithrin = Area:new(3, 94)
sithrin:set_custom_map_id("sithrin")
sithrin:set_extra_description_sid("TILE_EXTRA_DESCRIPTION_SITHRIN")

table.insert(snake_lands, sithrin)

-- Zaeda's Tower, recently emerging from its shroud of illusion, sits on
-- the southern tip of the Snake Lands, the only dry region of the
-- island.
local zaedas_tower = Area:new(5, 95)
zaedas_tower:set_custom_map_id("zaedas_tower")
zaedas_tower:set_extra_description_sid("TILE_EXTRA_DESCRIPTION_ZAEDAS_TOWER")

table.insert(snake_lands, zaedas_tower)

for k,v in pairs(snake_lands) do
  v:insert()
end

