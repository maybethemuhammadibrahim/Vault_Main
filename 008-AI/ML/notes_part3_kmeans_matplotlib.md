# Part 3: KMeans Clustering (Unsupervised) & Matplotlib/Seaborn Visualization

---

## 1. KMeans Clustering

KMeans is an **unsupervised** algorithm — no `y` labels, only `X` features.

### 1.1 Basic Usage

```python
from sklearn.cluster import KMeans

kmeans = KMeans(n_clusters=5, init='k-means++', random_state=42)
```

**Parameters:**
- `n_clusters` — how many clusters to form (you must decide this)
- `init='k-means++'` — smart initialization of centroids (default, better than random)
- `random_state=42` — reproducibility

### 1.2 Training & Predicting

**Two approaches:**

```python
# APPROACH 1: fit then predict separately
kmeans.fit(X)               # learns cluster centers from X
labels = kmeans.predict(X)  # assigns each row to nearest cluster

# APPROACH 2: fit_predict (does both at once) — USED IN YOUR EXAM
labels = kmeans.fit_predict(X)   # returns array of cluster labels
# e.g., [0, 2, 1, 0, 2, 1, 0, ...] — which cluster each row belongs to
```

**Notice: No `y` parameter!** Unlike supervised learning where you do `model.fit(X, y)`, KMeans only takes `X`:

```python
# supervised:   model.fit(X_train, y_train)   ← needs labels
# unsupervised: kmeans.fit(X)                  ← no labels needed
```

### 1.3 Attributes After Fitting

```python
kmeans.cluster_centers_    # numpy array of centroid coordinates
                           # shape: (n_clusters, n_features)
                           # e.g., for 5 clusters with 2 features: shape (5, 2)

kmeans.inertia_            # WCSS — sum of squared distances to nearest centroid
                           # lower = tighter clusters (but too low = too many clusters)

kmeans.labels_             # same as fit_predict output — cluster label for each row
```

### 1.4 The Elbow Method — Finding Optimal k

```python
wcss_list = []
for i in range(1, 11):                                    # try k=1 to k=10
    kmeans = KMeans(n_clusters=i, init='k-means++', random_state=42)
    kmeans.fit(x)
    wcss_list.append(kmeans.inertia_)                      # store WCSS for this k

# plot
mtp.plot(range(1, 11), wcss_list)
mtp.title('The Elbow Method Graph')
mtp.xlabel('Number of clusters(k)')
mtp.ylabel('wcss_list')
mtp.show()
```

**How to read:** WCSS always decreases as k increases. Look for the "elbow" — the point where the decrease sharply levels off. That's your optimal k.

```
WCSS
 |
 |\
 | \
 |  \___        ← elbow at k=3 or k=5
 |      \___
 |__________\___
 1  2  3  4  5  6  7  8  9  10   k
```

### 1.5 Storing Results Back in DataFrame

```python
df['cluster'] = kmeans.fit_predict(x_scaled)

# or
df['cluster_unscaled'] = kmeans_unscaled.fit_predict(x)
df['cluster_scaled'] = kmeans_scaled.fit_predict(x_scaled)
```

This adds a new column with cluster assignments (0, 1, 2, ...) so you can analyze which rows ended up in which cluster.

### 1.6 Comparing Scaled vs Unscaled Clustering

From your exam code — this is a common question:

```python
# WITHOUT scaling — large-valued features dominate distance calculation
kmeans_unscaled = KMeans(n_clusters=3, random_state=42)
clusters_unscaled = kmeans_unscaled.fit_predict(x)

# WITH scaling — all features contribute equally
scaler = StandardScaler()
x_scaled = scaler.fit_transform(x)
kmeans_scaled = KMeans(n_clusters=3, random_state=42)
clusters_scaled = kmeans_scaled.fit_predict(x_scaled)

# compare: how many customers changed cluster?
changed = (clusters_unscaled != clusters_scaled).sum()
```

### 1.7 Interpreting Cluster Centers

```python
# centers in scaled space — not human readable
centers_scaled = pd.DataFrame(kmeans_scaled.cluster_centers_, columns=x.columns)

# convert back to original units
centers_original = centers_scaled.copy()
centers_original[cols_to_scale] = scaler.inverse_transform(centers_scaled[cols_to_scale])
print(centers_original)
# now you can say: "Cluster 0 has avg income $50k, avg spending score 20"
```

---

## 2. Matplotlib — Plotting

### 2.1 Importing

```python
import matplotlib.pyplot as plt    # standard alias
# your exam code also uses:
import matplotlib.pyplot as mtp    # same thing, different alias
```

### 2.2 Line Plot (Elbow Method)

```python
plt.plot(range(1, 11), wcss_list)    # x-values, y-values
plt.title('The Elbow Method Graph')   # chart title
plt.xlabel('Number of clusters(k)')   # x-axis label
plt.ylabel('WCSS')                    # y-axis label
plt.show()                            # display the plot
```

**`plt.plot(x, y)`** — connects points with lines. `x` and `y` must be same length.

### 2.3 Scatter Plot (Cluster Visualization)

**Basic scatter:**
```python
plt.scatter(df['study_hours'], df['gpa'], c=df['cluster'])
#           ^x-axis data       ^y-axis     ^color by cluster
plt.title('student clusters')
plt.xlabel('study hours')
plt.ylabel('gpa')
plt.show()
```

**Parameters:**
- First arg: x-coordinates
- Second arg: y-coordinates
- `c=` : color — can be array of numbers (auto-colored) or color string
- `s=` : marker size in points² (e.g., `s=100` for large dots)
- `label=` : legend label for this scatter group

### 2.4 Multi-Scatter for Individual Clusters (from your KMeans code)

