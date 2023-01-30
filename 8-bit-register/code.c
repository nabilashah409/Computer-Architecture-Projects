#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

char getColour(char reg);
void setColor();
int main()
{
        int sel = 0;
        char word[1024];
        unsigned char reg = 0;
        while (1)
        {
                printf
                ("\n1) Author info\n2) check status\n3) clear status\n4) save status\n5) load status\n6) set LED color\n7) clear LED color\n8) toggle reset\n9) turn on\n10) turn off\n0) Exit\n");
                scanf("%d", &sel);
                switch (sel)
                {
                case 1:
                        printf("\nNabila Shah Lnu \n2840468");
                        continue;
                case 2:
                        if (reg & (1 << 1)){
                            if(((reg & (1 << 6)) == 0) & ((reg & (1 << 5)) > 0)){
                                printf("Color: Red");
                            }
                            else if(((reg & (1 << 6)) > 1) & ((reg & (1 << 5)) == 0)){
                                printf("Color: Green");
                            }
                            else if (((reg & (1 << 6)) > 0) & ((reg & (1 << 5)) > 0)){
                                printf("Color: Blue");
                            }
                            else{
                                printf("Color: None");
                            }

                            printf("\nReset pin: %s",((reg >> 7) == 0) ? "Low" : "High");
                            printf("\nPower: ON");
                        }
                        else
                            printf("OFF");
                        continue;
                case 3:
                        reg = 0;
                        continue;
                case 4:
                        printf("Enter file name\n");
                        scanf("%s", word);
                        sel = open(word, O_CREAT | O_WRONLY, 0600 );
                        write(sel, &reg, 1);
                        close(sel);
                        continue;

                case 5:
                        printf("Enter file name\n");
                        scanf("%s", word);
                        sel = open(word, O_RDONLY);
                        read(sel, &reg, 1);
                        close(sel);
                        continue;

                case 6:
                        printf("Enter selected color");
                        scanf("%s", word);
                        if((strcasecmp(word, "Red") != 0) & (strcasecmp(word, "Green") != 0) & (strcasecmp(word, "Blue") != 0)){
                            printf("Invalid color");
                        }
                        else if(strcasecmp(word, "Red") == 0){
                            reg = reg & ~(3 << 5) | (1 << 5);
                        }
                        else if(strcasecmp(word, "Green") == 0){
                            reg = reg & ~(3 << 5) | (1 << 6);
                        }
                        else {
                            reg = reg | (3 << 5);
                        }
                        continue;

                case 7:
                        reg = reg & ~(3 << 5);
                        continue;
                case 8:
                        reg = reg ^ (1 << 7);
                        continue;
                case 9:
                        reg = reg | (1 << 1);
                        continue;
                case 10:
                        reg = reg & ~(1 << 1);
                        continue;
                default:
                        return 0;
                        //for(int i =0 ; i<8; i++){
                          //  printf("%d",reg >> i & 1);  
                        //}
                        //continue;


                }
        }

    return 0;
}

