
#include "Rate_Derivative.h"


/**
* Project:    Project 1
* Filename:   Rate_Caplet.h
* Version:    v1 (9 March 2020)
* Author:     Ryan Sephton
* Summary:    Class to implement call options on interest rates.
*/
class Rate_Caplet :
	public Rate_Derivative
{
protected: 
	// Attributes
	double price;

	// Methods
	double analytic_price(double vol);
	

public:
	// Constructors & Destructor
	Rate_Caplet(const double& strike_price, const double& vol, const double& rate_1, const unsigned int& time_of_rate_1, const double& rate_2, const unsigned int& time_of_rate_2, const bool& continuous);
	Rate_Caplet(const double& strike_price, const double& caplet_price, const double& rate_1, const double& rate_2, const unsigned int& time_of_rate_1, const unsigned int& time_of_rate_2, const bool& continuous);
	Rate_Caplet();
	~Rate_Caplet() {};

	// Getter Method
	double get_price() { return price; };
};
