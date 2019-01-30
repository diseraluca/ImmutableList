// Copyright 2018 Luca Di Sera
//		Contact: disera.luca@gmail.com
//				 https://github.com/diseraluca
//				 https://www.linkedin.com/in/luca-di-sera-200023167
//
// This code is licensed under the MIT License. 
// More informations can be found in the LICENSE file in the root folder of this repository

/*! \file */

// TODO : Check all methods to see if they satisfy their expected algorithmic complexity
// TODO : Complete and revise documentation

#pragma once

#include <memory>
#include <sstream>

namespace lds {
	template <typename T>
	class immutable_list;

	/*!
	 * @class	immutable_list_iterator
	 *
	 * @brief	An immutable list iterator.
	 *
	 * @tparam	T	Generic type parameter.
	 */

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
		immutable_list_iterator() =default;
		immutable_list_iterator(const immutable_list_iterator<T>& other) =default;
		immutable_list_iterator(std::shared_ptr<typename immutable_list<T>::Node> node) : node{ node } {}

		immutable_list_iterator<T>& operator=(const immutable_list_iterator<T>& other) =default;
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

	/*!
	 * @class	immutable_list
	 *
	 * @brief	An immutable singly-linked list implementation
	 * 
	 * @tparam	T	Generic type parameter.
	 */

	template <typename T>
	class immutable_list {
		friend class immutable_list_iterator<T>;

	public:
		using value_type = T;
		using reference = value_type & ;
		using const_reference = const value_type&;
		using const_iterator = immutable_list_iterator<T>;
		using size_type = std::size_t;

	public:

		/*! @name Constructors
	     */
	     ///@{
	      
		immutable_list();

		explicit immutable_list(value_type& data);
		explicit immutable_list(value_type&& data);

		immutable_list(const immutable_list<T>& other) =default;

		template <typename InputIterator, 
			      typename = std::enable_if_t<std::is_base_of_v<std::input_iterator_tag, typename std::iterator_traits<InputIterator>::iterator_category>>>
		immutable_list(InputIterator first, InputIterator last);

		explicit immutable_list(std::initializer_list<T> list);

		///@}

	public:

		/*!
		 * @name Element Access
		 *
		 * Methods providing access to the data stored in the list.
		 *
		 * The data is returned trough const references that should not be modified.
		 */
		///@{

		[[nodiscard]] const_reference front() const;

		const_reference at(size_type index) const;
		const_reference operator[](size_type index) const;
		
		///@}

	public:

		/*! @name Iterators
		 */
		 ///@{
		  
		[[nodiscard]] const_iterator cbegin() const noexcept {
			return const_iterator{ this->head };
		}

		[[nodiscard]] const_iterator cend() const noexcept {
			return const_iterator{ this->tail.lock() };
		}

		///@}

	public:

		/*!
		 * @name Modifiers
		 *
		 * Modifiers provide a way to generate a new modified list from another one.
		 *
		 * Prepending elements, inserting elements and other operations can be done trough them.
		 *
		 * Each modifier return a new list with the same type of the original one that models the result of a modification.
		 *
		 *
		 * The original list isn't modified in any way.
		 * References and Iterators to the orignal list are not invalidated
		 *
		 * The new list and the orignal one may share some memory.
		 * 
		 * Modifiers ensures a Strong Exception Garuantee
		 *
		 */
		///@{
		
		[[nodiscard]] immutable_list<T> clear() const noexcept;

		[[nodiscard]] immutable_list<T> push_front(value_type& data) const;
		[[nodiscard]] immutable_list<T> push_front(value_type&& data) const;

		template <typename ...Args>
		[[nodiscard]] immutable_list<T> emplace_front(Args&&... args) const;

		[[nodiscard]] immutable_list<T> pop_front() const;

		[[nodiscard]] immutable_list<T> insert_after(const_iterator pos, const value_type& value) const;
		[[nodiscard]] immutable_list<T> insert_after(const_iterator pos, value_type&& value) const;
		[[nodiscard]] immutable_list<T> insert_after(const_iterator pos, size_type count, const value_type& value) const;
		[[nodiscard]] immutable_list<T> insert_after(const_iterator pos, std::initializer_list<T> list) const;

