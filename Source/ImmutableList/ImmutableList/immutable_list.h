// Copyright 2018 Luca Di Sera
//		Contact: disera.luca@gmail.com
//				 https://github.com/diseraluca
//				 https://www.linkedin.com/in/luca-di-sera-200023167
//
// This code is licensed under the MIT License. 
// More informations can be found in the LICENSE file in the root folder of this repository

#pragma once

#include <memory>
#include <sstream>

namespace lds {
	template <typename T>
	class immutable_list;

	template <typename T>
	class immutable_list_iterator {
		friend class immutable_list<T>;

	public:
		using value_type = T;
		using reference = const value_type&;
		using pointer = const value_type*;
		using difference_type = std::ptrdiff_t;
		using iterator_category = std::forward_iterator_tag;

	public:
		immutable_list_iterator() : node{} {}
		immutable_list_iterator(const immutable_list_iterator<T>& other) =default;
		immutable_list_iterator(std::shared_ptr<typename immutable_list<T>::Node> node) : node{ node } {}

		immutable_list_iterator<T>& operator=(const immutable_list_iterator<T>& other) { node = other.node; return *this; }
	public:
		template <typename T>
		friend bool operator==(const immutable_list_iterator<T>& left, const immutable_list_iterator<T>& right) noexcept;

		template <typename T>
		friend bool operator!=(const immutable_list_iterator<T>& left, const immutable_list_iterator<T>& right) noexcept;

		immutable_list_iterator<T>& operator++();
		immutable_list_iterator<T> operator++(int);

		[[nodiscard]] reference operator*() const;

		[[nodiscard]] pointer operator->() const;

	private:
		std::weak_ptr<typename immutable_list<T>::Node> node;
	};

	template <typename T>
	class immutable_list {
	public:
		using value_type = T;
		using reference = value_type & ;
		using const_reference = const value_type&;
		using const_iterator = immutable_list_iterator<T>;
		using size_type = std::size_t;

	public: // CONSTRUCTORS
		immutable_list() : head{ std::make_shared<Node>() }, tail{ head }, m_size{ 0 } {}

		explicit immutable_list(value_type data) : head{ std::make_shared<Node>(data) }, m_size{ 1 } { this->head->next = std::make_shared<Node>(); tail = this->head->next; }
		immutable_list(const immutable_list<T>& other) =default;

		immutable_list(const_iterator first, const_iterator last);

		explicit immutable_list(std::initializer_list<T> list);

	public: // ELEMENT ACCESS
		[[nodiscard]] const_reference front() const;

		const_reference at(size_type index) const;
		const_reference operator[](size_type index) const;

	public: // ITERATORS
		[[nodiscard]] const_iterator cbegin() const noexcept {
			return const_iterator{ this->head };
		}

		[[nodiscard]] const_iterator cend() const noexcept {
			return const_iterator{ this->tail.lock() };
		}

	public: // MODIFIERS
		[[nodiscard]] immutable_list<T> clear() const noexcept;
		[[nodiscard]] immutable_list<T> push_front(value_type data) const;
		[[nodiscard]] immutable_list<T> pop_front() const;

		[[nodiscard]] immutable_list<T> insert_after(const_iterator pos, value_type value) const;

	public: // CAPACITY
		[[nodiscard]] bool empty() const noexcept;

		[[nodiscard]] size_type size() const noexcept;

		[[nodiscard]] constexpr size_type max_size() const noexcept;

	public: // OPERATORS
		template <typename T>
		friend bool operator==(const immutable_list<T>& left, const immutable_list<T>& right);

		template <typename T>
		friend bool operator!=(const immutable_list<T>& left, const immutable_list<T>& right);

	public:
		struct Node {
		public:
			Node() =default;
			explicit Node(value_type data) : data{ data }, next{ nullptr } {}

		public:
			value_type data;
			std::shared_ptr<Node> next;
		};

	private:
		std::shared_ptr<Node> head;
		std::weak_ptr<Node> tail;
		size_type m_size;
	};

	//TODO: make a more elegant implementation
	template<typename T>
	inline immutable_list<T>::immutable_list(const_iterator first, const_iterator last)
	{
		if (first == last) {
			this->head = std::make_shared<Node>(*first);
			this->m_size = 0;
		}

		this->head = std::make_shared<Node>(*first);
		this->m_size = 1;

		Node* currentNode{ this->head.get() };
		while (++first != last) {
			currentNode->next = std::make_shared<Node>(*first);
			++this->m_size;

			currentNode = currentNode->next.get();
		}

		currentNode->next = std::make_shared<Node>();
		this->tail = currentNode->next;
	}

