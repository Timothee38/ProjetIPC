/*
 * Header: semaphore.h
 *
 * Provides wrapper functions around POSIX system calls semget,
 * semctl and semop allowing the creation, initialization, opening
 * and removal of a single semaphore at a time and implementing
 * operations down (also known as P or wait) and up (also known as V
 * or signal) on it.
 *
 *
 * Copyright 2014 Razmig Kéchichian, INSA de Lyon, TC department
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see http://www.gnu.org/licenses/.
 *
 */

#ifndef __SEMAPHORE_H_SEMAPHORE__
#define __SEMAPHORE_H_SEMAPHORE__

int create_semaphore(int key);
int open_semaphore(int key);
int remove_semaphore(int id);
int init_semaphore(int id, int val);
int up(int id);
int down(int id);

#endif