		template<typename... Args>
		[[nodiscard]] immutable_list<T> emplace_after(const_iterator pos, Args&&... args) const;

		template <typename InputIterator>
		[[nodiscard]] immutable_list<T> insert_after(const_iterator pos, InputIterator first, InputIterator last) const;

		///@}
		 
	private:
		template <typename U>
		[[nodiscard]] immutable_list<T> push_front_impl(U&& data) const;

		template <typename U>
		[[nodiscard]] immutable_list<T> insert_after_impl(const_iterator pos, size_type count, U&& value, std::true_type) const;

		template <typename InputIterator>
		[[nodiscard]] immutable_list<T> insert_after_impl(const_iterator pos, InputIterator first, InputIterator last, std::false_type) const;


	public:

		/*!
		 * @name Capacity
		 */
		 ///@{
	  
		[[nodiscard]] bool empty() const noexcept;

		[[nodiscard]] size_type size() const noexcept;

		[[nodiscard]] constexpr size_type max_size() const noexcept;

		///@}
		 
	public: // OPERATORS
		template <typename T>
		friend bool operator==(const immutable_list<T>& left, const immutable_list<T>& right);

		template <typename T>
		friend bool operator!=(const immutable_list<T>& left, const immutable_list<T>& right);

	private: // HELPERS
		const_iterator iteratorAt(size_type index) const;

	private:
		struct Node {
		public:
			Node() : data{}, next{} {}

			template <typename U>
			explicit Node(U&& data) : data{ std::forward<U>(data) }, next{ nullptr } {}

		public:
			value_type data;
			std::shared_ptr<Node> next;
		};

		std::shared_ptr<Node> head;
		std::weak_ptr<Node> tail;
		size_type m_size;
	};
	 
	/*!
	 * @brief	Default constructor
	 *
	 * @tparam	T	Generic type parameter.
	 */

	template <typename T>
	inline immutable_list<T>::immutable_list() : head{ std::make_shared<Node>() }, tail{ head }, m_size{ 0 } {}

	/*!
	 * @brief	Constructs a single-element list with containing the passed in data
	 *
	 * @tparam	T	Generic type parameter.
	 * @param	data	The data for the single element of the list
	 */

	template <typename T>
	inline immutable_list<T>::immutable_list(value_type& data) : head{ std::make_shared<Node>(data) }, m_size{ 1 } { 
		this->head->next = std::make_shared<Node>(); 
		tail = this->head->next; 
	}

	/*!
	 * @overload
	 */

	template <typename T>
	inline immutable_list<T>::immutable_list(value_type&& data) : head{ std::make_shared<Node>(std::move(data)) }, m_size{ 1 } {
		this->head->next = std::make_shared<Node>();
		tail = this->head->next;
	}

	/*!
	 * @brief	Constructs a new list from the content of the range [first, last)
	 * 			
	 * The range must be valid and last must be reachable from first.
	 * An empty range (first == last) creates an empty list.
	 * 
	 * immutable_lists created in this way do not share memory. It is ineffiecient to copy a whole list trough its iterators, if such
	 * a beahviour is needed, use the copy constructor for better performance.
	 * 
	 * This overloads participate in overload resolution only if InputIterator satisfies the LegacyInputIterator concept.
	 *
	 * @tparam	T	Generic type parameter.
	 * @param	first	The first element of the range
	 * @param	last 	The last element of the range
	 */

	template <typename T>
	template <typename InputIterator, typename >
	inline immutable_list<T>::immutable_list(InputIterator first, InputIterator last)
	{   //TODO: Find a better solution to abstract away the difference between an empty range and a range with one or more elements
		//TODO: Find a more elegant solution
		if (first == last) {
			this->head = std::make_shared<Node>();
			this->tail = this->head;
			this->m_size = 0;
			return;
		}

		this->head = std::make_shared<Node>(*first);
		this->m_size = 1;

		Node* currentNode = this->head.get();
		while (++first != last) {
			currentNode->next = std::make_shared<Node>(*first);
			currentNode = currentNode->next.get();

			++this->m_size;
		}

		currentNode->next = std::make_shared<Node>();
		this->tail = currentNode->next;
	}

