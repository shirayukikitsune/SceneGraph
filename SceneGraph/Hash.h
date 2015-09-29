#pragma once

#include <string>

namespace kitsune {
namespace scenegraph {

	// Adapted from http://stackoverflow.com/a/23684632
	namespace __hash {
		template<class>struct hasher;
		template<>
		struct hasher<std::string> {
			std::size_t constexpr operator()(char const *input)const {
				return *input ?
					static_cast<std::size_t>(*input) + 33LL * (std::size_t)((*this)(input + 1)) :
					5381LL;
			}
			std::size_t operator()(const std::string& str) const {
				return (*this)(str.c_str());
			}
		};
		template<typename T>
		std::size_t constexpr hash(T&& t) {
			return hasher< typename std::decay<T>::type >()(std::forward<T>(t));
		}
		inline namespace literals {
			std::size_t constexpr operator "" _hash(char const * s, const size_t) {
				return hasher<std::string>()(s);
			}
		}
	}

}
}

using namespace kitsune::scenegraph::__hash::literals;
