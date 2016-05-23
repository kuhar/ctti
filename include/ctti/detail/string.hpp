//
// Created by manu343726 on 4/08/15.
//

#ifndef CTTI_STRING_HPP
#define CTTI_STRING_HPP

#include "hash.hpp"
#include "array.hpp"

namespace ctti
{
    namespace detail
    {
#ifdef CTTI_STRING_MAX_LENGTH
		constexpr std::size_t max_string_length = CTTI_STRING_MAX_LENGTH;
#else
		constexpr std::size_t max_string_length = 256;
#endif
        struct string
        {
            #define CTTI_FORWARD(x) static_cast<decltype(x)&&>(x)
            
            template<typename Head, typename... Tail>
			explicit constexpr string(Head&& head, Tail&&... tail) :
				str_{CTTI_FORWARD(head), CTTI_FORWARD(tail)..., '\0'},
				length_{sizeof...(Tail)}, // Without null-terminator
				hash_{ sid_hash(length_, str_.data()) }
			{}
			
			#undef CTTI_FORWARD

            constexpr hash_t hash() const
            {
                return hash_;
            }

            constexpr const char* c_str() const
            {
                return str_.data();
            }

            constexpr std::size_t length() const
            {
                return length_;
            }

            constexpr std::size_t size() const
            {
                return length();
            }

            constexpr char operator[](std::size_t i) const
            {
                return str_[i];
            }

            template <std::size_t Begin, std::size_t End>
			constexpr string substr() const;

            /*friend std::ostream& operator<<(std::ostream& os, const string& str)
            {
                for (std::size_t i = 0u; i != str.length_; ++i)
                    os << str[i];
                return os;
            }*/

        private:
			ctti::detail::array<char, max_string_length> str_;
            std::size_t length_;
            hash_t hash_;
        };

		namespace
		{
			template<std::size_t... Is>
			constexpr string make_string(const char* str, index_sequence<Is...>)
			{
				return string(str[Is]..., '\0');
			}
		}

        template<std::size_t begin, std::size_t end>
        constexpr string make_string(const char* str)
        {
            static_assert(end -  begin <= max_string_length, "Fatal error: Range exceeds maximum string length");

            return make_string( str + begin, make_index_sequence<end - begin - 1>{} );
        }

        template<std::size_t N>
        constexpr string make_string(const char (&str)[N])
        {
            return make_string<0,N>(str);
        }
		
		template<std::size_t N>
		constexpr string make_string(ctti::detail::array<char,N> arr)
        {
			return make_string<0, N>(arr.data());
        }

		template <std::size_t Begin, std::size_t End>
		constexpr string string::substr() const
		{
			return make_string(str_.subarray<Begin, End>());
		}
    }
}

#endif //CTTI_STRING_HPP
