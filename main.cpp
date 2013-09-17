/* 
 * File:   main.cpp
 * Author: mazabinski
 *
 * Created on 17 wrzesień 2013, 22:23
 */
/*
					+IMMMM .~MMZ.
				 .MM NMMMMM  .MMMM
				MMM. MMMMMMZ   MMMM.
			  .MMM, .MMMMMMM  ..MMMM
			  .MMM. ZMMMMMMM.   MMMM.
			  .MMM  =MMMMMMM.   MMMM.
			  .MMM . MMMMMMM.  MMMM
				MMM: MMMMMMM .ZMMM
				  MMM MMMMMM.~MO
					  ~MMMN..   ...M.
						.?M8 .. +.NI
					   . .....  MNM D
						: D..Z...MO.?.
						  NM . M..  .~
						 .~I...     .,
						  .M.       M.
						 .M.        :
						.M        .MM
					   .7           M.
					   M            MO
					  M.            .8       .=MMMMMMM .
					 M.             .I    MM$          ,M
					.                M MM .             .M
					M                 N              .   M
				   .:                                M   ,.
													 +   .
					,                                Z  .M
				   .M                               ..  ,
					M                  .            M.  M..  =+, .
					 M        ?        D           :+   7  ..M$ ..
					 .Z        M.       ,         DMM.     .. =M.
					  ,M      .8,       M        M.. .MMM,...
			   .. N  M$ MI.    MM.      :.   ..M$
			 .$...  =  MM . D,,MM7       MMMM,
				   Z...    MN   MM.      MMOMMM7..
					  .D.,8      M.      :..:NM$
								 MM.      .MM~.,
								  MM.  ~=7DMMM$.

		  S   E   A    L       O   F      T   H   E       D   A   Y
 */


#include <cstdlib>
#include <iostream>
#include <thread>
#include <sys/time.h>
#include <string>
#include <iomanip>
#include <sstream>

#include <openssl/md5.h>

using namespace std;

/**
 * @return unsigned int value with thread count (read from stdin)
 * return 1 if 0 has been set, value if proper value has been set
 * or 64 if value more than 64 has been set
 */
unsigned int set_thread_num()
{
	unsigned int thread_num;

	try {
		cout << "Enter thread num (1-64): ";
		cin >> thread_num;

		if (thread_num == 0) {
			thread_num = 1;
			cout << "Setting thread num to 1" << endl;
		} else if (thread_num <= 64) {
			cout << "Setting thread num to " << thread_num << endl;
		} else {
			thread_num = 64;
			cout << "Setting thread num to 64" << endl;
		}
	} catch (...) {
		thread_num = 1;
		cout << "Exception, setting thread num to 1" << endl;
	}

	return thread_num;
}

/**
 * @return long double value with range_start value (read from stdin)
 * if any exception catched, (long double) time(NULL) is returned  
 */
long double set_range_start()
{
	long double range_start;
	try {
		cout << "Enter range start (long double): ";
		cin >> range_start;

		if (range_start < 0) {
			range_start = (long double) time(NULL);
		}
	} catch (...) {
		// set range start to time(NOW)
		range_start = (long double) time(NULL);
	}
	return range_start;
}

/**
 * @return long double value with range_stop value (read from stdin)
 * if any exception catched 0 is returned 
 */
long double set_range_stop()
{
	long double range_stop;
	try {
		cout << "Enter range stop (long double): ";
		cin >> range_stop;


		if (range_stop < 0) {
			range_stop = 0;
		}
	} catch (...) {
		// set range stop to 0
		range_stop = (long double) 0;
	}
	return range_stop;
}

/**
 * @return long double value with resolution value (read from stdin)
 * if any exception catched 1.0 is returned 
 */
long double set_resolution()
{
	long double resolution;
	try {
		cout << "Enter resolution: ";
		cin >> resolution;

		if (resolution <= 0) {
			resolution = 1.0;
		}
	} catch (...) {
		// set range stop to 1.0
		resolution = 1.0;
	}
	return resolution;
}

/**
 * 
 * @param start - value where should start
 * @param stop - value where should stop
 * @param resolution - resolution (simple iteration)
 * @param md5 - md5 as hashed string to find
 * @return long double value > 0 if found
 * or long double -1.0 value if not found
 */
long double find_md5_range(long double start, long double stop,
  long double resolution, string& md5)
{
	unsigned char digest[MD5_DIGEST_LENGTH];
	char md5_result[33];
	string str;

	for (start; start <= stop; start += resolution) {

		// convert to str
		stringstream sstr(stringstream::in | stringstream::out);
		sstr << setprecision(10) << start << endl;
		str = sstr.str();

		// generate MD5 hash
		MD5((const unsigned char*) str.c_str(), str.size(), (unsigned char*) &digest);

		// convert result to hex-str
		for (int i = 0; i < 16; i++) {
			sprintf(&md5_result[i * 2], "%02x", (unsigned int) digest[i]);
		}

		// if match, return value
		if (md5.compare(md5_result) == 0) {
			return start;
		}
	}
	return -1.0;
}

/**
 * Quick test
 * @return - nothing
 */
void test()
{
	string looking_for = "8f857f8cbb8bbd725823604e32069be2"; // value of (md5("22.7803"))
	long double start = 0.0;
	long double stop = 100.0;
	long double resolution = 0.0001;

	long double result = find_md5_range(start, stop, resolution, looking_for);
	if (result > 22.7802 && result < 22.7804) {
		cout << "TEST RESULT OK" << endl;
	} else {
		cout << "TEST RESULT FAIL" << endl;
	}
}

int main(int argc, char** argv)
{
	// turn off sync with stdio, it is not needed and can improve speed of streams
	//ios_base::sync_with_stdio(false);

	// run quick test
	test();
	unsigned int th_num;
	long double range_start, range_stop, resolution;

	// read basic config from stdin
	th_num = 1; // set_thread_num();
	range_start = set_range_start();
	range_stop = set_range_stop();
	resolution = set_resolution();

	if (range_start >= range_stop) {
		// switch
		long double tmp = range_stop;
		range_stop = range_start;
		range_start = tmp;
	}

	// quickly, without validation
	string md5;
	cout << "Enter hash to compare: ";
	cin >> md5;

	// one threaded prototype run:
	cout << "Result is: " <<
	  find_md5_range(range_start, range_stop, resolution, md5) << endl;

	return EXIT_SUCCESS;
}

