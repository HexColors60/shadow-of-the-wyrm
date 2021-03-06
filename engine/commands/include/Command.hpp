#pragma once
#include <map>
#include <string>
#include <memory>

class Command
{
  public:
    virtual ~Command();

    std::string get_name() const;

    virtual int get_key() const;

    virtual void set_custom_value(const std::string& key, const std::string& value);
    virtual std::string get_custom_value(const std::string& key) const;

    // Most commands can just check the existence of a command_confirmation
    // string, but some will need special logic.
    virtual bool requires_confirmation() const;
    std::string get_confirmation_sid() const;

    // Most commands will allow the message buffer to be cleared, but for some
    // (e.g., automatic movement engaged) we need to keep the text in place.
    virtual bool get_allow_clear_message_buffer() const;

  protected:
    friend class CommandFactory;
    Command(const std::string& name, int key);
    Command(const std::string& name, int key, const std::string& confirmation_text);

    std::string command_name;
    int key_pressed;
    std::string command_confirmation;
    std::map<std::string, std::string> custom_values;
};

using CommandPtr = std::unique_ptr<Command>;
