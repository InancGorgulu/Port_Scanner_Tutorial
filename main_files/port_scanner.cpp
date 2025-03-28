#include "../header_files/port_scanner.h"
#include <iostream>
#include <sstream>
#include <string>

// Initializes Winsock, configures the server address, and prepares the list of ports for scanning.
port_scanner::port_scanner(const std::string& addr_str, const std::string& port_str)
    : addr_input{addr_str}, port_input{port_str},
        port_list{}, server{}, wsa_data{}
{
    // Initialize the Winsock library with version 2.2.
    // WSAStartup must be called before any other Winsock function is used.
    int wsa_err{ WSAStartup(MAKEWORD(2, 2), &wsa_data) }; 
    if (wsa_err != 0)
    {
        throw std::runtime_error("Winsock initialization failed with error code: " + std::to_string(wsa_err));
    }

    // Set the address family to IPv4.
    server.sin_family = AF_INET; 

    // Convert the IP address from its textual form to binary form.
    // inet_pton returns 1 on success, 0 if the address is invalid, and -1 on error.
    int pton_err{ inet_pton(AF_INET, addr_input.c_str(), &server.sin_addr) };
    if (pton_err == 0)
    {
        throw std::runtime_error("Invalid IP address provided. WSAGetLastError returned: " + std::to_string(WSAGetLastError()));
    }
    else if (pton_err == -1)
    {
        throw std::runtime_error("An error occurred while converting IP notation. WSAGetLastError returned: " + std::to_string(WSAGetLastError()));
    }

    // Parse the port string to create the list of ports to be scanned.
    // The input can be a single port or a range specified with '-' or ','.
    parse_ports_list();
}

// Cleans up Winsock resources.
// It is important to call WSACleanup when done with Winsock to free allocated resources.
port_scanner::~port_scanner() 
{ 
    WSACleanup(); 
}

// Checks whether the specified port is open on the target IP address.
bool port_scanner::is_port_open(uint16_t port)
{
    // Creates a new socket with IPV4 and TCP specifications.
    SOCKET m_socket{socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)};
    if (m_socket == INVALID_SOCKET)
    {
        throw std::runtime_error("Socket initialization failed. WSAGetLastError code: " +
            std::to_string(WSAGetLastError()));
    }
    // Set the target port (convert from host byte order to network byte order).
    server.sin_port = htons(port); 

    // Attempt to establish a TCP connection:
    // - If connect() returns 0, the connection is successful and the port is open.
    // - If connect() returns an error with code WSAECONNREFUSED, the port is closed.
    // - Any other error is considered an unexpected issue and statement will throw error in this case.
    if (connect(m_socket, (sockaddr*)&server, sizeof(server)) == 0) 
    {
        closesocket(m_socket);
        return true;
    }
    else 
    { 
        int wsa_error{WSAGetLastError()};
        closesocket(m_socket);

        if (wsa_error == WSAECONNREFUSED) 
        {
            return false;
        }
        else 
        {
            throw std::runtime_error("connect() error. Error code: " + std::to_string(wsa_error));
        }
    }
}

// Converts a string to an integer, ensuring that the entire string is a valid numeric representation.
// If conversion fails, a runtime_error is thrown.
int port_scanner::string_to_int(const std::string& str)
{
    std::istringstream sstream{str};

    int value{0};

    sstream >> value;
    if (sstream.fail() || !sstream.eof())
    {
        throw std::runtime_error("Conversion failed for string: " + str);
    }

    return value;
}

// Generates a list of port numbers in the inclusive range [min, max] after validating the input.
// If the provided values are out of the valid port range (0-65535), an exception is thrown.
std::vector<uint16_t> port_scanner::create_port_list(int min, int max)
{
    if ((min < 0 || min > 65535) || (max < 0 || max > 65535))
    {
        throw std::runtime_error("Port number value should be between 0-65535.");
    }

    // Swap min and max if necessary, ensuring that min <= max.
    if (min > max)
    {
        std::swap(min, max);
    }
    
    std::vector<uint16_t> values{};
    // Preallocate vector capacity to improve performance when inserting multiple ports.
    values.reserve(max - min + 1);

    for (uint16_t i = min; i <= max; ++i)
    {
        values.push_back(i);
    }

    return values;
}

// Splits the given string using the specified delimiter and returns a vector of non-empty tokens.
std::vector<std::string> port_scanner::split(const std::string& str, const char delim)
{
    std::vector<std::string> tokens{};
    std::istringstream isstream{str};
    std::string token{};

    while (std::getline(isstream, token, delim))
    {
        if (!token.empty())
        {
            tokens.push_back(token);
        }
    }

    return tokens;
}

// Parses the port input string and fills the port_list vector.
// Supports both a single port and a range of ports (using '-' or ',' as delimiters).
void port_scanner::parse_ports_list()
{
    if (port_input.empty())
    {
        throw std::runtime_error("Empty input. Please enter a valid port string.");
    }

    std::vector<uint16_t> ports{};
    char delimiter{'\0'};

    // Determine the delimiter based on the input format.
    if (port_input.find('-') != std::string::npos)
        delimiter = '-';
    else if (port_input.find(',') != std::string::npos)
        delimiter = ',';

    if (delimiter != '\0')
    {
        std::vector<std::string> parts{ split(port_input, delimiter) };
        if (parts.size() != 2)
        {
            throw std::runtime_error("Invalid range format in port string: " + port_input);
        }
        int start = string_to_int(parts[0]);
        int end = string_to_int(parts[1]);

        ports = create_port_list(start, end);
        std::cout << "Scanning ports from " << start << " to " << end << "...\n" << std::flush;
    }
    else
    {
        // If no delimiter is found, treat the input as a single port number.
        int port{ string_to_int(port_input) };

        if (port <= 65535 && port >= 0)
        {
            ports.push_back(port);

            std::cout << "Only one port provided. Checking port: " << port << '\n' << std::flush;
        }
        else
        {
            throw std::runtime_error("Invalid port number: " + std::to_string(port));
        }
    }

    port_list = ports;
}

// Iterates through the list of ports and reports whether each port is open or closed.
void port_scanner::scan()
{
    std::cout << "Initiating port scan...\n" << std::flush;

    // Loop over all ports stored in port_list.
    for (uint16_t port : port_list)
    {
        std::cout << "Port " << port << " : " << std::flush;
        try
        {
            if (is_port_open(port))
                std::cout << "OPEN\n" << std::flush;
            else
                std::cout << "CLOSED\n" << std::flush;
        }
        catch (const std::exception& ex)
        {
            // Report any unexpected errors during the connection attempt.
            std::cout << "ERROR: (" << ex.what() << ")\n" << std::flush;
        }
    }
}
