
#include "00_Client.hpp"

Client::Client(int fd) : _fd(fd), _authenticated(false), _passOk(false) {}

Client::~Client() {}

int Client::getFd() const { return _fd; }

const std::string &Client::getNickname() const { return _nickname; }

const std::string &Client::getUsername() const { return _username; }

const std::string &Client::getFullname() const { return _fullname; }

bool Client::isAuthenticated() const { return _authenticated; }

bool Client::isPassOk() const { return _passOk; }

std::string &Client::getReadBuffer() { return _readBuffer; }

void Client::setNickname(const std::string &nick) { _nickname = nick; }

void Client::setUsername(const std::string &user) { _username = user; }

void Client::setFullname(const std::string &name) { _fullname = name; }

void Client::setAuthenticated(bool value) { _authenticated = value; }

void Client::setPassOk(bool value) { _passOk = value; }
