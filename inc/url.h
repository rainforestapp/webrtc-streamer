#include <iostream>
#include <string>
#include <regex>
#pragma once

class Url {
  public:
    Url() {
      this->isEmpty = true;
    }
    Url(std::string url) {
      this->ParseUrl(url);
    }

		void ParseUrl(std::string url) {
      // Official regex specified in https://tools.ietf.org/html/rfc3986#page-50
      std::regex url_regex (
        R"(^(([^:\/?#]+):)?(//([^\/?#]*))?([^?#]*)(\?([^#]*))?(#(.*))?)",
        std::regex::extended
      );
      std::cout << "Parsing URL: " << url << std::endl;
      std::smatch match_result;
      bool matched = std::regex_match(url, match_result, url_regex);
      if (!matched) {
        this->isEmpty = true;
        return;
      }
      this->host = match_result[4];
      std::cout << "Got back host: " << this->host << std::endl;

      int index = this->host.find('@');
      if (index != std::string::npos) {
        std::string auth_credentials = this->host.substr(0, index - 1);
        int colonIndex = auth_credentials.find(':');
        this->username = auth_credentials.substr(0, colonIndex);
        this->password = auth_credentials.substr(colonIndex + 1, auth_credentials.length() - colonIndex);
        std::cout << "Slicing host for index: " << index + 1 << ":" << this->host.length() - index << std::endl;
        this->host = this->host.substr(index + 1, this->host.length() - index);
      } else {
        this->username = "";
        this->password = "";
      }


      int portIndex = this->host.find(':');
      if (portIndex != std::string::npos) {
        this->port = this->host.substr(portIndex + 1, this->host.size() - portIndex);
        this->host = this->host.substr(0, portIndex);
      }


      this->scheme = match_result[2];
      this->path = match_result[5];
      this->query = match_result[7];
      this->isEmpty = false;
      std::cout << "Got back: " << std::endl;
      std::cout << "Username: " << this->username << std::endl;
      std::cout << "Password: " << this->password << std::endl;
      std::cout << "Host: " << this->host << std::endl;
      std::cout << "Port: " << this->port << std::endl;
      std::cout << "Path: " << this->path << std::endl;
      std::cout << "Finished parsing URL: " << this->toString() << std::endl;
    }

    std::string getHostUrl() {
      return host + (port.length() ? (":" + port) : "");
    }

    bool isDomainOf(std::string domain) {
      bool found = true;
      // ensure host ends with the domain
      if (host.size() < domain.size()) {
        found = false;
      }
      else {
        found = host.compare(host.length() - domain.length(), domain.length(), domain) == 0;
      }

      if (!found) {
        std::cout << "Can not find " << domain << " in " << host << std::endl;
      }

      return found;
    }

    std::string toString() {
      if (isEmpty) {
        return "";
      }

      return scheme + "://" 
        + (username.length() || password.length() ? username + ":" + password + "@" : "")
        + host + (port.length() ? (":" + port) : "")
        + "/" + path
        + (query.length() ? "?" + query : "");
    }
	
    std::string username;
    std::string password;
    std::string scheme;
    std::string host;
    std::string path;
    std::string query;
    std::string port;
    bool isEmpty;
};
