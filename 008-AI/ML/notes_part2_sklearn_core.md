# Part 2: Scikit-learn — Core Pipeline (Preprocessing, Splitting, Training, Predicting, Evaluating)

This is the most important library. Every ML task in your exam follows the same pipeline:

```
Load Data → Preprocess → Split → Train → Predict → Evaluate
```

---

## 1. Importing sklearn modules

sklearn is NOT imported as one block. You import **specific modules** for each task:

```python
# data preprocessing
from sklearn.preprocessing import StandardScaler

# splitting data
from sklearn.model_selection import train_test_split

# models (each model has its own module)
from sklearn.linear_model import LinearRegression
from sklearn.linear_model import LogisticRegression
from sklearn.svm import SVC
from sklearn.tree import DecisionTreeClassifier
from sklearn.ensemble import RandomForestClassifier
from sklearn.cluster import KMeans

# evaluation metrics
from sklearn.metrics import accuracy_score
from sklearn.metrics import mean_squared_error
from sklearn.metrics import mean_absolute_error
from sklearn.metrics import precision_score, recall_score, f1_score

# utilities
from sklearn.utils import resample
from sklearn.tree import export_text
from sklearn import datasets    # built-in datasets
```

---

## 2. Built-in Datasets

```python
from sklearn import datasets

iris = datasets.load_iris()
```

**What `load_iris()` returns:** a Bunch object (like a dictionary):

```python
iris.data      # numpy array, shape (150, 4) — 150 samples, 4 features
               # features: sepal length, sepal width, petal length, petal width

iris.target    # numpy array, shape (150,) — labels: 0, 1, or 2
               # 0=setosa, 1=versicolor, 2=virginica

iris.feature_names   # ['sepal length', 'sepal width', ...]
iris.target_names    # ['setosa', 'versicolor', 'virginica']
```

**Standard pattern:**
```python
X = iris.data      # features (capital X = matrix/2D)
y = iris.target    # target (lowercase y = vector/1D)
```

---

## 3. Preprocessing

### 3.1 StandardScaler — Feature Scaling

**Why scale?** Different features have different ranges (e.g., income: 15000-90000 vs age: 20-65). Algorithms like SVM and KMeans use *distance*, so features with larger numbers dominate unfairly.

**What it does:** transforms each feature to have **mean=0, std=1**.

Formula: `z = (x - mean) / std`

```python
from sklearn.preprocessing import StandardScaler

scaler = StandardScaler()
```

### `fit_transform()` vs `fit()` + `transform()`

```python
# OPTION 1: fit and transform in one step (used for training data)
X_scaled = scaler.fit_transform(X)
# fit   = calculate mean and std from X
# transform = apply the formula to X

# OPTION 2: two separate steps
scaler.fit(X_train)                    # learn mean/std from training data
X_train_scaled = scaler.transform(X_train)  # apply to training data
X_test_scaled = scaler.transform(X_test)    # apply SAME mean/std to test data
```

**IMPORTANT: in real pipelines you fit on training data only, then transform both train and test with the same scaler. But in your exam codes, they often use `fit_transform` on everything for simplicity.**

### Example from your code:

```python
scaler = StandardScaler()
X3_scaled = scaler.fit_transform(X3)

# X3 before:    [[5000, 45, 20, 0.8],      — raw values, different scales
#                [150,  22,  2, 0.1], ...]

# X3_scaled:    [[0.52, 0.89, 0.91, 0.82],  — all features now comparable
#                [-1.2, -1.0, -1.1, -0.9], ...]
```

### Scaling specific columns only:

```python
x_scaled = x.copy()                                          # don't modify original
cols_to_scale = ['gender', 'annual_income', 'spending_score']
scaler = StandardScaler()
x_scaled[cols_to_scale] = scaler.fit_transform(x[cols_to_scale])
# only the specified columns get scaled, 'age' stays in original units
```

### `inverse_transform()` — going back to original scale:

