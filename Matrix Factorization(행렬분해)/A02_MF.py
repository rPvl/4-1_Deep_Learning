import numpy as np


class MF():
    def __init__(self):
        ### currently Do Nothing here
        ### R, P, Q are the placeholder for matrix R, P, Q respectively
        ### a 2-dimensional array(or list) is expected for each matrix
        self.R = None
        self.P = None
        self.Q = None

    def train(self, R, k, reg_lambda, lr_alpha, max_iterations, epsilon):
        ############# Write code here - begin
        self.P = np.random.normal(0.0, 1/k, size=(255, k)) #평균0.0, 표준편차 1/k. 가우시안 분포 랜덤 샘플링. 크기 255 * k
        self.Q = np.random.normal(0.0, 1/k, size=(255, k))
        #print(type(self.P))

        #iteration 이전 L 값
        Lprev = 0
        for u in range(255):
            for i in range(255):
                if R[u, i] > 0:
                    Lprev += (R[u, i] - np.dot(self.P[u, :], self.Q[i, :])) ** 2 + reg_lambda * (np.sum(self.P[u, :] ** 2) + np.sum(self.Q[i, :] ** 2))

        for i_max in range(max_iterations):#훈련 횟수
            for u in range(255):
                for i in range(255):
                    if R[u,i]>0:#행렬 값이 0보다 큰것만
                        #값 업데이트. 행 = 행 - alpha * 목적함수를 행에 대해 미분한 것.
                        #array 특정 행 접근 방법 : array[i,:]
                        self.P[u,:] = self.P[u,:] + 2 * lr_alpha * ((R[u,i] - np.dot(self.P[u,:], self.Q[i,:])) * self.Q[i,:] - reg_lambda * self.P[u,:])
                        self.Q[i,:] = self.Q[i,:] + 2 * lr_alpha * ((R[u,i] - np.dot(self.P[u,:], self.Q[i,:])) * self.P[u,:] - reg_lambda * self.Q[i,:])
            #목적함수
            Lcur=0
            for u in range(255):
                for i in range(255):
                    if R[u,i]>0:
                        Lcur += (R[u,i] - np.dot(self.P[u,:],self.Q[i,:]))**2 + reg_lambda * (np.sum(self.P[u,:] **2) + np.sum(self.Q[i,:] **2))

            print("i_max : ",(i_max+1))
            print("abs(Lcur - Lprev) : ", abs(Lcur - Lprev))

            if abs(Lcur - Lprev) < epsilon:
                break

            #이전 반복후 계산된 값 넘기기
            Lprev=Lcur

        ############# Write code here - end
        return 1


    def P_MultipliedBy_Q_Transpose(self):
        ############# Write code here - begin
        ##### You should calculate "reconst_R = P * Q^T"
        ############# Write code here - end
        reconst_R=np.dot(self.P, self.Q.T)#행렬 곱.  Q.T:전치행렬

        ### a 2-dimensional array(or list) is expected for reconst_R
        return reconst_R