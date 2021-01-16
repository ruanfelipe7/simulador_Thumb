//UFC - Camous Quixadá
/*Equipe: Alan Nascimento
		  Antonio David
		  Ruan Felipe*/

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
using namespace std;

#define ISSET(x, a, b) ((x >> a) & (0xFFFF >> (16-b))) //retorna o o valor selecionado por a e b da variavel x
#define ISSET32(x, a, b) ((x >> a) & (0xFFFFFFFF >> (32-b))) //retorna o o valor selecionado por a e b da variavel x
#define SETBIT(x,y) (x |= (1<<y))  //seta o bit especificado por y
#define CLRBIT(x,y) (x &= ~(1<<y))
#define SETZF(x) (x | (1<<30))  //seta o bit 30 do valor x
#define CLRZF(x) (x & ~(1<<30)) //limpa o bit 30 do valor x
#define SETCLRNFLAG(x,y) ((y & (1<<31))?(x | (1<<31)):((x & ~(1<<31)))) //pega o bit 31 do y e armazena no bit 31 do x
#define SETCLRFLAG(x,y,f) ((y)?(x | (1<<f)):(x & ~(1<<f))) //pega o bit f e seta ou limpa a flag do cpsr

#define SHIFTLR(x)  ((x>>1) & 0x7fffffff);


#define THUMB 5
#define ZERO 30
#define OVERFLOWFLAG 28
#define CARRYFLAG 29
#define NEGATIVO 31

typedef unsigned short int usInt;

typedef struct{
	unsigned int address;
	unsigned int value;
}celula;

usInt decodeOP(usInt instruction, int begin, int size);
void executeOP(usInt instruction, usInt opcode);
void LSL(int *Ld, int *Lm, usInt immed);
void LSR(int *Ld, int *Lm, usInt immed);
void ASR(int *Ld, int *Lm, usInt immed);
void printRegisters();
void init();
void ADD1(int *Ld, int *Ln, int *Lm);
void SUB1(int *Ld, int *Ln, int *Lm);
void ADD2(int *Ld, int *Ln, usInt immed);
void SUB2(int *Ld, int *Ln, usInt immed);
void CMP(int *Ld, usInt immed);
void MOV(int *Ld, usInt immed);
void ADD3(int *Ld, usInt immed);
void SUB3(int *Ld, usInt immed);
void AND(int *Ld, int *Lm);
void EOR(int *Ld, int *Lm);
void LSL2(int *Ld, int *Lm);
void LSR2(int *Ld, int *Lm);
void ASR2(int *Ld, int *Lm);
void ADC(int *Ld, int *Lm);
void SBC(int *Ld, int *Lm);
void ROR(int *Ld, int *Lm);
void TST(int *Ln, int *Lm);
void NEG(int *Ld, int *Lm);
void CMP2(int *Ln, int *Lm);
void CMN(int *Ln, int *Lm);
void ORR(int *Ld, int *Lm);
void MUL(int *Ld, int *Lm);
void BIC(int *Ld, int *Lm);
void MVN(int *Ld, int *Lm);

void CPY(int *Ld, int *Lm);
void ADD4(int *Ld, int *Hm);
void ADD4_1(int *Ld, unsigned int *Hm);

void ADD5(int *Hd, int *Lm);
void ADD5_1(unsigned int *Hd, int *Lm);

void ADD6(int *Hd, int *Hm);
void ADD6_1(unsigned int *Hd, unsigned int *Hm);
void ADD6_2(unsigned int *Hd, int *Hm);
void ADD6_3(int *Hd, unsigned int *Hm);

void MOV2(int *Ld, int *Hm);
void MOV2_1(int *Ld, unsigned int *Hm);

void MOV3(int *Hd, int *Lm);
void MOV3_1(unsigned int *Hd, int *Lm);

void MOV4(int *Hd, int *Hm);
void MOV4_1(unsigned int *Hd,unsigned int *Hm);
void MOV4_2(unsigned int *Hd, int *Hm);
void MOV4_3(int *Hd, unsigned int *Hm);

void CMP3(int *Hm, int *Ln);
void CMP3_1(unsigned int *Hm, int *Ln);

void CMP4(int *Hn, int *Lm);
void CMP4_1(unsigned int *Hn, int *Lm);

void CMP5(int *Hn, int *Hm);
void CMP5_1(unsigned int *Hn, unsigned int *Hm);
void CMP5_2(unsigned int *Hn, int *Hm);
void CMP5_3(int *Hn, unsigned int *Hm);



void BX(int *Lm);
void BLX(int *Lm);

void LDR(int *Ld, int immed);
void STR(int *Ln, int *Lm, int *Ld);
void STRH(int *Ln, int *Lm, int *Ld);
void STRB(int *Ln, int *Lm, int *Ld);
void LDRSB(int *Ln, int *Lm, int *Ld);
void LDR2(int *Ln, int *Lm, int *Ld);//aqui
void LDRH(int *Ln, int *Lm, int *Ld);
void LDRB(int *Ln, int *Lm, int *Ld);
void LDRSH(int *Ln, int *Lm, int *Ld);
void STR2(int immed,int *Ln,int *Ld);
void LDR4(int immed, int *Ln, int *Ld);
void STRB2(int immed, int *Ln, int *Ld);
void LDRB2(int immed, int *Ln, int *Ld);
void STRH2(int immed, int *Ln, int *Ld);
void LDRH2(int immed, int *Ln, int *Ld);
void STR3(int immed, int *Ld);
void LDR3(int immed, int *Ld);//aqui
void ADD7(int immed, int *Ld);
void ADD8(int immed, int *Ld);
void ADD9(int immed);
void SUB4(int immed);
void SXTH(int  *Lm, int  *Ld);
void SXTB(int  *Lm, int  *Ld);
void UXTH(int  *Lm, int  *Ld);
void UXTB(int  *Lm, int  *Ld);
// void ADD10(int  immed); NÃO TEM
// void SUB5(int immed); NÃO TEM
void REV(int *Lm, int *Ld);
void REV16(int *Lm, int *Ld);
void REVSH(int *Lm, int *Ld);
//novas funçẽoes
int PUSH(int List, int R);
int POP(int List, int R);
void SETENDLE();
void SETENDBE();
void CPSIE(int interrupts);
void CPSID(int interrupts);
void BKPT(usInt immed);
void STMIA(int *Ln, int List);
void LDMIA(int *Ln, int List);
void B(int Offset, int cond);
void UNDEFINED();
void SWI(usInt immed);
void B2(int Offset);
void BLX2(int Offset);
void B3(int Offset);
void BL(int Offset);
void loadMemory();
celula searchMemory(unsigned int address);

vector<celula> memoria;

void addFlags(int a, int b);
void subFlags(int a, int b);



int r[13]; //Array of registers
int cpsr=0; //Variable of flags
unsigned int *sp;
unsigned int *lr;
unsigned int *pc; //Pointer of the pc


bool compareAdress(const celula& a, const celula& b){
	return a.address<b.address;
}

void insert_sorted(celula c){
	for(int i =0; i != memoria.size(); i++){
		if(memoria[i].address == c.address){
			memoria[i].value = c.value;
			return;		
		}
	}
	memoria.push_back(c);
	sort(memoria.begin(), memoria.end(), compareAdress);

}


int main(int argc, char const *argv[]){
	usInt opcode;

	// celula h;
	// h.address = 0x80000100;
	// h.value = 10;
	// insert_sorted(h);

	sp = (unsigned int *) malloc(sizeof(unsigned int));
	lr = (unsigned int *) malloc(sizeof(unsigned int));
	pc = (unsigned int *) malloc(sizeof(unsigned int));

	*sp = 0x82000000;
	*pc = 0x80000000;

	init();

	loadMemory();

	usInt codigo;

	celula cel;

	do{
		//if(*pc >= 0x80000000 && *pc < 0x80000100){


			if(*pc%4==0){
				cel = searchMemory(*pc);
				codigo = ISSET32(cel.value, 0, 16);
			}else{
				cel = searchMemory(*pc-2);
				codigo = ISSET32(cel.value, 16, 16);
			}

			//printf("%d\n", codigo);


			if(!cel.value){
				printRegisters();
				exit(1);
			}


			if(codigo == 0xe7fe){//se a instrução for b .
				printRegisters();
				exit(1);
			}

			*pc += 2;
			opcode = decodeOP(codigo, 12, 4);
			executeOP(codigo, opcode);

		/*}else{
			//printf("\033[31m DATA ABORT \033[0;0m\n");
		}*/

	}while(1);


	return 0;
}

