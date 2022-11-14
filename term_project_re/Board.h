#pragma once

#include <GL/glut.h>
#include <string>




#define w 60
#define h 50		


class Board {

public :
	Board() {
		init(1);
	}
	void init(int vsmode1) {
		vsmode = vsmode1;
		start_game_time = clock();
		for (int i = 0; i < w; i++)
			for (int j = 0; j < h; j++)
			{
				board[i][j] = 0;
				ground_board[i][j] = 0;
				new_board[i][j] = 0;
			}

		for (int i = 0; i < w+2; i++)
			for (int j = 0; j < h+2; j++)
				boundary[i][j] = 0;

		if (vsmode == 1) {
			for (int i = 29; i <= 31; i++) {
				for (int j = 24; j <= 26; j++) {
					board[i][j] = 1;
					boundary[i + 1][j + 1] = 1;
					ground_board[i][j] = 1;
				}
			}
		}
		else if (vsmode == 2) {
			for (int i = 0; i <= 2; i++) {
				for (int j = 0; j <= 2; j++) {
					board[i][j] = 1;
					boundary[i + 1][j + 1] = 1;
					ground_board[i][j] = 1;
				}
			}
			for (int i = 57; i <= 59; i++) {
				for (int j = 47; j <= 49; j++) {
					board[i][j] = 11;
					boundary[i + 1][j + 1] = 11;
					ground_board[i][j] = 11;
				}
			}
		}
		game_stat = 0;
		myground = 0;
		myground1 = 0;

		ground_percent = 0;
		ground_percent1 = 0;
		fever = 0;
		isfevermode = 0;
		isnewrecord = 0;

		game_time = 0;

		record_in.open("record.txt",ios::in);
		for (int i = 0; i < 4; i++)
			if (!record_in.eof())record_in >> record[i];

		
		record_in.close();
	}


	void record_update() {
		record_out.open("record.txt", ios::trunc);
		for (int i = 0; i < 4; i++)
			record_out << record[i] << endl;
		record_out.close();
	}
	void record_reset() {
		record_out.open("record.txt", ios::trunc);
		record_out.close();
	}

	void draw_string(void * font, const char* str, float x, float y) {
		glRasterPos2f(x, y);
		for (int i = 0; i < strlen(str); i++)
			glutBitmapCharacter(font, str[i]);
	}

	int board[w][h]; //  현재 보드 상태 (땅이랑 길 표시)    땅 1 길 2  + 주인 번호 *10

	int boundary[w + 2][h + 2]; // 보드의 가장 바깥 여백만 생각할 곳

	int new_board[w][h]; // 방금 생겨난곳 표시

	int ground_board[w][h]; // 땅만 표시 // 길은 제외


	int game_stat; // 한번 죽음 = 1, 게임을 이김=2, 평상시= 0 게임이 끝나버림=4 , fevermode=3  5 0이이김 6 1가이김 7 무승뷰

	int Alivezombie; // 좀비의 살아있는 수
	float fever = 0; // fevermode도달: 100이면 fevermode, 하나 잡으면 25 추가, 1초당 1씩 감소
	int myground=0; // 내땅갯수
	int myground1= 0; // 내땅갯수

	int ground_percent = 0; // 내땅 퍼센트
	int ground_percent1 = 0; // 내땅 퍼센트 1플레이어


	int wincondition = 85; // 승리조건	
	int wincondition_2 = 65; // 승리조건	2인모드


	int difficulty = 0; //난이도
	int hard;// 좀비 개수
	int isfevermode = 0;
	float start_game_time = 0;
	float game_time=0;
	float end_game_time = 0;
	int isnewrecord = 0;
	float record[4]; // 최고기록
	int vsmode = 0;
	int time_limit = 60000; // 시간제한 ms

	ifstream record_in;
	ofstream record_out;

	void set_stat(int a) {
		game_stat = a;
	}