```python
centers_scaled_original_units[cols_to_scale] = scaler.inverse_transform(centers_scaled[cols_to_scale])
# converts scaled values back to original units for human interpretation
```

---

## General Flow To get an idea(syntax will be discussed):

How data is setup

### Step 1: The Manual Separation (Defining $X$ and $y$)
Assume you load your data into a DataFrame called `df`. Before doing any machine learning, you must physically separate the column you want to predict from the rest of the data.

```python
import pandas as pd

# 1. Load the full dataset (contains Size, Bedrooms, and Price)
df = pd.read_csv('houses.csv')

# 2. DEFINE THE TARGET (y)
# You isolate the single column you want the model to predict.
y = df['Price'] 

# 3. DEFINE THE FEATURES (X)
# You take the original dataframe and DROP the target column. 
# What remains are the columns the model will use to learn.
X = df.drop('Price', axis=1) 
```

At this point:
*   `y` is just a 1D list of prices: `[250000, 300000, 200000...]`
*   `X` is a 2D table containing only Size and Bedrooms.

### Step 2: The Split
Now, you pass those pre-separated variables into the splitter.

```python
from sklearn.model_selection import train_test_split

X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.3, random_state=42)
```
Because you already removed 'Price' from `X`, `train_test_split` simply takes the `X` table and chops 30% of the rows off to make `X_test`. It does the same to the `y` list to make `y_test`.

### Step 3: Training (How the model knows what to do)
The model finally learns the relationship when you call the `.fit()` method. 

```python
from sklearn.linear_model import LinearRegression

model = LinearRegression()

# THIS is where the model connects the inputs to the output
model.fit(X_train, y_train)
```
When you run `.fit(X_train, y_train)`, the model algorithm essentially does this internally:
1. It looks at row 1 of `X_train` (e.g., Size=1500, Bedrooms=3).
2. It looks at the exact corresponding row in `y_train` (e.g., $250,000).
3. It updates its internal mathematical equation to make the inputs equal the output.
4. It repeats this for every row in the training data until it finds the optimal equation.


## 4. Train-Test Split

```python
from sklearn.model_selection import train_test_split

X_train, X_test, y_train, y_test = train_test_split(
    X,              # features (2D array/DataFrame)
    y,              # target (1D array/Series)
    test_size=0.3,  # 30% for testing, 70% for training
    random_state=42 # seed for reproducibility — same split every run
)
```

**What it returns (in this exact order!):**
1. `X_train` — training features (70% of rows)
2. `X_test` — testing features (30% of rows)
3. `y_train` — training labels (matching rows)
4. `y_test` — testing labels (matching rows)

**Example with numbers:**
```python
# if X has 100 rows, test_size=0.3:
# X_train: 70 rows    X_test: 30 rows
# y_train: 70 labels  y_test: 30 labels
```

**`random_state=42`** — just a seed number. Could be any integer. Ensures the same random split every time you run the code. Without it, you'd get different splits each run.

**`test_size`** — common values: `0.2` (80/20 split) or `0.3` (70/30 split).

### A Little Dry Run To Understand
The purpose of `train_test_split` is to hide a portion of your data (30% in this case) so you can test if your model actually learned the underlying patterns, or if it just "memorized" the answers. 

To understand `y_train` and `y_test`, we first have to understand the difference between **$X$** and **$y$**.

*   **$X$ (Features):** The inputs or clues you are giving the model. (e.g., Square footage, number of bedrooms). It is usually capitalized because it represents a 2D table/matrix.
*   **$y$ (Target):** The exact answer you want the model to predict. (e.g., The price of the house). It is lowercase because it is usually a 1D list/series.

Let's look at a simple example with 10 houses.

### The Original Dataset (100% of the data)
Imagine this is your entire pandas DataFrame before any splitting.