usInt decodeOP(usInt instruction, int begin, int size){
	return ISSET(instruction, begin, size);
}
void executeOP(usInt instruction, usInt opcode){
	usInt op, Ld, Lm, Ln, Ls, immed, Hm, List, Hd, Hn, R, Offset;
	switch(opcode){

		case 0:
			op = ISSET(instruction, 11, 1);
			Ld = ISSET(instruction, 0, 3);
			Lm = ISSET(instruction, 3, 3);
			immed = ISSET(instruction, 6, 5);
			if (!op)
				LSL(&r[Ld], &r[Lm], immed);
			else
				LSR(&r[Ld], &r[Lm], immed);
			break;

		case 1:  // 0001 1100 0000 0001
			op = ISSET(instruction, 11, 1);
			Ld = ISSET(instruction, 0, 3);
			if (!op){
				Lm = ISSET(instruction, 3, 3);
				immed = ISSET(instruction, 6, 5);
				ASR(&r[Ld], &r[Lm], immed);
			}else{
				op = ISSET(instruction, 10, 1);
				Ln = ISSET(instruction, 3, 3);
				if (!op){
					op = ISSET(instruction, 9, 1);
					Lm = ISSET(instruction, 6, 3);
					if (!op)
						ADD1(&r[Ld], &r[Ln], &r[Lm]);
					else
						SUB1(&r[Ld], &r[Ln], &r[Lm]);
				}else{
					op = ISSET(instruction, 9, 1);
					immed = ISSET(instruction, 6, 3);
					if (!op)
						ADD2(&r[Ld], &r[Ln], immed);
					else
						SUB2(&r[Ld], &r[Ln], immed);
				}
			}
		break;

		case 2:
			op = ISSET(instruction, 11, 1);
			immed = ISSET(instruction, 0, 8);
			if (!op){
				Ld = ISSET(instruction, 8, 3);
				MOV(&r[Ld], immed);
			}
			else{
				Ln = ISSET(instruction, 8, 3); //Ld ou Ln
				CMP(&r[Ln], immed);
			}
		break;

		case 3:
			op = ISSET(instruction, 11, 1);
			Ld = ISSET(instruction, 8, 3); //Ld ou Ln
			immed = ISSET(instruction, 0, 8);
			if (!op)
				ADD3(&r[Ld], immed);
			else
				SUB3(&r[Ld], immed);
		break;

		case 4:
			Ld = ISSET(instruction, 0, 3);
			op = ISSET(instruction, 11, 1);
			if (op==0){
				op = ISSET(instruction, 8, 3);
				if(op==0){
					op = ISSET(instruction, 6, 2);
					Lm = ISSET(instruction, 3, 3);
					Ld = ISSET(instruction, 0, 3);
					if(op==0){
						AND(&r[Ld], &r[Lm]);
					}else if(op==1){
						EOR(&r[Ld], &r[Lm]);
					}else if(op==2){
						LSL2(&r[Ld], &r[Lm]);
					}else if(op==3){
						LSR2(&r[Ld], &r[Lm]);
					}
				}else if(op==1){
					op = ISSET(instruction, 6, 2);
					Ld = ISSET(instruction, 0, 3);
					Lm = ISSET(instruction, 3, 3);
					//Ld = ISSET(instruction, 3, 0);
					if(op==0){
						ASR2(&r[Ld], &r[Lm]);
					}else if(op==1){
						ADC(&r[Ld], &r[Lm]);
					}else if(op==2){
						SBC(&r[Ld], &r[Lm]);
					}else if(op==3){
						ROR(&r[Ld], &r[Lm]);
					}
				}else if(op==2){
					op = ISSET(instruction, 6, 2);
					Ln = ISSET(instruction, 0, 3);
					Ld = ISSET(instruction, 0, 3);
					Lm = ISSET(instruction, 3, 3);
					if(op==0){
						TST(&r[Ln], &r[Lm]);
					}else if(op==1){
						NEG(&r[Ld], &r[Lm]);
					}else if(op==2){
						CMP2(&r[Ln], &r[Lm]);
					}else if(op==3){
						CMN(&r[Ln], &r[Lm]);
					}
				}else if(op==3){
					op = ISSET(instruction, 6, 2);
					Ld = ISSET(instruction, 0, 3);
					Lm = ISSET(instruction, 3, 3);
					if(op==0){
						ORR(&r[Ld], &r[Lm]);
					}else if(op==1){
						MUL(&r[Ld], &r[Lm]);
					}else if(op==2){
						BIC(&r[Ld], &r[Lm]);
					}else if(op==3){
						MVN(&r[Ld], &r[Lm]);
					}
				}else if(op==6){
					op = ISSET(instruction, 6, 2);
					Ld = ISSET(instruction, 0, 3);
					Lm = ISSET(instruction, 3, 3);
					if(op==0){
						CPY(&r[Ld], &r[Lm]);
					}else if(op==1){
						Hm = Lm & 7;
						Hm = Hm | (1<<3);
						unsigned int *b;
						if(Hm <= 12){
							MOV2(&r[Ld], &r[Hm]);
						}else{
							if (Hm==13){
								b = sp;
							}else if(Hm==14){
								b = lr;
							}else if(Hm==15){
								b = pc;
							}
							MOV2_1(&r[Ld], b);	
						}
					}else if(op==2){
						Hd = Ld & 7;
						Hd = Hd | (1<<3);
						unsigned int *a;
						if(Hd <= 12){
							MOV3(&r[Hd], &r[Lm]);
						}else{
							if (Hd==13){
								a = sp;
							}else if(Hd==14){
								a = lr;
							}else if(Hd==15){
								a = pc;
							}
							MOV3_1(a, &r[Lm]);	
						}
					}else if(op==3){
						Hd = Ld & 7;
						Hm = Lm & 7;
						Hm = Hm | (1<<3);
						Hd = Hd | (1<<3);
						unsigned int *a;
						unsigned int *b;
						if(Hd<13 && Hm<13){
							MOV4(&r[Hd], &r[Hm]);
						}else{

							if (Hd==13){
								a = sp;
							}else if(Hd==14){
								a = lr;
							}else if(Hd==15){
								a = pc;
							}

							if (Hm==13){
								b = sp;
							}else if(Hm==14){
								b = lr;
							}else if(Hm==15){
								b = pc;
							}

							if (Hd>12 && Hm>12){
								MOV4_1(a, b);
							}else if(Hd > 12 && Hm <= 12){
								MOV4_2(a, &r[Hm]);
							}else if(Hd <= 12 && Hm > 12){
								MOV4_3(&r[Hd], b);
							}
						}

					}
				}else if(op==4){
					op = ISSET(instruction, 6, 2);
					Ld = ISSET(instruction, 0, 3);
					Lm = ISSET(instruction, 3, 3);
					if(op==1){
						Hm = Lm & 7;
						Hm = Hm | (1<<3);
						unsigned int *b;
						if(Hm <= 12){
							ADD4(&r[Ld], &r[Hm]);
						}else{
							if (Hm==13){
								b = sp;
							}else if(Hm==14){
								b = lr;
							}else if(Hm==15){
								b = pc;
							}
							ADD4_1(&r[Ld], b);
						}
					}else if(op==2){
						Hd = Ld & 7;
						Hd = Hd | (1<<3);
						unsigned int *a;
						if(Hd <= 12){
						 	ADD5(&r[Hd], &r[Lm]);
						}else{
							if (Hd==13){
								a = sp;
							}else if(Hd==14){
								a = lr;
							}else if(Hd==15){
								a = pc;
							}
							ADD5_1(a, &r[Lm]);
						}
					}else if(op==3){
						Hd = Ld & 7;
						Hm = Lm & 7;
						Hm = Hm | (1<<3);
						Hd = Hd | (1<<3);
						unsigned int *a;
						unsigned int *b;
						if(Hd<13 && Hm<13){
							ADD6(&r[Hd], &r[Hm]);
						}else{

							if (Hd==13){
								a = sp;
							}else if(Hd==14){
								a = lr;
							}else if(Hd==15){
								a = pc;
							}

							if (Hm==13){
								b = sp;
							}else if(Hm==14){
								b = lr;
							}else if(Hm==15){
								b = pc;
							}

							if (Hd>12 && Hm>12){
								ADD6_1(a, b);
							}else if(Hd > 12 && Hm <= 12){
								ADD6_2(a, &r[Hm]);
							}else if(Hd <= 12 && Hm > 12){
								ADD6_3(&r[Hd], b);
							}
						}
						
					}

				}else if(op==5){
					op = ISSET(instruction, 6, 2);
					Ln = ISSET(instruction, 0, 3);
					Lm = ISSET(instruction, 3, 3);
					if(op==1){
						Hm = Lm & 7;
						Hm = Hm | (1<<3);
						unsigned int *b;
						if(Hm <= 12){
							CMP3(&r[Hm], &r[Ln]);
						}else{
							if (Hm==13){
								b = sp;
							}else if(Hm==14){
								b = lr;
							}else if(Hm==15){
								b = pc;
							}			
							CMP3_1(b, &r[Ln]);				
						}
					}else if(op==2){
						Hn = Ln & 7;
						Hn = Hn | (1<<3);
						unsigned int *a;
						if(Hn <= 12){
							CMP4(&r[Hn], &r[Lm]);
						}else{
							if (Hn==13){
								a = sp;
							}else if(Hn==14){
								a = lr;
							}else if(Hn==15){
								a = pc;
							}
							CMP4_1(a, &r[Lm]);
						}
					}else if(op==3){
						Hn = Ln & 7;
						Hm = Lm & 7;
						Hm = Hm | (1<<3);
						Hn = Hn | (1<<3);
						unsigned int *a;
						unsigned int *b;
						if(Hn < 13 && Hm < 13){
							CMP5(&r[Hn], &r[Hm]);
						}else{
							if (Hn==13){
								a = sp;
							}else if(Hn==14){
								a = lr;
							}else if(Hn==15){
								a = pc;
							}

							if (Hm==13){
								b = sp;
							}else if(Hm==14){
								b = lr;
							}else if(Hm==15){
								b = pc;
							}

							if (Hn>12 && Hm>12){
								CMP5_1(a, b);
							}else if(Hn > 12 && Hm <= 12){
								CMP5_2(a, &r[Hm]);
							}else if(Hn <= 12 && Hm > 12){
								CMP5_3(&r[Hn], b);
							}	
						}
					}
				}else if(op==7){
					op = ISSET(instruction, 7, 1);
					Lm = ISSET(instruction, 3, 4);
					if(op == 0){
						BX(&r[Lm]);
					}else if(op==1){
						BLX(&r[Lm]);
					}
				}
			}else if(op==1){
				Ld = ISSET(instruction, 8, 3);
				immed = ISSET(instruction, 0, 8);
				LDR(&r[Ld], immed);
			}
		break;

		case 5:

		op = ISSET(instruction, 11, 1);
		Ld = ISSET(instruction, 0, 3);
		Ln = ISSET(instruction, 3, 3);
		Lm = ISSET(instruction, 6, 3);

		if(op==0){
			op = ISSET(instruction, 9, 2);
			if(op==0){
				STR(&r[Ln], &r[Lm], &r[Ld]);
			}else if(op==1){
				STRH(&r[Ln], &r[Lm], &r[Ld]);
			}else if(op==2){
				STRB(&r[Ln], &r[Lm], &r[Ld]);
			}else if(op==3){
				LDRSB(&r[Ln], &r[Lm], &r[Ld]);
			}
		}else if(op==1){
			op = ISSET(instruction, 9, 2);
			if(op==0){
				LDR2(&r[Ln], &r[Lm], &r[Ld]);
			}else if(op==1){
				LDRH(&r[Ln], &r[Lm], &r[Ld]);
			}else if(op==2){
				LDRB(&r[Ln], &r[Lm], &r[Ld]);
			}else if(op==3){
				LDRSH(&r[Ln], &r[Lm], &r[Ld]);
			}
		}
		break;

		case 6:
		op = ISSET(instruction, 11, 1);
		Ld = ISSET(instruction, 0, 3);
		Ln = ISSET(instruction, 3, 3);
		immed = ISSET(instruction, 6, 5);

		if(op==0){
			STR2(immed, &r[Ln], &r[Ld]);
		}else if(op==1){
			LDR4(immed, &r[Ln], &r[Ld]);
		}
		break;

		case 7:
		op = ISSET(instruction, 11, 1);
		Ld = ISSET(instruction, 0, 3);
		Ln = ISSET(instruction, 3, 3);
		immed = ISSET(instruction, 6, 5);

		if(op==0){
			STRB2(immed, &r[Ln], &r[Ld]);
		}else if(op==1){
			LDRB2(immed, &r[Ln], &r[Ld]);
		}
		break;

		case 8:
			op = ISSET(instruction, 11, 1);
			Ld = ISSET(instruction, 0, 3);
			Ln = ISSET(instruction, 3, 3);
			immed = ISSET(instruction, 6, 5);

			if(op==0){
				STRH2(immed, &r[Ln], &r[Ld]);
			}else if(op==1){
				LDRH2(immed, &r[Ln], &r[Ld]);
			}
			break;

		case 9:
			op = ISSET(instruction, 11, 1);
			Ld = ISSET(instruction, 8, 3);
			immed = ISSET(instruction, 0, 8);

			if(op==0){
				STR3(immed, &r[Ld]);
			}else if(op==1){
				LDR3(immed, &r[Ld]);
			}
			break;

		case 10:
			op = ISSET(instruction, 11, 1);
			Ld = ISSET(instruction, 8, 3);
			immed = ISSET(instruction, 0, 8);

			if(op==0){
				ADD7(immed, &r[Ld]);
			}else if(op==1){
				ADD8(immed, &r[Ld]);
			}
			break;

		case 11:  //B404   1011 0100 0000 0100
		op = ISSET(instruction, 11, 1);
		if(op==0){
			op = ISSET(instruction, 9, 2);
			if(op==0){
				op = ISSET(instruction, 8, 1);
				if(op==0){
					op = ISSET(instruction, 7, 1);
					immed = ISSET(instruction, 0, 7);
					if(op==0){
						ADD9(immed);
					}else if(op==1){
						SUB4(immed);
					}
				}
			}else if(op==1){
				op = ISSET(instruction, 8, 1);
				if(op==0){
					op = ISSET(instruction, 6, 2);
					Lm = ISSET(instruction, 3, 3);
					Ld = ISSET(instruction, 0, 3);
					if(op ==0){
						SXTH(&r[Lm], &r[Ld]);
					}else if(op ==1){
						SXTB(&r[Lm], &r[Ld]);
					}else if(op ==2){
						UXTH(&r[Lm], &r[Ld]);
					}else if(op ==3){
						UXTB(&r[Lm], &r[Ld]);
					}
				}
			}else if(op==2){
				List = ISSET(instruction, 0, 8);
				R = ISSET(instruction, 8, 1);
				PUSH(List, R);
			}else if(op==3){
				op = ISSET(instruction, 8, 1);
				if(op==0){
					op = ISSET(instruction, 4, 4);
					if(op==5){
						op = ISSET(instruction, 0, 4);
						if(op==0){
							SETENDLE();
						}else if(op==8){
							SETENDBE();
						}
					}else if(op==6){
						op = ISSET(instruction, 0, 3);
						CPSIE(op);
					}else if(op==7){
						op = ISSET(instruction, 0, 3);
						CPSID(op);
					}
				}
			}
		}else if(op==1){
			op = ISSET(instruction, 9, 2);
			if(op==1){
				op = ISSET(instruction, 8, 1);
				if(op==0){
					op = ISSET(instruction, 6, 2);
					Lm = ISSET(instruction, 3, 3);
					Ld = ISSET(instruction, 0, 3);
					if(op ==0){
						REV(&r[Lm], &r[Ld]);
					}else if(op ==1){
						REV16(&r[Lm], &r[Ld]);
					}else if(op ==2){
						//SEM INSTRUÇÃO
					}else if(op ==3){
						REVSH(&r[Lm], &r[Ld]);
					}
				}
			}else if(op==3){
				op = ISSET(instruction, 8, 1);
				if(op==0){
					immed = ISSET(instruction, 0, 8);
					BKPT(immed);
				}
			}else if(op==2){
				List = ISSET(instruction, 0, 8);
				R = ISSET(instruction, 8, 1);
				POP(List, R);
			}
		}
		break;

		case 12:
			op = ISSET(instruction, 11, 1);
			Ln = ISSET(instruction, 8, 3);
			List = ISSET(instruction, 0, 8);
			if(op==0){
				STMIA(&r[Ln], List);
			}else if(op==1){
				LDMIA(&r[Ln], List);
			}
		break;

		case 13:
			op = ISSET(instruction, 8, 4);
			Offset = ISSET(instruction, 0, 8);
			immed = ISSET(instruction, 0, 8);
			if(op<14){
				B(Offset, op);
			}else if(op==14){
				UNDEFINED();
			}else if(op==15){
				SWI(immed);
			}

		break;

		case 14:
			op = ISSET(instruction, 11, 1);
			if(op==0){
				Offset = ISSET(instruction, 0, 11);
				B2(Offset);
			}else if(op==1){
				Offset = ISSET(instruction, 1, 10);
				BLX2(Offset);
			}

		break;

		case 15:
			op = ISSET(instruction, 11, 1);
			Offset = ISSET(instruction, 0, 11);
			if(op==0){
				B3(Offset);
			}else if(op==1){
				BL(Offset);
			}

		break;

		default:
			UNDEFINED();
	}
}

