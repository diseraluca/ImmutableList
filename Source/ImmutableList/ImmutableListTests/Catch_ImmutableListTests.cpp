#include "catch.hpp"

#include <immutable_list.h>

using namespace lds;

TEST_CASE("immutable_list::push_front creates and returns a new list with an added node before the head", "[immutable_list][push_front]") {
	int frontValue{ 15 };
	
	immutable_list<int> list{ 14 };
	auto newList = list.push_front(frontValue);

	SECTION("The new list size is one more than the original list") {
		REQUIRE(newList.size() == (list.size() + 1));
	}

	SECTION("The first element of the new list contains the data passed to push_front") {
		REQUIRE(newList.front() == frontValue);
	}
}

TEST_CASE("immutable_list supports const iterators for its range", "[immutable_list][iterators]") {
	immutable_list<char> list{};

	SECTION("immutable_list::cbegin is identical to immutable_list::cend if the list is empty") {
		REQUIRE(list.cbegin() == list.cend());
	}

	SECTION("immutable_list::cbegin returns an iterator to the first element of the list") {
		char newValue = 'b';

		REQUIRE(*(list.push_front(newValue).cbegin()) == newValue);
	}

	SECTION("immutable_list::cbegin will reach immutable_list::cend by being incremented, after traversing all the elements of the list") {
		auto newList = list.push_front('b').push_front('c').push_front('d');
		auto iterator = newList.cbegin();

		CHECK(iterator != newList.cend());
		for (std::size_t _{ 0 }; _ < newList.size(); ++iterator, ++_) {}
		REQUIRE(iterator == newList.cend());
	}
}