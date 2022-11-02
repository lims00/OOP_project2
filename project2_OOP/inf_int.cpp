#define _CRT_SECURE_NO_WARNINGS
#include "inf_int.h"
#include <iostream>
#include <string.h>
#include <stdlib.h>
/*
Originally written by
컴퓨터공학부
정진경
*/

inf_int::inf_int()
{
	this->digits = new char[2];	// 동적할당

	this->digits[0] = '0';		// default 값 0 설정
	this->digits[1] = '\0';
	this->length = 1;
	this->thesign = true;
}

inf_int::inf_int(int n) {
	char buf[100];

	if (n < 0) {		// 음수 처리
		this->thesign = false;
		n = -n;
	}
	else {
		this->thesign = true;
	}

	int i = 0;
	while (n > 0) {			// 숫자를 문자열로 변환하는 과정
		buf[i] = n % 10 + '0';

		n /= 10;
		i++;
	}

	if (i == 0) {	// 숫자의 절댓값이 0일 경우
		new (this) inf_int();	// 생성자 재호출...gcc에서 컴파일에러가 있다고 함. inf_int()의 경우 별개의 인스턴스가 생성됨. 
	}
	else {
		buf[i] = '\0';

		this->digits = new char[i + 1];
		this->length = i;
		strcpy(this->digits, buf);
	}
}

inf_int::inf_int(const char* str)
{
	// to be filled 
	// 부호 처리 
	// "100"이 들어왔다면 내부 표현에 맞게 "001"로 변환
	// ex) "-1053" -> thesign=false, digits="3501", len=4
	unsigned int i;
	if ((int)strlen(str) == 1 && str[0] == '0') {
		this->digits = new char[2];
		this->digits[0] = '0';
		this->digits[1] = '\0';
		this->length = 1;
		this->thesign = true;
	}
	else if (str[0] == '-') {
		this->thesign = false;
		this->length = (unsigned int)strlen(str) - 1;
		this->digits = new char[this->length + 1];
		for ( i = 0; i < this->length; i++) {
			this->digits[i] = str[this->length - i];
		}	
		this->digits[this->length] = '\0';
	}
	else {
		this->thesign =true;
		this->length = (unsigned int)strlen(str);
		this->digits = new char[this->length + 1];
		for ( i = 0; i < this->length; i++) {
			this->digits[i] = str[this->length - i-1];
		}
		this->digits[this->length] = '\0';
	}
}

inf_int::inf_int(const inf_int& a) {
	this->digits = new char[a.length + 1];

	strcpy(this->digits, a.digits);
	this->length = a.length;
	this->thesign = a.thesign;
}

inf_int::~inf_int() {
	delete digits;		// 메모리 할당 해제
}

inf_int& inf_int::operator=(const inf_int& a)
{
	if (this->digits) {
		delete this->digits;		// 이미 문자열이 있을 경우 제거.
	}
	this->digits = new char[a.length + 1];

	strcpy(this->digits, a.digits);
	this->length = a.length;
	this->thesign = a.thesign;

	return *this;
}

bool operator==(const inf_int& a, const inf_int& b)
{
	// we assume 0 is always positive.
	if ((strcmp(a.digits, b.digits) == 0) && a.thesign == b.thesign)	// 부호가 같고, 절댓값이 일치해야함.
		return true;
	return false;
}

bool operator!=(const inf_int& a, const inf_int& b)
{
	return !operator==(a, b);
}

bool operator>(const inf_int& a, const inf_int& b){
	// to be filled
	// 절대값 비교
	// 둘 다 양수일 경우 절댓값 비교한 것을 그대로 return
	// 둘 다 음수일 경우 절댓값 비교의 것을 반전하여 return
	// 부호가 다를 경우, a가 양수일 경우 b는 음수, a가 음수일 경우 b는 양수이기에 a의 부호진리값을 반환하면 됨
	unsigned int i;
	if (a.length > b.length) {
		if (a.thesign == true)  return true; 
		else return false;
	}
	else if (a.length == b.length) {
		if (a.thesign == b.thesign) {
			for ( i = 1; i < a.length; i++) {
				if (a.digits[a.length - i]> b.digits[b.length - i]) {
					if (a.thesign == true)return true;
					else return false;
				}
				else if (a.digits[a.length - i]<b.digits[b.length - i]) {
					if (a.thesign == true) return false;
					else return true;
				}
			}
		}
		else if (a.thesign == true && b.length == false) {
			return true;
		}
		else return false;
	}
	else {
		if (a.thesign == false)  return true;
		else return false;
	}
}

bool operator<(const inf_int& a, const inf_int& b){
	if (operator>(a, b) || operator==(a, b)) {
		return false;
	}
	else {
		return true;
	}
}