void LSL(int *Ld, int *Lm, usInt immed){
	*Ld = *Lm << immed ;
	if(immed){
		if(ISSET32(*Lm, (32-immed), 1)){
			SETBIT(cpsr, CARRYFLAG);
		}else{
			CLRBIT(cpsr, CARRYFLAG);
		}
	}

	if(ISSET32(*Ld, 31, 1)){
		SETBIT(cpsr, NEGATIVO);
	}else{
		CLRBIT(cpsr, NEGATIVO);
	}
	if(*Ld == 0){
		SETBIT(cpsr, ZERO);
	}else{
		CLRBIT(cpsr, ZERO);
	}
}

void LSR(int *Ld, int *Lm, usInt immed){
	//printf("Chamou o LSR\n");
	int aux = *Lm;
	for (int i = 0; i < immed; ++i)
	{
		aux = SHIFTLR(aux);
	}
	*Ld = aux;
	if((immed == 0)){
		if(ISSET32(*Lm, 31, 1))
			SETBIT(cpsr, CARRYFLAG);
		else
			CLRBIT(cpsr, CARRYFLAG);
	}else if(immed > 0){
		if(ISSET32(*Lm, (immed-1), 1))
			SETBIT(cpsr, CARRYFLAG);
		else
			CLRBIT(cpsr, CARRYFLAG);
	}

	if(ISSET32(*Ld, 31, 1)){
		SETBIT(cpsr, NEGATIVO);
	}else{
		CLRBIT(cpsr, NEGATIVO);
	}
	if(*Ld == 0){
		SETBIT(cpsr, ZERO);
	}else{
		CLRBIT(cpsr, ZERO);
	}

}

