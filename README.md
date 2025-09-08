# SysML v2 API MCP-Server

> [!WARNING]
> This project is currently **WORK IN PROGRESS**, has not yet been officially released, and is by no means suitable for productive use!

## About This Project

The Model Context Protocol (MCP) is basically an open protocol for communication between AI models (e.g., ChatGPT, Google Gemini or Claude) and external systems. MCP was officially unveiled by Antropic PBC on November 25, 2024, and released as an open-source project (see https://modelcontextprotocol.io/).

The goal of this project is to develop a MCP server written in C++ that provides AI models and agents with a SysML v2 model as context. For this purpose, the server can access a REST endpoint via http/https that offers resources and methods defined by the [SysML v2 Application Programming Interface (API) & Services](https://www.omg.org/spec/SystemsModelingAPI) specification (You can find the pilot implementation here: https://github.com/Systems-Modeling/SysML-v2-API-Services).

## License

MIT license (© 2025 Stephan Roth)