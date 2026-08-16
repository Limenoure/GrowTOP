#ifndef NETWORK__COMMAND__HPP
#define NETWORK__COMMAND__HPP

#include <vector>
#include <string>
#include <functional>

#include <Network/CommandHeader.hpp>

class Command
{
public:
	Command(const std::string& name, const std::vector<std::string>& aliases = {}) : m_name(name)
	{
		m_aliases = aliases;
	}

	~Command()
	{
		m_aliases.clear();
	}

	std::string GetName() const { return m_name; }

	std::vector<std::string> GetAliases() const { return m_aliases; }

	std::string GetDescription() const { return m_description; }
	std::string SetDescription(const std::string& description) { return m_description = description; }

	int GetPermission() const { return m_role; }
	void SetPermission(const int& permission) { m_role = permission; }

	std::function<void(const CommandHeader&)> GetFunc() const { return m_func; }
	void SetFunc(std::function<void(const CommandHeader&)> func) { m_func = func; }

private:
	std::string m_name;
	std::vector<std::string> m_aliases;
	std::string m_description;
	int m_role = 0;

	std::function<void(const CommandHeader&)> m_func;

};

#endif // NETWORK__COMMAND__HPP