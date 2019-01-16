#include "catch.hpp"

#include <immutable_list.h>

using namespace lds;

TEST_CASE("immutable_list::push_front creates and returns a new list with an added node at before the head", "[immutable_list][push_front]") {
	immutable_list<int> list{ 14 };

	CHECK(list.size() == 1);

	auto newList = list.push_front(15);

	REQUIRE(newList.size() == (list.size() + 1));
}