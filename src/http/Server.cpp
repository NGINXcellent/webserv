/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvargas <dvargas@student.42.rio>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/28 17:22:33 by lfarias-          #+#    #+#             */
/*   Updated: 2023/10/24 18:43:21 by dvargas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/http/Server.hpp"

#include <iostream> // cout
#include <fstream>
#include <sstream>  // stringstream
#include <algorithm>
#include <sys/wait.h>

#include "../../include/http/HttpTime.hpp"
#include "../../include/http/HttpRequestFactory.hpp"
#include "../../include/http/HttpResponseComposer.hpp"
#include "../../include/http/MimeType.hpp"
#include "../../include/io/TcpServerSocket.hpp"
#include "../../include/io/FileSystem.hpp"
#include "../../include/io/FileReader.hpp"
#include "../../include/io/FileWriter.hpp"
#include "../../include/http/HttpStatus.hpp"

Server::Server(const struct s_serverConfig& config) {
  port = strtol(config.port.c_str(), NULL, 0);
  host = config.host;
  server_name = config.server_name;
  srv_max_body_size = config.srv_max_body_size;
  error_pages = config.error_page;
  locations = config.location;
}

Server::~Server(void) {}

HttpStatusCode Server::resolve(HttpRequest *request, HttpResponse *response) {
  std::string uTimestamp = request->getUnmodifiedSinceTimestamp();

  if (!uTimestamp.empty() && \
      HttpTime::isModifiedSince(uTimestamp, request->getResource())) {
    // shim
    return (Precondition_Failed);
  }

  std::string requestMethod = request->getMethod();
  std::vector<std::string> allowedMethods = request->getAllowedMethods();
  std::vector<std::string>::iterator it;
  it = std::find(allowedMethods.begin(), allowedMethods.end(), requestMethod);

  if (!allowedMethods.empty() && it == allowedMethods.end()) {
    return (Method_Not_Allowed);
  }

  HttpStatusCode opStatus = Ready;

  if (requestMethod == "GET")
    opStatus = get(request, response);
  else if (requestMethod == "DELETE")
    opStatus = del(request, response);
  else if (requestMethod == "POST")
    opStatus = post(request, response);
  else
    opStatus = Not_Implemented;

  return (opStatus);
}

void Server::process(std::string &buffer, HttpRequest *req, HttpResponse *res) {
  HttpRequestFactory::setupRequest(req, buffer, locations);
  HttpStatusCode status = HttpRequestFactory::check(req, server_name);
  std::cout << " WILL PRINT CGI HERE ->>>>>>> ";
  std::cout << req->getCGI() << std::endl;

  if (status == Ready) {
    res->setProtocol("HTTP", req->getProtocolMainVersion(),
                                  req->getProtocolSubVersion());
    status = resolve(req, res);
  }

  if (status != Ready) {
    HttpResponseComposer::buildErrorResponse(res, status, error_pages, \
                                          req->getProtocolMainVersion(), \
                                          req->getProtocolSubVersion());
  }
}

void Server::processCgi(HttpRequest *req, int pipe[2], std::vector<std::string> &env_vars) {
  std::cout << "I HAVE RUN!" << std::endl;
  setupCgiVars(req, env_vars);
  std::string method = req->getMethod();
  HttpStatusCode op_code = Ready;

  if (method == "GET") {
    op_code = getCgi(req, env_vars);
  } else if (method == "POST") {
    op_code = postCgi(req, pipe, env_vars);
  }

  (void)op_code;
}

HttpStatusCode Server::post(HttpRequest *request, HttpResponse *response) {
  HttpStatusCode opStatus = Ready;
  response->setLastModifiedTime(
      HttpTime::getLastModifiedTime(request->getResource()));

  // BODY SIZE CHECK
  //  MESMA LOGICA DE CHECAGEM COM STATUS CODE EM REQUEST, VAI FICAR ASSIM ?
  if (request->getRedirectionCode() != 0) {
    response->setStatusCode(request->getRedirectionCode());
    response->setLocation(request->getRedirectionPath());
    return (Ready);
  }

  if (srv_max_body_size != SIZE_T_MAX) {
    if (request->getContentLength() > srv_max_body_size) {
      return (Payload_Too_Large);
    }
  }
  // END BODY SIZE CHECK ---

  PostType pType = request->getPostType();

  // WE NEED TO CHANGE THIS TO A SWITCH AND ENCAPSULATE THE LOGIC

  if (pType == None) {
    throw std::runtime_error("Wrong POST REQUEST, NONE");
  } else if (pType == Chunked || pType == UrlEncoded) {
    std::string location = request->getLocationWithoutIndex();
    if (FileSystem::check(location, F_OK) != 0) {
      response->setStatusCode(No_Content);
    } else {
      response->setStatusCode(Created);
    }

    opStatus = FileWriter::writeToFile(location, request->getRequestBody());
  } else if (pType == Multipart) {
    MultiPartMap multiParts = request->getMultipartMap();
    MultiPartMap::iterator it = multiParts.begin();
    MultiPartMap::iterator ite = multiParts.end();

    for (; it != ite; it++) {
      std::string location = request->getLocationWithoutIndex();
      std::string filename = location + '/' +  it->first + "_fromClient";
      std::cout << filename << std::endl;

      if(FileSystem::check(filename, F_OK) != Ready) {
        response->setStatusCode(Created);
      }
      else {
        response->setStatusCode(No_Content);
      }

      opStatus = FileWriter::writeToFile(filename, it->second);
    }
  }

  if (opStatus != Ready || opStatus != No_Content || opStatus != Created) {
    return opStatus;
  }
  response->setContentType(MimeType::identify(request->getResource()));
  response->setContentLength(0);
  return (Ready);
}

char** buildCharMatrix(const std::vector<std::string> &strList) {
  size_t listSize = strList.size();
  char **char_matrix = new char*[listSize + 1];

  for (size_t i = 0; i < listSize; i++) {
    size_t str_size = strList[i].size();
    char *c_string = new char[str_size + 1];
    std::strcpy(c_string, strList[i].c_str());
    c_string[strList[i].size()] = '\0';
    char_matrix[i] = c_string;
  }

  char_matrix[listSize] = NULL;
  return (char_matrix);
}


bool Server::handleCgiGet(HttpRequest *request) {
  int cgiPipe[2];
  std::string cgiPath = "/home/dvargas/Desktop/webserv/tests/sites/cgi/index.php";
  std::vector<std::string> env_vars;
  std::vector<std::string> cmd_and_args;

  if (pipe(cgiPipe) == -1) {
    std::cerr << "the pipe broke!" << std::endl;
    return false;
  }

  // process cgi
  processCgi(request, cgiPipe, env_vars);
  pid_t child_pid = fork();

  if (child_pid < 0) {
    std::cerr << "error on fork" << std::endl;
    // send a 500 error
  }

  if (child_pid == 0) {
    close(cgiPipe[0]);
    dup2(cgiPipe[1], STDOUT_FILENO);
    close(cgiPipe[1]);
// char* argv[] = {const_cast<char*>("php"), const_cast<char*>(cgiPath.c_str()), NULL};
    cmd_and_args.push_back("php");
    //cmd_and_args.push_back("." + client->getRequest()->getResource());
    cmd_and_args.push_back("/home/dvargas/Desktop/webserv/tests/sites/cgi/index.php");
    char **cmdMatrix = buildCharMatrix(cmd_and_args);
    char **envMatrix = buildCharMatrix(env_vars);
    // char *cmdMatrix1[] = { "php", "/home/dvargas/Desktop/webserv/tests/sites/cgi/index.php", NULL };
    // char *envMatrix2[] = { NULL };

    // Somente se quiser printar a matrix se pa
    // std::cerr << "DEBUGGING matrixes" << std::endl;
    // for (int i = 0; cmdMatrix[i] != NULL; i++) {
    //   std::cerr << "cmd matrix: " << cmdMatrix[i] << std::endl;
    // }

    // for (int j = 0; envMatrix[j] != NULL; j++) {
    //   std::cerr << "env matrix: " << envMatrix[j] << std::endl;
    // }
    if (execve("/usr/bin/php", cmdMatrix, envMatrix) < 0) {
      std::cerr << "execve error" << std::endl;
      exit(-1);
    }
  } else {
    close(cgiPipe[1]);
    int status = 0;
    setCgiPid(child_pid);
    setCgiFd(cgiPipe[0]);
    // waitpid(child_pid, &status, WNOHANG);
    waitpid(child_pid, &status, 0);
    if (WIFEXITED(status)) {
        // O processo filho terminou normalmente
        int exitStatus = WEXITSTATUS(status);
        std::cout << "CGI return status: " << exitStatus << std::endl;
    } else {
        // O processo filho não terminou normalmente
        std::cerr << "CGI process ERROR" << std::endl;
    }
  }
  return true;
}

void Server::prepareData(char* resourceData, long long resourceSize) {
  std::cout << "RUNNING CGI" << std::endl;
  int readPipeFd = getCgiFd();
  // char buffer[4096];

  while(42) {
    int bytes_read = read(readPipeFd, resourceData, resourceSize);
    
    if (bytes_read > 0) {
      std::cout << "DEBUG cgi: " << resourceData << std::endl;
      // socket->sendData(connectionFd, resourceData, bytes_read);
    } else {
      break;
    }
  }

  close(readPipeFd);
}

HttpStatusCode Server::get(HttpRequest *request, HttpResponse *response) {
  if (request->getRedirectionCode() != 0) {
    response->setStatusCode(request->getRedirectionCode());
  }

  std::string fullpath = request->getIndexPath();
  HttpStatusCode opStatus = Ready;

  if (FileSystem::isDirectory(fullpath)) {
    opStatus = FileSystem::check(fullpath, R_OK | X_OK);

    if (opStatus != 0 || !request->isDirListActive()) {
      // return (Forbidden);
      // LOOKS LIKE 42 TESTER HERE WANTS NOT FOUND
      return (Not_Found);
    }

    if (FileSystem::check(request->getIndexPath(), F_OK) == 0 && \
        request->isDirListActive()) {
      std::map<std::string, struct file_info*> entries;
      opStatus = FileReader::getDirContent(fullpath.c_str(), entries);

      if (opStatus != Ready) {
        return (opStatus);
      }

      HttpResponseComposer::buildDirListResponse(request, response, entries);
      return (Ready); // or opStatus Code
    } else {
      fullpath = request->getIndexPath();
    }
  }


  if (request->getCGI() == true) {
    char* resourceData= new char[4096];
    long long resourceSize = 999999;
    handleCgiGet(request);
    prepareData(resourceData, resourceSize);
    response->setMsgBody(resourceData);
    response->setContentLength(resourceSize);
    delete[] resourceData;
  }
  else {
    opStatus = FileSystem::check(fullpath, F_OK | R_OK);
    char* resourceData;
    long long resourceSize;
    if (opStatus != 0) {
      return (opStatus);
    }

    std::string unmodifiedTimestmap = request->getModifiedTimestampCheck();

    if (!HttpTime::isModifiedSince(unmodifiedTimestmap, fullpath)) {
      response->setStatusCode(Not_Modified);
      return (Ready);
    }

    opStatus = FileReader::getContent(fullpath.c_str(), \
                                      &resourceData, &resourceSize);

    if (opStatus != 0) {
      return (opStatus);
    }
      response->setMsgBody(resourceData);
      response->setContentLength(resourceSize);
  }

  response->setLastModifiedTime(HttpTime::getLastModifiedTime(fullpath));
  response->setContentType(MimeType::identify(fullpath));
  return (Ready);
}

HttpStatusCode Server::del(HttpRequest *request, HttpResponse *response) {
  std::ifstream inputFile;
  HttpStatusCode opStatus = Ready;

  // we need to rework this. a file can only be deleted if:
  // the folder it is contained has EXECUTE permission
  // the file itself has write permission
  std::string path = request->getIndexPath();
  if (access(path.c_str(), F_OK) == -1) {
    opStatus = Not_Found;
  } else if (access(path.c_str(), R_OK | W_OK) == -1) {
    opStatus = Method_Not_Allowed;
  } else if (remove(path.c_str()) != 0) {
    opStatus = Internal_Server_Error;
  }

  if (opStatus != Ready) {
    return (opStatus);
  }

  std::string msg = "File succefully deleted\n";
  char *msgBody = new char[msg.size()];

  for (size_t i = 0; i < msg.size(); i++) {
    msgBody[i] = msg[i];
  }

  response->setStatusCode(No_Content);
  response->setMsgBody(msgBody);
  response->setContentLength(msg.size());
  return (Ready);
}

void Server::setupCgiVars(HttpRequest *request, std::vector<std::string> &env_vars) {
  std::string server_software = "SERVER_SOFTWARE=webserv/0.1"; 
  std::string server_name = "SERVER_NAME=" + getServerName();
  // std::string server_port = "SERVER_PORT: " + port; - need to translate this
  // std::string document_root = "DOCUMENT_ROOT: " + root;
  //std::string gateway_interface = "CGI/1.1";
  std::string request_method = "REQUEST_METHOD=" + request->getMethod();
  
  std::string http_host = "HTTP_HOST=localhost";
  std::string http_connection = "HTTP_CONNECTION=Close";

  std::string script_name = "SCRIPT_NAME=" + request->getResource(); // the path used to access the resource
  //
  env_vars.push_back(server_software);
  env_vars.push_back(server_name);
  //env_vars.push_back(gateway_interface);
  env_vars.push_back(request_method);
  env_vars.push_back(http_host);
  env_vars.push_back(http_connection);
  env_vars.push_back(script_name);
}

HttpStatusCode Server::getCgi(HttpRequest *request, std::vector<std::string> &env_vars) {
  env_vars.push_back(request->getQueryString());
  return (Ready);
}

HttpStatusCode Server::postCgi(HttpRequest *request, int pipe[2], std::vector<std::string> &env_vars) {

  (void)pipe; 
  (void)request;
  (void)env_vars;
  return (Ready);
}

int   Server::getPort(void) {
  return (port);
}

std::string Server::getHost(void) {
  return (host);
}

std::string Server::getServerName(void) {
  return (server_name);
}

int Server::getCgiPid() {
  return cgiPid;
}

void Server::setCgiPid(int pid) {
  cgiPid = pid;
}

int Server::getCgiFd() {
  return cgiFd;
}

void Server::setCgiFd(int fd) {
  cgiFd = fd;
}