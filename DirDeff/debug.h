/*
 * debug.h
 *
 *  Created on: Feb 3, 2015
 *      Author: kinga
 */

#ifndef DEBUG_H_
#define DEBUG_H_

#include <iostream>

#define DEBUG

#ifdef DEBUG
#define DEBUG_MSG(str) do { std::cout << str << std::endl; } while( false )
#else
#define DEBUG_MSG(str) do { } while ( false )
#endif



#endif /* DEBUG_H_ */
