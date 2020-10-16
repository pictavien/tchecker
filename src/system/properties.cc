/*
 * This file is a part of the TChecker project.
 *
 * See files AUTHORS and LICENSE for copyright details.
 *
 */
#include <cassert>
#include "tchecker/system/properties.hh"

namespace tchecker {
    void
    properties_t::add (std::string name, std::string kind, std::string formula)
    {
      if (has_property (name) || name.empty ())
        throw std::invalid_argument("invalid property name '" + name + "'");
      if (kind.empty())
        throw std::invalid_argument("empty property kind");
      if (formula.empty())
        throw std::invalid_argument("empty property formula");

      property_id_t id = index_.add(name);
      property_t newprop (id, name, kind, formula);

      properties_.push_back(newprop);
    }

    property_t const *
    properties_t::get (std::string const &name) const
    {
      property_t const *result = nullptr;
      try
        {
          property_id_t id = index_.key(name);
          return &(properties_[id]);
        }
      catch (std::invalid_argument &)
        {}
      return result;
    }

    property_t const *
    properties_t::get (property_id_t id) const
    {
      assert(properties_.size() == index_.size());
      if (id < properties_.size())
        return &(properties_[id]);
      return nullptr;
    }
}
