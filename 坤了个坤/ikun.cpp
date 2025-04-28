#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<easyx.h>
#include<windows.h>
#include<math.h>
#include<graphics.h>
#include<time.h>
#include<conio.h>
#include"tools.h"
#pragma  comment (lib,"winmm.lib")
#define max_layer      59
#define block_kinds_2  15
#define block_w        49
#define block_h        49
BOOL ad_flag = false;
int counttt = 0;
int  zbhbig = 0;
const int WIDTH2 = 500;//开始界面500
const int HEIGHT2 = 648;//开始界面648
const int WIDTH = 400;   // 宽度400
const int HEIGHT = 760;  // 高度760
const int INTERVAL = 48; // 间隔   图标得间隔
//  定义图片
IMAGE img_bk;
IMAGE img_bk2;
IMAGE img_item[3];
IMAGE img_begin_back;
IMAGE img_begin[2];
IMAGE img_return[2];
IMAGE jixu;
IMAGE falling;
IMAGE last;
IMAGE getout;
IMAGE replay;
IMAGE guodu;
//加载图片
void loadImg()
{
	loadimage(&img_bk, "ikunimage/bk.png", WIDTH, HEIGHT);
	loadimage(&img_begin_back, "ikunimage/back1.png", WIDTH2, HEIGHT2);
	loadimage(&guodu, "ikunimage/guodu.png", WIDTH, HEIGHT);
	loadimage(img_return + 0, "ikunimage/return1-1.png");
	loadimage(img_return + 1, "ikunimage/return1-2.png");
	loadimage(img_item + 0, "ikunimage/im0-1.png", 48, 48);
	loadimage(img_item + 1, "ikunimage/im1-1.png", 48, 48);
	loadimage(img_item + 2, "ikunimage/im2-1.png", 48, 48);
	loadimage(img_begin + 0, "ikunimage/begin1-1.png");
	loadimage(img_begin + 1, "ikunimage/begin1-2.png");
	loadimage(&jixu, "jixu.jpg");
	loadimage(&falling, "ikunimage/failing.png", 400, 866);
	loadimage(&last, "ikunimage/last.png", WIDTH, HEIGHT);
	loadimage(&getout, "ikunimage/getout.png", 360, 60);
	loadimage(&replay, "ikunimage/replay.png", 360, 60);
	loadimage(&img_bk2, "ikunimage/bk2.png", WIDTH, HEIGHT);
}

//广告函数
void ad()
{
	mciSendString((LPCSTR)"open ./resource/ikunvideo.avi type MPEGVideo alias video", NULL, 0, NULL);	//打开音乐
	mciSendString((LPCSTR)"play video", NULL, 0, NULL);
	Sleep(19000);
	mciSendString((LPCSTR)"close video", 0, 0, 0);
}


//背景音乐
void Ground_MUSIC() {
	mciSendString("open yang2.mp3 alias mymusic", NULL, 0, NULL);
	mciSendString("play mymusic repeat", NULL, 0, NULL);
}
void Check_MUSIC() {	//点击卡片的音乐
	mciSendString("open Eliminate.mp3 alias mymusic_ELI", NULL, 0, NULL);
	mciSendString("play mymusic_ELI from 0", NULL, 0, NULL);
}
void ELIM_MUSIC() {	//消除卡片的音乐
	mciSendString("open shield_hit_wood_wood_3.mp3 alias mymusic_check", NULL, 0, NULL);
	mciSendString("play mymusic_check from 0", NULL, 0, NULL);

}

void Close_Ground_MUSIC() {
	mciSendString("stop easy_disco.mp3", NULL, 0, NULL);
}
bool update = true;
// 算一下  数组得位置再哪里
// 找到图片 x
int  offsetX = (WIDTH - (2 * INTERVAL + 3 * 48)) / 2;
// 找到y得位置
int offsetY = (HEIGHT - (2 * INTERVAL + 3 * 48)) / 2;
// 7个卡位
int store[7] = { 0 };

struct block {
	int layer;//第几层
	int type;//他是哪一种方块
	int row, col;//这个方块在所在的层中，是第几行第几列
	int x, y;
	bool top;//true：在最上面 false：被覆盖了
	int off_x, off_y;//这一层的第一个方块的边界
};
struct block** maps[max_layer];
IMAGE imgBlocks[15][2];
struct block* clickedBlock;//刚刚单击的方块

