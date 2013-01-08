#pragma once
#include "Generator.hpp"
#include "Map.hpp"
#include "MapUtils.hpp"

class CavernGenerator : public Generator
{
  public:
    CavernGenerator(const std::string& map_exit_id);
    
    virtual MapPtr generate(const Dimensions& dimensions);

  protected:
    virtual bool get_permanence_default() const;
    virtual MapType get_map_type() const;
    void generate_cavern(MapPtr map);
    MapComponents get_cavern_components(MapPtr map);
    void connect_cavern_components(MapPtr map, const MapComponents& cc);
    void connect_caverns(MapPtr map, const Coordinate& first, const Coordinate& second);
    void reset_cavern_edges(MapPtr map);
    void generate_staircases(MapPtr map);
    void generate_staircase(MapPtr map, const TileType tile_type, const Direction direction);
};