| ID | Size (sq ft) | Bedrooms | **House Price ($)** |
| :--- | :--- | :--- | :--- |
| 1 | 1500 | 3 | **250,000** |
| 2 | 2000 | 4 | **300,000** |
| 3 | 1200 | 2 | **200,000** |
| 4 | 1800 | 3 | **280,000** |
| 5 | 2500 | 4 | **400,000** |
| 6 | 900 | 1 | **150,000** |
| 7 | 3000 | 5 | **450,000** |
| 8 | 1600 | 3 | **260,000** |
| 9 | 2200 | 4 | **350,000** |
| 10 | 1400 | 2 | **230,000** |

*   **$X$** is the `Size` and `Bedrooms` columns. Note: We have already split the dataframe to include only the X attributes
*   **$y$** is the `House Price` column. Note: These are extracted from the main dataframe

---

### What happens when you run `train_test_split`?

The function shuffles the data (because `random_state=42` ensures it shuffles the exact same way every time) and cuts it into 70% (7 rows) and 30% (3 rows).

#### 1. The Training Data (70% - 7 rows)
This is the study material for your model. It gets to see both the questions ($X$) and the answers ($y$).

**`X_train`** (The study questions)
| ID | Size (sq ft) | Bedrooms |
| :--- | :--- | :--- |
| 5 | 2500 | 4 |
| 1 | 1500 | 3 |
| 8 | 1600 | 3 |
| 3 | 1200 | 2 |
| 9 | 2200 | 4 |
| 2 | 2000 | 4 |
| 6 | 900 | 1 |

**`y_train`** (The study answers)
| ID | **House Price ($)** |
| :--- | :--- |
| 5 | **400,000** |
| 1 | **250,000** |
| 8 | **260,000** |
| 3 | **200,000** |
| 9 | **350,000** |
| 2 | **300,000** |
| 6 | **150,000** |

*How it works:* You run `model.fit(X_train, y_train)`. The model looks at `X_train` and `y_train` together and figures out the math: *"Ah, it seems like every extra bedroom adds about $50k, and every square foot adds..."*

---

#### 2. The Testing Data (30% - 3 rows)
This is the final exam. 

**`X_test`** (The exam questions)
| ID | Size (sq ft) | Bedrooms |
| :--- | :--- | :--- |
| 7 | 3000 | 5 |
| 4 | 1800 | 3 |
| 10 | 1400 | 2 |

*How it works:* You run `predictions = model.predict(X_test)`. The model looks at these 3 houses and guesses the prices based on what it learned earlier. **It does not get to see the actual prices yet.**

**`y_test`** (The exam answer key)
| ID | **House Price ($)** |
| :--- | :--- |
| 7 | **450,000** |
| 4 | **280,000** |
| 10 | **230,000** |

*How it works:* You—the programmer—hold onto `y_test`. Once the model makes its predictions, you compare the model's guesses against `y_test` (the true, hidden answers). If the model guessed $440,000 for house 7, you know it's highly accurate. If it guessed $100,000, you know the model failed.

---

## 5. The Universal Model API: fit → predict → score

**EVERY sklearn model follows the same pattern:**

```python
# Step 1: Create the model object
model = SomeAlgorithm(parameters)

# Step 2: Train (fit) on training data
model.fit(X_train, y_train)
# X_train: 2D array of features
# y_train: 1D array of labels/targets
# what happens internally: model learns patterns from data

# Step 3: Predict on new/test data
y_pred = model.predict(X_test)
# X_test: 2D array of features (same number of columns as X_train)
# returns: 1D array of predicted labels/values

# Step 4: Evaluate
score = model.score(X_test, y_test)  # returns accuracy (classification) or R² (regression)
```

### This pattern is used EVERYWHERE in your exam codes:

