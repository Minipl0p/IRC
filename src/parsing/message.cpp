
#include "Message.hpp"

Message tokenizeLine(const std::string &line)
{
	Message msg;
	size_t	i = 0;

	// if start by : it's a prefix that indicate who send it. we don't use this but have to skip it
	// if exist
	if (i < line.size() && line[i] == ':') {
		size_t end = line.find(' ', i);
		if (end == std::string::npos)
			return msg;
		msg.prefix = line.substr(1, end - 1);
		i		   = end;
	}

	// skip space
	while (i < line.size() && line[i] == ' ')
		i++;

	// the next word is the command
	size_t end = line.find(' ', i);
	if (end == std::string::npos) {
		msg.command = line.substr(i);
		return msg;
	}
	msg.command = line.substr(i, end - i);
	i			= end;

	// find and store param or trailling
	while (i < line.size()) {
		while (i < line.size() && line[i] == ' ')
			i++;
		if (i >= line.size())
			break;

		// if trailling
		if (line[i] == ':') {
			msg.params.push_back(line.substr(i + 1));
			break;
		}

		// if no trailling
		end = line.find(' ', i);
		if (end == std::string::npos) {
			msg.params.push_back(line.substr(i));
			break;
		}
		msg.params.push_back(line.substr(i, end - i));
		i = end;
	}
	return msg;
}
