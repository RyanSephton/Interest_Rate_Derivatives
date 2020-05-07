#include "Rate_Floorlet.h"

/**
* Project:    Project 1
* Filename:   Rate_Floorlet.cpp
* Version:    v1 (9 March 2020)
* Author:     Ryan Sephton
* Summary:    Class to implement put options on interest rates.
*/


/**
* Constructor for a put option on an underlying interest rate. 
* This option inherits from Rate_Derviative and stores an analytic price.
* @param strike_price const double reference, denotes the strike (or exercise) price of the option.
* @param vol const double reference, denotes the interest rate volatility.
* @param rate_1 const double reference, denotes the interest rate at time t_1.
* @param time_of_rate_1 const usigned int reference, denotes the time at which the first interest rate occurs.
* @param rate_2 const double reference, denotes the interest rate at time t_2.
* @param time_of_rate_2 const usigned int reference, denotes the time at which the second interest rate occurs.
* @param continuous const boolean reference, denotes whether interest is continuously(true) or discretely(false) compounded.
*/
Rate_Floorlet::Rate_Floorlet(const double& strike_price, const double& vol, const double& rate_1, const unsigned int& time_of_rate_1, const double& rate_2, const unsigned int& time_of_rate_2, const bool& continuous)
	:Rate_Derivative(strike_price, vol, rate_1, time_of_rate_1, rate_2, time_of_rate_2, continuous)
{
	price = analytic_price(volatility);
}


/**
* Constructor for a put option of known price but unknown volatility.
* This option inherits from Rate_Derviative and uses linear interpolation
* to determine the volatility of the option.
* @param strike_price const double reference, denotes the strike (or exercise) price of the option.
* @param floorlet_price const double reference, denotes the fair price of the option.
* @param rate_1 const double reference, denotes the interest rate at time t_1.
* @param rate_2 const double reference, denotes the interest rate at time t_2.
* @param time_of_rate_1 const usigned int reference, denotes the time at which the first interest rate occurs.
* @param time_of_rate_2 const usigned int reference, denotes the time at which the second interest rate occurs.
* @param continuous const boolean reference, denotes whether interest is continuously(true) or discretely(false) compounded.
*/
Rate_Floorlet::Rate_Floorlet(const double& strike_price, const double& floorlet_price, const double& rate_1, const double& rate_2, const unsigned int& time_of_rate_1, const unsigned int& time_of_rate_2, const bool& continuous)
	: Rate_Derivative(strike_price, 0.5, rate_1, time_of_rate_1, rate_2, time_of_rate_2, continuous) //arbitrary vol that will be updated upon construction
{
	determine_volatility(floorlet_price);
	price = analytic_price(volatility);
}


/**
* Default Constructor for a put option on an underlying interest rate.
* All parameters and the option price are initialised to 0.
*/
Rate_Floorlet::Rate_Floorlet()
	:Rate_Derivative()
{
	price = 0;
}

/**
* Function to determine the price of the floorlet analytically,
* given the option volatility. This implementation overrides the 
* pure virtual implementation in Rate_Derivative
*/
double Rate_Floorlet::analytic_price(double vol)
{
	double p_2 = derivative_term_struct.get_price_2(); 
	double price_at_vol = -p_2 * (forward_rate*cdf_normal(-1.*d1(vol)) - strike*cdf_normal(-1.*d2(vol)));
	return price_at_vol;
}

