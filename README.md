*This project has been created as part of the 42 curriculum by rcompain, pchazalm.*

# ft_irc

## Description

`ft_irc` is a custom IRC (Internet Relay Chat) server written from scratch in
C++98. The goal of the project is to understand and implement the low-level
network mechanisms behind a real-time chat protocol: non-blocking sockets,
a single-threaded event loop with `poll()`, and the parsing/handling of the
IRC protocol as described in RFC 1459 / RFC 2812.

`ircserv` handles multiple clients simultaneously, without ever forking or
using threads, and is compatible with real IRC clients such as irssi, HexChat
or WeeChat. It supports:

- Password-protected connections
- Nicknames & usernames registration
- Channels: join, part, topic, invite, kick, quit, mode, privmsg
- Channel modes: `i` (invite-only), `t` (topic protection), `k` (key),
  `o` (operator), `l` (user limit)
- Private messages between clients and/or channels
- Operator and regular user privileges

## Instructions

### Compilation

```sh
make
```

Other available rules: `make clean`, `make fclean`, `make re`.

### Execution

```sh
./ircserv <port> <password>
```

- `port`: listening port (must be between 1024 and 65535)
- `password`: password required by clients to authenticate

Example:

```sh
./ircserv 6667 mypassword
```

### Connecting a client

```sh
nc -C 127.0.0.1 port mypassword
```

### Project structure

```
src/
├── main.cpp          # entry point, poll loop
├── server/           # Server class, socket setup, commands
├── clients/          # Client class
├── channels/          # Channel class
├── parsing/          # message parsing
├── reply/            # numeric replies (RFC 2812)
└── signals/          # signal handling (graceful shutdown)
```

## Resources

- [RFC 1459 — Internet Relay Chat Protocol](https://datatracker.ietf.org/doc/html/rfc1459)
- [RFC 2812 — IRC Client Protocol](https://datatracker.ietf.org/doc/html/rfc2812)
- [modern IRC documentation (modern.ircdocs.horse)](https://modern.ircdocs.horse/)
- `man 2 poll`, `man 2 socket`, `man 2 bind`, `man 2 listen`, `man 2 accept`
- 42 subject sheet for `ft_irc`

### AI usage

AI (Claude) was used punctually during this project.