	void draw() {  // 판을 그림

		
		for (int i = 0; i < w; i++)
			for (int j = 0; j < h; j++) {
				
				if (board[i][j]%10 == 1 && new_board[i][j]==0) {
				if(board[i][j]/10==0) glColor3f(1, 1, 0);
				else if (board[i][j] / 10 == 1) glColor3f(1, 0.5, 1);
				glBegin(GL_POLYGON);
				glVertex2f(i, j);
				glVertex2f(i + 1, j);
				glVertex2f(i + 1, j + 1);
				glVertex2f(i, j + 1);
				glEnd();
				}
				if (board[i][j]%10 == 2) {
				//glColor3f(1, 0.6, 0.6);
					if (board[i][j] / 10 == 0) glColor3f(1, 0.6, 0);
					else if (board[i][j] / 10 == 1) glColor3f(0.6, 0.2, 0.6);

				glBegin(GL_POLYGON);
				
				glVertex2f(i+0.1, j+0.1);
				glVertex2f(i + 0.9, j+0.1);
				glVertex2f(i + 0.9, j + 0.9);
				glVertex2f(i+0.1, j + 0.9);

				glEnd();
				}
			
			}


	}
	void new_efferc_draw() {
		
		for (int i = 0; i < w; i++)
			for (int j = 0; j < h; j++) {
				if (new_board[i][j] > 0) {
					new_board[i][j]--;
					if(board[i][j]==1+0)glColor3f((20 - new_board[i][j]) / 20.0, (20 - new_board[i][j]) / 20.0, 0);
					if (board[i][j] == 1 + 10)glColor3f((20 - new_board[i][j]) / 20.0, 0.2*(20 - new_board[i][j]) / 20.0, (20 - new_board[i][j]) / 20.0);

					//glColor3f(1, 1, 0);
					//glColor3f((20-new_board[i][j])/20.0, (20 - new_board[i][j]) / 20.0, 0);
					//glColor3f(1, 1, new_board[i][j] / 20.0);
					glBegin(GL_POLYGON);
					glVertex2f(i, j);
					glVertex2f(i + 1, j);
					glVertex2f(i + 1, j + 1);
					glVertex2f(i, j + 1);
					glEnd();
					
				}
			}
	}
	void gui(int life,int life1) {
		if (vsmode == 1) {
			glColor3f(1, 1, 1);
			draw_string(GLUT_BITMAP_TIMES_ROMAN_24, ("LIFE : " + to_string(life)).c_str(), 2, 57);  // 라이프 표시

			int x = 5, y = 55;
			int s = 1;
			for (int i = 0; i < life; i++) {
				glColor3f(1, 0, 0);
				glBegin(GL_POLYGON);
				glVertex2f(x + 5 * s * i, y);
				glVertex2f(x + 5 * s * i + s, y + s);
				glVertex2f(x + 5 * s * i + 2 * s, y);
				glVertex2f(x + 5 * s * i, y - 2 * s);
				glVertex2f(x + 5 * s * i - 2 * s, y);
				glVertex2f(x + 5 * s * i - s, y + s); // 하트모양 그리기
				glEnd();

			}

			glColor3f(1, 1, 1);
			glBegin(GL_QUADS);
			glVertex2f(w, h + 0.8);
			glVertex2f(0, h + 0.8);
			glVertex2f(0, h - 0.25);
			glVertex2f(w, h - 0.25);
			glEnd();
			glBegin(GL_QUADS);
			glVertex2f(w - 20, h + 10);
			glVertex2f(w - 19.5, h + 10);
			glVertex2f(w - 19.5, h + 0.25);
			glVertex2f(w - 20, h - 0.25);
			glEnd();
			glBegin(GL_QUADS);
			glVertex2f(20.25, h + 10);
			glVertex2f(19.75, h + 10);
			glVertex2f(19.75, h - 0.25);
			glVertex2f(20.25, h - 0.25);
			glEnd();
			if (isfevermode == 0) {
				glColor3f(1 - 0.02*fever, 1 - 0.02*fever, 1 - 0.005*fever);
				glBegin(GL_QUADS);
				glVertex2f(fever*w / 100, h + 0.8);
				glVertex2f(0, h + 0.8);
				glVertex2f(0, h - 0.25);
				glVertex2f(fever*w / 100, h - 0.25);
				glEnd(); // ui 가르는 구분선: fever생각한 게이지
			}
			else {
				glColor3f((float)(rand() % 100) / 100, (float)(rand() % 100) / 100, (float)(rand() % 100) / 100);
				glBegin(GL_QUADS);
				glVertex2f(fever*w / 100, h + 0.8);
				glVertex2f(0, h + 0.8);
				glVertex2f(0, h - 0.25);
				glVertex2f(fever*w / 100, h - 0.25);
				glEnd(); // ui 가르는 구분선: fever생각한 게이지
			}
			glColor3f(1, 0, 0);
			draw_string(GLUT_BITMAP_TIMES_ROMAN_10, "FEVER", 29, 50);

			myground = 0;
			for (int i = 0; i < w; i++)
				for (int j = 0; j < h; j++) {
					if (board[i][j] == 1) myground++;
				}  // 내땅 수 계산
			ground_percent = myground * 100 / w / h;
			//	if (ground_percent >= 90) 	game_stat = 2;   // 내땅이 85퍼 이상이면 게임 이김

			glColor3f(1, 1, 1);
			glBegin(GL_QUADS);
			glVertex2f(58, 55);
			glVertex2f(42, 55);
			glVertex2f(42, 53);
			glVertex2f(58, 53);
			glEnd();
			glColor3f(1, 0.5, 0);
			glBegin(GL_QUADS);
			glVertex2f(42 + 16 * (float)ground_percent / 100, 55);
			glVertex2f(42, 55);
			glVertex2f(42, 53);
			glVertex2f(42 + 16 * (float)ground_percent / 100, 53);
			glEnd();
			glColor3f(0, 0, 1);
			glBegin(GL_QUADS);
			glVertex2f(42 + 16 * (float)wincondition / 100 + 0.08, 55);
			glVertex2f(42 + 16 * (float)wincondition / 100 - 0.08, 55);
			glVertex2f(42 + 16 * (float)wincondition / 100 - 0.08, 53);
			glVertex2f(42 + 16 * (float)wincondition / 100 + 0.08, 53);  // 따먹은 땅을 게이지로 표현
			glEnd();
			glColor3f(1, 1, 1);
			draw_string(GLUT_BITMAP_TIMES_ROMAN_24, ("LAND : " + to_string(ground_percent) + "%").c_str(), 42, 57);
			draw_string(GLUT_BITMAP_TIMES_ROMAN_10, (to_string(wincondition) + "%").c_str(), 41 + 16 * (float)wincondition / 100 + 0.08, 51.8);

			if (hard == 3) {
				draw_string(GLUT_BITMAP_TIMES_ROMAN_24, "DIFFICULTY : ", 22.5, 57);
				glColor3f(0, 1, 1);
				draw_string(GLUT_BITMAP_TIMES_ROMAN_24, "EASY ", 27, 54);
			}
			if (hard == 6) {
				draw_string(GLUT_BITMAP_TIMES_ROMAN_24, "DIFFICULTY : ", 22.5, 57);
				glColor3f(1, 1, 0);
				draw_string(GLUT_BITMAP_TIMES_ROMAN_24, "NORMAL ", 25, 54);
			}
			if (hard == 10) {
				draw_string(GLUT_BITMAP_TIMES_ROMAN_24, "DIFFICULTY : ", 22.5, 57);
				glColor3f(1, 0.7, 0);
				draw_string(GLUT_BITMAP_TIMES_ROMAN_24, "HARD ", 27, 54);
			}
			if (hard == 20) {
				draw_string(GLUT_BITMAP_TIMES_ROMAN_24, "DIFFICULTY : ", 22.5, 57);
				glColor3f(1, 0, 0);
				draw_string(GLUT_BITMAP_TIMES_ROMAN_24, "HELL ", 27, 54);
			}


			glColor3f(0, 1, 0);
			if (record[difficulty] > 1000)
				draw_string(GLUT_BITMAP_HELVETICA_18, ("best:" + to_string((int)(record[difficulty] / 1000)) + "." + to_string((int)record[difficulty] / 10 % 100) + "s").c_str(), 25.5, 51.5); //최고기록 표시
			else
				draw_string(GLUT_BITMAP_HELVETICA_18, "No Record", 25.7, 51.5);


			glColor3f(0.2, 0.5, 1);
			if (game_stat != 2 && game_stat != 4) end_game_time = clock();
			game_time = -start_game_time + end_game_time;
			draw_string(GLUT_BITMAP_HELVETICA_12, ("time:" + to_string((int)(game_time / 1000)) + "." + to_string((int)game_time / 10 % 100)).c_str(), 27.5, 48); // 현재 시간
		}
		else if (vsmode == 2) {
			glColor3f(1, 1, 0);
			draw_string(GLUT_BITMAP_TIMES_ROMAN_24, "PLAYER 1", 2, 57);
			glColor3f(1,0.5,1);
			draw_string(GLUT_BITMAP_TIMES_ROMAN_24, "PLAYER 2", 48, 57);
			int x = 5, y = 55;
			int s = 1;
			for (int i = 0; i < life; i++) {
				glColor3f(1, 0, 0);
				glBegin(GL_POLYGON);
				glVertex2f(x + 5 * s * i, y);
				glVertex2f(x + 5 * s * i + s, y + s);
				glVertex2f(x + 5 * s * i + 2 * s, y);
				glVertex2f(x + 5 * s * i, y - 2 * s);
				glVertex2f(x + 5 * s * i - 2 * s, y);
				glVertex2f(x + 5 * s * i - s, y + s); // 하트모양 그리기
				glEnd();

			}
			x = 45; y = 55;
			s = 1;
			for (int i = 0; i < life1; i++) {
				glColor3f(1, 0, 0);
				glBegin(GL_POLYGON);
				glVertex2f(x + 5 * s * i, y);
				glVertex2f(x + 5 * s * i + s, y + s);
				glVertex2f(x + 5 * s * i + 2 * s, y);
				glVertex2f(x + 5 * s * i, y - 2 * s);
				glVertex2f(x + 5 * s * i - 2 * s, y);
				glVertex2f(x + 5 * s * i - s, y + s); // 하트모양 그리기
				glEnd();

			}

			glColor3f(1, 1, 1);
			glBegin(GL_QUADS);
			glVertex2f(w - 20, h + 10);
			glVertex2f(w - 19.5, h + 10);
			glVertex2f(w - 19.5, h);
			glVertex2f(w - 20, h);
			glEnd();
			glBegin(GL_QUADS);
			glVertex2f(20, h + 10);
			glVertex2f(19.5, h + 10);
			glVertex2f(19.5, h);
			glVertex2f(20, h);  // 양쪽 가르는 하얀선
			glEnd();
			
			glColor3f(0, 0, 0);
			glBegin(GL_QUADS);
			glVertex2f(w, h + 2.2);
			glVertex2f(0, h + 2.2);
			glVertex2f(0, h);
			glVertex2f(w, h);    // 게이지
			glEnd();

			glColor3f(1,1, 1);
			glBegin(GL_QUADS);
			glVertex2f(w, h + 2.1);
			glVertex2f(0, h + 2.1);
			glVertex2f(0, h+0.1);
			glVertex2f(w, h+0.1);    // 게이지
			glEnd();

			glColor3f(0, 0, 0);
			glBegin(GL_QUADS);
			glVertex2f(w, h + 2);
			glVertex2f(0, h + 2);
			glVertex2f(0, h + 0.2);
			glVertex2f(w, h + 0.2);    // 게이지
			glEnd();
		
			
			myground = 0;
			myground1 = 0;
			
			for (int i = 0; i < w; i++)
				for (int j = 0; j < h; j++) {
					if (ground_board[i][j] == 1) myground++;
					else if (ground_board[i][j] == 11) myground1++;
				}
			ground_percent = myground * 100 / w / h;
			ground_percent1 = myground1 * 100 / w / h;

			glColor3f(1, 1, 0);
			glBegin(GL_QUADS);
		//	cout << myground << endl;
			glVertex2f((float)myground / h, h + 2);
			glVertex2f(0, h + 2);
			glVertex2f(0, h+0.2 );
			glVertex2f((float)myground / h, h + 0.2);  // 노란색 게이지
			glEnd(); 
			glColor3f(1, 0.5, 1);
			glBegin(GL_QUADS);
			glVertex2f(w, h + 2);
			glVertex2f(w-(float)myground1/h, h + 2);
			glVertex2f(w-(float)myground1/h, h + 0.2);
			glVertex2f(w, h + 0.2);  // 보라색 게이지
			glEnd();
		
			if (myground + myground1 >= 2950) {
				float dx = (rand()%100/100.0-0.5)*0.5;
				glColor3f(1, 0, 0);
				glBegin(GL_QUADS);
				glVertex2f((float)(w*h-myground1+myground)/2/h +0.6+dx, h + 2);
				glVertex2f((float)(w*h - myground1 + myground) /2/ h - 0.6 + dx, h + 2);
				glVertex2f((float)(w*h - myground1 + myground) /2/ h - 0.6 + dx, h );
				glVertex2f((float)(w*h - myground1 + myground) /2/ h + 0.6 + dx, h );  // 경합선
				glEnd();
			}
			
				if (game_stat != 2 && game_stat != 4 && game_stat != 5 && game_stat != 6 && game_stat != 7) end_game_time = clock();
			game_time = -start_game_time + end_game_time;
			if (game_time <= time_limit-10000) { // 시간많이남음
				glColor3f(0.5, 1, 1);
				draw_string(GLUT_BITMAP_TIMES_ROMAN_24, "TIME", 27, 57);
				draw_string(GLUT_BITMAP_TIMES_ROMAN_24, (to_string((int)(time_limit - game_time) / 1000 / 60) + ":" + to_string((int)(time_limit - game_time) / 1000 % 60)).c_str(), 28, 54);
			}
			else if (game_time > time_limit-10000) { // 시간 얼마안남음
				glColor3f(1, 0, 0); 
				float dx = (rand() % 100) / 100.0 - 0.2;
				float dy= (rand() % 100) / 100.0 - 0.2;
				draw_string(GLUT_BITMAP_TIMES_ROMAN_24, "TIME", 27+dx, 57+dy);
				draw_string(GLUT_BITMAP_TIMES_ROMAN_24, (to_string((int)(time_limit - game_time) / 1000 / 60) + ":" + to_string((int)((time_limit - game_time)>0? (time_limit - game_time) :0) / 1000 % 60)).c_str(), 28+dx, 54+dy);
			}

			glColor3f(1, 1, 1);
			glBegin(GL_QUADS);
			glVertex2f(w*(wincondition_2 / 100.0), h + 2);
			glVertex2f(w*(wincondition_2 / 100.0) + 0.3, h + 2);
			glVertex2f(w*(wincondition_2 / 100.0) + 0.3, h +0.2);
			glVertex2f(w*(wincondition_2 / 100.0), h + 0.2);  //승리 조건선

			glColor3f(0, 0, 0);
			glBegin(GL_QUADS);
			glVertex2f(w*(wincondition_2 / 100.0)+0.1, h + 2);
			glVertex2f(w*(wincondition_2 / 100.0) + 0.2, h + 2);
			glVertex2f(w*(wincondition_2 / 100.0) + 0.2, h + 0.2);
			glVertex2f(w*(wincondition_2 / 100.0)+0.1, h + 0.2);  //승리 조건선
			glEnd();




			glColor3f(1, 1, 1);
			glBegin(GL_QUADS);
			glVertex2f(w*((100-wincondition_2) / 100.0), h + 2);
			glVertex2f(w*((100 - wincondition_2) / 100.0) + 0.3, h + 2);
			glVertex2f(w*((100 - wincondition_2) / 100.0) + 0.3, h + 0.2);
			glVertex2f(w*((100 - wincondition_2) / 100.0), h + 0.2);  // 승리조건선
			glEnd();

			glColor3f(0, 0, 0);
			glBegin(GL_QUADS);
			glVertex2f(w*((100 - wincondition_2) / 100.0)+0.1, h + 2);
			glVertex2f(w*((100 - wincondition_2) / 100.0) + 0.2, h + 2);
			glVertex2f(w*((100 - wincondition_2) / 100.0) + 0.2, h + 0.2);
			glVertex2f(w*((100 - wincondition_2) / 100.0)+0.1, h + 0.2);  // 승리조건선
			glEnd();



			glColor3f(0.5, 0.5, 1);
			draw_string(GLUT_BITMAP_HELVETICA_18, "VS!!!", 28, 50.5);



			glColor3f(1, 0, 0);
			glBegin(GL_POLYGON);
			glVertex2f(w/2.0, h + 2.3);
			glVertex2f(w / 2.0+0.7, h + 3);
			glVertex2f(w/2.0-0.7, h +3);  // 승리조건선
			glEnd();

		}
	}

