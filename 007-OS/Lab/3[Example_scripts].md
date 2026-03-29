## Task 1: Move PNG and GIF files to their own folders
```bash
#!/bin/bash

# Script to move PNG and GIF files into separate folders

read -p "Enter dir: " dir

# Check if directory exists
if [[ ! -d "$dir" ]]; then
    echo "Dir does not exist"
    exit 1
fi

# Loop through all files in the directory
for file in "$dir"/*; do
    # Skip if not a regular file
    [[ ! -f "$file" ]] && continue
    
    # Extract extension and filename
    fileExt="${file##*.}"
    fileName="${file##*/}"
    
    # Move PNG files to png folder
    if [[ "$fileExt" = "png" ]]; then
        mkdir -p "$dir/png"
        mv "$file" "$dir/png/"
        echo "$fileName moved to png/"
    # Move GIF files to gif folder
    elif [[ "$fileExt" = "gif" ]]; then
        mkdir -p "$dir/gif"
        mv "$file" "$dir/gif/"
        echo "$fileName moved to gif/"
    else
        echo "File has ext: $fileExt (not moved)"
    fi
done
```

---

## Task 2: Move DOC and DOCX files to Documents folder
```bash
#!/bin/bash

# Script to move DOC and DOCX files to Documents folder

read -p "Enter dir: " dir

# Check if directory exists
if [[ ! -d "$dir" ]]; then
    echo "$dir does not exist"
    exit 1
fi

# Loop through all files in the directory
for file in "$dir"/*; do
    # Extract extension and filename
    fileExt="${file##*.}"
    fileName="${file##*/}"

    # Skip if not a regular file
    [[ ! -f "$file" ]] && continue

    # Move DOC and DOCX files to Documents folder
    if [[ "$fileExt" = "doc" || "$fileExt" = "docx" ]]; then
        mkdir -p "$dir/Documents"
        mv "$file" "$dir/Documents/"
        echo "$fileName moved to Documents/"
    else 
        echo "File has extention: $fileExt"
    fi
done
```

---

## Task 3: Create a backup archive of a directory
```bash
#!/bin/bash

# Script to create a compressed backup of a directory

read -p "Enter the dir: " dir

# Remove trailing slash if present
dir="${dir%/}"

# Create backup name with directory name and current date
backupName="${dir##*/}_$(date +%Y%m%d).tar.gz"

# Create compressed archive
tar -czf "$backupName" -C "$(dirname "$dir")" "$(basename "$dir")"

echo "$backupName created"
```


## Task 4: Restore a directory from a backup archive
```bash
#!/bin/bash

read -p "Enter backup archive (.tar.gz): " archive
read -p "Enter destination dir: " dest

if [[ ! -f "$archive" ]]; then
    echo "Archive does not exist"
    exit 1
fi

mkdir -p "$dest"
tar -xzf "$archive" -C "$dest"

echo "Restored $archive to $dest"
```

---

## Task 5: Backup a file with current date appended
```bash
#!/bin/bash

read -p "Enter filename: " file

if [[ ! -f "$file" ]]; then
    echo "File does not exist"
    exit 1
fi

# Extract extension and base name
fileExt="${file##*.}"
baseName="${file%.*}"
dateStr=$(date +%Y%m%d)

# Handle files without extensions
if [[ "$fileExt" == "$file" ]]; then
    backupName="${file}_backup_${dateStr}"
else
    backupName="${baseName}_backup_${dateStr}.${fileExt}"
fi

cp "$file" "$backupName"
echo "Created backup: $backupName"
```

---

## Task 6: List files sorted from largest to smallest
```bash
#!/bin/bash

read -p "Enter dir: " dir

if [[ ! -d "$dir" ]]; then
    echo "Dir does not exist"
    exit 1
fi

# ls -lS sorts by size (largest first), -h makes sizes readable
ls -lSh "$dir"
```

---

## Task 7: List files sorted from smallest to largest
```bash
#!/bin/bash

read -p "Enter dir: " dir

if [[ ! -d "$dir" ]]; then
    echo "Dir does not exist"
    exit 1
fi

# -r reverses the sort order
ls -lShr "$dir"
```

---

