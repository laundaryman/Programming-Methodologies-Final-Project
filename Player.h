#pragma once


#define w 60		
#define h 50		
#define pn 50
#include <GL/glut.h>


class Player {
public:

	Player() {
		fx = 30;
		fy = 25;
		init();
	}
	Player(int n) {
		fx = 30;
		fy = 25;
		init();
		num = n;
	}
	void init(int xx,int yy) {
		init();
		fx = xx;
		fy = yy;
		x = fx;
		y = fy;
	}
	void init() {
		x = fx;
		y = fy;
		dir = -1;
		start_check = 0;
		isroad = 0;
		player_stat = 0;
		life = 3;
		particle_time = 0;
	}
	int fx, fy;// �ʱ� ��ġ
	int x, y; // ��ġ : ��ǥ
	int dir;  // ���� 12�ú��� �ð�������� 0123
	int player_stat; // player�� �ڱ� ��� ������ 1, �� ������ 2, ��ҿ��� 0
	int start_check; //�������� �ƴ��� üũ
	int isroad = 0; // �濡�ֳ���
	int life = 3; // ���
	int die_x, die_y; // ���� ��ġ
	int num; // �÷��̾� ��ȣ
	class particle {
	public:
		particle() {
			reset();
		}
		void reset() {
			v = (rand() % 100) / 100.0 * 15 + 0.5;
			theta = (rand() % 100);
			a = (rand() % 100) / 100.0*0.4 + 0.2;
			r = 0.7 + (rand() % 100) / 100.0*0.3;
			g = 0.7 + (rand() % 100) / 100.0*0.3;
			b = 0.7 + (rand() % 100) / 100.0*0.3;
			size = (rand() % 100) / 100.0*0.4 + 0.2;
		}
		float v;
		float theta;
		float a;
		float size;
		float r, g, b;
	};
	particle particles[pn];
	int particle_time = 0;
	void set_dir(int d) {
		if (d < 0 || d>3) return;

		if (dir < 0) dir = d;   //ó���̸� ������ ����
		else if (((x == 0 && d == 3) || (x == 59 && d == 1) || (y == 0 && d == 2) || (y == 49 && d == 0))) return; // ��Ÿ���ٰ� �����ùںҰ�
		else if (((dir - d) + 4) % 4 != 2) dir = d; // �ݴ밡 �ƴϸ� �ٲٱ�
		else if (isroad==0&&((x == 0 && d == 1) || (x == 59 && d == 3) || (y == 0 && d == 0) || (y == 49 && d == 2))) dir=d;  // ������ ���� �ù��� ���ϰ���

	}

	int loose_life(int board[][h], int boundary[][h + 2],int ground_board[][h]) {
		life--;
		if (life < 0) life = 0;
		particle_time = 25;
		die_x = x;
		die_y = y;
		for (int i = 0; i < pn; i++) particles[i].reset();
		if (life <= 0) return 4;

		x = fx;
		y = fy;
		dir = -1;
		start_check = 0;
		isroad = 0;

		for (int i = 0; i < w ; i++) 
			for (int j = 0; j < h; j++) {
				if (board[i][j] == 2+10*num) { board[i][j] = ground_board[i][j]; boundary[i + 1][j + 1] = ground_board[i][j]; }
			}
		/*
		board[fx][fy]=1+10*num;
		boundary[fx+1][fy+1] = 1 + 10 * num;
		ground_board[fx][fy] = 1 + 10 * num;
		*/

		return 0;


	}

