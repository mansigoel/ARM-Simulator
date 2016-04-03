
/* 

The project is developed as part of Computer Architecture class
Project Name: Functional Simulator for subset of ARM Processor

Developer's Name: Mansi Goel, Taneea S Agrawaal
Developer's Email id: mansi14062@iiitd.ac.in, taneea14166@iiitd.ac.in
Date: 31.03.2015

*/


/* myARMSim.cpp
   Purpose of this file: implementation file for myARMSim
*/

#include "myARMSim.h"
#include <stdlib.h>
#include <stdio.h>


//Register file
static unsigned int R[16];
//flags
static int N,C,V,Z;
//memory
static unsigned char MEM[4000];

//intermediate datapath and control path signals
static unsigned int inst;
static unsigned int operand1;
static unsigned int operand2;
static unsigned int destination;
static unsigned int PC;
static int offset;
static unsigned int address = 0x0;
static unsigned int flag;
static unsigned int opcode;
static unsigned int result;
static unsigned int cond;
static unsigned int immediate;
unsigned int *to_be;
static unsigned int R0[1024],R1[1024],R2[1024],R3[1024],R4[1024],R5[1024],R6[1024],R7[1024],R8[1024],R9[1024],R10[1024],R11[1024],R12[1024],R13[1024],R14[1024],R15[1024];

void run_armsim() {
  int i;
  /*for(i=0; i < 4000; i = i+4){
    printf("%x %x\n", i, read_word(MEM, i));
  }*/
  while(1) {
    fetch();
    decode();
    execute();
    mem();
    write_back();
  }
}

// it is used to set the reset values
//reset all registers and memory content to 0
void reset_proc() {
  int i;
  for(i=0;i<16;i++)
    R[i] = 0;
  for(i=0;i<4000;i++)
    MEM[i] = '0';
  PC =0;
  operand1=destination=offset=address=flag=opcode=result=immediate=cond =0; operand2 = 0;
  N = C = V = Z = 0;
  //*(to_be) = 0;
}

//load_program_memory reads the input memory, and pupulates the instruction 
// memory
void load_program_memory(char *file_name) {
  FILE *fp;
  unsigned int instruction;
  fp = fopen(file_name, "r");
  if(fp == NULL) {
    printf("Error opening input mem file\n");
    exit(1);
  }
  while(fscanf(fp, "%x %x", &address, &instruction) != EOF) {
    write_word(MEM, address, instruction);

  }
  fclose(fp);
}

//writes the data memory in "data_out.mem" file
void write_data_memory() {
  FILE *fp;
  unsigned int i;
  fp = fopen("data_out.mem", "w");
  if(fp == NULL) {
    printf("Error opening dataout.mem file for writing\n");
    return;
  }
  
  for(i=0; i < 4000; i = i+4){
    fprintf(fp, "%x %x\n", i, read_word(MEM, i));
  }
  fclose(fp);
}

//should be called when instruction is swi_exit
void swi_exit() {
  write_data_memory();
  exit(0);
}
 

//reads from the instruction memory and updates the instruction register

void fetch() {
  /*int *data;
  data = (int*)(mem+address);
  inst = (*data);
  
  address+=4;*/
  inst = read_word(MEM,R[15]);
  printf("Fetch instruction 0x%x from address 0x%x\n",inst,R[15]);
  R[15] +=4;
  

}
      //reads the instruction register, reads operand1, operand2 fromo register file, decides the operation to be performed in execute stage
  
