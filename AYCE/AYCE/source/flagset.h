/*****************************************************************************/
/*!
\file   flagset.h
\author Yeongki Baek
\par    email: yeongki.baek\@digipen.edu
\par    GAM300
\par    Pre-Production
\date   07/12/2016
\brief
This is the interface file for flagset features
Copyright 2016, Digipen Institute of Technology
*/
/*****************************************************************************/
#pragma once
#include "precompiled.h"
#include <iostream>
#include <bitset>
#include <cassert>

#define stringify( name ) # name

/*
FlagSet implements a bitset usable with `enum` and `enum class`.

It provide a typesafe interface for manipulating the bitset. This helps
prevents mistake as the various operator and function will refuse a
parameter that doesn't match the expected enum type.

A flagset supports one user-defined enumeration. The number of flags
(ie the member of the user enumeration) is not limited, as the underlying
bitset (std::bitset) can have an arbitrary large size.

The last enumeration member shall be: "LASTFLAG"

*/
namespace VK
{
	template<typename T>
	struct FlagSet
	{
		FlagSet() = default;
		FlagSet(const FlagSet &o) : bitset(o.bitset) {}

		FlagSet &operator|=(const T &val)
		{
			bitset.set(static_cast<size_t>(static_cast<utype>(val)));
			return *this;
		}

		FlagSet &operator&=(const T &val)
		{
			bool tmp = bitset[static_cast<size_t>(static_cast<utype>(val))] ? true : false;
			bitset.reset();
			bitset[static_cast<size_t>(static_cast<utype>(val))] = tmp;
			return *this;
		}

		FlagSet &operator|=(const FlagSet &o)
		{
			bitset |= o.bitset;
			return *this;
		}

		FlagSet &operator&=(const FlagSet &o)
		{
			bool tmp = bitset.test(static_cast<utype>(val));
			bitset.reset();
			bitset.set(static_cast<utype>(val), tmp);
			return *this;
		}

		FlagSet &operator=(const FlagSet &o) = default;
		/**
		* Return a bitset containing the result of the
		* bitwise AND between *this and val.
		*
		* The resulting bitset can contain at most 1 bit.
		*/
		FlagSet operator&(const T&val)
		{
			FlagSet ret(*this);
			ret &= val;

			assert(ret.bitset.count() <= 1);
			return ret;
		}

		/**
		* Perform a AND binary operation between *this and
		* `val` and return the result as a copy.
		*/
		FlagSet operator&(const FlagSet &val)
		{
			FlagSet ret(*this);
			ret.bitset &= val.bitset;

			return ret;
		}

		/**
		* Return a bitset containing the result of the
		* bitwise OR between *this and val.
		*
		* The resulting bitset contains at least 1 bit.
		*/
		FlagSet operator|(const T&val)
		{
			FlagSet ret(*this);
			ret |= val;

			assert(ret.bitset.count() >= 1);
			return ret;
		}

		/**
		* Perform a OR binary operation between *this and
		* `val` and return the result as a copy.
		*/
		FlagSet operator|(const FlagSet &val)
		{
			FlagSet ret(*this);
			ret.bitset |= val.bitset;

			return ret;
		}

		FlagSet operator~()
		{
			FlagSet cp(*this);
			cp.bitset.flip();

			return cp;
		}

		/**
		* The bitset evaluates to true if any bit is set.
		*/
		explicit operator bool() const
		{
			return bitset.any();
		}

		/**
		* Below are the method from std::bitset that we expose.
		*/

		bool operator==(const FlagSet &o) const
		{
			return bitset == o.bitset;
		}

		std::size_t size() const
		{
			return bitset.size();
		}

		std::size_t count() const
		{
			return bitset.count();
		}

		FlagSet &set()
		{
			bitset.set();
			return *this;
		}

		FlagSet &reset()
		{
			bitset.reset();
			return *this;
		}

		FlagSet &flip()
		{
			bitset.flip();
			return *this;
		}

		FlagSet &set(const T &val, bool value = true)
		{
			bitset.set(static_cast<utype>(val), value);
			return *this;
		}

		FlagSet &reset(const T&val)
		{
			bitset.reset(static_cast<utype>(val));
			return *this;
		}

		FlagSet &flip(const T &val)
		{
			bitset.flip(static_cast<utype>(val));
			return *this;
		}

		bool operator[](const T&val)
		{
			return bitset[static_cast<utype>(val)];
		}

		/**
		* Overload for std::ostream
		*/
		friend std::ostream& operator<<(std::ostream& stream, const FlagSet& me)
		{
			return stream << me.bitset;
		}

	private:
		using utype = std::underlying_type_t<T>;
		std::bitset<static_cast<utype>(T::LAST_FLAG)> bitset;
	};
} // namespace VK