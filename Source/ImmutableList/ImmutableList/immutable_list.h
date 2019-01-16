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
	class immutable_list {
	public:
		using value_type = T;
		using size_type = std::size_t;

	public:
		immutable_list() : head{ nullptr }, m_size{ 0 } {}
		explicit immutable_list(value_type&& data) : head{ std::make_shared<Node>(std::forward<value_type>(data)) }, m_size{ 1 } {}

	public:
		[[nodiscard]] immutable_list push_front(value_type&& data) const {
			immutable_list newList{ std::forward<value_type>(data) };
			
			newList.head->next = this->head;
			newList.m_size = 1 + this->m_size;

			return newList;
		}

	public:
		[[nodiscard]] bool empty() const noexcept {
			return !!this->m_size;
		}

		[[nodiscard]] size_type size() const noexcept {
			return this->m_size;
		}

		[[nodiscard]] size_type max_size() const noexcept {
			return std::numeric_limits<size_type>::max();
		}

	private:
		struct Node {
		public:
			explicit Node(value_type&& data) : data{ std::forward<value_type>(data) }, next{ nullptr } {}
			explicit Node(value_type&& data, Node next) : data{ std::forward<value_type>(data) }, next{ next } {}

		public:
			value_type data;
			std::shared_ptr<Node> next;
		};

		std::shared_ptr<Node> head;
		size_type m_size;
	};
}