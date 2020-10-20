/*
 * This file is a part of the TChecker project.
 *
 * See files AUTHORS and LICENSE for copyright details.
 *
 */

#ifndef TCHECKER_PROPERTIES_HH
# define TCHECKER_PROPERTIES_HH

# include "tchecker/basictypes.hh"
# include "tchecker/utils/array.hh"
# include "tchecker/utils/index.hh"
# include "tchecker/utils/iterator.hh"

namespace tchecker {

    /*!
     \class property_index_t
     \brief Index of property names
     */
    class property_index_t
        : public tchecker::autokey_index_t<tchecker::property_id_t, std::string> {
     public:
      using tchecker::autokey_index_t<tchecker::property_id_t, std::string>::autokey_index_t;
    };

    class properties_t;

    class property_t {
     private:
      friend class properties_t;

      property_t (property_id_t id, std::string name, std::string kind, std::string formula)
      : id_(id), name_(std::move(name)), kind_(std::move(kind)), formula_(std::move(formula))
      {}

     public:
      property_t (property_t const &property) = default;
      property_t (property_t &&property) = default;
      ~property_t () = default;

      tchecker::property_t & operator= (tchecker::property_t const &) = default;


      tchecker::property_t & operator= (tchecker::property_t &&) = default;

      inline property_id_t id() const {
        return id_;
      }

      inline const std::string &name () const
      {
        return name_;
      }

      inline const std::string &kind () const
      {
        return kind_;
      }

      inline const std::string &formula () const
      {
        return formula_;
      }

     protected:
      property_id_t id_;
      std::string name_;
      std::string kind_;
      std::string formula_;
    };


    class properties_t {
     public:
      properties_t() : index_(), properties_() {}
      properties_t(properties_t const &props) = default;
      properties_t(properties_t &&props) = default;
      ~properties_t() = default;

      tchecker::properties_t &operator=(properties_t const &) = default;
      tchecker::properties_t &operator=(properties_t &&) = default;

      inline bool has_property (std::string const &name) const {
        return get(name) != nullptr;
      }

      /*
       * \pre ! (has_property (name) || name.empty ())
       * \pre ! kind.empty()
       * \pre ! formula.empty())
       * \throw std::invalid_argument if a pre-condition is not satisfied
       */
      void add (std::string name, std::string kind, std::string formula);


      property_t const *get (std::string const &name) const ;

      property_t const *get (property_id_t id) const ;

      inline property_index_t const &index() const {
        return index_;
      }

      inline property_id_t size() const {
        return static_cast<tchecker::property_id_t >(index_.size());
      }

      using const_property_iterator_t = std::vector<property_t>::const_iterator;
      inline tchecker::range_t<const_property_iterator_t> properties() const {
        return tchecker::make_range(properties_.cbegin(), properties_.cend());
      }

     private:
      property_index_t index_;
      std::vector<property_t> properties_;
    };
}

#endif // TCHECKER_PROPERTIES_HH
