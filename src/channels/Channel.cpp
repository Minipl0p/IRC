
#include "Channel.hpp"

Channel::Channel(const std::string &name)
	: _name(name), _topic(""), _topicLocked(false), _topicSetter(""), _topicTimestamp("0"),
	  _key(""), _userLimit(0), _inviteOnly(false)
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

size_t Channel::memberCount() const { return _members.size(); }

const std::map<std::string, Client *> &Channel::getMembers() const { return _members; }

const std::map<std::string, Client *> &Channel::getModerators() const { return _moderators; }

void Channel::addModerator(Client &client) { _moderators[client.getNickname()] = &client; }

void Channel::removeModerator(Client &client) { _moderators.erase(client.getNickname()); }

bool Channel::isModerator(Client &client) const
{
	return _moderators.find(client.getNickname()) != _moderators.end();
}

const std::string &Channel::getTopic() const { return _topic; }

void Channel::setTopic(const std::string &topic) { _topic = topic; }

std::string Channel::getTopicTimestamp() const { return _topicTimestamp; }

std::string Channel::getTopicSetter() const { return _topicSetter; }

void Channel::setTopicSetter(const std::string &setter) { _topicSetter = setter; }

void Channel::setTopicTimestamp(const std::string &timestamp) { _topicTimestamp = timestamp; }

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

std::string Channel::getMembersList() const
{
	std::string list;
	for (CliIt it = _members.begin(); it != _members.end(); ++it) {
		if (_moderators.count(it->first))
			list += "@";
		list += it->first + " ";
	}
	return list;
}

void Channel::renameMember(const std::string &oldNick, const std::string &newNick)
{
	std::map<std::string, Client *>::iterator it = _members.find(oldNick);
	if (it != _members.end()) {
		Client *c = it->second;
		_members.erase(it);
		_members[newNick] = c;
	}
	std::map<std::string, Client *>::iterator it2 = _moderators.find(oldNick);
	if (it2 != _moderators.end()) {
		Client *c = it2->second;
		_moderators.erase(it2);
		_moderators[newNick] = c;
	}
}

void Channel::removeInvite(Client &client) { _invited.erase(client.getNickname()); }

void Channel::promoteNextModerator()
{
	if (_moderators.empty() && !_members.empty()) {
		Client *newOp					  = _members.begin()->second;
		_moderators[newOp->getNickname()] = newOp;
	}
}
