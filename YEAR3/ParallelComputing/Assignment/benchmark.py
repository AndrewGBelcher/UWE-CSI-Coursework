######################################################################################
#  Author: Andrew Belcher
#  Description: Bruteforce Algorithm Benchmark Evaluation Script
#  Dependancies: matplot, numpy, scipy, symfit, progress
#  Date: 24/10/19
######################################################################################

import os
import time
import matplotlib.pyplot as plt
import numpy as np
from scipy.optimize import curve_fit
from symfit import parameters, variables, log, Fit, Model
from progress.bar import Bar


# Setup variable for benchmarking
rounds = []
time_average = 0 
run_attempts = 150   # number of runs per thread number being evalutated
num_of_bytes = 3    # number of bytes to bruteforce in AES-128-CBC (5 is max for brute program)
max_threads = int(os.popen('nproc --all').read())
thread_test_count = 50 + max_threads
threads = []
times = []

graph_2 = True


# Check if program is created, if not make it.
#check_for_bin = os.popen('[ -f brute ] && echo "Found" || echo "Not found"').read()
check_for_bin = os.popen('[ -f mpibrute ] && echo "Found" || echo "Not found"').read()

if(check_for_bin=='Found\n'):
    print('[+]Bruteforce program exists')
else:
    print('[-]Bruteforce program doesnt exist')
    print('\t[*]Running make to create it...')
    os.popen('make')

# Check for benchmark directories if not create em
check_for_benchmark_dir = os.popen('[ -d benchmark ] && echo "Found" || echo "Not found"').read()

if(check_for_benchmark_dir=='Found\n'):
    print('[+]Benchmark folder exists')
else:
    print('[-]Benchmark folder doesnt exist')
    print('\t[*]Creating folder')
    os.popen('mkdir benchmark')

check_for_benchmark_dir2 = os.popen('[ -d benchmark/logs ] && echo "Found" || echo "Not found"').read()

if(check_for_benchmark_dir2=='Found\n'):
    print('[+]Logs folder exists')
else:
    print('[-]Logs folder doesnt exist')
    print('\t[*]Creating folder')
    os.popen('mkdir benchmark/logs')

check_for_benchmark_dir3 = os.popen('[ -d benchmark/graphs ] && echo "Found" || echo "Not found"').read()

if(check_for_benchmark_dir3=='Found\n'):
    print('[+]Graphs folder exists')
else:
    print('[-]Graphs folder doesnt exist')
    print('\t[*]Creating folder')
    os.popen('mkdir benchmark/graphs')



# Create log file for this benchmark
print('[+]Generating Benchmark Log')
datetime = time.strftime("%Y%m%d-%H%M%S")
benchmark_log_filename = datetime + '_benchlog.log'
benchmark_log_file = open('benchmark/logs/'+benchmark_log_filename,"w+") 


# Log machine info to the top of the log
print('[+]Grabbing machine specs..')
cpu_info_cmd = os.popen('lscpu').read()
benchmark_log_file.write(cpu_info_cmd)


# Method for calculating best fit line
def func(x, a, b, c):
    return a * np.exp(-b * x) + c

def Average(lst): 
    return sum(lst) / len(lst) 

#amdahls law
# need to take percentage of increase from last time with current time
# 1/(1-10) + 10/threadnum

amdahl_increase = []

# Run benchmark on bruteforce algorithm
print('[+]Running Benchmark Test')
bar = Bar('Testing', max=thread_test_count-1)

