#pragma once
#include <GL/glut.h>
#include <cstdlib>
#include<math.h>

#define w 60		
#define h 50
#define pn 80
class Zombie {
public:



	Zombie() {
		x = 0;
		y = 0;
		alive = 1;

		zr = 0.4 + (rand() % 100) / 100.0*0.6;
		zg = 0.4 + (rand() % 100) / 100.0*0.6;
		zb = 0.4 + (rand() % 100) / 100.0*0.6;


		for (int i = 0; i < pn; i++)
			particles[i].set(zr, zg, zb);

	}
	Zombie(int a, int b) {
		x = a;
		y = b;
		alive = 1;
		
		zr = 0.4 + (rand() % 100) / 100.0*0.6;
		zg = 0.4 + (rand() % 100) / 100.0*0.6;
		zb = 0.4 + (rand() % 100) / 100.0*0.6;

		for (int i = 0; i < pn; i++)
			particles[i].set(zr, zg, zb);
		
	}
	void init() {
		x = 0;
		y = 0;
		alive = 1;
	}

	int x, y; // 위치 : 좌표
	int dir;  // 방향 12시부터 시계방향으로 0123
	int alive;
	class particle {
	public:
		particle() {
			v = (rand() % 100) / 100.0 * 15 + 0.5;
			theta = (rand() % 100);
			a = (rand() % 100) / 100.0*0.4 + 0.2;
			r = 0.3 + (rand() % 100) / 100.0*0.4;
			g = 0.3 + (rand() % 100) / 100.0*0.4;
			b = 0.3 + (rand() % 100) / 100.0*0.4;
			size = (rand() % 100) / 100.0*0.4 + 0.2;
		}
		void set(float zr,float zg, float zb) {
			r = zr + (rand() % 100) / 100.0*0.4-0.2;
			g = zg + (rand() % 100) / 100.0*0.4 - 0.2;
			b = zb + (rand() % 100) / 100.0*0.4 - 0.2;
		}
		float v;
		float theta;
		float a;
		float size;
		float r, g, b;
	};
	particle particles[pn];
	int particle_time = 0;
	float time=0;
	float ptime = 0;
	float v = 0.7;
	float zr, zg, zb;

	int move(int board[][h],int board1[][h+2],int s) { // 보드의 정보를 받고 움직이기 
		if (alive == 1) {
			int px = x;
			int py = y;
			int possible = 0;

			ptime = time;
			time += v;
			if ((int)time - (int)ptime > 0) {
				while (possible == 0) {
					dir = rand() % 4;
					int movex = (dir % 2 == 0 ? 0 : -dir + 2);
					int movey = (dir % 2 == 1 ? 0 : -dir + 1);
					if (board[px + movex][py + movey] != 1) possible = 1;
				}

				x += (dir % 2 == 0 ? 0 : -dir + 2);
				y += (dir % 2 == 1 ? 0 : -dir + 1);
			}

			if (x < 0) x = 0;
			if (x > 59) x = 59;
			if (y < 0) y = 0;
			if (y > 49) y = 49;

			if (board[x][y] == 2) {
				return 1;
			}
			// 길을 밟았는지 -> 게임오버
			return 0;
			// 땅을피해가자

		}
	}
	void draw() {
		if (alive == 1) {
			glColor3f(zr, zg, zb);
			glBegin(GL_POLYGON);
			glVertex2f(x, y);
			glVertex2f(x + 1, y);
			glVertex2f(x + 1, y + 1);
			glVertex2f(x, y + 1);
			glEnd();
		}
	}
	void draw_particles() {
		
		if (particle_time > 0) {
			//cout << particle_time << endl;
			for (int i = 0; i < pn; i++) {
				float l = (1 - pow(0.5, (25-particle_time)/4.0))*(particles[i].v);
				glColor4f(particles[i].r, particles[i].g, particles[i].b,3.0* particle_time/25);
				glBegin(GL_POLYGON);
				for(int j=0;j<40;j++)
				glVertex2f(x+l*sin(particles[i].theta)+particles[i].size*(sin(3.1415926*2/40*j)), y + l * cos(particles[i].theta) + particles[i].size*(cos(3.1415926 * 2 / 40 * j)));
				
				glEnd();
			}
			particle_time--;
		}
	}
};