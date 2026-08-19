# 让内嵌图片/视频/音频/pdf 正常返回方案

## 目标
修复 [http.c](file:///home/xuezha/c语言/0802/http.c)，使 `index.html` 里内嵌的图片（fan.png 等）、视频（123.mp4）、音频（录音.mp3）、pdf（图解HTTP.pdf）、链接（next.html）都能被浏览器正确请求并显示。

## 涉及文件
- `/home/xuezha/c语言/0802/http.c` —— 唯一需修改
- `/home/xuezha/c语言/0802/my.h` —— 已含 `stdio.h`/`string.h`，**无需改动**
- `/home/xuezha/c语言/0802/html/` —— 资源已就绪（index.html、next.html、fan.png、jidianqi.png、http.png、123.mp4、录音.mp3、录音(2).m4a、图解HTTP.pdf、a.txt）

## 当前问题（基于实际阅读 http.c）
1. **[http.c:40-44](file:///home/xuezha/c语言/0802/http.c#L40) 路径映射只对 `/` 生效（关键 bug）**：只有 `path=="/"` 时才给 `filepath` 赋值。浏览器请求 `/fan.png`、`/123.mp4`、`/next.html` 时 `filepath` 是未初始化垃圾值 → `fopen` 失败 → 404。内嵌资源根本到不了 Content-Type 那步。
2. **[http.c:65](file:///home/xuezha/c语言/0802/http.c#L65) `ctype` 硬编码 `text/html`**：所有文件都按 HTML 类型返回，浏览器无法把 png/mp4/mp3/pdf 当对应类型解析。
3. **中文文件名需 URL 解码**：浏览器把 `图解HTTP.pdf`、`录音.mp3` 编码成 `%E5%9B%BE...` 发请求，服务器若不解码就 `fopen` 不到中文文件名 → 404。
4. 次要：[http.c:68](file:///home/xuezha/c语言/0802/http.c#L68) `"HTTP/1.1 200 ok"` 状态文本建议大写 `OK`（不阻塞）。

## 修改方案

### 改动 1：修复 filepath 映射（对所有路径生效）+ 安全检查
把 [http.c:40-44](file:///home/xuezha/c语言/0802/http.c#L40) 的 `if(strcmp(path,"/")==0){...}` 改为：先统一把 `/` 变成 `/index.html`，再对**所有**路径拼 `filepath`，并拦截 `..`：

```c
if (strcmp(path, "/") == 0) strcpy(path, "/index.html");
char decoded[516];
url_decode(decoded, path);                      // 改动3 的解码函数
snprintf(filepath, sizeof(filepath), "html%s", decoded);
if (strstr(filepath, "..") != NULL) { close(c_fd); return NULL; }  // 防路径穿越
```
- `path` 以 `/` 开头，所以 `html%s` → `html/fan.png`、`html/next.html`、`html/index.html`，全对。
- 注意：原来 `"html/%s"` + `index.html`（无斜杠）的写法废弃，统一用 `html%s` + 带斜杠 path。

### 改动 2：新增 `get_type()` 按后缀返回 MIME 类型
在 `handle` 上方（文件顶部函数外）新增：

```c
const char *get_type(const char *path)
{
    if (strstr(path, ".html")) return "text/html";
    if (strstr(path, ".png"))  return "image/png";
    if (strstr(path, ".jpg") || strstr(path, ".jpeg")) return "image/jpeg";
    if (strstr(path, ".gif"))  return "image/gif";
    if (strstr(path, ".ico"))  return "image/x-icon";
    if (strstr(path, ".mp4"))  return "video/mp4";
    if (strstr(path, ".mp3"))  return "audio/mpeg";
    if (strstr(path, ".m4a"))  return "audio/mp4";
    if (strstr(path, ".wav"))  return "audio/wav";
    if (strstr(path, ".pdf"))  return "application/pdf";
    if (strstr(path, ".css"))  return "text/css";
    if (strstr(path, ".js"))   return "application/javascript";
    if (strstr(path, ".txt"))  return "text/plain";
    return "application/octet-stream";   // 未知类型：让浏览器当下载处理
}
```

覆盖 `html/` 目录里所有现有文件类型 + 常见类型。`strstr` 简单够用；若想更严谨可改用 `strrchr(path,'.')` 取最后扩展名，但学习阶段 `strstr` 足够。

### 改动 3：新增 `url_decode()` 支持中文文件名
在 `get_type` 旁边新增（用 `sscanf("%2x")` 解析百分号编码，无需 `ctype.h`）：

```c
void url_decode(char *dst, const char *src)
{
    while (*src) {
        if (*src == '%' && src[1] && src[2]) {
            int ch;
            sscanf(src + 1, "%2x", &ch);   // 把 %E5 等转成字节
            *dst++ = (char)ch;
            src += 3;
        } else {
            *dst++ = *src++;
        }
    }
    *dst = '\0';
}
```
作用：`%E5%9B%BE%E8%A7%A3HTTP.pdf` → `图解HTTP.pdf`，`%E5%BD%95%E9%9F%B3.mp3` → `录音.mp3`，这样 `fopen` 才能打开中文文件名的文件。

### 改动 4：ctype 用 get_type(filepath)
[http.c:65](file:///home/xuezha/c语言/0802/http.c#L65) 把
```c
const char *ctype = "text/html";
```
改为
```c
const char *ctype = get_type(filepath);
```
（用 `filepath` 判断后缀，因为它已是完整路径含扩展名。）

### 改动 5（小）：状态文本大写
[http.c:68](file:///home/xuezha/c语言/0802/http.c#L68) `"HTTP/1.1 200 ok"` → `"HTTP/1.1 200 OK"`。

## 改动后 handle 关键片段（从解析到响应）
```c
// 解析请求行
char mode[16], path[516], version[16];
char filepath[1024];
sscanf(buf, "%s %s %s", mode, path, version);
if (strcmp(mode, "GET") != 0) { close(c_fd); return NULL; }

// 路径映射 + 解码 + 安全
if (strcmp(path, "/") == 0) strcpy(path, "/index.html");
char decoded[516];
url_decode(decoded, path);
snprintf(filepath, sizeof(filepath), "html%s", decoded);
if (strstr(filepath, "..") != NULL) { close(c_fd); return NULL; }

// 打开文件，失败 404
FILE *fp = fopen(filepath, "rb");
if (fp == NULL) {
    const char *body = "404 Not Found";
    char resp[256];
    int len = snprintf(resp, sizeof(resp),
        "HTTP/1.1 404 Not Found\r\n"
        "Content-Type: text/plain\r\n"
        "Content-Length: %zu\r\n"
        "Connection: close\r\n"
        "\r\n%s", strlen(body), body);
    write(c_fd, resp, len);
    close(c_fd);
    return NULL;
}

// 文件大小 + 按后缀定类型
fseek(fp, 0, SEEK_END);
long filesize = ftell(fp);
rewind(fp);
const char *ctype = get_type(filepath);

// 发响应头
char header[256];
int hlen = snprintf(header, sizeof(header),
    "HTTP/1.1 200 OK\r\n"
    "Content-Type: %s\r\n"
    "Content-Length: %ld\r\n"
    "Connection: close\r\n"
    "\r\n", ctype, filesize);
write(c_fd, header, hlen);

// 边读边发 body
char fbuf[4096];
size_t r;
while ((r = fread(fbuf, 1, sizeof(fbuf), fp)) > 0)
    write(c_fd, fbuf, r);
fclose(fp);
close(c_fd);
return NULL;
```

## 假设与决策
- **修 filepath 映射是核心**：当前非 `/` 路径全 404，这是内嵌资源显示不出来的主因，必须先修。
- **按后缀判 MIME**：用 `strstr` 简单实现，覆盖 html/png/jpg/gif/ico/mp4/mp3/m4a/wav/pdf/css/js/txt，未知类型回 `application/octet-stream`（浏览器当下载）。
- **URL 解码支持中文**：`图解HTTP.pdf`、`录音.mp3` 是中文文件名，不解码就 404。用 `sscanf("%2x")` 实现，不引入新头文件。
- **`Connection: close`** 保持不变，单请求单响应，简单。
- **`..` 安全检查放在解码后**：因为 `%2e%2e` 解码后才暴露 `..`。
- my.h 无需改动。

## 验证步骤
1. 编译：`cc http.c -o h`（无警告无报错）
2. 运行：`./http`（端口 6815）
3. 浏览器访问 `http://192.168.76.131:6815/`：
   - `index.html` 文字显示 ✓
   - 三张图片（fan.png、jidianqi.png、http.png）正常显示 ✓
   - 视频控件能播放 123.mp4 ✓
   - 音频控件能播放 录音.mp3（中文，验证 URL 解码）✓
   - 点"图解HTTP"链接能下载/打开 图解HTTP.pdf（中文）✓
   - 点"下一页"能跳到 next.html ✓
4. `curl -v http://127.0.0.1:6815/fan.png` → 应见 `Content-Type: image/png` + `Content-Length` 正确
5. `curl -v 'http://127.0.0.1:6815/%E5%BD%95%E9%9F%B3.mp3'` → 应见 `Content-Type: audio/mpeg`（验证中文解码）
6. 访问不存在的 `/xxx.html` → 404

## 说明
- 大文件（如 13MB 的图解HTTP.pdf、3MB 的 123.mp4）用 `fread` 4KB 分块发送，内存占用低，没问题。
- 视频用 `Connection: close` + 完整发送，浏览器能播放但**不支持拖动进度条跳转**（那需要支持 `Range` 请求，是后续进阶功能，不在本次范围）。
