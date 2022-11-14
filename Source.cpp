#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib,"winmm")

#include <iostream>
#include <fstream>
#include <time.h>
#include <vector>
using namespace std;


#include <GL/glut.h>


#include "Player.h"
#include "Board.h"
#include "Zombie.h"


#define WIDTH 792		// window's width
#define HEIGHT 792		// window's height  // WIDTH=HIGHT=unit*(2n+60)   (WIDTH,unit)    �⺻(600,10) Ʋ (720,10)  ���� Ű�� (660,11) (792,11)    Ȯ�� (720,12) (864,12) Ŀ������ ���� ��Ī�� ����
#define UNIT 11


#define w 60		
#define h 50		

#define fps 30


int Zombienum=10; 
int sp_key;				// special key
int display_stat = 0; // display ����. 0�̸� ��ó������. 1�̸� ������. 2�̸� esc������ ��. 3�̸� �ٽý��� .5�̸� �״� ����. 6�̸� ���� �Դ� ����,7�̸� fever 8�� 2�� ���
int pressedhowmany0 = 0;
int vsmode = 0;
int pressedhowmany1= 0;
int pressedhowmany2 = 0;
int pressedhowmany3 = 0;
int no_key;


clock_t start = clock();
clock_t start2 = clock();
clock_t start3 = clock();


clock_t end1;
clock_t end2;
clock_t end3;


Player play(0);
Player play1(1);

Board bd;
vector<Zombie> zombie;


int shake_time = 0;
float shake_x = 0, shake_y = 0;

int redscreen_time = 0;

int pause_time = 0;

int screen_mode = 0; // 0 ���� 1�Ͼ�

int dead_player = 0; //�����ְ� ������ 0 0�� 1 1�� 2 �Ѵ�

int p_display_stat = 0;

int info_mode = 0;


void shake() {  // �̰� �����Ű�� ���� �����մϴ�
	shake_time =20;
	//shake_time = 150;
}
void redscreen(int mode) { // ����ȭ�� ����
	redscreen_time = 22;
	screen_mode = mode;
}

void shake_handle() { // ���� �Լ�
	float size = 2.0;
	if (shake_time > 0&&bd.isfevermode==0) { shake_x = size*(rand() % 20 - 10.0) / 10*(shake_time/20.0);  shake_y = size * (rand() % 20 - 10.0) / 10* (shake_time / 20.0); shake_time--; }
	//if (shake_time > 0) {shake_y= shake_x = size*1.5  * (shake_time )/100;   shake_time--; }
	else { shake_x = 0; shake_y = 0; }
	if (bd.isfevermode == 1) {
		shake_x = size * (rand() % 20 - 10.0) / 10 * (bd.fever / 500.0);  shake_y = size * (rand() % 20 - 10.0) / 10 * (bd.fever / 500.0);
	}
}

void redscreen_handle() {
	if (redscreen_time > 0) { 
		redscreen_time--;
	}
}
void draw_redscreen() {
	if (redscreen_time > 0) {
		if(screen_mode==0)glColor4f(1, 0, 0, 0.1+(redscreen_time/22.0)*0.5); //����
		else if (screen_mode == 1)glColor4f(1, 1, 1, 0.1 + (redscreen_time / 22.0)*0.5); // �Ͼ�
		else if (screen_mode == 2)glColor4f(1, 0, 0,  0.1+ (redscreen_time / 22.0)*0.3); // ���ѻ���
		else if (screen_mode == 3)glColor4f(1, 1, 1, 0.1 + (redscreen_time / 22.0)*0.4); // �����Ͼ�

		glBegin(GL_POLYGON);
		glVertex2f(-20, -20);
		glVertex2f(80, -20);
		glVertex2f(80, 80);
		glVertex2f(-20, 80);
		glEnd();
	}
}


void init() {
	srand(time(NULL));
	if(vsmode==1) play.init(30,25);
	else if (vsmode == 2) {
		play.init(1,1);
		play1.init(58,48);
	}
	bd.init(vsmode);
	redscreen_time = 0;
	pause_time = 0;
	shake_time = 0;
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	

}

void draw_string(void * font, const char* str, float x, float y) {
	glRasterPos2f(x, y);
	for (int i = 0; i < strlen(str); i++)
		glutBitmapCharacter(font, str[i]);
}

