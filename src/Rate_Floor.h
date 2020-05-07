#pragma once
#include "Rate_Floorlet.h"


/**
* Project:    Project 1
* Filename:   Rate_Floor.h
* Version:    v1 (9 March 2020)
* Author:     Ryan Sephton
* Summary:    Class to store multiple interest rate floorlets.
*/
class Rate_Floor
{
private:
	// Attributes
	std::vector<Rate_Floorlet> floorlets; //Floorlets that comprise the floor
	bool continuous_compounding{ false };
	std::vector<double> floorlet_prices;
	std::vector<double> floorlet_forward_rates;
	std::vector<double> interest_rates;
	std::vector<unsigned int> maturities;
	std::vector<double> floorlet_volatilities;
	std::vector<double> floorlet_strikes;


public:
	// Constructor & Destructor
	Rate_Floor(const std::vector<double>& strikes, const std::vector<double>& volatilities, const std::vector<double>& rates, std::vector<unsigned int>& time_of_rates, const bool& continuous);
	Rate_Floor(const std::vector<double>& strikes, const std::vector<double>& prices, std::vector<unsigned int>& time_of_rates, const std::vector<double>& rates, const bool& continuous);
	~Rate_Floor() {};

	// Getter & Print Methods
	std::vector<double> get_prices() { return floorlet_prices; };
	std::vector<double> get_volatilities() { return floorlet_volatilities; };
	void print_prices();
	void print_volatilities();
	void print_forward_rate();
};

