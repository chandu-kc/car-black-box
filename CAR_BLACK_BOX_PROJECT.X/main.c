/*
 * File:   main.c
 * Author: LENOVO
 *
 * Created on December 14, 2023, 7:38 AM
 */


#include <xc.h>

#include "clcd.h"
#include "adc.h"
#include "matrix_keypad.h"
#include<string.h>
#include "timer0.h"
#include "uart.h"
#include "i2c.h"
#include "ext_eeprom.h"
#include "ds1307.h"


void write_internal_eeprom(unsigned char address, unsigned char data);
unsigned char read_internal_eeprom(unsigned char address);

char ev[][3] = {"ON", "GR", "GN", "G1", "G2", "G3", "G4", "C ", "DL", "CL", "ST", "CP"};
char menu[][20] = {"View log      ", "Clear log       ", "Download log     ", "Set time      ", "Change password     "};
char o_flag = 0;

//----
unsigned char clock_reg[3];
unsigned char calender_reg[4];
unsigned char time[9];
unsigned char date[11];

//--

void display_time(void) {
    clcd_print(time, LINE2(0));


}

static void get_time(void) {
    clock_reg[0] = read_ds1307(HOUR_ADDR);
    clock_reg[1] = read_ds1307(MIN_ADDR);
    clock_reg[2] = read_ds1307(SEC_ADDR);

    if (clock_reg[0] & 0x40) {
        time[0] = '0' + ((clock_reg[0] >> 4) & 0x01);
        time[1] = '0' + (clock_reg[0] & 0x0F);
    } else {
        time[0] = '0' + ((clock_reg[0] >> 4) & 0x03);
        time[1] = '0' + (clock_reg[0] & 0x0F);
    }
    time[2] = ':';
    time[3] = '0' + ((clock_reg[1] >> 4) & 0x0F);
    time[4] = '0' + (clock_reg[1] & 0x0F);
    time[5] = ':';
    time[6] = '0' + ((clock_reg[2] >> 4) & 0x0F);
    time[7] = '0' + (clock_reg[2] & 0x0F);
    time[8] = '\0';
}

static void init_config(void) {
    init_clcd();
    init_adc();
    init_matrix_keypad();
    init_timer0();
    GIE = 0;

    init_i2c();
    init_ds1307();

}

int pq = 1;

