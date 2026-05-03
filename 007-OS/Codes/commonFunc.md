# File I/O in C — Exam Notes (System Call Level)

> These are **low-level POSIX system calls** (`#include <unistd.h>` / `#include <fcntl.h>`), not the higher-level `fopen/fread` C standard library. Both are covered below for comparison.

---

## 1. The 4 Core System Calls — At a Glance

```
open(path, flags, [mode])  →  returns fd (file descriptor, an int)
read(fd, buf, count)       →  returns bytes read  (0 = EOF, -1 = error)
write(fd, buf, count)      →  returns bytes written (-1 = error)
close(fd)                  →  returns 0 = ok, -1 = error
```

> **File Descriptor (fd):** Just an integer. 0 = stdin, 1 = stdout, 2 = stderr. Every file you open gets the next available integer (3, 4, 5 …).

---

## 2. `open()` — Opening / Creating a File

```c
#include <fcntl.h>
#include <sys/stat.h>  // for mode constants (S_IRUSR etc.)

int fd = open(const char *path, int flags);
int fd = open(const char *path, int flags, mode_t mode);  // mode only needed when creating
```

- Returns a **file descriptor** (int ≥ 3) on success, **-1** on error.
- Always check the return value.

### `flags` — How to Open (Pick One Base + Add Options)

**Base flag — exactly one required:**

| Flag | Meaning |
|------|---------|
| `O_RDONLY` | Read only |
| `O_WRONLY` | Write only |
| `O_RDWR` | Read and write |

**Option flags — combine with `\|`:**

| Flag | Meaning |
|------|---------|
| `O_CREAT` | Create file if it doesn't exist (needs `mode`) |
| `O_TRUNC` | Truncate file to 0 bytes if it exists |
| `O_APPEND` | Always write at end of file |
| `O_EXCL` | Fail if file already exists (use with `O_CREAT`) |
| `O_NONBLOCK` | Don't block on open (pipes, devices) |

### `mode` — File Permissions (Only When Using `O_CREAT`)

| Constant | Octal | Meaning |
|----------|-------|---------|
| `S_IRUSR` | 0400 | Owner read |
| `S_IWUSR` | 0200 | Owner write |
| `S_IXUSR` | 0100 | Owner execute |
| `S_IRGRP` | 0040 | Group read |
| `S_IROTH` | 0004 | Others read |

> **Shorthand:** `0644` = owner rw, group r, others r. `0755` = owner rwx, group rx, others rx.

```c
// Common open() combinations:
int fd;

fd = open("file.txt", O_RDONLY);                          // read existing file
fd = open("file.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644); // create/overwrite
fd = open("file.txt", O_WRONLY | O_APPEND);               // append to existing
fd = open("file.txt", O_RDWR  | O_CREAT | O_EXCL,  0644); // create, fail if exists
```

---

## 3. `read()` — Reading from a File

```c
#include <unistd.h>

ssize_t read(int fd, void *buf, size_t count);
```

| Parameter | Meaning |
|-----------|---------|
| `fd` | File descriptor from `open()` |
| `buf` | Buffer to read into |
| `count` | Max bytes to read |
| **returns** | Bytes actually read, `0` = EOF, `-1` = error |

```c
char buf[128];
ssize_t n = read(fd, buf, sizeof(buf) - 1);   // leave room for '\0'
if (n == -1) { perror("read"); }
else if (n == 0) { printf("EOF reached\n"); }
else {
    buf[n] = '\0';                             // null-terminate for string use
    printf("Read %zd bytes: %s\n", n, buf);
}
```

> **Key point:** `read()` does NOT null-terminate. You must do `buf[n] = '\0'` yourself before treating it as a string.

### Reading in a loop (correct way for large files):

```c
char buf[512];
ssize_t n;
while ((n = read(fd, buf, sizeof(buf))) > 0) {
    // process n bytes in buf
    write(STDOUT_FILENO, buf, n);   // e.g. echo to stdout
}
if (n == -1) perror("read");
```

---

## 4. `write()` — Writing to a File

```c
#include <unistd.h>

ssize_t write(int fd, const void *buf, size_t count);
```

| Parameter | Meaning |
|-----------|---------|
| `fd` | File descriptor |
| `buf` | Data to write |
| `count` | Number of bytes to write |
| **returns** | Bytes actually written, `-1` = error |

```c
const char *msg = "Hello, file!\n";
ssize_t n = write(fd, msg, strlen(msg));
if (n == -1) perror("write");
```

> **Gotcha:** `write()` may write fewer bytes than requested (partial write). In production code, loop until all bytes are written.

### Correct write loop:

```c
ssize_t write_all(int fd, const void *buf, size_t len) {
    size_t written = 0;
    while (written < len) {
        ssize_t n = write(fd, (char *)buf + written, len - written);
        if (n == -1) return -1;
        written += n;
    }
    return written;
}
```

---

## 5. `close()` — Closing a File

