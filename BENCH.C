/*
	ベンチマークテスト  [ bench.c ]
*/
static char sccsid[]="@(#)bench.c   0.12    (UME20)     02/11/90";

#include <conio.h>
#include <dos.h>
#include <graph.h>
#include <stdio.h>
#include <stdlib.h>

#define		WA		10000	/* 待つわ時間 */


char	*bench;				/* benchのイメージバッファ */

main()
{
	init();					/* 初期設定 */
	get_bench();			/* benchiの描画とイメージの取得 */
	_setvisualpage(0);
	startmes();				/*スタートメッセージ */
	
	bench_mark();			/* ベンチマーク本体 */
	
	owari();				/* おしまい処理 */
	exit(0);
	/*NOTREACHED*/
}

/********************************************************************
 ******* 初期設定 ***************************************************
 *******************************************************************/
init()
{
	_setvideomode(_98RESSCOLOR);
	_setactivepage(0);
	_setvisualpage(1);
	_clearscreen(_GCLEARSCREEN);
	_displaycursor(_GCURSOROFF);
}

/********************************************************************
 ******* benchの描画とイメージの取得 ********************************
 *******************************************************************/
get_bench()
{
	/* メモリの確保 */
	bench = (char *)malloc(_imagesize(0,0,32,16));
	if(bench == NULL){
		err_no_memory();
		/*NOTREACHED*/
	}
	/* benchの描画 */
	draw_bench();
	
	/* benchの格納 */
	_getimage(0,0,31,15,bench);
	
	/* benchの後始末 */
	_setcolor(0);
	_rectangle(_GFILLINTERIOR,0,0,32,16);
	_setcolor(7);
}

/******* benchの描画 ***********************************************/
draw_bench()
{
	_moveto(7,7);
	_lineto(6,6);	_lineto(5,6);	_lineto(4,7);	_lineto(0,7);
	_lineto(0,6);	_lineto(4,4);	_lineto(8,4);	_lineto(10,6);
	_lineto(12,6);	_lineto(15,4);	_lineto(20,2);	_lineto(30,0);
	_lineto(31,0);	_lineto(31,2);	_lineto(30,2);	_lineto(20,4);
	_lineto(15,6);	_lineto(14,7);	_lineto(14,8);	_lineto(15,9);
	_lineto(20,11);	_lineto(30,13);	_lineto(31,13);	_lineto(31,15);
	_lineto(30,15);	_lineto(20,13);	_lineto(15,11);	_lineto(12,9);
	_lineto(10,9);	_lineto(8,11);	_lineto(4,11);	_lineto(0,9);
	_lineto(0,8);	_lineto(4,8);	_lineto(5,9);	_lineto(6,9);
	_lineto(7,8);	_lineto(7,7);
}

/********************************************************************
 ******* スタートメッセージ *****************************************
 *******************************************************************/
startmes()
{
	puts("\x1b[2J\x1b[>5h\x1b[8;15H+-----------------------------------------------+");
	puts("\x1b[9;15H|    これからペンチマークテストをやるのぢゃ．   |");
	puts("\x1b[10;15H|                 \x1b[7;5m Hit Any Key \x1b[m                 |");
	puts("\x1b[11;15H+-----------------------------------------------+");
	getch();
	puts("\x1b[>1h\x1b[2J\x1b[7m");
}

/********************************************************************
 ******* ベンチマーク本体 *******************************************
 *******************************************************************/
