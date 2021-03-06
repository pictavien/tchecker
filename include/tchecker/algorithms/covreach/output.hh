/*
 * This file is a part of the TChecker project.
 *
 * See files AUTHORS and LICENSE for copyright details.
 *
 */

#ifndef TCHECKER_ALGORITHMS_COVREACH_OUTPUT_HH
#define TCHECKER_ALGORITHMS_COVREACH_OUTPUT_HH

#include <iostream>
#include <stack>
#include <tuple>
#include <unordered_set>

#include "tchecker/algorithms/covreach/graph.hh"
#include "tchecker/utils/shared_objects.hh"

/*!
 \file output.hh
 \brief Outputters for graph generated by covering reachability algorithm
 */

namespace tchecker {
  
  namespace covreach {
    
    /*!
     \class dot_ouputter_t
     \brief Output a graph using the dot file format
     \tparam NODE_OUTPUTTER : typoe of node outputter
     */
    template <class NODE_OUTPUTTER>
    class dot_outputter_t {
    public:
      /*!
       \brief Constructor
       \param args : arguments to a constructor of NODE_OUTPUTTER
       */
      template <class ... ARGS>
      dot_outputter_t(std::tuple<ARGS...> args)
      : _node_outputter(std::make_from_tuple<NODE_OUTPUTTER>(args))
      {}
      
      /*!
       \brief Copy constructor
       */
      dot_outputter_t(tchecker::covreach::dot_outputter_t<NODE_OUTPUTTER> const &) = default;
      
      /*!
       \brief Move constructor
       */
      dot_outputter_t(tchecker::covreach::dot_outputter_t<NODE_OUTPUTTER> &&) = default;
      
      /*!
       \brief Destructor
       */
      ~dot_outputter_t() = default;
      
      /*!
       \brief Assignment operator
       */
      tchecker::covreach::dot_outputter_t<NODE_OUTPUTTER> &
      operator= (tchecker::covreach::dot_outputter_t<NODE_OUTPUTTER> const &) = default;
      
      /*!
       \brief Move-assignment operator
       */
      tchecker::covreach::dot_outputter_t<NODE_OUTPUTTER> &
      operator= (tchecker::covreach::dot_outputter_t<NODE_OUTPUTTER> &&) = default;
      
      /*!
       \brief Graph output
       \tparam GRAPH : type of graph, should inherit from tchecker::covreach::graph_t
       \tparam HASH : type of hash function on GRAPH::node_ptr_t
       \tparam EQUAL : type of equality predicate on GRAPH::node_ptr_t
       \param os : output stream
       \param g : graph
       \param name : graph name
       \post g has been output to os
       \return os after g has been output
       */
      template
      <class GRAPH, class HASH=std::hash<typename GRAPH::node_ptr_t>, class EQUAL=std::equal_to<typename GRAPH::node_ptr_t>>
      std::ostream & output(std::ostream & os, GRAPH const & g, std::string const & name)
      {
        using node_ptr_t = typename GRAPH::node_ptr_t;
        using edge_ptr_t = typename GRAPH::edge_ptr_t;
        
        std::stack<node_ptr_t> waiting;
        std::unordered_set<node_ptr_t, HASH, EQUAL> passed;
        
        os << "digraph " << name << " {" << std::endl;
        os << "node [shape=\"box\",style=\"rounded\"];" << std::endl;
        
        auto root_nodes = g.root_nodes();
        for (node_ptr_t const & n : root_nodes) {
          waiting.push(n);
          passed.insert(n);
        }
        
        while ( ! waiting.empty() ) {
          node_ptr_t n = waiting.top();
          waiting.pop();
          
          os << "n" << n->identifier() << " [label=\"";
          _node_outputter.output(os, *n);
          os << "\"]" << std::endl;
          
          auto outgoing_edges = g.outgoing_edges(n);
          for (edge_ptr_t const & e : outgoing_edges) {
            node_ptr_t const & src = g.edge_src(e);
            node_ptr_t const & tgt = g.edge_tgt(e);
            
            os << "n" << src->identifier() << " -> " << "n" << tgt->identifier() << " ";
            if (g.edge_type(e) == tchecker::covreach::ABSTRACT_EDGE)
              os << "[color=blue]" << std::endl;
            else
              os << "[color=black]" << std::endl;
            
            if (passed.find(tgt) == passed.end()) {
              waiting.push(tgt);
              passed.insert(tgt);
            }
          }
        }
        
        os << "}" << std::endl;
        
        os.flush();
        
        passed.clear();
        
        return os;
      }
    private:
      NODE_OUTPUTTER _node_outputter;  /*!< Outputter of nodes */
    };
    
  } // end of namespace covreach
  
} // end of namespace tchecker

#endif // TCHECKER_ALGORITHMS_COVREACH_OUTPUT_HH
