#ifndef SENDER_H
#define SENDER_H

#include <vector>
#include <string>

#include <arpa/inet.h>
#include <errno.h>
//#include <getopt.h>
#include <stdexcept>
#include <sys/socket.h>
#include <unistd.h>
#include <stdexcept>

#include <iostream>


class Sender
{
public:
    Sender();
    ~Sender();
    void SendWolSignals(const std::vector<const std::string *> &macs);

private:
    const unsigned port{ 60000 };
    const unsigned long bcast{ 0xFFFFFFFF };

    std::string GetEther(const std::string &hardware_addr);
    unsigned GetHexFromString(const std::string &);


    class socket_handle
    {
        public:
            explicit socket_handle(int descriptor) : _descriptor(descriptor) {
                if (_descriptor < 0)
                    throw std::runtime_error("Failed to open socket");
            }
            socket_handle(const socket_handle&) = delete;
            socket_handle& operator=(const socket_handle&) = delete;    // copy construction disabled?

            int get() const {
                return _descriptor;
            }

            ~socket_handle() {
                close(_descriptor);
            }

        private:
            const int _descriptor;
    };

};



#endif // SENDER_H