```c
#include <unistd.h>

int close(int fd);   // returns 0 on success, -1 on error
```

```c
if (close(fd) == -1) perror("close");
```

> Always close file descriptors. Each process has a **limit** (usually 1024). Leaking fds = resource leak.

---

## 6. Complete Template — open, read, write, close

```c
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>      // open(), O_* flags
#include <unistd.h>     // read(), write(), close()
#include <string.h>     // strlen()

int main() {
    int fd;
    ssize_t n;
    char buf[256];

    /* ── WRITE: create/overwrite a file ── */
    fd = open("demo.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) { perror("open for write"); exit(EXIT_FAILURE); }

    const char *text = "Signals and file I/O in C!\n";
    if (write(fd, text, strlen(text)) == -1) perror("write");

    close(fd);   // always close before reopening

    /* ── READ: open existing file ── */
    fd = open("demo.txt", O_RDONLY);
    if (fd == -1) { perror("open for read"); exit(EXIT_FAILURE); }

    n = read(fd, buf, sizeof(buf) - 1);
    if (n == -1) { perror("read"); close(fd); exit(EXIT_FAILURE); }

    buf[n] = '\0';   // null-terminate!
    printf("Read: %s", buf);

    close(fd);

    /* ── APPEND: add to end without truncating ── */
    fd = open("demo.txt", O_WRONLY | O_APPEND);
    if (fd == -1) { perror("open for append"); exit(EXIT_FAILURE); }

    write(fd, "Appended line.\n", 15);
    close(fd);

    return 0;
}
```

---

## 7. Error Handling Pattern (Use Every Time)

```c
int fd = open("file.txt", O_RDONLY);
if (fd == -1) {
    perror("open");       // prints: "open: No such file or directory"
    exit(EXIT_FAILURE);
}
```

`perror("label")` automatically appends the human-readable error from `errno`. Always use it.

---

## 8. Standard File Descriptors — Always Open

```
0  →  STDIN_FILENO   →  stdin  (keyboard)
1  →  STDOUT_FILENO  →  stdout (screen)
2  →  STDERR_FILENO  →  stderr (screen, errors)
```

You can read/write directly to these without `open()`:

```c
write(STDOUT_FILENO, "hello\n", 6);  // same as printf
write(STDERR_FILENO, "error!\n", 7); // same as fprintf(stderr,...)
read(STDIN_FILENO, buf, sizeof(buf)); // same as fgets from keyboard
```

---

## 9. Comparison — POSIX vs C Standard Library

| | POSIX (`unistd.h`) | C Stdlib (`stdio.h`) |
|--|-------------------|---------------------|
| Open | `open(path, flags, mode)` | `fopen(path, "r"/"w"/"a")` |
| Read | `read(fd, buf, n)` | `fread(buf, size, n, fp)` / `fgets()` |
| Write | `write(fd, buf, n)` | `fwrite(buf, size, n, fp)` / `fprintf()` |
| Close | `close(fd)` | `fclose(fp)` |
| Handle type | `int fd` | `FILE *fp` |
| Buffered? | ❌ No (raw) | ✅ Yes (buffered) |
| Portable? | Linux/Unix | All platforms |

```c
/* C stdlib equivalent of the template above */
FILE *fp = fopen("demo.txt", "w");   // "r", "w", "a", "r+", "w+", "a+"
fprintf(fp, "Hello!\n");
fclose(fp);
```

### `fopen` mode strings:

| Mode | Meaning |
|------|---------|
| `"r"` | Read only, file must exist |
| `"w"` | Write only, create/truncate |
| `"a"` | Append, create if needed |
| `"r+"` | Read + write, file must exist |
| `"w+"` | Read + write, create/truncate |
| `"a+"` | Read + append, create if needed |

---

## 10. Quick Syntax Cheat Sheet

```
OPEN:   int fd = open(path, O_RDONLY);
              open(path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
              open(path, O_WRONLY | O_APPEND);
              open(path, O_RDWR  | O_CREAT | O_EXCL,  0644);

READ:   ssize_t n = read(fd, buf, sizeof(buf) - 1);
        buf[n] = '\0';   ← always!

WRITE:  ssize_t n = write(fd, buf, strlen(buf));

CLOSE:  close(fd);

ERROR:  if (fd == -1)  { perror("open");  exit(1); }
        if (n  == -1)  { perror("read");  exit(1); }
        if (n  == -1)  { perror("write"); exit(1); }

FDs:    0=stdin  1=stdout  2=stderr

FLAGS:  O_RDONLY  O_WRONLY  O_RDWR        ← base (pick one)
        O_CREAT   O_TRUNC   O_APPEND  O_EXCL  ← options (combine with |)

MODE:   0644 = rw-r--r--    0755 = rwxr-xr-x
```

> **Memory aid for flags:** "**R**eally **W**ild **R**abbits **C**hew **T**hrough **A**pples **E**agerly" → RDONLY, WRONLY, RDWR, CREAT, TRUNC, APPEND, EXCL