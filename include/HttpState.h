#ifndef HTTPSTATE_H_
#define HTTPSTATE_H_

//定义HTTP相应的一些状态信息
const char* ok_200_title = "ok";
const char* error_400_title = "Bad Request";
const char* error_400_form = "Your request has syntax or is inherently impossible to satisfy.\n";
const char* error_403_title = "Forbidden";
const char* error_403_form = "you do not have permission to get file from this server.\n";
const char* error_404_title = "not found";
const char* error_404_form = "the requested file was not found on this server.\n";
const char* error_500_title = "internal error";
const char* error_500_form = "there was an unuaual problem serving the request file.\n";
//网站的根目录
const char* doc_root = "/home/ubuntu/OWL/bin";

#endif