#include "Bond.h"
#include <iostream>

/**
* Project:    Project 1
* Filename:   Bond.cpp
* Version:    v1 (9 March 2020)
* Author:     Ryan Sephton
* Summary:    Class to implement bonds.
*/


/**
* Constructor for a general (coupon-paying) bond.
* Used to construct the fair price for the bond given a set of coupons,
* payment dates, the principal, and the expiry.
* @param coupon_payments const vector float reference, denotes the magnitude of the coupons.
* @param payment_dates const vector unsigned int reference, denotes the dates of each coupon payment.
* @param princpal const float reference, denotes the principal of the bond, paid at maturity.
* @param expiry const const unsigned int reference, denotes the expiry date of the bond.
*/
Bond::Bond(std::vector<float>& coupon_payments, const std::vector <unsigned int>& payment_dates, const float& princpal, const unsigned int& expiry)
{

	if (princpal <= 0) 
	{
		throw 4;
	}
	
	if (coupon_payments.size() == payment_dates.size()) //Check each coupon is paid on a corresponding date
	{
		maturity = expiry;
		principal = princpal;
		zero_coupon = false;
		coupon_payments.at(coupon_payments.size()-1) += princpal;   // Due to simplifying assumption, last coupon also pays back the principal amount
		coupons = coupon_payments;
		coupon_dates = payment_dates;
	}
	else {
		throw 3;
	}

	
}


/**
* Constructor for a zero coupon bond.
* Used to construct the fair price for the bond given the principal, interest rate, and the expiry.
* @param princpal const float reference, denotes the principal of the bond, paid at maturity.
* @param interest_rate const float reference, denotes the zero rate from t=0 to t=T.
* @param expiry const const unsigned int reference, denotes the expiry date of the bond.
*/
Bond::Bond(const float& princpal, const float& interest_rate, const unsigned int& expiry)
{

	if (princpal <= 0) // see other constructor.
	{
		throw 1;
	}
	maturity = expiry;
	principal = princpal;
	interest_rates.at(0) = interest_rate;
	zero_coupon = true;
}

Bond::~Bond()
{
	// Destructor
}

/**
* Function to return the fair price of a bond to the user (ZCB or coupon-paying)
* @param interests vector float reference, denotes the interest rates at the time of each
*        coupon (for ZCB this will be one rate).
* @param expiry const const unsigned int reference, denotes the expiry date of the bond.
*/
float Bond::get_price(const std::vector<float>& interests, const unsigned int& expiry)
{
	for (unsigned int i = 0; i < interests.size(); i++)
	{
		if (interests.at(i) > 1)
		{
			throw 2; // Make sure all of the interest rates are expressed as decimals i.e. 5.8% -> 0.058
		}
	}
	maturity = expiry;
	interest_rates = interests;
	price();
	float bond_value = float(bond_price); // Calculate the bond price
	return bond_value;
}

/**
* Function to return the fair price of a bond to the user (ZCB or coupon-paying)
*/
float Bond::get_price()
{
	price();
	float bond_value = float(bond_price); // Calculate the bond price
	return bond_value;
}

/**
* Function to calculate the fair price of a bond to the user (ZCB or coupon-paying)
*/
void Bond::price() 
{

	if (!zero_coupon && interest_rates.size() != coupons.size())
	{
		throw 3; // Check each coupon has a corresponding rate.
	}

	double value{ 0 };
	if (zero_coupon)
	{
		double t = maturity/double(365);
		value = principal * exp(-1* interest_rates.at(interest_rates.size() - 1)*t); //ZCB just priced as principal, time discounted.
	}
	else {

		for (unsigned int i = 0; i < coupons.size(); i++)
		{
			double t = coupon_dates.at(i) / double(365);
			value += coupons.at(i)*exp(-1 * interest_rates.at(i)*t); // discount all coupons (principal is last coupon)
		}
	}
	
	bond_price = value;

	return;
}

/**
* Function to compute the yield to maturity for a bond with a given price
* using a newton-rapshon solver (see problem sheet 2 pdf for more details.
*/
float Bond::ytm()
{

if (bond_price < 0) // Prevent erroneous bond_prices 
{
	throw 2;
}

double true_price = bond_price; // The evaluation function f(x) = true_price - numerically computed price


//  --------------------------------------------------------------------------------------------
//(THE QUALITY OF SOLUTION MAY BE SENSITIVE TO INITIAL GUESS)
// Here we make an apriori guess that the yield is the arithmetic mean 
// of the interest rates (this is a heuristic that works very well)
double ytm{ 0 };

for (unsigned int i = 0; i < interest_rates.size(); i++)
{
	ytm += interest_rates.at(i);
}

ytm /= interest_rates.size();


// Start Numerical Iterations
double error = 1; // Fractional error between numerical approximation to price and analytic result for price.
double tolerance = 0.000000001; // Breakout tolerance, controls the number of iterations used
double price_at_ytm{ 0 };

std::cout << "Starting Numerical Solver for Bond Yield Approximation" << std::endl;
while (abs(error) > tolerance) // make sure the yield produces an arbitrarily accurate value of the bond price
{
	ytm = update_yield(ytm); // generate a new proposed yield value
	price_at_ytm = get_price_at_yield(ytm);
	error = ((price_at_ytm - true_price) / true_price);
	std::cout << "True Price: " << true_price << " Current Price: " << price_at_ytm << " Yield Value: " << ytm << std::endl;
}

return float(ytm);
}



/**
* Function to calculate the price of a bond for a given yield.
* @param y, const double reference, denotes the estimated yield of the bond
*/
double Bond::get_price_at_yield(const double& y)
{
	double price_at_y{ 0 };
	for (unsigned int i = 0; i < coupon_dates.size(); i++)
	{
		price_at_y += coupons.at(i) * exp((-1 * y*coupon_dates.at(i)) / 365); //see above for more detail
	}
	return price_at_y;
}


/**
* Function to generate a new proposed value of the yield based on the newton-raphson method
* where the evaluation criteria is for the yield to recover the analytical value of the bond price.
* @param y, const double reference, denotes the current yield of the bond
*/
double Bond::update_yield(const double& y)
{
	double current_price{ 0 };
	current_price = get_price_at_yield(y);

	double dp_dy{ 0 }; //Store the derivative of the bond price w.r.t yield

	double t_i{ 0 };
	for (unsigned int i = 0; i < coupon_dates.size(); i++)
	{
		t_i = coupon_dates.at(i) / 365;
		dp_dy += coupons.at(i) * t_i * exp(-1 * y*t_i); //see https://en.wikipedia.org/wiki/Newton%27s_method
	}

	double correction{ 0 };
	correction = (bond_price - current_price) / dp_dy; // take a step towards root (size of step is proportional to first order taylor expansion error)

	double y_new{ 0 };
	y_new = y - correction;

	return y_new;
}

/**
* Function to return the yield to maturity for a bond to the user.
*/
float Bond::get_ytm()
{
	double yield{ 0 };

	if (!zero_coupon)
	{
		yield = ytm(); 
	}
	else {
		yield = interest_rates.at(interest_rates.size() - 1); 
		// If the bond is a ZCB, Newton-Raphson is overkill- just return the 0 rate for when the principal is paid
	}

	return float(yield);
}
