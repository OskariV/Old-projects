/*
 * ITMwriter.h
 *
 *  Created on: 25 Jan 2019
 *      Author: omvir
 */

#ifndef ITMWRITER2_H_
#define ITMWRITER2_H_

#include "ITM_write.h"
#include <string>

class ITMwriter {
public:
	ITMwriter();
	void write(std::string input);
	void write(const char *in);
	void write(int input);
	virtual ~ITMwriter();
private:
	std::string printing;
};

#endif /* ITMWRITER2_H_ */
