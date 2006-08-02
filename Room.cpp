/*
 *  Room.cpp
 *  openc2e
 *
 *  Created by Alyssa Milburn on Tue May 25 2004.
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

#include "Room.h"
#include "World.h"

Room::Room() {
	for (unsigned int i = 0; i < CA_COUNT; i++)
		ca[i] = catemp[i] = 0.0f;
}

Room::Room(unsigned int x_l, unsigned int x_r, unsigned int y_l_t, unsigned int y_r_t,
		unsigned int y_l_b, unsigned int y_r_b) {
	if (x_l > x_r) std::swap(x_l, x_r);
	x_left = x_l;
	x_right = x_r;
	if (y_l_b < y_l_t) std::swap(y_l_b, y_l_t);
	if (y_r_b < y_r_t) std::swap(y_r_b, y_r_t);
	y_left_ceiling = y_l_t;
	y_right_ceiling = y_r_t;
	y_left_floor = y_l_b;
	y_right_floor = y_r_b;

	Point ul(x_l, y_l_t);
	Point ur(x_r, y_r_t);
	Point bl(x_l, y_l_b);
	Point br(x_r, y_r_b);

	left = Line(ul, bl);
	right = Line(ur, br);
	top = Line(ul, ur);
	bot = Line(bl, br);

	for (unsigned int i = 0; i < CA_COUNT; i++)
		ca[i] = catemp[i] = 0.0f;
}

void Room::tick() {
	if (world.carates.find(type) == world.carates.end()) return;
	std::map<unsigned int, cainfo> &rates = world.carates[type];

	for (unsigned int i = 0; i < CA_COUNT; i++) {
		if (rates.find(i) == rates.end()) continue;
		cainfo &info = rates[i];

		// adjust for loss
		ca[i] -= (ca[i] * info.loss);

		if (catemp[i] > 1.0f) catemp[i] = 1.0f;
		else if (catemp[i] < 0.0f) catemp[i] = 0.0f;

		// adjust for gain from agents
		if (catemp[i] > ca[i]) {
			float diff = catemp[i] - ca[i];
			catemp[i] = ca[i] + (diff * info.gain);
			if (catemp[i] > 1.0f)
				catemp[i] = 1.0f;
		} else {
			catemp[i] = ca[i];
		}
	}
}

void Room::postTick() {
	if (world.carates.find(type) == world.carates.end()) return;
	std::map<unsigned int, cainfo> &rates = world.carates[type];

	for (unsigned int i = 0; i < CA_COUNT; i++) {
		if (rates.find(i) == rates.end()) continue;
		cainfo &info = rates[i];

		ca[i] = catemp[i];

		// adjust for diffusion to/from surrounding rooms
		// TODO: absolutely no clue if this is correct
		for (std::map<Room *,RoomDoor *>::iterator d = doors.begin(); d != doors.end(); d++) {
			Room *dest = (d->second->first == this) ? d->second->second : d->second->first;
			assert(dest);
			float possiblediffusion = (dest->catemp[i] * info.diffusion * (d->second->perm / 100.0f));
			if (possiblediffusion > 1.0f) possiblediffusion = 1.0f;
			if (possiblediffusion > ca[i])
				ca[i] = possiblediffusion;
		}
	}
}

void Room::resetTick() {
	for (unsigned int i = 0; i < CA_COUNT; i++)
		catemp[i] = 0.0f;
}

/* vim: set noet: */
