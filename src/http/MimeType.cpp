/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MimeType.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfarias- <lfarias-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/29 12:12:09 by lfarias-          #+#    #+#             */
/*   Updated: 2023/07/29 20:29:14 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/http/MimeType.hpp"
#include <iostream>

bool MimeType::is_set = false;
std::map<std::string, std::string> MimeType::types;

std::string MimeType::identify(const std::string &str) {
  init_table();
  int pos = str.rfind('.');
  std::string extension = str.substr(pos, std::string::npos);
  std::string type = types[extension];
  if (type == "")
    return "application/octet-stream";
  else
    return type;
}

void MimeType::init_table(void) {
  if (is_set)
    return;

  types.insert(std::make_pair(".aac", "audio/aac"));
  types.insert(std::make_pair(".abw", "application/x-abiword"));
  types.insert(std::make_pair(".arc", "application/x-freearc"));
  types.insert(std::make_pair(".avif", "image/avif"));
  types.insert(std::make_pair(".azw", "application/vnd.amazon.ebook"));
  types.insert(std::make_pair(".bin", "application/octet-stream"));
  types.insert(std::make_pair(".bmp", "image/bmp"));
  types.insert(std::make_pair(".bz", "application/x-bzip"));
  types.insert(std::make_pair(".bz2", "application/x-bzip2"));
  types.insert(std::make_pair(".cda", "application/x-cdf"));
  types.insert(std::make_pair(".csh", "application/x-csh"));
  types.insert(std::make_pair(".css", "text/css"));
  types.insert(std::make_pair(".csv", "text/csv"));
  types.insert(std::make_pair(".doc", "application/msword"));
  types.insert(std::make_pair(".eot", "application/vnd.ms-fontobject"));
  types.insert(std::make_pair(".epub", "application/epub+zip"));
  types.insert(std::make_pair(".gz", "application/gzip"));
  types.insert(std::make_pair(".gif", "image/gif"));
  types.insert(std::make_pair(".htm", "text/html"));
  types.insert(std::make_pair(".html", "text/html"));
  types.insert(std::make_pair(".ico", "image/vnd.microsoft.icon"));
  types.insert(std::make_pair(".ics", "text/calendar"));
  types.insert(std::make_pair(".jar", "application/java-archive"));
  types.insert(std::make_pair(".jpeg", "image/jpeg"));
  types.insert(std::make_pair(".jpg", "image/jpeg"));
  types.insert(std::make_pair(".js", "text/javascript"));
  types.insert(std::make_pair(".mid", "audio/midi"));
  types.insert(std::make_pair(".midi", "audio/midi"));
  types.insert(std::make_pair(".mjs", "text/javascript"));
  types.insert(std::make_pair(".mp3", "audio/mpeg"));
  types.insert(std::make_pair(".mp4", "video/mp4"));
  types.insert(std::make_pair(".mpeg", "video/mpeg"));
  types.insert(std::make_pair(".odp", "application/vnd.oasis.opendocument.presentation"));
  types.insert(std::make_pair(".ods", "application/vnd.oasis.opendocument.spreadsheet"));
  types.insert(std::make_pair(".odt", "application/vnd.oasis.opendocument.text"));
  types.insert(std::make_pair(".oga", "audio/ogg"));
  types.insert(std::make_pair(".ogv", "video/ogg"));
  types.insert(std::make_pair(".ogx", "application/ogg"));
  types.insert(std::make_pair(".opus", "audio/opus"));
  types.insert(std::make_pair(".otf", "font/otf"));
  types.insert(std::make_pair(".png", "image/png"));
  types.insert(std::make_pair(".pdf", "application/pdf"));
  types.insert(std::make_pair(".php", "application/x-httpd-php"));
  types.insert(std::make_pair(".ppt", "application/vnd.ms-powerpoint"));
  types.insert(std::make_pair(".pptx", "application/vnd.openxmlformats-officedocument.presentationml.presentation"));
  types.insert(std::make_pair(".rar", "application/vnd.rar"));
  types.insert(std::make_pair(".rtf", "application/rtf"));
  types.insert(std::make_pair(".sh", "application/x-sh"));
  types.insert(std::make_pair(".svg", "image/svg+xml"));
  types.insert(std::make_pair(".tar", "applicaton/x-tar"));
  types.insert(std::make_pair(".tiff", "image/tiff"));
  types.insert(std::make_pair(".tif", "image/tiff"));
  types.insert(std::make_pair(".ts", "video/mp2t"));
  types.insert(std::make_pair(".ttf", "font/ttf"));
  types.insert(std::make_pair(".text", "text/plain"));
  types.insert(std::make_pair(".vsd", "application/vnd.visio"));
  types.insert(std::make_pair(".wav", "audio/wav"));
  types.insert(std::make_pair(".weba", "audio/webm"));
  types.insert(std::make_pair(".webm", "video/webm"));
  types.insert(std::make_pair(".webp", "image/webp"));
  types.insert(std::make_pair(".woff", "font/woff"));
  types.insert(std::make_pair(".woff2", "font/woff2"));
  types.insert(std::make_pair(".xhtml", "aaplication/xhtml+xml"));
  types.insert(std::make_pair(".xls", "application/vnd.ms-excel"));
  types.insert(std::make_pair(".xml", "text/xml"));
  types.insert(std::make_pair(".xul", "application/vnd.mozilla.xul+xml"));
  types.insert(std::make_pair(".zip", "application/zip"));
  types.insert(std::make_pair(".3gp", "audio/3gpp"));
  types.insert(std::make_pair(".3g2", "audio/3gpp2"));
  types.insert(std::make_pair(".7z", "application/x-7z-compressed"));

  is_set = true;
}
