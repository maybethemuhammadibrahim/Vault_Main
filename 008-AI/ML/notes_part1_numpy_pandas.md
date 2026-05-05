# Part 1: NumPy & Pandas — From Zero to Exam-Ready

---

# 1. NumPy (Numerical Python)

NumPy is the backbone — almost every ML library uses it under the hood. Your data eventually becomes NumPy arrays.

## 1.1 Importing

```python
import numpy as np          # standard alias, ALWAYS use 'np'
```

## 1.2 Creating Arrays

```python
# from a plain python list
a = np.array([1, 2, 3])          # shape: (3,)  — 1D array
b = np.array([[1,2],[3,4]])      # shape: (2,2) — 2D array (matrix)

print(a)       # [1 2 3]
print(b)       # [[1 2]
               #  [3 4]]

print(a.shape) # (3,)      — tuple telling you dimensions
print(b.shape) # (2, 2)    — 2 rows, 2 cols
```

**What `.shape` returns:**
- `(n,)` = 1D array with n elements
- `(r, c)` = 2D array with r rows, c columns
- This is critical because sklearn expects specific shapes

## 1.3 Quick Array Generators

```python
np.zeros((3, 2))      # 3x2 matrix of all 0.0
np.ones((2, 3))       # 2x3 matrix of all 1.0
np.arange(0, 10, 2)   # [0 2 4 6 8]  — like range() but returns array
np.linspace(0, 1, 5)  # [0.  0.25 0.5 0.75 1.0] — 5 evenly spaced points
```

## 1.4 Indexing & Slicing (used heavily in your exam codes)

```python
arr = np.array([10, 20, 30, 40, 50])

arr[0]      # 10         — first element
arr[-1]     # 50         — last element
arr[1:4]    # [20 30 40] — index 1 to 3 (end exclusive)
```

### 2D Indexing — THIS IS IN YOUR EXAM CODE

```python
data = np.array([
    [1, 2, 3],
    [4, 5, 6],
    [7, 8, 9]
])

data[0, :]     # [1 2 3]   — entire row 0
data[:, 0]     # [1 4 7]   — entire column 0
data[0:2, 1:]  # [[2 3]    — rows 0-1, columns 1 onwards
               #  [5 6]]
```

### The `iloc` style indexing from your KMeans code:

```python
# from your code:
x = df.iloc[:, [3, 4]].values
```

This is actually **Pandas** `.iloc` (see below), but `.values` at the end converts the result into a **NumPy array**. So sklearn receives a NumPy array.

## 1.5 Boolean Indexing / Masking

From your SVM code:

```python
y = iris.target                   # [0 0 0 ... 1 1 1 ... 2 2 2]
y = (y == 0).astype(int)          # converts to binary: 1 where class=0, else 0
```

Breaking it down:

```python
y == 0          # [True True True ... False False ... False False]  (boolean array)
(y == 0).astype(int)  # [1 1 1 ... 0 0 ... 0 0]  (True→1, False→0)
```

From your outlier handling code:

```python
q_95 = df3['spend_6mo'].quantile(0.95)
df3['spend_6mo'] = np.where(df3['spend_6mo'] > q_95, q_95, df3['spend_6mo'])
```

### `np.where(condition, value_if_true, value_if_false)`

```python
arr = np.array([1, 99999, 3, 4])
q_95 = np.percentile(arr, 95)     # 95th percentile value
 
# np.where(condition, value_if_true, value_if_false)
arr = np.where(arr > q_95, q_95, arr)   # cap values above 95th pct
# → [1, capped_val, 3, 4]
```
> **Data flow**: `np.where` checks each element of `arr` against `q_95`. If `arr[i] > q_95`, replace with `q_95`; else keep original.
 


This is used for **capping outliers** — any value above the 95th percentile gets replaced with the 95th percentile value.

## 1.6 Math Operations (element-wise)

```python
a = np.array([1, 2, 3])
b = np.array([4, 5, 6])

a + b    # [5 7 9]
a * b    # [4 10 18]     — element-wise, NOT matrix multiply
a ** 2   # [1 4 9]
a.mean() # 2.0
a.sum()  # 6
```

## 1.7 NumPy Summary — What You MUST Know for the Exam

| Concept | Syntax | Used Where |
|---------|--------|------------|
| Create array | `np.array([...])` | everywhere |
| Shape | `arr.shape` | understanding data dimensions |
| Boolean mask | `(arr == val).astype(int)` | SVM binary conversion |
| Conditional replace | `np.where(cond, a, b)` | outlier capping |
| `.values` | `df[...].values` | converting pandas → numpy for sklearn |

