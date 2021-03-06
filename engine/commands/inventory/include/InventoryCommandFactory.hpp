#pragma once
#include "CommandFactory.hpp"

class InventoryCommandFactory : public CommandFactory
{
  public:
    InventoryCommandFactory();
    virtual ~InventoryCommandFactory();

    virtual CommandPtr create(const int key, const std::string& command) override;
    virtual CommandFactoryType get_factory_type() const override;

    virtual CommandFactory* clone() override;

  private:
    ClassIdentifier internal_class_identifier() const override;
};