void processNormalKeys(unsigned char key, int x, int y) {
	end3 = clock();
	if (end3 - start3 > 1000 / (fps+5)) {
	if (key == 13) {
		if(display_stat!=1)PlaySound(TEXT("coin.wav"), NULL, SND_ASYNC | SND_ALIAS);
			no_key = 4;
	}	
	else if (key == 27) {

		no_key = 5;
	}
	else if (key == 'R') bd.record_reset();// r������ ��� ���� (������ ���)
	else if (key == 'w'|| key == 'W') no_key = 0;
	else if (key == 'd' || key == 'D') no_key = 1;
	else if (key == 's' || key == 'S') no_key = 2;
	else if (key == 'a' || key == 'A') no_key = 3;
	else if (key == 8) no_key = -1;
	else no_key = -2;
	start3 = end3;
	}

	if (display_stat == 0) {
		if (info_mode == 0) {
			if (no_key == 4 && vsmode == 1) {
				display_stat = 1;
				init();
				//			bd.start_game_time = clock();
				zombie.clear();
				int z = 0;
				Zombienum = bd.hard;
				bd.Alivezombie = Zombienum;
				for (int i = 0; z <= Zombienum; i++) {  // ���� ����
					int zx = rand() & 60;
					int zy = rand() % 50;
					if ((zx < 29 || zx>31) && (zy < 24 || zy>26)) {
						Zombie zo(zx, zy);
						zombie.push_back(zo);
						z++;
					}
				}

			}
			else if (no_key == 4 && vsmode == 0) {
				vsmode = pressedhowmany0 + 1;
				if (vsmode == 2) {
					display_stat = 8;
					init();
					zombie.clear();
					Zombienum = 0;
				}
			}
			else if (no_key == 5) {
				p_display_stat = display_stat;
				display_stat = 2;
			}

			else if (no_key == -1 && vsmode == 1) {
				vsmode = 0;
			}
		}
		
	}
	else  if (display_stat == 2) {
		if (no_key == 4 && pressedhowmany2 == 0) {
			exit(0);
		}
		else if (no_key == 4 && pressedhowmany2 == 1) {
			display_stat = p_display_stat;
		}
	}
	else if (display_stat == 3) {
		if (no_key == 4 && pressedhowmany3 == 0) {
			display_stat = 0;
			vsmode = 0;

		}
		else if (no_key == 4 && pressedhowmany3 == 1) {
			exit(0);
		}
	}
	else if (display_stat == 8) {
		if (0 <= no_key && no_key <= 3) play1.set_dir(no_key);
	}


	//	cout << "$";
}
void processSpecialKeys(int key, int x, int y) {
	/* Implement: Set key input */
	end2 = clock();
	if (end2 - start2 > 1000 / (fps+5)) {
		switch (key)
		{
		case GLUT_KEY_UP:
			sp_key = 0;
			break;
		case GLUT_KEY_RIGHT:
			sp_key = 1;
			break;
		case GLUT_KEY_DOWN:
			sp_key = 2;
			break;
		case GLUT_KEY_LEFT:
			sp_key = 3;
			break;
		case GLUT_KEY_F1:
			sp_key = -2;
			break;
		default:
			sp_key = -1;
			break;
		}
		start2 = end2;
	}
	if (display_stat == 0) {
		if (info_mode == 0) {
			if (sp_key == -2) info_mode = 1;

			if (vsmode == 1) {
				if (sp_key == 0 && pressedhowmany1 > 0) {
					pressedhowmany1--;
				}
				else if (sp_key == 2 && pressedhowmany1 < 3) {
					pressedhowmany1++;
				}
			}
			else if (vsmode == 0) {
				if (sp_key == 0 && pressedhowmany0 > 0) pressedhowmany0--;
				else if (sp_key == 2 && pressedhowmany0 < 1) pressedhowmany0++;
			}
		}
		else if (info_mode == 1) {
			if (sp_key == -2) info_mode = 0;
		}
	}
	else if (display_stat == 1||display_stat==8) {
		play.set_dir(sp_key);
	}
	else if (display_stat == 2) {
		if (sp_key == 1 && pressedhowmany2 == 0) {
			pressedhowmany2 = 1;
		}
		else if (sp_key == 3 && pressedhowmany2 == 1) {
			pressedhowmany2 = 0;
		}
	}
	else if (display_stat == 3) {
		if (sp_key == 1 && pressedhowmany3 == 0) {
			pressedhowmany3 = 1;
		}
		else if (sp_key == 3 && pressedhowmany3 == 1) {
			pressedhowmany3 = 0;
		}
	}
}