## Task 8: Move files older than 7 days into Old_Files
```bash
#!/bin/bash

read -p "Enter dir: " dir

if [[ ! -d "$dir" ]]; then
    echo "Dir does not exist"
    exit 1
fi

dest="$dir/Old_Files"
mkdir -p "$dest"

# find files modified more than 7 days ago and move them
find "$dir" -maxdepth 1 -type f -mtime +7 -exec mv {} "$dest/" \;

echo "Moved files older than 7 days to $dest/"
```

---

## Task 9: Delete empty files and print a message
```bash
#!/bin/bash

read -p "Enter dir: " dir

if [[ ! -d "$dir" ]]; then
    echo "Dir does not exist"
    exit 1
fi

# find empty files and loop through them
find "$dir" -maxdepth 1 -type f -empty | while read -r file; do
    rm "$file"
    echo "Deleted empty file: $(basename "$file")"
done
```

---

## Task 10: Copy .txt files to another directory
```bash
#!/bin/bash

read -p "Enter source dir: " src
read -p "Enter destination dir: " dest

if [[ ! -d "$src" ]]; then
    echo "Source dir does not exist"
    exit 1
fi

mkdir -p "$dest"

for file in "$src"/*.txt; do
    [[ ! -f "$file" ]] && continue

    cp "$file" "$dest/"
    echo "Copied: $(basename "$file")"
done
```

---

## Task 11: Rename files with a sequential number
```bash
#!/bin/bash

read -p "Enter dir: " dir

if [[ ! -d "$dir" ]]; then
    echo "Dir does not exist"
    exit 1
fi

count=1

for file in "$dir"/*; do
    [[ ! -f "$file" ]] && continue

    fileExt="${file##*.}"
    baseName="${file%.*}"
    
    # Handle files without extensions
    if [[ "$fileExt" == "$(basename "$file")" ]]; then
        newName="${baseName}${count}"
    else
        newName="${baseName}${count}.${fileExt}"
    fi

    mv "$file" "$newName"
    echo "Renamed $(basename "$file") to $(basename "$newName")"
    ((count++))
done
```

---

## Task 12: Rename .log files by appending current date
```bash
#!/bin/bash

read -p "Enter dir: " dir

if [[ ! -d "$dir" ]]; then
    echo "Dir does not exist"
    exit 1
fi

dateStr=$(date +%Y%m%d)

for file in "$dir"/*.log; do
    [[ ! -f "$file" ]] && continue

    baseName="${file%.*}"
    newName="${baseName}_${dateStr}.log"
    
    mv "$file" "$newName"
    echo "Renamed $(basename "$file") to $(basename "$newName")"
done
```

---

## Task 13: Move files modified today to Today_Files
```bash
#!/bin/bash

read -p "Enter dir: " dir

if [[ ! -d "$dir" ]]; then
    echo "Dir does not exist"
    exit 1
fi

dest="$dir/Today_Files"
mkdir -p "$dest"

# -daystart measures time from beginning of today, -mtime 0 means within that day
find "$dir" -maxdepth 1 -type f -daystart -mtime 0 -exec mv {} "$dest/" \;

echo "Moved today's files to $dest/"
```

---

## Task 14: Move files greater than 5MB to Large_Files
```bash
#!/bin/bash

read -p "Enter dir: " dir

if [[ ! -d "$dir" ]]; then
    echo "Dir does not exist"
    exit 1
fi

dest="$dir/Large_Files"
mkdir -p "$dest"

# -size +5M finds files larger than 5 Megabytes
find "$dir" -maxdepth 1 -type f -size +5M -exec mv {} "$dest/" \;

echo "Moved files > 5MB to $dest/"
```

---

## Task 15: Organize files into folders by extension
```bash
#!/bin/bash

read -p "Enter dir: " dir

if [[ ! -d "$dir" ]]; then
    echo "Dir does not exist"
    exit 1
fi

for file in "$dir"/*; do
    [[ ! -f "$file" ]] && continue

    fileExt="${file##*.}"
    
    # Define folder based on extension
    case "$fileExt" in
        txt|doc|docx)
            folder="Documents"
            ;;
        jpg|jpeg|png|gif)
            folder="Images"
            ;;
        sh|py|js|bash)
            folder="Scripts"
            ;;
        *)
            folder="Other"
            ;;
    esac

    mkdir -p "$dir/$folder"
    mv "$file" "$dir/$folder/"
    echo "Moved $(basename "$file") to $folder/"
done
```