---

# 2. Pandas

Pandas = your data loading & manipulation workhorse. Think of it as **Excel in Python**.

## 2.1 Importing

```python
import pandas as pd    # standard alias, ALWAYS 'pd'
```

## 2.2 Core Data Structures

### DataFrame — 2D table (rows + columns)

```python
# creating from a dictionary
data = {
    'name': ['Ali', 'Sara', 'Omar'],
    'age': [20, 22, 21],
    'gpa': [3.5, 3.8, 3.2]
}
df = pd.DataFrame(data)

#    name  age  gpa
# 0  Ali   20   3.5
# 1  Sara  22   3.8
# 2  Omar  21   3.2
```

### Series — 1D labeled array (a single column)

```python
s = pd.Series([10, 20, 30])
# 0    10
# 1    20
# 2    30
```

When you pick one column from a DataFrame, you get a Series:
```python
df['age']   # returns a Series
```

## 2.3 Loading Data

### From CSV file (your KMeans code):

```python
df = pd.read_csv('Mall_Customers.csv')
```

**What it does:** reads the CSV file, first row becomes column headers, returns a DataFrame.

### From string (your exam tasks):

```python
from io import StringIO

data_str = """name,age,score
Ali,20,85
Sara,22,90"""

df = pd.read_csv(StringIO(data_str))
```

`StringIO` wraps a string to make it behave like a file. `read_csv` doesn't know the difference.

## 2.4 Exploring Data

```python
df.head()        # first 5 rows — quick look
df.head(3)       # first 3 rows
df.shape         # (num_rows, num_cols)
df.columns       # list of column names
df.dtypes        # data type of each column
df.info()        # summary: non-null counts, dtypes
df.describe()    # statistics: mean, std, min, max, etc.
```

## 2.5 Selecting Columns — CRITICAL FOR EXAM

### Single column → Series

```python
df['price']           # returns Series
```

### Multiple columns → DataFrame

```python
df[['age', 'gpa']]   # returns DataFrame with only these 2 columns
```

**Note the double brackets!** `[[...]]` — outer for indexing, inner for the list.

### From your exam codes:

```python
# selecting specific columns for features
X2 = df2[['word_freq', 'email_length', 'has_hyperlinks', 'sender_trusted']]
y2 = df2['is_spam']

# dropping a column to get features (everything EXCEPT target)
X1 = df1.drop('price', axis=1)    # axis=1 means drop COLUMN (not row)
y1 = df1['price']
```

### `drop()` explained:

```python
df.drop('col_name', axis=1)   # axis=1 = column direction → removes a column
df.drop(0, axis=0)            # axis=0 = row direction → removes row at index 0
```

**axis=0 → rows, axis=1 → columns** — remember: 0 goes down, 1 goes across.

## 2.6 Selecting by Position: `.iloc`

```python
df.iloc[0]           # first row (as Series)
df.iloc[0:3]         # rows 0,1,2
df.iloc[:, 0]        # all rows, first column
df.iloc[:, [3, 4]]   # all rows, columns at index 3 and 4
```

From your KMeans code:

```python
x = df.iloc[:, [3, 4]].values
# [:, [3, 4]]  → all rows, columns 3 and 4 (Annual Income, Spending Score)
# .values      → convert from pandas DataFrame to numpy array
```

## 2.7 Handling Missing Values — IN YOUR EXAM

```python
df['sq_meters'].isnull()          # boolean Series: True where value is NaN
df['sq_meters'].isnull().sum()    # count of missing values

# fill missing with median:
df['sq_meters'] = df['sq_meters'].fillna(df['sq_meters'].median())
```

### `fillna(value)` — what it does:

```python
# BEFORE: [120.5, NaN, 180.0, 90.0]
# median of non-null = (120.5 + 180.0 + 90.0) / ... = let's say 120.5
# AFTER:  [120.5, 120.5, 180.0, 90.0]  — NaN replaced with median
```

**Why median?** Less affected by outliers than mean. Standard practice for numerical missing data.

## 2.8 Encoding Categorical Variables — IN YOUR EXAM

### One-Hot Encoding with `get_dummies()`

```python
# original: neighborhood column has values 'suburb', 'urban', 'rural'
df1 = pd.get_dummies(df1, columns=['neighborhood'], drop_first=True)
```

**Before:**

| sq_meters | neighborhood |
|-----------|-------------|
| 120.5     | suburb      |
| 95.0      | urban       |
| 200.0     | suburb      |

**After** (drop_first=True drops 'rural' as reference):

