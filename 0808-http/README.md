# High-Concurrency HTTP Server

高并发 HTTP 静态文件服务器 —— 基于 C 语言从零实现,epoll 多路 IO 复用 + 线程池 + sendfile 零拷贝 + 异步日志。

## 功能特性

- ⚡ **epoll 多路 IO 复用**: 替代传统 accept 阻塞循环,单线程管理大量连接
- 🧵 **线程池**: 预创建工作线程,任务队列 + 互斥锁 + 条件变量调度
- 📦 **零拷贝**: sendfile 直接从文件到 socket,绕过用户态内存拷贝
- 📝 **异步日志**: 独立日志线程 + 环形缓冲区,非阻塞写盘
- ⚙️ **配置文件**: server.conf 集中管理端口/线程数/日志/超时等参数
- 🔒 **安全防护**: 路径穿越检测 + 超时断开 + 优雅退出
- 🚀 **性能优化**: HTTP 响应预拼接 + 长连接支持

## 技术架构

```
                    ┌── worker thread 1
  client ──▶ epoll ─┼── worker thread 2
   (ET模式)         ├── worker thread 3
                    └── worker thread N
                         │
                         ├── http_util (请求解析/响应构造)
                         ├── sendfile  (零拷贝文件传输)
                         └── log       (异步日志)
```

## 目录结构

```
0808-http/
├── main.c           主程序: epoll事件循环 + 连接管理
├── config.h/c       配置文件解析 (server.conf)
├── thread_pool.h/c  线程池 (互斥锁+条件变量+任务队列)
├── http_util.h/c    HTTP 请求解析 + 响应构造
├── log.h/c          异步日志系统
├── server.conf      配置文件
└── README.md
```

## 编译运行

```bash
gcc -o http_server main.c config.c thread_pool.c http_util.c log.c -lpthread
./http_server [端口]
```

默认端口读 `server.conf`,命令行参数优先级最高。

## 配置说明 (server.conf)

| 参数 | 默认值 | 说明 |
|------|--------|------|
| port | 8080 | 监听端口 |
| root_path | ./www | 静态文件根目录 |
| max_listen_queue | 128 | listen 队列长度 |
| thread_pool_num | 4 | 工作线程数 |
| task_queue_max | 256 | 任务队列上限 |
| log_enable | 1 | 是否启用日志 |
| log_file | server.log | 日志文件路径 |
| log_flush_interval | 3 | 日志刷盘间隔(秒) |
| read_timeout_ms | 5000 | 读超时(毫秒) |
| write_timeout_ms | 5000 | 写超时(毫秒) |

## 核心技术点

### 1. epoll 多路 IO 复用
- `epoll_create1` 创建 epoll 实例
- `epoll_ctl` 添加/删除/修改监听 fd
- `epoll_wait` 阻塞等待事件,返回就绪列表
- ET 边沿触发模式 + 非阻塞 IO
- ConnInfo 结构体通过 `epoll_event.data.ptr` 传递连接上下文

### 2. 线程池
- 预创建 worker 线程,避免频繁创建销毁开销
- 任务队列(链表实现)存放待处理 fd
- `pthread_mutex_t` 保护队列
- `pthread_cond_t` 同步:无任务时线程挂起,有任务时唤醒
- 优雅销毁:设置 shutdown 标志,广播所有线程退出

### 3. sendfile 零拷贝
- 静态文件传输直接走 `sendfile()` 系统调用
- 数据从文件系统缓存直接到 socket,不经过用户态
- 比传统 read+write 少两次内存拷贝,大文件性能提升明显

### 4. 异步日志
- 独立日志线程,业务线程不阻塞写盘
- 环形缓冲区暂存日志
- 定时刷盘 + 缓冲区满时刷盘双触发
- 三级日志级别: INFO / WARN / ERR

### 5. 安全防护
- 路径穿越检测: 解析 `..` 后校验是否在 root_path 内
- 读写超时: 防止慢连接占着资源
- 信号处理: SIGINT 优雅退出(销毁线程池+关闭日志)
- SIGPIPE 忽略: 防止写已关闭 socket 导致进程崩溃

## 性能优化清单

- [x] epoll 替代 accept 阻塞循环
- [x] 线程池替代 per-connection per-thread
- [x] sendfile 零拷贝传输文件
- [x] HTTP 响应头预拼接,减少运行时字符串操作
- [x] 异步日志,业务线程不阻塞 IO
- [x] ET 边沿触发 + 非阻塞 IO

## License

MIT
