#pragma once
#include <iostream>
#include <time.h>		
#include <stdio.h>			
#include <stdlib.h>
#include <conio.h>
#include <string.h>
//#include <fstream>		//c++ 파일 입출력

class SmapleStu
{
public:
	SmapleStu();
	SmapleStu(int a);
	~SmapleStu();

public:
	void begin(int a);		//초기값 생성

public:
	int num;
	char name[5];
	int kuk;
	int eng;
	int su;
	int sum;
	double avg;
};