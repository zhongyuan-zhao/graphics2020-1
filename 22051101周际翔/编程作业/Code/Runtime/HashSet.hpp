// Copyright 2018-2021 JXMaster. All rights reserved.
/*
* @file HashSet.hpp
* @author JXMaster
* @date 2020/2/14
*/
#pragma once
#include "Source/HashTable.hpp"

namespace Luna
{
	template <
		typename _Kty,
		typename _Hash = hash<_Kty>,		// Used to hash the key value.
		typename _KeyEqual = equal_to<_Kty>>	// Used to compare the element.
	class HashSet
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
		using insert_return_type = iterator;

	private:

		using table_type = Impl::HashTable<key_type, value_type, Impl::SetExtractKey<key_type, value_type>, hasher, key_equal>;

		table_type m_base;

		HashSet(table_type&& base) :
			m_base(move(base)) {}

	public:

		HashSet() :
			m_base() {}
		HashSet(const HashSet& rhs) :
			m_base(rhs.m_base) {}
		HashSet(HashSet&& rhs) :
			m_base(move(rhs.m_base)) {}
		HashSet& operator=(const HashSet& rhs)
		{
			m_base = rhs.m_base;
			return *this;
		}
		HashSet& operator=(HashSet&& rhs)
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
		Pair<iterator, bool> insert(const value_type& value)
		{
			return m_base.insert(value);
		}
		Pair<iterator, bool> insert(value_type&& value)
		{
			return m_base.insert(move(value));
		}
		insert_return_type insert(node_type&& node)
		{
			return m_base.insert(move(node));
		}
		template <typename _M>
		Pair<iterator, bool> insert_or_assign(const key_type& key, _M&& value)
		{
            return m_base.template insert_or_assign<_M>(key, forward<_M>(value));
		}
		template <typename _M>
		Pair<iterator, bool> insert_or_assign(key_type&& key, _M&& value)
		{
            return m_base.template insert_or_assign<_M>(move(key), forward<_M>(value));
		}
		template <typename... _Args>
		Pair<iterator, bool> emplace(_Args&&... args)
		{
			return m_base.emplace(forward<_Args>(args)...);
		}
		iterator erase(const_iterator pos)
		{
			return m_base.erase(pos);
		}
		usize erase(const key_type& key)
		{
			return m_base.erase(key);
		}
		void swap(HashSet& rhs)
		{
			HashSet tmp(move(rhs));
			rhs = move(*this);
			*this = move(tmp);
		}
		node_type extract(const_iterator pos)
		{
			return m_base.extract(pos);
		}
	};
}
