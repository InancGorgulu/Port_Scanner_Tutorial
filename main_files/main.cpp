#include <iostream>
#include <sstream>
#include "../header_files/port_scanner.h"

int main()
{
    std::string addr_input{};
    std::string port_input{};

    // Prompt the user for the target IP address.
    std::cout << "Please enter the IP Address: " << std::flush;
    std::getline(std::cin, addr_input);

    if (addr_input.empty())
    {
        std::cerr << "IP address cannot be empty. Exiting.\n";
        return 1;
    }

    // Prompt the user for the port or port range.
    std::cout << "Please enter the Port interval (e.g., 80,443 or 1000-1020): " << std::flush;
    std::getline(std::cin, port_input);

    if (port_input.empty())
    {
        std::cerr << "Port input cannot be empty. Exiting.\n";
        return 1;
    }

    try
    {
        // Create a port_scanner object with the provided IP address and port(s).
        port_scanner scanner_obj{addr_input, port_input};
        // Begin the scanning process.
        scanner_obj.scan();
    }
    catch (const std::exception& ex)
    {
        // If any error occurs during scanning, output the error message.
        std::cerr << "ERROR: (" << ex.what() << ")\n";
        return 1;
    }

    return 0;
}