void ASR(int *Ld, int *Lm, usInt immed){
	int cf;
	int nf;
	int zf;
	if (immed == 0){
		cf = ISSET32(*Lm, 31, 1);

		if (cf){
			SETBIT(cpsr, CARRYFLAG);
		}else{
			CLRBIT(cpsr, CARRYFLAG);
		}

		if (!cf){
			*Ld = 0;
		}else{
			*Ld = 0xFFFFFFFF;
		}
	}else if(immed > 0){
		int v = immed-1;
		cf = ISSET32(*Lm, v, 1);

		if (cf){
			SETBIT(cpsr, CARRYFLAG);
		}else{
			CLRBIT(cpsr, CARRYFLAG);
		}

		*Ld = *Lm >> immed;
	}

	nf = ISSET32(*Ld, 31, 1);

	if (nf){
		SETBIT(cpsr, NEGATIVO);
	}else{
		CLRBIT(cpsr, NEGATIVO);
	}

	if (*Ld==0){
		SETBIT(cpsr, ZERO);
	}else{
		CLRBIT(cpsr, ZERO);
	}
}

void ADD1(int *Ld, int *Ln, int *Lm){
	//printf("Chamou o ADD1\n");
	*Ld = *Ln + *Lm;
	addFlags(*Ln, *Lm);
	int nf = ISSET32(*Ld, 31, 1);

	if (nf){
		SETBIT(cpsr, NEGATIVO);
	}else{
		CLRBIT(cpsr, NEGATIVO);
	}

	if (*Ld==0){
		SETBIT(cpsr, ZERO);
	}else{
		CLRBIT(cpsr, ZERO);
	}
}

void SUB1(int *Ld, int *Ln, int *Lm){
	//printf("Chamou o SUB1\n");

	*Ld = *Ln - *Lm;
	subFlags(*Ln, *Lm);

	int nf = ISSET32(*Ld, 31, 1);

	if (nf){
		SETBIT(cpsr, NEGATIVO);
	}else{
		CLRBIT(cpsr, NEGATIVO);
	}

	if (*Ld==0){
		SETBIT(cpsr, ZERO);
	}else{
		CLRBIT(cpsr, ZERO);
	}
}

void ADD2(int *Ld, int *Ln, usInt immed){
	//printf("Chamou o ADD2\n");
	*Ld = *Ln + immed;
	addFlags(*Ln, immed);

	int nf = ISSET32(*Ld, 31, 1);

	if (nf){
		SETBIT(cpsr, NEGATIVO);
	}else{
		CLRBIT(cpsr, NEGATIVO);
	}

	if (*Ld==0){
		SETBIT(cpsr, ZERO);
	}else{
		CLRBIT(cpsr, ZERO);
	}
}

void SUB2(int *Ld, int *Ln, usInt immed){
	//printf("Chamou o SUB2\n");
	*Ld = *Ln - immed;
	subFlags(*Ln, immed);

	int nf = ISSET32(*Ld, 31, 1);

	if (nf){
		SETBIT(cpsr, NEGATIVO);
	}else{
		CLRBIT(cpsr, NEGATIVO);
	}

	if (*Ld==0){
		SETBIT(cpsr, ZERO);
	}else{
		CLRBIT(cpsr, ZERO);
	}
}

void MOV(int *Ld, usInt immed){
	//printf("Chamou o MOV\n");

	*Ld = immed;
	//realiza a verificação se a movimentação foi zero
	cpsr = SETCLRFLAG(cpsr, !(*Ld), ZERO);

	//realiza a verificação se a movimentação teve valor negativo
	cpsr = SETCLRNFLAG(cpsr,immed);
}

void CMP(int *Ln, usInt immed){
	//printf("Chamou o CMP\n");
	//int aux2 = (~(immed) + 1);
	int aux = *Ln - immed;

	//pega o bit 31 do aux e armazena no bit 31 do cpsr
	cpsr=SETCLRNFLAG(cpsr,aux);

	//realiza a verificação se a movimentação foi zero
	cpsr = SETCLRFLAG(cpsr, !(aux), ZERO);

	subFlags(*Ln, immed);
	//verificação do C flag

	//verificação do V flag

}

void ADD3(int *Ld, usInt immed){
	//printf("Chamou o ADD3\n");
	addFlags(*Ld, immed);
	*Ld = *Ld+immed;

	//pega o bit 31 do aux e armazena no bit 31 do cpsr
	cpsr=SETCLRNFLAG(cpsr,*Ld);

	//realiza a verificação se a movimentação foi zero
	cpsr = SETCLRFLAG(cpsr, !(*Ld), ZERO);


	//verificação do C flag

	//verificação do V flag

}

void SUB3(int *Ld, usInt immed){
	//printf("Chamou o SUB3\n");
	subFlags(*Ld, immed);
	*Ld = *Ld - immed;

	//pega o bit 31 do aux e armazena no bit 31 do cpsr
	cpsr=SETCLRNFLAG(cpsr,*Ld);

	//realiza a verificação se a movimentação foi zero
	cpsr = SETCLRFLAG(cpsr, !(*Ld), ZERO);



}

void AND(int *Ld, int *Lm){
	*Ld = *Ld & *Lm;

	//pega o bit 31 do aux e armazena no bit 31 do cpsr
	cpsr=SETCLRNFLAG(cpsr,*Ld);

	//realiza a verificação se a operação AND foi zero
	cpsr = SETCLRFLAG(cpsr, !(*Ld), ZERO);



}

void EOR(int *Ld, int *Lm){
	*Ld = *Ld ^ *Lm;

	//pega o bit 31 do aux e armazena no bit 31 do cpsr
	cpsr=SETCLRNFLAG(cpsr,*Ld);

	//realiza a verificação se a operação AND foi zero
	cpsr = SETCLRFLAG(cpsr, !(*Ld), ZERO);


	//verificação do C flag

	//verificação do V flag

}

void LSL2(int *Ld, int *Lm){
	int aux = ISSET32(*Ld, 0, 8);
	int aux2 = ISSET32(*Lm, 0, 8);
	if(aux2 < 32){
		if(ISSET32(*Ld, (32-aux), 1))
			SETBIT(cpsr, CARRYFLAG);
		else
			CLRBIT(cpsr, CARRYFLAG);
		*Ld = *Ld << aux2;

	}else if(aux2 == 32){
		if(ISSET32(*Ld, 0, 1))
			SETBIT(cpsr, CARRYFLAG);
		else
			CLRBIT(cpsr, CARRYFLAG);
		*Ld = 0;
	}else if(aux2 > 32){
		CLRBIT(cpsr, CARRYFLAG);
	}

	if(ISSET32(*Ld, 31, 1)){
		SETBIT(cpsr, NEGATIVO);
	}else{
		CLRBIT(cpsr, NEGATIVO);
	}

	if(*Ld == 0){
		SETBIT(cpsr, ZERO);
	}else{
		CLRBIT(cpsr, ZERO);
	}
}

void LSR2(int *Ld, int *Lm){

	int aux = ISSET32(*Ld, 0, 8);
	int aux2 = ISSET32(*Lm, 0, 8);

	if(aux2 < 32){
		if(ISSET32(*Ld, (aux-1), 1))
			SETBIT(cpsr, CARRYFLAG);
		else
			CLRBIT(cpsr, CARRYFLAG);
		for (int i = 0; i < *Lm; ++i)
		{
			*Ld = SHIFTLR(*Ld);
		}

	}else if(aux2 == 32){
		if(ISSET32(*Ld, 31, 1))
			SETBIT(cpsr, CARRYFLAG);
		else
			CLRBIT(cpsr, CARRYFLAG);
		*Ld = 0;
	}else if(aux2 > 32){
		CLRBIT(cpsr, CARRYFLAG);
	}


	if(ISSET32(*Ld, 31, 1)){
		SETBIT(cpsr, NEGATIVO);
	}else{
		CLRBIT(cpsr, NEGATIVO);
	}

	if(*Ld == 0){
		SETBIT(cpsr, ZERO);
	}else{
		CLRBIT(cpsr, ZERO);
	}
}

void ASR2(int *Ld, int *Lm){
	int v = ISSET32(*Lm, 0, 8);
	int cf, nf, zf;

	if (v<32 && v!=0){
		v = ISSET32(*Lm, 0, 8)-1;
		cf = ISSET32(*Ld, v, 1);

		if (cf){
			SETBIT(cpsr, 29);
		}else{
			CLRBIT(cpsr, 29);
		}
		v = ISSET32(*Lm, 0, 8);
		*Ld = *Ld >> v;
	 }else{
	 	cf = ISSET32(*Ld, 31, 1);
	 	if (cf){
			SETBIT(cpsr, 29);
		}else{
			CLRBIT(cpsr, 29);
		}

		if (cf==0){
			*Ld= 0;
		}else{
			*Ld= 0xFFFFFFFF;
		}

	 }

	nf = ISSET32(*Ld, 31, 1);
	if (nf){
		SETBIT(cpsr, 31);
	}else{
		CLRBIT(cpsr, 31);
	}

	zf = ISSET32(*Ld, 30, 1);
	if (zf==0){
		SETBIT(cpsr, 30);
	}else{
		CLRBIT(cpsr, 30);
	}

}

