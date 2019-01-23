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

TEST_CASE("An immutable_list can be constructed from another list range", "[immutable_list][constructors]") {
	int endValue{ 3 };

	immutable_list<int> list{ 1, 2, 3, 4 };
	immutable_list<int> newList{ list.cbegin(), std::find(list.cbegin(), list.cend(), endValue) };

	SECTION("The new list is equal to original list range") {
		REQUIRE(std::equal(newList.cbegin(), newList.cend(), list.cbegin()));
	}
}

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

TEST_CASE("immutable_list provides access methods to individual elements", "[immutable_list][element_access][exception]") {
	immutable_list<int> list{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

	SECTION("operator[] returns the element at index i counting from 0") {
		REQUIRE(list[1] == 1);
		REQUIRE(list[5] == 5);
	}

	SECTION("immutable_list::at returns the element at index i counting from 0") {
		REQUIRE(list[4] == 4);
		REQUIRE(list[9] == 9);
	}

	SECTION("immutable_list::at throws std::out_of_range if the index is greater than the elements in the list") {
		immutable_list<int>::size_type outOfRangeIndex{ list.size() };

		REQUIRE_THROWS_AS(list.at(outOfRangeIndex), std::out_of_range);
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

TEST_CASE("immutable_list::clear return an new empty immutable_list") {
	immutable_list<int> list{ 15 };

	CHECK(list != list.clear());
	REQUIRE(list.clear().empty());
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

TEST_CASE("immutable_list::insert_after returns a new list with new elements inserted after the given position") {
	int valueToInsertAfter{ 4 };
	int newValue{ 5 };
	immutable_list<int> list{ 1, 2, 3, 4, 6, 7, 8, 9 };
	auto it{ std::find(list.cbegin(), list.cend(), valueToInsertAfter) };

	auto newList{ list.insert_after(it, newValue) };

	SECTION("The new list size is one more than the original list") {
		REQUIRE(newList.size() == (list.size() + 1));
	}

	SECTION("The newly inserted value is inserted right after the given position") {
		auto newIt{ std::find(newList.cbegin(), newList.cend(), valueToInsertAfter) };

		REQUIRE(*(++newIt) == newValue);
	}

	SECTION("The two lists are equal before the inserted value") {
		REQUIRE(std::equal(list.cbegin(), ++it, newList.cbegin()));
	}

	SECTION("The two lists are equal after the inserted value") {
		auto newIt{ std::find(newList.cbegin(), newList.cend(), newValue) };

		REQUIRE(std::equal(++it, list.cend(), ++newIt));
	}
}

TEST_CASE("immutable_lists can be compared for equality and inequality", "[immutable_list][operators][equality][inequality]") {
	immutable_list<int> list{ 14 };

	SECTION("A list is always equal to itself") {
		REQUIRE(list == list);
	}

	SECTION("Two lists with different elements are not equal to one another") {
		REQUIRE(list != list.push_front(15));
	}

	SECTION("Two lists of different size are unequal") {
		auto list2{ list.push_front(16) };

		CHECK(list.size() != list2.size());
		REQUIRE(list != list2);
	}

	SECTION("Two lists with the same elements are equal") {
		immutable_list<int> list2{ 14 };

		REQUIRE(list == list2);
	}
}