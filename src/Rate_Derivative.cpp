#include "Rate_Derivative.h"
#include <cmath>
#include <iostream>

/**
* Project:    Project 1
* Filename:   Rate_Derivative.cpp
* Version:    v1 (9 March 2020)
* Author:     Ryan Sephton
* Summary:    Class to implement interest rate derivatives.
*/

/**
* Constructor for an interest rate derivative, which is comprised of various
* parameters associated with the derivatives and a term structure (see term_structure.cpp).
* @param strike_price const double reference, denotes the strike (or exercise) price of the option.
* @param vol const double reference, denotes the interest rate volatility.
* @param rate_1 const double reference, denotes the interest rate at time t_1.
* @param time_of_rate_1 const usigned int reference, denotes the time at which the first interest rate occurs.
* @param rate_2 const double reference, denotes the interest rate at time t_2.
* @param time_of_rate_2 const usigned int reference, denotes the time at which the second interest rate occurs.
* @param continuous const boolean reference, denotes whether interest is continuously(true) or discretely(false) compounded.
*/
Rate_Derivative::Rate_Derivative(const double& strike_price, const double& vol, const double& rate_1, const unsigned int& time_of_rate_1, const double& rate_2, const unsigned int& time_of_rate_2, const bool& continuous)
{
	if (time_of_rate_2 < time_of_rate_1)
	{
		throw 1; // t_2 should come chronologically after t_1.
	}
	else if (rate_2 < 0. || rate_1< 0. || time_of_rate_1 < 0 || time_of_rate_2 < 0 || strike_price <0. || vol < 0.)
	{
		throw 2; // Ensure all parameters take reasonable values.
	}

	strike = strike_price;
	volatility = vol;
	derivative_term_struct = Term_Structure(rate_1, time_of_rate_1, rate_2, time_of_rate_2);
	continuous_compounding = continuous;

	if (continuous_compounding)
	{
		forward_rate = derivative_term_struct.get_continuous_fwd_rate();
	}
	else {
		forward_rate = derivative_term_struct.get_discrete_fwd_rate();
	}

	t_1 = derivative_term_struct.get_t1(); //Needed for pricing the derivative.
}


/**
* Default constructor to set up an empty interest rate derivative,
* with all values initialised to 0.
*/
Rate_Derivative::Rate_Derivative()
{
	strike = 0;
	volatility = 0;
	forward_rate = 0;
	t_1 = 0;
	derivative_term_struct = Term_Structure();
}


/**
* Function to compute the CDF of the standard normal distribution.
*/
double Rate_Derivative::cdf_normal(double x)
{
	return 0.5 * erfc(-x / sqrt(2));
}


/**
* Function to linearly interpolate the derivatives volatility,
* given the derivatives term structure and fair price.
* @param option_price const double reference, denotes the fair price of the option
*/
void Rate_Derivative::determine_volatility(const double& option_price)
{
	// See accompanying pdf for further details
	// Choose interval [a,b] s.t. y(a)y(b)<0
	double x_1 = 0.000000001;
	double y_1 = analytic_price(x_1) - option_price;
	double x_2 = 0.99999999999;
	double y_2 = analytic_price(x_2) -option_price;

	if (y_1*y_2 > 0)
	{
		volatility = std::numeric_limits<double>::quiet_NaN();
		std::cout << "WARNING: negative volatility implied. NaN volatility returned." << std::endl;
		return;
	}
	double x_opt{ 0 };
	double y_opt{ 100 };

	double tolerance = 0.000000001; //Define breakout tolerance
	while (abs(y_opt) > tolerance)
	{
		// Linearly constrict interval to converge on root
		x_opt = (1. / (abs(y_2) + abs(y_1))) * ((x_2 * abs(y_1)) + (x_1 * abs(y_2)));
		y_opt = analytic_price(x_opt) - option_price;

		if (y_opt > 0)
		{
			x_2 = x_opt;
			y_2 = y_opt;
		}
		else {
			x_1 = x_opt;
			y_1 = y_opt;
		}

	}
	
	volatility = x_opt;
	return;
}
