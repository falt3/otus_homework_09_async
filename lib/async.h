/**
 * @file async.h
 * @author Липаткин Дмитрий
 * @brief 
 * @version 0.1
 * @date 2023-09-25
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#pragma once

#include <cstddef>

int connect(int size);
void receive(const char *p, std::size_t len, int id);
void disconnect(int id);