#include "sender.h"

Sender::Sender()
{

}


void
Sender::SendWolSignals(const std::vector<const std::string *> &macs)
{
    for(auto mac : macs)
    {
        const std::string ether_addr{ GetEther(*mac) };

        socket_handle packet{ socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP) };
//        int packet{ socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP) };

        // Build the message to send.
        //   (6 * 0XFF followed by 16 * destination address.)
        std::string message(6, 0xFF);

        for (size_t i = 0; i < 16; ++i)
            message += ether_addr;

        // Set socket options.
        const int optval{1};

        if (setsockopt(packet.get(), SOL_SOCKET, SO_BROADCAST, &optval, sizeof(optval)) < 0)
            throw std::runtime_error("Failed to set socket options");

        // Set up address
        sockaddr_in addr {};
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = bcast;
        addr.sin_port = htons(port);

        // Send the packet out.
        if (sendto(packet.get(), message.c_str(), message.length(), 0,
            reinterpret_cast<sockaddr *>(&addr), sizeof(addr)) < 0) {
            throw std::runtime_error("Failed to send packet");
            }
//        close(packet);
    }
}


std::string
Sender::GetEther(const std::string& hardware_addr){

    std::string ether_addr;

    for (size_t i = 0; i < hardware_addr.length();)
    {
        // Parse two characters at a time.
        unsigned hex = GetHexFromString(hardware_addr.substr(i, 2));
        i += 2;

        ether_addr += static_cast<char>(hex & 0xFF);

        // We might get a colon here, but it is not required.
        if (hardware_addr[i] == ':')
            ++i;
    }

    if (ether_addr.length() != 6)
        throw std::runtime_error(hardware_addr + " not a valid ether address");

    return ether_addr;
}


unsigned
Sender::GetHexFromString(const std::string& s)
{
    unsigned hex{0};

    for (size_t i = 0; i < s.length(); ++i)
    {
        hex <<= 4;

        if (isdigit(s[i]))
            hex |= s[i] - '0';

        else if (s[i] >= 'a' && s[i] <= 'f')
            hex |= s[i] - 'a' + 10;

        else if (s[i] >= 'A' && s[i] <= 'F')
            hex |= s[i] - 'A' + 10;

        else
            throw std::runtime_error("Failed to parse hexadecimal " + s);
    }
    return hex;
}
