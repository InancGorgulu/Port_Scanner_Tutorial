#ifndef PORT_SCANNER_H
#define PORT_SCANNER_H

#include <string>
#include <vector>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

// The port_scanner class encapsulates functionality for scanning TCP ports for a given IP address.
class port_scanner
{
private:
	std::string addr_input;     // Input IP address as a string.
	std::string port_input;     // Input port or port range as a string.
	std::vector<uint16_t> port_list; // List of ports to scan.
	sockaddr_in server;         // Server address structure.
	WSADATA wsa_data;           // Winsock data structure.

    // Helper function to parse the port string and populate port_list.
    void parse_ports_list();

    // Converts a string to an integer and validates the conversion.
    int string_to_int(const std::string& str);

    // Splits a string by a given delimiter and returns a vector of non-empty tokens.
    std::vector<std::string> split(const std::string& str, const char delim);

    // Creates a vector containing all port numbers in the inclusive range [min, max].
    std::vector<uint16_t> create_port_list(int min, int max);

    // Checks whether a single port is open by attempting a TCP connection.
    bool is_port_open(uint16_t port);

public:
    // Initiates a port scan on the specified IP address and port or port range.
    void scan();

    // Accepts an IP address and a port (or port range) as strings.
    port_scanner(const std::string& addr_str, const std::string& port_str);

    // Ensures that Winsock resources are properly cleaned up.
    ~port_scanner();
};

#endif