int chao[7] = { 0, };
void initLayer(int layer, int rows, int cols, int off_x, int off_y) {
	maps[layer] = (struct block**)malloc(sizeof(struct block*) * (rows * cols + 1));
	if (maps[layer] == NULL) exit(1);
	for (int row = 0; row < rows; row++) {
		for (int col = 0; col < cols; col++) {
			maps[layer][row * cols + col] = (struct block*)malloc(sizeof(struct block));
			maps[layer][row * cols + col]->off_x = off_x;
			maps[layer][row * cols + col]->off_y = off_y;
			maps[layer][row * cols + col]->row = row;
			maps[layer][row * cols + col]->col = col;
			maps[layer][row * cols + col]->type = 1 + rand() % block_kinds_2;
			maps[layer][row * cols + col]->top = true;
			maps[layer][row * cols + col]->x = off_x + col * block_w;
			maps[layer][row * cols + col]->y = off_y + row * block_h;
			maps[layer][row * cols + col]->layer = layer;
		}
	}
	maps[layer][rows * cols] = NULL;
}

void initMap() {
	int layer = 0;
	for (; layer < 12; layer++)  initLayer(layer, 1, 1, 30, 440 + layer * 10);
	for (; layer < 24; layer++)  initLayer(layer, 1, 1, 414, 440 + (layer - 12) * 10);
	for (; layer < 30; layer++)  initLayer(layer, 1, 1, 94, 503 + (layer - 24) * 10);
	for (; layer < 36; layer++)  initLayer(layer, 1, 1, 346, 503 + (layer - 30) * 10);
	for (; layer < 42; layer++)  initLayer(layer, 1, 1, 20 + (layer - 36) * 10, 120);
	for (; layer < 48; layer++)  initLayer(layer, 1, 1, 243 - (layer - 42) * 10, 530);

	initLayer(layer++, 4, 3, 159, 309);
	initLayer(layer++, 5, 3, 159, 228);
	initLayer(layer++, 5, 4, 129, 204);
	initLayer(layer++, 5, 5, 99, 180);
	initLayer(layer++, 3, 6, 70, 209);
	initLayer(layer++, 1, 4, 134, 133);
	initLayer(layer++, 1, 4, 134, 118);
	initLayer(layer++, 1, 4, 134, 103);
	initLayer(layer++, 2, 5, 100, 258);
	initLayer(layer++, 2, 5, 100, 243);
	initLayer(layer++, 2, 5, 100, 228);

}
//开始游戏
BOOL BEGIN() {
	//粘贴开始界面
	initgraph(WIDTH2, HEIGHT2);
	loadImg();
	BOOL flag = true;
	MOUSEMSG msg;
	while (flag) {

		BeginBatchDraw();
		putimage(0, 0, &img_begin_back);
		//粘贴开始图标
		putimage(130, 400, img_begin + 1, SRCAND);
		putimage(130, 400, img_begin, SRCPAINT);
		//粘贴结束图标
		putimage(180, 520, img_return + 1, SRCAND);
		putimage(180, 520, img_return, SRCPAINT);
		//获取鼠标信息
		msg = GetMouseMsg();
		if (msg.uMsg == WM_LBUTTONDOWN && msg.x > 130 && msg.x < 355 && msg.y>400 && msg.y < 512) {
			closegraph();
			flag = false;
		}
		else if (msg.uMsg == WM_LBUTTONDOWN && msg.x > 180 && msg.x < 300 && msg.y>520 && msg.y < 641) {
			closegraph();
			return false;
		}
		EndBatchDraw();
	}
}
//过渡
BOOL BEGIN1() //过度画面
{
	initgraph(WIDTH, HEIGHT);
	loadImg();
	BOOL flag = true;
	MOUSEMSG msg;
	while (flag) {

		BeginBatchDraw();
		putimage(0, 0, &guodu);
		//粘贴开始图标
		putimage(30, 600, &jixu);
		//
		putimage(260, 580, img_return + 1, SRCAND);
		putimage(260, 580, img_return, SRCPAINT);

		msg = GetMouseMsg();
		if (msg.uMsg == WM_LBUTTONDOWN && msg.x > 30 && msg.x < 255 && msg.y>600 && msg.y < 713) {
			closegraph();
			flag = false;
		}
		else if (msg.uMsg == WM_LBUTTONDOWN && msg.x > 260 && msg.x < 380 && msg.y>580 && msg.y < 701) {
			closegraph();
			return false;
		}
		EndBatchDraw();
	}
}
//判断失败
BOOL LOSE()
{
	initgraph(400, 766);
	loadImg();
	BOOL flag = true;
	MOUSEMSG msg;
	while (flag) {

		BeginBatchDraw();
		putimage(0, 0, &falling);
		//粘贴开始图标
		//putimage(20, 566, &replay);
		putimage(20, 566, &getout);
		msg = GetMouseMsg();
		if (msg.uMsg == WM_LBUTTONDOWN && msg.x > 20 && msg.x < 380 && msg.y>566 && msg.y < 626) {
			closegraph();
			return false;
		}
		EndBatchDraw();
	}
}
//判断胜利
BOOL WIN()
{
	initgraph(WIDTH, HEIGHT);
	loadImg();
	MOUSEMSG msg;
	BeginBatchDraw();
	putimage(0, 0, &last);
	msg = GetMouseMsg();
	if (msg.uMsg == WM_LBUTTONDOWN && msg.x > 0 && msg.x < 400 && msg.y>0 && msg.y < 760) {
		closegraph();
		return false;
	}
	EndBatchDraw();
}
//三肖代码
void vanish() {
	int k = 0;
	int flag = 0;
	for (int i = 0; i < 7; i++) {
		for (int j = i + 1; j < 7 && flag == 0; j++) {
			if (chao[i] == chao[j] && chao[i] != 0) {
				for (k = j + 1; k < 7 && flag == 0; k++) {
					if (chao[i] == chao[k] && chao[i] != 0) {
						chao[i] = 0;
						chao[j] = 0;
						chao[k] = 0;
						ELIM_MUSIC();
						if (ad_flag == true) {
							counttt++;
						}
						i = 0;
						flag = 1;
					}

				}
				if (k == 7) {
					break;
				}
			}

		}
	}


}
//槽中方块的绘制
void updateWindow() {
	BeginBatchDraw();//开始缓冲
	putimage(0, 0, &img_bk2);
	for (int i = 0; i < max_layer; i++) {
		for (int k = 0; maps[i][k]; k++) {
			struct block* p = maps[i][k];



			IMAGE* img = &imgBlocks[p->type - 1][p->top];
			putimagePNG(p->x, p->y, img);
		}
	}
	for (int i = 0; i < 7; i++) {
		if (chao[i]) {
			int x = 23 + i * 50;
			int y = 618;
			putimagePNG(x, y, &imgBlocks[chao[i] - 1][1]);
		}
	}
	EndBatchDraw();//结束缓冲
}
bool isCover(struct block* b1, struct block* b2) {
	int off = 4;
	return rectIntersect(
		b1->x + off, b1->y + off, b1->x + block_w - off, b1->y + block_h - off,
		b2->x + off, b2->y + off, b2->x + block_w - off, b2->y + block_h - off);
}
//到底
void setTops() {
	for (int layer = max_layer - 1; layer >= 0; layer--) {
		for (int k = 0; maps[layer][k]; k++) {
			if (maps[layer][k]->type == 0) {
				continue;
			}
			for (int j = 0; j < layer; j++) {
				for (int index = 0; maps[j][index]; index++) {
					if (maps[j][index]->type == 0) {
						continue;
					}

					//判断是否发生覆盖
					if (isCover(maps[j][index], maps[layer][k])) {
						maps[j][index]->top = false;
					}
				}
			}
		}
	}
}
//判断点击
bool userClick() {
	ExMessage msg;
	if (peekmessage(&msg) && msg.message == WM_LBUTTONDOWN) {
		for (int layer = max_layer - 1; layer >= 0; layer--) {
			for (int k = 0; maps[layer][k]; k++) {
				struct block* p = maps[layer][k];
				if (p->type &&
					msg.x > p->x && msg.x < p->x + block_w &&
					msg.y > p->y && msg.y < p->y + block_h) {
					clickedBlock = p;
					//zbhbig = 1;
					return   p->top;
				}
			}
		}
		
	}
	if (msg.message == WM_LBUTTONDOWN && msg.x > 310 && msg.x < 390 && msg.y>650 && msg.y < 750) {
		zbhbig = 2;
	}
	return false;
}