	/*!
	 * @brief	Constructs a new list from the contents of the initializer_list
	 *
	 * @tparam	T	Generic type parameter.
	 * @param	list	Initializer_list to initialize the elements of the list with
	 */

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

	/*!
	 * @brief	Gets the first element of the container
	 * 			
	 * Calling front on an empty list is considered undefined behaviour.
	 *
	 * @tparam	T	Generic type parameter.
	 *
	 * @returns	A const reference to the data in the first element of the list
	 */

	template<typename T>
	typename inline immutable_list<T>::const_reference immutable_list<T>::front() const
	{
		return this->head->data;
	}

	/*!
	 * @brief	Gets the ith element of the list. at is range checked.
	 *
	 * @exception	std::out_of_range	Thrown when index >= size().
	 *
	 * @tparam	T	Generic type parameter.
	 * @param	index	Zero-based index of the desired element
	 *
	 * @returns	A const reference to the data in the ith element
	 */

	template<typename T>
	typename inline immutable_list<T>::const_reference immutable_list<T>::at(immutable_list<T>::size_type index) const
	{
		if (index >= this->m_size) {
			throw std::out_of_range((std::stringstream() << "The list does not contain index " << index).str());
		}

		return (*this)[index];
	}

	/*!
	 * @brief	Gets the ith element of the list.
	 *
	 * @param	index	Zero-based index of the the desired element.
	 *
	 * @returns	A const reference to the data in the ith element
	 */

	template<typename T>
	typename inline immutable_list<T>::const_reference immutable_list<T>::operator[](immutable_list<T>::size_type index) const
	{
		return *iteratorAt(index);
	}

	 /*!
	  * @brief	Creates a new list with no elements from this list. Equivalent to creating an empty
	  * 		list with the default constructor.
	  *
	  * @tparam	T	Generic type parameter.
	  *
	  * @returns	A empty list.
	  */

	template<typename T>
	inline immutable_list<T> immutable_list<T>::clear() const noexcept
	{
		return immutable_list<T>();
	}

	/*!
	 * @brief	Generates a new list with an element prepended to it
	 * 			
	 * 			Does not make a copy of the whole list.
	 *
	 * @tparam	T	Generic type parameter.
	 * @param [in,out]	data	The data to prepend.
	 *
	 * @returns	A new list with an element prepended.
	 */

	template<typename T>
	inline immutable_list<T> immutable_list<T>::push_front(value_type& data) const
	{
		return this->push_front_impl(data);
	}

	/*!
	 * @overload
	 */

	template<typename T>
	inline immutable_list<T> immutable_list<T>::push_front(value_type&& data) const
	{
		return this->push_front_impl(std::move(data));
	}

	template<typename T>
	template<typename U>
	inline immutable_list<T> immutable_list<T>::push_front_impl(U && data) const
	{
		immutable_list<T> newList{ std::forward<U>(data) };

		newList.head->next = this->head;
		newList.tail = this->tail.lock();
		newList.m_size = 1 + this->m_size;

		return newList;
	}

	/*!
	 * @brief Generates a new list with an element prepended to it
	 *
	 * The element is constructed in place trough brace initialization.
	 * Does not make a copy of the whole list.
	 *
	 * @tparam	T		Generic type parameter
	 * @tparam	Args	Variadic type parameter
	 * @param	args	The arguments to pass to the element constructor
	 *
	 * @returns	A new list with an element prepended
	 */

	template <typename T>
	template <typename ...Args>
	inline immutable_list<T> immutable_list<T>::emplace_front(Args&&... args) const {
		// TODO: check if there is a sense in trying a variadic emplace constructor
		immutable_list<T> newList{ T{std::forward<Args>(args)...} };

		newList.head->next = this->head;
		newList.tail = this->tail.lock();
		newList.m_size = 1 + this->m_size;

		return newList;
	}