```python
# each cluster plotted separately with its own color and label
mtp.scatter(x[y_predict == 0, 0], x[y_predict == 0, 1],
            s=100, c='blue', label='Cluster 1')
mtp.scatter(x[y_predict == 1, 0], x[y_predict == 1, 1],
            s=100, c='green', label='Cluster 2')
# ...etc for each cluster

# plot centroids
mtp.scatter(kmeans.cluster_centers_[:, 0], kmeans.cluster_centers_[:, 1],
            s=300, c='yellow', label='Centroid')

mtp.title('Clusters of customers')
mtp.xlabel('Annual Income (k$)')
mtp.ylabel('Spending Score (1-100)')
mtp.legend()     # show the legend (uses 'label' from each scatter)
mtp.show()
```

**Breaking down the boolean indexing:**

```python
x[y_predict == 0, 0]
# y_predict == 0     → boolean array: [True, False, True, ...]
# x[boolean, 0]      → rows where True, column 0 (x-coordinate)
# x[boolean, 1]      → rows where True, column 1 (y-coordinate)
```

So each `scatter()` call plots only the points belonging to one cluster.

### 2.5 Common plt Functions Summary

| Function | Purpose | Example |
|----------|---------|---------|
| `plt.plot(x, y)` | Line chart | Elbow method |
| `plt.scatter(x, y)` | Dot chart | Cluster viz |
| `plt.title(str)` | Set title | `plt.title('My Chart')` |
| `plt.xlabel(str)` | X-axis label | `plt.xlabel('k')` |
| `plt.ylabel(str)` | Y-axis label | `plt.ylabel('WCSS')` |
| `plt.legend()` | Show legend | After labeled scatter |
| `plt.show()` | Display plot | Always call at the end |

---

## 3. Complete Pipeline Patterns (Cheat Sheet)

### Pattern 1: Supervised Classification

```python
import pandas as pd
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import StandardScaler
from sklearn.svm import SVC                        # or any classifier
from sklearn.metrics import accuracy_score

# 1. load data
df = pd.read_csv(...)   # or pd.read_csv(StringIO(...))

# 2. separate features and target
X = df.drop('target_column', axis=1)
y = df['target_column']

# 3. preprocess (if needed)
scaler = StandardScaler()
X_scaled = scaler.fit_transform(X)

# 4. split
X_train, X_test, y_train, y_test = train_test_split(X_scaled, y, test_size=0.3, random_state=42)

# 5. train
model = SVC(kernel='linear')
model.fit(X_train, y_train)

# 6. predict
y_pred = model.predict(X_test)

# 7. evaluate
print(f"Accuracy: {accuracy_score(y_test, y_pred):.2f}")
```

### Pattern 2: Supervised Regression

```python
from sklearn.linear_model import LinearRegression
from sklearn.metrics import mean_absolute_error

# same steps 1-4 as above, but y is continuous (prices, scores, etc.)

# 5. train
model = LinearRegression()
model.fit(X_train, y_train)

# 6. predict
preds = model.predict(X_test)

# 7. evaluate with regression metrics
mae = mean_absolute_error(y_test, preds)
```

### Pattern 3: Unsupervised Clustering

```python
from sklearn.cluster import KMeans
from sklearn.preprocessing import StandardScaler

# 1. load and select features (NO target column)
X = df[['feature1', 'feature2', 'feature3']]

# 2. scale
scaler = StandardScaler()
X_scaled = scaler.fit_transform(X)

# 3. elbow method to find k
wcss = []
for k in range(2, 11):
    km = KMeans(n_clusters=k, random_state=42)
    km.fit(X_scaled)
    wcss.append(km.inertia_)
plt.plot(range(2, 11), wcss)
plt.show()

# 4. train with chosen k
kmeans = KMeans(n_clusters=3, random_state=42)
df['cluster'] = kmeans.fit_predict(X_scaled)

# 5. visualize
plt.scatter(df['feature1'], df['feature2'], c=df['cluster'])
plt.show()
```

---

## 4. Quick Reference — What Returns What

| Expression | Returns | Type | Shape |
|-----------|---------|------|-------|
| `pd.read_csv(...)` | table of data | DataFrame | (rows, cols) |
| `df.drop('col', axis=1)` | df without that column | DataFrame | (rows, cols-1) |
| `df['col']` | single column | Series | (rows,) |
| `scaler.fit_transform(X)` | scaled data | ndarray | same as X |
| `train_test_split(X,y,...)` | X_train, X_test, y_train, y_test | tuple of 4 | split sizes |
| `model.fit(X, y)` | the trained model itself | Model object | — |
| `model.predict(X)` | predicted labels/values | ndarray | (n_samples,) |
| `model.score(X, y)` | accuracy or R² | float | — |
| `accuracy_score(y_true, y_pred)` | accuracy 0-1 | float | — |
| `kmeans.fit_predict(X)` | cluster labels | ndarray | (n_samples,) |
| `kmeans.inertia_` | WCSS value | float | — |
| `kmeans.cluster_centers_` | centroid coords | ndarray | (k, features) |

---

## 5. Common Gotchas

1. **predict() needs 2D input**: `model.predict([features])` not `model.predict(features)` for single sample
2. **accuracy_score order**: `(y_true, y_pred)` — true first, predicted second
3. **axis=1 for columns, axis=0 for rows** in `df.drop()`
4. **fit_transform on train, transform on test** (or fit_transform on all if no split)
5. **KMeans: fit(X) not fit(X, y)** — unsupervised has no y
6. **`.values`** to convert DataFrame/Series to numpy array when needed
7. **`[0]`** after predict for single prediction: `model.predict(new_data)[0]`
