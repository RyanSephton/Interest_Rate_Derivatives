#include "Term_Structure.h"
#include <cmath>

/**
* Project:    Project 1
* Filename:   Term_Structure.cpp
* Version:    v1 (9 March 2020)
* Author:     Ryan Sephton
* Summary:    Class to implement interest rate term structures.
*/


/**
* Constructor for a general interest rate term structure, based on the price
* of two zero coupon bonds. Term Structure can be used to calculate forward rates.
* @param zcb_1 const bond reference, denotes the first zero coupon bond.
* @param expiry_1 const usigned int reference, denotes the maturity of the first zcb.
* @param zcb_2 const bond reference, denotes the second zero coupon bond.
* @param expiry_2 const usigned int reference, denotes the maturity of the second zcb.
* @param freq const int reference, denotes the compounding frequency.
*/
Term_Structure::Term_Structure(Bond& zcb_1, const unsigned int& expiry_1, Bond& zcb_2, const unsigned int& expiry_2, const int& freq)
{
	double p_1 = zcb_1.get_price();
	double p_2 = zcb_2.get_price();

	if (expiry_2 <= expiry_1)
	{
		throw 1; // Second rate should occur chronologically after the first, i.e. t_1 < t_2.
	}
	else if (p_1 <= 0. || p_2 <= 0. || expiry_1 <= 0 || expiry_2 <= 0 || freq <= 0)
	{
		throw 2; // All rates, times, and compounding frequency must be positive.
	}
	
	// Check if principal is different on the ZCB's
	float principal_1 = zcb_1.get_principal();
	float principal_2 = zcb_2.get_principal();

	if (abs(principal_1 - principal_2)>= 0.001)
	{
		// Normalise the principals
		p_1 = p_1 / principal_1;
		p_2 = p_2 / principal_2;
	}

	price_1 = p_1;
	price_2 = p_2;
	t_1 = expiry_1;
	t_2 = expiry_2;
	compounding_frequency = freq;
}


/**
* Constructor for a general interest rate term structure, based on two consecutive
* interest rates. Term Structure can be used to calculate forward rates.
* @param r_1 const double reference, denotes the first interest zero rate.
* @param time_of_rate_1 const usigned int reference, denotes the time of rate 1.
* @param r_2 const double reference, denotes the second interest zero rate.
* @param time_of_rate_2 const usigned int reference, denotes the time of rate 2.
* @param freq const int reference, denotes the compounding frequency.
*/
Term_Structure::Term_Structure(const double& r_1, const unsigned int& time_of_rate_1, const double& r_2, const unsigned int& time_of_rate_2, const  int& freq) 
{
	if (time_of_rate_2 <= time_of_rate_1)
	{
		throw 1; // Second rate should occur chronologically after the first, i.e. t_1 < t_2.
	}else if (r_1 <= 0 || r_2 <= 0 || time_of_rate_1 <= 0 || time_of_rate_2 <= 0 || freq <= 0)
	{
		throw 2; // All rates, times, and compounding frequency must be positive.
	}

	compounding_frequency = freq;
	t_1 = time_of_rate_1;
	t_2 = time_of_rate_2;


	// convert rates to ZCB prices
	double T_1 = time_of_rate_1 / double(365);
	price_1 = exp(-1 *r_1*T_1);
	double T_2 = time_of_rate_2 / double(365);
	price_2 = exp(-1 * r_2*T_2);
}

/**
* Default constructor that sets up an empty term structure.
* Compounding frequency is initialised to 4 by assumption of the project.
*/
Term_Structure::Term_Structure()
{
	compounding_frequency = 4;
	price_1 = 0;
	price_2 = 0;
	t_1 = 0;
	t_2 = 0;
	continuous_forward_rate = 0;
	discrete_forward_rate = 0;
}

/**
* Function to calculate the forward rates based on the two consecutive zero rates.
*/
void Term_Structure::calculate_rates()
{
	// For more details on this calculation- see accompanying pdf.
	int n_compound_increments = int(((t_2 - t_1)*compounding_frequency) / 365.); 
	discrete_forward_rate = (pow((price_1 / price_2), 1. / n_compound_increments)-1)*compounding_frequency; // f=n((p_1/p_2)^(1/n_compounds)-1)
	continuous_forward_rate = log(price_1 / price_2) / double((t_2 - t_1)/365.);
	return;
}

/**
* Getter function to return forward rate based on continuous compounding.
*/
double Term_Structure::get_continuous_fwd_rate()
{
	calculate_rates();
	return continuous_forward_rate;
}

/**
* Getter function to return forward rate based on discrete compounding.
*/
double Term_Structure::get_discrete_fwd_rate()
{
	calculate_rates();
	return discrete_forward_rate;
}
