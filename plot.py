import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv("regression_plot.csv")

df = df.dropna()
df = df[pd.to_numeric(df['dt'], errors='coerce').notnull()]
df = df[pd.to_numeric(df['dT'], errors='coerce').notnull()]
df = df[pd.to_numeric(df['y_fit'], errors='coerce').notnull()]

df['dt'] = pd.to_numeric(df['dt'])
df['dT'] = pd.to_numeric(df['dT'])
df['y_fit'] = pd.to_numeric(df['y_fit'])

# Plot
plt.figure(figsize=(8, 5))
plt.scatter(df['dt'], df['dT'], label='Data Eksperimen', color='blue')
plt.plot(df['dt'], df['y_fit'], label='Regresi Linier', color='red', linewidth=2)

plt.title("Regresi Linier: ΔT terhadap Δt")
plt.xlabel("Δt (s)")
plt.ylabel("ΔT (K)")
plt.grid(True)
plt.legend()
plt.tight_layout()
plt.savefig("regresi_plot.png", dpi=300)
plt.show()