	void message() { // 메세지 출력
		if (game_stat == 2 || (4 <= game_stat && game_stat <= 7)) {
			glColor4f(0, 0, 0,0.8);
			glBegin(GL_QUADS);
			glVertex2f(47, 33);
			glVertex2f(14, 33);
			glVertex2f(14, 17);
			glVertex2f(47, 17);
			glEnd();
			glColor3f(1, 1, 1);
			glLineWidth(5.0);
			glBegin(GL_LINES);
			glVertex2f(47, 33);
			glVertex2f(14, 33);
			glEnd();
			glLineWidth(5.0);
			glBegin(GL_LINES);
			glVertex2f(14, 17);
			glVertex2f(14, 33);
			glEnd();
		//	glColor3f(1, 0, 0);
			glLineWidth(5.0);
			glBegin(GL_LINES);
			glVertex2f(14, 17);
			glVertex2f(47, 17);
			glEnd();
		//	glColor3f(1, 0, 0);
			glLineWidth(5.0);
			glBegin(GL_LINES);
			glVertex2f(47, 33);
			glVertex2f(47, 17);
			glEnd();//POPUP 창
			glColor3f(1, 0, 0);
		}
		if (game_stat == 4) {
			glColor3f(1, 0, 0);
			draw_string(GLUT_BITMAP_TIMES_ROMAN_24, "GAME OVER",23.5,30);
		}
		else if (game_stat == 2) {
			glColor3f(0.5, 0.5, 1);
			draw_string(GLUT_BITMAP_TIMES_ROMAN_24, "YOU WON!", 24, 30);

			glColor3f(1, 1, 1);
			draw_string(GLUT_BITMAP_HELVETICA_18, ("time:"+to_string((int)(game_time / 1000)) + "." + to_string((int)game_time / 10 % 100)+"s").c_str(), 26, 27.5);  // 시간 표시

			if (clock() / 100 % 10 < 7) {
				glColor3f(1, 0, 0);
				if(isnewrecord==1)
					draw_string(GLUT_BITMAP_HELVETICA_18, "NEW RECORD!", 24, 25);  // 최고기록
			}

		}
		else if(game_stat==5){
			glColor3f(1, 0.7, 0);
			draw_string(GLUT_BITMAP_TIMES_ROMAN_24, "PLAYER 1 WIN!", 22, 30);
		}
		else if (game_stat == 6) {
			glColor3f(1, 0.5, 1);
			draw_string(GLUT_BITMAP_TIMES_ROMAN_24, "PLAYER 2 WIN!", 22, 30);
		}
		else if (game_stat == 7) {
			glColor3f(0, 0, 1);
			draw_string(GLUT_BITMAP_TIMES_ROMAN_24, "DRAW!", 26., 30);
		}

	}