void main(void) {
    init_config();
    unsigned int mode = 1;
    char array[10] = "WELCOME";
    clcd_print(array, LINE1(5));
    unsigned long int de = 0;
    unsigned int comeback = 0;

    for (de = 0; de < 100000; de++);
    unsigned short adc_reg_val;
    int g = 0, c = 0, previous_g = 0;
    int flag = 2;
    char arr[10];
    static int address = 0x00;
    //char time[9] = "12:00:00";
    clcd_print("ON  ", LINE2(9));
    //-----------------
    static char password[5];
    
//     if (pq) {
//        write_ext_eeprom(200, '1');
//        write_ext_eeprom(201, '1');
//        write_ext_eeprom(202, '1');
//        write_ext_eeprom(203, '0');
//        pq = 0;
//    }
    int pq1;
    for (pq1 = 0; pq1 < 4; pq1++) {
        password[pq1] = read_ext_eeprom(200 + pq1);
    }





    int p = 0;
    char enter_password[5] = {0};

    int attempt = 3, q = 0;
    extern int count;
    //--------------
    int m = 0;
    int sflag = 0;
    int time1 = 0;
    int prekey;
    //------
    int scv = 0;
    int time2 = 0;
    int prekey2;
    unsigned int del = 0;
    int once = 1;
    //----
    int cf;
    int d;
    int once1 = 1;
    int del1 = 0;
    //---
    int p1 = 0;
    int p2 = 0;
    char enter_password1[5] = {0};
    char enter_password2[5] = {0};
    unsigned int del23=0;
     unsigned int del24=0;
     //-----
     int x2=0;
     
     
     //------
     int settime=1;
     unsigned int time23=0;
     int bsw=0;
     int del25=0;
     int setf=0;
     
     int hour12;
     int minutes12;
     int seconds12;
     int delay1234=0;
    get_time();
    //-----
    arr[0] = time[0];
    arr[1] = time[1];
    arr[2] = time[3];
    arr[3] = time[4];
    arr[4] = time[6];
    arr[5] = time[7];

    arr[6] = 'O';
    arr[7] = 'N';

    arr[8] = 0 + 48;
    arr[9] = 0 + 48;
    for (int ad = 0; ad < 10; ad++) {

        write_internal_eeprom((address), arr[ad]);

        address++;
        if (address == 100) {
            o_flag = 1;
            address = 0;
        }
    }

    while (1) {





        unsigned int key = read_switches(STATE_CHANGE);
        get_time();
        if (g == 7 && (key == 2 || key == 3)) {
            g = 2;
        } else if (key == 2 && g < 6) {
            g++;
        } else if (key == 3 && g > 1) {
            g--;
        } else if (key == 1) {
            g = 7;
        }
        //------------------
        if (g != previous_g) {
            arr[0] = time[0];
            arr[1] = time[1];
            arr[2] = time[3];
            arr[3] = time[4];
            arr[4] = time[6];
            arr[5] = time[7];

            arr[6] = ev[g][0];
            arr[7] = ev[g][1];

            arr[8] = (adc_reg_val / 10) + 48;
            arr[9] = (adc_reg_val % 10) + 48;

            for (int ad = 0; ad < 10; ad++) {

                write_internal_eeprom((address), arr[ad]);

                address++;
                if (address == 100) {
                    o_flag = 1;
                    address = 0;
                }
            }
        }

        previous_g = g;


        switch (mode) {
            case 1:

                get_time(); //read time
                display_time();
                clcd_print(ev[g], LINE2(9));
                clcd_print(" TIME    ", LINE1(0));
                clcd_print("EV  ", LINE1(9));
                clcd_print("SP ", LINE1(13));
                //line 2
                // clcd_print("12:00:00 ", LINE2(0));


                adc_reg_val = read_adc(CHANNEL4) / 10.33;
                if (adc_reg_val < 100) {
                    clcd_putch('0' + (adc_reg_val / 10), LINE2(13));
                    clcd_putch('0' + (adc_reg_val % 10), LINE2(14));
                }

                clcd_putch(' ', LINE2(15));

                if (key == 5) {
                    mode = 2;
                    comeback = 0;
                    clcd_print("                ", LINE2(0));
                }
                break;
            case 2:
                
                if(del24++==1200)
                {
                    mode=1;
                }
                else if(key!=255)
                {
                    del24=0;
                }
                    
                password[0] = read_ext_eeprom(200 + 0);
                password[1] = read_ext_eeprom(200 + 1);
                password[2] = read_ext_eeprom(200 + 2);
                password[3] = read_ext_eeprom(200 + 3);

                if (p < 4) {
                    clcd_print(" ENTER PASSWORD ", LINE1(0));

                    if (key == 5) {
                        enter_password[p] = '1';

                        clcd_putch('*', LINE2(p + 4));
                        p++;

                    } else if (key == 6) {
                        enter_password[p] = '0';

                        clcd_putch('*', LINE2(p + 4));
                        p++;
                    }
                } else {
                    if (strcmp(enter_password, password) == 0) {

                        mode = 3;
                        p=0;

                    } else {
                        // clcd_print(" WRONG PASSWORD ", LINE2(0));

                        if (attempt > 1) {
                            del24=0;
                            clcd_print(" WRONG PASSWORD ", LINE1(0));

                            clcd_print("ATTEMPT LEFT", LINE2(3));
                            clcd_putch(attempt + 47, LINE2(0));
                            if (q++ == 1000) {
                                attempt--; //make attempt =0 when mode is change
                                p = q = 0;
                                CLEAR_DISP_SCREEN;
                            }

                        } else {


                            del24=0;
                            clcd_print("YOU ARE BLOCKED>", LINE1(0));

                            clcd_print("WAIT--> ", LINE2(0));


                            clcd_putch('0' + (count % 10), LINE2(10));
                            clcd_putch('0' + ((count / 10) % 10), LINE2(9));
                            clcd_putch('0' + ((count / 10) / 10), LINE2(8));
                            clcd_print(" sec ", LINE2(11));

                            TMR0IE = 1;
                            PEIE = 1;
                            GIE = 1;
                            if (count == 0) {
                                TMR0IE = 0;
                                GIE = 0;
                                PEIE = 0;
                                p = 0;
                                attempt = 3;
                                count = 120;
                                clcd_print("                ", LINE2(0));
                            }




                        }

                    }
                }





                break;
                //-----
            case 3:
            {
                clcd_print(menu[m], LINE1(2));

                clcd_print(menu[m + 1], LINE2(2));

                unsigned int key1 = read_switches(LEVEL_CHANGE);

                if (key1 != 255) {
                    time1++;
                    prekey = key1;
                    if (time1 == 1000) {
                        time1 = 0;
                        if (key1 == 5) {
                            CLEAR_DISP_SCREEN;
                            mode = m + sflag + 3 + 1;

                            // clcd_print("hi ssas",LINE1(0));

                            //clcd_putch(mode+48,LINE1(10));
                            //while(1);
                        } else if (key1 == 6) {
                            //long press operation for mkp 6
                            mode=1;
                            CLEAR_DISP_SCREEN;
                        }
                    }
                } else if (time1 < 1000 && time1 > 0) {
                    time1 = 0;
                    if (prekey == 6) {
                        if (sflag == 0) {
                            sflag = 1;
                        } else if (m < 3) {
                            m++;
                        }

                    } else if (prekey == 5) {
                        if (sflag == 1) {
                            sflag = 0;
                        } else if (m > 0) {
                            m--;
                        }
                    }
                }




                //----------------

                if (sflag == 0) {
                    clcd_print("* ", LINE1(0));
                    clcd_print("  ", LINE2(0));
                } else if (sflag == 1) {
                    clcd_print("  ", LINE1(0));
                    clcd_print("* ", LINE2(0));
                }



                break;
            }
                //----
            case 4:
            {

                char a = 0;
                /*
                if(key==5 )
                {
                    if(o_flag == 0)
                    {
                        if(scv< (address/10) -1)
                        scv++;
                    }
                    else
                    {
                        a = address/10;
                        if(scv< 9)
                            scv++;
                    }
                }
                else if(key==6 && scv>0 )
                {
                    scv--;
                
                }
                 */
                unsigned int key2 = read_switches(LEVEL_CHANGE);
                //if(del++==300)
                {
                    // del=0;
                    if (key2 != 255) {
                        time2++;
                        prekey2 = key2;
                        if (time2 == 1000) {
                            time2 = 0;
                            if (key2 == 6) {
                                mode = 3;
                                //                            time2=0; 
                            }
                        }
                    } else if (time2 < 1000 && time2 > 0) {
                        time2 = 0;

                        if (prekey2 == 5) {
                            if (o_flag == 0) {
                                if (scv < (address / 10) - 1)
                                    scv++;

                            } else {
                                a = address / 10;
                                if (scv < 9)
                                    scv++;
                            }
                        } else if (prekey2 == 6 && scv > 0) {
                            scv--;

                        }


                    }
                }




                char event1 = read_internal_eeprom(0 + ((scv + a) % 10 * 10));
                char event2 = read_internal_eeprom(1 + ((scv + a) % 10 * 10));
                char event3 = read_internal_eeprom(2 + ((scv + a) % 10 * 10));
                char event4 = read_internal_eeprom(3 + ((scv + a) % 10 * 10));
                char event5 = read_internal_eeprom(4 + ((scv + a) % 10 * 10));
                char event6 = read_internal_eeprom(5 + ((scv + a) % 10 * 10));
                char event7 = read_internal_eeprom(6 + ((scv + a) % 10 * 10));
                char event8 = read_internal_eeprom(7 + ((scv + a) % 10 * 10));
                char event9 = read_internal_eeprom(8 + ((scv + a) % 10 * 10));
                char event10 = read_internal_eeprom(9 + ((scv + a) % 10 * 10));

                clcd_print("view log...>>>>>", LINE1(0));
                clcd_putch(scv + 48, LINE2(0));
                clcd_putch(' ', LINE2(1));
                clcd_putch(event1, LINE2(2));
                clcd_putch(event2, LINE2(3));
                clcd_putch(':', LINE2(4));
                clcd_putch(event3, LINE2(5));
                clcd_putch(event4, LINE2(6));
                clcd_putch(':', LINE2(7));
                clcd_putch(event5, LINE2(8));
                clcd_putch(event6, LINE2(9));
                clcd_putch(' ', LINE2(10));
                clcd_putch(event7, LINE2(11));
                clcd_putch(event8, LINE2(12));
                clcd_putch(' ', LINE2(13));
                clcd_putch(event9, LINE2(14));
                clcd_putch(event10, LINE2(15));


                break;
            }
            case 5:
            {
                if (once) {
                    once--;
                    address = 0;
                    o_flag = 0;
                    clcd_print("CLEAR LOG  .....", LINE1(0));
                    clcd_print("SUCCESSFULL .....", LINE2(0));


                    arr[0] = 48;
                    arr[1] = 48;
                    arr[2] = 48;
                    arr[3] = 48;
                    arr[4] = 48;
                    arr[5] = 48;

                    arr[6] = 'C';
                    arr[7] = 'L';

                    arr[8] = 48;
                    arr[9] = 48;

                    for (int ad = 0; ad < 10; ad++) {

                        write_internal_eeprom((address), arr[ad]);

                        address++;
                        if (address == 100) {
                            o_flag = 1;
                            address = 0;
                        }
                    }
                }

                if (del++ == 20000) {
                    address = 0;
                    o_flag = 0;
                    scv = 0;
                    mode = 3;
                    del = 0;
                    once = 1;
                }




                break;
            }
            case 6:
            {
                init_uart();

                clcd_print("   DOWNLOAD LOG    ", LINE1(0));
                clcd_print("    SUCCESSFULL    ", LINE2(0));
                int s;
                //            if(once1)
                {
                    //                once1--;



                }

                if (del1++ == 1000) {
                    puts("\n\r");
                    arr[0] = time[0];
                    arr[1] = time[1];
                    arr[2] = time[3];
                    arr[3] = time[4];
                    arr[4] = time[6];
                    arr[5] = time[7];

                    arr[6] = 'D';
                    arr[7] = 'L';

                    arr[8] = (adc_reg_val / 10) + 48;
                    arr[9] = (adc_reg_val % 10) + 48;

                    for (int ad = 0; ad < 10; ad++) {

                        write_internal_eeprom((address), arr[ad]);

                        address++;
                        if (address == 100) {
                            o_flag = 1;
                            address = 0;
                        }
                    }

                    if (o_flag == 0) {
                        s = 0;
                        cf = address / 10;
                    } else {
                        s = address / 10;
                        cf = 10;
                    }


                    puts("S.NO   TIME      EV     SP\n\r");
                    for (d = 0; d < cf; d++) {
                        char eevent1 = read_internal_eeprom(0 + ((d + s) % 10 * 10));
                        char eevent2 = read_internal_eeprom(1 + ((d + s) % 10 * 10));
                        char eevent3 = read_internal_eeprom(2 + ((d + s) % 10 * 10));
                        char eevent4 = read_internal_eeprom(3 + ((d + s) % 10 * 10));
                        char eevent5 = read_internal_eeprom(4 + ((d + s) % 10 * 10));
                        char eevent6 = read_internal_eeprom(5 + ((d + s) % 10 * 10));
                        char eevent7 = read_internal_eeprom(6 + ((d + s) % 10 * 10));
                        char eevent8 = read_internal_eeprom(7 + ((d + s) % 10 * 10));
                        char eevent9 = read_internal_eeprom(8 + ((d + s) % 10 * 10));
                        char eevent10 = read_internal_eeprom(9 + ((d + s) % 10 * 10));
                        putch(' ');
                        putch(d + 48);
                        putch(' ');
                        putch(' ');
                        putch(' ');
                        putch(' ');
                        putch(eevent1);
                        putch(eevent2);
                        putch(':');
                        putch(eevent3);
                        putch(eevent4);
                        putch(':');
                        putch(eevent5);
                        putch(eevent6);
                        putch(' ');
                        putch(' ');
                        putch(' ');
                        putch(' ');
                        putch(eevent7);
                        putch(eevent8);
                        putch(' ');
                        putch(' ');
                        putch(' ');
                        putch(' ');
                        putch(eevent9);
                        putch(eevent10);

                        puts("\n\r");



                    }
                    mode = 3;
                    del1 = 0;
                    //once1=1;

                }
                break;


            }
            case 7:
            {
                clcd_print("TIME (HH:MM:SS)", LINE1(0));
                clcd_putch(' ', LINE2(0));
                 int hour;
                int minutes;
                int seconds;
                int h;
                int h1;
                int m;
                int m1;
                int s;
                int s1;
                
                if(settime)
                {
                 hour = read_ds1307(HOUR_ADDR);
                 h = ((hour >> 4) & 0x03);
		         h1 =  (hour & 0x0F);
                 minutes=read_ds1307(MIN_ADDR);
                 m = ((minutes >> 4) & 0x0F);
	             m1 =  (minutes & 0x0F);
                 seconds = read_ds1307(SEC_ADDR);
                 s =  ((seconds >> 4) & 0x0F);
	             s1 =  (seconds & 0x0F);
                 settime=0;
                  hour12=(h*10+h1);
                 minutes12=(m*10+m1);
                 seconds12=(s*10+s1);
                }
                
//               

                
                if(delay1234++ <400)
                {
                     clcd_putch( '0'+(hour12%10), LINE2(2));
                clcd_putch('0'+ (hour12/10), LINE2(1));
                
                clcd_putch(':', LINE2(3));
                 clcd_putch('0'+(minutes12%10), LINE2(5));
                clcd_putch('0'+(minutes12/10), LINE2(4));
                clcd_putch(':', LINE2(6));
                 clcd_putch('0'+(seconds12%10), LINE2(8));
                clcd_putch('0'+(seconds12/10), LINE2(7));
                }
                else if(delay1234 > 400 && delay1234 < 800)
                {
                    if(x2==1)
                    {
                        clcd_putch( ' ', LINE2(2));
                         clcd_putch(' ', LINE2(1));
                    }
                    else if(x2==2)
                    {
                         clcd_putch(' ', LINE2(5));
                         clcd_putch(' ', LINE2(4));
                        
                    }
                    else if(x2==0)
                    {
                        clcd_putch(' ', LINE2(8));
                       clcd_putch(' ', LINE2(7));
                    }
                    
                }
                else if(delay1234 == 800)
                {
                    delay1234=0;
                }
                
                
//              
                
                
                unsigned int key3 = read_switches(LEVEL_CHANGE);
                if(key3!=255)
                {
                    time23++;
                    bsw=key3;
                    if(time23==1000)
                    {
                         
                        time23=0;
                       if(key3==5)
                       {
                           
                           write_ds1307(HOUR_ADDR,((hour12/10)<<4)| hour12%10);
                           write_ds1307(MIN_ADDR,(( minutes12/10)<<4)|  minutes12%10);
                           write_ds1307(SEC_ADDR,((seconds12/10)<<4)|  seconds12%10);
                           
                           
                            settime=1;
                             CLEAR_DISP_SCREEN;
                             clcd_print("  SET TIME      ", LINE1(0));
                            for(int i = 5000;i--;)
                                clcd_print("   SUCCESSFULL  ", LINE2(0)); 

                          
                                
                                 mode=3;
                                 setf=1;
                            
                           
                       }
                       else if(key3==6)
                       {
                          settime=1;
                           mode=3;
                           
                       }
                        
                           
                    }
                }
                   
                else if (time23 < 1000 && time23 > 0) {
                        time23 = 0;

                        if (bsw == 5)
                        {
                            x2=(x2+1)%3;
                         } 
                        else if (bsw == 6)
                        {
                            if(x2==1)
                            {
                                hour12++;
                                hour12%=24;
                            }
                            else if(x2==2)
                            {
                               minutes12++;
                               minutes12%=60;
                            }
                            else if(x2==0)
                            {
                                seconds12++;
                                seconds12%= 60;
                            }
                            
                            

                        }


                }
                    
                
                
                
                
                //----
                
                
               
                
                
                break;
            }
            
           
            case 8:
            {


                if (p1 < 4) {

                    clcd_print("entr new password", LINE1(0));
                    clcd_putch(' ', LINE2(0));
                    if (key == 5) {
                        enter_password1[p1] = '1';

                        clcd_putch('*', LINE2(p1 + 4));
                        p1++;

                    } else if (key == 6) {
                        enter_password1[p1] = '0';

                        clcd_putch('*', LINE2(p1 + 4));
                        p1++;
                    }
                    if(p1==4)
                    {
                        CLEAR_DISP_SCREEN;
                    }
                }
                else if(p2 < 4)
                    {

                        clcd_print("re_entr new password", LINE1(0));
                        clcd_putch(' ', LINE2(0));
                        if (key == 5) {
                            enter_password2[p2] = '1';

                            clcd_putch('*', LINE2(p2 + 4));
                            p2++;

                        } else if (key == 6) {
                            enter_password2[p2] = '0';

                            clcd_putch('*', LINE2(p2 + 4));
                            p2++;
                        }
                        
                        if(p2==4)
                    {
                        CLEAR_DISP_SCREEN;
                    }


                } 
                else 
                {
                    if (strcmp(enter_password1, enter_password2) == 0)
                    {
                        clcd_print("PASSWORD CHANGE ", LINE1(0));
                        clcd_print("  SUCCESSFULL   ", LINE2(0));


                        write_ext_eeprom(200,enter_password2[0] );
                         write_ext_eeprom(201,enter_password2[1] );
                          write_ext_eeprom(202,enter_password2[2] );
                           write_ext_eeprom(203,enter_password2[3] );
                        
                    }
                     else 
                    {
                        clcd_print("PASSWORD CHANGE ", LINE1(0));
                        clcd_print("  FAILURE   ", LINE2(0));
                        
                    }
                    
                    if(del23++==300)
                    {
                        del23=0;
                        p1=0;
                        p2=0;
                        mode=3;
                    }
                    
                    
                }


                break;
            }






        }
    }

}
