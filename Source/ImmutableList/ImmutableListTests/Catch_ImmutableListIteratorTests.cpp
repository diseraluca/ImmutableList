#include "catch.hpp"

#include <immutable_list.h>

using namespace lds;

TEST_CASE("immutable_list_iterator is default constructable") {
	REQUIRE(std::is_default_constructible<immutable_list_iterator<int>>::value);
}

TEST_CASE("immutable_list_iterator is copy constructable") {
	REQUIRE(std::is_copy_constructible<immutable_list_iterator<char>>::value);
}

TEST_CASE("immutable_list_iterator is copy assignable") {
	REQUIRE(std::is_copy_assignable<immutable_list_iterator<immutable_list<char>>>::value);
}

TEST_CASE("immutable_list_iterator is destructible") {
	REQUIRE(std::is_destructible<immutable_list_iterator<float>>::value);
}

TEST_CASE("immutable_list_iterator can be compared for equivalence") {
	REQUIRE(immutable_list_iterator<int>() == immutable_list_iterator<int>());
}

TEST_CASE("immutable_list_iterator can be compared for inequality") {
	REQUIRE_FALSE(immutable_list_iterator<int>() != immutable_list_iterator<int>());
}

TEST_CASE("immutable_list_iterator can be dereferenced") {
	immutable_list<char> list{'a'};
	immutable_list<immutable_list<char>> list2{ immutable_list<char>('a') };

	REQUIRE(*(list.cbegin()));
	REQUIRE(list2.cbegin()->size());
}

TEST_CASE("immutable_list_iterator can be incremented") {
	immutable_list<bool> list(true);

	SECTION("immutable_list_iterator returns a dereferenceable result or a past-the-end result when incremented") {
		auto iterator = list.cbegin();
		++iterator;

		REQUIRE((iterator == list.cend() || *iterator));
	}

	SECTION("immutable_list_iterators that compare equals still do after both being incremented") {
		auto iterator = list.cbegin();
		auto iterator2 = list.cbegin();

		REQUIRE(iterator == iterator2);
		REQUIRE(++iterator == ++iterator2);
	}
}