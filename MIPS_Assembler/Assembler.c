#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include<stdint.h>
struct data_address
{
    char* data;
    int address;
};
struct data_address da[128];

uint32_t instruction;
uint8_t data[512];
uint32_t text[128];
uint8_t machineCode[1024];
char *reg1, *reg2, *reg3, *input, *label, *dataType, *value;
int rd, rs, rt, immediate, immediateValue, funct, shamt;
int i=0, j=0, dataValue=0;

void setValuesForRType(char* line){
    input = strtok(line, "\t");
    input = strtok(NULL, "\t");
    reg1 = strtok(input, ",");
    reg2 = strtok(NULL, ",");
    reg3 = strtok(NULL, ",");
    rd = atoi(++reg1);
    rs = atoi(++reg2);
    rt = atoi(++reg3);
    shamt=0;
}

void setValuesForRTypeWithoutRs(char* line){
    input = strtok(line, "\t");
    input = strtok(NULL, "\t");
    reg1 = strtok(input, ",");
    reg2 = strtok(NULL, ",");
    reg3 = strtok(NULL, ",");
    rd = atoi(++reg1);
    rt = atoi(++reg2);
    shamt = atoi(reg3);
    rs=0;
}

void setValuesForIType(char* line){
    input = strtok(line, "\t");
    input = strtok(NULL, "\t");
    reg1 = strtok(input, ",");
    reg2 = strtok(NULL, ",");
    reg3 = strtok(NULL, ",");
    rt = atoi(++reg1);
    rs = atoi(++reg2);
    if(strstr(reg3, "x") != NULL){
        immediate = (int)strtol(reg3, NULL, 0);
    }
    else
        immediate = atoi(reg3);
    
}

void setValuesForITypeWithoutRs(char* line){
    input = strtok(line, "\t");
    input = strtok(NULL, "\t");
    reg1 = strtok(input, ",");
    reg2 = strtok(NULL, ",");
    rt = atoi(++reg1);
    immediate = atoi(reg2);
    rs=0;
}

void setValuesForlwAndsw(char* line){
    input = strtok(line, "\t");
    input = strtok(NULL, "\t");
    reg1 = strtok(input, ","); // rt
    input = strtok(NULL, ",");  // immt($rs)
    reg2 = strtok(input, "(");
    reg3 = strtok(NULL, "(");
    rt = atoi(++reg1);
    immediate = atoi(reg2);
    rs = atoi(++reg3);
}
void setInstructionForRType(int opcode, int funct){
    instruction = instruction | opcode << 26;
    instruction = instruction | rs << 21;
    instruction = instruction | rt << 16;
    instruction = instruction | rd << 11;
    instruction = instruction | shamt << 6;
    instruction = instruction | funct;
    text[i] = instruction;
    i++;
}

void setInstructionForIType(int opcode){
    instruction = instruction | opcode << 26;
    instruction = instruction | rs << 21;
    instruction = instruction | rt << 16;
    instruction = instruction | immediate;
    text[i] = instruction;
    i++;
}
void allocateMemoryForDataType(char* dataType, int value){
    if(strstr(dataType, ".word") != NULL || strstr(dataType, ".int") != NULL){
        data[i+3] |= value;
        data[i+2] |= value >> 8;
        data[i+1] |= value >> 16;
        data[i] |= value >> 24;
        i = i + 4;
    }
    else if(strstr(dataType, ".array") != NULL){
        i = i + value;
    }
}

void createLabel(char* label){
    da[dataValue].data = label;
    da[dataValue].address = i;
    dataValue++;
}

int main()
{
    FILE *fp = fopen("multfw1.asm","r");
    char line[128];
    bool isData = false;
    bool isText = false;
    if(fp == NULL){
        perror("Error in opening file");
        return -1;
    }

    while(!feof(fp)){
        fgets(line, sizeof(line), fp);
        instruction = 0x0;
        if(strstr(line, "data") != NULL){
            isData = true;
        }
        else if(strstr(line, "text") != NULL){
            isData = false;
            isText = true;
            i=0;
        }
        else if(isData){
            
            //to check if it is a label
            if(strstr(line, ":") != NULL){
                label = strtok(line, ":");
                createLabel(label);
                dataType = strtok(NULL, "\t");
                value = strtok(NULL, "\t");
                //check for multiple values
                if(strstr(value, ",")){
                    value = strtok(value, ",");
                    while(value != NULL){
                        allocateMemoryForDataType(dataType, atoi(value));
                        value = strtok(NULL, ",");
                    }
                }
                else{
                    allocateMemoryForDataType(dataType, atoi(value));
                }
            }
            else{
                dataType = strtok(line, "\t");
                value = strtok(NULL, "\t");
                allocateMemoryForDataType(dataType, atoi(value));
            }
        }
        else if(isText){
            if(strstr(line, "addi") != NULL){
                setValuesForIType(line);
                setInstructionForIType(8);
            }
            else if(strstr(line, "add") != NULL){
                setValuesForRType(line);
                setInstructionForRType(0, 32);
            }
            else if(strstr(line, "sub") != NULL){
                setValuesForRType(line);
                setInstructionForRType(0, 34);
            }
            else if(strstr(line, "sll") != NULL){
                setValuesForRTypeWithoutRs(line);
                setInstructionForRType(0, 0);
            }
            else if(strstr(line, "srl") != NULL){
                setValuesForRTypeWithoutRs(line);
                setInstructionForRType(0, 2);
            }
            else if(strstr(line, "slt") != NULL){
                setValuesForRType(line);
                setInstructionForRType(0, 42);
            }
            else if(strstr(line, "lui") != NULL){
                setValuesForITypeWithoutRs(line);
                setInstructionForIType(15);
            }
            else if(strstr(line, "ori") != NULL){
                setValuesForIType(line);
                setInstructionForIType(13);
            }
            else if(strstr(line, "lw") != NULL){
                setValuesForlwAndsw(line);
                setInstructionForIType(35);
            }
            else if(strstr(line, "sw") != NULL){
                setValuesForlwAndsw(line);
                setInstructionForIType(43);
            }
            else if(strstr(line, "la") != NULL){
                input = strtok(line, "\t");
                input = strtok(NULL, "\t");
                reg1 = strtok(input, ",");
                reg2 = strtok(NULL, ",");
                rt = atoi(++reg1);  
                rs = 0;
                for(j=0;j<dataValue;j++){
                    if(strcmp(da[j].data,reg2) == 0){
                        immediateValue = da[j].address;
                    }
                }
                immediate = immediateValue >> 16;
                setInstructionForIType(15); //lui
                rs = rt;
                immediate = immediateValue & 0x0000FFFF;
                setInstructionForIType(13); //ori
            }
        }
    }
    fclose(fp);
    for(i = 0; i < 512; i++){
        machineCode[i] = data[i];
    }
    for(i = 512, j=0; i < 1024, j<512; i+=4, j++){
        machineCode[i+3] = machineCode[i+3] | text[j];
        machineCode[i+2] = machineCode[i+2] | text[j] >> 8;
        machineCode[i+1] = machineCode[i+1] | text[j] >> 16;
        machineCode[i] = machineCode[i] | text[j] >> 24;
    }

    FILE *fpo = fopen("output.bin", "wb" );
        fwrite(&machineCode,sizeof(machineCode),1,fpo);
}

