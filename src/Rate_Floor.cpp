#include "Rate_Floor.h"
#include <iostream>

/**
* Project:    Project 1
* Filename:   Rate_Floor.cpp
* Version:    v1 (9 March 2020)
* Author:     Ryan Sephton
* Summary:    Class to store multiple interest rate floorlets.
*/


/**
* Constructor for a rate floor, which is comprised of a set of rate floorlets,
* that denote a put option on an interest rate. Each floorlet has a fair price,
* volatility, and a strike price as well as some other dependencies (see Rate_Floorlet.cpp).
* @param strikes const vector double reference, denotes the strike (or exercise) prices of the options.
* @param volatilities const vector double reference, denotes the interest rate volatilities.
* @param rates const double reference, denotes the interest rates at each discrete time interval t_i.
* @param time_of_rates const vector usigned int reference, denotes the times at which each of the rates occurs.
* @param continuous const boolean reference, denotes whether interest is continuously(true) or discretely(false) compounded.
*/
Rate_Floor::Rate_Floor(const std::vector<double>& strikes, const std::vector<double>& volatilities, const std::vector<double>& rates, std::vector<unsigned int>& time_of_rates, const bool& continuous)
{
	int n_rates = rates.size();
	int n_strikes = strikes.size();
	int n_volatilities = volatilities.size();
	int n_dates = time_of_rates.size();

	if (((n_rates - 1) != n_strikes) || (n_volatilities != n_strikes) || ((n_rates - 1) != n_volatilities) || (n_dates != n_rates) || ((n_dates - 1) != n_strikes))
	{
		throw 3; // Check that for N options, N+1 rates and times, as well as, N strikes and volatilities have been given.
	}

	interest_rates = rates;
	floorlet_strikes = strikes;
	maturities = time_of_rates;
	floorlet_volatilities = volatilities;
	continuous_compounding = continuous;

	std::vector<Rate_Floorlet> floors(n_strikes, Rate_Floorlet());
	std::vector<double> prices(n_strikes, 0);
	std::vector<double> forward_rates(n_strikes, 0);
	
	// Construct each floorlet based on the given parameters-store the prices and forward rates.
	if (continuous_compounding)
	{
		for (int i = 0; i < n_strikes; i++)
		{
			Rate_Floorlet floorlet = Rate_Floorlet(floorlet_strikes.at(i), floorlet_volatilities.at(i), interest_rates.at(i), maturities.at(i), interest_rates.at(i + 1), maturities.at(i + 1), true);
			floors.at(i) = floorlet;
			prices.at(i) = floorlet.get_price();
			forward_rates.at(i) = floorlet.get_fwd_rate();
		}
	}
	else {

		for (int i = 0; i < n_strikes; i++)
		{
			Rate_Floorlet floorlet = Rate_Floorlet(floorlet_strikes.at(i), floorlet_volatilities.at(i), interest_rates.at(i), maturities.at(i), interest_rates.at(i + 1), maturities.at(i + 1), false);
			floors.at(i) = floorlet;
			prices.at(i) = floorlet.get_price();
			forward_rates.at(i) = floorlet.get_fwd_rate();
		}
	}
	floorlet_forward_rates = forward_rates;
	floorlets = floors;
	floorlet_prices = prices;
}


/**
* Constructor for a rate floor, which is comprised of a set of rate floorlets,
* this constructor takes in the price of each floorlet and calculates the corresponding 
* volatility.Each floorlet has a fair price, volatility, and a strike price as well as 
* some other dependencies (see Rate_Floorlet.cpp).
* @param strikes const vector double reference, denotes the strike (or exercise) prices of the options.
* @param prices const vector double reference, denotes the price of each floorlet.
* @param time_of_rates const vector usigned int reference, denotes the times at which each of the rates occurs.
* @param rates const double reference, denotes the interest rates at each discrete time interval t_i.
* @param continuous const boolean reference, denotes whether interest is continuously(true) or discretely(false) compounded.
*/
Rate_Floor::Rate_Floor(const std::vector<double>& strikes, const std::vector<double>& prices, std::vector<unsigned int>& time_of_rates, const std::vector<double>& rates, const bool& continuous)
{
	int n_rates = rates.size();
	int n_strikes = strikes.size();
	int n_prices = prices.size();
	int n_dates = time_of_rates.size();

	if (((n_rates - 1) != n_strikes) || (n_prices != n_strikes) || ((n_rates - 1) != n_prices) || (n_dates != n_rates) || ((n_dates - 1) != n_strikes))
	{
		throw 3; // Check that for N options, N+1 rates and times, as well as, N strikes and volatilities have been given.
	}

	interest_rates = rates;
	floorlet_strikes = strikes;
	maturities = time_of_rates;
	floorlet_prices = prices;
	continuous_compounding = continuous;

	std::vector<Rate_Floorlet> floors(n_strikes, Rate_Floorlet());
	std::vector<double> volatilities(n_strikes, 0);
	std::vector<double> forward_rates(n_strikes, 0);

	// Construct each floorlet based on the given parameters-store the prices and forward rates.
	if (continuous_compounding)
	{
		for (int i = 0; i < n_strikes; i++)
		{
			Rate_Floorlet floorlet = Rate_Floorlet(floorlet_strikes.at(i), prices.at(i), interest_rates.at(i), interest_rates.at(i + 1), maturities.at(i), maturities.at(i + 1), true);
			floors.at(i) = floorlet;
			volatilities.at(i) = floorlet.get_volatility();
			forward_rates.at(i) = floorlet.get_fwd_rate();
		}
	}
	else {

		for (int i = 0; i < n_strikes; i++)
		{
			Rate_Floorlet floorlet = Rate_Floorlet(floorlet_strikes.at(i), prices.at(i), interest_rates.at(i), interest_rates.at(i + 1), maturities.at(i), maturities.at(i + 1), false);
			floors.at(i) = floorlet;
			volatilities.at(i) = floorlet.get_volatility();
			forward_rates.at(i) = floorlet.get_fwd_rate();
		}
	}

	floorlet_forward_rates = forward_rates;
	floorlets = floors;
	floorlet_volatilities = volatilities;
}


/**
* Function to print the fair prices of the floorlets to the console.
*/
void Rate_Floor::print_prices()
{
	std::cout << "Floor Prices: ";
	for (auto &p : floorlet_prices)
	{
		std::cout << p << " ";
	}
	std::cout << std::endl;
	return;
}

/**
* Function to print the inferred volatilities to the console.
*/
void Rate_Floor::print_volatilities()
{
	std::cout << "Floor Volatilities: ";
	for (auto &p : floorlet_volatilities)
	{
		std::cout << p << " ";
	}
	std::cout << std::endl;
	return;
}


/**
* Function to print the forward rates to the console.
*/
void Rate_Floor::print_forward_rate()
{
	std::cout << "Forward Rates: ";
	for (auto &p : floorlet_forward_rates)
	{
		std::cout << p << " ";
	}
	std::cout << std::endl;
	return;
}
