## 1. Creating a Bash Script

1.  Create a new text file with a `.sh` extension (e.g., `script.sh`).
2.  Add the standard bash shebang as the very first line to dictate the interpreter:
    ```bash
    #!/bin/bash
    ```
3.  Write your bash commands below the shebang.

## 2. Executing a Bash Script

### Making the Script Executable
By default, new files do not have execute permissions. Modify the file permissions using `chmod`:
```bash
chmod +x script.sh
```

### Running the Script
Execute the script by specifying its path relative to the current directory:
```bash
./script.sh
```

## 3. Variables and Quoting

Define variables without spaces around the equals sign: `hi=Hello`. The `$` symbol signifies a variable.

| Method | Description | Example Command | Output |
| :--- | :--- | :--- | :--- |
| **No Quote** | Simple. The bash shell interprets the variable. | `echo $hi` | `Hello` |
| **Escape Character (`\`)** | Escapes special behavior. Instructs bash to treat `$` as a literal character. | `echo \$hi` | `$hi` |
| **Single Quote (`' '`)** | Literal. Outputs exactly the enclosed contents. | `echo '$hi'` | `$hi` |
| **Double Quote (`" "`)** | Interpreted. Allows variable expansion within the string. | `echo "$hi"` | `Hello` |

## 4. Command-Line Arguments

Arguments passed during script execution are accessed via numbered variables (index is the order of the argument).

* `$0` : The script name.
* `$1` : The first argument following the script name.
* `$2` : The second argument following the script name.

*Note: Only arguments 1 through 9 (`$1` to `$9`) are captured directly by single-digit variables. Accessing arguments beyond 9 requires curly braces (e.g., `${10}`) or the `shift` command.*

## 5. Standard I/O, Pipes, and Redirection

### Standard I/O Streams
Programs communicate using three standard data streams.

| Stream ID | Name | Shorthand | Purpose |
| :--- | :--- | :--- | :--- |
| **0** | Standard In | Stdin | Command line inputs |
| **1** | Standard Out | Stdout | Normal output |
| **2** | Standard Error | Stderr | Error or other diagnostic information |

### Pipes and Redirection Operators
Redirection controls where standard input comes from and where standard output/error goes. The `>` symbol redirects standard output by default.

| Operator | Action | Description / Example |
| :--- | :--- | :--- |
| `<` | Input | Directs a file. (`A < file`: Uses contents of file as input for A) |
| `<<` | Input | Directs a stream literal (Here-document). |
| `<<<` | Input | Directs a string (Here-string). |
| `>` | Output | Writes output to file. Overwrites/"clobbers" existing content. (`B > file`) |
| `>>` | Output | Appends output to file. Creates the file if it does not exist. (`C >> file`) |
| `2>` | Error | Creates a new file and writes standard error there. (`D 2> file`) |
| `&>` | Combined | Combines standard error and standard out, writing both to a file. (`E &> file`) |
| `\|` | Pipe | Uses the standard out of the first command as standard in of the second. (`F \| G`) |
| `\|&` | Combined Pipe | Combines standard out and error of the first command as standard in of the second. (`H \|& K`) |
| `\| tee` | Split Output | Writes standard out to both the terminal and to a file. (`M \| tee file`) |

**Combined Example:**
```bash
cat sample.vcf | cut -f1,2,7 | sort -k3 > sorted.txt
```
*Reads `sample.vcf`, pipes to `cut` to extract specific columns, pipes to `sort` to organize by the 3rd column, and redirects the final output to overwrite `sorted.txt`.*

## Redirection Operator Examples

```bash
#!/bin/bash

# 1. Output redirection (>): Creates or overwrites a file
echo "Initial data" > data.txt

# 2. Append redirection (>>): Adds to an existing file
echo "More data" >> data.txt

# 3. Standard Error redirection (2>): Captures error messages
ls /nonexistent_directory 2> errors.log

# 4. Input redirection (<): Feeds a file into a command
wc -l < data.txt

# 5. Pipe (|) and combined redirection (&>): Pipes output, saves all output/errors
cat data.txt | grep "data" &> combined_output.log
```

---

## Loops

### For Loop
**Recommended (Best Practice):**
```bash
for content in *; do
  echo "$content"
done
```

**Alternative Valid Syntaxes (Less Preferred):**
```bash
# Line break before 'do'
for content in *
do
  echo "$content"
done

# Single line (useful for quick terminal commands, less readable in scripts)
for content in *; do echo "$content"; done
```

### While Loop
Executes a block of commands repeatedly as long as the test condition evaluates to true (exit status `0`).
```bash
count=1
while [[ $count -le 5 ]]; do
  echo "Count is: $count"
  ((count++))
done
```

### Until Loop
Executes a block of commands repeatedly as long as the test condition evaluates to false (exit status `>=1`). It stops executing once the condition becomes true.
```bash
count=1
until [[ $count -gt 5 ]]; do
  echo "Count is: $count"
  ((count++))
done
```

---

## Conditional Constructs

| Construct | Syntax | Purpose |
| :--- | :--- | :--- |
| **Test** | `[[ .. ]]` | Evaluates the expression inside the brackets. Returns `0` (TRUE) or `1` (FALSE). |
| **If** | `if` | Executes commands based on conditional logic. |
| **Case** | `case` | Selectively executes commands based on pattern matching. Ideal for parsing inputs. |
| **Select** | `select` | Creates interactive user menus and executes commands based on the selection. |
| **Arithmetic** | `(( .. ))` | Performs mathematical operations. Precision requires caution. |

---

## Tests: `[[ .. ]]`

Double square brackets evaluate expressions and return an exit status. 
* **0** = Success / True
* **>= 1** = Error / False

To check the exit status of the most recent command, use `echo $?`.

| Test Type | Syntax | Example | Exit Status (`$?`) |
| :--- | :--- | :--- | :--- |
| **Standard** | `[[ expression ]]` | `[[ 1 == 1 ]]` | `0` (True) |
| | | `[[ 1 == 2 ]]` | `1` (False) |
| **Negative** | `[[ ! expression ]]` | `[[ ! cow == dog ]]` | `0` (True) |
| **AND** | `[[ expr1 && expr2 ]]` | `[[ 1 == 2 && cow == cow ]]`| `1` (False) |
| **OR** | `[[ expr1 \|\| expr2 ]]`| `[[ 1 == 1 \|\| cow == dog ]]`| `0` (True) |

### Operator Strictness in Tests
When using the `[[ ]]` test construct, **you cannot mix operators**. You must use integer operators for numbers and string operators for characters/strings. 
* **Strings:** Use `==`, `!=`, `<`, `>`. Using integer operators (like `-eq`) on strings causes syntax errors or causes bash to evaluate the strings as `0`.
* **Integers:** Use `-eq`, `-ne`, `-lt`, `-gt`, etc. Using string operators on integers performs a lexicographical (alphabetical) comparison, which yields incorrect mathematical results (e.g., `[[ "10" < "2" ]]` evaluates to True because `1` comes before `2`).

---

## String Comparison Operators

| Operator | Description | Example |
| :--- | :--- | :--- |
| `==` | Equal to | `[[ "$a" == "$b" ]]` |
| `!=` | Not equal to | `[[ "$a" != "$b" ]]` |
| `<` | Less than (ASCII alphabetical order) | `[[ "$a" < "$b" ]]` |
| `>` | Greater than (ASCII alphabetical order) | `[[ "$a" > "$b" ]]` |
| `-z` | String length is zero (empty) | `[[ -z "$a" ]]` |
| `-n` | String length is non-zero | `[[ -n "$a" ]]` |

**Example Script:**
```bash
#!/bin/bash

str1="alpha"
str2="beta"
str3=""

if [[ "$str1" == "$str2" ]]; then
  echo "Strings are identical."
elif [[ -z "$str3" ]]; then
  echo "str3 is empty."
fi
```

---

## Integer Comparison Operators

| Operator | Description | Example |
| :--- | :--- | :--- |
| `-eq` | Equal to | `[[ $a -eq $b ]]` |
| `-ne` | Not equal to | `[[ $a -ne $b ]]` |
| `-lt` | Less than | `[[ $a -lt $b ]]` |
| `-le` | Less than or equal to | `[[ $a -le $b ]]` |
| `-gt` | Greater than | `[[ $a -gt $b ]]` |
| `-ge` | Greater than or equal to | `[[ $a -ge $b ]]` |

**Example Script:**
```bash
#!/bin/bash

num1=10
num2=20

if [[ $num1 -eq $num2 ]]; then
  echo "Values are equal."
elif [[ $num1 -lt $num2 ]]; then
  echo "num1 is less than num2."
fi
```

---

## File and Directory Test Operators

| Operator | Description | Example |
| :--- | :--- | :--- |
| `-e` | Exists (file or directory) | `[[ -e $path ]]` |
| `-f` | Exists and is a regular file | `[[ -f $file ]]` |
| `-d` | Exists and is a directory | `[[ -d $dir ]]` |
| `-r` | Exists and has read permission | `[[ -r $file ]]` |
| `-w` | Exists and has write permission | `[[ -w $file ]]` |
| `-x` | Exists and has execute permission | `[[ -x $file ]]` |
| `-s` | Exists and size is greater than zero | `[[ -s $file ]]` |

**Example Script:**
```bash
#!/bin/bash

target="/etc/hosts"
dir_path="/tmp"

if [[ -f $target && -r $target ]]; then
  echo "Target is a readable file."
fi

if [[ -d $dir_path ]]; then
  echo "Directory exists."
fi

if [[ ! -e "/nonexistent_file.txt" ]]; then
  echo "Path does not exist."
fi
```

## If Statements (Simple)

The `if` statement controls execution flow based on the results of conditional tests. Tests are evaluated in order; only the commands under the first true condition are executed.

**Keywords:**
* `if`: Begins the conditional test.
* `then`: Precedes the commands to run if the condition is true.
* `elif`: (Optional) "Else If" - extends the statement with additional conditions.
* `else`: (Optional) Catch-all for when no previous conditions are met.
* `fi`: Closes the `if` statement block.

**Example Script:**
```bash
#!/bin/bash

value=10

if [[ $value -gt 100 ]]; then
  echo "Value is greater than 100."
elif [[ $value -eq 10 ]]; then
  echo "Value is exactly 10."
else
  echo "Value is less than 100 and not 10."
fi
```


## Functions

Functions group reusable blocks of code. They must be declared before they are invoked.

### Syntax

| Method | Syntax |
| :--- | :--- |
| **Standard (POSIX)** | `name() { commands; }` |
| **Bash-specific** | `function name { commands; }` |

*Note: The Standard (POSIX) syntax is recommended for maximum compatibility across different shells.*

### Arguments and Return Values

Functions process arguments similarly to the main script, using numbered variables (`$1`, `$2`). 

| Concept | Description |
| :--- | :--- |
| **Passing Arguments** | Append arguments after the function call: `my_func arg1 arg2` |
| **Accessing Arguments** | Use `$1`, `$2`, etc., *inside* the function. `$0` remains the script name. |
| **Exit Status (`return`)** | Returns an integer (0-255) to indicate success or failure. Accessed via `$?` immediately after the function call. |
| **Returning Data** | Use `echo` inside the function and capture the output using command substitution: `result=$(my_func)` |

### Variable Scope

By default, all variables in a bash script are global, even if defined inside a function. Use the `local` keyword inside a function to restrict the variable's scope exclusively to that function and its children.

**Example Script:**
```bash
#!/bin/bash

# Define the function
calculate_area() {
  # Declare local variables to prevent overwriting global variables
  local length=$1
  local width=$2
  local area=$((length * width))
  
  # Output the result so it can be captured
  echo "$area"
  
  # Return success
  return 0
}

# Invoke the function, pass arguments, and capture the output
result=$(calculate_area 5 10)

# Check if the function executed successfully
if [[ $? -eq 0 ]]; then
  echo "The area is: $result"
fi
```