	/*!
	 * @brief	Generates a new list with an the front element removed
	 * 			
	 * Does not need to copy the list.
	 *
	 * @tparam	T	Generic type parameter.
	 *
	 * @returns	A new list with the front element removed
	 */

	template<typename T>
	inline immutable_list<T> immutable_list<T>::pop_front() const
	{
		immutable_list<T> newList{};
		newList.head = this->head->next;
		newList.m_size = 1 - this->m_size;

		return newList;
	}

	/*!
	 * @brief	Generates a new list with one or more elements inserted after the given position
	 * 			
	 * - inserts value after the element pointed to by pos  
	 * - same as above
	 * - inserts count copies of the value after the element pointed to by pos
     * - inserts elements from range [first, last) after the element pointed to by pos.
     * - inserts elements from initializer list list.
	 *
	 * Copies of the elements in the range [begin, pos] and (pos, end] are made.
	 * 
	 * @tparam	T	Generic type parameter.
	 * @param	pos  	The position after which the new elements should be inserted. Must be a valid iterator in the range [begin, end) of this list.
	 * @param	value	The data to insert in the new elements
	 * @param	count	The number of elements to insert in the new list
	 * @param	first,last	The range of elements to insert
	 * @param	list	initializer_list to insert the values from
	 *
	 * @returns	A new list with one or more elements inserted after the given position
	 */

	template<typename T>
	inline immutable_list<T> immutable_list<T>::insert_after(const_iterator pos, const value_type& value) const
	{
		return this->insert_after_impl(pos, 1, value, std::true_type());
	}

	/*!
	 * @overload
	 */

	template<typename T>
	inline immutable_list<T> immutable_list<T>::insert_after(const_iterator pos, value_type&& value) const
	{
		return this->insert_after_impl(pos, 1, std::move(value), std::true_type());
	}

	/*!
	 * @overload
	 */

	template<typename T>
	inline immutable_list<T> immutable_list<T>::insert_after(const_iterator pos, size_type count, const value_type & value) const
	{
		return this->insert_after_impl(pos, count, value, std::true_type());
	}

	/*!
	 * @overload
	 */

	template<typename T>
	template<typename InputIterator>
	inline immutable_list<T> immutable_list<T>::insert_after(const_iterator pos, InputIterator first, InputIterator last) const
	{
		return this->insert_after_impl(pos, first, last, std::false_type());
	}

	/*!
	 * @overload
	 */

	template<typename T>
	inline immutable_list<T> immutable_list<T>::insert_after(const_iterator pos, std::initializer_list<T> list) const
	{
		return this->insert_after_impl(pos, list.begin(), list.end(), std::false_type());
	}

	template<typename T>
	template<typename U>
	inline immutable_list<T> immutable_list<T>::insert_after_impl(const_iterator pos, size_type count, U&& value, std::true_type) const
	{
		// TODO: Benchmark a flattened out solution to remove the creation and destruction of the unused sentinel node
		immutable_list<T> newList{ this->cbegin(), ++pos };
		auto lastNode{ newList.iteratorAt(newList.m_size - 1).node.lock() };

		// Inserts the new elements
		for (size_type index{ 0 }; index < count; ++index) {
			lastNode->next = std::make_shared<Node>(std::forward<U>(value) );
			lastNode = lastNode->next;
		}
		lastNode->next = pos.node.lock();

		// Connects back to the original list
		newList.tail = this->tail;
		newList.m_size += count + std::distance(pos, this->cend());
		
		return newList;
	}