	template<typename T>
	inline immutable_list<T>::immutable_list(std::initializer_list<T> list)
	{
		this->head = std::make_shared<Node>();
		this->tail = this->head;

		auto lastElement{ std::rend(list) };
		for (auto element{ std::rbegin(list) }; element != lastElement ; ++element) {
			auto node{ std::make_shared<Node>(*element) };
			node->next = this->head;

			this->head = node;
		}

		this->m_size = list.size();
	}

	template<typename T>
	typename inline immutable_list<T>::const_reference immutable_list<T>::front() const
	{
		return this->head->data;
	}

	template<typename T>
	typename inline immutable_list<T>::const_reference immutable_list<T>::at(immutable_list<T>::size_type index) const
	{
		if (index >= this->m_size) {
			throw std::out_of_range((std::stringstream() << "The list does not contain index " << index).str());
		}

		return (*this)[index];
	}

	template<typename T>
	typename inline immutable_list<T>::const_reference immutable_list<T>::operator[](immutable_list<T>::size_type index) const
	{
		auto element{ this->cbegin() };
		while (index--) { ++element; }

		return *element;
	}

	template<typename T>
	inline immutable_list<T> immutable_list<T>::clear() const noexcept
	{
		return immutable_list<T>();
	}

	template<typename T>
	inline immutable_list<T> immutable_list<T>::push_front(value_type data) const
	{
		immutable_list<T> newList{ data };

		newList.head->next = this->head;
		newList.tail = this->tail.lock();
		newList.m_size = 1 + this->m_size;

		return newList;
	}

	template<typename T>
	inline immutable_list<T> immutable_list<T>::pop_front() const
	{
		immutable_list<T> newList{};
		newList.head = this->head->next;
		newList.m_size = 1 - this->m_size;

		return newList;
	}

	// TODO: Benchmark this implementation and a flattened out one ( e.g copying the list manually so we don't have to traverse to the last element )
	// TODO: Change to an elegant solution
	template<typename T>
	inline immutable_list<T> immutable_list<T>::insert_after(const_iterator pos, value_type value) const
	{
		immutable_list<T> newList{ this->cbegin(), ++pos };
		auto it{ newList.cbegin() };
		while (it.node.lock()->next != newList.cend().node.lock())
		{
			++it;
		}

		it.node.lock()->next = std::make_shared<Node>(value);
		it.node.lock()->next->next = pos.node.lock();
		newList.tail = this->tail;

		++newList.m_size;
		while (pos++ != this->cend()) {
			++newList.m_size;
		}

		return newList;
	}

	template<typename T>
	inline bool immutable_list<T>::empty() const noexcept
	{
		return !this->m_size;
	}

	template<typename T>
	typename inline immutable_list<T>::size_type immutable_list<T>::size() const noexcept
	{
		return this->m_size;
	}

	template<typename T>
	typename inline constexpr immutable_list<T>::size_type immutable_list<T>::max_size() const noexcept
	{
		return std::numeric_limits<size_type>::max();
	}

	template<typename T>
	inline bool operator==(const immutable_list_iterator<T>& left, const immutable_list_iterator<T>& right) noexcept
	{
		return left.node.lock() == right.node.lock();
	}

	template<typename T>
	inline bool operator!=(const immutable_list_iterator<T>& left, const immutable_list_iterator<T>& right) noexcept
	{
		return !(left.node.lock() == right.node.lock());
	}

	template<typename T>
	bool operator==(const immutable_list<T>& left, const immutable_list<T>& right)
	{
		return std::equal(left.cbegin(), left.cend(), right.cbegin(), right.cend());
	}

	template<typename T>
	bool operator!=(const immutable_list<T>& left, const immutable_list<T>& right)
	{
		return !std::equal(left.cbegin(), left.cend(), right.cbegin(), right.cend());
	}

	template<typename T>
	inline immutable_list_iterator<T>& immutable_list_iterator<T>::operator++()
	{
		this->node = this->node.lock()->next;

		return *this;
	}

	template<typename T>
	inline immutable_list_iterator<T> immutable_list_iterator<T>::operator++(int)
	{
		immutable_list_iterator<T> previous{ *this };
		++(*this);

		return previous;
	}

	template<typename T>
	typename inline immutable_list_iterator<T>::reference immutable_list_iterator<T>::operator*() const
	{
		return this->node.lock()->data;
	}

	template<typename T>
	typename inline immutable_list_iterator<T>::pointer immutable_list_iterator<T>::operator->() const
	{
		return &(this->node.lock()->data);
	}
}