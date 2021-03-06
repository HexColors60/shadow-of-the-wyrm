#pragma once

// Used for player/monster movements, etc.
enum struct Direction
{
  DIRECTION_SOUTH_WEST = 1,
  DIRECTION_SOUTH = 2,
  DIRECTION_SOUTH_EAST = 3,
  DIRECTION_WEST = 4,
  DIRECTION_NULL = 5, /* Don't move */
  DIRECTION_EAST = 6,
  DIRECTION_NORTH_WEST = 7,
  DIRECTION_NORTH = 8,
  DIRECTION_NORTH_EAST = 9,
  DIRECTION_UP = 10,
  DIRECTION_DOWN = 11
};

#define CDIRECTION_SOUTH_WEST 1
#define CDIRECTION_SOUTH 2
#define CDIRECTION_SOUTH_EAST 3
#define CDIRECTION_WEST 4
#define CDIRECTION_NULL 5
#define CDIRECTION_EAST 6
#define CDIRECTION_NORTH_WEST 7
#define CDIRECTION_NORTH 8
#define CDIRECTION_NORTH_EAST 9
#define CDIRECTION_UP 10
#define CDIRECTION_DOWN 11

// More general - used in terrain generation, etc.
enum struct CardinalDirection
{
  CARDINAL_DIRECTION_NULL = -1,
  CARDINAL_DIRECTION_NORTH = 0,
  CARDINAL_DIRECTION_EAST  = 1,
  CARDINAL_DIRECTION_SOUTH = 2,
  CARDINAL_DIRECTION_WEST  = 3
};

// Used for things like scything attack, where it
// follows a rotation around some centre.
enum struct RotationDirection
{
  ROTATION_DIRECTION_NULL = -1,
  ROTATION_DIRECTION_CLOCKWISE = 0,
  ROTATION_DIRECTION_COUNTERCLOCKWISE = 1
};

enum struct DirectionCategory
{
  DIRECTION_CATEGORY_NONE = 0,
  DIRECTION_CATEGORY_CARDINAL = 1,
  DIRECTION_CATEGORY_CARDINALORDINAL = 2
};
