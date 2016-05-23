#pragma once

namespace ctti
{
	namespace detail
	{
		using size_type = decltype(sizeof(0));
		// From libstdc++.
		// Stores a tuple of indices.  Used by tuple and pair, and by bind() to
		// extract the elements in a tuple.
		template <detail::size_type... _Indexes>
		struct _Index_tuple
		{
		    typedef _Index_tuple<_Indexes..., sizeof...(_Indexes)> __next;
		};
		
		// Builds an _Index_tuple<0, 1, 2, ..., _Num-1>.
		template <detail::size_type _Num>
		struct _Build_index_tuple
		{
		    typedef typename _Build_index_tuple<_Num - 1>::__type::__next __type;
		};
		
		template <>
		struct _Build_index_tuple<0>
		{
		    typedef _Index_tuple<> __type;
		};
		
		template <typename _Tp, _Tp... _Idx>
		struct integer_sequence
		{
		    typedef _Tp value_type;
		    static constexpr detail::size_type size()
		    {
		        return sizeof...(_Idx);
		    }
		};
		template <typename _Tp, _Tp _Num, typename _ISeq = typename _Build_index_tuple<_Num>::__type>
		struct _Make_integer_sequence;
		template <typename _Tp, _Tp _Num, detail::size_type... _Idx>
		struct _Make_integer_sequence<_Tp, _Num, _Index_tuple<_Idx...> >
		{
		    static_assert(_Num >= 0, "Cannot make integer sequence of negative length");
		    typedef integer_sequence<_Tp, static_cast<_Tp>(_Idx)...> __type;
		};
		/// Alias template make_integer_sequence
		template <typename _Tp, _Tp _Num>
		using make_integer_sequence = typename _Make_integer_sequence<_Tp, _Num>::__type;
		/// Alias template index_sequence
		template <detail::size_type... _Idx>
		using index_sequence = integer_sequence<detail::size_type, _Idx...>;
		/// Alias template make_index_sequence
		template <detail::size_type _Num>
		using make_index_sequence = make_integer_sequence<detail::size_type, _Num>;
		/// Alias template index_sequence_for
		template <typename... _Types>
		using index_sequence_for = make_index_sequence<sizeof...(_Types)>;
		
		
		template<typename T, detail::size_type Size>
		struct array;

		template<detail::size_type Begin, detail::size_type End, typename T>
		constexpr array<T, End - Begin> make_array(const T* const arr);

		template<typename T, detail::size_type Size>
		struct array
		{
			template<typename Head, typename... Tail>
			explicit constexpr array(Head head, Tail... tail) :
				data_{head, tail...}
			{}

			template<detail::size_type... Is>
			constexpr array(const T* const arr, index_sequence<Is...>) :
					data_{ arr[Is]... }
			{}

			constexpr const T* data() const
			{
				return &data_[0];
			}

			constexpr T operator[](detail::size_type i) const
			{
				return data_[i];
			}

			constexpr detail::size_type size() const
			{
				return Size;
			}

			template<detail::size_type Begin, detail::size_type End>
			constexpr array<T, End - Begin> subarray() const
			{
				return make_array<Begin, End>(data_);
			}
			/*
			template<typename Lhs, typename Rhs, detail::size_type LhsN, detail::size_type RhsN>
			friend constexpr bool operator==(const array<Lhs,LhsN>& lhs, const array<Rhs,RhsN>& rhs)
			{
				if(LhsN == RhsN)
				{
					for (detail::size_type i = 0; i < LhsN; ++i)
					{
						if (lhs[i] != rhs[i])
							return false;
					}

					return true;
				}
				else
					return false;
			}

			template<typename Lhs, typename Rhs, detail::size_type LhsN, detail::size_type RhsN>
			friend constexpr bool operator!=(const array<Lhs,LhsN>& lhs, const array<Rhs,RhsN>& rhs)
			{
				return !(lhs == rhs);
			}
		    */

		private:
			T data_[Size];
		};

		namespace
		{
			template<typename T, detail::size_type... Is>
			constexpr auto make_array(const T* arr, index_sequence<Is...>)
			{
				return array<T, sizeof...(Is)>{ arr[Is]... };
			}
		}

		template<detail::size_type Begin, detail::size_type End, typename T>
		constexpr array<T, End - Begin> make_array(const T* arr)
		{
			return make_array( arr + Begin, make_index_sequence<End - Begin>{} );
		}

		template<typename T, detail::size_type N>
		constexpr array<T, N> make_array(const T(&arr)[N])
		{
			return make_array(arr, make_index_sequence<N>{});
		}
	}
}