/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvargas <dvargas@student.42.rio>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/28 17:22:33 by lfarias-          #+#    #+#             */
/*   Updated: 2023/10/25 08:25:36 by dvargas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/http/Server.hpp"

#include <iostream> // cout
#include <fstream>
#include <sstream>  // stringstream
#include <algorithm>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

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
        epollfd = epoll_create(1);
        if (epollfd == -1) {
            perror("epoll_create1");
            exit(EXIT_FAILURE);
        }
}

Server::~Server(void) { close(epollfd); }
void Server::addDescriptorToEpoll(int fd) {
        // Adiciona o descritor ao epoll para monitoramento
        struct epoll_event event;
        event.events = EPOLLIN;
        event.data.fd = fd;
        if (epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event) == -1) {
            perror("epoll_ctl");
            exit(EXIT_FAILURE);
        }
    }
void setNonBlocking(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1) {
        perror("fcntl");
        exit(EXIT_FAILURE);
    }
    if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1) {
        perror("fcntl");
        exit(EXIT_FAILURE);
    }
}
    void Server::handleEpollEvents(int timeout, std::string& cgiOutput) {
        // Espera por eventos usando epoll com o timeout especificado
        std::cout << "chegamos no handle epoll events" << std::endl;
        struct epoll_event events[10];
        int numEvents = epoll_wait(epollfd, events, 10, timeout);

        if (numEvents == -1) {
            perror("epoll_wait");
            exit(EXIT_FAILURE);
        }
    for (int i = 0; i < numEvents; ++i) {
        if (events[i].events & EPOLLIN) {
            // O descritor está pronto para leitura (events[i].data.fd contém o descritor de arquivo)
            ssize_t bytesRead;
            char buffer[4096];
            while ((bytesRead = read(events[i].data.fd, buffer, sizeof(buffer))) > 0) {
                cgiOutput.append(buffer, bytesRead);
                // std::cout << "cgiOutputttttttttt: " << cgiOutput << std::endl;
            }

            if (bytesRead == -1 && (errno == EAGAIN || errno == EWOULDBLOCK)) {
                // Não há mais dados para ler no momento
            } else if (bytesRead == -1) {
                perror("read");
                exit(EXIT_FAILURE);
            }

            // Processar cgiOutput conforme necessário
        } else {
            // Outros tipos de eventos, se necessário
        }
    }
    }
HttpStatusCode Server::getCGI(HttpRequest *request, HttpResponse *response) {
 // Cria um pipe para capturar a saída do processo CGI
  std::string cgiPath = "./cgi-test.php";
  std::cout << cgiPath << std::endl;
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }
    setNonBlocking(pipefd[0]);
    setNonBlocking(pipefd[1]);

    // Cria um novo processo
    pid_t childPid = fork();

    if (childPid == -1) {
        // Erro ao criar o processo filho
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (childPid == 0) {
        // Este é o código executado no processo filho

        // Configura variáveis de ambiente para passar a query string para o CGI
        std::cout << request->getQueryString() << std::endl;
        setenv("QUERY_STRING", request->getQueryString().c_str(), 1);

        // Fecha a extremidade de leitura do pipe
        close(pipefd[0]);

        // Redireciona a saída padrão (stdout) para o pipe
        dup2(pipefd[1], STDOUT_FILENO);

        close(pipefd[1]);

        char* argv[] = {const_cast<char*>("php"), const_cast<char*>(cgiPath.c_str()), NULL};
        // Substitui o processo atual pelo programa CGI
        execve("/usr/bin/php", argv, NULL);

        // Se o execl falhar, algo deu errado
        perror("execl");
        exit(EXIT_FAILURE);
    } else {
        // Este é o código executado no processo pai

        // Fecha a extremidade de escrita do pipe
        close(pipefd[1]);
        addDescriptorToEpoll(pipefd[0]);
        std::string cgiOutput;
        handleEpollEvents(5000, cgiOutput);
        close(pipefd[0]);
        // std::cout << cgiOutput << std::endl;
        const char* bodyData = cgiOutput.c_str(); // Converter para const char*
        char* bodyCopy = new char[strlen(bodyData) + 1];
        strncpy(bodyCopy, bodyData, strlen(bodyData) + 1);

        // Aguarda o término do processo filho
        // int status;
        // waitpid(childPid, &status, WNOHANG);

        // if (WIFEXITED(status)) {
        //     // O processo filho terminou normalmente
        //     int exitStatus = WEXITSTATUS(status);
        //     std::cout << "O processo CGI terminou com status " << exitStatus << std::endl;
        // } else {
        //     // O processo filho não terminou normalmente
        //     std::cerr << "Erro: O processo CGI não terminou normalmente." << std::endl;
        // }
      response->setMsgBody(bodyCopy);
      response->setContentLength(strlen(bodyCopy));
      // response->setContentType("text/html");
    }
    return (Ready);
}

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
  if(request->getCGI() == true) {
    if(requestMethod == "GET")
      opStatus = getCGI(request, response);
    return opStatus;
  }
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

  opStatus = FileSystem::check(fullpath, F_OK | R_OK);

  if (opStatus != 0) {
    return (opStatus);
  }

  std::string unmodifiedTimestmap = request->getModifiedTimestampCheck();

  if (!HttpTime::isModifiedSince(unmodifiedTimestmap, fullpath)) {
    response->setStatusCode(Not_Modified);
    return (Ready);
  }

  char *resourceData;
  long long resourceSize;
  opStatus = FileReader::getContent(fullpath.c_str(), \
                                    &resourceData, &resourceSize);

  if (opStatus != 0) {
    return (opStatus);
  }

  response->setLastModifiedTime(HttpTime::getLastModifiedTime(fullpath));
  response->setContentType(MimeType::identify(fullpath));
  response->setMsgBody(resourceData);
  response->setContentLength(resourceSize);
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

int   Server::getPort(void) {
  return (port);
}

std::string Server::getHost(void) {
  return (host);
}

std::string Server::getServerName(void) {
  return (server_name);
}