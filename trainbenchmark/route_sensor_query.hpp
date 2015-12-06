
#ifndef SPRINCLE_ROUTE_SENSOR_QUERY_HPP
#define SPRINCLE_ROUTE_SENSOR_QUERY_HPP

#include <tuple>
#include <boost/network/uri.hpp>
#include <sprincle/rete.hpp>
#include <caf/all.hpp>

#include <iostream>

using namespace sprincle;
using namespace std;
using namespace caf;
namespace net = boost::network;

namespace sprincle {

  namespace route_sensor {

    struct network {

      using edge_t = tuple<unsigned long, unsigned long>;
      using in_0_t = input_node<edge_t, primary>;
      using in_1_t = input_node<edge_t, secondary>;
      using join_node_0_t = join_node<edge_t, edge_t, primary, match_pair<0,1>>;
      using join_node_1_t = join_node<join_node_0_t::result_tuple_t, edge_t, primary, match_pair<1,0>>;
      using antijoin_node_0_t = antijoin_node<join_node_1_t::result_tuple_t, edge_t, primary, match_pair<2,0>, match_pair<3,1>>;

      const actor antijoin_node_0;
      const actor join_node_1;
      const actor join_node_0;

      const actor in_definedBy;
      const actor in_sensor;
      const actor in_follows;
      const actor in_switch;

      const size_t input_size = 4;

      network(const actor& production) :
        antijoin_node_0(spawn<antijoin_node_0_t>(production)),
        join_node_1(spawn<join_node_1_t>(antijoin_node_0)),
        join_node_0(spawn<join_node_0_t>(join_node_1)),
        in_definedBy(spawn<in_1_t>(antijoin_node_0)),
        in_sensor(spawn<in_1_t>(join_node_1)),
        in_follows(spawn<in_1_t>(join_node_0)),
        in_switch(spawn<in_0_t>(join_node_0))
      {}

      network(const network&) = delete;
      network(network&&) = delete;
      network& operator=(const network&) = delete;

    };

    using edge_t = typename network::edge_t;

    //callback on triple read
    template<class Actor>
    void on_triple(const Actor& self, const network& network, string&& subject, string&& predicate, string&& object) {

      if(net::uri::uri(predicate).fragment() == string("switch")) {
        self->send(
          network.in_switch,
          primary::value,
          delta<edge_t> {
            set<edge_t> {
              make_tuple(
                stol(net::uri::uri(subject).fragment().substr(1)),
                stol(net::uri::uri(object).fragment().substr(1))
              )
            },
            set<edge_t> { /* no negatives */ }
          });
      } else if(net::uri::uri(predicate).fragment() == string("follows")) {
        self->send(
          network.in_follows,
          primary::value,
          delta<edge_t> {
            set<edge_t> {
              make_tuple(
                stol(net::uri::uri(subject).fragment().substr(1)),
                stol(net::uri::uri(object).fragment().substr(1))
              )
            },
            set<edge_t> { /* no negatives */ }
          });
      } else if(net::uri::uri(predicate).fragment() == string("sensor")) {
        self->send(
          network.in_sensor,
          primary::value,
          delta<edge_t> {
            set<edge_t> {
              make_tuple(
                stol(net::uri::uri(subject).fragment().substr(1)),
                stol(net::uri::uri(object).fragment().substr(1))
              )
            },
            set<edge_t> { /* no negatives */ }
          });
      } else if(net::uri::uri(predicate).fragment() == string("definedBy")) {
        self->send(
          network.in_definedBy,
          primary::value,
          delta<edge_t> {
            set<edge_t> {
              make_tuple(
                stol(net::uri::uri(subject).fragment().substr(1)),
                stol(net::uri::uri(object).fragment().substr(1))
              )
            },
            set<edge_t> { /* no negatives */ }
          });
      }
    }

  }

}

#endif