```python
# SVM
svm = SVC(kernel='rbf', C=1, gamma='scale')
svm.fit(X_train, y_train)
y_pred = svm.predict(X_test)

# Linear Regression
LR = LinearRegression()
ModelLR = LR.fit(x_train, y_train)      # fit() returns the model itself
PredictionLR = ModelLR.predict(x_test)

# Decision Tree
DT = DecisionTreeClassifier()
ModelDT = DT.fit(x_train, y_train)
PredictionDT = DT.predict(x_test)

# Logistic Regression
model2 = LogisticRegression()
model2.fit(X2, y2)
predictions2 = model2.predict(X2)

# Random Forest
rf_model = RandomForestClassifier(random_state=42)
rf_model.fit(x_train, y_train)
rf_pred = rf_model.predict(x_test)
```

**Notice:** the API is identical. Only the constructor (`SVC()`, `LinearRegression()`, etc.) and its parameters change.

### `fit()` returns the model itself:
```python
ModelLR = LR.fit(x_train, y_train)
# ModelLR is the SAME object as LR — fit() trains AND returns itself
# so these are equivalent:
LR.fit(x_train, y_train)
predictions = LR.predict(x_test)
```

---

## 6. Supervised Learning Models — Parameters You Need to Know

### 6.1 LinearRegression

```python
from sklearn.linear_model import LinearRegression

model = LinearRegression()   # no required parameters
model.fit(X_train, y_train)  # y_train is continuous (e.g., price: 250000)
preds = model.predict(X_test)  # returns continuous values
```

**Input:** X = features (2D), y = continuous target (1D)  
**Output from predict:** continuous values (floats)

### 6.2 LogisticRegression (Classification, despite the name!)

```python
from sklearn.linear_model import LogisticRegression

model = LogisticRegression()
model.fit(X_train, y_train)  # y_train is categorical (e.g., 0 or 1)
preds = model.predict(X_test)  # returns class labels (0 or 1)
```

**Input:** X = features, y = discrete class labels  
**Output from predict:** class labels (integers)

### 6.3 SVC (Support Vector Classifier)

```python
from sklearn.svm import SVC

svm = SVC(kernel='rbf', C=1, gamma='scale')
```

**Parameters:**
- `kernel` — defines the boundary shape:
  - `'linear'` → straight line boundary
  - `'rbf'` → curved boundary (default, more flexible)
  - `'poly'` → polynomial boundary
- `C` — regularization: higher C = fit training data more tightly (risk overfitting)
- `gamma` — `'scale'` (default, auto-calculated) or a float; controls how far influence of single point reaches

```python
# from your code - linear kernel for hyperplane classification
svm_model = SVC(kernel='linear')
svm_model.fit(X_train, y_train)
svm_accuracy = accuracy_score(y_test, svm_model.predict(X_test))
```

### 6.4 DecisionTreeClassifier

```python
from sklearn.tree import DecisionTreeClassifier

DT = DecisionTreeClassifier()         # default: no depth limit
DT = DecisionTreeClassifier(max_depth=3)  # limits tree to 3 levels

DT.fit(x_train, y_train)
preds = DT.predict(x_test)
```

**Parameters:**
- `max_depth` — limits how deep the tree can grow (prevents overfitting)
  - `None` (default) = grow until all leaves are pure or have min samples

**Special methods:**

```python
# model.score() — built-in accuracy calculation
training_acc = DT.score(x_train, y_train)   # returns float 0.0-1.0
# internally does: accuracy_score(y_train, DT.predict(x_train))

# export tree rules as text
from sklearn.tree import export_text
rules = export_text(DT, feature_names=list(X3.columns))
print(rules)
# output like:
# |--- spend_6mo <= 2250.00
# |   |--- class: 0
# |--- spend_6mo >  2250.00
# |   |--- class: 1
```

### 6.5 RandomForestClassifier

```python
from sklearn.ensemble import RandomForestClassifier

rf = RandomForestClassifier(random_state=42)
rf.fit(x_train, y_train)
rf_pred = rf.predict(x_test)
```

Random Forest = many decision trees voting together. In your exam it's used same as any other classifier. `random_state=42` for reproducibility.