	void draw_info() {
		glColor3f(1, 1, 1);
		draw_string(GLUT_BITMAP_9_BY_15, "press f1 to close info", 2, 58);  // 줄간격 2 ~3
		glColor3f(1, 0.7, 0);
		draw_string(GLUT_BITMAP_9_BY_15, "1P MODE:", 5, 53);
		glColor3f(1, 1, 1);
		draw_string(GLUT_BITMAP_9_BY_15, "Kill the Zombies and Expand your territory", 5, 51);
		draw_string(GLUT_BITMAP_9_BY_15, "Run faster to challenge the", 5, 49);
		glColor3f(1, 1, 0);
		draw_string(GLUT_BITMAP_9_BY_15, "NEW RECORD", 28, 49);
		glColor3f(1, 1, 1);
		draw_string(GLUT_BITMAP_9_BY_15, "Kill zombies in a row for", 5, 47);
		glColor3f(1, 1, 0);
		draw_string(GLUT_BITMAP_9_BY_15, "UNSTOPPABLE FEVER MODE", 26, 47);
		glColor3f(1, 0.7, 0);
		draw_string(GLUT_BITMAP_9_BY_15, "WIN CONDITION: 85% land expansion or killing all zombies", 5, 45);
		glColor3f(1, 0.5, 1);
		draw_string(GLUT_BITMAP_9_BY_15, "2P MODE:", 5, 41);
		glColor3f(1, 1, 1);
		draw_string(GLUT_BITMAP_9_BY_15, "Challenge your opponent for territory expansion", 5, 39);
		glColor3f(1, 0.5, 1);
		draw_string(GLUT_BITMAP_9_BY_15, "WIN CONDITION: 65% land expansion...", 5, 37);
		draw_string(GLUT_BITMAP_9_BY_15, "OR taking over more land in limited time", 5, 35);
		glColor3f(1, 0, 0);
		draw_string(GLUT_BITMAP_9_BY_15, "CAUTION: YOU HAVE ONLY THREE LIFE", 5, 29);
		glColor3f(1, 1, 1);
		draw_string(GLUT_BITMAP_9_BY_15, "If you lose all your life, you are defeated AT ONCE!", 5, 27);
		draw_string(GLUT_BITMAP_9_BY_15, "Press enter to select menu", 5, 23);
		draw_string(GLUT_BITMAP_9_BY_15, "Press esc to pause or quit", 5, 21);
		draw_string(GLUT_BITMAP_9_BY_15, "Press backspace if you want to go back to starting menu", 5, 19);
		//draw_string(GLUT_BITMAP_9_BY_15, "or go back to starting menu", 5, 17);

		glColor3f(1, 1, 1);
		draw_string(GLUT_BITMAP_9_BY_15, "Gamemaker: 2017-12932 Ahn Hyeong Seo", 26, 9);
		draw_string(GLUT_BITMAP_9_BY_15, "           2017-10673 Kim Sung Yoon", 26, 7);
	}

};