void decode() {
   flag = ((inst>>26)&(0x3));
   opcode;
   cond = ((inst>>28)&(0xF));
  if(flag==0)
  {
      opcode = ((inst>>21)&(0xF));
      immediate = ((inst>>25)&(0x1));
      operand1 = ((inst>>16)&(0xF));
      destination = ((inst>>12)&(0xF));
      if(immediate==0){
        operand2 = ((inst)&(0xF));
        if(opcode==0){
          printf("DECODE: Operation is AND, First Operand is R%d, Second Operand is R%d, Destination Register is R%d.\nRead Registers: R%d = %d, R%d = %d\n",operand1,operand2,destination,operand1,R[operand1],operand2,R[operand2]);
        }
        else if(opcode==1) printf("DECODE: Operation is XOR, First Operand is R%d, Second Operand is R%d, Destination Register is R%d.\nRead Registers: R%d = %d, R%d = %d\n",operand1,operand2,destination,operand1,R[operand1],operand2,R[operand2]);
        else if(opcode==2) printf("DECODE: Operation is SUB, First Operand is R%d, Second Operand is R%d, Destination Register is R%d.\nRead Registers: R%d = %d, R%d = %d\n",operand1,operand2,destination,operand1,R[operand1],operand2,R[operand2]);
        else if(opcode==4) printf("DECODE: Operation is ADD, First Operand is R%d, Second Operand is R%d, Destination Register is R%d.\nRead Registers: R%d = %d, R%d = %d\n",operand1,operand2,destination,operand1,R[operand1],operand2,R[operand2]);
        else if(opcode==5) printf("DECODE: Operation is ADC, First Operand is R%d, Second Operand is R%d, Destination Register is R%d.\nRead Registers: R%d = %d, R%d = %d\n",operand1,operand2,destination,operand1,R[operand1],operand2,R[operand2]);
        else if(opcode==10) printf("DECODE: Operation is CMP, First Operand is R%d, Second Operand is R%d, Destination Register is R%d.\nRead Registers: R%d = %d, R%d = %d\n",operand1,operand2,destination,operand1,R[operand1],operand2,R[operand2]);
        else if(opcode==12) printf("DECODE: Operation is ORR, First Operand is R%d, Second Operand is R%d, Destination Register is R%d.\nRead Registers: R%d = %d, R%d = %d\n",operand1,operand2,destination,operand1,R[operand1],operand2,R[operand2]);
        else if(opcode==13) printf("DECODE: Operation is MOV, First Operand is R%d, Second Operand is R%d, Destination Register is R%d.\nRead Registers: R%d = %d, R%d = %d\n",operand1,operand2,destination,operand1,R[operand1],operand2,R[operand2]);
        else if(opcode==15) printf("DECODE: Operation is MNV, First Operand is R%d, Second Operand is R%d, Destination Register is R%d.\nRead Registers: R%d = %d, R%d = %d\n",operand1,operand2,destination,operand1,R[operand1],operand2,R[operand2]);

      }

      else if(immediate==1){
        operand2 = ((inst)&(0xFF));
        if(opcode==0){
          printf("DECODE: Operation is AND, First Operand is R%d, immediate Second Operand is %d, Destination Register is R%d.\nRead Registers: R%d = %d\n",operand1,operand2,destination,operand1,R[operand1]);
        }
        else if(opcode==1) printf("DECODE: Operation is XOR, First Operand is R%d, immediate Second Operand is %d, Destination Register is R%d.\nRead Registers: R%d = %d\n",operand1,operand2,destination,operand1,R[operand1]);
        else if(opcode==2) printf("DECODE: Operation is SUB, First Operand is R%d, immediate Second Operand is %d, Destination Register is R%d.\nRead Registers: R%d = %d\n",operand1,operand2,destination,operand1,R[operand1]);
        else if(opcode==4) printf("DECODE: Operation is ADD, First Operand is R%d, immediate Second Operand is %d, Destination Register is R%d.\nRead Registers: R%d = %d\n",operand1,operand2,destination,operand1,R[operand1]);
        else if(opcode==5) printf("DECODE: Operation is ADC, First Operand is R%d, immediate Second Operand is %d, Destination Register is R%d.\nRead Registers: R%d = %d\n",operand1,operand2,destination,operand1,R[operand1]);
        else if(opcode==10) printf("DECODE: Operation is CMP, First Operand is R%d, immediate Second Operand is %d, Destination Register is R%d.\nRead Registers: R%d = %d\n",operand1,operand2,destination,operand1,R[operand1]);
        else if(opcode==12) printf("DECODE: Operation is ORR, First Operand is R%d, immediate Second Operand is %d, Destination Register is R%d.\nRead Registers: R%d = %d\n",operand1,operand2,destination,operand1,R[operand1]);
        else if(opcode==13) printf("DECODE: Operation is MOV, First Operand is R%d, immediate Second Operand is %d, Destination Register is R%d.\nRead Registers: R%d = %d\n",operand1,operand2,destination,operand1,R[operand1]);
        else if(opcode==15) printf("DECODE: Operation is MNV, First Operand is R%d, immediate Second Operand is %d, Destination Register is R%d.\nRead Registers: R%d = %d\n",operand1,operand2,destination,operand1,R[operand1]);

      }
  }
  else if(flag==1)
  {
      opcode = ((inst>>20)&(0x3F));
      if(opcode==25)
      {
        operand1 = ((inst>>16)&(0xF));
        operand2 = ((inst)&(0xFFF));
        destination = ((inst>>12)&(0xF));
        printf("DECODE: Operation is LDR, Base Register is R%d, Offset is %d, Destination Register is R%d.\n",operand1,operand2,destination);
      }
      else if(opcode==24)
      {
        operand1 = ((inst>>16)&(0xF));
        operand2 = ((inst)&(0xFFF));
        destination = ((inst>>12)&(0xF));
        printf("DECODE: Operation is STR, Base Register is R%d, Offset is %d, Register to be stored in memory is R%d.\n,Read Register: R%d = %d\n",operand1,operand2,destination,destination,R[destination]);
      }
  }
  else if(flag==2)
  {
    opcode = ((inst>>24)&(0x3));
    offset = (inst&(0xFFFFFF));
    if(opcode==2){
      if(cond==0){
        printf("DECODE: Operation is BEQ\n");
      }
      else if(cond==1) printf("DECODE: Operation is BNE\n");
      else if(cond==11) printf("DECODE: Operation is BLT\n");
      else if(cond==13) printf("DECODE: Operation is BLE\n");
      else if(cond==12) printf("DECODE: Operation is BGT\n");
      else if(cond==10) printf("DECODE; Operation is BGE\n");
      else if(cond==14) printf("DECODE: Operation is BAL\n");
    }
  }

}
//executes the ALU operation based on ALUop
void execute() {
  result=0,to_be=0;
  if(flag == 0)
  {   
      if(immediate == 0)
      {
        if(opcode == 0)
        {
          printf("EXECUTE: AND %d and %d\n",R[operand1],R[operand2]);
          result = (R[operand1])&(R[operand2]); 
        }
        else if(opcode == 1)
        {
          printf("EXECUTE: XOR %d and %d\n",R[operand1],R[operand2]);
          result = (R[operand1]^R[operand2]);
        }
        else if(opcode == 2)
        {
          printf("EXECUTE: SUB %d and %d\n",R[operand1],R[operand2]);
          result = R[operand1] - R[operand2];
        }
        else if(opcode == 4)
        {
          printf("EXECUTE: ADD %d and %d\n",R[operand1],R[operand2]);
          result = R[operand1] + R[operand2];
        }
        else if(opcode == 5)
        {
          printf("EXECUTE: ADC %d and %d\n",R[operand1],R[operand2]);
          result = R[operand1] + R[operand2];
          //C=1;
        }
        else if(opcode == 10)
        {
          printf("EXECUTE: CMP %d and %d\n",R[operand1],R[operand2]);
          if(R[operand1] == R[operand2]){
            result = 0;
            Z=1;
          }
          else if(R[operand1] < R[operand2]){
            result = -1;
            N=1;
          }
          else{
            result = 1;
          }
        }
        else if(opcode == 12)
        {
          printf("EXECUTE: ORR %d and %d\n",R[operand1],R[operand2]);
          result = (R[operand1])|(R[operand2]);
        }
        else if(opcode == 13)
        {
          printf("EXECUTE: MOV value of R%d in R%d\n,",operand2,destination);
          result = R[operand2];
        }
        else if(opcode == 15)
        {
          printf("EXECUTE: MNV NOT of this R%d in R%d\n",operand2,destination);
          result = ~(R[operand2]);
        }
      }
      else if(immediate == 1)
      {
        if(opcode == 0)
        {
          printf("EXECUTE: AND %d and %d\n",R[operand1],operand2);
          result = (R[operand1])&(operand2); 
        }
        else if(opcode == 1)
        {
          printf("EXECUTE: XOR %d and %d\n",R[operand1],operand2);
          result = ((R[operand1])^(operand2));
        }
        else if(opcode == 2){        
          printf("EXECUTE: SUB %d and %d\n",R[operand1],operand2);
          result = R[operand1] - operand2;
        }
        else if(opcode == 4)
        {
          printf("EXECUTE: ADD %d and %d\n",R[operand1],operand2);
          result = R[operand1] + operand2;
        }
        else if(opcode == 5)
        {
          printf("EXECUTE: ADC %d and %d\n",R[operand1],operand2);
          result = R[operand1] + operand2;
          //C=1;
        }
        else if(opcode == 10)
        {
          printf("EXECUTE: CMP %d and %d\n",R[operand1],operand2);
          if(R[operand1] == operand2){
            result = 0;
            Z=1;
          }
          else if(R[operand1] < operand2){
            result = -1;
            N=1;
          }
          else{
            result = 1;
          }
        }
        else if(opcode == 12)
        {
          printf("EXECUTE: ORR %d and %d\n",R[operand1],operand2);
          result = ((R[operand1])|(operand2));
        }
        else if(opcode == 13)
        {
          printf("EXECUTE: MOV %d in R%d\n",operand2,destination);
          result = operand2;
        }
        else if(opcode == 15)
        {
          printf("EXECUTE: MNV NOT of %d in R%d\n",operand2,destination);
          result = ~(operand2);
        }
      }
    }
    else if(flag == 1)
    { 
      if(opcode == 25)
      {
        int k;
        k = (operand2/4);
        if(operand1 == 0){ result = R0[k]; printf("EXECUTE: Put in R%d,  R%d's element number %d\n",destination,operand1,k+1);}
        else if(operand1 == 1){ result = R1[k]; printf("EXECUTE: Put in R%d,  R%d's element number %d\n",destination,operand1,k+1);}
        else if(operand1 == 2){ result = R2[k]; printf("EXECUTE: Put in R%d,  R%d's element number %d\n",destination,operand1,k+1);}
        else if(operand1 == 3){ result = R3[k]; printf("EXECUTE: Put in R%d,  R%d's element number %d\n",destination,operand1,k+1);}
        else if(operand1 == 4){ result = R4[k]; printf("EXECUTE: Put in R%d,  R%d's element number %d\n",destination,operand1,k+1);}
        else if(operand1 == 5){ result = R5[k]; printf("EXECUTE: Put in R%d,  R%d's element number %d\n",destination,operand1,k+1);}
        else if(operand1 == 6){ result = R6[k]; printf("EXECUTE: Put in R%d,  R%d's element number %d\n",destination,operand1,k+1);}
        else if(operand1 == 7){ result = R7[k]; printf("EXECUTE: Put in R%d,  R%d's element number %d\n",destination,operand1,k+1);}
        else if(operand1 == 8){ result = R8[k]; printf("EXECUTE: Put in R%d,  R%d's element number %d\n",destination,operand1,k+1);}
        else if(operand1 == 9){ result = R9[k]; printf("EXECUTE: Put in R%d,  R%d's element number %d\n",destination,operand1,k+1);}
        else if(operand1 == 10){ result = R10[k]; printf("EXECUTE: Put in R%d,  R%d's element number %d\n",destination,operand1,k+1);}
        else if(operand1 == 11){ result = R11[k]; printf("EXECUTE: Put in R%d,  R%d's element number %d\n",destination,operand1,k+1);}
        else if(operand1 == 12){ result = R12[k]; printf("EXECUTE: Put in R%d,  R%d's element number %d\n",destination,operand1,k+1);}
        else if(operand1 == 13){ result = R13[k]; printf("EXECUTE: Put in R%d,  R%d's element number %d\n",destination,operand1,k+1);}
        else if(operand1 == 14){ result = R14[k]; printf("EXECUTE: Put in R%d,  R%d's element number %d\n",destination,operand1,k+1);}
        else if(operand1 == 15){ result = R15[k]; printf("EXECUTE: Put in R%d,  R%d's element number %d\n",destination,operand1,k+1);}
      }
      else if(opcode == 24)
      {
        int k;
        k = (operand2)/4;
        if(operand1 == 0){ to_be = &R0[k]; printf("EXECUTE: Put R%d's value in element number %d in R%d\n",operand1,k+1,destination);}
        else if(operand1 == 1){ to_be = &R1[k]; printf("EXECUTE: Put R%d's value in element number %d in R%d\n",operand1,k+1,destination);}
        else if(operand1 == 2){ to_be = &R2[k]; printf("EXECUTE: Put R%d's value in element number %d in R%d\n",operand1,k+1,destination);}
        else if(operand1 == 3){ to_be = &R3[k]; printf("EXECUTE: Put R%d's value in element number %d in R%d\n",operand1,k+1,destination);}
        else if(operand1 == 4){ to_be = &R4[k]; printf("EXECUTE: Put R%d's value in element number %d in R%d\n",operand1,k+1,destination);}
        else if(operand1 == 5){ to_be = &R5[k]; printf("EXECUTE: Put R%d's value in element number %d in R%d\n",operand1,k+1,destination);}
        else if(operand1 == 6){ to_be = &R6[k]; printf("EXECUTE: Put R%d's value in element number %d in R%d\n",operand1,k+1,destination);}
        else if(operand1 == 7){ to_be = &R7[k]; printf("EXECUTE: Put R%d's value in element number %d in R%d\n",operand1,k+1,destination);}
        else if(operand1 == 8){ to_be = &R8[k]; printf("EXECUTE: Put R%d's value in element number %d in R%d\n",operand1,k+1,destination);}
        else if(operand1 == 9){ to_be = &R9[k]; printf("EXECUTE: Put R%d's value in element number %d in R%d\n",operand1,k+1,destination);}
        else if(operand1 == 10){ to_be = &R10[k]; printf("EXECUTE: Put R%d's value in element number %d in R%d\n",operand1,k+1,destination);}
        else if(operand1 == 11){ to_be = &R11[k]; printf("EXECUTE: Put R%d's value in element number %d in R%d\n",operand1,k+1,destination);}
        else if(operand1 == 12){ to_be = &R12[k]; printf("EXECUTE: Put R%d's value in element number %d in R%d\n",operand1,k+1,destination);}
        else if(operand1 == 13){ to_be = &R13[k]; printf("EXECUTE: Put R%d's value in element number %d in R%d\n",operand1,k+1,destination);}
        else if(operand1 == 14){ to_be = &R14[k]; printf("EXECUTE: Put R%d's value in element number %d in R%d\n",operand1,k+1,destination);}
        else if(operand1 == 15){ to_be = &R15[k]; printf("EXECUTE: Put R%d's value in element number %d in R%d\n",operand1,k+1,destination);}
      }
    }
    else if(flag == 2)
   {
      if(opcode == 2)
      {
        signed int sig;
        int bit = ((offset>>23)&&1);
        if(bit==1)
          sig = ((0xFF000000)|(offset*4));
        else
          sig = offset*4;
        if(cond== 0)
        {
          printf("EXECUTE: BEQ offset is: %d\n",offset);
          if(Z==1)
            R[15] = R[15] + 4 + sig;
        }
        else if(cond == 1)
        {
          printf("EXECUTE: BNE offset is: %d\n",offset);
          if(Z!=1)
            R[15] = R[15] + 4 + sig;
        }
        else if(cond == 11)
        {
          printf("EXECUTE: BLT offset is: %d\n",offset);
          if((N==1)&&(Z==0))
            R[15] = R[15] + 4 + sig;
        }
        else if(cond == 12)
        {
          printf("EXECUTE: BGT offset is: %d\n",offset);
          if((N==0)&&(Z==0))
            R[15] = R[15] + 4 + sig;
        }
        else if(cond == 13)
        {
          printf("EXECUTE: BLE offset is: %d\n",offset);
          if((N==1)||(Z==1))
            R[15] = R[15] + 4 + sig;
        }
        else if(cond == 14)
        {
          printf("EXECUTE: BAL offset is: %d\n",offset);
          R[15] = R[15] + 4 + sig;
        }
        else if(cond == 10)
        {
          printf("EXECUTE: BGE offset is: %d\n",offset);
          if((N==0)||(Z==1))
            R[15] = R[15] + 4 + sig;
        }
      }
    }
}
//perform the memory operation

