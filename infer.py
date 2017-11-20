import re
import numpy as np
import parse_output

N_ITER = 1000
eps = 1e-6

A = parse_output.get_A()#read data
# The parts of data will be truth probabilities

npred = A.shape[0]
nruns = A.shape[1]

# run_status = np.ones(shape=[nruns]) # 0 means fail, 1 means true
run_status = parse_output.get_run_status() # 0 means fail, 1 means true
ind_zero = np.where(run_status==0)[0] # Indices for failed runs
ind_nonzero = np.where(run_status!=0)[0] # Indices for successful runs

Q = np.ones(shape=[npred])
Q_compl = np.ones(shape=[npred])

R = np.ones(shape=[npred,nruns])
R_compl = np.ones(shape=[npred,nruns])

F = np.ones(shape=[npred])
F_compl= np.ones(shape=[npred])

S = np.ones(shape=[npred])
S_compl= np.ones(shape=[npred])

for n_iter in range(N_ITER):
    Q = (F/(S+eps))*(F_compl/(S_compl+eps))
    Q_compl = (S/(F+eps))*(S_compl/(F_compl+eps))

    for i in range(npred):
        R_kj = np.sum(R[:, ind_zero], axis=0)
        R_ij = R[i,ind_zero]
        R_compl_kj = np.sum(R_compl[:, ind_zero], axis=0)
        R_compl_ij = R_compl[i,ind_zero]
        F[i] = np.sum(A[i,ind_zero]*(R_ij/R_kj))
        F_compl[i] = np.sum((1-A[i,ind_zero])*(R_compl_ij/R_compl_kj))
        
        R_kj = np.sum(R[:, ind_nonzero], axis=0)
        R_ij = R[i,ind_nonzero]
        R_compl_kj = np.sum(R_compl[:, ind_nonzero], axis=0)
        R_compl_ij = R_compl[i,ind_nonzero]
        S[i] = np.sum(A[i,ind_nonzero]*(R_ij/(R_kj+eps)))
        S_compl[i] = np.sum((1-A[i,ind_nonzero])*(R_compl_ij/(R_compl_kj+eps)))
    R = A*np.tile(run_status,(npred,1))*np.transpose(np.tile(Q,(nruns,1))) + A*np.tile((1-run_status),(npred,1))/np.transpose(np.tile(Q_compl,(nruns,1)))
    R_compl = (1-A)*np.tile(run_status,(npred,1))*np.transpose(np.tile(Q_compl,(nruns,1))) + (1-A)*np.tile((1-run_status),(npred,1))/np.transpose(np.tile(Q,(nruns,1)))
    # R_compl = (1-A)*np.transpose(np.tile(run_status,(npred,1)))*np.tile(Q_compl,(1,nruns)) + (1-A)*np.transpose(np.tile((1-run_status),(npred,1)))/np.tile(Q,(1,nruns))



ranking = list(reversed([x for _,x in sorted(zip(Q,range(npred)))]))
print([s+1 for s in ranking])