for t in range(1,thread_test_count,1):
    for x in range(0,run_attempts,1):

        start = time.time()
        hexbyte=hex(x).lstrip("0x").rstrip("L")
        #mp_brute_cmd = os.popen('./brute -t '+str(t)+' -k '+ str(num_of_bytes)+' -p').read()
        mp_brute_cmd = os.popen('mpirun -np '+ str(t) + ' ./mpibrute -k '+ str(num_of_bytes)).read()
        #print mp_brute_cmd
        #mp_brute_cmd = os.popen('./brute -t '+str(t)+' -k '+ str(num_of_bytes)+' -b '+hexbyte+hexbyte+hexbyte+hexbyte+hexbyte+hexbyte+hexbyte+hexbyte).read()
        end = time.time() - start
        time_average += end
        benchmark_log_file.write(mp_brute_cmd)
        benchmark_log_file.write("[FINISHED]time taken: "+ str(end))

    # Average 16 attempts to guage the algos performance across a spectrum
    time_average = time_average / run_attempts
    times.append(time_average) 

    if(t > 1):
        parallel_per = (times[0] - times[1])/times[0]
        #print parallel_per
        #amdahl_increase.append(times[0]/(times[0]*(parallel_per+(1.0/float(t))*(1.0-parallel_per))))
        amdahl_increase.append(100.0*(1.0-(1.0/(times[0]/(times[0]*(parallel_per+(1.0/float(t))*(1.0-parallel_per)))))))
        #print (times[0]/(times[0]*(0.20+((1.0/float(t))*(0.80)))))

    time_average=0 
    threads.append(t) 
    bar.next()

# Done
bar.finish()


uneffected_times = amdahl_increase[0:7]
gustafson_trend_val = Average(uneffected_times)
print gustafson_trend_val
gustafson_trend = []

#amdahls law or gustafson_trend graphing
if graph_2:
    for x in range(1,thread_test_count,1):
        #gustafson_trend.append()
        #print 100-parallel_per*(1-max_threads)+max_threads
        print parallel_per
        #gustafson_trend.append(100-parallel_per*(1-max_threads)+max_threads)
        #gustafson_trend.append(thread_test_count*(1.0-parallel_per)+)
        gustafson_trend.append(100.0*(1.0-parallel_per+x*1.0-parallel_per))
        #print (100.0*(1.0-(1.0/(gustafson_trend_val))))

    plt.plot(threads[1:],amdahl_increase, 'r', label='Amdahs Law')
    #plt.plot(threads[1:],gustafson_trend[1:], 'b', label='Gustafsons Trend')

    # Setup graph formating amdahl
    #plt.suptitle('OpenMPI implementation Amdahls Law vs Gustafsons Trend(AES-128-CBC)')
    plt.suptitle('OpenMPI implementation Amdahls Law(AES-128-CBC)')
    plt.title('random averaged/number of target bytes:'+str(num_of_bytes))
    plt.ylabel('Speedup (%)')
    #plt.xlabel('Number of Threads'+'('+str(thread_test_count)+')')
    plt.xlabel('Number of Processes'+'('+str(thread_test_count)+')')
    min_ylim, max_ylim = plt.ylim()
    #plt.axvline(max_threads, color='k', linestyle='dashed', linewidth=1)
    #plt.text(max_threads*2, max_ylim*4, 'Max threads: {}'.format(max_threads))
    plt.text(max_threads*2, max_ylim*4, 'Max processes: {}'.format(max_threads))
    plt.legend(loc=1)

#normal performance graphing
else:

    # Plot the raw performance
    plt.plot(threads,times,color='Red',label='Bruteforce Algorithm Performance')


    # Plot the best fit line
    threads = np.asarray(threads)
    popt, pcov = curve_fit(func, threads, times, bounds=(0, [3., 2., 1.]))
    plt.plot(threads, func(threads, *popt), 'b-', label='Fit')


    # Setup graph formating
    #plt.suptitle('OpenMP implementation Performance (AES-128-CBC)')
    plt.suptitle('OpenMPI implementation Performance (AES-128-CBC)')
    plt.title('random averaged/number of target bytes:'+str(num_of_bytes))
    plt.ylabel('Time (s)')
    #plt.xlabel('Number of Threads'+'('+str(thread_test_count)+')')
    plt.xlabel('Number of Processes'+'('+str(thread_test_count)+')')
    min_ylim, max_ylim = plt.ylim()
    plt.axvline(max_threads, color='k', linestyle='dashed', linewidth=1)
    #plt.text(max_threads*2, max_ylim*4, 'Max threads: {}'.format(max_threads))
    plt.text(max_threads*2, max_ylim*4, 'Max Processes: {}'.format(max_threads))
    plt.legend(loc=1)


# Save the generated graphs
#plt.savefig('benchmark/graphs/' + datetime + '-bruteforce_performance_openmp' + '.png')
plt.savefig('benchmark/graphs/' + datetime + '-bruteforce_performance_openmpi' + '.png')
plt.show()

raw_input()