// Project1.cpp : Defines the entry point for the console application.
//
#include "Bond.h"
#include "Term_Structure.h"
#include "Rate_Cap.h"
#include "Rate_Floor.h"
#include <iostream>

/**
* Project:    Project 1
* Filename:   main_P1.cpp
* Version:    v1 (9 March 2020)
* Author:     Ryan Sephton
* Summary:    Solution to Project 1
*/


void print_error_messages(int error_code) //Function to print error messages to console
{
	if (error_code == 1)
	{
		std::cout << "ERROR: second rate should occur chronologically after the first, i.e. t_1 < t_2.";
	}
	if (error_code == 2)
	{
		std::cout << "ERROR: All rates, times, volatilities and compounding frequency must be positive.";
	}
	if (error_code == 3)
	{
		std::cout << "ERROR: To initialise N options, user must supply N volatilities and strikes as well as, N+1 interest rates and corresponding times.";
	}
	if (error_code == 4)
	{
		std::cout << "ERROR: Principal of a bond msut be a positive number.";
	}
	return;
}


int main()
{	
	try
	{
		//Scenario 1
		std::cout << "Scenario 1:" << std::endl;
		std::vector<double> scenario_1_interest_rates(8, 0.069395);
		std::vector<double> scenario_1_volatilities(7, 0.2);
		std::vector<unsigned int> scenario_1_times_of_rates{ 91,182,273,365,456,547,638,730 };

		for (double i = 0.07; i <= 0.09; i += 0.01)
		{
			std::cout << "\n Strike " << i << std::endl;
			std::vector<double> scenario_1_my_strikes(7, i);
			Rate_Cap scenario_1_caps = Rate_Cap(scenario_1_my_strikes, scenario_1_volatilities, scenario_1_interest_rates, scenario_1_times_of_rates, true);
			Rate_Floor scenario_1_floors = Rate_Floor(scenario_1_my_strikes, scenario_1_volatilities, scenario_1_interest_rates, scenario_1_times_of_rates, true);
			scenario_1_caps.print_prices();
			scenario_1_floors.print_prices();
		}



		//Scenario 2
		std::cout << "\n \n Scenario 2: " << std::endl;
		std::vector<double> scenario_2_interest_rates{ 0.0622136643736064, 0.0651662751075432, 0.0685099346423619, 0.0710649661757764,0.0732256419305335, 0.0747963097299377, 0.0757776640037515, 0.0764644687689206 };
		std::vector<unsigned int> scenario_2_times_of_rates{ 91,182,273,365,456,547,638,730 };
		std::vector<double> scenario_2_volatilities{ 0.1533, 0.1731, 0.1727, 0.1752, 0.1809, 0.1800, 0.1805 };
		std::vector<double> scenario_2_strikes(7, 0.059);

		Rate_Cap scenario_2_caps = Rate_Cap(scenario_2_strikes, scenario_2_volatilities, scenario_2_interest_rates, scenario_2_times_of_rates, true);
		Rate_Floor scenario_2_floors = Rate_Floor(scenario_2_strikes, scenario_2_volatilities, scenario_2_interest_rates, scenario_2_times_of_rates, true);

		scenario_2_caps.print_prices();
		scenario_2_floors.print_prices();



		// Scenario 3
		std::cout << "\n \n Scenario 3" << std::endl;
		std::vector<double> scenario_3_caplet_prices{ 0.0004, 0.001, 0.0017, 0.0022, 0.0027, 0.0033, 0.0038 };
		Rate_Cap scenario_3_caps = Rate_Cap(scenario_2_strikes, scenario_3_caplet_prices, scenario_2_times_of_rates, scenario_2_interest_rates, true);
		scenario_3_caps.print_volatilities();


		// Bonus Question
		std::cout << "\n \n Bonus Question" << std::endl;
		std::vector<double> bonus_caplet_prices{ 0.015, 0.022, 0.024, 0.029, 0.029, 0.03, 0.025 };
		Rate_Cap bonus_question_caps = Rate_Cap(scenario_2_strikes, bonus_caplet_prices, scenario_2_times_of_rates, scenario_2_interest_rates, true);
		bonus_question_caps.print_volatilities();
		
	}
	catch (int e)
	{
		print_error_messages(e);
	}
	getchar();
	return 0;
}