inf_int operator+(const inf_int& a, const inf_int& b)
{
	inf_int c;
	unsigned int i;

	if (a.thesign == b.thesign) {	// 이항의 부호가 같을 경우 + 연산자로 연산
		for (i = 0; i < a.length; i++) {
			c.Add(a.digits[i], i + 1);
		}
		for (i = 0; i < b.length; i++) {
			c.Add(b.digits[i], i + 1);
		}

		c.thesign = a.thesign;

		return c;
	}
	else {	// 이항의 부호가 다를 경우 - 연산자로 연산
		c = b;
		c.thesign = a.thesign;

		return a - c;
	}
}

inf_int operator-(const inf_int& a, const inf_int& b){//to be filled
	inf_int c;
	unsigned int i;

	if (a.thesign == b.thesign) {
		if (b.length > a.length) {//b-a
			for (i = 0; i < b.length; i++) {
				c.Add(b.digits[i], i + 1);
			}
			for (i = 0; i < a.length; i++) {
				if (c.digits[i] < a.digits[i]) {
					c.digits[i] = c.digits[i] + 10 - a.digits[i] + '0';
					if (c.digits[i + 1] != '0') {
						c.digits[i + 1] = c.digits[i + 1]-1;
					}
					else {
						unsigned int j = i+1;
						while (c.digits[j] == '0') {
							c.digits[j] = '9';
							j++;
						}
						c.digits[j] = c.digits[j] -1;
					}
				}
				else if (a.digits[i] < b.digits[i]) {
					c.digits[i] = c.digits[i] - a.digits[i] + '0';
				}
				else {
					c.digits[i] = '0';
				}
			}
			if ((c.digits[i] == '0')&&(c.length==i))c.digits[i] = '\0';
			if (a.thesign) {
				c.thesign = false;
			}
			else {
				c.thesign = true;
			}
			return c;
		}
		else if (b.length < a.length) {
			for (i = 0; i < a.length; i++) {
				c.Add(a.digits[i], i + 1);
			}
			for (i = 0; i < b.length; i++) {//a-b 즉 c-b진행
				if (b.digits[i] > c.digits[i]) {
					c.digits[i] = c.digits[i] + 10 - b.digits[i] + '0';
					if (c.digits[i + 1] != '0') {
						c.digits[i + 1] = c.digits[i + 1]-1;
					}
					else {
						unsigned int j = i+1;
						while (c.digits[j] == '0') {
							c.digits[j] = '9';
							j++;
						}
						c.digits[j] = c.digits[j]-1;
					}
				}
				else if (c.digits[i] > b.digits[i]) {
					c.digits[i] = c.digits[i] - b.digits[i] + '0';
				}
				else {
					c.digits[i] = '0';
				}
			}
			if ((c.digits[i] == '0')&&(c.length==i))c.digits[i] = '\0';
			
			if (a.thesign) {
				c.thesign = true;
			}
			else {
				c.thesign = false;
			}
			return c;
		}
		else {
			if (a > b) {
				for (i = 0; i < a.length; i++) {
					c.Add(a.digits[i], i + 1);
				}
				for (i = 0; i < b.length; i++) {//a-b
					if (b.digits[i] > c.digits[i]) {
						c.digits[i] = c.digits[i] + 10 - b.digits[i]+'0';
						if (c.digits[i + 1] != '0') {
							c.digits[i + 1] = c.digits[i + 1]-1;
						}
						else {
							unsigned int j = i+1;
							while (c.digits[j] == '0') {
								c.digits[j] = '9';
								j++;
							}
							c.digits[j] = c.digits[j]-1;
						}
					}
					else if (c.digits[i] > b.digits[i]) {
						c.digits[i] = c.digits[i] - b.digits[i] + '0';
					}
					else {
						c.digits[i] = '0';
					}
				}
				if ((c.digits[i] == '0') && (c.length == i))c.digits[i] = '\0';
				if (a.thesign) {
					c.thesign = true;
				}
				else {
					c.thesign = false;
				}
				return c;
			}else {
				for (i = 0; i < b.length; i++) {
					c.Add(a.digits[i], i + 1);
				}
				for (i = 0; i < a.length; i++) {
					if (c.digits[i] < a.digits[i]) {
						c.digits[i] = c.digits[i] + 10 - a.digits[i] + '0';
						if (c.digits[i + 1] != '0') {
							c.digits[i + 1] -=1;
						}
						else {
							unsigned int j = i+1;
							while (c.digits[j] == '0') {
								c.digits[j] = '9';
								j++;
							}
							c.digits[j] = c.digits[j] - 1;
						}
					}
					else if (a.digits[i] < c.digits[i]) {
						c.digits[i] = c.digits[i] - a.digits[i] + '0';
					}
					else {
						c.digits[i] = '0';
					}
				}

				if ((c.digits[i] == '0') && (c.length == i))c.digits[i] = '\0';
				if (a.thesign) {
					c.thesign = false;
				}
				else {
					c.thesign = true;
				}
				return c;
			}
		}
	}
	else {
		if (a.thesign == false && b.thesign == true) {
			c.thesign = false;
			return a + b;
		}
		else {
			c.thesign = true;
			return a + b;
		}
	}

}

