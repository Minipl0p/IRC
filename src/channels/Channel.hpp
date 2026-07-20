
#pragma once

#include <map>
#include <string>

#include "../clients/Client.hpp"

class Channel
{
  private:
	std::string						_name;
	std::map<std::string, Client *> _members;
	std::map<std::string, Client *> _moderators;
	std::string						_topic;
	bool							_topicLocked;
	std::string						_key;
	size_t							_userLimit;
	bool							_inviteOnly;
	std::map<std::string, Client *> _invited;

  public:
	/* ——— Constructor & Destructor ————————————————————————————————————————— */
	Channel(const std::string &name);
	~Channel();

	/* ——— Identity —————————————————————————————————————————————————————————— */
	const std::string &getName() const;

	/* ——— Members —————————————————————————————————————————————————————————— */
	void								   addMember(Client &client);
	void								   removeMember(Client &client);
	bool								   isMember(Client &client) const;
	bool								   isEmptyChannel() const;
	size_t								   memberCount() const;
	const std::map<std::string, Client *> &getMembers() const;
	void 								   renameMember(const std::string &oldNick, const std::string &newNick);
	std::string 						   getMembersList() const;



	/* ——— Opérators          ——————————————————————————————————————————————— */
	void addModerator(Client &client);
	void removeModerator(Client &client);
	bool isModerator(Client &client) const;

	/* ——— Topic      ——————————————————————————————————————————————————————— */
	const std::string &getTopic() const;
	void			   setTopic(const std::string &topic);
	bool			   isTopicLocked() const;
	void			   setTopicLocked(bool locked);

	/* ——— password               —————————————————————————————————————————— */
	bool			   hasKey() const;
	const std::string &getKey() const;
	void			   setKey(const std::string &key);
	bool			   keyIsValid(const std::string &key);
	void			   removeKey();

	/* ——— Limit members         ——————————————————————————————————————————— */
	bool   hasLimit() const;
	size_t getLimit() const;
	void   setLimit(size_t limit);

	/* ——— Invite         ————————————————————————————————————————————————— */
	bool isInviteOnly() const;
	void setInviteOnly(bool inviteOnly);
	void addInviteMember(Client &client);
	bool isInvited(Client &client) const;
};

typedef std::map<std::string, Channel *>::iterator		ChanIt;
typedef std::map<std::string, Client *>::const_iterator CliIt;
