// Copyright 2018 Luca Di Sera
//		Contact: disera.luca@gmail.com
//				 https://github.com/diseraluca
//				 https://www.linkedin.com/in/luca-di-sera-200023167
//
// This code is licensed under the MIT License. 
// More informations can be found in the LICENSE file in the root folder of this repository

#pragma once

#include <memory>

namespace lds {
	template <typename T>
	class immutable_list;

	template <typename T>
	class immutable_list_iterator {
	public:
		using value_type = T;
		using reference = const value_type&;
		using pointer = const value_type*;
		using difference_type = std::ptrdiff_t;
		using iterator_category = std::forward_iterator_tag;

	public:
		immutable_list_iterator() : node{} {}
		immutable_list_iterator(const immutable_list_iterator<T>& other) : node{ other.node } {};
		immutable_list_iterator(std::shared_ptr<typename immutable_list<T>::Node> node) : node{ node } {}

		immutable_list_iterator<T>& operator=(const immutable_list_iterator<T>& other) { node = other.node; return *this; }
	public:
		template <typename T>
		friend bool operator==(const immutable_list_iterator<T>& left, const immutable_list_iterator<T>& right) noexcept;

		template <typename T>
		friend bool operator!=(const immutable_list_iterator<T>& left, const immutable_list_iterator<T>& right) noexcept;

		immutable_list_iterator<T>& operator++();
		immutable_list_iterator<T>& operator++(int);

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
		immutable_list() : head{ nullptr }, m_size{ 0 } {}

		explicit immutable_list(value_type data) : head{ std::make_shared<Node>(data) }, m_size{ 1 } {}

	public: // ELEMENT ACCESS
		[[nodiscard]] const_reference front() const;

	public: // ITERATORS
		[[nodiscard]] const_iterator cbegin() const noexcept {
			return const_iterator{ this->head };
		}

		[[nodiscard]] const_iterator cend() const noexcept {
			return const_iterator{ nullptr };
		}

	public: // MODIFIERS
		[[nodiscard]] immutable_list<T> push_front(value_type data) const;

	public: // CAPACITY
		[[nodiscard]] bool empty() const noexcept;

		[[nodiscard]] size_type size() const noexcept;

		[[nodiscard]] size_type max_size() const noexcept;

	public:
		struct Node {
		public:
			explicit Node(value_type data) : data{ data }, next{ nullptr } {}
			explicit Node(value_type data, Node next) : data{ data }, next{ next } {}

		public:
			value_type data;
			std::shared_ptr<Node> next;
		};

	private:
		std::shared_ptr<Node> head;
		size_type m_size;
	};

	template<typename T>
	typename inline immutable_list<T>::const_reference immutable_list<T>::front() const
	{
		return this->head->data;
	}

	template<typename T>
	inline immutable_list<T> immutable_list<T>::push_front(value_type data) const
	{
		immutable_list<T> newList{ data };

		newList.head->next = this->head;
		newList.m_size = 1 + this->m_size;

		return newList;
	}

	template<typename T>
	inline bool immutable_list<T>::empty() const noexcept
	{
		return !!this->m_size;
	}

	template<typename T>
	typename inline immutable_list<T>::size_type immutable_list<T>::size() const noexcept
	{
		return this->m_size;
	}

	template<typename T>
	typename inline immutable_list<T>::size_type immutable_list<T>::max_size() const noexcept
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
	inline immutable_list_iterator<T>& immutable_list_iterator<T>::operator++()
	{
		this->node = this->node.lock()->next;

		return *this;
	}

	template<typename T>
	inline immutable_list_iterator<T>& immutable_list_iterator<T>::operator++(int)
	{
		auto previous{ *this };
		++(*this);

		return previous;
	}


	template<typename T>
	typename inline immutable_list_iterator<T>::reference immutable_list_iterator<T>::operator*() const
	{
		auto shared = this->node.lock();

		return shared->data;
	}

	template<typename T>
	typename inline immutable_list_iterator<T>::pointer immutable_list_iterator<T>::operator->() const
	{
		auto shared = this->node.lock();
		
		return &(shared->data);
	}
}