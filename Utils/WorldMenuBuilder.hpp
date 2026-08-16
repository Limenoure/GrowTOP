#ifndef UTILS__WORLDMENUUTILS__HPP
#define UTILS__WORLDMENUUTILS__HPP

#include <string>
#include <format>

class WorldMenuBuilder
{
public:
	WorldMenuBuilder() : m_elements("") {}
	~WorldMenuBuilder() = default;

	std::string Get() const { return m_elements; }

	WorldMenuBuilder set_default(const std::string& worldName)
	{
		m_elements += std::format("default|{}\n", worldName);
		return *this;
	}

    WorldMenuBuilder add_floater(const std::string& text, const int& players, const double& scale, const uint32_t& color) 
    {
        m_elements += std::format("add_floater|{}|{}|{}|{}\n", text, players, scale, color);
        return *this;
    }

    WorldMenuBuilder add_button(const std::string& text, const std::string& worldName, const double& scale, const uint32_t& color) 
    {
        m_elements += std::format("add_button|{}|{}|{}|{}\n", text, worldName, scale, color);
        return *this;
    }

    WorldMenuBuilder add_spacer() 
    {
        m_elements += "add_spacer|\n";
        return *this;
    }

    WorldMenuBuilder add_heading(const std::string& text) 
    {
        m_elements += std::format("add_heading|{}\n", text);
        return *this;
    }

    WorldMenuBuilder add_filter() 
    {
        m_elements += "add_filter\n";
        return *this;
    }

    WorldMenuBuilder set_max_rows(const int& rowCount) 
    {
        m_elements += std::format("set_max_rows|{}\n", rowCount);
        return *this;
    }

    WorldMenuBuilder setup_simple_menu() 
    {
        m_elements += "setup_simple_menu\n";
        return *this;
    }

private:
	std::string m_elements = "";

};

#endif // UTILS__TIMEUTILS__HPP