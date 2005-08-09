/*
 *  c16Image.h
 *  openc2e
 *
 *  Created by Alyssa Milburn on Sat May 22 2004.
 *  Copyright (c) 2004 Alyssa Milburn. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 */

#include "creaturesImage.h"
#include <istream>
#include "mmapifstream.h"

class c16Image : public creaturesImage {
private:
	unsigned int **lineoffsets;

public:
	c16Image() { }
  c16Image(mmapifstream *);
	void readHeader(std::istream &in);
	void writeHeader(std::ostream &out);
};

class s16Image : public creaturesImage {
public:
	s16Image() { }
  s16Image(mmapifstream *);
	void readHeader(std::istream &in);
	void writeHeader(std::ostream &out);
};
/* vim: noet : */
