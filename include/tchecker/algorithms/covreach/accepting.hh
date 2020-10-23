/*
 * This file is a part of the TChecker project.
 *
 * See files AUTHORS and LICENSE for copyright details.
 *
 */

#ifndef TCHECKER_ALGORITHMS_COVREACH_ACCEPTING_HH
#define TCHECKER_ALGORITHMS_COVREACH_ACCEPTING_HH

#include <string>

#include <boost/dynamic_bitset.hpp>

#include "tchecker/utils/log.hh"
#include "tchecker/basictypes.hh"
#include "tchecker/fsm/details/system.hh"
#include "tchecker/vm/vm.hh"
#include "tchecker/vm/compilers.hh"
#include "tchecker/parsing/parsing.hh"
#include "tchecker/expression/typechecking.hh"

/*!
 \file accepting.hh
 \brief Accepting conditions for covering reachability algorithm
 */

namespace tchecker {
  
  namespace covreach {

      /*!
       \brief Type of accepting condition
       */
      template <class NODE_PTR>
      class accepting_condition_t {
       public:
        accepting_condition_t() = default;
        virtual ~accepting_condition_t() = default;

        virtual bool operator() (NODE_PTR const &)= 0;
      };

      /*!
     \class accepting_labels_t
     \brief Accepting conditon based on labels
     \tparam NODE_PTR : type of pointer to node
     */
    template <class NODE_PTR>
    class accepting_labels_t : public accepting_condition_t<NODE_PTR> {
    public:
      /*!
       \brief Constructor
       \param index : maps label names to label identifiers
       \param accepting_labels : range of label names, should dereference to std::string
       */
      template <class LABELS_RANGE>
      accepting_labels_t(tchecker::label_index_t const & index, LABELS_RANGE const & accepting_labels)
      : _accepting_labels(index.size()), _node_labels(index.size())
      {
        for (std::string const & label : accepting_labels)
          _accepting_labels[index.key(label)] = 1;
      }
      
      /*!
       \brief Copy constructor
       */
      accepting_labels_t(tchecker::covreach::accepting_labels_t<NODE_PTR> const &) = default;
      
      /*!
       \brief Move constructor
       */
      accepting_labels_t(tchecker::covreach::accepting_labels_t<NODE_PTR> &&) = default;
      
      /*!
       \brief Destructor
       */
      ~accepting_labels_t() = default;
      
      /*!
       \brief Assignment operator
       */
      tchecker::covreach::accepting_labels_t<NODE_PTR> &
      operator= (tchecker::covreach::accepting_labels_t<NODE_PTR> const &) = default;
      
      /*!
       \brief Move-assignment operator
       */
      tchecker::covreach::accepting_labels_t<NODE_PTR> &
      operator= (tchecker::covreach::accepting_labels_t<NODE_PTR> &&) = default;
      
      /*!
       \brief Predicate
       \param node : a node
       \return true if node is accepting w.r.t. accepting labels, false otherwise
       */
      bool operator() (NODE_PTR const & node)
      {
        _node_labels.reset();
        for (auto const * loc : node->vloc())
          for (tchecker::label_id_t id : loc->labels())
            _node_labels[id] = 1;
        return _accepting_labels.is_subset_of(_node_labels);
      }
    private:
      boost::dynamic_bitset<> _accepting_labels;  /*!< Set of accepting labels */
      mutable boost::dynamic_bitset<> _node_labels;       /*!< Set of node labels */
    };

    template <class NODE_PTR, typename SYSTEM>
    class accepting_properties_t : public accepting_condition_t<NODE_PTR> {
    public:
      using system_t = SYSTEM;

      accepting_properties_t(system_t const &system, std::vector<tchecker::property_t> const &props, tchecker::log_t & log)
      : system_(system),
        vm_(system.intvars().flattened().size(), system.clocks().flattened().size()),
        log_(log),
        properties_(props),
        bytecodes_(),
        current_vloc (nullptr)
      {
        compile_atomic_properties();
      }

      /*!
       \brief Copy constructor
       */
      accepting_properties_t(tchecker::covreach::accepting_properties_t<NODE_PTR, SYSTEM> const &) = default;

      /*!
       \brief Move constructor
       */
      accepting_properties_t(tchecker::covreach::accepting_properties_t<NODE_PTR, SYSTEM> &&) = default;

      /*!
       \brief Destructor
       */
      ~accepting_properties_t() {
        for(tchecker::bytecode_t* bptr : bytecodes_)
          delete[] bptr;
      }

