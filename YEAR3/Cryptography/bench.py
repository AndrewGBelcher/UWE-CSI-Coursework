

import os
import time
import matplotlib.pyplot as plt
import numpy as np
from scipy.optimize import curve_fit

from symfit import parameters, variables, log, Fit, Model
from progress.bar import Bar



passlen = [1,2,3,4,5]#,6]
times = [0,0.001,1.408,53.347,4807.493]#,906992.640]
# Plot the raw performance
plt.plot(passlen,times,"r",label='performance',markevery=100)
plt.title('Alphanum Password BruteForce')
plt.suptitle('Average Performance')
plt.ylabel('Time (s)')
plt.xlabel('Password Length')



plt.show()
