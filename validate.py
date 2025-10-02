import numpy as np
import matplotlib.pyplot as plt
from scipy.stats import beta

# Load samples
samples = np.loadtxt('samples.dat')

# Analytical posterior: Beta(9, 5)
theta = np.linspace(0, 1, 1000)
analytical = beta.pdf(theta, 9, 5)

# Plot
plt.figure(figsize=(10, 6))
plt.hist(samples[1000:], bins=50, density=True, alpha=0.6, label='MCMC samples')
plt.plot(theta, analytical, 'r-', linewidth=2, label='True Beta(9,5)')
plt.xlabel('θ (coin bias)')
plt.ylabel('Density')
plt.legend()
plt.title('MCMC Recovery of Posterior')
plt.savefig('posterior_validation.png', dpi=150)
plt.show()

print(f"MCMC mean: {np.mean(samples[1000:]):.3f}")
print(f"True mean: {9/(9+5):.3f}")