void clearBlock(struct block* block) {
	if (!block || block->type == 0)return;

	for (int j = 0; j < block->layer; j++) {
		for (int index = 0; maps[j][index]; index++) {
			if (maps[j][index]->type == 0)continue;
			if (isCover(maps[j][index], block)) {
				maps[j][index]->top = true;
			}
		}
	}
}
//将用户点类型击转入槽中
void work() {
	clearBlock(clickedBlock);
	int i = 0;
	for (; chao[i] && i < 7; i++);
	if (i < 7)
	{
		chao[i] = clickedBlock->type;
	}
	else
	{
		closegraph();
		LOSE();
	}

	clickedBlock->type = 0;
	setTops();

}

void initGame() {
	loadImg();//图片地址
	initMap();
	srand(time(NULL));//配置随机种子
	char fileName[256];
	for (int i = 0; i < block_kinds_2; i++) {
		for (int j = 0, k = 2; j < 2; j++, k--) {

			sprintf_s(fileName, sizeof(fileName), "ikunimage/im%d-%d.png", i, k);
			loadimage(&imgBlocks[i][j], fileName);
		}
	}
	clickedBlock = NULL;
}
int tran()      //过度函数（包含第二关）
{

	if (BEGIN1() == false)  return 0;
	initgraph(WIDTH, HEIGHT);
	loadImg();
	initGame();
	setTops();
	updateWindow();
	int timer = 0;
	while (1) {
		timer += getDelay();
		if (timer >= 10) {
			timer = 0;
			update = true;
		}
		if (userClick()){//zbhbig == 1) {
			Check_MUSIC();
			update = true;
			work();//把点击的方块移动到底部等
			//zbhbig = 0;
		}
		if (update) {
			updateWindow();
		}

		if (zbhbig == 2) {
			Check_MUSIC();
			//Close_Ground_MUSIC();
			mciSendString("pause mymusic", NULL, 0, NULL);
			ad();//广告函数
			//Ground_MUSIC();
			mciSendString("resume mymusic", NULL, 0, NULL);
			ad_flag = true;
			zbhbig = 0;
		}
		vanish();
		if (counttt == 3)
		{
			break;
		}
	}
	return 0;
}

