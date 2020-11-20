#pragma once
#include <vector>
#include "ContainerRequirements.h"
#include "plf_colony.h"
#include <concepts>
#include <list>

//TODO: Several data types? can go with tuple for now tho but may want SoA
template<typename TObjectType, ContainerRequirement ... TContainerRequirements>
class Container
{
public:
	template <typename T>
	constexpr static bool HasRequirement = ContainsRequirement<T, TContainerRequirements...>;
	constexpr static bool Iterable = HasRequirement<Iterable>;
	constexpr static bool Indexable = HasRequirement<Indexable> || HasRequirement<RandomAccess>;
	constexpr static bool RandomAccess = HasRequirement<RandomAccess>;
	constexpr static bool StableElements = HasRequirement<StableElements>;
	constexpr static bool ConstantInsertion = HasRequirement<ConstantInsertion>;
	constexpr static bool ConstantDeletion = HasRequirement<ConstantDeletion>;

	//TODO: Change how this is handled? Make Storage class? either way this is not optimal
	static auto StorageType()
	{
		if constexpr (!RandomAccess && !ConstantInsertion)
			return plf::colony<TObjectType>();
		else if constexpr (!StableElements && !ConstantInsertion && !ConstantDeletion)
			return std::vector<TObjectType>();
		else if constexpr (!RandomAccess && !Indexable)
			return std::list<TObjectType>();
		else
			static_assert(false, "No container backend supports these requirements!");
	}

	using Storage = decltype(StorageType());

	template <typename T>
	constexpr static bool UsesStorage = std::is_same_v<Storage, T>;

	auto Add(TObjectType aObject)
	{
		if constexpr (UsesStorage<plf::colony<TObjectType>>)
		{
			return myBackend.insert(aObject);
		}
		else
		{
			return myBackend.push_back(aObject);
		}
	}

	constexpr TObjectType& operator[](const size_t aIndex) requires Indexable
	{
		if constexpr (UsesStorage<plf::colony<TObjectType>>)
		{
			return *myBackend.get_iterator_from_index(aIndex);
		}
		else
		{
			return myBackend[aIndex];
		}
	}

	constexpr const TObjectType& operator[](const size_t aIndex) const requires Indexable
	{
		if constexpr (UsesStorage<plf::colony<TObjectType>>)
		{
			return *myBackend.get_iterator_from_index(aIndex);
		}
		else
		{
			return myBackend[aIndex];
		}
	}

	void Sort() requires Iterable && RandomAccess
	{
		std::sort(begin(), end());
	}
	
	auto begin() const requires Iterable
	{
		return myBackend.begin();
	}

	auto end() const requires Iterable
	{
		return myBackend.end();
	}

	auto begin() requires Iterable
	{
		return myBackend.begin();
	}

	auto end() requires Iterable
	{
		return myBackend.end();
	}

	Storage myBackend;
};