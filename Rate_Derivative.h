#pragma once
#include "Term_Structure.h"


/**
* Project:    Project 1
* Filename:   Rate_Derivative.h
* Version:    v1 (9 March 2020)
* Author:     Ryan Sephton
* Summary:    Class to implement interest rate derivatives.
*/

class Rate_Derivative {

protected:
	// Derivative Attributes
	Term_Structure derivative_term_struct; //term structure of the underlying
	bool continuous_compounding;
	double strike;
	double volatility;
	double forward_rate;
	double t_1;


	//Derivative Methods
	double cdf_normal(double x);
	void determine_volatility(const double& option_price); //Infers volatility based off fair price of option
	virtual double analytic_price(double vol)=0; // Implemented in child classes (Rate_Floorlet & Rate_Caplet)

    // Parameters for analytic pricing of derivatives (See Paul Wilmott, Financial Derivatives)
	double d1(const double& vol) { return 1.0 / vol / sqrt(t_1 / 365.) * (log(forward_rate / strike) + (vol*vol / 2.0) * (t_1 / 365.)); };
	double d2(const double& vol) { return d1(vol) - vol* sqrt(t_1 / 365.); }; 

public:
	// Constructors and Destructor
	Rate_Derivative(const double& strike_price, const double& vol, const double& rate_1, const unsigned int& time_of_rate_1, const double& rate_2, const unsigned int& time_of_rate_2, const bool& continuous);
	Rate_Derivative();
	~Rate_Derivative() {};

	//Getter Methods
	double get_fwd_rate() { return forward_rate; };
	double get_volatility() { return volatility; };
};