      /*!
       \brief Assignment operator
       */
      tchecker::covreach::accepting_properties_t<NODE_PTR, SYSTEM> &
      operator= (tchecker::covreach::accepting_properties_t<NODE_PTR, SYSTEM> const &) = default;

      /*!
       \brief Move-assignment operator
       */
      tchecker::covreach::accepting_properties_t<NODE_PTR, SYSTEM> &
      operator= (tchecker::covreach::accepting_properties_t<NODE_PTR, SYSTEM> &&) = default;

      /*!
       \brief Predicate
       \param node : a node
       \return true if node is accepting w.r.t. accepting labels, false otherwise
       */
      bool
      operator() (NODE_PTR const & node)
      {
        bool result = true;
        auto valuation = node->intvars_valuation_ptr();
        current_vloc = &node->vloc();

        for(int i = 0; i < bytecodes_.size(); i++) {
          tchecker::bytecode_t const *bytecode = bytecodes_[i];
          bool res = check_atomic_property (bytecode, properties_[i], *valuation);
          if (! res)
            result = false;
        }
        return result;
      }

    private:
      using node_t = typename NODE_PTR::shared_object_t;
      using vloc_t = typename node_t::state_t::vloc_t;

      system_t const &system_;
      tchecker::vm_t vm_;
      tchecker::log_t log_;
      std::vector<tchecker::property_t> properties_;
      std::vector<tchecker::bytecode_t *> bytecodes_;
      vloc_t const *current_vloc;

      using intvars_t = typename node_t::state_t::intvars_valuation_t;


      inline bool check_loc_id (process_id_t pid, loc_id_t lid) {
        return (*current_vloc)[pid]->id () == lid;
      }

      inline bool check_label_id (process_id_t pid, label_id_t lid) {
        for (tchecker::label_id_t label_id : (*current_vloc)[pid]->labels ())
          {
            if (lid == label_id)
              return true;
          }
        return false;
      }

      bool
      check_atomic_property (tchecker::bytecode_t const *bytecode,
                             property_t const &prop, intvars_t & intvars_val)
      {
        assert (bytecode != nullptr);
        assert (current_vloc != nullptr);
        try {
            return (bool)
            vm_.run (bytecode,
                     [=] (process_id_t pid, loc_id_t lid) { return check_loc_id(pid, lid); },
                     [=] (process_id_t pid, label_id_t lid) { return check_label_id(pid, lid); },
                     intvars_val,
                     clock_constraint_throw_container,
                     clock_reset_throw_container);
          }
        catch (std::exception const & e) {
            throw std::runtime_error(e.what()
                                     + (", in evaluation of " + prop.formula() + " from valuation "
                                        + tchecker::to_string(intvars_val,
                                                              system_.intvars().flattened().index())));
          }
      }

      void compile_atomic_properties()
      {
        assert ( bytecodes_.empty() );

        for(auto const &prop : properties_) {
            assert (prop.kind() == "atomic");
            std::string context = "property '" + prop.name () +"'";
            tchecker::bytecode_t *bcode = compile_atomic_property(prop.formula(), context);
            bytecodes_.push_back(bcode);
          }
        assert (bytecodes_.size() == properties_.size());
      }

      tchecker::loc_id_t find_loc(std::string pname, std::string lname) {
        return system_.location(pname, lname)->id();
      }

      tchecker::label_id_t find_label(std::string lname) {
        return system_.labels().key(lname);
      }

      tchecker::bytecode_t *
      compile_atomic_property(std::string atomic_prop, std::string context)
      {
        std::ostringstream oss;
        tchecker::log_t log (&oss);

        std::unique_ptr<tchecker::expression_t> expr (tchecker::parsing::parse_expression (context, atomic_prop, log));
        if (expr == nullptr)
          throw std::runtime_error (oss.str());

        tchecker::integer_variables_t localvars;
        std::unique_ptr<tchecker::typed_expression_t> texpr(
        tchecker::typecheck(*expr,
                            system_.processes(),
                            [=] (std::string pname, std::string lname) { return find_loc(pname, lname); },
                            [=] (std::string lname) { return find_label(lname); },
                            system_.events(),
                            localvars,
                            system_.intvars(),
                            system_.clocks(),
                            [&] (std::string const & msg) { log.error(context, msg); }));
        if (! oss.str().empty()) {
            throw std::runtime_error (oss.str());
        }
        return tchecker::compile(*texpr);
      }
    };

  } // end of namespace covreach
  
} // end of namespace tchecker

#endif // TCHECKER_ALGORITHMS_COVREACH_ACCEPTING_HH
