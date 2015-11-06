//
// Created by david on 10/8/15.
//

#ifndef SPRINCLE_RETE_HPP
#define SPRINCLE_RETE_HPP

#include <vector>
#include <tuple>
#include <array>
#include <utility>
#include <algorithm>
#include <iostream>

#include <boost/iterator/filter_iterator.hpp>
#include <boost/range.hpp>

#include <caf/all.hpp>

#include "detail.hpp"

using namespace std;
using namespace caf;

namespace {

  template<class predicate_t, class vector_t>
  decltype(auto) filter_impl(predicate_t&& p, vector_t&& v) noexcept {

    auto good = boost::make_iterator_range(
      boost::filter_iterator<predicate_t, decltype(begin(forward<vector_t>(v)))>
        (forward<predicate_t>(p), begin(forward<vector_t>(v)), end(forward<vector_t>(v))),
      boost::filter_iterator<predicate_t, decltype(begin(forward<vector_t>(v)))>
        (forward<predicate_t>(p), end(forward<vector_t>(v)), end(forward<vector_t>(v)))
    );

    return remove_reference_t<vector_t>(good.begin(), good.end());
  };

}

namespace sprincle {

  /*
   * delta class. Container.
   * Implemented concepts are: TMC, TMA, TCC, TCA, DC
   */
  template<typename tuple_t>
  struct delta {

    using change_t = tuple_t;
    using changeset_t = vector<change_t>;

    changeset_t positive;
    changeset_t negative;

    delta() : positive(), negative() {}

    template<typename other_changeset_t>
    delta(other_changeset_t&& p, other_changeset_t&& n) :
      positive(forward<other_changeset_t>(p)),
      negative(forward<other_changeset_t>(n))
    {}

  };

  /*
   * Can be a Trimmer Node
   */
  template<typename tuple_t, size_t... I>
  struct trimmer {
    static decltype(auto) behavior(event_based_actor* self) {
      return caf::behavior {
        [=](const delta<tuple_t>& changes) {

          auto insert = [](auto& to, const auto& from) {
            auto i = 0;
            for (const auto& tuple : from) {
              to[i] = project<I...>(tuple);
              ++i;
            }
          };

          using projected_change_t = decltype(project<I...>(declval<typename delta<tuple_t>::change_t>()));
          using projected_changeset_t = typename delta<projected_change_t>::changeset_t;

          projected_changeset_t positives(changes.positive.size());
          projected_changeset_t negatives(changes.negative.size());

          insert(positives, changes.positive);
          insert(negatives, changes.negative);

          return delta<projected_change_t>(move(positives), move(negatives));
        },
        others >> [=] {
          //TODO: Print error
        }
      };
    }
  };


  /*
   * Can be an Equality Node, Inequality Node and Predicate Evaluator
   */
  template<class tuple_t, class predicate_t>
  struct filter {
    static decltype(auto) behavior(event_based_actor* self, const predicate_t& predicate) {
      return caf::behavior {
        [=](const delta<tuple_t>& changes) {

          return delta<tuple_t>(
            filter_impl(predicate, changes.positive),
            filter_impl(predicate, changes.negative)
          );

        },
        others >> [=] {
          //TODO: Print error
        }
      };
    };

  };
}




#endif //SPRINCLE_RETE_HPP
