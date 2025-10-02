import numpy as np
import matplotlib.pyplot as plt

chains = np.loadtxt('chains.dat')

# Compute R-hat
burnin = len(chains) // 4
chains_burnin = chains[burnin:, :]
W = np.mean(np.var(chains_burnin, axis=0, ddof=1))
chain_means = np.mean(chains_burnin, axis=0)
B = (len(chains_burnin) / (chains.shape[1] - 1)) * np.sum((chain_means - np.mean(chain_means))**2)
var_plus = ((len(chains_burnin) - 1) / len(chains_burnin)) * W + (1 / len(chains_burnin)) * B
rhat = np.sqrt(var_plus / W)

fig, axes = plt.subplots(4, 1, figsize=(12, 8), sharex=True)
for i in range(4):
    axes[i].plot(chains[:, i], linewidth=0.5, alpha=0.8)
    axes[i].axhline(y=0.643, color='r', linestyle='--', alpha=0.5)
    axes[i].set_ylabel(f'Chain {i+1}')
    axes[i].grid(alpha=0.3)

axes[-1].set_xlabel('Iteration')
fig.suptitle(f'MCMC Trace Plots - Chain Convergence', fontsize=14)
axes[0].text(0.98, 0.95, f'Gelman-Rubin R̂ = {rhat:.4f}',
             transform=axes[0].transAxes, fontsize=12, verticalalignment='top',
             horizontalalignment='right',
             bbox=dict(boxstyle='round', facecolor='lightblue', alpha=0.8))
plt.tight_layout()
plt.savefig('trace_plots.png', dpi=150)
print(f"R-hat: {rhat:.4f}")
