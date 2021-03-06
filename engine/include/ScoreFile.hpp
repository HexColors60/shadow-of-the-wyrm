#pragma once
#include <fstream>
#include <iostream>
#include "Creature.hpp"
#include "ScoreFileEntry.hpp"

// RAII class representing the high score file.  When constructed, the
// score file will be read (or an exception will be thrown) into a
// data structure representing the file.  The write operation can be
// used to attempt to write a creature to the internal data
// structure - the function will return true if it was successful, false
// otherwise.
class ScoreFile
{
  public:
    ScoreFile();
    ScoreFile(const bool read_score_file);

    bool write(CreaturePtr creature);
    std::vector<ScoreFileEntry> get_entries() const;

    bool is_full() const;
    void save();

  protected:
    void read_score_file();

    bool version_ok(std::istream& stream);
    bool read_file(std::istream& stream);

    std::vector<ScoreFileEntry> entries;
};

