import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from sklearn.preprocessing import MinMaxScaler

df = pd.read_csv('Iris.csv')
print(df.head())

if 'Id' in df.columns:
    df = df.drop('Id', axis=1)
X = df.drop('Species', axis=1)
y = df['Species']
species = y.unique()
colors = ['r', 'g', 'b']


X_meaned = X - np.mean(X, axis=0)
cov_matrix = np.cov(X_meaned, rowvar=False)
eigenvalues, eigenvectors = np.linalg.eigh(cov_matrix)

sorted_idx = np.argsort(eigenvalues)[::-1]
eigenvalues = eigenvalues[sorted_idx]
eigenvectors = eigenvectors[:, sorted_idx]

n_components = 2
eigenvectors_subset = eigenvectors[:, :n_components]
X_reduced = np.dot(X_meaned, eigenvectors_subset)
X_pca_shifted = MinMaxScaler().fit_transform(X_reduced)
loadings = pd.DataFrame(
    eigenvectors_subset,
    columns=['PC1', 'PC2'],
    index=X.columns
)

top_pc1 = loadings['PC1'].abs().sort_values(ascending=False).index[:2].tolist()
top_pc2 = loadings['PC2'].abs().sort_values(ascending=False).index[:2].tolist()

plt.figure(figsize=(7,5))

for color, spec in zip(colors, species):
    subset = X_pca_shifted[y == spec]
    plt.scatter(subset[:, 0], subset[:, 1], label=spec, color=color)

plt.xlabel(top_pc1[0])
plt.ylabel(top_pc2[0])
plt.title('Manual PCA on Iris Dataset (4D → 2D)')
plt.legend(title='Species')
plt.show()