//游戏初始化

//第一关功能的实现
void Game1() {
	// 创建一个头数组
	// 初始换 
	int map[3][3] = { 0 };
	int count = 0;
	for (int r = 0; r < 3; r++)
	{
		for (int c = 0; c < 3; c++)
		{
			map[r][c] = r + 1;  //1 2  3 
		}

	}
	// 打乱这个数组
	for (int r = 0; r < 3; r++)
	{
		for (int c = 0; c < 3; c++)
		{
			// 数组当中得行和列随机打乱  1 2 3 
			int r1 = rand() % 3;
			int c1 = rand() % 3;
			// 打乱行和列  交换打乱
			int temp = map[r1][c1]; 2, 3,
				map[r1][c1] = map[r][c];
			// 交换
			map[r][c] = temp;
		}
	}
	// 绘制地图   1 定义  2 加载  3  绘制
	while (true & counttt < 3)
	{
		BeginBatchDraw();
		// 绘制地图  背景地图
		putimage(0, 0, &img_bk);
		// 绘制  渲染
		for (int r = 0; r < 3 && counttt < 3; r++)
		{
			for (int c = 0; c < 3 && counttt < 3; c++)
			{
				if (map[r][c])
					// 开始渲染             x  , y   图片
					putimage(offsetX + c * (48 + INTERVAL), offsetY + r * (48 + INTERVAL),
						img_item + (map[r][c] - 1));
			}
		}
		for (int i = 0; i < 7; i++)
		{
			if (store[i])
			{
				putimage((i * 50) + 26, 620, img_item + (store[i] - 1));
			}
		}
		EndBatchDraw();
		// 就是  需要 点击了  创建  下面得数组       
		ExMessage  msg = { 0 };
		// 点击鼠标以后  是否有消息  消息是     VM_LBUTTONDOWN 左键
		if (peekmessage(&msg) && msg.message == WM_LBUTTONDOWN)
		{
			Check_MUSIC();
			for (int r = 0; r < 3 && counttt < 3; r++) {
				for (int c = 0; c < 3 && counttt < 3; c++)
				{
					// 鼠标坐标得x
					int  x = offsetX + c * (48 + INTERVAL);
					// 鼠标坐标y
					int y = offsetY + r * (48 + INTERVAL);
					// 判断一下鼠标是否点击了图片
					if (msg.x > x && msg.x < x + 48 && msg.y>y && msg.y < y + 48)
					{
						// 求出 图片对应数组得位置 
						int col = (msg.x - offsetX) / (48 + INTERVAL);
						int row = (msg.y - offsetY) / (48 + INTERVAL);

						// 点击到得图片渲染到下面得数组

						for (int i = 0; i < 7; i++)
						{
							// 下面得数组没有
							if (store[i] == 0) {
								//  上面得数组给下面得数组图片
								store[i] = map[row][col];
								break;
							}
						}
						// 记录一下给得次数
						int cnt = 0;
						for (int i = 0; i < 7; i++)
						{
							if (store[i] == map[row][col]) {
								cnt++;
							}
						}
						// 如果达到了3 
						if (cnt == 3)
						{
							for (int i = 0; i < 7; i++)
							{
								if (store[i] == map[row][col])
								{
									store[i] = 0;
									ELIM_MUSIC();
								}

							}
							count++;
							if (count == 3)
							{
								tran();
								break;
							}
						}
						map[row][col] = 0;

					}

				}

			}
		}

	}

}
int main(void) {
	//loadmusic();//第一关音乐
	mciSendString("open yang2.mp3 alias mymusic", NULL, 0, NULL);//播放音乐mciSendString("pause mymusic", NULL, 0, NULL);
	mciSendString("play mymusic repeat", NULL, 0, NULL);
	if (BEGIN() == false)  return 0;
	initgraph(WIDTH, HEIGHT);
	Game1();
	WIN();
	system("pause");
	return 0;
}