void ADC(int *Ld, int *Lm){
	//printf("Chamou o ADC\n");
	*Ld += *Lm + ISSET32(cpsr,29,1);
	addFlags(*Ld, *Lm);

	int nf = ISSET32(*Ld, 31, 1);
	if (nf){
		SETBIT(cpsr, 31);
	}else{
		CLRBIT(cpsr, 31);
	}

	int zf = ISSET32(*Ld, 30, 1);
	if (zf==0){
		SETBIT(cpsr, 30);
	}else{
		CLRBIT(cpsr, 30);
	}
}

void SBC(int *Ld, int *Lm){
	//printf("Chamou o SBC\n");
	*Ld -= *Lm;
	if ((ISSET32(cpsr,29,1)))
	{
		*Ld-=1;
	}
	subFlags(*Ld, *Lm);

	int nf = ISSET32(*Ld, 31, 1);
	if (nf){
		SETBIT(cpsr, 31);
	}else{
		CLRBIT(cpsr, 31);
	}

	int zf = ISSET32(*Ld, 30, 1);
	if (zf==0){
		SETBIT(cpsr, 30);
	}else{
		CLRBIT(cpsr, 30);
	}
}

void ROR(int *Ld, int *Lm){
	int aux = 0;
	for (int i = 0; i < *Lm; ++i)
	{
		aux = *Ld & 1;
		aux = aux << 31;
		LSR(Ld, Ld, 1);
		*Ld |= aux;
	}

	aux = ISSET32(*Lm, 0, 8);
	int aux2 = ISSET32(*Ld, 0, 8);
	int aux3 = ISSET32(*Lm ,0, 5);
	if(aux3 == 0){
		if(ISSET32(*Ld, 31, 1)){
			SETBIT(cpsr, CARRYFLAG);
		}else{
			CLRBIT(cpsr, CARRYFLAG);
		}
	}else if(aux3 > 0){
		if(ISSET32(*Ld, (aux3-1), 1)){
			SETBIT(cpsr, CARRYFLAG);
		}else{
			CLRBIT(cpsr, CARRYFLAG);
		}
	}

	if(ISSET32(*Ld, 31, 1)){
		SETBIT(cpsr, NEGATIVO);
	}else{
		CLRBIT(cpsr, NEGATIVO);
	}

	if(*Ld == 0){
		SETBIT(cpsr, ZERO);
	}else{
		CLRBIT(cpsr, ZERO);
	}

}

void TST(int *Ln, int *Lm){
	int aux = *Ln & *Lm;

	//pega o bit 31 do aux e armazena no bit 31 do cpsr
	cpsr=SETCLRNFLAG(cpsr,aux);

	//realiza a verificação se a movimentação foi zero
	cpsr = SETCLRFLAG(cpsr, !(aux), ZERO);


	//verificação do C flag

	//verificação do V flag
}

void NEG(int *Ld, int *Lm){
	*Ld = 0 - *Lm;
	subFlags(0, *Lm);

	if(ISSET32(*Ld, 31, 1)){
		SETBIT(cpsr, NEGATIVO);
	}else{
		CLRBIT(cpsr, NEGATIVO);
	}

	if(*Ld == 0){
		SETBIT(cpsr, ZERO);
	}else{
		CLRBIT(cpsr, ZERO);
	}
}

void CMP2(int *Ln, int *Lm){
	//printf("Chamou o CMP\n");
	int aux2 = (~(*Lm) + 1);
	int aux = *Ln + aux2;

	//pega o bit 31 do aux e armazena no bit 31 do cpsr
	cpsr=SETCLRNFLAG(cpsr,aux);

	//realiza a verificação se a movimentação foi zero
	cpsr = SETCLRFLAG(cpsr, !(aux), ZERO);

	subFlags(*Ln, *Lm);

}

void CMN(int *Ln, int *Lm){
	int aux = *Ln + *Lm;

	//pega o bit 31 do aux e armazena no bit 31 do cpsr
	cpsr=SETCLRNFLAG(cpsr,aux);

	//realiza a verificação se a movimentação foi zero
	cpsr = SETCLRFLAG(cpsr, !(aux), ZERO);

	addFlags(*Ln, *Lm);
}

void ORR(int *Ld, int *Lm){
	*Ld |= *Lm;
	//pega o bit 31 do aux e armazena no bit 31 do cpsr
	cpsr=SETCLRNFLAG(cpsr,*Ld);

	//realiza a verificação se a movimentação foi zero
	cpsr = SETCLRFLAG(cpsr, !(*Ld), ZERO);
}

void MUL(int *Ld, int *Lm){
	*Ld *= *Lm;
		//pega o bit 31 do aux e armazena no bit 31 do cpsr
	cpsr=SETCLRNFLAG(cpsr,*Ld);

	//realiza a verificação se a movimentação foi zero
	cpsr = SETCLRFLAG(cpsr, !(*Ld), ZERO);
}

void BIC(int *Ld, int *Lm){
	*Ld &= ~(*Lm);

	//pega o bit 31 do aux e armazena no bit 31 do cpsr
	cpsr=SETCLRNFLAG(cpsr,*Ld);

	//realiza a verificação se a movimentação foi zero
	cpsr = SETCLRFLAG(cpsr, !(*Ld), ZERO);
}

void MVN(int *Ld, int *Lm){
	//printf("Chamou MVN\n");
	*Ld = ~(*Lm);
	//pega o bit 31 do aux e armazena no bit 31 do cpsr
	cpsr=SETCLRNFLAG(cpsr,*Ld);

	//realiza a verificação se a movimentação foi zero
	cpsr = SETCLRFLAG(cpsr, !(*Ld), ZERO);
}

void CPY(int *Ld, int *Lm){
	*Ld = *Lm;
}
void ADD4(int *Ld, int *Hm){
	//printf("Chamou o ADD4\n");
	*Ld = *Ld + *Hm;
}
void ADD4_1(int *Ld, unsigned int *Hm){
	//printf("Chamou o ADD4_1\n");
	*Ld = *Ld + *Hm;
}
void ADD5(int *Hd, int *Lm){
	//printf("Chamou o ADD5\n");
	*Hd = *Hd + *Lm;
}
void ADD5_1(unsigned int *Hd, int *Lm){
	//printf("Chamou o ADD5_1\n");
	*Hd = *Hd + *Lm;
}
void ADD6(int *Hd, int *Hm){
	//printf("Chamou o ADD6\n");
	*Hd = *Hd + *Hm;
}
void ADD6_1(unsigned int *Hd, unsigned int *Hm){
	//printf("Chamou o ADD6_1\n");
	*Hd = *Hd + *Hm;
}
void ADD6_2(unsigned int *Hd, int *Hm){
	//printf("Chamou o ADD6_2\n");
	*Hd = *Hd + *Hm;
}
void ADD6_3(int *Hd, unsigned int *Hm){
	//printf("Chamou o ADD6_3\n");
	*Hd = *Hd + *Hm;
}
void MOV2(int *Ld, int *Hm){
	//printf("Chamou o MOV2\n");
	*Ld = *Hm;
}
void MOV2_1(int *Ld, unsigned int *Hm){
	//printf("Chamou o MOV2_1\n");
	*Ld = *Hm;
}
void MOV3(int *Hd, int *Lm){
	//printf("Chamou o MOV3\n");
	*Hd = *Lm;
}
void MOV3_1(unsigned int *Hd, int *Lm){
	//printf("Chamou o MOV3_1\n");
	*Hd = *Lm;
}
void MOV4(int *Hd, int *Hm){
	//printf("chamou Mov4\n");
	*Hd = *Hm;
}
void MOV4_1(unsigned int *Hd,unsigned int *Hm){
	//printf("chamou Mov4_1\n");
	*Hd = *Hm;
}
void MOV4_2(unsigned int *Hd, int *Hm){
	//printf("chamou Mov4_2\n");
	*Hd = *Hm;
}
void MOV4_3(int *Hd, unsigned int *Hm){
	//printf("chamou Mov4_3\n");
	*Hd = *Hm;
}
void CMP3(int *Hm, int *Ln){
	//printf("Chamou o CMP3\n");
	int aux = *Ln - *Hm;

	//pega o bit 31 do aux e armazena no bit 31 do cpsr
	cpsr=SETCLRNFLAG(cpsr,aux);

	//realiza a verificação se a movimentação foi zero
	cpsr = SETCLRFLAG(cpsr, !(aux), ZERO);

	subFlags(*Ln, *Hm);
	//verificação do C flag

	//verificação do V flag
}

void CMP3_1(unsigned int *Hm, int *Ln){
	//printf("Chamou o CMP3_1\n");
	int aux = *Ln - *Hm;

	//pega o bit 31 do aux e armazena no bit 31 do cpsr
	cpsr=SETCLRNFLAG(cpsr,aux);

	//realiza a verificação se a movimentação foi zero
	cpsr = SETCLRFLAG(cpsr, !(aux), ZERO);

	subFlags(*Ln, *Hm);
	//verificação do C flag

	//verificação do V flag
}

