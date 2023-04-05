import pandas as pd
import numpy as np
import matplotlib.pyplot as plt 
from scipy.optimize import curve_fit

# 1. Read data from the CSV file
data = pd.read_csv('timing_output.csv')
#print(data.columns)

# 2. Plot the data as "time vs number of threads"
threads = data['number_of_threads']
time = data['time']
time_parallel = data['parallel_time']
time_serial = time - time_parallel
#print(time_serial)
time_parallel = time_parallel * threads
serial_frac = time_serial / time
speedup = time_serial / time_parallel 
speedup = 1/speedup
plt.scatter(threads, serial_frac, label='Data', marker='o', color='blue')
#print(speedup)
mask = threads <= 16
threadscut = threads[mask]
speedupcut = speedup[mask]
serialcut = serial_frac[mask]

# Define Amdahl's law function
def amdahl_law(threads, serial_fraction):
    return serial_fraction + (1 - serial_fraction) / threads

# 3. Determine the value of the 'serial fraction' by fitting the data to Amdahl's law
popt, _ = curve_fit(amdahl_law, threadscut, serialcut)
serial_fraction = popt[0]
#serial_fraction = 2

print(f'Serial Fraction: {serial_fraction}')

# 4. Plot the Amdahl's law curve
threads_range = np.linspace(1, threads.max(), 100)
amdahl_curve = amdahl_law(threads_range, serial_fraction)

plt.plot(threads_range, amdahl_curve, label="Amdahl's Law", color='red')
plt.xlabel('Number of Threads')
plt.ylabel('Time')
plt.title('Time vs Number of Threads and Amdahl\'s Law Curve')
plt.legend()
plt.savefig('plot.png')
plt.show()