	template<typename T>
	template<typename InputIterator>
	inline immutable_list<T> immutable_list<T>::insert_after_impl(const_iterator pos, InputIterator first, InputIterator last, std::false_type) const
	{
		// TODO: Benchmark a flattened out solution to remove the creation and destruction of the unused sentinel node
		immutable_list<T> newList{ this->cbegin(), ++pos };
		auto lastNode{ newList.iteratorAt(newList.m_size - 1).node.lock() };

		// Inserts the new elements
		newList.m_size += std::distance(pos, this->cend()) + std::distance(first, last);
		while(first != last) {
			lastNode->next = std::make_shared<Node>(*first);
			lastNode = lastNode->next;

			++first;
		}
		
		// Connects back to the orignal list
		lastNode->next = pos.node.lock();
		newList.tail = this->tail;

		return newList;
	}

	/*!
	 * @brief Inserts a new element after the position pos
	 * 		 
	 * The element is constructed in place trough brace initialization.
	 * 
	 * @tparam	T		Generic type parameter
	 * @tparam	Args	Variadic type parameter
	 * @param	pos		The position after which the new elements should be inserted. Must be a valid iterator in the range [begin, end) of this list. 
	 * @param	args	The arguments to pass to the element constructor
	 * 					
	 * @returns	A new list with one element inserted after the given position
	 */

	template<typename T>
	template<class ...Args>
	inline immutable_list<T> immutable_list<T>::emplace_after(const_iterator pos, Args && ...args) const
	{
		auto newList{ immutable_list<T>(this->cbegin(), ++pos) };
		auto lastNode{ newList.iteratorAt(newList.m_size - 1).node.lock() };

		lastNode->next = std::make_shared<Node>(T{ std::forward<Args>(args)... });
		lastNode->next->next = pos.node.lock();

		newList.m_size += std::distance(pos, this->cend()) + 1;

		return newList;
	}
	 
	/*!
	 * @brief	Checks if the list is empty
	 *
	 * @tparam	T	Generic type parameter.
	 *
	 * @returns	true if the list is empty, false otherwise.
	 */

	template<typename T>
	inline bool immutable_list<T>::empty() const noexcept
	{
		return !this->m_size;
	}

	/*!
	 * @brief	Gets the current number of elements in the list
	 *
	 * @tparam	T	Generic type parameter.
	 *
	 * @returns	The number of elements in the list
	 */

	template<typename T>
	typename inline immutable_list<T>::size_type immutable_list<T>::size() const noexcept
	{
		return this->m_size;
	}

	/*!
	 * @brief	Gets the theorethical maximum size of a list
	 *
	 * Generally, some kind of failure is expected before reaching the theoretical, implementation, limit ( e.g running out of memory )
	 * 
	 * @tparam	T	Generic type parameter.
	 *
	 * @returns	Maximum number of elements.
	 */

	template<typename T>
	typename inline constexpr immutable_list<T>::size_type immutable_list<T>::max_size() const noexcept
	{
		return std::numeric_limits<size_type>::max();
	}
	
	template<typename T>
	typename inline immutable_list<T>::const_iterator immutable_list<T>::iteratorAt(size_type index) const
	{
		return std::next(this->cbegin(), index);
	}

	// FRIEND FUNCTIONS

	/*!
	 * @brief	Equality operator
	 * 			
	 * @returns	true if the lists are equal, false otherwise
	 */

	template<typename T>
	bool operator==(const immutable_list<T>& left, const immutable_list<T>& right)
	{
		// TODO: Benchmark to see if in a tight loop preemptively exiting if the lists are of different sizes improves performance
		return std::equal(left.cbegin(), left.cend(), right.cbegin(), right.cend());
	}

	/*!
	 * @brief	Inequality operator
	 *
	 * @returns	true if !(left == right), false otherwise
	 */

	template<typename T>
	bool operator!=(const immutable_list<T>& left, const immutable_list<T>& right)
	{
		return !(left == right);
	}

	// IMMUTABLE_LIST_ITERATOR IMPLEMENTATION //

	template<typename T>
	inline bool operator==(const immutable_list_iterator<T>& left, const immutable_list_iterator<T>& right) noexcept
	{
		return left.node.lock() == right.node.lock();
	}

	template<typename T>
	inline bool operator!=(const immutable_list_iterator<T>& left, const immutable_list_iterator<T>& right) noexcept
	{
		return !(left == right);
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