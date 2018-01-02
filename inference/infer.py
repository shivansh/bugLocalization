import re
import numpy as np
import parse_output

N_ITER = 20
eps = 1e-3

A = parse_output.get_A()#read data
# The parts of data will be truth probabilities

npred = A.shape[0]
print("Number of predicates:", npred)
nruns = A.shape[1]
print("Number of runs:", nruns)

# run_status = np.ones(shape=[nruns]) # 0 means fail, 1 means true
run_status = parse_output.get_run_status() # 0 means fail, 1 means true
ind_zero = np.where(run_status==0)[0] # Indices for failed runs
print("Number of failed runs:", ind_zero.size)
ind_nonzero = np.where(run_status!=0)[0] # Indices for successful runs
print("Number of failed runs:", ind_nonzero.size)


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
        # if i == 0 and n_iter == 0:
        #     print(R_kj)
        #     print(R_ij)
        #     print(F[i])
        F_compl[i] = np.sum((1-A[i,ind_zero])*(R_compl_ij/(R_compl_kj+eps)))
        
        R_kj = np.sum(R[:, ind_nonzero], axis=0)
        R_ij = R[i,ind_nonzero]
        R_compl_kj = np.sum(R_compl[:, ind_nonzero], axis=0)
        R_compl_ij = R_compl[i,ind_nonzero]
        S[i] = np.sum(A[i,ind_nonzero]*(R_ij/(R_kj+eps)))
        S_compl[i] = np.sum((1-A[i,ind_nonzero])*(R_compl_ij/(R_compl_kj+eps)))

    R = A*np.tile((1-run_status),(npred,1))*np.transpose(np.tile(Q,(nruns,1))) + A*np.tile((run_status),(npred,1))/np.transpose(np.tile(Q,(nruns,1)) + eps)
    R_compl = (1-A)*np.tile((1-run_status),(npred,1))*np.transpose(np.tile(Q_compl,(nruns,1))) + (1-A)*np.tile((run_status),(npred,1))/np.transpose(np.tile(Q_compl,(nruns,1)) + eps)

    # if n_iter==N_ITER-1 or n_iter==N_ITER-2:
    #     print(Q)
    #     print(list(reversed([x for _,x in sorted(zip(Q,range(npred)))])))


ranking = list(reversed([x for _,x in sorted(zip(Q,range(npred)))]))
print("Order of the predicates according to quality:")
print([s+1 for s in ranking])
