// Copyright 2018-2021 JXMaster. All rights reserved.
/*
* @file HashMultiSet.hpp
* @author JXMaster
* @date 2020/12/26
*/
#pragma once
#include "Source/HashTable.hpp"

namespace Luna
{
	template <
		typename _Kty,
		typename _Hash = hash<_Kty>,		// Used to hash the key value.
		typename _KeyEqual = equal_to<_Kty>>	// Used to compare the element.
		class HashMultiSet
	{
	public:
		using key_type = _Kty;
		using value_type = _Kty;
		using hasher = _Hash;
		using key_equal = _KeyEqual;
		using reference = value_type&;
		using const_reference = const value_type&;
		using pointer = value_type*;
		using const_pointer = const value_type*;
		using iterator = Impl::Iterator<value_type, false>;
		using const_iterator = Impl::Iterator<value_type, true>;
		using local_iterator = Impl::BucketIterator<value_type, false>;
		using const_local_iterator = Impl::BucketIterator<value_type, true>;
		using node_type = Impl::Node<value_type>;

	private:

		using table_type = Impl::HashTable<key_type, value_type, Impl::SetExtractKey<key_type, value_type>, hasher, key_equal>;

		table_type m_base;

		HashMultiSet(table_type&& base) :
			m_base(move(base)) {}

	public:

		HashMultiSet() :
			m_base() {}
		HashMultiSet(const HashMultiSet& rhs) :
			m_base(rhs.m_base) {}
		HashMultiSet(HashMultiSet&& rhs) :
			m_base(move(rhs.m_base)) {}
		HashMultiSet& operator=(const HashMultiSet& rhs)
		{
			m_base = rhs.m_base;
			return *this;
		}
		HashMultiSet& operator=(HashMultiSet&& rhs)
		{
			m_base = move(rhs.m_base);
			return *this;
		}
	public:

		iterator begin()
		{
			return m_base.begin();
		}
		const_iterator begin() const
		{
			return m_base.begin();
		}
		const_iterator cbegin() const
		{
			return m_base.cbegin();
		}
		iterator end()
		{
			return m_base.end();
		}
		const_iterator end() const
		{
			return m_base.end();
		}
		const_iterator cend() const
		{
			return m_base.cend();
		}

		local_iterator begin(usize n)
		{
			return m_base.begin(n);
		}
		const_local_iterator begin(usize n) const
		{
			return m_base.begin(n);
		}
		const_local_iterator cbegin(usize n) const
		{
			return m_base.cbegin(n);
		}
		local_iterator end(usize n)
		{
			return m_base.end(n);
		}
		const_local_iterator end(usize n) const
		{
			return m_base.end(n);
		}
		const_local_iterator cend(usize n) const
		{
			return m_base.cend(n);
		}
		bool empty() const
		{
			return m_base.empty();
		}
		usize size() const
		{
			return m_base.size();
		}
		usize bucket_count() const
		{
			return m_base.bucket_count();
		}
		usize bucket_size(usize n) const
		{
			return m_base.bucket_size(n);
		}
		usize bucket(const key_type& key) const
		{
			return m_base.bucket(key);
		}
		f32 load_factor() const
		{
			return m_base.load_factor();
		}
		f32 max_load_factor() const
		{
			return m_base.max_load_factor();
		}
		void clear()
		{
			m_base.clear();
		}
		hasher hash_function() const
		{
			return m_base.hash_function();
		}
		key_equal key_eq() const
		{
			return m_base.key_eq();
		}

		void rehash(usize new_buckets_count)
		{
			m_base.rehash(new_buckets_count);
		}
		void reserve(usize new_cap)
		{
			m_base.reserve(new_cap);
		}
		void max_load_factor(f32 ml)
		{
			m_base.max_load_factor(ml);
		}
		iterator find(const key_type& key)
		{
			return m_base.find(key);
		}
		const_iterator find(const key_type& key) const
		{
			return m_base.find(key);
		}
		usize count(const key_type& key) const
		{
			return m_base.count(key);
		}
		Pair<iterator, iterator> equal_range(const key_type& key)
		{
			return m_base.equal_range(key);
		}
		Pair<const_iterator, const_iterator> equal_range(const key_type& key) const
		{
			return m_base.equal_range(key);
		}
		bool contains(const key_type& key) const
		{
			return m_base.contains(key);
		}
		iterator insert(const value_type& value)
		{
			return m_base.multi_insert(value);
		}
		iterator insert(value_type&& value)
		{
			return m_base.multi_insert(move(value));
		}
		iterator insert(node_type&& node)
		{
			return m_base.multi_insert(move(node));
		}
		template <typename... _Args>
		iterator emplace(_Args&&... args)
		{
			return m_base.multi_emplace(forward<_Args>(args)...);
		}

		iterator erase(const_iterator pos)
		{
			return m_base.erase(pos);
		}
		usize erase(const key_type& key)
		{
			return m_base.multi_erase(key);
		}
		void swap(HashMultiSet& rhs)
		{
			HashMultiSet tmp(move(rhs));
			rhs = move(*this);
			*this = move(tmp);
		}
		node_type extract(const_iterator pos)
		{
			return m_base.extract(pos);
		}
	};
}