	int move(int board[][h],int boundary[][h+2],int ground_board[][h]) { // ������ ������ �ް� �����̱�  
		// 0����ϸ� ��� 1����ϸ� �ڻ� -10-n ����ϸ� n�� �÷��̾� ����

		int px=x, py=y;

		if (dir >= 0) {
			x += (dir % 2 == 0 ? 0 : -dir + 2);
			y += (dir % 2 == 1 ? 0 : -dir + 1);
		}
		if (x < 0) x = 0;
		if (x > 59) x = 59;
		if (y < 0) y = 0;
		if (y > 49) y = 49;

		if ((px != x || py != y)&&start_check>=1) {
			board[px][py] = 2+10 * num;
			boundary[px+1][py+1] = 2 + 10 * num;
		}
		if (ground_board[px][py] == 1 + 10 * num && (ground_board[x][y] != 1 + 10 * num)) {
			start_check = 1;
			isroad = 1;
		}
		
		if (board[x][y] == 2 + 10 * num) {
			player_stat = 1;
			return 1;
		}
		else if (board[x][y] == 1 + 10 * num) {
			if(ground_board[px][py] == 1 + 10 * num)player_stat = 0;
			else player_stat = 2;
			start_check = 0; isroad = 0;
		}
		else if (board[x][y] % 10 == 2) {
			
			return -10-(board[x][y] / 10);
		}
		else player_stat = 0;
		
		
							// ���� ��Ҵ���  -> ���

							// ������Ҵ���  -> ĥ�ϱ� ����

		return 0;
	}
	void draw() {
		if(num==0)glColor3f(0, 0.7, 0);
		else if (num == 1)glColor3f(0, 0, 0.7);

		glBegin(GL_POLYGON);
		glVertex2f(x, y);
		glVertex2f(x + 1, y);
		glVertex2f(x + 1, y + 1);
		glVertex2f(x, y + 1);
		glEnd();

		glColor3f(1, 1, 1);
		glBegin(GL_POLYGON);
		glVertex2f(x + 0.1, y + 0.1);
		glVertex2f(x + 0.9, y + 0.1);
		glVertex2f(x + 0.9, y + 0.9);
		glVertex2f(x + 0.1, y + 0.9);
		glEnd();
	}
	void draw(float r,float g,float b) {
		glColor3f(0, 0.7, 0);
		glBegin(GL_POLYGON);
		glVertex2f(x, y);
		glVertex2f(x + 1, y);
		glVertex2f(x + 1, y + 1);
		glVertex2f(x, y + 1);
		glEnd();

		glColor3f(r, g, b);
		glBegin(GL_POLYGON);
		glVertex2f(x + 0.1, y + 0.1);
		glVertex2f(x + 0.9, y + 0.1);
		glVertex2f(x + 0.9, y + 0.9);
		glVertex2f(x + 0.1, y + 0.9);
		glEnd();
	}

	void draw_shake(int pt) {

		float dx = x +0.7* (rand() % 100 / 100.0-0.5);
		float dy = y +0.7* (rand() % 100 / 100.0 - 0.5);

		//float s =(10 - pt) / 10.0;

		glColor3f(0, 0.7, 0);
		glBegin(GL_POLYGON);
		glVertex2f(dx, dy);
		glVertex2f(dx + 1, dy);
		glVertex2f(dx + 1, dy + 1);
		glVertex2f(dx, dy + 1);
		glEnd();

		glColor3f(1, 1, 1);
		glBegin(GL_POLYGON);
		glVertex2f(dx + 0.1, dy + 0.1);
		glVertex2f(dx + 0.9, dy + 0.1);
		glVertex2f(dx + 0.9, dy + 0.9);
		glVertex2f(dx + 0.1, dy + 0.9);
		glEnd();
	}

	void paint(int board1[][h+2],int x,int y) { // �����԰� ĥ�ϱ� (��� ���忡�� �� ���ɼ��� ��������)
		if (x < 0 || x >= w + 2 || y < 0 || y >= h + 2) return;
		else if (board1[x][y] != num*10+1 && board1[x][y] != num * 10 + 2&&board1[x][y]!=3) {
			board1[x][y] = 3;
			paint(board1, x + 1, y);
			paint(board1, x, y + 1);
			paint(board1, x - 1, y);
			paint(board1, x, y - 1); 
		}
	}

	void interpret(int board[][h],int board1[][h + 2],int nboard[][h],int ground_board[][h]) {
		for (int i = 0; i < w+2; i++) {
			for (int j = 0; j < h+2; j++) {
				if(board1[i][j]!=3&&board1[i][j] != 1+10*num) {
					if (i - 1 >= 0 && i - 1 < w && j - 1 >= 0 && j - 1 < h) {
						if (board[i - 1][j - 1] != 1+10*num)nboard[i - 1][j - 1] = 20;
						board[i - 1][j - 1] = 1+10*num;
						ground_board[i - 1][j - 1] = 1 + 10 * num;

					}
					
				}
				if (i == 0 || j == 0 || i == w + 1 || j == h + 1) board1[i][j] = 0;
					else board1[i][j] = board[i - 1][j - 1];
				}
			}
	}
	void draw_particles() {

		if (particle_time > 0) {
			//cout << particle_time << endl;
			for (int i = 0; i < pn; i++) {
				float l = (1 - pow(0.5, (25 - particle_time) / 4.0))*(particles[i].v);
				glColor4f(particles[i].r, particles[i].g, particles[i].b, 3.0* particle_time / 25);
				glBegin(GL_POLYGON);
				for (int j = 0; j<40; j++)
					glVertex2f(die_x + l * sin(particles[i].theta) + particles[i].size*(sin(3.1415926 * 2 / 40 * j)), die_y + l * cos(particles[i].theta) + particles[i].size*(cos(3.1415926 * 2 / 40 * j)));

				glEnd();
			}
			particle_time--;
		}
	}
};