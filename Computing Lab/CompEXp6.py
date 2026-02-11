from sklearn.decomposition import PCA
import matplotlib.pyplot as plt
import pandas as pd
from sklearn.preprocessing import MinMaxScaler
import numpy as np

df = pd.read_csv('Iris.csv')
print(df.head())

if 'Id' in df.columns:
    df = df.drop('Id', axis=1)

X = df.drop('Species', axis=1)
y = df['Species'] 
species = y.unique()
colors = ['r', 'g', 'b'] 

pca = PCA(n_components=2)
X_pca = pca.fit_transform(X)
X_pca_shifted = MinMaxScaler().fit_transform(X_pca)

loadings = pd.DataFrame(
    pca.components_.T,
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
plt.ylabel(top_pc1[1])
plt.title('PCA on Iris Dataset (4D → 2D)')
plt.legend(title='Species')
plt.show()



