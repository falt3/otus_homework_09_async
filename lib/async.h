#pragma once

#include <cstddef>

int connect(int size);
void receive(char *p, std::size_t len, int id);
void disconnect(int id);