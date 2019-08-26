#pragma once

namespace StringConstants {
constexpr char ErrRecvSoc[] = "could not receive a message from socket";
constexpr char ErrSendSoc[] = "could not send a message on a socket";
constexpr char ErrCloseSoc[] =
    "could not close client socket for connection.";
constexpr char QuitCmd[] = "quit";
constexpr char RecvQuitCmd[] = "received quit command from connection ";
constexpr char AttemptClose[] = ". attempting to close connection.";

constexpr char ErrCreateSoc[] = "Could not create socket";
constexpr char ErrBindSoc[] = "Could not bind listening socket to network address";
constexpr char ErrListenSoc[] = "Could not mark socket for listening";
constexpr char ErrAcceptCon[] = "Could not accept a connection on a socket";

}  // namespace string_constants