| sq_meters | neighborhood_suburb | neighborhood_urban |
|-----------|--------------------|--------------------|
| 120.5     | True               | False              |
| 95.0      | False              | True               |
| 200.0     | True               | False              |

**Why `drop_first=True`?** If suburb=0 and urban=0, it MUST be rural. So we avoid redundancy (prevents multicollinearity in regression).

### Label Encoding with `factorize()`

```python
df['vehicle_type_encoded'] = pd.factorize(df['vehicle_type'])[0]
# 'SUV' → 0, 'Sedan' → 1, 'Truck' → 2, 'Hatchback' → 3
# [0] because factorize returns (codes_array, unique_values) tuple
```

**When to use which:**
- `get_dummies` → for regression (need independent columns)
- `factorize` → for tree-based models or clustering (single numeric column ok)

## 2.9 Copying DataFrames

**General syntax:**
```python
new_df = original_df.copy()
```

**Exam example:**
```python
x_scaled = x.copy()   # independent copy — changes to x_scaled don't affect x
```

**Why `.copy()`?** Without it, `x_scaled = x` would just create a reference — both variables point to the same data. Changing one changes both.

## 2.10 Adding New Columns

**General syntax:**
```python
df['new_column_name'] = some_values   # creates column if new, overwrites if exists
```

**Exam example:**
```python
df['cluster'] = kmeans.fit_predict(x_scaled)   # adds new column with cluster labels
result['cluster_unscaled'] = clusters_unscaled  # same pattern
```

Simply assign to `df['new_col_name']` — if column exists it overwrites, if not it creates.

## 2.11 Filtering Rows — IN YOUR EXAM

**General syntax:**
```python
filtered_df = df[df['column_name'] == value]       # keep rows matching condition
filtered_df = df[df['column_name'] > threshold]     # works with any comparison
```

**Exam example:**
```python
# from your fraud detection code:
df_majority = df[df['class'] == 0]    # all rows where class is 0
df_minority = df[df['class'] == 1]    # all rows where class is 1
```

**How it works:**
```python
df['class'] == 0     # [True, True, False, True, ...]  — boolean Series
df[boolean_series]   # keeps only rows where True
```

## 2.12 Concatenating DataFrames

**General syntax:**
```python
combined_df = pd.concat([df_a, df_b])              # stack vertically (more rows)
combined_df = pd.concat([df_a, df_b], axis=1)      # stack horizontally (more columns)
```

**Exam example:**
```python
df_upsampled = pd.concat([df_majority, df_minority_upsampled])
```

`pd.concat([df1, df2])` — stacks them vertically (rows on top of each other). Like gluing two tables together.

## 2.13 Value Counts

**General syntax:**
```python
df['column'].value_counts()                # count occurrences, sorted by frequency
df['column'].value_counts().sort_index()   # count occurrences, sorted by value
```

**Exam example:**
```python
pd.Series(clusters_unscaled).value_counts().sort_index()
# output:
# 0    4      ← 4 data points in cluster 0
# 1    3      ← 3 data points in cluster 1
# 2    3      ← 3 data points in cluster 2
```

`.value_counts()` counts how many times each unique value appears. `.sort_index()` sorts by the value (0,1,2) instead of by count.

## 2.14 `reindex()` — Aligning Columns

**General syntax:**
```python
df = df.reindex(columns=reference_df.columns, fill_value=default)
```

**Exam example:**
```python
new_house = new_house.reindex(columns=X1.columns, fill_value=0)
```

**What this does:** ensures `new_house` has EXACTLY the same columns in the same order as `X1`. Any missing columns get filled with `fill_value=0`. This is crucial when your training data went through `get_dummies` and created specific columns — the new prediction data must match.

## 2.15 Pandas Summary Table

| Operation | Syntax | Example from Exam |
|-----------|--------|-------------------|
| Load CSV file | `pd.read_csv('file.csv')` | KMeans Mall data |
| Load CSV string | `pd.read_csv(StringIO(str))` | All inline datasets |
| Select columns | `df[['col1','col2']]` | Feature selection |
| Drop column | `df.drop('col', axis=1)` | Separating X from y |
| Fill missing | `df['col'].fillna(median)` | Data cleaning |
| One-hot encode | `pd.get_dummies(df, columns=[...])` | Neighborhood encoding |
| Label encode | `pd.factorize(df['col'])[0]` | Vehicle type |
| Filter rows | `df[df['col'] == val]` | Majority/minority split |
| Concat | `pd.concat([df1, df2])` | Upsampling |
| Add column | `df['new'] = values` | Cluster labels |
| iloc | `df.iloc[:, [3,4]].values` | KMeans feature extraction |
| Copy | `df.copy()` | Safe modification |
