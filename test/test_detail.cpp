//
// Created by david on 10/13/15.
//

#define BOOST_TEST_MODULE DetailTest

#include <boost/test/unit_test.hpp>
#include <sprincle/details/project.hpp>
#include <sprincle/details/exactly.hpp>


#include <tuple>
#include <string>
#include <utility>

using namespace std;
using namespace sprincle;

BOOST_AUTO_TEST_SUITE( DetailTestSuite )

BOOST_AUTO_TEST_CASE( ProjectionShouldWork_0 )
{

  auto _0 = 5;
  auto _1 = 6;
  auto _2 = 7;

  auto input = std::make_tuple(_0, _1, _2);

  auto preserve = details::project<decltype(input), 0, 1, 2>();

  auto output = preserve(input);

  BOOST_CHECK( output == input );

}

BOOST_AUTO_TEST_CASE( ProjectionShouldWork_1 )
{
  auto _0 = std::string("duck");
  auto _1 = 4;
  auto _2 = 7;

  auto t = std::make_tuple(_0, _1, _2);
  auto trim = details::project<decltype(t), 1>();
  auto actual = trim(t);

  BOOST_CHECK( get<0>(actual) == _1 );

}

BOOST_AUTO_TEST_CASE( Exactly_1 )
{
  auto _0 = 0;
  auto _1 = 2;
  auto _2 = 3;

  auto compare = details::make_exactly(std::make_tuple(_0, _1, _2));


  auto the_same = compare(std::make_tuple(_0, _1, _2));

  BOOST_CHECK( the_same );

}

BOOST_AUTO_TEST_SUITE_END()