void CMP4(int *Hn, int *Lm){
	//printf("Chamou o CMP4\n");
	int aux = *Hn - *Lm;

	//pega o bit 31 do aux e armazena no bit 31 do cpsr
	cpsr=SETCLRNFLAG(cpsr,aux);

	//realiza a verificação se a movimentação foi zero
	cpsr = SETCLRFLAG(cpsr, !(aux), ZERO);


	subFlags(*Hn, *Lm);
	//verificação do C flag

	//verificação do V flag
}
void CMP4_1(unsigned int *Hn, int *Lm){
	//printf("Chamou o CMP4_1\n");
	int aux = *Hn - *Lm;

	//pega o bit 31 do aux e armazena no bit 31 do cpsr
	cpsr=SETCLRNFLAG(cpsr,aux);

	//realiza a verificação se a movimentação foi zero
	cpsr = SETCLRFLAG(cpsr, !(aux), ZERO);


	subFlags(*Hn, *Lm);
	//verificação do C flag

	//verificação do V flag
}
void CMP5(int *Hn, int *Hm){
	//printf("Chamou o CMP5\n");
	int aux = *Hn - *Hm;

	//pega o bit 31 do aux e armazena no bit 31 do cpsr
	cpsr=SETCLRNFLAG(cpsr,aux);
	//realiza a verificação se a movimentação foi zero
	cpsr = SETCLRFLAG(cpsr, !(aux), ZERO);

	subFlags(*Hn, *Hm);
	//verificação do C flag

	//verificação do V flag
}

void CMP5_1(unsigned int *Hn, unsigned int *Hm){
	//printf("Chamou o CMP5_1\n");
	int aux = *Hn - *Hm;

	//pega o bit 31 do aux e armazena no bit 31 do cpsr
	cpsr=SETCLRNFLAG(cpsr,aux);
	//realiza a verificação se a movimentação foi zero
	cpsr = SETCLRFLAG(cpsr, !(aux), ZERO);

	subFlags(*Hn, *Hm);
	//verificação do C flag

	//verificação do V flag
}

void CMP5_2(unsigned int *Hn, int *Hm){
	//printf("Chamou o CMP5_2\n");
	int aux = *Hn - *Hm;

	//pega o bit 31 do aux e armazena no bit 31 do cpsr
	cpsr=SETCLRNFLAG(cpsr,aux);
	//realiza a verificação se a movimentação foi zero
	cpsr = SETCLRFLAG(cpsr, !(aux), ZERO);

	subFlags(*Hn, *Hm);
	//verificação do C flag

	//verificação do V flag
}

void CMP5_3(int *Hn, unsigned int *Hm){
	//printf("Chamou o CMP5_3\n");
	int aux = *Hn - *Hm;

	//pega o bit 31 do aux e armazena no bit 31 do cpsr
	cpsr=SETCLRNFLAG(cpsr,aux);
	//realiza a verificação se a movimentação foi zero
	cpsr = SETCLRFLAG(cpsr, !(aux), ZERO);

	subFlags(*Hn, *Hm);
	//verificação do C flag

	//verificação do V flag
}

void BX(int *Lm){
	int aux = *Lm & 1;
	//cpsr = SETCLRFLAG(cpsr, aux, THUMB);
	//printf("%d CPSR\n", cpsr);
	//if(aux)
		//*Lm = *Lm - 1;
	*pc = *Lm;
}
void BLX(int *Lm){
	int aux = *Lm & 1;
	//cpsr = SETCLRFLAG(cpsr, aux, THUMB);
	//printf("%d CPSR\n", cpsr);
	if(aux)
		*Lm = *Lm - 1;
	*lr = *pc;
	*pc = *Lm;
}
void LDR(int *Ld, int immed){
	//printf("Chamou o LDR\n");
	unsigned int address = (*pc & 0xFFFFFFFC) + (immed * 4);
	celula nova = searchMemory(address);
	*Ld = nova.value;
	//printf("%x\n", nova.address);
	//printf("%d\n", *Ld);

}
void STR(int *Ln, int *Lm, int *Ld){
	//printf("Chamou o STR\n");
	unsigned int address;
	address = *Ln + *Lm;
	if(address >= 0x80000000 && address < 0x80000100){
			printf("\033[31m DATA ABORT \033[0;0m\n");
			return;
	}
	celula n;
	n.address = address;
	n.value = *Ld;
	insert_sorted(n);

}
void STRH(int *Ln, int *Lm, int *Ld){
	//printf("Chamou o STRH\n");
	unsigned int address1;
	address1 = *Ln + *Lm;
	*Ld = *Ld & 0xFFFF;
	celula n;
	n.address = address1;
	n.value = *Ld;
	if(n.address >= 0x80000000 && n.address < 0x80000100){
		printf("\033[31m DATA ABORT \033[0;0m\n");
		return;
	};

	insert_sorted(n);
}
void STRB(int *Ln, int *Lm, int *Ld){
	unsigned int address1 = *Ln + *Lm;
	*Ld = *Ld & 0xFF;
	celula n;
	n.address = address1;
	n.value = *Ld;
	insert_sorted(n);
	if(n.address >= 0x80000000 && n.address < 0x80000100){
		printf("\033[31m DATA ABORT \033[0;0m\n");
		return;
	};

}
void LDRSB(int *Ln, int *Lm, int *Ld){
		//printf("Chamou o LDRSB\n");
		unsigned int address1;
		address1 = *Ln + *Lm;
		celula k;
		if(address1 >= 0x80000000 && address1 < 0x80000100){
			printf("\033[31m DATA ABORT \033[0;0m\n");
			return;
		};
		k = searchMemory(address1);
		*Ld = (k.value & 0xFF);
		if(ISSET32(*Ld, 7, 1)){
			*Ld = *Ld | 0xFFFFFF00;
		}
}

void LDR2(int *Ln, int *Lm, int *Ld){
	//printf("Chamou o LDR2\n");
	unsigned int adressMem = *Ln + *Lm;
	if(adressMem < 0x80000000 || adressMem >= 0x80000100){
		celula aux = searchMemory(adressMem);
		*Ld = aux.value;
	}else{
		printf("\033[31m DATA ABORT \033[0;0m\n");
	}

}
void LDRH(int *Ln, int *Lm, int *Ld){
	//printf("Chamou o LDRH\n");

	unsigned int adressMem = *Ln + *Lm;
	if(adressMem < 0x80000000 || adressMem >= 0x80000100){
		celula aux = searchMemory(adressMem);
		*Ld = 0Xffff & aux.value;
	}else{
		printf("\033[31m DATA ABORT \033[0;0m\n");
	}

}
void LDRB(int *Ln, int *Lm, int *Ld){
	//printf("Chamou o LDRB\n");

	unsigned int adressMem = *Ln + *Lm;
	if(adressMem < 0x80000000 || adressMem >= 0x80000100){
		celula aux = searchMemory(adressMem);
		*Ld = 0Xff & aux.value;
	}else{
		printf("\033[31m DATA ABORT \033[0;0m\n");
	}

}
void LDRSH(int *Ln, int *Lm, int *Ld){
	//printf("Chamou o LDRSH\n");
		unsigned int address1;
		address1 = *Ln + *Lm;
		celula k;
		if(address1 >= 0x80000000 && address1 < 0x80000100){
			printf("\033[31m DATA ABORT \033[0;0m\n");
			return;
		};
		k = searchMemory(address1);
		*Ld = (k.value & 0xFFFF);
		if(ISSET32(*Ld, 15, 1)){
			*Ld = *Ld | 0xFFFF0000;
		}
}
void STR2(int immed,int *Ln,int *Ld){
	//printf("Chamou o STR2\n");
	unsigned int address;
	address = *Ln + (immed * 4);
	if(address >= 0x80000000 && address < 0x80000100){
			printf("\033[31m DATA ABORT \033[0;0m\n");
			return;
	}
	celula n;
	n.address = address;
	n.value = *Ld;
	insert_sorted(n);

}
void LDR4(int immed, int *Ln, int *Ld){
	//printf("Chamou o LDR4\n");

	unsigned int adressMem = *Ln + (immed * 4);
	if(adressMem < 0x80000000 || adressMem >= 0x80000100){
		celula aux = searchMemory(adressMem);
		*Ld = aux.value;
	}else{
		printf("\033[31m DATA ABORT \033[0;0m\n");
	}

}
void STRB2(int immed, int *Ln, int *Ld){
	//printf("Chamou o STRB2\n");
	unsigned int address;
	address = *Ln + immed;
	if(address >= 0x80000000 && address < 0x80000100){
			printf("\033[31m DATA ABORT \033[0;0m\n");
			return;
	}
	celula n;
	n.address = address;
	n.value = *Ld & 0xFF;
	insert_sorted(n);

}
void LDRB2(int immed, int *Ln, int *Ld){
	//printf("Chamou o LDRB2\n");

	unsigned int adressMem = *Ln + (immed);
	if(adressMem < 0x80000000 || adressMem >= 0x80000100){
		celula aux = searchMemory(adressMem);
		*Ld = 0xFF & aux.value;
	}else{
		printf("\033[31m DATA ABORT \033[0;0m\n");
	}

}
void STRH2(int immed, int *Ln, int *Ld){
	//printf("Chamou o STRH2\n");
	unsigned int address;
	address = *Ln + (immed * 2);
	if(address >= 0x80000000 && address < 0x80000100){
			printf("\033[31m DATA ABORT \033[0;0m\n");
			return;
	}
	celula n;
	n.address = address;
	n.value = *Ld & 0xFFFF;
	insert_sorted(n);
}
void LDRH2(int immed, int *Ln, int *Ld){
	//printf("Chamou o LDRH2\n");

	unsigned int adressMem = *Ln + (immed * 2);
	if(adressMem < 0x80000000 || adressMem >= 0x80000100){
		celula aux = searchMemory(adressMem);
		*Ld = 0xFFFF & aux.value;
	}else{
		printf("\033[31m DATA ABORT \033[0;0m\n");
	}

}
void STR3(int immed, int *Ld){
	//printf("Chamou o STR3\n");
	unsigned int address;
	address = *sp + (immed*4);
	if(address >= 0x80000000 && address < 0x80000100){
			printf("\033[31m DATA ABORT \033[0;0m\n");
			return;
	}
	celula n;
	n.address = address;
	n.value = *Ld;
	insert_sorted(n);

}
void LDR3(int immed, int *Ld){
	//printf("Chamou o LDR3\n");

	unsigned int adressMem = *sp + (immed * 4);
	if(adressMem < 0x80000000 || adressMem >= 0x80000100){
		celula aux = searchMemory(adressMem);
		*Ld = aux.value;
	}else{
		printf("\033[31m DATA ABORT \033[0;0m\n");
	}

}

