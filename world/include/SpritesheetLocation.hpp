#pragma once
#include <string>
#include "common.hpp"
#include "Colours.hpp"
#include "ISerializable.hpp"

class SpritesheetLocation : public ISerializable
{
  public:
    SpritesheetLocation();
    SpritesheetLocation(const std::string& new_index, const Coordinate& c);
    SpritesheetLocation(const std::string& new_index, const std::string& new_ref_id);

    bool operator==(const SpritesheetLocation& sl) const;

    void set_index(const std::string& new_index);
    std::string get_index() const;

    void set_reference_id(const std::string& new_reference_id);
    std::string get_reference_id() const;
    bool uses_reference_id() const;

    void set_coordinate(const Coordinate& c);
    Coordinate get_coordinate() const;

    bool serialize(std::ostream& stream) const override;
    bool deserialize(std::istream& stream) override;

  protected:
    std::string index;
    std::string reference_id;
    Coordinate coord;

  private:
    ClassIdentifier internal_class_identifier() const override;
};