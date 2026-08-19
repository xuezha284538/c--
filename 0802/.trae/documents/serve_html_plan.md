# 实现"发送 HTML 文件"方案

## 目标
让 [http.c](file:///home/xuezha/c语言/0802/http.c) 能根据浏览器请求的路径，从 `html/` 目录读取对应的 `.html` 文件并发送给浏览器，浏览器能正常渲染网页。**本次范围聚焦 HTML 文件**；`index.html` 里引用的图片/视频/音频/pdf 等资源作为下一步再支持（本轮它们会因 Content-Type 不对而不显示，属于预期）。

## 涉及文件
- `/home/xuezha/c语言/0802/http.c` —— 唯一需修改的源文件
- `/home/xuezha/c语言/0802/my.h` —— 已含 `stdio.h` 等，**无需改动**（`fopen/fread/fseek/ftell/fclose` 可用）
- `/home/xuezha/c语言/0802/html/` —— 网站根目录（`index.html`、`next.html` 等已就绪）

## 当前问题（基于实际阅读 http.c）
1. **[http.c:38](file:///home/xuezha/c语言/0802/http.c#L38) 编译错误**：`snsprintf` 拼写错（应为 `snprintf`），且行尾缺 `;` → 编译不过。
2. **路径映射错误**：拼成 `./index.html`，但文件实际在 `html/` 目录下；且只处理 `/`，没处理 `/next.html` 等。
3. **算出的 `filepath` 没被使用**：下方 `while(1)` 仍是硬编码 `"200 ok"` 响应，**根本没读文件**。
4. **[http.c:58](file:///home/xuezha/c语言/0802/http.c#L58) `Connection: keep-alive` 与 `break`+`close(c_fd)` 矛盾**：声称保活却立即断开。第一版用 `Connection: close`（读一次、回一次、断开），简单正确。
5. **`while(1)` 结构混乱**：`n` 在循环外读了一次，循环内没再读却反复用；对发文件来说单次读+单次响应即可，应去掉循环。
6. 次要：[http.c:27](file:///home/xuezha/c语言/0802/http.c#L27) `path[516]` 可用但偏大，无妨。

## 修改方案

只改 `handle` 函数体（从解析请求行到响应结束）。`main` 不动。

### 步骤 1：解析请求行（已有，保留并修正）
[http.c:27-28](file:///home/xuezha/c语言/0802/http.c#L27) 已用 `sscanf(buf,"%s %s %s",mode,path,version)` 解析出 `mode`、`path`、`version`，保留。非 GET 直接关闭返回（已有）。

### 步骤 2：路径映射（修正 [http.c:34-39](file:///home/xuezha/c语言/0802/http.c#L34)）
- `/` → `/index.html`
- 拼接网站目录：`filepath = "html" + path`（path 以 `/` 开头），如 `html/index.html`、`html/next.html`
- **安全**：若 `path` 含 `..` 直接 403/关闭，防止路径穿越读系统文件

```c
if (strcmp(path, "/") == 0) strcpy(path, "/index.html");
char filepath[1024];
snprintf(filepath, sizeof(filepath), "html%s", path);   // html/index.html
if (strstr(filepath, "..") != NULL) { close(c_fd); return NULL; }
```

### 步骤 3：打开文件，失败 → 404
```c
FILE *fp = fopen(filepath, "rb");   // rb 二进制，保险
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
```

### 步骤 4：取文件大小 + 定 Content-Type
```c
fseek(fp, 0, SEEK_END);
long fsize = ftell(fp);
rewind(fp);   // 回到开头，等价于 fseek(fp,0,SEEK_SET)

const char *ctype = "text/html";   // 本轮只发 html，固定 text/html
// （下一步扩展：.png→image/png .mp4→video/mp4 .mp3→audio/mpeg .pdf→application/pdf …）
```
本轮因为只处理 HTML，`Content-Type` 直接写 `text/html`。下一步做多媒体时再写成按后缀判断的 `get_type()` 函数。

### 步骤 5：先发响应头，再边读边发文件内容
```c
char header[256];
int hlen = snprintf(header, sizeof(header),
    "HTTP/1.1 200 OK\r\n"
    "Content-Type: %s\r\n"
    "Content-Length: %ld\r\n"
    "Connection: close\r\n"
    "\r\n", ctype, fsize);
write(c_fd, header, hlen);

char fbuf[4096];
size_t r;
while ((r = fread(fbuf, 1, sizeof(fbuf), fp)) > 0)
    write(c_fd, fbuf, r);

fclose(fp);
close(c_fd);
return NULL;
```

### 步骤 6：删掉旧的 `while(1)` 和硬编码 "200 ok" 响应
[http.c:41-69](file:///home/xuezha/c语言/0802/http.c#L41) 的整个 `while(1)` 块替换为上面的步骤 3-5。`msg[256]` ([http.c:10](file:///home/xuezha/c语言/0802/http.c#L10)) 若不再使用可删除。

## 改动后 handle 完整示意
```c
void * handle(void *p)
{
    int c_fd = (int)(long)p;
    char buf[1024] = {0};
    int n = read(c_fd, buf, sizeof(buf) - 1);
    if (n <= 0) { close(c_fd); return NULL; }

    struct sockaddr_in addr;
    socklen_t len = sizeof(addr);
    getpeername(c_fd, (struct sockaddr *)&addr, &len);
    char ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &addr.sin_addr, ip, INET_ADDRSTRLEN);
    int port = ntohs(addr.sin_port);
    printf("ip:%s 端口%d 已连接\n", ip, port);
    printf("接收到 :\n%s\n", buf);

    // 1. 解析请求行
    char mode[16], path[516], version[16];
    sscanf(buf, "%s %s %s", mode, path, version);
    if (strcmp(mode, "GET") != 0) { close(c_fd); return NULL; }

    // 2. 路径映射 + 安全检查
    if (strcmp(path, "/") == 0) strcpy(path, "/index.html");
    char filepath[1024];
    snprintf(filepath, sizeof(filepath), "html%s", path);
    if (strstr(filepath, "..") != NULL) { close(c_fd); return NULL; }

    // 3. 打开文件，失败 404
    FILE *fp = fopen(filepath, "rb");
    if (fp == NULL) {
        const char *body = "404 Not Found";
        char resp[256];
        int l = snprintf(resp, sizeof(resp),
            "HTTP/1.1 404 Not Found\r\n"
            "Content-Type: text/plain\r\n"
            "Content-Length: %zu\r\n"
            "Connection: close\r\n"
            "\r\n%s", strlen(body), body);
        write(c_fd, resp, l);
        close(c_fd);
        return NULL;
    }

    // 4. 文件大小 + Content-Type（本轮固定 html）
    fseek(fp, 0, SEEK_END);
    long fsize = ftell(fp);
    rewind(fp);
    const char *ctype = "text/html";

    // 5. 发头
    char header[256];
    int hlen = snprintf(header, sizeof(header),
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: %s\r\n"
        "Content-Length: %ld\r\n"
        "Connection: close\r\n"
        "\r\n", ctype, fsize);
    write(c_fd, header, hlen);

    // 6. 边读边发 body
    char fbuf[4096];
    size_t r;
    while ((r = fread(fbuf, 1, sizeof(fbuf), fp)) > 0)
        write(c_fd, fbuf, r);

    fclose(fp);
    close(c_fd);
    return NULL;
}
```

## 假设与决策
- **本轮只发 HTML**：`Content-Type` 固定 `text/html`。`index.html` 里的 `<img>/<video>/<a>` 引用的图片/视频/pdf 本轮会因类型不对而不显示（浏览器请求它们时，要么 404 要么以错误类型返回）——这是预期，下一步做多媒体类型时解决。
- **网站根目录 = `html/`**：因为文件实际放在 `html/` 下。运行程序时工作目录须在 `/home/xuezha/c语言/0802/`，否则相对路径找不到。
- **`Connection: close`**：读一次、回一次、断开，简单正确。`keep-alive` 需循环处理多个请求且解析请求边界，留给以后。
- **用 `fopen/fread` 而非 `open/read`**：与 `stdio` 风格一致，`fseek/ftell` 取大小方便。
- **安全拦截 `..`**：防止 `/../../etc/passwd` 路径穿越。
- **`path[516]` 保留**：够用。

## 验证步骤
1. 编译：`gcc http.c -o http -lpthread`（须无报错，确认 `snsprintf` 错误已修）
2. 在 `/home/xuezha/c语言/0802/` 下运行：`./http`
3. 浏览器访问 `http://192.168.76.131:6899/` → 应渲染 `index.html`（标题 project、几个图片占位、视频/音频控件）
4. 点页面里的"下一页"链接 → 访问 `/next.html`，应显示"这是 next"
5. 手动访问不存在的 `http://192.168.76.131:6899/xxx.html` → 应显示 `404 Not Found`
6. `curl -v http://127.0.0.1:6899/` → 应看到 `HTTP/1.1 200 OK`、`Content-Type: text/html`、`Content-Length: <index.html字节数>`，body 为 HTML 内容

## 下一步预告（不在本轮）
- 扩展 `get_type(path)` 按后缀返回类型：`.png→image/png` `.jpg→image/jpeg` `.mp4→video/mp4` `.mp3/.m4a→audio/mpeg` `.pdf→application/pdf` `.ico→image/x-icon`，即可让 `index.html` 里的图片/视频/音频/pdf 全部正常显示。
