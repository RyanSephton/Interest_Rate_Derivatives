#pragma once
#include "Rate_Caplet.h"


/**
* Project:    Project 1
* Filename:   Rate_Cap.h
* Version:    v1 (9 March 2020)
* Author:     Ryan Sephton
* Summary:   Class to store several rate caplets.
*/
class Rate_Cap
{
private:
	// Attributes
	std::vector<Rate_Caplet> caplets; //Caplets that comprise the cap
	bool continuous_compounding{ false };
	std::vector<double> caplet_prices;
	std::vector<double> caplet_forward_rates;
	std::vector<double> interest_rates;
	std::vector<unsigned int> maturities;
	std::vector<double> caplet_volatilities;
	std::vector<double> caplet_strikes;


public:
	// Constructor & Destructor
	Rate_Cap(const std::vector<double>& strikes, const std::vector<double>& volatilities, const std::vector<double>& rates, std::vector<unsigned int>& time_of_rates, const bool& continuous);
	Rate_Cap(const std::vector<double>& strikes, const std::vector<double>& prices, std::vector<unsigned int>& time_of_rates, const std::vector<double>& rates, const bool& continuous);
	~Rate_Cap() {};

	// Getter & Print Methods
	std::vector<double> get_prices() { return caplet_prices; };
	std::vector<double> get_volatilities() { return caplet_volatilities; };
	void print_prices();
	void print_volatilities();
	void print_forward_rate();
};