void idle() {
	/* Implement: Move the square */

		
	

		end1 = clock();

		if (end1 - start > 1000 / fps) {

			if ((bd.game_stat == 2 || bd.game_stat == 4) && display_stat == 1) {
				display_stat = 3;
				
			}//restart display�� ����. �������̾��ٰ� �̱� ��Ȳ�϶���!
			if (display_stat == 1) {

				if (bd.fever>0&&bd.isfevermode==0) bd.fever -= 0.04;
				else if (bd.fever > 0 && bd.isfevermode == 1) bd.fever -= 0.9;
				else if ((int)bd.fever == 0) {
					bd.game_stat =0;
					bd.isfevermode = 0;
				}
				if (pause_time > 0) { start = end1; pause_time--; return; }


				if (play.life <= 0) {
					bd.game_stat = 4;
					display_stat = 3;
					return;
				}

				int play_move = play.move(bd.board, bd.boundary, bd.ground_board);
				int chk = 0;  // ����Ҿ����� üũ�ϴ� ����
				int dir = rand() % 4;
				for (int i = 0; i < Zombienum; i++) {
					if (bd.game_stat == 0) {
						if (play_move == 1 || zombie[i].move(bd.board, bd.boundary, dir) == 1 || (play.x == zombie[i].x&&play.y == zombie[i].y&&zombie[i].alive == 1))
						{ // �÷��̾� ��ȥ�� �׾�����, ���� ���� ��Ҵ���, ����� �÷��̾�� �������� üũ
							pause_time = 10;
							display_stat = 5;
							bd.fever = 0;
							glutPostRedisplay();
							return;
							break;
						}
					}
					else if (bd.game_stat == 3) {
						if (play_move == 1) {
							pause_time = 10;
							display_stat = 5;
							bd.fever = 0;
							bd.isfevermode = 0;
							glutPostRedisplay();
							return;
							break;
						}
						zombie[i].move(bd.board, bd.boundary, dir);
					}
				}

				if (chk == 0&&bd.isfevermode==0) bd.set_stat(0);

				//if (pause_time == 20) return;   //

				if (play.life <= 0) {
					bd.game_stat = 4;
					display_stat = 3;
				}// �������� 0�̸� ���ӽ����� 4�� ���� (���ӿ���),displaystat�� 3���ε�

				

				if (play.player_stat == 2 && chk == 0) {  // �÷��̾ �������� ĥ�ϱ� && �� �������� ���� ��찡 �ƴϾ���� (���ÿ� ĥ�ϸ鼭 ���������Ҵ°�츦 ���ܽ�Ű�� ����)
					play.paint(bd.boundary, 0, 0);
					play.interpret(bd.board, bd.boundary,bd.new_board,bd.ground_board);

					PlaySound(TEXT("coin.wav"), NULL, SND_ASYNC | SND_ALIAS);
				}
				if (bd.Alivezombie == 0||bd.ground_percent>=bd.wincondition) {
					bd.set_stat(2);
					if (bd.record[pressedhowmany1] == 0 || bd.game_time < bd.record[pressedhowmany1]) { bd.record[pressedhowmany1] = bd.game_time; bd.isnewrecord = 1; bd.record_update(); }
				}   // ���� ���� ������ ���ӽ����� 2�� ����(�̱� ����), displaystat�� 3���ε�
				

				if ((int)bd.fever == 99&&bd.game_stat!=2) {
					bd.set_stat(3);
					bd.isfevermode = 1;
				}  // fever����� ��� ���ӽ����� 3���� ����.

				//start = end1

				


				int chk2 = 0;
				for (int i = 0; i < Zombienum; i++) {  // ���� ���� ������ ���� ����
					if (bd.board[zombie[i].x][zombie[i].y] == 1) {
						if (zombie[i].alive == 1) { 
							bd.Alivezombie--; chk2 = 1; zombie[i].particle_time = 25; 
							pause_time = 5;
							display_stat = 5;
							zombie[i].alive = 0;
							display_stat = 6;
							if(bd.fever<=75&&bd.isfevermode==0) bd.fever += 25;
							else if(bd.isfevermode==0) bd.fever = 100;
							cout << "killed!!!" << endl;
							cout << bd.Alivezombie<<" zombies left" << endl;
							glutPostRedisplay();
						}
						

					}
				}
				if (bd.isfevermode == 1) {
					shake();
				}
				if (chk2 == 1) shake();

				if (display_stat == 6) return;

				

			}
			if (display_stat == 8) {

				if (pause_time > 0) { start = end1; pause_time--; return; }


				if (play.life <= 0&& play1.life <= 0) {
					bd.game_stat = 7;
					display_stat = 3;
					return;
				}
				if (play.life <= 0 ) {
					bd.game_stat = 6;
					display_stat = 3;
					return;
				}
				if ( play1.life <= 0) {
					bd.game_stat = 5;
					display_stat = 3;
					return;
				}


				int play_move = play.move(bd.board, bd.boundary,bd.ground_board);
				int play1_move = play1.move(bd.board, bd.boundary, bd.ground_board);
				int chk = 0;  // ����Ҿ����� üũ�ϴ� ����
				int chk1 = 0;  // ����Ҿ����� üũ�ϴ� ����
				
				

					if (bd.game_stat == 0) {
						if ((-10 == play1_move &&-11==play1_move) || (play.x == play1.x&&play.y == play1.y)) {
							dead_player = 2;
							pause_time = 10;
							display_stat = 5;
							glutPostRedisplay();
						
							return;
						}
						if (play_move == 1 ||-10==play1_move )
						{ // �÷��̾� ��ȥ�� �׾�����, �ٸ��ְ� �׿�����  �㫉����
							dead_player = 0;
							pause_time = 10;
							display_stat = 5;
							glutPostRedisplay();
						

							return;
						}
						if (play1_move == 1 || -11 == play_move )
						{ // �÷��̾� ��ȥ�� �׾�����, �ٸ��ְ� �׿�����  �㫉����
							dead_player = 1;
							pause_time = 10;
							display_stat = 5;
							glutPostRedisplay();
						
							return;
						}

						if (bd.game_time >= bd.time_limit) { // �ð��ʰ�
							if (bd.myground > bd.myground1) bd.game_stat = 5;
							if (bd.myground < bd.myground1) bd.game_stat = 6;
							if (bd.myground == bd.myground1) bd.game_stat = 7;
							display_stat = 3;
							return;

						}
						if (bd.ground_percent >= bd.wincondition_2) // ���� ����
						{
							bd.game_stat = 5;
							display_stat = 3;
							return;
						}
						if (bd.ground_percent1 >= bd.wincondition_2) // ���̸���
						{
							bd.game_stat = 6;
							display_stat = 3;
							return;
						}


					}
						
				

				//if (chk == 0&&) bd.set_stat(0);

				//if (pause_time == 20) return;   //

				



				if (play.player_stat == 2 && chk == 0) {  // �÷��̾ �������� ĥ�ϱ� && �� �������� ���� ��찡 �ƴϾ���� (���ÿ� ĥ�ϸ鼭 ���������Ҵ°�츦 ���ܽ�Ű�� ����)
					play.paint(bd.boundary, 0, 0);
					play.interpret(bd.board, bd.boundary, bd.new_board, bd.ground_board);
					PlaySound(TEXT("coin.wav"), NULL, SND_ASYNC | SND_ALIAS);

				}
				if (play1.player_stat == 2 && chk == 0) {  // �÷��̾ �������� ĥ�ϱ� && �� �������� ���� ��찡 �ƴϾ���� (���ÿ� ĥ�ϸ鼭 ���������Ҵ°�츦 ���ܽ�Ű�� ����)
					play1.paint(bd.boundary, 0, 0);
					play1.interpret(bd.board, bd.boundary, bd.new_board, bd.ground_board);
					PlaySound(TEXT("coin.wav"), NULL, SND_ASYNC | SND_ALIAS);

				}
			/*	if ( bd.ground_percent >= bd.wincondition) {
					bd.set_stat(2);
					if (bd.record[pressedhowmany1] == 0 || bd.game_time < bd.record[pressedhowmany1]) { bd.record[pressedhowmany1] = bd.game_time; bd.isnewrecord = 1; bd.record_update(); }
				}   // ���� ���� ������ ���ӽ����� 2�� ����(�̱� ����), displaystat�� 3���ε�*/



				   //start = end1



			}
			if (display_stat == 5&&bd.isfevermode==0) {
				if(pause_time==5&&play.life>0) PlaySound(TEXT("player_die.wav"), NULL, SND_ASYNC | SND_ALIAS);
				if (pause_time > 0) { pause_time--; }
				else {
					if (vsmode == 1) {
						display_stat = 1;
						if (play.life > 0) {

							play.loose_life(bd.board, bd.boundary,bd.ground_board);  // �̰� ���� pauseŸ���� �����ϰ� �ٷ� ����Ʈ�����÷��� ��Ű�� ����� �غ���
							bd.set_stat(1);
							shake();
							redscreen(0);

						}
					}
					else if (vsmode == 2) {
						display_stat = 8;
						if (dead_player ==2) {

							if(play.life>0)play.loose_life(bd.board, bd.boundary, bd.ground_board);  // �̰� ���� pauseŸ���� �����ϰ� �ٷ� ����Ʈ�����÷��� ��Ű�� ����� �غ���
							if (play1.life>0)play1.loose_life(bd.board, bd.boundary, bd.ground_board);
							//bd.set_stat(1);
							shake();
							redscreen(0);

						}
						else if (dead_player==0&&play.life > 0) {

							play.loose_life(bd.board, bd.boundary, bd.ground_board);  // �̰� ���� pauseŸ���� �����ϰ� �ٷ� ����Ʈ�����÷��� ��Ű�� ����� �غ���
							//bd.set_stat(1);
							shake();
							redscreen(0);

						}
						else if (dead_player == 1 && play1.life > 0) {

							play1.loose_life(bd.board, bd.boundary, bd.ground_board);  // �̰� ���� pauseŸ���� �����ϰ� �ٷ� ����Ʈ�����÷��� ��Ű�� ����� �غ���
							//bd.set_stat(1);
							shake();
							redscreen(0);

						}

					}
					
				}
			}

			if (display_stat == 6) {

				if (pause_time == 3) PlaySound(TEXT("zombie_die.wav"), NULL, SND_ASYNC | SND_ALIAS);

				if (pause_time > 0) { pause_time--; }
				else {
					display_stat = 1;
					shake();
					redscreen(1);
					
				}
			}


			start = end1;

			if ((display_stat != 5&&display_stat!=6)) {
				shake_handle();
				redscreen_handle();
			}
			

			glutPostRedisplay();
			
		}

		

	
		

}
void renderScene() {
	// Clear Color and Depth Buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//float tt = (shake_time / 60.0);

	int space = (WIDTH / UNIT - w) / 2;
	gluOrtho2D(shake_x-space, w+ shake_x+ space,  shake_y- space, w+shake_y+ space);

	float wid=0.5;
	glColor3f(1,1,1);
	glBegin(GL_POLYGON);
	glVertex2f(-wid, -wid);
	glVertex2f(w+ wid, -wid);
	glVertex2f(w+ wid, w+ wid);
	glVertex2f(-wid, w+ wid);
	glEnd();

	glColor3f(0, 0, 0);
	glBegin(GL_POLYGON);
	glVertex2f(0, 0);
	glVertex2f(w , 0);
	glVertex2f(w , w );
	glVertex2f(0, w );
	glEnd();


	//gluOrtho2D(tt, w - tt, 0 + tt, w - tt);
	//gluOrtho2D(shake_x+tt, w + shake_x-tt, 0 + shake_y+tt, w + shake_y-tt);
	if (display_stat == 0) {  // ��ó�� ���̵� ����
		if (info_mode == 1) {
			bd.draw_info();
		}
		else {
			glColor3f(1, 1, 1);
			draw_string(GLUT_BITMAP_9_BY_15, "press f1 for info", 2, 58);

			if (vsmode == 0) {
				glColor3f(1 - 0.8*pressedhowmany0, 1 - 0.8*pressedhowmany0, 1 - 0.8*pressedhowmany0);
				draw_string(GLUT_BITMAP_TIMES_ROMAN_24, "1 player", 26, 40);
				glColor3f(0.2 + 0.8*pressedhowmany0, 0.2 + 0.8*pressedhowmany0, 0.2 + 0.8*pressedhowmany0);
				draw_string(GLUT_BITMAP_TIMES_ROMAN_24, "2 player", 26, 20);

			}
			else if (vsmode == 1) {
				glColor3f(0, 1, 1);
				draw_string(GLUT_BITMAP_TIMES_ROMAN_24, "EASY", 27, 45);
				glColor3f(1, 1, 0);
				draw_string(GLUT_BITMAP_TIMES_ROMAN_24, "NORMAL", 25, 35);
				glColor3f(1, 0.7, 0);
				draw_string(GLUT_BITMAP_TIMES_ROMAN_24, "HARD", 27, 25);
				glColor3f(1, 0, 0);
				draw_string(GLUT_BITMAP_TIMES_ROMAN_24, "HELL", 27, 15);
				if (pressedhowmany1 == 0) {
					glColor3f(0, 1, 1);
					glBegin(GL_TRIANGLES);
					glVertex3f(26.7, 45.9, 0);
					glVertex3f(25.5, 45.4, 0);
					glVertex3f(25.5, 46.4, 0);
					glEnd();
					bd.hard = 3;
				}
				if (pressedhowmany1 == 1) {
					glColor3f(1, 1, 0);
					glBegin(GL_TRIANGLES);
					glVertex3f(24.7, 35.9, 0);
					glVertex3f(23.5, 35.4, 0);
					glVertex3f(23.5, 36.4, 0);
					glEnd();
					bd.hard = 6;
				}
				if (pressedhowmany1 == 2) {
					glColor3f(1, 0.7, 0);
					glBegin(GL_TRIANGLES);
					glVertex3f(26.7, 25.9, 0);
					glVertex3f(25.5, 25.4, 0);
					glVertex3f(25.5, 26.4, 0);
					glEnd();
					bd.hard = 10;
				}
				if (pressedhowmany1 == 3) {
					glColor3f(1, 0, 0);
					glBegin(GL_TRIANGLES);
					glVertex3f(26.7, 15.9, 0);
					glVertex3f(25.5, 15.4, 0);
					glVertex3f(25.5, 16.4, 0);
					glEnd();
					bd.hard = 20;
				}
				bd.difficulty = pressedhowmany1;
			}
		}
	}
	else if (display_stat == 1) {  // ������
		if (no_key == 5) {
			p_display_stat = display_stat;
			display_stat = 2;
		}
		if (no_key == -1) {
			display_stat = 0;
		}
		bd.draw();
		bd.new_efferc_draw();

		for (int i = 0; i < Zombienum; i++) {
			zombie[i].draw();
		}
		if(bd.game_stat==0) play.draw();
		else if(bd.game_stat==3) play.draw((float)(rand() % 100) / 100, (float)(rand() % 100) / 100, (float)(rand() % 100) / 100);
		bd.gui(play.life,0);
		for (int i = 0; i < Zombienum; i++) {
			zombie[i].draw_particles();
		}

		play.draw_particles();

		if (bd.isfevermode==1 && clock() % 500 <= 1000 / fps) {  // �󸶾ȳ����黡����
			redscreen(3);
		}

		draw_redscreen();
	
	}
	else if (display_stat == 2) {  // esc�������� ȭ��
		glColor3f(1, 1, 1);
		draw_string(GLUT_BITMAP_TIMES_ROMAN_24, "DO YOU WANT TO QUIT?", 16, 40);
		if (pressedhowmany2 == 0) {
			glColor3f(1, 1, 0);
			draw_string(GLUT_BITMAP_TIMES_ROMAN_24, "YES", 18, 25);
			glColor3f(1, 1, 1);
			draw_string(GLUT_BITMAP_TIMES_ROMAN_24, "NO", 40, 25);
		}
		else if (pressedhowmany2 == 1) {
			glColor3f(1, 1, 1);
			draw_string(GLUT_BITMAP_TIMES_ROMAN_24, "YES", 18, 25);
			glColor3f(1, 1, 0);
			draw_string(GLUT_BITMAP_TIMES_ROMAN_24, "NO", 40, 25);
		}
	}
	else if (display_stat == 3) {  // ���� �������� ȭ��
		bd.draw();
		bd.new_efferc_draw();

		bd.isfevermode= 0;
		for (int i = 0; i < Zombienum; i++) {
			zombie[i].draw();
		}
		//play.draw();
		if(vsmode==1)bd.gui(play.life,0);
		else if (vsmode == 2)bd.gui(play.life, play1.life);

		for (int i = 0; i < Zombienum; i++) {
			zombie[i].draw_particles();
		}
		play.draw_particles();
		draw_redscreen();
		
		bd.message();
		if (pressedhowmany3 == 0) {
			glColor3f(0, 1, 0);
			draw_string(GLUT_BITMAP_TIMES_ROMAN_24, "Restart", 18, 22);
			glColor3f(1, 1, 1);
			draw_string(GLUT_BITMAP_TIMES_ROMAN_24, "Quit", 38, 22);
		}
		else if (pressedhowmany3 == 1) {
			glColor3f(1, 1, 1);
			draw_string(GLUT_BITMAP_TIMES_ROMAN_24, "Restart", 18, 22);
			glColor3f(0, 1, 0);
			draw_string(GLUT_BITMAP_TIMES_ROMAN_24, "Quit", 38, 22);
		}
	}
	else if (display_stat == 5) {  // ��������
		if (vsmode==1) {
			if (no_key == 5) {
				p_display_stat = display_stat;
				display_stat = 2;
			}
			bd.draw();
			bd.new_efferc_draw();

			for (int i = 0; i < Zombienum; i++) {
				zombie[i].draw();
			}
			play.draw_shake(pause_time);
			bd.gui(play.life,0);
			for (int i = 0; i < Zombienum; i++) {
				zombie[i].draw_particles();
			}
			play.draw_particles();
			draw_redscreen();
		}
		else if (vsmode == 2) {
			bd.draw();
			bd.new_efferc_draw();

			if (dead_player == 0) { play.draw_shake(pause_time);	play1.draw(); }
			if (dead_player == 1) { play1.draw_shake(pause_time);	play.draw(); }
			if (dead_player == 2) { play.draw_shake(pause_time);	play1.draw_shake(pause_time); }



			bd.gui(play.life,play1.life);
			for (int i = 0; i < Zombienum; i++) {
				zombie[i].draw_particles();
			}

			play.draw_particles();
			play1.draw_particles();

			draw_redscreen();
		}
	}
	else if (display_stat == 6) {  // ���μ���
		if (no_key == 5) {
			p_display_stat = display_stat;
			display_stat = 2;
		}
		bd.draw();
		bd.new_efferc_draw();


		for (int i = 0; i < Zombienum; i++) {
			zombie[i].draw();
		}
		play.draw();
		bd.gui(play.life,play1.life);
		
		play.draw_particles();
		draw_redscreen();

	}
	else if (display_stat == 8) {  // 2�� ������
		if (no_key == 5) {
			p_display_stat = display_stat;
			display_stat = 2;
		}
		if (no_key == -1) {
			vsmode = 0;
			display_stat = 0;
		}
		bd.new_efferc_draw();
		bd.draw();


		if (bd.game_stat == 0) { play.draw(); play1.draw(); }

		bd.gui(play.life, play1.life);
		for (int i = 0; i < Zombienum; i++) {
			zombie[i].draw_particles();
		}

		play.draw_particles();
		play1.draw_particles();


		if (bd.game_time >= bd.time_limit-10000&&(int)bd.game_time%1000<=1000/fps) {  // �󸶾ȳ����黡����
			redscreen(2);
		}
		draw_redscreen();

	}
	//cout << display_stat<<endl;
	glutSwapBuffers();
}

void main(int argc, char **argv) {
	// init GLUT and create Window
	PlaySound(TEXT("coin.wav"), NULL, SND_ASYNC | SND_ALIAS);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(50, 50);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow("ddang dda mugi");
	init();

	// register callbacks
	glutDisplayFunc(renderScene);
	glutSpecialFunc(processSpecialKeys);
	glutKeyboardFunc(processNormalKeys);
	glutIdleFunc(idle);

	// enter GLUT event processing cycle
	glutMainLoop();
}
