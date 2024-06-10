import pandas as pd
import matplotlib.pyplot as plt

# def read_data(file_path):
#     with open(file_path, 'r') as file:
#         lines = file.readlines()

#     data = []
#     for line in lines:
#         parts = line.split()
#         print(parts)
#         particles = int(parts[0])
#         threads = int(parts[1])
#         time = float(parts[2])
#         data.append((particles, threads, time))
    
#     return data

def time_versus_thread(in_file_path, out_file_path, label):  
    data = pd.read_csv(in_file_path, sep=" ", header=None, names=['particles', 'threads', 'time'])

    particles_list = data['particles'].unique()
    

    plt.figure(figsize=(12, 8))
    for particles in particles_list:
        subset = data[data['particles'] == particles]
        plt.plot(subset['threads'], subset['time'], marker='o', label=f'{particles} particles')

    plt.xlabel('Number of threads')
    plt.ylabel('Running time')
    plt.title(f'Running time of 50 frames {label} versus number of threads for different number of particle')
    plt.legend()
    plt.grid(True)
    plt.savefig(out_file_path)

in_file_list = ['time_brute_force.txt', 'time_bht_default.txt', 'time_bht_theta_1.txt', 'time_bht_theta_2.txt']
out_file_list = ['bruteforce_time_vs_thread.png', 'bht_default_time_vs_thread.png', 'bht_1_time_vs_thread.png','bht_2_time_vs_thread.png']
label_list = ['without barnes-hut tree', 'with barnes-hut tree and theta = 0.5', 'with barnes-hut tree and theta = 1', 'with barnes-hut tree and theta = 2']


def compare_bh():
    data_brute_force = pd.read_csv('time_brute_force.txt', sep=" ", header=None, names=['particles', 'threads', 'time'])
    data_default = pd.read_csv('time_bht_default.txt', sep=" ", header=None, names=['particles', 'threads', 'time'])
    data_theta_1 = pd.read_csv('time_bht_theta_1.txt', sep=" ", header=None, names=['particles', 'threads', 'time'])
    data_theta_2 = pd.read_csv('time_bht_theta_2.txt', sep=" ", header=None, names=['particles', 'threads', 'time'])
    data_list = [data_brute_force, data_default, data_theta_1, data_theta_2]
    label_list = ['no barnes-hut','theta = 0.5', 'theta = 1', 'theta = 2']
    color_list = ['black', 'red', 'blue', 'green']
    plt.figure(figsize=(12, 8))

    for i in range (len(data_list)):
        data = data_list[i]
        thread_list = [1, 10]
        for thread in thread_list:
            subset = data[data['threads'] == thread]
            plt.plot(subset['particles'], subset['time'], marker='o', color=color_list[i], label=f'{thread} thread, {label_list[i]}')

    plt.xlabel('Number of particles')
    plt.ylabel('Running time')
    plt.title('Comparision of running time of 50 frames of brute force versus barnes-huts with regard to number of particles')
    # plt.xticks(data['particles'])
    plt.legend()
    plt.grid(True)
    plt.savefig('barnes-hut_theta_comparision.png')

for i in range (4):
    time_versus_thread(in_file_list[i], out_file_list[i], label_list[i])

compare_bh()