#pragma once

#include <vector>
#include "common.hpp"
#include "Map.hpp"

class Dimensions;

class Generator
{  
  #ifdef UNIT_TESTS
  friend class GeneratorTest;
  #endif

  public:
   Generator(const std::string& new_map_exit_id, TileType new_map_terrain_type);

   // Generate the map and its contents (creatures, features, items, etc.)
   virtual MapPtr generate_and_initialize(const uint danger_level);
   virtual MapPtr generate_and_initialize(const uint danger_level, const Dimensions& dim);
   
   virtual MapPtr generate();
   virtual MapPtr generate(const Dimensions& dim) = 0;
   
   virtual void set_terrain_type(const TileType new_map_terrain_type);
   virtual TileType get_terrain_type() const;
   
  protected:
    virtual void initialize(MapPtr map, const uint danger_level);
    virtual void fill(const MapPtr map, const TileType& tile_type);
    
    // Generate the creatures.  Returns true if creatures were created, false otherwise.
    virtual bool generate_creatures(MapPtr map, const uint danger_level);
    virtual bool update_creatures(MapPtr map, const uint danger_level);
    
    // Seed the initial items.  Returns true if the items were created, false otherwise.
    virtual bool generate_initial_items(MapPtr map, const uint danger_level);
    virtual bool update_items(MapPtr map, const uint danger_level);
    
    std::string map_exit_id;
    TileType map_terrain_type;
};

typedef boost::shared_ptr<Generator> GeneratorPtr;