//apartir desse
void ADD7(int immed, int *Ld){
	//printf("Chamou o ADD7\n");
	*Ld = (*pc & 0xFFFFFFFC) + (immed*4);
}
void ADD8(int immed, int *Ld){
	//printf("Chamou o ADD8\n");
	*Ld = *sp + (immed << 2);

}
void ADD9(int immed){
	//printf("Chamou o ADD9\n");
	*sp = *sp + (immed << 2);

}
void SUB4(int immed){
	//printf("Chamou o SUB4\n");
	*sp = *sp - (immed << 2);

}
void SXTH(int  *Lm, int  *Ld){

	//printf("Chamou o SXTH\n");

	int aux2 = ISSET(*Lm, 0, 16);

	int verificador = ISSET(*Lm, 15, 1);

	if(verificador){
		*Ld = aux2 | 0xFFFF0000;
	}else{
		*Ld = aux2 & 0x0000FFFF;
	}


}
void SXTB(int  *Lm, int  *Ld){
	//printf("Chamou o SXTB\n");

	//Atenção não afeta as flags

	int aux2 = ISSET(*Lm, 0, 8);

	int verificador = ISSET(*Lm, 7, 1);

	if(verificador){
		*Ld = aux2 | 0xFFFFFF00;
	}else{
		*Ld = aux2 & 0x000000FF;
	}

}
void UXTH(int  *Lm, int  *Ld){
	//printf("Chamou o UXTH\n");

	*Ld = *Lm & 0x0000ffff;

}
void UXTB(int  *Lm, int  *Ld){
	//printf("Chamou o UXTB\n");

	*Ld = *Lm & 0x000000ff;

}

void REV(int *Lm, int *Ld){
	//printf("Chamou o REV\n");
	//Atenção nao altera flags

	int aux2 = ISSET(*Lm, 0, 8);
	*Ld |= (aux2 << 24);

	aux2 = ISSET(*Lm, 8, 8);
	*Ld |= (aux2 << 16);

	aux2 = ISSET(*Lm, 16, 8);
	*Ld |= (aux2 << 8);

	aux2 = ISSET(*Lm, 24, 8);
	*Ld |= aux2;


}
void REV16(int *Lm, int *Ld){
	//printf("Chamou o REV16\n");

	//Atenção nao altera flags
	int aux2 = ISSET(*Lm, 0, 8);
	*Ld |=( aux2 << 8);

	aux2 = ISSET(*Lm, 8, 8);
	*Ld |= aux2;

	aux2 = ISSET(*Lm, 16, 8);
	*Ld |= (aux2 << 24);

	aux2 = ISSET(*Lm, 24, 8);
	*Ld |= (aux2 << 16);

}
void REVSH(int *Lm, int *Ld){
	//printf("Chamou o REVSH\n");

	int aux2 = ISSET(*Lm, 0, 8);
	*Ld |=( aux2 << 8);

	aux2 = ISSET(*Lm, 8, 8);
	*Ld |= aux2;

	aux2 = ISSET(*Lm, 7, 1);

	if(aux2){
		*Ld |= 0xFFFF0000;
	}else{
		*Ld &= 0x0000FFFF;
	}
}

int PUSH(int List, int R){
	//printf("Chamou o PUSH\n");
	celula cel;
	if(R){
		*sp -= 4;
		if(*sp < 0x80000100){
			printf("\033[31m DATA ABORT \033[0;0m\n");
			return -1;
		}
		cel.address = *sp;
		cel.value = *lr;
		insert_sorted(cel);
	}
	for (int i = 7; i >= 0; --i)
	{
		if (ISSET(List,i,1))
		{
			*sp -= 4;
			if(*sp < 0x80000100){
				printf("\033[31m DATA ABORT \033[0;0m\n");
				return -1;
			}
			cel.address = *sp;
			cel.value = r[i];
			insert_sorted(cel);
		}
	}
	return 0;

}

int POP(int List, int R){
	//printf("Chamou o POP\n");
	

	for (int i = 0; i < 8; ++i)
	{
		if (ISSET(List,i,1))
		{
			if(*sp > 0x82000000){
				printf("\033[31m DATA ABORT \033[0;0m\n");
				return -1;
			}
			r[i] = searchMemory(*sp).value;
			*sp += 4;
		}
	}
	
	if(R){
		if(*sp > 0x82000000){
			printf("\033[31m DATA ABORT \033[0;0m\n");
			return -1;
		}
		*pc = searchMemory(*sp).value;
		*sp += 4;
	}
	return 0;
}

void SETENDLE(){
	//printf("Chamou o SETENDLE\n");

	cpsr &= ~(0x00000200); // limpa o bit E (bit 9)

}

void SETENDBE(){
	//printf("Chamou o SETENDBE\n");
	cpsr |= 0x00000200; // set o bit E (bit 9)

}

void CPSIE(int interrupts){
	//printf("Chamou o CPSIE\n");

	int A = ISSET32(cpsr, 8, 1);
	int I = ISSET32(cpsr, 7, 1);
	int F = ISSET32(cpsr, 6, 1);
	
	if(ISSET(interrupts, 0, 1)){
		if(F){
			cpsr &= ~(0x00000040);
		}		
	}
	if(ISSET(interrupts, 1, 1)){
		if(I){
			cpsr &= ~(0x00000080);
		}
	}
	if(ISSET(interrupts, 2, 1)){
		if(A){
			cpsr &= ~(0x00000100);
		}	
	}
		

	


}

void CPSID(int interrupts){
	//printf("Chamou o CPSID\n");

	int A = ISSET(cpsr, 8, 1);
	int I = ISSET(cpsr, 7, 1);
	int F = ISSET(cpsr, 6, 1);

	if(ISSET(interrupts, 0, 1)){
		if(F){
			cpsr |= (0x00000040);
		}		
	}
	if(ISSET(interrupts, 1, 1)){
		if(I){
			cpsr |= (0x00000080);
		}
	}
	if(ISSET(interrupts, 2, 1)){
		if(A){
			cpsr |= (0x00000100);
		}	
	}

}

void BKPT(usInt immed){
	printf("Chamou o BKPT\n");
	printRegisters();

}

void STMIA(int *Ln, int List){
	//printf("Chamou o STMIA\n");
	celula cel;
	for (int i = 0; i < 8; ++i)
	{
		if (ISSET(List,i,1))
		{
			if(*Ln >= 0x80000000 && *Ln < 0x80000100){
				printf("\033[31m DATA ABORT \033[0;0m\n");
				return;
			}
			cel.address = *Ln;
			cel.value = r[i];
			insert_sorted(cel);
			*Ln += 4;
		}
	}

}

void LDMIA(int *Ln, int List){
	//printf("Chamou o LDMIA\n");
	for (int i = 0; i < 8; ++i)
	{
		if (ISSET(List,i,1))
		{
			if(*Ln >= 0x80000000 && *Ln < 0x80000100){
				printf("\033[31m DATA ABORT \033[0;0m\n");
				return;
			}
			r[i] = searchMemory(*Ln).value;
			*Ln += 4;
		}
	}

}

