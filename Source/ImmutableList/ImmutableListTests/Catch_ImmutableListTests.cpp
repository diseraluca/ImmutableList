// Copyright 2018 Luca Di Sera
//		Contact: disera.luca@gmail.com
//				 https://github.com/diseraluca
//				 https://www.linkedin.com/in/luca-di-sera-200023167
//
// This code is licensed under the MIT License. 
// More informations can be found in the LICENSE file in the root folder of this repository

#include "catch.hpp"

#include <immutable_list.h>

using namespace lds;

TEST_CASE("An immutable_list can be constructed from an initializer list", "[immutable_list][constructors]") {
	std::initializer_list<int> initializer{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	immutable_list<int> list{ initializer };

	SECTION("The resuting list has the same size of the initializer_list") {
		REQUIRE(list.size() == initializer.size());
	}

	SECTION("The resulting list contains all the elemnents in the initializer_list in the same order") {
		REQUIRE(std::equal(initializer.begin(), initializer.end(), list.cbegin(), list.cend()));
	}
}

TEST_CASE("immutable_list::push_front creates and returns a new list with an added node before the head", "[immutable_list][push_front][modifiers]") {
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

TEST_CASE("immutable_list::pop_front creates and returns a new list with the head element removed", "[immutable_list][pop_front][modifiers]") {
	immutable_list<int> list{ 13 };
	auto newList{ list.pop_front() };

	SECTION("The new list size is one less than the original list") {
		REQUIRE(newList.size() == (list.size() - 1));
	}

	SECTION("Popping from a one element list will return an empty list") {
		CHECK(list.size() == 1);
		REQUIRE(newList.empty());
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