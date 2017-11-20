import re
import numpy as np

def get_A():

    eps = 1e-2

    with open("./output_data/output.txt",'r') as f:
        nruns = 0
        npred = 0
        for line in f:
            if "Socket instantiated" in line:
                nruns += 1
            if line[0].isdigit():
                npred += 1
        npred = npred//nruns

    with open("./output_data/output.txt",'r') as f:
        true = np.zeros([npred,nruns]) + eps
        false = np.zeros([npred,nruns]) + eps
        run_iter = -1
        pred_iter = 0
        for line in f:
            if "Socket instantiated" in line:
                run_iter += 1
            if line[0].isdigit():
                n = [int(s) for s in re.split("[ :\n]", line) if s.isdigit()]
                true[pred_iter%npred][run_iter] += n[0]
                false[pred_iter%npred][run_iter] += n[1]
            pred_iter += 1
            if(pred_iter>=17):
                pred_iter = 0

        A = true/(false+true)
        return A


def get_run_status():
    with open("./output_data/output.txt",'r') as f:
        nruns = 0
        npred = 0
        for line in f:
            if "Socket instantiated" in line:
                nruns += 1
            if line[0].isdigit():
                npred += 1
        npred = npred//nruns

    with open("./output_data/output.txt",'r') as f:
        run_status = np.zeros([nruns])
        run_iter = 0
        for line in f:
            if "Socket instantiated" in line:
                run_iter += 1
            if "File transfer complete" in line:
                run_status[run_iter - 1] = 1

    return run_status