inf_int operator*(const inf_int& a, const inf_int& b){
	inf_int c;
	inf_int result;
	int k;
	unsigned i=1;
	unsigned j = 0;
	unsigned longlength = 0;
	unsigned int m = 0;
	int multi = 0; //큰수
	int num = 0; // 하나씩 곱해주는 작은 수
	int intnum = 0;
	if (a.length > b.length) {
		longlength = a.length;
		for (i = 0; i <b.length; i++) {
			
			result.length = longlength+i;
			result.digits = (char*)realloc(result.digits, result.length+1);
			for ( k = 0; k < result.length; k++) {
				result.digits[k] = '0';
			}
			result.digits[k] = '\0';
			num = (unsigned int)(b.digits[i]-'0'); // result에 b[i]자리수를 곱해준다.
			for ( m=0; m < longlength; m ++ ){
				multi = (unsigned int)( a.digits[m]-'0');
				intnum = num * multi;
				result.digits[m+i] += (intnum % 10);

				if (result.digits[m + i] > '9') {
					result.digits[m + i] -= 10;
					result.digits[m + i + 1] += 1;
				}

				result.digits[m+i+1] += (unsigned int)(intnum / 10);

				if (result.digits[m + 1 + i] > '9') {
					result.digits[m + i + 1] -= 10;
					result.digits[m + i + 2] += 1;
				}
			}
			for (unsigned int n = 0; n<result.length; n++) {
				c.Add(result.digits[n], n + 1);
			}
			
		}

		if (a.thesign != b.thesign) {
			c.thesign = false;
		}
		else c.thesign = true;

	}
	else {//b의 길이가 더 길거나 같을 때 
		longlength =b.length;
		
		for (i = 0; i < a.length; i++) {

			result.length = longlength + i;
			result.digits = (char*)realloc(result.digits, result.length + 1);
			for (k = 0; k < result.length; k++) {
				result.digits[k] = '0';
			}
			result.digits[k] = '\0';
			num = (unsigned int)(a.digits[i] - '0'); // result에 b[i]자리수를 곱해준다.
			for (m = 0; m < longlength; m++) {
				multi = (unsigned int)(b.digits[m] - '0');
				intnum = num * multi;
				result.digits[m + i] += (intnum % 10);

				if (result.digits[m + i] > '9') {
					result.digits[m + i] -= 10;
					result.digits[m + i + 1] += 1;
				}

				result.digits[m + i + 1] += (unsigned int)(intnum / 10);

				if (result.digits[m + 1 + i] > '9') {
					result.digits[m + i + 1] -= 10;
					result.digits[m + i + 2] += 1;
				}
			}
			for (unsigned int n = 0; n < result.length; n++) {
				c.Add(result.digits[n], n + 1);
			}

		}

		if (a.thesign != b.thesign) {
			c.thesign = false;
		}
		else c.thesign = true;


	}
	




	return c;
}


ostream& operator<<(ostream& out, const inf_int& a)
{
	int i;

	if (a.thesign == false) {
		out << '-';
	}
	for (i = a.length - 1; i >= 0; i--) {
		out << a.digits[i];
	}
	return out;
}



void inf_int::Add(const char num, const unsigned int index)	// a의 index 자리수에 n을 더한다. 0<=n<=9, ex) a가 391일때, Add(a, 2, 2)의 결과는 411
{
	if (this->length < index) {
		this->digits = (char*)realloc(this->digits, index + 1);

		if (this->digits == NULL) {		// 할당 실패 예외처리
			std::cout<< "Memory reallocation failed, the program will terminate." << std::endl;

			exit(0);
		}

		this->length = index;					// 길이 지정
		this->digits[this->length] = '\0';	// 널문자 삽입
	}

	if (this->digits[index - 1] < '0') {	// 연산 전에 '0'보다 작은 아스키값인 경우 0으로 채움. 쓰여지지 않았던 새로운 자리수일 경우 발생
		this->digits[index - 1] = '0';
	}

	this->digits[index - 1] += num - '0';	// 값 연산


	if (this->digits[index - 1] > '9') {	// 자리올림이 발생할 경우
		this->digits[index - 1] -= 10;	// 현재 자릿수에서 (아스키값) 10을 빼고
		Add('1', index + 1);			// 윗자리에 1을 더한다
	}
}
