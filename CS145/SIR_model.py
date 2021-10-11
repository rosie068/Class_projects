import os
import pandas as pd
import glob
import numpy as np
import matplotlib.pyplot as plt
import datetime as dt
from scipy import integrate, stats, optimize
import sys
import argparse

# MAPE calculation function
def mape(actual, pred): 
    actual, pred = np.array(actual), np.array(pred)
    return np.mean(np.abs((actual - pred) / actual)) * 100

# ODE for the SIR model
def deriv(y, t, N, beta, gamma):
    S, I, R = y
    dSdt = -beta * S * I / N
    dIdt = beta * S * I / N - gamma * I
    dRdt = gamma * I
    return dSdt, dIdt, dRdt

'''
# ODE for SIRD model, did not perform as well as SIR + poly fit, not used
#SIRD model with new death_rate alpha
def deriv(y, t, N, beta, gamma, alpha):
    S, I, R, D = y
    dSdt = -beta * S * I / N
    dIdt = beta * S * I / N - gamma * I  - alpha * D
    dRdt = gamma * I
    dDdt = alpha * D
    return dSdt, dIdt, dRdt, dDdt
'''

def sigmoid(x, L ,x0, k, b):
    y = L / (1 + np.exp(-k*(x-x0)))+b
    return (y)

#replaced by polynomial fit
def linear(x, b, m):
    return m*x+b

def Get_MAPE (x):
    contact_rate = x[0]
    recover_rate = x[1]
    ret = integrate.odeint(deriv, y0, t, args=(population, contact_rate, recover_rate))
    S, I, R = ret.T
    pred_confirmed_cases = np.round(population - S) 
    #If you are not susceptible, assume you had contracted covid **NOT INCLUDING VACCINES**
    return mape(state_train_data["Confirmed"][-test_days_split:], pred_confirmed_cases[-test_days_split:])

def Predict_April (x):
    contact_rate = x[0]
    recover_rate = x[1]
    t_total = np.linspace(0, train_days+april_days, train_days+april_days)
    ret = integrate.odeint(deriv, y0, t_total, args=(population, contact_rate, recover_rate))
    S, I, R = ret.T

    return np.round(population - S[-30:])

