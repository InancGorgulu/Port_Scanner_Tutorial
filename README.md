# Port Scanner Tutorial

Port Scanner Tutorial is a beginner-friendly TCP port scanner built with **C++**. It uses the **Winsock API** to check whether specified ports on a given IP address are open or closed.

This project is designed as a **tutorial** for those who want to learn how to work with sockets, handle port ranges, and implement network scanning logic in C++.

> ⚠️ **Note:** This implementation is intended for **localhost (127.0.0.1)** use only. It is not designed or tested for scanning external IP addresses or remote hosts.

## 📌 Overview

This repository provides a step-by-step implementation of a **basic TCP port scanner** that:

- Prompts the user to enter a **target IP address** and **port or port range**
- Initializes the **Winsock** library
- Parses and validates **port ranges** (single, dash-separated, or comma-separated)
- Tries to connect to each port using **TCP socket**
- Reports whether each port is **OPEN** or **CLOSED**
- Handles errors gracefully and displays meaningful messages

The goal of this tutorial is to help beginners learn **TCP connection checking**, **port parsing**, and **Winsock API basics** using C++.

## ⚙️ Prerequisites

Before compiling or running this project, make sure you have the following:

- ✅ **Windows operating system** (required for Winsock)
- ✅ **C++ compiler** (MSVC, MinGW, or Clang with Windows support)
- ✅ **Basic understanding of networking concepts** (IP addresses, TCP ports)

## 📝 Notes

- 📌 This scanner is designed **only for local testing on localhost (127.0.0.1)**.
- ⚠️ Scanning external hosts is not supported or tested in this tutorial.
- 🧪 You can experiment with known open ports.
  * For example, you can quickly start a simple HTTP server using Python for testing: _python -m http.server 8080_)
- 💡 The code can be expanded to support **multithreaded scanning** or **remote hosts** in future versions.

## 📜 License

This project is licensed under the **MIT License** – feel free to use, modify, and share it for educational purposes.

## 📬 Contact

For questions or further assistance, feel free to open an issue on GitHub or contact with me.






