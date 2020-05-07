#pragma once
#include "Bond.h"

/**
* Project:    Project 1
* Filename:   Term_Structure.h
* Version:    v1 (9 March 2020)
* Author:     Ryan Sephton
* Summary:    Class to implement interest rate term structures.
*/

class Term_Structure {
protected:

	//Attributes
	int compounding_frequency;
	double price_1;
	double price_2;
	unsigned int t_1;
	unsigned int t_2;
	double continuous_forward_rate; //Continuous Compounding
	double discrete_forward_rate; // Nominal Rate Compounding

	//Methods
	void calculate_rates();


public:

	//Constructors & Desructor
	Term_Structure(Bond& zcb_1, const unsigned int& expiry_1, Bond& zcb_2, const unsigned int& expiry_2, const int& freq = 4);
	Term_Structure(const double& r_1, const unsigned int& time_of_rate_1, const double& r_2, const unsigned int& time_of_rate_2, const  int& freq = 4);
	Term_Structure();
	~Term_Structure() {};
	
	//Methods
	double get_continuous_fwd_rate();
	double get_discrete_fwd_rate();
	double get_price_2() { return price_2; };
	double get_price_1() { return price_1; };
	unsigned int get_t1() { return t_1; };
	unsigned int get_t2() { return t_2; };
};