'''
###get mape and prediction based on SIRD model, did not performa as well so not used
def Get_MAPE_SIRD (x):
    contact_rate = x[0]
    recover_rate = x[1]
    death_rate = x[2]
    ret = integrate.odeint(deriv, y0, t, args=(pop, contact_rate, recover_rate, death_rate))
    S, I, R, D = ret.T
    pred_confirmed_cases = np.round(pop - S) # INCLUDING VACCINES
    pred_death_cases = np.round(D) # INCLUDING VACCINES
    #avg_mape = mape(state_train_data["Confirm+Vaccine"][-test_days_split:], 
        pred_confirmed_cases[-test_days_split:]) + mape(state_train_data["Deaths"][-test_days_split:], 
        pred_death_cases[-test_days_split:])
    sum_mape = mape(state_train_data["Confirmed"][-test_days_split:], 
        pred_confirmed_cases[-test_days_split:]) + mape(state_train_data["Deaths"][-test_days_split:], 
        pred_death_cases[-test_days_split:])

    return sum_mape

def Predict_April_SIRD (x):
    contact_rate = x[0]
    recover_rate = x[1]
    death_rate = x[2]
    t_total = np.linspace(0, train_days+april_days, train_days+april_days)
    ret = integrate.odeint(deriv, y0, t_total, args=(pop, contact_rate, recover_rate, death_rate))
    S, I, R, D = ret.T

    return np.round(pop - S[-30:]), np.round(D[-30:])
'''

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='SIR model for Team 3 in CS 145 Spring \'21 that takes in 3 files')
    parser.add_argument('-t', '--train_trendency', required=True, dest='train',
                        help='File containing the basic training data.')
    parser.add_argument('-p', '--population', required=True, dest='population',
                        help='Output file name.')
    parser.add_argument('-v', '--train_vaccine', required=False, dest='vaccine',
                        help='File containg vaccine information, not used in current version of model, room for further improvement')
    args = parser.parse_args()
    train_data_path = args.train
    #vaccine_data_path = args.vaccine
    population_data_path = args.population

    #read in training files
    train_data = pd.read_csv(train_data_path)
    #vaccine_data = pd.read_csv(vaccine_data_path)
    state_pop_data_2020 = pd.read_csv(population_data_path)

    train_data = train_data.drop("Unnamed: 0", axis=1)

    submission_confirmed_df = pd.DataFrame(columns=["Confirmed"], index=range(1500))
    index_names = train_data[ train_data['Date'] != "01-12-2021" ].index
    day_1_train_data = train_data.drop(index_names)
    day_1_train_data = day_1_train_data.dropna(subset=["Recovered"])

    state_id = 0

    train_days = 79 #len(state_train_data["Date"].index)
    train_days_split = int(train_days * 0.8)
    test_days_split = train_days - train_days_split
    april_days = 30
    sum_mape = 0
    num_states = 50

    ## predicting Confirmed with SIR model
    for i, row in state_pop_data_2020.iterrows():
        state = row['State']
        #print(state)
        population = row['Population']
        index_names = train_data[ train_data['Province_State'] != state ].index
        state_train_data = train_data.drop(index_names)

        # Initial number of infected and recovered individuals, I0 and R0.
        I0, R0 = state_train_data["Active"].iloc[0], state_train_data["Recovered"].iloc[0]
        if pd.isnull(R0):
            R0 = 0
        S0 = population - I0 - R0
        # A grid of time points (in days)
        t = np.linspace(0, train_days, train_days)
        # Initial conditions vector
        y0 = S0, I0, R0

        # Reasonings for bounds: 
        # Contact rate: based on previous runs, the value is never over 0.2, so I bounded at 0.3
        # Recovery Rate: it usually takes at least 1-2 weeks to get over covid. 1/7 = 0.14
        x_min = optimize.dual_annealing(Get_MAPE, bounds=[[0,0.3],[0,0.15]], seed=1234, x0=[0,0], maxiter=5000)
        #print(x_min.fun)
        #print(x_min.x)
        sum_mape = sum_mape + x_min.fun
        april_confirmed_data = Predict_April(x_min.x)

        for index, num_confirmed_case in enumerate(april_confirmed_data):
            submission_confirmed_df.loc[(index * 50) + state_id] = [num_confirmed_case]

        state_id = state_id + 1

        mean_test_mape = sum_mape / num_states

    #death prediction based on sigmoid + polynomial optimization
    submission_deaths_df = pd.DataFrame(columns=["Deaths"], index=range(1500))
            
    # Using sigmoid to fit curve
    state_id = 0
    sum_mape = 0
    num_states = 50

    for i, row in state_pop_data_2020.iterrows():
        state = row['State']
        index_names = train_data[ train_data['Province_State'] != state ].index
        state_train_data = train_data.drop(index_names)
        state_death_data = state_train_data["Deaths"]

        train_days = range(len(state_death_data))
        total_days = range(len(state_death_data)+30)
        p0 = [max(state_death_data), np.median(train_days), 1, min(state_death_data)]
        try: 
            popt, pcov = optimize.curve_fit(sigmoid, train_days, state_death_data, p0)
            #Predict April Data
            april_deaths_data = np.round(sigmoid(total_days, *popt)[-30:])
        except:
            #If sigmoid function can't fit, then use polynomial
            fitted_values = []
            fitted_graph = np.poly1d(np.polyfit(train_days, state_death_data, 2))

            for day in total_days:
                fitted_values.append(np.round(fitted_graph(day)))

            april_deaths_data = fitted_values[-30:]

        for index, num_deaths in enumerate(april_deaths_data):
            submission_deaths_df.loc[(index * 50) + state_id] = [num_deaths]

        state_id = state_id + 1

    # Save Data
    submission_df = pd.DataFrame(index=range(1500))
    submission_df['ID'] = submission_df.index.values
    submission_df['Confirmed'] = submission_confirmed_df.iloc[:,0].values
    submission_df['Deaths'] = submission_deaths_df.iloc[:,0].values
    submission_path = "Team3.csv"
    submission_df.to_csv(submission_path, index=False)

