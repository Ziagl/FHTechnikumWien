#pragma once

#include "precomp.h"

class MyTimer {
private:
	clock_t m_start;
	float* m_pSeconds;
public:
	MyTimer(float* in_pSeconds) : m_pSeconds(in_pSeconds) {
		m_start = clock();
	}
	~MyTimer(void) {
		clock_t time_diff = clock() - m_start;
		*m_pSeconds = (float)time_diff / CLOCKS_PER_SEC;
	}
};