#pragma once
#include <vector>


/**
* Project:    Project 1
* Filename:   Bond.h
* Version:    v1 (9 March 2020)
* Author:     Ryan Sephton
* Summary:    Class to implement bond objects.
*/

class Bond {
private:
	//Bond Attributes
	unsigned int maturity;
	float principal;
	bool zero_coupon;
	double bond_price{ -1 }; // Negative initialisation used to perform internal check that price call is made before yield call
	std::vector<float> coupons{ 0 }; 
	std::vector<unsigned int> coupon_dates{ 0 }; 
	std::vector<float> interest_rates{ 0 };
	 
	// Bond Methods
	float ytm();  // computes yield to maturity for the bond
	void price(); // computes the intrinsic value for the bond
	double update_yield(const double& y); 
	double get_price_at_yield(const double& y); 
public:
	// Constructors
	Bond(std::vector<float>& coupon_payments, const std::vector <unsigned int>& payment_dates, const float& princpal,const unsigned int& expiry);
	Bond(const float& princpal, const float& interest_rate, const unsigned int& expiry);

	//Destructor
	~Bond();

	//Bond Methods
	float get_price(const std::vector<float>& interests, const unsigned int& expiry); // coupon paying bond pricing
	float get_price(); // ZCB pricing
	float get_principal() { return principal; };
	float get_ytm();
};