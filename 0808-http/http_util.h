#ifndef HTTP_UTIL_H
#define HTTP_UTIL_H
#include <stddef.h>
#include <sys/types.h>

#define HTTP_BUF_SIZE 4096
#define URI_MAX_LEN 256
#define FILEPATH_MAX 512

// MIME查询：根据扩展名返回Content-Type
const char *get_mime_type(const char *path);
// URL解码（处理 %XX 与 +，支持中文/空格文件名）
void url_decode(char *s);
// 解析GET请求行，提取URI（去掉查询串 ?xxx）
int parse_get_request(char *buf, char *uri_out);
// 路径防穿越检测（禁止 .. 出现）
int path_safe_check(const char *uri);
// 拼接根目录 + URI 为真实磁盘路径（末尾/自动补 index.html）
void path_combine(char *dst, int dst_size, const char *root, const char *uri);
// 错误响应 400/403/404/500
void send_error_response(int fd, int code);
// 200正常文件响应，sendfile发送
int send_file_response(int fd, const char *filepath);
// 循环读取直到读完请求头 \r\n\r\n 或缓冲满
int read_http_request(int fd, char *buf, int max_len);
// 循环写，保证写满（处理部分写/EINTR）
ssize_t write_all(int fd, const void *buf, size_t n);
#endif
