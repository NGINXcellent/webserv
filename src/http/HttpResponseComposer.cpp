/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponseComposer.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfarias- <lfarias-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/05 21:21:24 by lfarias-          #+#    #+#             */
/*   Updated: 2023/09/02 15:14:16 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/http/HttpResponseComposer.hpp"
#include "../../include/http/HttpStatus.hpp"
#include "../../include/http/HttpTime.hpp"
#include "../../include/http/MimeType.hpp"
#include "../../include/io/FileReader.hpp"

#include <dirent.h>
#include <sstream>
#include <fstream>
#include <iostream>
#include <sys/stat.h>
#include <unistd.h>

void HttpResponseComposer::buildErrorResponse(HttpResponse *response, \
                                       int error_code, \
                                       std::map<int, std::string> error_pages, \
                                       int protoMainVersion, \
                                       int protoSubVersion) {
  // shim
  response->setProtocol("HTTP", protoMainVersion, protoSubVersion);
  response->setStatusCode(error_code);

  if (HttpResponseComposer::getCustomPage(response, error_code, error_pages))
    return;

  std::string content = "<html><head><title>";
  std::stringstream ss;
  ss << error_code;
  std::string errorMsg = HttpStatus::getMessage(error_code);
  std::cout << errorMsg << std::endl;

  content += ss.str() + " " + errorMsg + "</title></head>";
  content += "<body><center><h1>" + ss.str() + " " + errorMsg + \
    "</h1></center></body>";
  content += "<hr><center>webserv/0.1</center></body></html>";

  char *responseContent = new char[content.size()];

  for (size_t i = 0; i < content.size(); i++) {
    responseContent[i] = content[i];
  }

  response->setContentType("text/html");
  response->setMsgBody(responseContent);
  response->setContentLength(content.size());
}

bool HttpResponseComposer::getCustomPage(HttpResponse *response, \
                                         int error_code, \
                                std::map<int, std::string> error_pages) {
  // shim
  std::map<int, std::string>::iterator it = error_pages.find(error_code);

  if (it == error_pages.end())
    return (false);

  std::string filename = error_pages[error_code];
  char *resourceData;
  long long resourceSize;
  int readStatus = FileReader::getContent(filename, &resourceData, &resourceSize);

  if (readStatus != 0) {
    std::cout << "[ERROR]\tCan't find or read custom error page:";
    std::cout << filename << std::endl;
    return (false);
  }

  response->setContentType(MimeType::identify(filename));
  response->setMsgBody(resourceData);
  response->setContentLength(resourceSize);
  return (true);
}

// This should die in a fire, but it does produce a pretty html page.
// "but why didn't you make every line 80 columns wide?"
// answer: because I'm not a masochist
void HttpResponseComposer::formatDirListStyle(const std::string &path, \
                                              std::string &site_style) {
 site_style += "<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"UTF-8\"><meta name=\"viewport\" content=\"width=device-width,initial-scale=1\">";
 site_style += "<title>Index of " + path + "</title>";
 site_style += "<style>body,li,ul{margin:0;padding:0}body{font-family:Helvetica,sans-serif;background-color:#121212;color:#e0e0e0}header{background-color:#1e1e1e;color:#fff;padding:1em;text-align:center}main{margin:2em auto;max-width:800px;padding:1em;background-color:#1e1e1e;border-radius:5px;box-shadow:0 0 10px rgba(255,255,255,.1)}.file-list{list-style:none;padding:0}.file-list li{border-bottom:1px solid #333;padding:.75em;display:flex;align-items:center;transition:background-color .2s}.file-list a{text-decoration:none;color:#007bff}.file-list li:hover{background-color:#333}</style></head>";
 site_style += "<body><header><h1>Index of " + path + "<h1></header><main><ul class=\"file-list\"><pre>";
}

void HttpResponseComposer::formatEntryName(const std::string &path, \
                                  std::string &responseStr, \
                                  struct file_info *info) {
  std::string entryName = info->fileName;
  std::string icon;
  responseStr += "<li>";

  if (!info->isDir) {
    icon = "&#128190 ";
  } else {
    icon = "&#128193 ";
    entryName += "/";
  }

  if (entryName == "..") {
    size_t pos = path.find_last_of('/');
    std::string parent = path.substr(0, pos);
    responseStr += "<a href=\"" + parent + "\">" + icon + entryName + "</a>";
  } else {
    responseStr += "<a href=\"" + path + "/" + entryName + "\">";
    responseStr += icon + entryName + "</a>";
  }
  
  int whiteSpaces = 50 - entryName.size();

  for (int i = 0; i < whiteSpaces; ++i) {
    responseStr += " ";
  }

  responseStr += HttpTime::fmtModifiedTime(info->lastModified); 

  for (int j = 0; j < 10; ++j) {
    responseStr += " ";
  }

  if (info->isDir) {
    responseStr += "-";
  } else {
    std::stringstream ss;
    ss << info->fileSize;
    responseStr += ss.str();
  }

  responseStr += "</li>";
  delete info;
}

void HttpResponseComposer::buildDirListResponse(HttpRequest *request, HttpResponse *response, \
                                                std::map<std::string, struct file_info *> &entries) {
  std::string path = request->getResource();
  std::string responseStr;
  formatDirListStyle(path, responseStr);

  std::map<std::string, struct file_info *>::iterator it = entries.begin();
  std::map<std::string, struct file_info *>::iterator ite = entries.end();

  for (; it != ite; ++it) {
    formatEntryName(path, responseStr, it->second);
  }

  responseStr += "</pre></ul></main></body></html>";
  char *msg = new char[responseStr.size()];

  for (size_t i = 0; i < responseStr.size(); ++i) {
    msg[i] = responseStr[i];
  }

  int protoMain = request->getProtocolMainVersion();
  int protoSub = request->getProtocolSubVersion();
  response->setProtocol("HTTP", protoMain, protoSub);
  response->setStatusCode(Ok);
  response->setMsgBody(msg);
  response->setContentLength(responseStr.size());
}
