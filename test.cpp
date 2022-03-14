#include "Serializer.hpp"

#include <string>
#include <stdexcept>
#include <sstream>
#include <iostream>

struct stdStringHelper
{
	std::size_t m_tempCap;
	std::size_t m_tempSize;

	std::string & m_s;

	stdStringHelper(std::string & s) noexcept
		: m_s{ s }
	{
	}

	auto get(std::size_t idx)
	{
		switch (idx)
		{
		case 0:
			this->m_tempCap = this->m_s.capacity();
			return std::make_pair<const void *, std::size_t>(&this->m_tempCap, sizeof(std::size_t));
		case 1:
			this->m_tempSize = this->m_s.size();
			return std::make_pair<const void *, std::size_t>(&this->m_tempSize, sizeof(std::size_t));
		case 2:
			return std::make_pair<const void *, std::size_t>(this->m_s.c_str(), sizeof(char) * this->m_tempSize);
		default:
			throw std::runtime_error("Out of range!");
		}
	}
	auto put(std::size_t idx)
	{
		switch (idx)
		{
		case 0:
			return std::make_pair<void *, std::size_t>(&this->m_tempCap, sizeof(std::size_t));
		case 1:
			this->m_s.reserve(this->m_tempCap);
			return std::make_pair<void *, std::size_t>(&this->m_tempSize, sizeof(std::size_t));
		case 2:
			this->m_s.resize(this->m_tempSize);
			return std::make_pair<void *, std::size_t>(this->m_s.data(), sizeof(char) * this->m_tempSize);
		default:
			throw std::runtime_error("Out of range!");
		}
	}
};

char tohex(char num)
{
	if (num >= 0 && num <= 9)
	{
		return num + '0';
	}
	else if (num >= 10 && num <= 15)
	{
		return num - 10 + 'A';
	}
	else
	{
		throw std::runtime_error("Out of range!");
	}
}

void printBytes(const std::string & str)
{
	for (std::size_t i = 0; i < str.size(); ++i)
	{
		if ((i % 8) == 0)
		{
			std::cout << '\n';
		}
		std::cout << tohex((str[i] >> 4) & 0x0F) << tohex(str[i] & 0x0F) << ' ';
	}

	std::cout << '\n';
}

using stdStringSerializer = slzr::Serializer<std::string, 3, stdStringHelper>;

int main(int argc, char ** argv)
{
	if (argc < 2)
	{
		std::cerr << "Need at least 1 extra argument!" << std::endl;
		return 1;
	}

	std::string str{ argv[1] };
	std::cout << "String contents before serializing: " << str << std::endl;

	stdStringSerializer ser{ str };

	std::stringstream ss;
	ser.serialize(ss);

	std::cout << "Serialized bytes:\n";
	printBytes(ss.str());

	ss.seekg(0);
	std::cout << "Moved stringstream position back to 0.\n";

	str = "Messed up";
	std::cout << "Changed string to something else.\n";
	str.clear();
	std::cout << "Cleared string.\n";

	std::cout << "Str contents before de-serializing: " << str << std::endl;
	ser.deserialize(ss);

	std::cout << "Str contents after de-serializing: " << str << std::endl;

	return 0;
}

