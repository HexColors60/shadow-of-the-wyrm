#include "KeepTile.hpp"

TileType KeepTile::get_tile_type() const
{
  return TileType::TILE_TYPE_KEEP;
}

std::string KeepTile::get_tile_description_sid() const
{
  return TileTextKeys::TILE_DESC_KEEP;
}

Tile* KeepTile::clone()
{
  return new KeepTile(*this);
}

ClassIdentifier KeepTile::internal_class_identifier() const
{
  return ClassIdentifier::CLASS_ID_KEEP_TILE;
}

#ifdef UNIT_TESTS
#include "unit_tests/KeepTile_test.cpp"
#endif