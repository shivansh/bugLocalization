import re
import numpy as np

def get_A():

    eps = 1e-4

    with open("./output_data/output.txt",'r') as f:
        nruns = 0
        npred = 0
        successful_runs = 0
        failing_case_separate = 0
        for line in f:
            if "Socket instantiated" in line:
                nruns += 1
            if "File transfer complete" in line:
                successful_runs += 1
            if "Error while connecting socket: Child process terminated with an error!" in line:
                failing_case_separate += 1
            if line[0].isdigit():
                npred += 1
        npred = npred//(2*nruns - failing_case_separate)

    print(npred,nruns)
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
        for line in f:
            if "Socket instantiated" in line:
                nruns += 1

    with open("./output_data/output.txt",'r') as f:
        run_status = np.zeros([nruns])
        run_iter = 0
        successful_runs = 0
        for line in f:
            if "Socket instantiated" in line:
                run_iter += 1
            if "File transfer complete" in line:
                run_status[run_iter - 1] = 1
                successful_runs += 1

    # print(run_status)
    return run_status


# get_A()
# get_run_status()