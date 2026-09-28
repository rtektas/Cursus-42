
## 1. Basics of an HTTP server

An HTTP server binds to a TCP port, listens for incoming connections, reads raw bytes off the socket and parses them into an HTTP request (method, path, headers, body), processes that request (serve a static file, proxy to CGI, return an error), then writes an HTTP response (status line, headers, body) back to the client. It is stateless — each request/response cycle is self-contained.

---

## 2. Event mechanism

We use **`poll()`**.

---

## 3. How poll() works

Every iteration we build a fresh `pollfd` vector. Each entry holds a `fd`, the events we care about (`POLLIN`, `POLLOUT`), and a `revents` field the kernel fills on return. We call:

```cpp
int p = poll(&fds[0], fds_size, 1000);
```

It blocks up to 1000ms, then returns. We walk the vector and act only on entries where `revents != 0`.

---

## 4. One poll() — accept + read/write

Yes, **one `poll()`**, one vector, three categories of fd all registered together before each call:

| fd type | registered events |
|---|---|
| Server listening sockets | `POLLIN` |
| Client sockets | `POLLIN` / `POLLOUT` (from `get_poll_events()`) |
| CGI stdout pipe (`cgi_ut_fd_`) | `POLLIN` |
| CGI stdin pipe (`cgi_in_fd_`) | `POLLOUT` |

`accept()` is called when a server fd fires. `on_read()` / `on_writ()` are called when a client fd fires. CGI handlers are called when a pipe fd fires. All dispatched from the same post-poll loop.

The `non-blocking` requirement refers to the **I/O operations on the sockets and pipes**, not to `poll()` itself. You satisfy that by calling:

```cpp
fcntl(client_fd, F_SETFL, O_NONBLOCK);
```

on every client fd after `accept()`. That is what makes the I/O non-blocking.

`poll()` **is allowed to block** — that is literally its job. It suspends the process efficiently until something is ready, which is far better than busy-looping. The timeout of 1000ms is also intentional and correct in your case: it ensures the main loop wakes up periodically even when no I/O fires, so you can handle:

- Client timeouts (`CLIENT_TIMEOUT` check at the top of the loop)
- CGI timeouts (`cgi_.check_timeout()`)
- The `Signals::should_stop()` check

If `poll()` had `timeout = -1` (block forever), those time-based checks would never run until an I/O event happened. The 1000ms timeout is a deliberate design choice, not a violation.

**The constraint means:** don't do blocking `recv()`/`send()` directly on sockets — gate all I/O through `poll()` and use non-blocking fds. Which is exactly what you do. ✅

---

## 5. poll() in the main loop — read AND write simultaneously

The `while (!Signals::should_stop())` loop calls `poll()` once per iteration. Client fds are registered with both `POLLIN` and `POLLOUT` where appropriate via `get_poll_events()`, and the dispatch checks both in every pass:

```cpp
if (fds[i].revents & POLLIN)  { ... on_read() ... }
else
if (fds[i].revents & POLLOUT) { ... on_writ() ... }
```

✅ Both directions checked simultaneously, every iteration.

---

## 6. Code path from poll() to client I/O

**Client read path:**
1. `poll()` returns — `revents & POLLIN` on a client fd
2. `server_fd_port_.find(fd)` → miss
3. `cgi_fd_map.find(fd)` → miss
4. `clients.find(fd)` → hit
5. `fds[i].revents & POLLIN` → `it->second->on_read()` → `recv()` called inside

**CGI output path:**
1. `poll()` returns — `revents & (POLLIN | POLLHUP)` on `cgi_ut_fd_`
2. `cgi_fd_map.find(fd)` → hit
3. `client.cgi_.on_stdut_ready(client)` → `read()` on the pipe called inside

**No I/O call exists outside this poll-gated dispatch.** ✅

---

## 7 & 8. Error handling on read/recv/write/send — both -1 and 0

Inside `on_read()` the return value of `recv()` is checked for both cases and mapped to explicit states:

- `n == 0` → `READ_CLOSE` (peer closed) → `con_manager.rmv_client(fd)` ✅
- `n < 0` → `READ_ERROR` → `con_manager.rmv_client(fd)` ✅

Same pattern in `on_writ()` for `send()`:

- fatal/error → `WRIT_FATAL` → `con_manager.rmv_client(fd)` ✅

Checking only one of `-1` or `0` is not enough — we handle both. ✅

---

## 9. errno after read/write — NOT used for control flow

All branching after `recv()`/`send()` is driven by return value states (`READ_ERROR`, `READ_CLOSE`, `WRIT_FATAL`, `BUILD_FATAL_ERR`). The only `errno` check inside `run()` is after `accept()`, which is not a read or write operation and is explicitly exempt from this rule. ✅

---

## 10. No I/O on event-driven fds without poll()

Every socket `recv()`/`send()` and every CGI pipe `read()`/`write()` is reached only through the post-poll dispatch block. There is no code path that calls I/O on a socket or pipe without `poll()` having first indicated readiness on that fd. ✅

---

## 11. Disk files don't stall the event loop

Config parsing happens once at startup before the loop. Static file reads use `open()`/`read()` on regular disk files inside request handling — these are **not** registered in the `pollfd` vector and don't go through `poll()`. This is explicitly allowed: the kernel always considers regular files ready, so they never block the event loop. ✅

---

## 12. Compilation — no relinking

```bash
make        # full build
make        # second run → nothing recompiles or relinks
```

All object dependencies are tracked correctly in the Makefile. ✅

---

