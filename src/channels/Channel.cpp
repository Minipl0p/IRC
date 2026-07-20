
#include "Channel.hpp"

Channel::Channel(const std::string &name)
	: _name(name), _topic(""), _topicLocked(false), _key(""), _userLimit(0), _inviteOnly(false)
{}

Channel::~Channel() {}

const std::string &Channel::getName() const { return _name; }

void Channel::addMember(Client &client) { _members[client.getNickname()] = &client; }

void Channel::removeMember(Client &client)
{
	_members.erase(client.getNickname());
	_moderators.erase(client.getNickname());
}

bool Channel::isMember(Client &client) const
{
	return _members.find(client.getNickname()) != _members.end();
}

bool Channel::isEmptyChannel() const { return _members.empty(); }

size_t Channel::membreCount() const { return _members.size(); }

const std::map<std::string, Client *> &Channel::getMembers() const { return _members; }

void Channel::addModerator(Client &client) { _moderators[client.getNickname()] = &client; }

void Channel::removeModerator(Client &client) { _moderators.erase(client.getNickname()); }

bool Channel::isModerator(Client &client) const
{
	return _moderators.find(client.getNickname()) != _moderators.end();
}

const std::string &Channel::getTopic() const { return _topic; }

void Channel::setTopic(const std::string &topic) { _topic = topic; }

bool Channel::isTopicLocked() const { return _topicLocked; }

void Channel::setTopicLocked(bool locked) { _topicLocked = locked; }

bool Channel::hasKey() const { return _key != ""; }

const std::string &Channel::getKey() const { return _key; }

void Channel::setKey(const std::string &key) { _key = key; }

void Channel::removeKey() { _key = ""; }

bool Channel::hasLimit() const { return _userLimit > 0; }

size_t Channel::getLimit() const { return _userLimit; }

void Channel::setLimit(size_t limit) { _userLimit = limit; }

bool Channel::isInviteOnly() const { return _inviteOnly; }

void Channel::setInviteOnly(bool inviteOnly) { _inviteOnly = inviteOnly; }

void Channel::addInviteMember(Client &client) { _invited[client.getNickname()] = &client; }

bool Channel::isInvited(Client &client) const
{
	return _invited.find(client.getNickname()) != _invited.end();
}

bool Channel::keyIsValid(const std::string &key)
{
	if (key == _key)
		return true;
	return false;
}
