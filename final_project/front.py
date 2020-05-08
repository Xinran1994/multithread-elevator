import time
import math
import matplotlib.pyplot as plt


def percentile(data, percentile):
    size = len(data)
    return sorted(data)[int(math.ceil((size * percentile) / 100)) - 1]

def show_plot():
    time.sleep(1)  # wait for plot data
    f = open('analysis.csv')
    waiting_time = []
    cabin_time = []
    total_time = []
    for line in f.readlines():
        create_time, load_time, unload_time = [int(t) for t in line.split(",")]
        waiting_time.append(load_time - create_time)
        cabin_time.append(unload_time - load_time)
        total_time.append(unload_time - create_time)

    fig, ax = plt.subplots(3, sharex="all")
    fig.canvas.set_window_title('requests Time Analysis')

    num_bins = max(len(set(waiting_time)), 10)
    # the histogram of the data
    ax[0].hist(waiting_time, num_bins, alpha=0.7, edgecolor='black')
    average0 = sum(waiting_time) * 1.0 / len(waiting_time)
    ax[0].axvline(average0, color='k', linestyle='dashed', linewidth=1)
    ax[0].text(average0, 0, 'avg', rotation=90)
    percent_90_0 = percentile(waiting_time, 90)
    ax[0].axvline(percent_90_0, color='k', linestyle='dashed', linewidth=1)
    ax[0].text(percent_90_0, 0, '90%', rotation=90)
    percent_95_0 = percentile(waiting_time, 95)
    ax[0].axvline(percent_95_0, color='k', linestyle='dashed', linewidth=1)
    ax[0].text(percent_95_0, 0, '95%', rotation=90)
    percent_99_0 = percentile(waiting_time, 99)
    ax[0].axvline(percent_99_0, color='k', linestyle='dashed', linewidth=1)
    ax[0].text(percent_99_0, 0, '99%', rotation=90)

    ax[0].set_xlabel('seconds')
    ax[0].set_ylabel('# of orders')
    ax[0].set_title("Waiting Time (Before pick up)")

    num_bins = max(len(set(cabin_time)), 10)
    ax[1].hist(cabin_time, num_bins, alpha=0.7, edgecolor='black')
    average1 = sum(cabin_time) * 1.0 / len(cabin_time)
    ax[1].axvline(average1, color='k', linestyle='dashed', linewidth=1)
    ax[1].text(average1, 0, 'avg', rotation=90)
    percent_90_1 = percentile(cabin_time, 90)
    ax[1].axvline(percent_90_1, color='k', linestyle='dashed', linewidth=1)
    ax[1].text(percent_90_1, 0, '90%', rotation=90)
    percent_95_1 = percentile(cabin_time, 95)
    ax[1].axvline(percent_95_1, color='k', linestyle='dashed', linewidth=1)
    ax[1].text(percent_95_1, 0, '95%', rotation=90)
    percent_99_1 = percentile(cabin_time, 99)
    ax[1].axvline(percent_99_1, color='k', linestyle='dashed', linewidth=1)
    ax[1].text(percent_99_1, 0, '99%', rotation=90)

    ax[1].set_xlabel('seconds')
    ax[1].set_ylabel('# of orders')
    ax[1].set_title("Time spent in an elevator")


    num_bins = max(len(set(total_time)), 10)
    ax[2].hist(total_time, num_bins, alpha=0.7, edgecolor='black')
    average2 = sum(total_time) * 1.0 / len(total_time)
    ax[2].axvline(average2, color='k', linestyle='dashed', linewidth=1)
    ax[2].text(average2, 0, 'avg', rotation=90)
    percent_90_2 = percentile(total_time, 90)
    ax[2].axvline(percent_90_2, color='k', linestyle='dashed', linewidth=1)
    ax[2].text(percent_90_2, 0, '90%', rotation=90)
    percent_95_2 = percentile(total_time, 95)
    ax[2].axvline(percent_95_2, color='k', linestyle='dashed', linewidth=1)
    ax[2].text(percent_95_2, 0, '95%', rotation=90)
    percent_99_2 = percentile(total_time, 99)
    ax[2].axvline(percent_99_2, color='k', linestyle='dashed', linewidth=1)
    ax[2].text(percent_99_2, 0, '99%', rotation=90)

    ax[2].set_xlabel('seconds')
    ax[2].set_ylabel('# of orders')
    ax[2].set_title("Total Time (Total time to finish an order)")

    plt.tight_layout()
    plt.show()

def main():
    show_plot()



if __name__ == '__main__':
    main()
