#ifndef UTILS__DIALOGUTILS__HPP
#define UTILS__DIALOGUTILS__HPP

#include <string>
#include <format>

class DialogBuilder
{
public:
	DialogBuilder() : m_elements("") {}
	~DialogBuilder() = default;

	std::string Get() const { return m_elements; }

	DialogBuilder set_default_color(const char& c)
	{
		m_elements += std::format("set_default_color|`{}\n", c);
		return *this;
	}

	DialogBuilder add_textbox(const std::string& text, const std::string& direction = "left")
	{
		m_elements += std::format("add_textbox|{}|{}|\n", text, direction);
		return *this;
	}

	DialogBuilder add_smalltext(const std::string& text, const std::string& direction = "left")
	{
		m_elements += std::format("add_smalltext|{}|{}|\n", text, direction);
		return *this;
	}

	DialogBuilder add_label(const std::string& text, const std::string& size = "small", const std::string& direction = "left")
	{
		m_elements += std::format("add_label|{}|{}|{}|\n", text, size, direction);
		return *this;
	}

	DialogBuilder embed_data(const std::string& key, const std::string& val)
	{
		m_elements += std::format("embed_data|{}|{}\n", key, val);
		return *this;
	}

private:
	std::string m_elements = "";
};

#endif // UTILS__TIMEUTILS__HPP