
class bond {
public:
	bond() {}
	//current time t0, years to maturity T0, coupon payment C0, principal P0
	bond(double t0, double T0, double C0, double P0);
	void setPrice(double price0);
	double getPrice();
	void setYtm(double ytm0);
	double getYtm();
	~bond() {}

	double getTimeToMaturity() { return T; }
	double getTimeNow() { return t; }
	double getPrincipal() { return P; }
	double getCoupon() { return C; }

private:
	double getContinuousPrice(double r);

	double price; //bond price
	double ytm; // year to maturity

	double T; // Time to Maturity
	double t; // Time Now
	double P; // Principal amout
	double C; // Coupon payment
};