void mem() {
  if(cond==14)
  {
    if(opcode==24)
    {
      if(operand1==0) printf("MEMORY: Load %d from memory\n",R0[operand2/4]);
      else if(operand1==1) printf("MEMORY: Load %d from memory\n",R1[operand2/4]);
      else if(operand1==2) printf("MEMORY: Load %d from memory\n",R2[operand2/4]);
      else if(operand1==3) printf("MEMORY: Load %d from memory\n",R3[operand2/4]);
      else if(operand1==4) printf("MEMORY: Load %d from memory\n",R4[operand2/4]);
      else if(operand1==5) printf("MEMORY: Load %d from memory\n",R5[operand2/4]);
      else if(operand1==6) printf("MEMORY: Load %d from memory\n",R6[operand2/4]);
      else if(operand1==7) printf("MEMORY: Load %d from memory\n",R7[operand2/4]);
      else if(operand1==8) printf("MEMORY: Load %d from memory\n",R8[operand2/4]);
      else if(operand1==9) printf("MEMORY: Load %d from memory\n",R9[operand2/4]);
      else if(operand1==10) printf("MEMORY: Load %d from memory\n",R10[operand2/4]);
      else if(operand1==11) printf("MEMORY: Load %d from memory\n",R11[operand2/4]);
      else if(operand1==12) printf("MEMORY: Load %d from memory\n",R12[operand2/4]);
      else if(operand1==13) printf("MEMORY: Load %d from memory\n",R13[operand2/4]);
      else if(operand1==14) printf("MEMORY: Load %d from memory\n",R14[operand2/4]);
      else if(operand1==15) printf("MEMORY: Load %d from memory\n",R15[operand2/4]);
    }
    else if(opcode==25)
    {
      if(operand1==0) printf("MEMORY: Store %d in memory\n",destination);
      else if(operand1==1) printf("MEMORY: Store %d in memory\n",destination);
      else if(operand1==2) printf("MEMORY: Store %d in memory\n",destination);
      else if(operand1==3) printf("MEMORY: Store %d in memory\n",destination);
      else if(operand1==4) printf("MEMORY: Store %d in memory\n",destination);
      else if(operand1==5) printf("MEMORY: Store %d in memory\n",destination);
      else if(operand1==6) printf("MEMORY: Store %d in memory\n",destination);
      else if(operand1==7) printf("MEMORY: Store %d in memory\n",destination);
      else if(operand1==8) printf("MEMORY: Store %d in memory\n",destination);
      else if(operand1==9) printf("MEMORY: Store %d in memory\n",destination);
      else if(operand1==10) printf("MEMORY: Store %d in memory\n",destination);
      else if(operand1==11) printf("MEMORY: Store %d in memory\n",destination);
      else if(operand1==12) printf("MEMORY: Store %d in memory\n",destination);
      else if(operand1==13) printf("MEMORY: Store %d in memory\n",destination);
      else if(operand1==14) printf("MEMORY: Store %d in memory\n",destination);
      else if(operand1==15) printf("MEMORY: Store %d in memory\n",destination);
    }
    else
      printf("MEMORY: No memory operation\n");
  }
  else
    printf("MEMORY: No memory operation\n");

}
//writes the results back to register file
void write_back() {
  if(flag==0)
  {
    if(opcode==10)
      printf("WRITEBACK: No writeback operation required\n");
    else
    {
      R[destination] = result;
      printf("WRITEBACK: write %d to R%d\n",result,destination);
    }
  }
  else if(flag==1)
  {
    if(opcode==25)
    {
      R[destination] = result;
      printf("WRITEBACK: write %d to R%d\n",result,destination);
    }
    else if(opcode==24)
    {
      *(to_be) = R[destination];
      printf("WRITEBACK: write %d to memory array\n",*(to_be));
    }
  }
  else if(flag==2)
  {
    printf("WRITEBACK: No writeback operation required\n");
  }
  else if(flag==3)
  {
    printf("EXIT:\n");
    swi_exit();
  }
  printf("\n");
}


int read_word(char *mem, unsigned int address) {
  int *data;
  data =  (int*) (mem + address);
  return *data;
}

void write_word(char *mem, unsigned int address, unsigned int data) {
  int *data_p;
  data_p = (int*) (mem + address);
  *data_p = data;
}
