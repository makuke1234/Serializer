#pragma once

#include <utility>
#include <ostream>
#include <istream>

#if __cplusplus > 201703L

#include <concepts>
namespace slzr
{
	template<typename T, typename U>
	concept HelperConcept = requires (T a, std::size_t b)
	{
		{ a.get(b) } -> std::convertible_to<std::pair<const void *, std::size_t> >;
		{ a.put(b) } -> std::convertible_to<std::pair<void *, std::size_t> >;
	} && std::is_constructible_v<T, U &>;
}

#endif

namespace slzr
{
	template<typename T, std::size_t N, typename Helper>
	#if __cplusplus > 201703L
		requires HelperConcept<Helper, T>
	#endif
	class Serializer
	{
	private:
		Helper m_helper;

	public:
		Serializer(T & obj)
			: m_helper{ obj }
		{
		}

		std::ostream & serialize(std::ostream & out)
		{
			for (std::size_t i = 0; i < N; ++i)
			{
				std::pair<const void *, std::size_t> data = this->m_helper.get(i);
				// Write data to file
				out.write(static_cast<const char *>(data.first), data.second);
			}

			return out;
		}
		std::istream & deserialize(std::istream & in)
		{
			for (std::size_t i = 0; i < N; ++i)
			{
				std::pair<void *, std::size_t> data = this->m_helper.put(i);
				// Read from file
				in.read(static_cast<char *>(data.first), data.second);
			}

			return in;
		}
	};
}