---

## 7. Evaluation Metrics

### 7.1 Classification Metrics

```python
from sklearn.metrics import accuracy_score, precision_score, recall_score, f1_score

# accuracy: what % of predictions were correct overall
accuracy = accuracy_score(y_test, y_pred)
# = correct predictions / total predictions
# e.g., 45 correct out of 50 → 0.90

# precision: of all predicted positive, how many were actually positive
precision = precision_score(y_test, y_pred)

# recall: of all actual positives, how many did we catch
recall = recall_score(y_test, y_pred)

# f1: harmonic mean of precision and recall (balance between them)
f1 = f1_score(y_test, y_pred)
```

**Arguments are always: (y_true, y_pred)** — actual labels first, predicted labels second.

**From your exam code:**

```python
# simple accuracy
print("SVM Accuracy:", accuracy_score(y_test, y_pred))  # e.g., 0.9556

# full evaluation suite
print(f"accuracy {accuracy_score(y_test, lr_pred):.3f}")   # .3f = 3 decimal places
print(f"precision {precision_score(y_test, lr_pred):.3f}")
print(f"recall {recall_score(y_test, lr_pred):.3f}")
print(f"f1 {f1_score(y_test, lr_pred):.3f}")
```

**Using `.score()` directly on the model:**

```python
# equivalent to accuracy_score for classifiers
tracDT = DT.score(x_train, y_train)   # float between 0 and 1
TrainingAccDT = tracDT * 100           # convert to percentage
```

### 7.2 Regression Metrics

```python
from sklearn.metrics import mean_squared_error, mean_absolute_error

# MSE: average of squared differences
mse = mean_squared_error(y_test, predictions)

# MAE: average of absolute differences (more interpretable)
mae = mean_absolute_error(y_test, predictions)

# arguments: (y_true, y_pred) — same order as classification metrics
```

**From your exam code:**
```python
mae = mean_absolute_error(y1, predictions1)
print(f"Mean Absolute Error: {mae:.2f}")   # e.g., 3245.67
```

---

## 8. Predicting on New (Unseen) Data

```python
# from your linear regression exam code:
new_house = pd.DataFrame({
    'sq_meters': [130.0],
    'bedrooms': [3],
    'bathrooms': [2],
    'age': [10],
    'neighborhood_suburb': [1],
    'neighborhood_urban': [0]
})

# ensure columns match training data
new_house = new_house.reindex(columns=X1.columns, fill_value=0)

# predict
predicted_price = model1.predict(new_house)[0]
#                                           ^^^ [0] to get scalar from array
```

**Key points:**
1. Wrap in DataFrame with **same column names** as training data
2. Values in **lists** `[130.0]` because DataFrame expects sequences
3. `.reindex(columns=X1.columns, fill_value=0)` aligns columns
4. `.predict()` returns array, `[0]` gets the single value

### Predicting a single sample (classification):

```python
pred = model2.predict([features])[0]    # features = [10, 300, 1, 0]
# [features] wraps the 1D list into 2D → [[10, 300, 1, 0]]
# because .predict() always expects 2D input
# [0] extracts the single prediction from the result array
```

---

## 9. Upsampling Imbalanced Data

```python
from sklearn.utils import resample

# separate majority and minority classes
df_majority = df[df['class'] == 0]    # 8 samples
df_minority = df[df['class'] == 1]    # 2 samples

# upsample minority: randomly duplicate minority rows to match
df_minority_upsampled = resample(
    df_minority,       # data to resample
    replace=True,      # allow picking same row multiple times
    n_samples=8,       # target number of samples
    random_state=42    # reproducibility
)

# combine back together
df_upsampled = pd.concat([df_majority, df_minority_upsampled])
# now both classes have ~8 samples → balanced dataset
```

**Why?** If class 0 has 8 samples and class 1 has 2, the model will just predict 0 always and get 80% accuracy. Upsampling forces the model to learn both classes.
