/*
  Brainlearn, a UCI chess playing engine derived from Stockfish
  Copyright (C) 2004-2024 The Brainlearn developers (see AUTHORS file)

  Brainlearn is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Brainlearn is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef UCIOPTION_H_INCLUDED
#define UCIOPTION_H_INCLUDED

#include <cstddef>
#include <functional>
#include <iosfwd>
#include <map>
#include <string>

namespace Brainlearn {
// Define a custom comparator, because the UCI options should be case-insensitive
struct CaseInsensitiveLess {
    bool operator()(const std::string&, const std::string&) const;
};

class Option;

class OptionsMap {
   public:
    void setoption(std::istringstream&);

    friend std::ostream& operator<<(std::ostream&, const OptionsMap&);

    Option  operator[](const std::string&) const;
    Option& operator[](const std::string&);

    std::size_t count(const std::string&) const;

   private:
    // The options container is defined as a std::map
    using OptionsStore = std::map<std::string, Option, CaseInsensitiveLess>;

    OptionsStore options_map;
};

// The Option class implements each option as specified by the UCI protocol
class Option {
   public:
    using OnChange = std::function<void(const Option&)>;

    Option(OnChange = nullptr);
    Option(bool v, OnChange = nullptr);
    Option(const char* v, OnChange = nullptr);
    Option(double v, int minv, int maxv, OnChange = nullptr);
    Option(const char* v, const char* cur, OnChange = nullptr);

    Option& operator=(const std::string&);
    void    operator<<(const Option&);
    operator int() const;
    operator std::string() const;
    bool operator==(const char*) const;

    friend std::ostream& operator<<(std::ostream&, const OptionsMap&);

   private:
    std::string defaultValue, currentValue, type;
    int         min, max;
    size_t      idx;
    OnChange    on_change;
};

}
#endif  // #ifndef UCIOPTION_H_INCLUDED
