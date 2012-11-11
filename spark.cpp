/*
*Author :Tharindra Galahena
*Project:SPARK - Crawling Robot A.I.
*Date   :11/11/2012
*License:
* 
*     Copyright 2012 Tharindra Galahena
*
* This program is free software: you can redistribute it and/or modifood_y it under the 
* terms of the GNU General Public License as published by the Free Software Foundation, 
* either version 3 of the License, or (at your option) any later version. This program is 
* distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even 
* the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
* General Public License for more details.
*
* You should have received a copy of the GNU General Public License along with This program. 
* If not, see http://www.gnu.org/licenses/.
*
*/

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <cmath>

using namespace std;

typedef struct point_i{
	int x;
	int y;
}point_i;

typedef struct point_d{
	double x;
	double y;
}point_d;

class spark{
	private:
		point_i  foot;
		point_d joint;
		point_i  body;
		int l1;
		int l2;
		int max_x;
		int max_y;
		int min_x;
		int min_y;
		double discount;
		double learning_rate;
		int exploration_rate;
		int d1;
		int d2;
		int steps;
		
	public:
		double V[5][4];
		
		spark(int bx, int by, int fx, int fy, double d, double l_rate, int exp);
		point_d get_joint();
		point_i get_foot();
		point_i get_body();
		int reward();
		int fature_reward(int x, int y);
		int apply_action(int x, int y);
		int apply_random_action();
		int apply_estimate_action();
		void try_and_learn();
		
};
spark::spark(int bx, int by, int fx, int fy, double d, double l_rate, int exp){
	d1 = d2 = 0;
	foot.x = fx;
	foot.y = fy;
	body.x = bx;
	body.y = by;
	min_x = 0;
	min_y = 0;
	max_x = 4;
	max_y = 3;
	l1 = 3;
	l2 = 3;
	discount = d;
	learning_rate = l_rate;
	exploration_rate = exp;
	for(int i = 0; i < 5; i++)
		for(int j = 0; j < 4; j++)
			V[i][j] = 0;
}
int spark::apply_action(int x, int y){
	d1 = d2;
	if(foot.y <= 0 && y <= 0){
		if((foot.x > 0 && foot.x <= max_x) || (foot.x <= 0 && x > 0) || (foot.x >= max_x && x < max_x)){
			body.x -= x;
			foot.x += x;
		}
	}else{
		foot.x += x;
		foot.y += y;
	}
	if(foot.x > max_x) foot.x = max_x;
	else if(foot.x < min_x) foot.x = 0;
	if(foot.y > max_y) foot.y = max_y;
	else if(foot.y < min_y) foot.y = 0;
	d2 = body.x;
}
int spark::fature_reward(int x, int y){
	int fx = foot.x;
	int fy = foot.y;
	int bx = body.x;
	int by = body.y;
	int tmp_d1 = d1;
	int tmp_d2 = d2;
	
	tmp_d1 = tmp_d2;
	if(fy <= 0 && y <= 0){
		if((fx > 0 && fx <= max_x) || (fx <= 0 && x > 0) || (fx >= max_x && x < max_x)){
			bx -= x;
			fx += x;
		}
	}else{
		fx += x;
		fy += y;
	}
	if(fx > max_x) fx = max_x;
	else if(fx < min_x) fx = 0;
	if(fy > max_y) fy = max_y;
	else if(fy < min_y) fy = 0;
	tmp_d2 = bx;
	if(tmp_d2 - tmp_d1 != 0){
		return tmp_d2 - tmp_d1 -1;
	}else{
		return -1;
	}
}
int spark::reward(){
	if(d2 - d1 != 0){
		return d2 - d1 -1;
	}else{
		return -1;
	}
}
int spark::apply_random_action(){
	
	double max = -1000;
	int max_s_x;
	int max_s_y;
	max_s_x = rand() % 3 - 1;
	max_s_y = rand() % 3 - 1;
	
	int oldfx = foot.x;
	int oldfy = foot.y;
	
	apply_action(max_s_x, max_s_y);
	
	cout << "random action   : " << max_s_x << " "  <<  max_s_y << "\t";
	cout << "distence : " << body.x << "\t";
	cout << "steps : " << steps << endl;
	
	int tmp = reward() + discount * V[foot.x][foot.y];
	V[oldfx][oldfy] += learning_rate * (tmp - V[oldfx][oldfy]);
}
int spark::apply_estimate_action(){
	double max = -100000;
	int max_s_x;
	int max_s_y;
	
	for(int i = -1; i < 2; i++){
		for(int j = -1; j < 2; j++){
			if(i == 0 && j == 0) continue;
			if(foot.x + i >= max_x || foot.x + i < 0) continue;
			if(foot.y + j >= max_y || foot.y + j < 0) continue;
			int Q = fature_reward(i, j) + 0.9 * V[foot.x + i][foot.y + j];
			if(Q > max){
				max = Q;
				max_s_x = i;
				max_s_y = j;
			} 
		}
	}
	int oldfx = foot.x;
	int oldfy = foot.y;
	
	apply_action(max_s_x, max_s_y);
	
	cout << "estimate action : " << max_s_x << ","  <<  max_s_y << "\t";
	cout << "distence : " << body.x << "\t";
	cout << "steps : " << steps << endl;
	
	double tmp = (double)reward() + discount * V[foot.x][foot.y];
	V[oldfx][oldfy] += learning_rate * (tmp - V[oldfx][oldfy]);
}
point_i spark::get_body(){
	return body;
}
point_i spark::get_foot(){
	return foot;
}

point_d spark::get_joint(){
	double x1 = (double)foot.x;
	double y1 = (double)foot.y;
	
	if(x1 == 0 && y1 == 0){
		joint.y = y1 + l1 / sqrt(2);
		joint.x = x1 - l1 / sqrt(2);
		return joint;
	}
	if(x1 <= 0 && y1 >= 0){
		joint.y = (double)y1 / 2.0;
		joint.x = -1.0 * sqrt(l1 * l1 - joint.y * joint.y);
		return joint;
	}
	
	if(y1 == 0){
		joint.x = (double)x1 / 2.0;
		joint.y = sqrt(l1 * l1 - joint.x * joint.x);
		return joint;
	}
	if(sqrt(y1 * y1 + x1 * x1) / 2.0 == l1){
		joint.x = (double)x1 / 2.0;
		joint.y = (double)y1 / 2.0;
	} 
	double k = (double)(foot.x * foot.x + foot.y * foot.y) / ((double) foot.y * 2.0);
	double m = (double)foot.x / (double)foot.y;
	
	double A = 1.0 + m * m;
	double B = -2.0 * k * m;
	double C = k * k - l1 * l1;
	
	double X1 = (-B + sqrt(B * B - 4 * A * C)) / (2.0 * A);
	double X2 = (-B - sqrt(B * B - 4 * A * C)) / (2.0 * A);
	double Y1 = (k - m * X1);
	double Y2 = (k - m * X2);
	
	double tmp = ((double)foot.y / foot.x) * X1;
	if(Y1 > tmp){
		joint.x = X1;
		joint.y = Y1;
		return joint;
	}
	joint.x = X2;
	joint.y = Y2;
	return joint;
}
void spark::try_and_learn(){
	steps++;
	int ch = rand() % 100;
	if(ch < exploration_rate)
		apply_random_action();
	else
		apply_estimate_action();
}
