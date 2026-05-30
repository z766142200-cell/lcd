/* Includes ------------------------------------------------------------------*/
#include "device_lcd.h"
#include "midware_GUI.h"
#include "Picture.h"
#include "QDTFT_demo.h"
#include "stm32f1xx_hal.h"
unsigned char Num[10]={0,1,2,3,4,5,6,7,8,9};
void Redraw_Mainmenu(void)
{

	Lcd_Clear(GRAY0);
	
	Gui_DrawFont_GBK16(16,0,BLUE,GRAY0,"全色动电子技术");
	Gui_DrawFont_GBK16(16,20,RED,GRAY0,"液晶测试程序");

	DisplayButtonUp(15,38,113,58); //x1,y1,x2,y2
	Gui_DrawFont_GBK16(16,40,YELLOW,GRAY0,"颜色填充测试");

	DisplayButtonUp(15,68,113,88); //x1,y1,x2,y2
	Gui_DrawFont_GBK16(16,70,BLUE,GRAY0,"文字显示测试");

	DisplayButtonUp(15,98,113,118); //x1,y1,x2,y2
	Gui_DrawFont_GBK16(16,100,RED,GRAY0,"图片显示测试");
	HAL_Delay(100);
}

void Num_Test(void)
{
	uint8_t i=0;
	Lcd_Clear(GRAY0);
	Gui_DrawFont_GBK16(16,20,RED,GRAY0,"Num Test");
	HAL_Delay(200);
	Lcd_Clear(GRAY0);

	for(i=0;i<10;i++)
	{
	Gui_DrawFont_Num32((i%3)*40,32*(i/3)+5,RED,GRAY0,Num[i+1]);
	HAL_Delay(100);
	}
	
}

void Font_Test(void)
{
	Lcd_Clear(GRAY0);
	Gui_DrawFont_GBK16(16,10,BLUE,GRAY0,"文字显示测试");

	HAL_Delay(500);
	Lcd_Clear(GRAY0);
	Gui_DrawFont_GBK16(16,30,YELLOW,GRAY0,"电子技术");
	Gui_DrawFont_GBK16(16,50,BLUE,GRAY0,"专注液晶批发");
	Gui_DrawFont_GBK16(16,70,RED,GRAY0, "全程技术支持");
	Gui_DrawFont_GBK16(0,100,BLUE,GRAY0,"Tel:11111111111");
	Gui_DrawFont_GBK16(0,130,RED,GRAY0, "QQ:11111111");	
	HAL_Delay(500);	
}

void Color_Test(void)
{
	uint8_t i=1;
	Lcd_Clear(GRAY0);
	
	Gui_DrawFont_GBK16(20,10,BLUE,GRAY0,"Color Test");
	HAL_Delay(100);

	while(i--)
	{
		Lcd_Clear(WHITE);
		Lcd_Clear(BLACK);
		Lcd_Clear(RED);
	  	Lcd_Clear(GREEN);
	  	Lcd_Clear(BLUE);
	}		
}

//取模方式 水平扫描 从左到右 低位在前
void showimage(const unsigned char *p) //显示40*40 QQ图片
{
  	int i,j,k; 
	unsigned char picH,picL;
	Lcd_Clear(WHITE); //清屏  
	
	for(k=0;k<4;k++)
	{
	   	for(j=0;j<3;j++)
		{	
			Lcd_SetRegion(40*j+2,40*k,40*j+39,40*k+39);		//坐标设置
		    for(i=0;i<40*40;i++)
			 {	
			 	picL=*(p+i*2);	//数据低位在前
				picH=*(p+i*2+1);				
				LCD_WriteData_16Bit(picH<<8|picL);  						
			 }	
			 HAL_Delay(50);	//延时100ms 
		 }
	}		
}
// 92×100 图片 居中显示（基于你的代码风格修改）
void Lcd_ShowCenter_92x100(const unsigned char *p)
{
    int i;
    unsigned char picH,picL;
    Lcd_Clear(WHITE);

    // 128宽 160高屏幕，92*100居中坐标
    uint16_t x0 = (128 - 92)/2;
    uint16_t y0 = (160 - 100)/2;
    uint16_t x1 = x0 + 92 - 1;
    uint16_t y1 = y0 + 100 - 1;

    Lcd_SetRegion(x0+2, y0, x1, y1);

    // 92*100总像素
    for(i=0; i<92*100; i++)
    {
        picL = *p++;
        picH = *p++;
        LCD_WriteData_16Bit((picH<<8)|picL);
    }
    HAL_Delay(1500);
}
extern const unsigned char gImage_picture_xiaoqiao[18400] ;
void QDTFT_Test_Demo(void)
{
	Lcd_Init();
	LCD_LED_SET;//通过IO控制背光亮				
//  Redraw_Mainmenu();//绘制主菜单(部分内容由于分辨率超出物理值可能无法显示)
//	Color_Test();//简单纯色填充测试
//	Num_Test();//数码管字体测试
//	 Font_Test();//中英文显示测试		
//	 showimage(gImage_qq);//图片显示示例
	 Lcd_ShowCenter_92x100(gImage_picture_xiaoqiao);//图片显示示例
//	 HAL_Delay(300);
	LCD_LED_CLR;//IO控制背光灭	
	
}