bench_mark()
{
	struct dostime_t	st,en;	/* 計時用変数 */
	int	i,j;					/* 有象無象 */
	
	putchar(0x07);
	_dos_gettime(&st);			/* 開始時刻 */
	/* １巡目 */
	for(j=0; j<400; j+=16){
		for(i=0; i<640; i+=32){
			_putimage(i,j,bench,_GPSET);
		}
	}
	for(i=0; i<640; i+=32){
		for(j=0; j<400; j+=16){
			_putimage(i,j,bench,_GPRESET);
		}
	}
	for(j=400; j>=0; j-=16){
		for(i=0; i<640; i+=32){
			_putimage(i,j,bench,_GPSET);
		}
	}
	/* ２巡目 */
	for(j=0; j<10; j++){
		for(i=608-32*j; i>=32*j; i-=32){
			_putimage(i,384-16*j,bench,_GPRESET);
		}
		for(i=368-16*j; i>=16*j; i-=16){
			_putimage(32*j,i,bench,_GPRESET);
		}
		for(i=32+32*j; i<640-32*j; i+=32){
			_putimage(i,j*16,bench,_GPRESET);
		}
		for(i=16+16*j; i<=368-16*j; i+=16){
			_putimage(608-32*j,i,bench,_GPRESET);
		}
	}
	for(i=0; i<640; i+=32){
		for(j=0; j<400; j+=16){
			_putimage(i,j,bench,_GPSET);
		}
	}
	
	_dos_gettime(&en);			/* 終了時刻 */
	putchar(0x07);
	i = (int)(en.second - st.second);
	if(i < 0)
		i += 60;
	
	/* おわらい */
	_setcolor(0);
	_rectangle(_GFILLINTERIOR,108,104,534,296);
	_setcolor(7);
	_rectangle(_GBORDER,108,104,534,296);
	_rectangle(_GBORDER,108,105,533,295);
	_settextwindow(7,16,17,63);
	_settextcolor(7);
	_outtext("\nおう，ペンチマークテストとやらが終ったぞ．\n");
	_outtext("\nなんだか知らんが２５００個分のぺンチを描くのに\n\n　");
	_outtext(itoa(i,bench,10));
	_outtext(" 秒 もかかっちょる．\n");
	_outtext("\nもっと精進せぇよ！\n");
	_outtext(" \x1b[17;18H\x1b[7m Hit Any Key \x1b[m");
	getch();
	mesout("\n\n\n\n　　　…………なんぢゃ？\n");
	mesout("\n\n\n\n　“ペンチ”じゃなくて“ベンチ”ぢゃと？\n");
	mesout("\n\n\n\n　‥‥‥‥‥‥‥‥\n");
	mesout("\n\n\n\n　……お呼びでない？\n");
	mesout("\n\n\n\n　‥‥‥‥‥‥‥‥\n");
	mesout("\n\n\n\n　お呼びでない……\n");
	mesout("\n\n\n\n　‥‥‥‥‥‥‥‥\n");
	mesout("\n\n\n\n　お呼びでない ょぅﾃﾞｽﾈ……\n");
	mesout("\n\n\n\n　‥‥‥‥‥‥‥‥‥‥‥‥\n");
	mesout("\n\n\n\n　‥‥‥‥‥‥‥‥\n");
	mesout("\n\n\n\n　‥‥‥‥\n");
	mesout("\n\n\n\n　くぉりゃまたしつれーいたしましたッ！！！\n");
	puts("\x1b[2J");
	puts(" \x1b[18;18H\x1b[7m Hit Any Key \x1b[m");
	_settextwindow(7,16,16,63);
	_settextposition(0,0);
	_outtext("\n");
	i = 1;
	while(!kbhit()){
		_settextcolor(i);
		_outtext("ハラホロヒレハレハラホロヒレハレハラホロヒレハレ");
		matu();
		i++;
		if(i > 7)
			i = 1;
	}
	getch();
}

/******* メッセージ出力 ********************************************/
mesout(mes)
char	*mes;
{
	puts("\x1b[2J");
	_settextposition(0,0);
	_outtext(mes);
	puts(" \x1b[17;18H\x1b[7m Hit Any Key \x1b[m");
	getch();
}

/****** けなげに待つ ***********************************************/
matu()
{
	unsigned int	i;
	for(i=0; i<WA; i++){
		unsigned int	j;
		j = i + 256;		/*プラグマは面倒(汗;)*/
	}
}

/********************************************************************
 ******* おしまい処理 ***********************************************
 *******************************************************************/
owari()
{
	_setactivepage(0);
	_setvisualpage(0);
	_settextcolor(7);
	_displaycursor(_GCURSORON);
	_clearscreen(_GCLEARSCREEN);
	_setvideomode(_DEFAULTMODE);
	puts("\x1b[m\x1b[>1l\x1b[2J\x1b[>5l");
}

/*****************************************************************
 ******* 各種エラー表示 ******************************************
 ****************************************************************/

/******* メモリが足りない ***************************************/
err_no_memory()
{
	fputs("benchi : メモリが足りません\n",stderr);
	exit(1);
	/*NOTREACHED*/
}
