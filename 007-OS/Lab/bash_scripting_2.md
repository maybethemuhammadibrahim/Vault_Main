## 1. Foundational Concepts

* **Script Creation & Execution (`#!/bin/bash`, `chmod +x`, `./script.sh`):** Already done.
* **Variables & Quoting (`var="value"`, `$var`):** Already done.
* **Command-Line Arguments (`$1`, `$2`):** Already done.
* **Standard I/O, Pipes (`|`), & Redirection (`>`, `>>`):** Already done.

---

## 2. Control Blocks & Logic (Deep Dive)

### If/Else Statements
**Note:** The structural syntax (`if`, `then`, `elif`, `else`, `fi`) was **already done**. The provided codes use the POSIX standard test command `[ ]` instead of the bash-specific `[[ ]]` covered previously.

**Technical Depth:**
* `[ ]` is a command itself (an alias for `test`). It requires spaces around the brackets and operators. It is POSIX-compliant but less robust.
* `[[ ]]` is a bash keyword. It is safer, handles spaces in variables better without quoting, and supports pattern matching.
* **Recommendation:** Continue using `[[ ]]` in bash scripts as established in previous notes, even though the provided lab examples use `[ ]`.

### For Loops
**Note:** The syntax best practices (where to place `do`) were **already done**. The mechanical execution is an **extra addition**.

**Technical Depth (Iteration and Globbing):**
A `for` loop iterates over a list of items. In the provided codes, it is primarily used with "globbing" (wildcard expansion).
```bash
for file in "$dir"/*; do
```
1.  **Globbing:** The `*` expands to a list of all files/directories within `"$dir"`.
2.  **Iteration:** The loop assigns the first path to the variable `file`, executes the `do...done` block, and repeats for the next item.
3.  **Safety Check:** It is standard practice to place an `if [ -f "$file" ]` (or `[[ -f "$file" ]]`) inside the loop to ensure the script acts only on files, not directories, since `*` grabs everything.

---

## 3. Extra Additions (New Concepts from Codes)

### User Input
**Extra Addition:** Reading interactive input during execution.
* **Command:** `read`
* **Flag:** `-p` displays a prompt string before waiting for input.
* **Syntax:** `read -p "Prompt: " variable_name`

### Functions
**Extra Addition:** Encapsulating reusable code.
* **Definition:** Declared using the function name followed by parentheses and a code block.
    ```bash
    function_name() {
        # commands
    }
    ```
* **Arguments:** Functions process arguments exactly like scripts. `$1`, `$2`, etc., inside a function refer to the arguments passed to that function, *not* the arguments passed to the main script.
* **Execution:** Call the function by its name followed by any arguments: `function_name "arg1"`

### Command Substitution
**Extra Addition:** Capturing the standard output of a command and using it as a variable value or argument.
* **Syntax:** `$(command)`
* **Examples from code:**
    * `size=$(stat -c%s "$1")`
    * `count=$(find "$dir" -type f -mtime +$days | wc -l)`
    * `echo "Script run at $(date)"`
* **Mechanism:** Bash runs the command inside the parentheses, captures the output, and replaces the `$(...)` text with that output before evaluating the rest of the line.

### Arithmetic Expansion
**Extra Addition:** Performing integer math. (Note: The `(( ))` construct was briefly mentioned, but `$(( ))` for variable assignment is new).
* **Syntax:** `$(( expression ))`
* **Example from code:** `count=$((count+1))`
* **Mechanism:** Evaluates the mathematical expression and returns the result. Variables inside do not strictly need the `$` prefix (e.g., `$((count + 1))` and `$(( $count + 1 ))` both work).

### Parameter Expansion (String Manipulation)
**Extra Addition:** Extracting substrings or modifying variables directly.
* **Syntax (Extract Extension):** `ext="${file##*.}"`
* **Mechanism:** The `##*.` operator strips the longest match of `*.` from the beginning of the string.
    * If `file="document.tar.gz"`, `${file##*.}` returns `gz`.
    * If `file="image.jpg"`, it returns `jpg`.

### External Utilities Demonstrated
**Extra Addition:** The scripts leverage specific Linux binaries to achieve complex tasks.
1.  **`stat -c%s "$file"`**: Returns the file size in bytes.
2.  **`find "$dir" -type f -mtime +$days`**: Locates files (`-type f`) modified more than X days ago (`-mtime +$days`).
3.  **`find ... -exec rm -v {} \;`**: Executes the `rm` command on every item found by `find`.
4.  **`tar -czf`**: Creates (`-c`), compresses (`-z`), and specifies a filename (`-f`) for an archive.
5.  **`awk` / `grep` / `tail` / `wc -l`**: Text processing tools used in the monitoring and cleanup scripts to filter output streams and count lines.