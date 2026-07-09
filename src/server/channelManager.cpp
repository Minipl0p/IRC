#include "Server.hpp"

void Server::addChannelToLst(Channel &chan) { _lstChannels[chan.getName()] = &chan; }

bool Server::findChannelToLst(Channel &chan) const
{
	return _lstChannels.find(chan.getName()) != _lstChannels.end();
}

void Server::deleteChannelToLst(Channel &chan)
{
	ChanIt it = _lstChannels.find(chan.getName());
	if (it == _lstChannels.end())
		return;

	delete it->second;
	_lstChannels.erase(it);
}
