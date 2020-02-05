import pathlib
import subprocess
import matplotlib
import matplotlib.pyplot as plt
import numpy as np

def scree_plot(particles_glob, working_dir):
    """
    scree_plot takes a glob expression for a list of particles and generates
    a scree_plot showing the explainability of each mode. This is used to compute
    the compactness of the SSM.
    """

    print('Compactness\n'\
          '-----------')

    # Can't use list of args because it is incompatible with shell=True. We
    # need shell=True for glob expansion
    cmd = f'shapeworks readparticlesystem --names {particles_glob} -- '\
          f'compactness --nmodes 1 --saveto {working_dir}/scree.txt'
    print(cmd)
    subprocess.call(cmd, shell=True)

    # Load scree plot data
    Y = np.loadtxt(f'{working_dir}/scree.txt')
    N = len(Y)
    X = np.arange(1, N+1)

    # Plot data
    plt.plot(X, Y, linewidth=7.0)
    fig = matplotlib.pyplot.gcf()
    fig.set_size_inches(10, 10)
    plt.title('Scree Plot')
    plt.xlabel('Mode')
    plt.ylabel('Scree')
    plt.xticks(X)
    plt.grid()
    plt.show()

    print()