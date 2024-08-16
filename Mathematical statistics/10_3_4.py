import numpy as np
import scipy.stats as stats
import matplotlib.pyplot as plt

# Data
data = [(-0.32, -1.66), (5.04, 7.03), (4.76, 4.93), (5.19, 6.72), (1.94, 2.36), (1.50, 0.64), (3.16, 5.05), (1.27, 1.24),
    (-2.00, 0.59), (-0.40, 0.04), (5.19, 8.85), (2.36, 2.70), (-0.97, -0.42), (3.99, 3.53), (2.94, 4.62), (1.26, -0.46),
    (2.20, 1.05), (2.22, 2.40), (2.13, 2.25), (1.67, 5.13), (6.15, 5.73), (2.45, 3.01), (4.12, 6.34), (3.76, 2.50),
    (2.96, 4.57), (2.12, 6.40), (4.26, 4.29), (0.63, -1.37), (3.95, 4.21), (4.68, 4.83), (6.78, 5.34), (-0.71, 2.23),
    (2.00, 1.39), (0.90, 1.81), (3.69, 3.92), (0.77, 1.16), (1.58, 3.69), (-2.66, -1.10), (4.86, 5.43), (2.75, 2.56),
    (0.55, 2.69), (1.38, 3.24), (0.26, 2.47), (1.92, 3.01), (3.99, 0.75), (2.00, 3.06), (0.36, 0.21), (3.95, 3.09),
    (0.84, 4.89), (3.47, 3.47)]

# Split data into two lists
x = [pair[0] for pair in data]
y = [pair[1] for pair in data]

# Calculate Pearson correlation coefficient
correlation, _ = stats.pearsonr(x, y)
print('Pearson correlation coefficient:', correlation)

# Calculate t-statistic
t_statistic = correlation * np.sqrt(len(x)) / np.sqrt(1 - correlation**2)

# Calculate p-value
p_value = 2 * (1 - stats.t.cdf(np.abs(t_statistic), df=len(x) - 2))

print('t-statistic:', t_statistic)
print('p-value:', p_value)

# Calculate regression line parameters
a1, a0 = np.polyfit(x, y, 1)

print('a0:', a0)
print('a1:', a1)

# Calculate y values for regression line
y_fit = a0 + a1 * np.array(x)

# Print the function
print(f"y = {a0} + {a1} * x")

# Create scatter plot
plt.scatter(x, y, label='Data points')

# Add regression line
plt.plot(x, y_fit, color='red', label='Regression line')

# Calculate variance of x
variance_x = np.var(x, ddof=1)

# Calculate a1
a1 = correlation / variance_x

print('a1:', a1)

# Calculate mean values of x and y
mean_x = np.mean(x)
mean_y = np.mean(y)

# Calculate a0
a0 = mean_y - a1 * mean_x

print('a0:', a0)

# Calculate y values for regression line
y_fit = a0 + a1 * np.array(x)

# Print the function
print(f"y = {a0} + {a1} * x")

# Add regression line
plt.plot(x, y_fit, color='blue', label='Regression line')

# Add legend
plt.legend()

# Display plot
plt.show()