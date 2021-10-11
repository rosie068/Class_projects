# CS145 S21 project
## team 3: Jason Jewik(905304247), Daniel Park(104809832), Zatao Wu(805170781), Rosemary He(204753981)

This is the code for reproducing the results obtained from team 3 in the project assignment in CS 145 taught by Professor Wei Wang, and TA Zijie Huang.

We assume 2 files are in the same directory as the script: 
	
	1. train_trendency.csv provides data including Confirmed, Deaths, Active, etc. for each state from January to March
	
	2. population.csv provides the population of each state as of 2020. We obtained the source file from https://github.com/JoshData/historical-state-population-csv

To run the program, type:
	
	python SIR_model.py -t train_trendency.csv -p population.csv

Our model produces a prediction for both Confirmed and Deaths cases for each state for the month of April, 2021. We obtain confirmed predictions from a SIR model, in which the optimal parameters are estimated with 5000 iterations of simulated annealing for each state. The death predictions are from fitting a sigmoid line; if no line fits, then we trying fitting a 2nd degree polynomial. The output file is 'team3.csv'.