void B(int Offset, int cond){
	//printf("Chamou o B\n");

	if (ISSET32(Offset, 7,1))
	{
		Offset |= 0xffffff00;
	}else{
		Offset &= 0x000000ff;
	}

	if(cond == 0x0){
		if(ISSET32(cpsr, 30, 1)){     //EQ
			*pc = *pc + (Offset << 1);
			*pc += 2;
		}
	}else if(cond == 0x1){
		if(!(ISSET32(cpsr, 30, 1))){  //NE
			*pc = *pc + (Offset << 1);
			*pc += 2;
		}
	}else if(cond == 0x2){
		if(ISSET32(cpsr, 29, 1)){    //CS ou HS
			*pc = *pc + (Offset << 1);
			*pc += 2;
		}
	}else if(cond == 0x3){
		if(!(ISSET32(cpsr, 29, 1))){    //CC ou LO
			*pc = *pc + (Offset << 1);
			*pc += 2;
		}
	}else if(cond == 0x4){
		if(ISSET32(cpsr, 31, 1)){    //MI
			*pc = *pc + (Offset << 1);
			*pc += 2;
		}
	}else if(cond == 0x5){
		if(!(ISSET32(cpsr, 31, 1))){    //PL
			*pc = *pc + (Offset << 1);
			*pc += 2;
		}
	}else if(cond == 0x6){
		if(ISSET32(cpsr, 28, 1)){    //VS
			*pc = *pc + (Offset << 1);
			*pc += 2;
		}
	}else if(cond ==  0x7){
		if(!(ISSET32(cpsr, 28, 1))){    //VC
			*pc = *pc + (Offset << 1);
			*pc += 2;
		}
	}else if(cond == 0x8){
		if(!(ISSET32(cpsr, 30, 1)) && (ISSET32(cpsr, 29, 1))){    //HI
			*pc = *pc + (Offset << 1);
			*pc += 2;
		}
	}else if(cond == 0x9){
		if((ISSET32(cpsr, 30, 1)) || (!(ISSET32(cpsr, 29, 1)))){    //LS
			*pc = *pc + (Offset << 1);
			*pc += 2;
		}
	}else if(cond == 0xA){
		//GE
		if( ((ISSET32(cpsr, 31, 1)) && (ISSET32(cpsr, 28, 1))) || ((!ISSET32(cpsr, 31, 1)) && (!ISSET32(cpsr, 28, 1))) ) {
			*pc = *pc + (Offset << 1);
			*pc += 2;
		}
	}else if(cond == 0xB){
		//LT
		if( ((ISSET32(cpsr, 31, 1)) && (!(ISSET32(cpsr, 28, 1)))) || ((!ISSET32(cpsr, 31, 1)) && (ISSET32(cpsr, 28, 1))) ) {
			*pc = (*pc + (Offset << 1));
			*pc += 2;
		}
	}else if(cond == 0xC){
		//GT
		if( ((ISSET32(cpsr, 31, 1)) && (!ISSET32(cpsr, 30, 1)) && (ISSET32(cpsr, 28, 1))) || ((!ISSET32(cpsr, 31, 1)) &&  (!ISSET32(cpsr, 30, 1)) && (!ISSET32(cpsr, 28, 1))) ) {
			*pc = *pc + (Offset << 1);
			*pc += 2;
		}
	}else if(cond == 0xD){
		//LE
		if( ((ISSET32(cpsr, 30, 1))) || ((ISSET32(cpsr, 31, 1)) &&  (!ISSET32(cpsr, 28, 1))) || ((!ISSET32(cpsr, 31, 1)) && (ISSET32(cpsr, 28, 1)))  ) {
			*pc = *pc + (Offset << 1);
			*pc += 2;

		}
	}


}

void UNDEFINED(){
	printf("Chamou o UNDEFINED\n");
	printRegisters();
	exit(1);
}

void SWI(usInt immed){
	int aux, aux2;
	aux = ISSET32(cpsr, 6, 1);
	aux2 = ISSET32(cpsr, 7, 1);
	if(!(aux) || !(aux2)){
		printf("Chamou o SWI de numero %x\n", immed);	
	}
	

}

void B2(int Offset){
	//printf("Chamou o B2\n");

	if (ISSET32(Offset, 10,1))
	{
		Offset |= 0xfffff800;
	}else{
		Offset &= 0x000007ff;
	}

	*pc = (*pc + (Offset << 1));
	*pc += 2;

}

void BLX2(int Offset){
	//printf("Chamou o BLx2\n");
	//printf("aqui offest %x\n", Offset);
	*pc = ((*lr + (Offset << 1)  & 0xFFFFFFFF)) +  ((Offset-1) << 1);
	//printf("aqui: %x\n",*pc);
	//*lr = *pc | 1;
	//SETCLRFLAG(cpsr, 0, THUMB);

}

void B3(int Offset){
	//printf("Chamou o B3\n");
	/*for(int i = 11; i <= 31; i++){
		Offset |= ((Offset >> 10) & 1) << i; 
	}*/
	if (ISSET32(Offset, 10,1))
	{
		Offset |= 0xfffff800;
	}else{
		Offset &= 0x000007ff;
	}
	//int offset = Offset;
	*lr = (*pc+2) + (Offset << 12);

}

void BL(int Offset){
	//printf("Chamou o BL\n");

	*pc = (*lr + (Offset << 1));
	//printf("aaaa: %x\n", *lr);
	//*lr = (*pc) | 1;
	
}

celula searchMemory(unsigned int address){

	for (int i = 0; i != memoria.size(); ++i)
	{
		if(memoria[i].address==address){
			return memoria[i];
		}
	}
	celula a;
	a.address = address;
	a.value = 0;
	return a;

}

celula breakInstVal(char *linha){
	char addressC[8];
	char valueC[8];
	char *aux = linha;

	int i = 0;
	while(1){
		if(linha[i]==':'){
			break;
		}

		addressC[i] = linha[i];
		i++;

	}

	i+=2;

	for (int j = 0; j < 8; ++j)
	{
		valueC[j] = linha[i+j];
	}

	celula a;
	a.address = strtol(addressC, NULL, 16);
	a.address+= 0x80000000;

	a.value =  strtol(valueC, NULL, 16);

	return a;

}

void loadMemory(){
	FILE *arq;
	arq = fopen("programa.txt","rt");
	char linha[20];

	if (arq == NULL){
    	printf("ERRO! O arquivo não foi aberto! Crie o arquivo programa.txt\n");
    	exit(1);
	}
	int i = 0;
	while(!feof(arq)){
		fgets(linha, 20, arq);

		i++;
		if(i>64){
			cout << "ULTRAPASSOU A MEMÓRIA DE PROGRAMA\n";
			break;
		}
		insert_sorted(breakInstVal(linha));

	}

	fclose(arq);
}

void printRegisters(){
	printf("\n\n\n\n\nREGISTERS\n");
	for (int i = 0; i < 13; ++i)
	{
		printf("r%d = 0x%x = %d\n",i,r[i],r[i]);
	}
	printf("PC = 0x%x = %d\n", *pc, *pc);
	printf("LR = 0x%x = %d\n", *lr, *lr);
	printf("SP = 0x%x = %d\n", *sp, *sp);
	printf("\n\nCurrent Program Status Register: ");

	ISSET32(cpsr,31,1) ? printf("N") : printf("n");
	ISSET32(cpsr,30,1) ? printf("Z") : printf("z");
	ISSET32(cpsr,29,1) ? printf("C") : printf("c");
	ISSET32(cpsr,28,1) ? printf("V") : printf("v");
	printf("qjifT_");

	switch(ISSET32(cpsr,0,5)){
		case 0x10:
			printf("USR");
			break;
		case 0x11:
			printf("FIQ");
			break;
		case 0x12:
			printf("IRQ");
			break;
		case 0x13:
			printf("SVC");
			break;
		case 0x17:
			printf("ABT");
			break;
		case 0x1b:
			printf("UND");
			break;
		case 0x1f:
			printf("SYS");
			break;
	}

	printf("\n\nMEMÓRIA DE PROGRAMA\n");

	for (int i = 0; i != memoria.size(); ++i)
	{
		if(memoria[i].address >= 0x80000000 && memoria[i].address < 0x80000100){
			printf("0x%08x : 0x%08x\n", memoria[i].address, memoria[i].value);
		}

	}

	printf("\n\nMEMÓRIA DE DADOS\n");

	for (int i = 0; i != memoria.size(); ++i)
	{
		if((memoria[i].address >= 0x00000000 && memoria[i].address < 0x80000000) || (memoria[i].address >= 0x82000000 && memoria[i].address < 0xFFFFFFFF)){
			printf("0x%08x : 0x%08x\n", memoria[i].address, memoria[i].value);
		}

	}

	printf("\n\nPILHA\n");

	for (int i = 0; i != memoria.size(); ++i)
	{
		if(memoria[i].address < 0x82000000 && memoria[i].address >= *sp){
			printf("0x%08x : 0x%08x\n", memoria[i].address, memoria[i].value);
		}

	}

}

void init(){

	cpsr = 0x33; //THUMB - SVC
	for (int i = 0; i < 13; ++i)
	{
		r[i] = 0;
	}
}


void subFlags(int a, int b)
{
	unsigned int maior  = 0xFFFFFFFF;
	 int maior2 = 0x7FFFFFFF;

	b = ~(b);


	if(b>maior-1){
		//printf("Carry\n");
		SETBIT(cpsr, CARRYFLAG);
		b++;
	}else{
		b++;
		int maiorx = maior + b;
		if(a>maiorx){
			//printf("TESTE\n");
			SETBIT(cpsr, CARRYFLAG);
		}else{
			CLRBIT(cpsr, CARRYFLAG);
		}	
	}

	

	if(a > (maior2+b)){
		SETBIT(cpsr, OVERFLOWFLAG);
	}else{
		CLRBIT(cpsr, OVERFLOWFLAG);
	}

}


void addFlags(int a, int b)
{
	unsigned int maior  = 0xFFFFFFFF;
	int maior2 = 0x7FFFFFFF;

	if(a > (maior-b)){
		SETBIT(cpsr, CARRYFLAG);
	}else{
		CLRBIT(cpsr, CARRYFLAG);
	}

	if(a > (maior2-b)){
		SETBIT(cpsr, OVERFLOWFLAG);
	}else{
		CLRBIT(cpsr, OVERFLOWFLAG);
	}

}
