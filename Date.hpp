#ifndef DateProcessor
#define DateProcessor
#include <cstdio>
#include <iostream>
#include <cstring>
#include <algorithm>
using namespace std;

const int MonthTable[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31, 0, 0, 0};
const int PrefixMonthTable[] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365, 365, 365, 365};

struct TimePoint {
    int Month, Date, Hour, Minute;
	
	TimePoint() {
		Month = Date = Hour = Minute = 0;
	}

    int calc() const {
        return (PrefixMonthTable[Month - 1] + Date - 1) * 1440 + Hour * 60 + Minute;
    }
    
    friend bool operator < (TimePoint a, TimePoint b) {
        return a.calc() < b.calc();
    }
    
    friend bool operator != (TimePoint a, TimePoint b) {
        return a.calc() != b.calc();
    }

    bool valid() {
        return (Month <= 12) 
		&& (Month >= 1) 
		&& (Date >= 1) 
		&& (Date <= MonthTable[Month])
		&& (Hour >= 0) 
		&& (Hour < 24) 
		&& (Minute >= 0) 
		&& (Minute < 60);
    }

    friend TimePoint operator + (TimePoint a, int b) {
        a.Minute += b;
        if(a.Minute >= 60) {
            a.Hour += a.Minute / 60;
            a.Minute %= 60;
        }
        
        if(a.Hour >= 24) {
            a.Date += a.Hour / 24;
            a.Hour %= 24;
        }
        
        while(a.Date > MonthTable[a.Month]) {
            a.Date -= MonthTable[a.Month];
            ++ a.Month;
        }
        return a;
    }

    friend int operator - (TimePoint a, TimePoint b) {
        return (a.calc() - b.calc()) / 1440;
    }
    
   	void ZeroPrint(int x) {
    	if(x < 10) printf("0%d", x);
    	else printf("%d", x);
	}

    void print() {
    	ZeroPrint(Month);
    	putchar('-');
    	ZeroPrint(Date);
    	putchar(' ');
    	ZeroPrint(Hour);
    	putchar(':');
    	ZeroPrint(Minute);
    }
};
#endif
